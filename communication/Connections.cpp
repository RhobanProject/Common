#include <threading/Thread.h>
#include "Connections.h"

namespace Rhoban
{
    Connections::~Connections()
    {
        map<string, BaseConnection *>::iterator it;

        for (it=connections.begin(); it!=connections.end(); it++) {
            delete (*it).second;
        }

        connections.clear();
    }

    BaseConnection *Connections::getConnection(string hostname, int port)
    {
        bool connect = false;
        ostringstream oss;
        oss << hostname << ":" << port;
        string key = oss.str();
        BaseConnection *connection = NULL;

        BEGIN_SAFE(mutex)
        if (connections.find(key) != connections.end()) {
            connection = connections[key];
            if (!connection->isConnected()) {
                if (!connecting[key]) {
                    connect = true;
                    connecting[key] = true;
                } else {
                    throw std::runtime_error("Connecting...");
                }
            }
        } else {
            connection = new BaseConnection();
            connect = true;
            connecting[key] = true;
            connections[key] = connection;
        }
        END_SAFE(mutex)
        mutex.unlock();
        
        if (connect) {
            try {
                connection->connectTo(hostname.c_str(), port);
            } catch (const std::runtime_error error) {
                connecting[key] = false;
                throw error;
            }
            connecting[key] = false;
        }

        return connection;
    }
};
