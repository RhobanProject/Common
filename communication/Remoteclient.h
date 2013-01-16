#ifndef _REMOTE_CLIENT_H
#define _REMOTE_CLIENT_H

#include <sockets/TCPClient.h>

#include "Client.h"

using namespace std

namespace Rhoban
{
    class RemoteClient : public Client, public TCPClient
    {
        public:
            void registerComponents();
    }
}

#endif // _REMOTE_CLIENT_H
