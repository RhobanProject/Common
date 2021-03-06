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
        ServerHub *serverHub = dynamic_cast<ServerHub*>(this->NetworkComponent::hub);

        if (serverHub) {
			vector<ui16> components = serverHub->getComponents();
			
			for (vector<ui16>::iterator it = components.begin(); it != components.end(); it++) {
                message->clear();
                message->uid = 0;
                message->destination = MSG_TYPE_SERVER;
                message->command = MSG_SERVER_REGISTER_COMPONENT;
                message->append(*it);
                this->BaseConnection::sendMessage(message);
				SERVER_MSG("Registering remote component " << *it);
            }
        }
    }

    Message *RemoteClient::process(Message * msg_in, Message * msg_out, bool sync, int timeout)
    {
        if (!msg_in->incoming) {
            return this->NetworkComponent::process(msg_in, msg_out, sync, timeout);
        }

        return NULL;
    }
            
    void RemoteClient::processAnswer(Message *msg_in)
    {
        if (!msg_in->incoming) {
            this->NetworkComponent::processAnswer(msg_in);
        }
    }

    bool RemoteClient::respondTo(ui16 id)
    {
        return true;
    }
}
