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
    };
}

#endif // _REMOTE_CLIENT_H
