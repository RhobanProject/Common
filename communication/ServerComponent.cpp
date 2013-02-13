/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
/*
 * ThreadedComponent.cpp
 *
 *  Created on: 13 mars 2011
 *      Author: hugo
 */
#include <stdlib.h>
#include <iostream>
#include <timing/TickMachine.h>

#include "ServerComponent.h"

Message *ServerComponent::doCall(Message *msg_in, Message *msg_out, bool sync, int timeout)
{
    if (respondTo(msg_in->destination)) {
        if (msg_in->answer) {
            processAnswer(msg_in);
            return NULL;
        } else {
            if(!msg_out) {
                msg_out = new Message();
            } else {
                msg_out->clear();
            }
            msg_out->uid = msg_in->uid;
            msg_out->destination = msg_in->source;
            msg_out->source = msg_in->destination;
            msg_out->command = msg_in->command;
            msg_out->answer = true;

            Message * answer = process(msg_in, msg_out, sync, timeout);

            if (answer)
                answer->write_header();

            return answer;
        }
    } else if (hub) {
        if (sync) {
            return hub->callSync(msg_in, msg_out, timeout);
        } else {
            return hub->call(msg_in, msg_out);
        }
    } else {
        throw string("Cannot route message to destination ") + my_itoa(msg_in->destination);
    }
};

bool ServerComponent::respondTo(ui16 id)
{
    return (id == DestinationID());
}

Message *ServerComponent::call(Message *msg_in, Message *msg_out)
{
    return doCall(msg_in, msg_out, false, 0);
}

Message *ServerComponent::callSync(Message *msg_in, Message *msg_out, int timeout)
{
    return doCall(msg_in, msg_out, true, timeout);
}

void ServerComponent::setHub(Callable * hub)
{
	this->hub = hub;
}

void ServerComponent::loadConfig(ConfigFile &config)
{
}

void ServerComponent::processAnswer(Message *message)
{
}
