/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef MAILBOX_ENTRY_H
#define MAILBOX_ENTRY_H

#include <cstdlib>
#include <cstdio>
#include <map>
#include <ctime>

#include <communication/types.h>
#include <threading/Condition.h>

#include "Message.h"

using namespace std;

namespace Rhoban
{
  typedef void sendCallback(Message *, void *);

  class MailboxEntry
  {
  public:
    MailboxEntry(ui32 uid, Condition *condition);
    MailboxEntry(ui32 uid, sendCallback *callback, void *data=NULL);
    ~MailboxEntry();
    void wait(int timeout);
    int isWaiting();
    int isCallback();
    void executeCallback(Message *message);
    ui32 getUid();
    time_t getCreationDate();
    Message * getResponse();
    void setResponse(Message * message);
    void broadcast();
  protected:
    Message * response;
    Condition * waiting;
    sendCallback* callback;
    ui32 uid;
    time_t creationDate;
	void *data;
  };
}

#endif // MAILBOX_ENTRY_H
