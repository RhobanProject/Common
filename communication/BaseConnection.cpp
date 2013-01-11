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
#include "BaseConnection.h"
#include "MailboxEntry.h"

using namespace std;

namespace Rhoban
{
    BaseConnection::BaseConnection(CommandsStore *commandsStore_) : mailbox(this)
    {
        commandsStore = commandsStore_;
    }
            
    void BaseConnection::connectTo(const char *address, int port)
    {
        TCPClient::connectTo(address, port);

        if (isConnected()) {
            startMailbox();
        }
    }

    void BaseConnection::sendMessage(Message *message)
    {
        transmit(message->getRaw(), message->getSize());
    }

    Message* BaseConnection::getMessage() 
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

    Message *BaseConnection::getMessage(Message *message)
    {
        message->clear();

        receiveAll(message->getBuffer(), MSG_HEADER_SIZE);
        message->read_header(message->getBuffer());

        message->alloc(message->getLength() + MSG_HEADER_SIZE);
        message->setSize(message->getLength() + MSG_HEADER_SIZE);
        receiveAll(message->getBuffer() + MSG_HEADER_SIZE, message->getLength());

        return message;
    }

    Message *BaseConnection::sendMessageReceive(Message *message, int timeout)
    {
        ui32 uid = message->getUid();

        //preparing message and condition
        MailboxEntry * entry = mailbox.addEntry(uid);

        entry->lock();
        sendMessage(message);
        entry->wait(timeout);
        entry->unlock();

        Message * retval = entry->getResponse();

        mailbox.deleteEntry(uid);

        return retval;
    }

    void BaseConnection::sendMessageCallback(Message *message, sendCallback *callback, void *data)
    {
        mailbox.addEntry(message->getUid(), callback, data);
        sendMessage(message);
    }

    void BaseConnection::startMailbox()
    {
        mailbox.start(NULL);
    }
}
