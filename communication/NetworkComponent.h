#ifndef _NETWORK_COMPONENT_H
#define _NETWORK_COMPONENT_H

#include "ServerComponent.h"
#include "Message.h"
#include "Client.h"
#include "Callable.h"

namespace Rhoban
{
    class NetworkComponent : public Client, public ServerComponent
    {
        public:
            NetworkComponent(Callable *hub);

            Message *process(Message *msg_in, Message *msg_out, bool sync = false, int timeout = 0);
            void processAnswer(Message *msg_in);
            
            const ui16 virtual DestinationID() const { return 0; }
    };
}

#endif // _NETWORK_COMPONENT_H
