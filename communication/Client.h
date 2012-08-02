#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <pthread.h>

#include <vector>
#include <map>

#include "rhobanProtocol.h"
#include "util.h"
#include "communicationCommon.h"
#include "ServerComponent.h"
#include "Message.h"

#define CLIENT_BUFFER_INITIAL_SIZE	32

using namespace Rhoban;

//A generic client compatible with rhoban communication protocol

namespace Rhoban
{
    class Client
    {
        public:
            virtual void shutdown();

        protected:

            Client();
            virtual ~Client();

            void log_error(string str);

            //used to read an incoming message
            //after the timeout, return null message
            //0 means infinite timeout
            Message * read_one_msg(uint timeout_ms);

            //used to read an incoming message when data is available
            //if no data, throws an exception
            Message * read_one_msg_now();

            //used to send an outgoing message
            static void send_msg(Message *msg, SOCKET sock );
            void send_msg(Message *msg);

            //vectors of in and out msgs managed by the client
            //one msg per component type
            vector<Message *> in_msgs;
            vector<Message *> out_msgs;

            bool connected;
            SOCKET sock;

            /*!
             * initializes connection with a server
             * throws an exception in case of problem
             * @param address
             * @param port
             */
            void init_connection(const char *address, int port);

        private:

            Buffer header_buffer;
    };
}

#endif // CLIENT_H
