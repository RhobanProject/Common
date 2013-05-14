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

    void BaseConnection::getMessage(Message *message)
    {
        message->clear();

        receiveAll(message->getBuffer(), MSG_HEADER_SIZE);
        message->read_header(message->getBuffer());

        message->alloc(message->getLength() + MSG_HEADER_SIZE);
        message->setSize(message->getLength() + MSG_HEADER_SIZE);
        receiveAll(message->getBuffer() + MSG_HEADER_SIZE, message->getLength());

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
