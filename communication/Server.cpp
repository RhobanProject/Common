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
    /**
     * Messages addressed to the server should be forwarded to the Core component
     */
    Server::Server(ServerHub *launcher_) : launcher(launcher_), nextClientId(1000)
    {
        launcher->registerComponent(new CoreServerComponent());
    }

    /**
     * Called when a client should be created
     */
    ServerInternalClient *Server::createClient()
    {
        return new ServerInternalClient(launcher, nextClientId++);
    }

    /**
     * Core server call
     */
    Message * CoreServerComponent::process(Message * msg_in, Message * msg)
    {
        try
        {
            switch (msg_in->command)
            {
                case MSG_SERVER_GET_VERSION:
                    {
                        //TODO get a real version number
                        msg->source = MSG_TYPE_SERVER;
                        msg->destination = msg_in->source;
                        msg->command = MSG_SERVER_GET_VERSION;
                        msg->append(1);
                        break;
                    }
                case MSG_SERVER_ECHO:
                    {
                        try
                        {
                            cout << "Echoing message of size " << msg_in->length  << endl;
                            msg->source = MSG_TYPE_SERVER;
                            msg->destination = msg_in->source;
                            msg->command = MSG_SERVER_ECHO;
                            msg->append(msg_in->read_string());
                            msg->append(msg_in->read_string());
                        }
                        catch(string exc)
                        {
                            cout << "Exception when echoing " << exc << endl;
                            msg->append(exc);
                            msg->append("");
                        }

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

    /**
     * Creating a server client
     */
    ServerInternalClient::ServerInternalClient(Callable *hub_, int clientId_) : hub(hub_)
    {
        SERVER_DEBUG("Threaded client "<< (intptr_t) this << " created ");
    }

    ServerInternalClient::ServerInternalClient()
    {
        SERVER_CAUTION("The internal client should ne be instanciated without parameters");
    }

    /**
     * Process a message
     */
    void ServerInternalClient::processMessage(Message * msg)
    {
        if (!msg) {
            throw string("Cannot process null message");
        }

        Message * msg_out = outMessages[msg->destination];
        msg_out->clear();
        msg_out->uid = msg->uid;
        msg_out->source = msg->destination;
        msg_out->destination = msg->source;
        msg_out->command = msg->command;


        // Calling the component on the given message
        SERVER_DEBUG("InternalClient ("<<this<<") <-- message l" << msg->length << " t"<< msg->destination << " st"<< msg->command << "("<<msg->uid<<") <-- remote ");
        
        try
        {
            Message *answer = hub->call(msg, msg_out);

            // If the component answered
            if (answer) {
                    SERVER_DEBUG("InternalClient ("<<this<<") --> message l" << msg->length << " t"<< msg->destination << " st"<< msg->command << "("<<msg->uid<<") --> remote ");
                sendMessage(answer);
            }
        } catch (string exc) {
            ostringstream smsg;
            smsg << "Failed to process message " << msg->uid << ": " << exc;
            SERVER_DEBUG(smsg.str());
            msg_out->command = MSG_ERROR_COMMAND;
            msg_out->append(smsg.str());
            sendMessage(msg_out);
        } catch (...)
        {
            ostringstream smsg;
            smsg << "Exception with message destination " << msg->destination << " command "
                << msg->command << " length " << msg->length << " uid "
                << msg->uid << ".";
            SERVER_DEBUG(smsg.str());
            msg_out->command = MSG_ERROR_COMMAND;
            msg_out->append(smsg.str());
            sendMessage(msg_out);
        }
    }

#define FD_ZERO2(p)     memset((p), 0, sizeof(*(p)))

    void ServerInternalClient::loop()
    {
        try
        {
            while (!dead)
            {
                Message *msg = readOneMessage(60000);

                if (msg) {
                    processMessage(msg);
                }
            }
        } catch (string exc)
        {
            SERVER_CAUTION("ServerInternalclient " << this << " exception "<< exc);
        } catch (...)
        {
            SERVER_CAUTION("ServerInternalclient " << this << " exception");
        }
    }

    ServerHub::ServerHub(bool thread_safe) : thread_safe(thread_safe)
    {
    }

    /*
       void ServerComponentInterface::set_component(int comp_nb, ServerComponent * comp)
       {
       components[comp_nb] = comp;
       }*/

    ServerHub::~ServerHub()
    {
    	BEGIN_SAFE(mutex)
    	for(map<ui16, Callable *>::iterator it = components.begin(); it != components.end(); it++)
    		if(it->first != MSG_TYPE_SERVER && it->second)
    		{
    			delete it->second;
    			it->second = 0;
    		}
    	END_SAFE(mutex)
    }

    Message* ServerHub::call(Message* msg_in, Message * msg_out)
    {
        ui16 comp_nb = msg_in->destination;

        Callable * comp = 0;

    	BEGIN_SAFE(mutex);

        map<ui16, Callable *>::iterator it = components.find(comp_nb);

        if (it == components.end() || it->second == NULL)
        {
            SERVER_CAUTION("Unable to find the component " << comp_nb);
    		throw string("Null component for destination: ") + my_itoa(comp_nb);
        } else {
            comp = it->second;
        }

    	END_SAFE(mutex);


    	Message * answer = 0;
    	if(thread_safe)
    	{
    		Mutex * mut = mutexes[comp_nb];
       		BEGIN_SAFE((*mut))
    		answer= comp->call(msg_in, msg_out);
    		END_SAFE((*mut))
    	}
    	else
    	{
    		answer = comp->call(msg_in, msg_out);
    	}
    	return answer;
    }
    
    /**
     * Registering a new internal component
     */
    void ServerHub::registerComponent(ServerComponent *component)
    {
        components[component->DestinationID()] = component;
        mutexes[component->DestinationID()] = new Mutex();
        component->setHub(this);
    }
}
