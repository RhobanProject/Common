/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <sys/time.h>
#include <sys/types.h>


#include <stdlib.h>
#include <iostream>
#include <string>
#include "string.h"

#include <errno.h>
#include <time.h>

#include "util.h"
#include "Encodings.h"

#include "Client.h"
#include "Message.h"
#include "Server.h"

using namespace std;

namespace Rhoban
{
    /**
     * Creating a client
     */
    Client::Client()
    {
        headerBuffer.alloc(MSG_HEADER_SIZE);

        for(int i =0;i < MSG_TYPE_MAX_NB; i++)
        {
            inMessages.push_back(new Message());
            outMessages.push_back(new Message());
        }
    }

    /**
     * Deleting a client
     */
    Client::~Client()
    {
        for(int i =0;i < MSG_TYPE_MAX_NB; i++)
        {
            if(inMessages[i])
            {
                delete(inMessages[i]);
                inMessages[i] = 0;
            }
            if(outMessages[i])
            {
                delete(outMessages[i]);
                outMessages[i] = 0;
            }
        }
    }

    /**
     * Reading one message from the client when the socket become available
     */
    Message * Client::readOneMessage(uint timeout_ms)
    {
        if (waitReady(timeout_ms)) {
            return readOneMessageNow();
        } else {
            SERVER_CAUTION("Client waited " << timeout_ms << " without getting anything");
            return NULL;
        }
    }

    /**
     * Reading one message now
     */
    Message * Client::readOneMessageNow()
    {
        int received = 0;
        int size = MSG_HEADER_SIZE;
        char *p = headerBuffer.buffer;

        // Reading headers
        SERVER_DEBUG("Client trying to read one message from socket");
        try {
            receiveAll(p, size);
        } catch (string exc) {
            string msg = "Client socket closed while reading headers ("+exc+")";
            SERVER_DEBUG(msg);
            throw msg;
        }

        // Reading the destination
        ui32 type = Encodings::decode_ushort(headerBuffer.buffer + Header::destination_offset); // Destination

        // Reads the message
        Message *in_msg = inMessages[type];
        in_msg->clear();
        in_msg->read_header(headerBuffer.buffer);
        size = in_msg->length;

        if (size) {
            in_msg->alloc(size + MSG_HEADER_SIZE);
            in_msg->setSize(size + MSG_HEADER_SIZE);
            char *p = in_msg->buffer + MSG_HEADER_SIZE;

            try {
                receiveAll(p, size);
            } catch (string exc) {
                throw string("Failed to receive the message ("+exc+")");
            }
        }

        return in_msg;
    }

    /**
     * Sends a message to the client
     */
    void Client::sendMessage(Message *msg)
    {
        if (!msg) {
            return;
        }

        msg->length = msg->getSize() - MSG_HEADER_SIZE;
        msg->write_header(msg->buffer);

        SERVER_DEBUG("Client --> message l" << msg->length << " d"<< msg->destination << " c"<< msg->command << "("<<msg->uid  <<" --> remote ");
        char * p = msg->buffer;
        int size = msg->getSize();

        transmitAll(p, size);
    }

    /**
     * Logs an error
     */
    void Client::logError(string str)
    {
        SERVER_CAUTION(str);
    }
}
