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
        launcher->registerComponent(new CoreServerComponent(this));
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
        ServerComponent *component = launcher->getComponent(client->DestinationID());

        if (component == client) {
            launcher->removeComponent(client->DestinationID());
        }
    }

    /**
     * Called when a client should be created
     */
    ServerInternalClient *Server::createClient()
    {
        ServerInternalClient *client = new ServerInternalClient(launcher, nextClientId++);

        // Registering the client as a component
        launcher->registerComponent(client);

        return client;
    }

    CoreServerComponent::CoreServerComponent(Server *server_) : server(server_)
    {
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

                case MSG_SERVER_REGISTER_COMPONENT:
                    {
                        ui16 type = msg_in->read_uint();
                        ServerComponent *component = server->launcher->getComponent(type);
                        ServerInternalClient *client = server->getClient(msg_in->source);

                        if (!client) {
                            string err = "Can't find the client " + msg_in->source;
                            SERVER_CAUTION(err);
                            throw err;
                        }

                        if (component != NULL) {
                            ServerInternalClient *client = dynamic_cast<ServerInternalClient*>(component);

                            if (!client) { // If the component is not a client, we can't remplace it
                                string err = "Can't replace the component " + my_itoa(type);
                                SERVER_CAUTION(err);
                                throw err;
                            } else { // Removing the client from the hub
                                SERVER_MSG("Component " << type << " is already connected, removing it to replace by a new client");
                                server->launcher->removeComponent(type);
                                client->stop();
                            }
                        }

                        // Registering a component 
                        SERVER_MSG("Registering new component " << type);
                        server->launcher->registerComponent(client);

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
    ServerInternalClient::ServerInternalClient(Callable *hub_, int clientId_) : Client(hub_), clientId(clientId_)
    {
        SERVER_DEBUG("Threaded client "<< (intptr_t) this << " created ");
    }

    ServerInternalClient::ServerInternalClient() : Client(NULL)
    {
        SERVER_CAUTION("The internal client should ne be instanciated without parameters");
    }
            
    Message *ServerInternalClient::process(Message * msg_in, Message * msg_out)
    {
        sendMessage(msg_in);
        return NULL;
    }

    void ServerInternalClient::loop()
    {
        readAndProcess();
    }

    ServerHub::ServerHub(bool thread_safe) : thread_safe(thread_safe)
    {
    }

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
        ServerComponent *component;
        ui16 comp_nb = msg_in->destination;

    	BEGIN_SAFE(mutex);
        component = getComponent(comp_nb);

        if (!component) {
            SERVER_CAUTION("Unable to find the component " << comp_nb);
        }
    	END_SAFE(mutex);

    	if (!component) {
    		cout << endl << "Null component: " << comp_nb << endl;
    		exit(0);
    	}

    	Message * answer = 0;
    	if (thread_safe) {
    		Mutex * mut = mutexes[comp_nb];
       		BEGIN_SAFE((*mut))
    		answer= component->call(msg_in, msg_out);
    		END_SAFE((*mut))
    	} else {
    		answer = component->call(msg_in, msg_out);
    	}

    	return answer;
    }
    
    /**
     * Registering a new component
     */
    void ServerHub::registerComponent(ServerComponent *component)
    {
        BEGIN_SAFE(mutex);
        SERVER_MSG("Registering component " << component->DestinationID());
        components[component->DestinationID()] = component;
        mutexes[component->DestinationID()] = new Mutex();
        component->setHub(this);
        END_SAFE(mutex);
    }
    
    /**
     * Registering a new internal component
     */
    ServerComponent *ServerHub::getComponent(ui16 type)
    {
        map<ui16, Callable *>::iterator it = components.find(type);

        if (it != components.end()) {
            return dynamic_cast<ServerComponent *>((*it).second);
        }

        return NULL;
    }
    
    /**
     * Registering a new internal component
     */
    void ServerHub::removeComponent(ui16 type)
    {
        SERVER_MSG("Unregistering component " << type);
        BEGIN_SAFE(mutex);
        components.erase(type);
        mutexes.erase(type);
    	END_SAFE(mutex);
    }
    
    /**
     * Returns the registered components
     */
    vector<ServerComponent *> ServerHub::getComponents()
    {
        vector<ServerComponent *> componentsVector;
        map<ui16, Callable *>::iterator it;

        for (it = components.begin(); it != components.end(); it++) {
            ServerComponent *component = dynamic_cast<ServerComponent *>((*it).second);

            if (component) {
                componentsVector.push_back(component);
            }
        }

        return componentsVector;
    }
}
