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

#include "rhobanProtocol.h"

using namespace std;

namespace Rhoban
{
    BaseConnection::BaseConnection()
    {
    }

    void BaseConnection::connectTo(const char *address, int port, bool runThread)
    {
        TCPClient::connectTo(address, port);

        if (isConnected() && runThread) {
            startMailbox();
        }
    }

    void BaseConnection::execute()
    {
        this->Mailbox::execute();
    }

    void BaseConnection::sendMessage(Message *message)
    {
        transmitAll(message->getRaw(), message->getSize());
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
        MailboxEntry * entry = addEntry(uid);

        entry->lock();
        sendMessage(message);
        entry->wait(timeout);
        entry->unlock();

        Message * retval = entry->getResponse();

        deleteEntry(uid);

        if(retval->command == MSG_ERROR_COMMAND) {
            ui32 dest = message->destination;
            if(dest < RHOBAN_MESSAGE_DESTINATIONS_NB)
            	throw string("Error from ") + RHOBAN_MESSAGE_DESTINATIONS[dest] + " : " + retval->read_string();
            else
            	throw string("Error message : ") + retval->read_string();
        } else {
            return retval;
        }
    }

    void BaseConnection::sendMessageCallback(Message *message, sendCallback *callback, void *data)
    {
        addEntry(message->getUid(), callback, data);
        sendMessage(message);
    }

    void BaseConnection::startMailbox()
    {
        start(NULL);
    }

    bool BaseConnection::isConnected()
    {
        return this->TCPClient::isConnected();
    }
}
