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
#include "Message.h"
#include "Mailbox.h"

using namespace std;
    
namespace Rhoban
{  
    class BaseConnection : public virtual TCPClient, public virtual Mailbox
    {
        public:
            BaseConnection();

            void sendMessage(Message *message);
            void startMailbox();
            Message *getMessage();
            Message *getMessage(Message *message);
            void connectTo(const char *address, int port, bool runThread = true);
            void execute();

            bool isConnected();
    };
}

#endif // _BASE_CONNECTION_H_
