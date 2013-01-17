#include "NetworkComponent.h"

namespace Rhoban
{
    NetworkComponent::NetworkComponent(Callable *hub) : Client(hub)
    {
        setHub(hub);
    }

    Message *NetworkComponent::process(Message * msg_in, Message * msg_out, bool sync, int timeout)
    {
        if (!sync) {
            sendMessage(msg_in);
            return NULL;
        } else {
            return sendMessageReceive(msg_in, timeout);
        }
    }
            
    void NetworkComponent::processAnswer(Message *msg_in)
    {
        sendMessage(msg_in);
    }
}
