/*
 * ThreadedComponent.cpp
 *
 *  Created on: 13 mars 2011
 *      Author: hugo
 */
#include <stdlib.h>
#include <iostream>
#include "ServerComponent.h"
#include "TickMachine.h"

#include "RoboardServerInterface.hpp"

Message * ServerComponent::call(Message * msg_in)
{
	return call(msg_in,new Message());
};


Message *ServerComponent::safe_call(Message *msg_in, Message *msg_out)
{
	BEGIN_THREAD_SAFE
	Message *msg = call(msg_in, msg_out);
	unlock();
	return msg;
	END_THREAD_SAFE
}


ServerComponent::ServerComponent()
{
};

ServerComponent::~ServerComponent()
{
};
