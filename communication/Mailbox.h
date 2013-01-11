/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef MAILBOX_H___
#define MAILBOX_H___

#include <cstdlib>
#include <cstdio>
#include <map>
#include <threading/Condition.h>
#include <threading/Mutex.h>
#include <threading/Thread.h>
#include "BaseConnection.h"
#include "MailboxEntry.h"

using namespace std;

namespace Rhoban
{
    //typedef void sendCallback(Message *, void *);

    class Mailbox : public Thread
    {
        public :
            Mailbox(BaseConnection *connection);
            ~Mailbox();

            void execute();

            MailboxEntry * addEntry(ui32 uid);
            MailboxEntry * addEntry(ui32 uid, sendCallback *callback, void *data=NULL);

            void deleteEntry(ui32 uid);

            int entryIsWaiting(ui32 uid);
            int entryIsCallback(ui32 uid);
            void garbageCollector();
        protected:
            Mutex process;
            BaseConnection *connection;
            map<ui32, MailboxEntry *> entries;
            int garbageCounter;

            void addEntry(MailboxEntry * entry);

    };
}

#endif // MAILBOX_H___
