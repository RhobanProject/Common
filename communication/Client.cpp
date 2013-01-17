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
    Client::Client(Callable *hub_) : hub(hub_)
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
     * Logs an error
     */
    void Client::logError(string str)
    {
        SERVER_CAUTION(str);
    }

    /**
     * Process a mailbox incoming message
     */
    void Client::processMailboxMessage(Message *msg)
    {
        processMessage(msg);
    }

    /**
     * Process a mailbox incoming answer
     */
    void Client::processMailboxAnswer(Message *msg)
    {
        processMessage(msg);
    }

    /**
     * Process a message
     */
    void Client::processMessage(Message *msg)
    {
        if (!msg) {
            throw string("Cannot process null message");
        }

        Message *msg_out;

        if (!msg->answer) {
            msg_out = outMessages[msg->destination];
            msg_out->clear();
            msg_out->uid = msg->uid;
            msg_out->destination = msg->destination;
            msg_out->source = msg->source;
            msg_out->command = msg->command;
        }

        // Calling the component on the given message
        SERVER_DEBUG("InternalClient ("<<this<<") <-- message l" << msg->length << " s"<< msg->source <<" t"<< msg->destination << " st"<< msg->command << "("<<msg->uid<<") <-- remote ");
        
        try
        {
            Message *answer = hub->call(msg, msg_out);

            // If the component answered
            if (!msg->answer && answer) {
                    SERVER_DEBUG("InternalClient ("<<this<<") --> message l" << answer->length << " s"<< answer->source <<" t"<< answer->destination << " st"<< answer->command << "("<<answer->uid<<") --> remote ");
                sendMessage(answer);
            }
        } catch (string exc) {
            ostringstream smsg;
            smsg << "Failed to process message " << msg->uid << ": " << exc;
            SERVER_DEBUG(smsg.str());

            msg_out->destination = msg->destination;
            msg_out->command = MSG_ERROR_COMMAND;
            msg_out->append(smsg.str());
            sendMessage(msg_out);
        } catch (...)
        {
            ostringstream smsg;
            smsg << "Exception with message destination " << msg->destination << " command "
                << msg->command << " length " << msg->length << " uid "
                << msg->uid << ".";
            SERVER_DEBUG(smsg.str());

            msg_out->destination = msg->destination;
            msg_out->command = MSG_ERROR_COMMAND;
            msg_out->append(smsg.str());
            sendMessage(msg_out);
        }
    }
}
