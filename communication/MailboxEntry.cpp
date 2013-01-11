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
#include "MailboxEntry.h"

using namespace std;

namespace Rhoban
{
MailboxEntry::MailboxEntry(ui32 uid, Condition *condition)
{
	this->uid = uid;
	waiting = condition;
	response = NULL;
	callback = NULL;
	creationDate = time(NULL);
	data = NULL; 
}

MailboxEntry::MailboxEntry(ui32 uid, sendCallback *callback, void *data)
{
	this->uid = uid;
	waiting = NULL;
	response = NULL;
	this->callback = callback;
	creationDate = time(NULL);
	this->data = data;    
}

MailboxEntry::~MailboxEntry()
{
	delete response;
	delete waiting;
}

void MailboxEntry::wait(int timeout)
{
	try
	{
		waiting->wait(timeout);
	}
	catch(string exc)
	{
		throw string("Mailbox entry failed to wait condition:\n\t");
	}
}

int MailboxEntry::isWaiting()
{
	return(waiting != NULL);
}

int MailboxEntry::isCallback()
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

Message * MailboxEntry::getResponse()
{
	return response;
}

void MailboxEntry::setResponse(Message * message)
{
	response=message;
}

void MailboxEntry::broadcast()
{
	try
	{
	waiting->lock();
	waiting->broadcast();
	waiting->unlock();
	}
	catch(...)
	{
		cout << "Failed to broadcast" << endl;
	}
}
}
