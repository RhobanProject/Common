/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <iostream>

#include <string.h>
#include <set>

#include <xml/XMLTools.h>
#include <util.h>

#include "rhobanProtocol.h"
#include "Server.h"
#include "Message.h"

namespace Rhoban
{
    Server::Server()
    {
    }

    SOCKADDR_IN sinserv; //parametres pour server
    int sinsize;

    void Server::close_listen_socket()
    {
        if (sock)
            close(sock);
        sock = 0;
        connected = false;
    }

    void Server::create_listen_socket()
    {

        //SOCKADDR_IN sin; //parametres pour sock

        memset(&sinserv, 0, sizeof(sinserv));
        sinserv.sin_family = AF_INET;
        sinserv.sin_addr.s_addr = INADDR_ANY;
        sinserv.sin_port = htons(this->port);
        sock = socket(AF_INET, SOCK_STREAM, 0);

#ifdef WIN32
        if(sock==INVALID_SOCKET)
#else
            if (sock == -1)
#endif
            {
                perror("socket()\n");
                throw string("Failed to create socket.");
            }
        SERVER_DEBUG("Created socket");

        /* ignore "socket already in use" errors */
#ifdef _WIN32
        char on = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
            throw string("Failed to set socket options.");
#else
        int on = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
            throw string("Failed to set socket options.");
#endif
        SERVER_DEBUG("Set socket options");

        if (bind(sock, (SOCKADDR*) &sinserv, sizeof(sinserv)))
        {
            perror("bind()\n");
            throw string("Failed to bind socket.");
        }
        SERVER_DEBUG("Binded socket");

        //wait for incoming connection
        if (listen(sock, MAX_CLIENT) == SOCKET_ERROR)
        {
            perror("listen()\n");
            throw string("Failed to listen incoming connections.");
        }
        SERVER_DEBUG("Listening incoming connections");

        sinsize = sizeof(sinserv);
        connected = true;
    }

    void Server::launch(ServerComponentInterface * launcher_, int port)
    {
        if (launcher_ == NULL) {
            throw string("Null interface");
        }

        launcher_->server = this;

#ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2,0),&wsa);
#else
        signal(SIGPIPE, SIG_IGN);
#endif

        launcher = launcher_;
        clients.clear();
        run = 1;
        connected = false;
        SERVER_DEBUG("Start server port " << port);

        this->port = port;

        TickTimer timer(1);

        while (run == 1)
        {
            if (!connected)
            {
                SERVER_MSG("Server creating listen socket");
                try
                {
                    create_listen_socket();
                } catch (string exc)
                {
                    SERVER_CAUTION("Server failed to create listen socket " << exc);
                }
            }
            if (connected)
            {
                SERVER_DEBUG("Server waiting client.");
#ifdef _WIN32
                SOCKET sock_wait_client = accept(sock,(SOCKADDR*)&sinserv, &sinsize);
#else
                SOCKET sock_wait_client = accept(sock, (SOCKADDR*)&sinserv,
                        (socklen_t*) &sinsize);
#endif
                SERVER_DEBUG("Server accepted connection.");
                if (sock_wait_client != INVALID_SOCKET)
                {
                    try
                    {
                        ServerInternalClient * client = new ServerInternalClient(
                                launcher, sock_wait_client);
                        clients.push_back(client);
                        client->start(NULL);
                        client->resume_thread();
                        SERVER_MSG("New client " << (long) client);
                    } catch (string exc)
                    {
                        SERVER_CAUTION("Error while running client: ");
                    } catch (...)
                    {
                        SERVER_CAUTION("Error while running client, disconnecting.");
                    }
                } else
                {
                    close_listen_socket();
                }
                //waits one sec
            }
            timer.wait_next_tick();

            //erase dead client, one at a time

            for (list<ServerInternalClient *>::iterator pclient = clients.begin(); pclient
                    != clients.end(); pclient++)
            {
                ServerInternalClient * client = *pclient;
                if (client->is_dead())
                {
                    SERVER_MSG("Erasing client " << (long) client);
                    clients.erase(pclient);
                    delete client;
                    break;
                }
            }
        }

        shutdown();
    }

    void Server::shutdown()
    {
        if (run)
        {
            run = 0;
            close_listen_socket();
            while (clients.size() > 0)
            {
                ServerInternalClient * client = clients.back();
                SERVER_MSG("Shutting down client " << (long) client << "...");
                client->shutdown();
                delete client;
                clients.pop_back();
                SERVER_MSG(" done");
            }
#ifdef _WIN32
            WSACleanup();
#endif
        }
    }

    Server::~Server()
    {
        if (launcher)
            delete (launcher);
    }

    Message * Server::call(Message * msg_in, Message * msg)
    {
        try
        {
            switch (msg_in->command)
            {
                case MSG_SERVER_GET_VERSION:
                    {
                        //TODO get a real version number
                        msg->append(1);
                        break;
                    }
                case MSG_SERVER_ECHO:
                    {
                        msg->destination = MSG_TYPE_SERVER;
                        msg->command = MSG_SERVER_ECHO;
                        msg->append(msg_in->read_string());
                        return msg;
                        break;
                    }

                case MSG_SERVER_XML_TO_FILE:
                    {
                        string filename = msg_in->read_string();
                        string stream = msg_in->read_string();
                        SERVER_MSG("Server writing stream " << endl << stream << endl
                                << " to file " << filename);
                        XMLTools::stream_to_file(filename, stream);
                        msg->append(string("Xml stream wrote to file ") + filename);
                        break;
                    }

                default:
                    {
                        break;
                    }
            }
        } catch (string str)
        {
            throw string("Failed to call server: " + str);
        } catch (...)
        {
            throw string("failed to call server.");
        }
        return msg;
    }

