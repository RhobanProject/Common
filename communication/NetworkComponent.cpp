#include "NetworkComponent.h"
#include "Server.h"

namespace Rhoban
{
    NetworkComponent::NetworkComponent(Callable *hub_)
    {
        setHub(hub_);
        headerBuffer.alloc(MSG_HEADER_SIZE);
    }

    Message *NetworkComponent::process(Message * msg_in, Message * msg_out, bool sync, int timeout)
    {
        if (!sync) {
            this->BaseConnection::sendMessage(msg_in);
            return NULL;
        } else {
            return this->BaseConnection::sendMessageReceive(msg_in, timeout);
        }
    }
            
    void NetworkComponent::processAnswer(Message *msg_in)
    {
        this->BaseConnection::sendMessage(msg_in);
    }

    /**
     * Deleting a client
     */
    NetworkComponent::~NetworkComponent()
    {
    	for (map<ui16,Message *>::iterator it = outMessages.begin(); it != outMessages.end(); it++) {
    		if (it->second != 0) {
    			delete it->second;
                }
        }
    	outMessages.clear();
    }

    Message * NetworkComponent::getOutMessage(ui16 destination)
    {
    	map<ui16,Message *>::iterator it = outMessages.find(destination);
    	if(it == outMessages.end())
    	{
    		Message * msg = new Message;
    		outMessages[destination] = msg;
    		return msg;
    	}
    	else
    		return it->second;

    }
    /**
     * Logs an error
     */
    void NetworkComponent::logError(string str)
    {
        SERVER_CAUTION(str);
    }

    /**
     * Process a mailbox incoming message
     */
    void NetworkComponent::processMailboxMessage(Message *msg)
    {
        processMessage(msg);
    }

    /**
     * Process a mailbox incoming answer
     */
    void NetworkComponent::processMailboxAnswer(Message *msg)
    {
        processMessage(msg);
    }

    /**
     * Process a message
     */
    void NetworkComponent::processMessage(Message *msg)
    {
        if (!msg) {
            throw string("Cannot process null message");
        }

        Message * msg_out = getOutMessage(msg->destination);
        msg_out->clear();
        msg_out->uid = msg->uid;
        msg_out->destination = msg->destination;
        msg_out->source = msg->source;
        msg_out->command = msg->command;

        // Calling the component on the given message
        SERVER_DEBUG("NetworkComponent ("<<this<<") <-- message l" << msg->length << " s"<< msg->source <<" t"<< msg->destination << " st"<< msg->command << "("<<msg->uid<<") <-- remote ");
        
        try
        {
            Message *answer = hub->call(msg, msg_out);

            // If the component answered
            if (!msg->answer && answer) {
                    SERVER_DEBUG("NetworkComponent ("<<this<<") --> message l" << answer->length << " s"<< answer->source <<" t"<< answer->destination << " st"<< answer->command << "("<<answer->uid<<") --> remote ");
                this->BaseConnection::sendMessage(answer);
            }
        } catch (string & exc) {
            ostringstream smsg;
            smsg << "Failed to process message " << msg->uid << ": " << exc;
            SERVER_DEBUG(smsg.str());

            msg_out->destination = msg->source;
            msg_out->source = msg->destination;
            msg_out->command = MSG_ERROR_COMMAND;
            msg_out->append(smsg.str());
            this->BaseConnection::sendMessage(msg_out);
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
            this->BaseConnection::sendMessage(msg_out);
        }
    }

    void NetworkComponent::execute()
    {
        this->BaseConnection::execute();
    }
}
