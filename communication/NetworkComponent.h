#ifndef _NETWORK_COMPONENT_H
#define _NETWORK_COMPONENT_H

#include <iostream>

#include "ServerComponent.h"
#include "BaseConnection.h"
#include "Message.h"

using namespace std;

#define CLIENT_BUFFER_INITIAL_SIZE 32

namespace Rhoban
{
    class NetworkComponent : public BaseConnection, public ServerComponent
    {
        public:
            NetworkComponent(Callable *hub);
            virtual ~NetworkComponent();

            Message *process(Message *msg_in, Message *msg_out, bool sync = false, int timeout = 0);
            void processAnswer(Message *msg_in);
            
            const ui16 virtual DestinationID() const { return 0; }

        protected:
            void logError(string message);
            virtual void processMessage(Message *msg);

            void processMailboxMessage(Message *msg);
            void processMailboxAnswer(Message *msg);

            //vectors of in and out msgs managed by the client
            //one msg per component type
            map<ui16, Message *> inMessages;
            map<ui16, Message *> outMessages;
            Message * getOutMessage(ui16 destination);
            
        private:
            Buffer headerBuffer;
    };
};

#endif // _NETWORK_COMPONENT_H