    ServerInternalClient::ServerInternalClient(
            ServerComponentInterface * launcher_, SOCKET sock_)
    {
        launcher = launcher_;
        sock = sock_;

        SERVER_DEBUG("Threaded client "<< (intptr_t) this << " created ");
    }

    void ServerInternalClient::process_message(Message * msg)
    {
        if (!msg)
            throw string("Cannot process null message");

        Message * msg_out = out_msgs[msg->destination];
        msg_out->clear();
        msg_out->uid = msg->uid;
        msg_out->destination = msg->destination;
        msg_out->command = msg->command;

        SERVER_DEBUG("InternalClient ("<<this<<") <-- message l" << msg->length << " t"<< msg->destination << " st"<< msg->command << "("<<msg->uid<<") <-- remote "<< sock);

        try
        {
            SERVER_DEBUG("InternalClient ("<<this<<") --> message l" << msg->length << " t"<< msg->destination << " st"<< msg->command << "("<<msg->uid<<") --> Component ");
            Message * answer = launcher->call(msg, msg_out);
            SERVER_DEBUG("InternalClient ("<<this<<") <-- message l" << msg->length << " t"<< msg->destination << " st"<< msg->command << "("<<msg->uid<<") <-- Component ");
            if (answer)
            {
                SERVER_DEBUG("InternalClient ("<<this<<") --> message l" << msg->length << " t"<< msg->destination << " st"<< msg->command << "("<<msg->uid<<") --> remote "<<sock);
                send_msg(answer);
            } else
                SERVER_DEBUG("No answer from client");
        } catch (string exc)
        {
            ostringstream smsg;
            //smsg << "Exception with message destination "<<msg->destination<<" command "<<msg->command<<" length "<<msg->length<<" uid "<<msg->uid<<":'";
            //smsg << exc<<"'.";
            smsg << "Failed to process message " << msg->uid << ": " << exc;
            SERVER_DEBUG(smsg.str());

            msg_out->destination = msg->destination;
            msg_out->command = MSG_ERROR_COMMAND;
            msg_out->append(smsg.str());
            send_msg(msg_out);
        } catch (...)
        {
            ostringstream smsg;
            smsg << "Exception with message destination " << msg->destination << " command "
                << msg->command << " length " << msg->length << " uid "
                << msg->uid << ".";
            SERVER_DEBUG(smsg.str());

            msg_out->destination = msg->destination;
            msg_out->command = MSG_ERROR_COMMAND;
            msg_out->append(smsg.str());
            send_msg(msg_out);
        }
    }

#define FD_ZERO2(p)     memset((p), 0, sizeof(*(p)))

    void ServerInternalClient::execute(void)
    {
        connected = true;
        try
        {
            while (connected)
            {
                Message * msg = read_one_msg(60000);

                if (msg)
                    process_message(msg);

            }
        } catch (string exc)
        {
            SERVER_CAUTION("ServerInternalclient " << this << " exception "<< exc);
        } catch (...)
        {
            SERVER_CAUTION("ServerInternalclient " << this << " exception");
        }
        shutdown();
    }

    ServerComponentInterface::ServerComponentInterface(bool thread_safe)
    {
        components.clear();
        this->thread_safe = thread_safe;
    }

    /*
       void ServerComponentInterface::set_component(int comp_nb, ServerComponent * comp)
       {
       components[comp_nb] = comp;
       }*/

    ServerComponentInterface::~ServerComponentInterface()
    {
        set<ServerComponent *> comps;

        for (int i = 0; i < MSG_TYPE_MAX_NB; i++)
        {
            // server is delete by the main !
            //taking care of duplicates
            if (components[i] && (i != MSG_TYPE_SERVER))
            {
                comps.insert(components[i]);
                components[i] = 0;
            }
        }

        for (set<ServerComponent *>::iterator comp = comps.begin(); comp
                != comps.end(); comp++)
            delete (*comp);
    }
    ;

    //it is up to the caller to dispose of the return msg
    Message* ServerComponentInterface::call(Message* msg_in)
    {
        return call(msg_in, new Message());
    }

    Message* ServerComponentInterface::call(Message* msg_in, Message * msg_out)
    {
        int comp_nb = msg_in->destination;

        ServerComponent * comp = 0;

        if (!components.count(comp_nb))
        {
            SERVER_DEBUG("Creating component "<< comp_nb);
            try
            {
                comp = create_component(comp_nb);
                components[comp_nb] = comp;
            } catch (const string & exc)
            {
                string msg = "Could not create component : " + exc;
                SERVER_CAUTION(msg);
                components[comp_nb] = 0;
                msg_out->append(msg);
                return msg_out;
            }
        } else
            comp = components.find(comp_nb)->second;

        if (comp)
            return thread_safe ? comp->safe_call(msg_in, msg_out) : comp->call(
                    msg_in, msg_out);
        else
            throw string("Failed to create component " + my_itoa(comp_nb));
    }
}
