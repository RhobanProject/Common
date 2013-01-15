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
		return process(msg_in,new Message());
	else if(hub)
		return hub->call(msg_in);
	else
		throw string("Cannot route message to destination ") + my_itoa(msg_in->destination);
};


