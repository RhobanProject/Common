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

#include <timing/TickMachine.h>
#include <logging/log.h>
#include <sockets/TCPServer.h>

#include "communicationCommon.h"
#include "ServerComponent.h"
#include "ThreadedClient.h"

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
    class ServerComponentInterface;

    class Server : public TCPServer<ServerInternalClient>
    {
        public:
            Server();

            /**
             * Creating a client
             */
            ServerInternalClient *createClient();

            /**
             * Registering a component to the server
             */
            void registerComponent(int id, ServerComponent *component);

            /**
             * Gets the server component
             */
            ServerComponent *getComponent(int id);

        protected:
            /**
             * Maps the components id to the server components
             */
            map<int, ServerComponent *> components;
    };

    /**
     * The core server component is the component used to talk with the server
     * system
     */
    class CoreServerComponent: public ServerComponent
    {
        public:
            /**
             * Used to handle incoming messages whose target is the server
             * itself
             * */
            Message *call(Message * msg_in, Message * msg_out);
    };

    /**
     * This is an internal client created by the server each time some external client is connecting
     */
    class ServerInternalClient : public TCPServerClient, public Client
    {
        protected:
            /**
             * The internal client process the message
             */
            void processMessage(Message * msg);

            /**
             * The clients main loop
             */
            void loop();
            
            Server *server;

        public:
            ServerInternalClient(Server * server_);
            ServerInternalClient();
    };
}

#endif // SERVER_H
