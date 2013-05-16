/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <cstdlib>
#include <cstdio>
#include <map>
#include <ctime>
#include <threading/Thread.h>
#include "MailboxEntry.h"

using namespace std;

namespace Rhoban
{
    MailboxEntry::MailboxEntry(ui32 uid)
    {
        hasResponse = false;
        this->uid = uid;
        waiting = false;
        callback = NULL;
        creationDate = time(NULL);
        data = NULL; 
        sameThreadResponded = false;
        threadId = Thread::currentThreadId();
    }

    MailboxEntry::MailboxEntry(ui32 uid, sendCallback *callback, void *data)
    {
        hasResponse = false;
        this->uid = uid;
        waiting = false;
        this->callback = callback;
        creationDate = time(NULL);
        this->data = data;    
        threadId = -1;
        sameThreadResponded = false;
    }

    MailboxEntry::~MailboxEntry()
    {
        //The response was not create by this object and should not be detsroyed by this object as well
        //delete response;
        //response = 0;
        //same thing
        //delete waiting;
    }

    void MailboxEntry::wait(int timeout)
    {
        try
        {
            waiting = true;
            Condition::wait(timeout);
            waiting = false;
        }
        catch(string exc)
        {
            waiting = false;
            throw string("Mailbox entry failed to wait condition:\n\t");
        }
    }

    bool MailboxEntry::isWaiting()
    {
        return waiting;
    }

    bool MailboxEntry::isCallback()
    {
        return(callback != NULL);
    }

    void MailboxEntry::executeCallback(Message *message)
    {
        callback(message, data);
    }

    ui32 MailboxEntry::getUid()
    {
        return uid;
    }

    time_t MailboxEntry::getCreationDate()
    {
        return creationDate;
    }

    Message MailboxEntry::getResponse()
    {
        return response;
    }

    void MailboxEntry::setResponse(const Message & message)
    {
        hasResponse = true;
        response = message;
    }

}
