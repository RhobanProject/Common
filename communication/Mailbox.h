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
#include "MailboxEntry.h"

using namespace std;

namespace Rhoban
{
    //typedef void sendCallback(Message *, void *);

    class Mailbox : public virtual Thread
    {
        public :
            Mailbox();
            ~Mailbox();

            MailboxEntry * addEntry(ui32 uid);
            MailboxEntry * addEntry(ui32 uid, sendCallback *callback, void *data=NULL);
            void deleteEntry(ui32 uid);

            virtual bool isConnected()=0;
            virtual void getMessage(Message *)=0;
            virtual void sendMessage(Message *message)=0;
            
            Message sendMessageReceive(Message *message, int timeout=1000);
            void sendMessageCallback(Message *message, sendCallback *callback, void *data=NULL);


        protected:

            void execute();
            void processIncomingMessage(Message *);
            virtual void processMailboxMessage(Message *);
            virtual void processMailboxAnswer(Message *);

            /*
             * Does nothing, can be overriden if needed?
             */
            virtual void processErrorMessage(Message *);
            void garbageCollector();

            Mutex process;
            map<ui32, MailboxEntry *> entries;
            int garbageCounter;

            void addEntry(MailboxEntry * entry);

    };
}

#endif // MAILBOX_H___
