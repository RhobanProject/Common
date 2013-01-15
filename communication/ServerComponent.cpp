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

Message * ServerComponent::call(Message * msg_in, Message *msg_out)
{


	if(msg_in->destination == DestinationID())
	{
		if(!msg_out)
			msg_out = new Message();
		else
			msg_out->clear();
		msg_out->destination = msg_in->source;
		msg_out->source = msg_in->destination;
		msg_out->command = msg_in->command;
		msg_out->uid = msg_in->uid;
		Message * answer = process(msg_in,msg_out);
		if(answer)
			answer->write_header(answer->buffer);
		return answer;
	}
	else if(hub)
	{
		return hub->call(msg_in,msg_out);
	}
	else
	{
		throw string("Cannot route message to destination ") + my_itoa(msg_in->destination);
	}
};


