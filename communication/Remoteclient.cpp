#include "RemoteClient.h"

using namespace std

namespace Rhoban
{
    void RemoteClient::registerComponents()
    {
        Message *message = new Message();
        vector<ServerComponent *> components = hub->getComponents();
        
        for (auto it = components.begin(); it != components.end(); it++) {
            ServerComponent *component = (*it);
            message->clear();
            message->uid = 0;
            message->destination = MSG_TYPE_SERVER;
            message->command = MSG_SERVER_REGISTER_COMPONENT;
            message->append_uint(component->DestinationID());

            sendMessage(message);
        }
    }
}
