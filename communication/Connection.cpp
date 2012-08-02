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
#include <string>
#include "Message.h"
#include "Connection.h"
#include "MailboxEntry.h"

using namespace std;

namespace Rhoban
{
  Connection::Connection():mailbox(this){}

  void Connection::sendMessage(Message *message)
  {
    transmit(message->getRaw(), message->getSize());
    delete(message);
  }
  
  Message* Connection::getMessage() 
  {
    Message* message = new Message;
    
    try {
      getMessage(message);
    } catch (string e) {
      delete message;
      throw e;
    }
    
    return message;
  }  

  Message *Connection::getMessage(Message *message)
  {
    message->clear();

    receiveAll(message->getBuffer(), MSG_HEADER_SIZE);
    message->read_header(message->getBuffer());

    message->alloc(message->getLength() + MSG_HEADER_SIZE);
    message->setSize(message->getLength() + MSG_HEADER_SIZE);
    receiveAll(message->getBuffer() + MSG_HEADER_SIZE, message->getLength());

    return message;
  }
  
  Message * Connection::sendMessageRecieve(Message *message, int timeout)
  {
    Message * retval;
    ui32 uid = message->getUid();
    
    Condition condition;
    
    mailbox.addEntry(new MailboxEntry(uid, &condition));

    mailbox.lock();
    sendMessage(message);
    mailbox.wait(uid, timeout);
    retval = mailbox.getResponse(uid);
    return retval;
  }
  
  void Connection::sendMessageCallback(Message *message, sendCallback *callback)
  {
    MailboxEntry *entry = new MailboxEntry(message->getUid(), callback);
    mailbox.addEntry(entry);
    sendMessage(message);
    delete(entry);
  }

  void Connection::startMailbox()
  {
    mailbox.start(NULL);
  }
}
