#ifndef THREADEDCLIENT_H
#define THREADEDCLIENT_H

#include "Client.h"

class ThreadedClient : public Client, public Thread
{
    protected:
        /**
         * To be overriden in derived classes
         * for example an internal server client waits for an incoming message 
         * and process it
         */
        void execute() = 0;

};

#endif // THREADEDCLIENT_H
