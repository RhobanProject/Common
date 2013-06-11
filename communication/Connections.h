#ifndef _CONNECTIONS_H
#define _CONNECTIONS_H

#include <map>
#include <threading/Mutex.h>

#include "BaseConnection.h"

namespace Rhoban
{
    /**
     * This is a pool of connections, it keep an internal mapping to get the required
     * connection when needed
     */
    class Connections
    {
        public:
            virtual ~Connections();

            /**
             * Gets the connection for hostname & port
             */
            BaseConnection *getConnection(string hostname, int port);

            /**
             * Remove the dead connections
             */
            void cleanUp();

        protected:
            Mutex mutex;
            map<string, BaseConnection*> connections;
    };
}

#endif
