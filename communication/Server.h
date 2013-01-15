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

#include <timing/TickMachine.h>
#include <logging/log.h>

#include "communicationCommon.h"
#include "ServerComponent.h"
#include "Client.h"

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
    class ServerInternalClient;
    class ServerHub;

    class Server: public ServerComponent
    {
        public:
            //used to handle incoming messages whose target is the server
            //itself
            Message * process(Message * msg_in, Message * msg_out);
            
        	/* The destination id of this component */
        	const ui16 DestinationID() const { return MSG_TYPE_SERVER; }


            Server(){};
            virtual ~Server();
            void launch(ServerHub * launcher_, int port);
            void shutdown();

        protected:

            //creates a server that will run the corrresponding interface

            void create_listen_socket();
            void close_listen_socket();

            //the list of internal clients talking to external clients
            list<ServerInternalClient *> clients;

            //contains the interface given to internal clients to
            // handle requests upon message reception
            ServerHub * launcher;

            int port;
            bool connected;

#ifdef WIN32
            SOCKET sock;
#else
            int sock;
#endif
            bool run;
    };

    /*!
     * This is an internal client created by the server each time some external client is connecting
     */

    class ServerInternalClient: public Client, public Thread
    {
        protected:
    	/*
    	 * This is used by the client to process messages
    	 */
            Callable * hub;

            //this is how the internal client process an incoming message
            //the message is forwarded to the corresponding component
            //and the answer of the component is sent back to the client
            void process_message(Message * msg);

        public:
            ServerInternalClient(Callable * hub, SOCKET sock);

        private:
            void execute();
    };

    /*!
     *  The serverhub encapsulates all components of the server
     *  and routes requests to the right components,
     *  and create the component if needed
	 *
     *  it is given by the server to its internal clients
     *  so they handle requests
     */
    class ServerHub : public Callable
    {
        public:

            //creates the component interface
            //thread safety guarantees that two clients wont access the same component at the same time,
            //but they may access different components in parallel
            ServerHub(bool thread_safe = true);

            //should be called after construction to set the server component
            //void set_component(int comp_nb, ServerComponent * comp)

            virtual ~ServerHub();


            Message * call(Message * msg_in, Message * msg_out);

            void setServer(Server * server);

        protected:

            //creates components dynamically
            //this should be instantiated by derived classes
            virtual ServerComponent * create_component(int comp_type) = 0;

            //components created by the server
            map<ui16, Callable *> components;
            map<ui16, Mutex *> mutexes;

            bool thread_safe;
            Mutex mutex;
    };
}

#endif // SERVER_H
