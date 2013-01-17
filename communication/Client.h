/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <pthread.h>

#include <vector>
#include <map>

#include <sockets/TCPClientBase.h>

#include "rhobanProtocol.h"
#include "util.h"
#include "communicationCommon.h"
#include "BaseConnection.h"
#include "Callable.h"
#include "Message.h"

#define CLIENT_BUFFER_INITIAL_SIZE 32

using namespace Rhoban;

/**
 * A generic client compatible with Rhoban communication protocol
 */
namespace Rhoban
{
    class ServerHub;

    class Client : public virtual TCPClientBase, public virtual BaseConnection
    {
        protected:
            Client(Callable *hub);
            virtual ~Client();

            void logError(string str);

            /**
             * Process a message
             */
            virtual void processMessage(Message *msg);

            void processMailboxMessage(Message *msg);
            void processMailboxAnswer(Message *msg);

            //vectors of in and out msgs managed by the client
            //one msg per component type
            vector<Message *> inMessages;
            vector<Message *> outMessages;
            
            Callable *hub;

        private:
            Buffer headerBuffer;
    };
}

#endif // CLIENT_H
