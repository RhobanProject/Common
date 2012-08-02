/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
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
