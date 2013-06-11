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
        } else {
            connection = new BaseConnection();
            connect = true;
            connections[key] = connection;
        }
        END_SAFE(mutex)
        mutex.unlock();
        
        if (connect) {
            connection->connectTo(hostname.c_str(), port);
        }

        return connection;
    }

    void Connections::cleanUp()
    {
        map<string, BaseConnection *>::iterator it;
        vector<string> toDelete;

        mutex.lock();
        for (it=connections.begin(); it!=connections.end(); it++) {
            BaseConnection *connection = (*it).second;

            if (!connection->isConnected()) {
                delete connection;

                toDelete.push_back((*it).first);
            }
        }

        for (vector<string>::iterator sit=toDelete.begin(); sit!=toDelete.end(); sit++) {
            connections.erase(*sit);
        }
        mutex.unlock();
    }
};
