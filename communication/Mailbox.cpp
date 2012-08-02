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
#include "Connection.h"
#include "Mailbox.h"
#include "MailboxEntry.h"

#define GARBAGETIMER 300
#define GARBAGECHECKRATE 100

using namespace std;

namespace Rhoban
{
  Mailbox::Mailbox(Connection* connection) 
  {
    this->connection = connection;
    garbageCounter = 0;
  }

  Mailbox::~Mailbox()
  {
    this->thread_state=Dead;
    entries.clear();
  }

  void Mailbox::execute()
  {
    while(connection->isConnected())
      {
	Message *message = connection->getMessage();

	process.lock();
	if(entries.count(message->getUid()))
	  {
	    if(entries[message->getUid()]->isWaiting())
	      {
		setResponse(message->getUid(), message);
		broadcastCondition(message->getUid());
	      }
	    else if(entries[message->getUid()]->isCallback())
	      {
		entries[message->getUid()]->executeCallback(message);
		deleteEntry(message->getUid());
		delete(message);
	      }
	  }
	else
	  delete(message);
	process.unlock();
      }
  }

  void Mailbox::deleteEntry(ui32 uid)
  {
    entries.erase(uid);
  }

  void Mailbox::wait(ui32 uid, int timeout)
  {
    entries[uid]->wait(timeout, &process);
    process.unlock();
  }

  void Mailbox::setResponse(ui32 uid, Message * message)
  {
    entries[uid]->setResponse(message);
  }

  Message * Mailbox::getResponse(ui32 uid)
  {
    return(entries[uid]->getResponse());
  }

  void Mailbox::lock()
  {
    process.lock();
  }

  void Mailbox::broadcastCondition(ui32 uid)
  {
    entries[uid]->broadcast();
  }
  
  void Mailbox::addEntry(MailboxEntry *entry)
  {
    entries[entry->getUid()]=entry;
    garbageCounter++;
    if(garbageCounter >= GARBAGECHECKRATE)
      {
	garbageCounter = 0;
	garbageCollector();
      }
  }
  
  void Mailbox::garbageCollector()
  {
    map<ui32, MailboxEntry *>::iterator it;
    for(it=entries.begin(); it!=entries.end(); ++it)
      {
	if(time(NULL) - it->second->getCreationDate() > GARBAGETIMER)
	  deleteEntry(it->second->getUid());
      }
  }
 
}
