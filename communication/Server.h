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

#define SERVER_CAUTION(...)     LOG_CPP(1 <= SERVER_LOG_LEVEL, "server:caution", __VA_ARGS__)
#define SERVER_MSG(...)         LOG_CPP(2 <= SERVER_LOG_LEVEL, "server", __VA_ARGS__)
#define SERVER_DEBUG(...)       LOG_CPP(3 <= SERVER_LOG_LEVEL, "server:debug", __VA_ARGS__)

namespace Rhoban
{
    class ServerInternalClient;
    class ServerComponentInterface;

    class Server: public ServerComponent
    {
        public:

            static void launch_server(ServerComponentInterface *launcher, int port);

            static void shutdown_server(void);

            static Server * get_server();

            //used to handle incoming messages whose target is the server
            //itself
            Message * call(Message * msg_in, Message * msg_out);

        protected:

            static Server * the_server;

            //creates a server that will run the corrresponding interface
            Server();
            virtual ~Server();

            void internal_launch_server(ServerComponentInterface * launcher_,
                    int port);
            void create_listen_socket();
            void close_listen_socket();
            void internal_shutdown_server(void);

            //the list of internal clients talking to external clients
            list<ServerInternalClient *> clients;

            //contains the interface given to internal clients to
            // handle requests upon message reception
            ServerComponentInterface * launcher;

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
    typedef Message * (*ComponentBehaviour)(//
            Message * msg_in,// the incoming message
            Message * msg_out//a place to store the outgoing message
            );

    class ServerInternalClient: public ThreadedClient
    {
        protected:
            ServerComponentInterface * launcher;

            //the map associates subcommands to behaviours
            //it is initialized by init_cals
            //it avids to use of switches
            map<ui32, ComponentBehaviour> calls;
            void init_calls();

            //this is how the internal client process an incoming message
            //the message is forwarded to the corresponding component
            //and the answer of the component is sent back to the client
            void process_message(Message * msg);

        public:
            ServerInternalClient(ServerComponentInterface * launcher, SOCKET sock);

        private:
            void execute();
    };

    /*!
     *  this is an interface given by the server to its internal clients
     *  so they handle requests
     *
     *  Basically this is a collection of component behaviours
     */
    class ServerComponentInterface
    {
        public:

            //creates the component interface
            ServerComponentInterface(bool thread_safe = true);

            //should be called after construction to set the server component
            //void set_component(int comp_nb, ServerComponent * comp)

            virtual ~ServerComponentInterface();

            /*!
             * threaded components can be called
             * they eat a msg_in,
             * do whatever they have to do
             * and output an answer msg
             */
            Message * call(Message * msg_in);

            /*!
             * if a msg_out is given,
             * they the component can use it
             * to place the answer message
             *
             * this saves some ressources:
             * memory allocation
             * can be done once for all by the caller (the server),
             * which is efficient in case of frequent and/or large messages
             *
             */
            Message * call(Message * msg_in, Message * msg_out);

        protected:

            //creates components dynamically
            //this should be instantiated by derived classes
            virtual ServerComponent * create_component(int comp_type) = 0;

            //components created by the server
            map<ui32, ServerComponent*> components;

            bool thread_safe;
    };
}

#endif // SERVER_H
