#ifndef _REMOTE_CLIENT_H
#define _REMOTE_CLIENT_H

#include <sockets/TCPClient.h>

#include "NetworkComponent.h"
#include "Callable.h"

using namespace std;

namespace Rhoban
{
    class RemoteClient : public NetworkComponent
    {
        public:
            RemoteClient(Callable *hub);

            /**
             * Register the components to remote server
             */
            void registerComponents();
    
            /**
             * Process a message
             */
            Message *process(Message * msg_in, Message * msg_out, bool sync = false, int timeout = 1000);
            void processAnswer(Message *msg_in);
            
            /**
             * A remote client responds to all the requests by sending them over the network
             */
            bool respondTo(ui16 type);
    };
}

#endif // _REMOTE_CLIENT_H
