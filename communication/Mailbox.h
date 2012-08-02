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
#include "Connection.h"
#include "MailboxEntry.h"

using namespace std;

namespace Rhoban
{
  typedef void sendCallback(Message *);

  class Mailbox : public Thread
  {
  public :
    Mailbox(Connection* connection);
    ~Mailbox();
    void execute();
    void addEntry(MailboxEntry *entry);
    void deleteEntry(ui32 uid); 
    int entryIsWaiting(ui32 uid);
    int entryIsCallback(ui32 uid);
    void setResponse(ui32 uid, Message * message);
    Message * getResponse(ui32 uid);
    void wait(ui32 uid, int timeout);
    void broadcastCondition(ui32 uid);
    void garbageCollector();
    void lock();
  protected:
    Mutex process;
    Connection *connection;
    map<ui32, MailboxEntry *> entries;
    int garbageCounter;
  };
}

#endif // MAILBOX_H___
