/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <map>
#include <set>

#include <timing/TickMachine.h>
#include <logging/log.h>
#include <sockets/TCPServer.h>

#include "communicationCommon.h"
#include "ServerComponent.h"
#include "NetworkComponent.h"
#include "Client.h"
#include "Callable.h"

using namespace std;
using namespace Rhoban;

#define MAX_CLIENT 32

/**
 * Server log level
 * 1: Caution
 * 2: Messages
 * 3: Debug
 */
#define SERVER_LOG_LEVEL 2

#define SERVER_CAUTION(...)     LOG_CPP(1, SERVER_LOG_LEVEL, "server:caution", __VA_ARGS__)
#define SERVER_MSG(...)         LOG_CPP(2, SERVER_LOG_LEVEL, "server", __VA_ARGS__)
#define SERVER_DEBUG(...)       LOG_CPP(3, SERVER_LOG_LEVEL, "server:debug", __VA_ARGS__)

namespace Rhoban
{
    class ServerComponent;
    class ServerInternalClient;
    class ServerHub;

    class Server : public TCPServer<ServerInternalClient>
    {
        public:
            Server(ServerHub *launcher);

            /**
             * Creating a client
             */
            ServerInternalClient *createClient();
            
            /**
             * Contains the interface given to the internal clients to handle
             * the requests
             */
            ServerHub * launcher;

            /**
             * Gets a client by ID
             */
            ServerInternalClient *getClient(ui16 id);
    
            void cleanClient(ServerInternalClient *client);

        protected:
            int nextClientId;
    };

    /**
     * The core server component is the component used to talk with the server
     * system
     */
    class CoreServerComponent: public ServerComponent
    {
        public:
            CoreServerComponent(Server *server_);
            Server *server;

            /**
             * Used to handle incoming messages whose target is the server
             * itself
             * */
            Message *process(Message * msg_in, Message * msg_out, bool sync = false, int timeout = 0);
            
            const ui16 virtual DestinationID() const { return MSG_TYPE_SERVER; }
    };

    /**
     * This is an internal client created by the server each time some external client is connecting
     */
    class ServerInternalClient : public TCPServerClient, public NetworkComponent
    {
        public:
            ServerInternalClient(Callable *hub, int clientId);
            ServerInternalClient();

            void execute();
            void loop();

            void processMailboxMessage(Message *message);

            /**
             * Implementation of ServerComponent
             */
            const ui16 virtual DestinationID() const { return clientId; }

            void addId(ui16 id);
            void removeId(ui16 id);

            bool isConnected();

            bool respondTo(ui16 type);

        protected:
            set<ui16> ids;
            int clientId;
    };

    /**
     *  The ServerHub encapsulates all components of the server
     *  and routes requests to the right components,
     *  and create the component if needed
	 *
     *  it is given by the server to its internal clients
     *  so they handle requests
     */
    class ServerHub : public Callable
    {
        public:
            /**
             * Creates a thread save server
             */
            ServerHub(bool thread_safe = true);

            /**
             * Delete the server hub
             */
            virtual ~ServerHub();

            /**
             * Registering a component to the server
             */
            void registerComponent(ServerComponent *component);

            /**
             * Registering a component by giving the id
             */
            void registerComponent(ui16 type, ServerComponent *component);

            /**
             * Removing a component
             */
            void removeComponent(ServerComponent *component);

            /**
             * Retreive a component
             */
            ServerComponent *getComponent(ui16 type);

            /*
             * Whether a component has been registered
             */
            bool hasComponent(ui16 type){ return getComponent(type) != fallbackComponent; };

            /**
             * Removes the component from the hub
             */
            void removeComponent(ui16 type);

            /**
             * Run a call
             */
            Message *doCall(Message *msg_in, Message *msg_out, bool sync, int timeout);
            Message *call(Message * msg_in, Message * msg_out);
            Message *callSync(Message * msg_in, Message * msg_out, int timeout);

            /**
             * Returns the components
             */
            vector<ServerComponent *> getComponents();

            /**
             * Component to use when no other one exists
             */
            ServerComponent *fallbackComponent;

        protected:
            /**
             * The internal client process the message
             */
            void processMessage(Message * msg);

            /**
             * Components
             */
            map<ui16, Callable *> components;
            map<ui16, Mutex *> mutexes;

            bool thread_safe;
            Mutex components_mutex;
    };
}

#endif // SERVER_H
