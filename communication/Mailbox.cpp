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
#include <threading/Condition.h>
#include <threading/Mutex.h>
#include <threading/Thread.h>
#include "Mailbox.h"
#include "Mailbox.h"
#include "MailboxEntry.h"

#include "rhobanProtocol.h"

#define GARBAGETIMER 300
#define GARBAGECHECKRATE 100

using namespace std;

namespace Rhoban
{
    Mailbox::Mailbox()
    {
        garbageCounter = 0;
    }

    Mailbox::~Mailbox()
    {
        this->thread_state=Dead;
        BEGIN_SAFE(process)
            entries.clear();
        END_SAFE(process)
    }

    void Mailbox::execute()
    {
        Message message;
        while (isConnected())
        {
            getMessage(&message);
            processIncomingMessage(&message);
        }
    }

    void Mailbox::processErrorMessage(Message * message)
    {
        try
        {
        	/*
            ui32 dest = message->destination;

            if(dest < RHOBAN_MESSAGE_DESTINATIONS_NB)
                cout << "Error message from " << RHOBAN_MESSAGE_DESTINATIONS[dest] << ":" << endl << "\t";
            else
                cout << "Error message from " << dest << ":" << endl << "\t";
            cout << message->read_string() << endl;
            */
        }
        catch(const string & exc)
        {
            cout << "Failed to process error message:\n\t " << exc;
        }
    }

    void Mailbox::processMailboxMessage(Message *message)
    {
        cout << "Mailbox is not supposed to process incoming messages" << endl;
    }

    void Mailbox::processMailboxAnswer(Message *message)
    {
    }

    void Mailbox::processIncomingMessage(Message * message)
    {
        ui32 uid = message->getUid();
        message->incoming = true;

        if(message->command == MSG_ERROR_COMMAND || message->destination == MSG_TYPE_ERROR) {
            processErrorMessage(message);
        }

        BEGIN_SAFE(process)
            if (message->answer) {
                map<ui32, MailboxEntry *>::iterator it = entries.find(uid);

                if(it != entries.end())
                {
                    MailboxEntry * entry = it->second;
                    bool deleteEntry = false;
                    BEGIN_SAFE((*entry))
                        if(entry->isWaiting())
                        {
                            entry->setResponse(*message);

                            if (entry->threadId == Thread::currentThreadId()) {
                                entry->sameThreadResponded = true;
                            } else {
                                entry->broadcast();
                            }
                        }
                        else
                        {
                            if(entry->isCallback())
                            {
                                entry->executeCallback(message);
                                deleteEntry = true;
                                entries.erase(uid);
                            }
                        }
                    END_SAFE((*entry))
                        if(deleteEntry)
                            delete entry;

                } else {
                    processMailboxAnswer(message);
                }
            } else {
                processMailboxMessage(message);
            }
        END_SAFE(process)
    }

    void Mailbox::deleteEntry(ui32 uid)
    {
        BEGIN_SAFE(process)
            MailboxEntry * entry = entries[uid];
        entries.erase(uid);
        delete entry;
        END_SAFE(process)
    }

    void Mailbox::addEntry(MailboxEntry * entry)
    {
        BEGIN_SAFE(process)
            entries[entry->getUid()]= entry;

        garbageCounter++;
        if(garbageCounter >= GARBAGECHECKRATE)
        {
            garbageCounter = 0;
            garbageCollector();
        }
        END_SAFE(process)
    }

    MailboxEntry * Mailbox::addEntry(ui32 uid)
    {
        MailboxEntry * entry = new MailboxEntry(uid);
        addEntry(entry);
        return entry;
    }

    MailboxEntry * Mailbox::addEntry(ui32 uid, sendCallback *callback, void *data)
    {
        MailboxEntry * entry = new MailboxEntry(uid, callback, data);
        addEntry(entry);
        return entry;
    }


    void Mailbox::garbageCollector()
    {
        vector<int> toDelete;

        for(map<ui32, MailboxEntry *>::iterator it=entries.begin(); it!=entries.end(); ++it) {
            MailboxEntry *entry = it->second;
            if(time(NULL) - entry->getCreationDate() > GARBAGETIMER) {
                toDelete.push_back(entry->getUid());
            }
        }

        for (vector<int>::iterator it=toDelete.begin(); it!=toDelete.end(); it++) {
            int uid = *it;
            MailboxEntry *entry = entries[uid];
            entries.erase(uid);
            delete entry;
        }
    }

    Message Mailbox::sendMessageReceive(Message *message, int timeout)
    {
        ui32 uid = message->getUid();

        //preparing message and condition
        MailboxEntry * entry = addEntry(uid);

        entry->lock();
        sendMessage(message);
		

        if (!entry->sameThreadResponded) {
            try {
                entry->wait(timeout);
            } catch (string exception) {
                entry->unlock();
                throw exception;
            }
            entry->unlock();
        }

        if (!entry->hasResponse) {
            throw string("Unable to get a response for the message");
        }

        Message retval = entry->getResponse();

        deleteEntry(uid);

        if(retval.command == MSG_ERROR_COMMAND) {
            ui32 dest = message->destination;
            if(dest < RHOBAN_MESSAGE_DESTINATIONS_NB)
            	throw string("Error from ") + RHOBAN_MESSAGE_DESTINATIONS[dest] + " : " + retval.read_string();
            else
            	throw string("Error message : ") + retval.read_string();
        } else {
            return retval;
        }
    }

    void Mailbox::sendMessageCallback(Message *message, sendCallback *callback, void *data)
    {
        addEntry(message->getUid(), callback, data);
        sendMessage(message);
    }
}
