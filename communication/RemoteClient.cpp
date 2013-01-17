#include <vector>
#include "RemoteClient.h"
#include "ServerComponent.h"
#include "Server.h"

using namespace std;

namespace Rhoban
{
    RemoteClient::RemoteClient(Callable *hub) : NetworkComponent(hub)
    {
        ServerHub *serverHub = dynamic_cast<ServerHub *>(hub);

        if (serverHub) {
            serverHub->fallbackComponent = this;
        }
    }

    void RemoteClient::registerComponents()
    {
        SERVER_MSG("Registering remote components");
        Message *message = new Message();
        ServerHub *serverHub = dynamic_cast<ServerHub*>(this->Client::hub);

        if (serverHub) {
            vector<ServerComponent *> components = serverHub->getComponents();
            vector<ServerComponent *>::iterator it;
            
            for (it = components.begin(); it != components.end(); it++) {
                ServerComponent *component = (*it);
                message->clear();
                message->uid = 0;
                message->destination = MSG_TYPE_SERVER;
                message->command = MSG_SERVER_REGISTER_COMPONENT;
                message->append((ui32)component->DestinationID());

                sendMessage(message);
                SERVER_MSG("Registering remote component " << component->DestinationID());
            }
        }
    }
}
