/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef _BASE_CONNECTION_H_
#define _BASE_CONNECTION_H_

namespace Rhoban{
    class Connection;
    class BaseConnection;
}

#include <cstdlib>
#include <cstdio>
#include <sockets/TCPClient.h>
#include "CommandsStore.h"
#include "Message.h"
#include "Mailbox.h"

using namespace std;
    
namespace Rhoban
{  
    typedef void sendCallback(Message *);

    class BaseConnection : public TCPClient
    {
        public:
            BaseConnection(CommandsStore *commandsStore_);

            void sendMessage(Message *message);
            Message *sendMessageRecieve(Message *message, int timeout=100);
            void sendMessageCallback(Message *message, sendCallback *callback);
            void startMailbox();
            Message *getMessage();
            Message *getMessage(Message *message);
        protected:
            CommandsStore *commandsStore;
            Mailbox mailbox;
    };
}

#endif // _BASE_CONNECTION_H_
