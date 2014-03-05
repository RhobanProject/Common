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
#include "ServerHub.h"
#include "NetworkComponent.h"

using namespace std;
using namespace Rhoban;

#define MAX_CLIENT 32


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

            /**
             * Sets the robot name hosted in this server
             */
            void setName(string name);

            /**
             * Gets the robot name
             */
            string getName();
    
            void cleanClient(ServerInternalClient *client);

        protected:
            string name;
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

}

#endif // SERVER_H
