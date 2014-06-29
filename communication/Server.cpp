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
    	SERVER_DEBUG("Server registering itself as a component")
        launcher->registerComponent(MSG_TYPE_SERVER, new CoreServerComponent(this));
    	SERVER_DEBUG("Server registered itself as a component")
    }

    void Server::setName(string name_)
    {
        name = name_;
    }

    string Server::getName()
    {
        return name;
    }

    /**
     * Get a client by id
     */
    ServerInternalClient *Server::getClient(ui16 id)
    {
        vector<ServerInternalClient *>::iterator it;

        for (it = clients.begin(); it != clients.end(); it++) {
            ServerInternalClient *client = (*it);

            if (client->DestinationID() == id) {
                return client;
            }
        }

        return NULL;
    }

    /**
     * Removing the client
     */
    void Server::cleanClient(ServerInternalClient *client)
    {
        launcher->removeComponent(client);
    }

    /**
     * Called when a client should be created
     */
    ServerInternalClient *Server::createClient()
    {
    	SERVER_DEBUG("Server creating client");
        ServerInternalClient *client = new ServerInternalClient(launcher, nextClientId++);
    	SERVER_DEBUG("Server created client");

        // Registering the client as a component
        launcher->registerComponent(client->DestinationID(), client);

        return client;
    }

    CoreServerComponent::CoreServerComponent(Server *server_) : server(server_)
    {
    }

    /**
     * Core server call
     */
    Message * CoreServerComponent::process(Message * msg_in, Message * msg, bool sync, int timeout)
    {
    	SERVER_DEBUG("Server core component processing message");
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

                case MSG_SERVER_REGISTER_COMPONENT:
                    {
                        ui16 type = msg_in->read_uint();
                        Callable *component = server->launcher->getComponent(type);
                        ServerInternalClient *client = server->getClient(msg_in->source);

                        if (!client) {
                            string err = "Can't find the client " + msg_in->source;
                            SERVER_CAUTION(err);
                            throw err;
                        }

                        if (component != NULL) {
                            ServerInternalClient *oldClient = dynamic_cast<ServerInternalClient*>(component);

                            if (!oldClient) { // If the component is not a client, we can't remplace it
                                string err = "Can't replace the component " + my_itoa(type);
                                SERVER_CAUTION(err);
                                throw err;
                            } else { // Removing the client from the hub
                                SERVER_MSG("Component " << type << " is already connected, removing it to replace by a new client");
                                server->launcher->removeComponent(type);
                                oldClient->removeId(type);
                            }
                        }

                        // Registering a component 
                        //SERVER_MSG("Registering new component " << type);

                        client->addId(type);
                        server->launcher->registerComponent(type, client);

				
                        msg->destination = type;
                        msg->append(string("Registered component "));
                        SERVER_MSG("Registered new component " << type);

                        break;
                    }
                case MSG_SERVER_GET_NAME:
                    msg->append(server->getName());
                    break;

                default:
                    {
                        break;
                    }
            }
        }
        catch (string str)
        {
            throw string("Failed to call server: " + str);
        }
        catch (...)
        {
            throw string("failed to call server.");
        }
        return msg;
    }

    /**
     * Creating a server client
     */
	ServerInternalClient::ServerInternalClient(Callable *hub_, int clientId_) : NetworkComponent(hub_), clientId(clientId_)
    {
        SERVER_DEBUG("Threaded client "<< (intptr_t) this << " created ");
    }

    bool ServerInternalClient::isConnected()
    {
        return !dead;
    }

	ServerInternalClient::ServerInternalClient() : NetworkComponent(NULL)
    {
        SERVER_CAUTION("The internal client should ne be instanciated without parameters");
    }

    void ServerInternalClient::processMailboxMessage(Message *msg)
    {
        msg->source = clientId;
        this->NetworkComponent::processMessage(msg);
    }
            
    void ServerInternalClient::addId(ui16 id)
    {
        ids.insert(id);
    }

    void ServerInternalClient::removeId(ui16 id)
    {
        ids.erase(id);
    }

    void ServerInternalClient::execute()
    {
        this->TCPServerClient::execute();
    }

    void ServerInternalClient::loop()
    {
        this->NetworkComponent::execute();
    }

    bool ServerInternalClient::respondTo(ui16 type)
    {
        return type==clientId || (ids.find(type) != ids.end());
    }

}
