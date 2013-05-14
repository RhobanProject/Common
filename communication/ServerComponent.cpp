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

ServerComponentTask::ServerComponentTask(ServerComponent * component, Message *msg_in, Message *msg_out):
component(component), cleanup_msg_out(false)
{
	if(!msg_out)
	{
		this->msg_out = new Message();
		cleanup_msg_out = true;
	}
	this->msg_out->uid = msg_in->uid;
	this->msg_out->destination = msg_in->source;
	this->msg_out->source = msg_in->destination;
	this->msg_out->command = msg_in->command;
	this->msg_out->answer = true;

	//copy incoming message
	this->msg_in = *msg_in;

	//starts the thread
	start();
}

ServerComponentTask::~ServerComponentTask()
{
}

void ServerComponentTask::execute()
{
	component->process(&msg_in,msg_out);

	/* There wont be any answer */
	Message useless;
	component->call(msg_out,&useless);

	if(cleanup_msg_out)
		delete msg_out;
}


ServerComponent::~ServerComponent()
{
	delete_tasks();
}

Message *ServerComponent::doCall(Message *msg_in, Message *msg_out, bool sync, int timeout)
{
	if (respondTo(msg_in->destination)) {
		if (msg_in->answer) {
			processAnswer(msg_in);
			return NULL;
		}
		else
		{
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

			if(isAsync(msg_in))
			{
				 processAsync(msg_in);
				 return NULL;
			}
			else
			{
				Message * answer = process(msg_in, msg_out, sync, timeout);
				if (answer)
					answer->write_header();
				return answer;
			}

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


void ServerComponent::delete_tasks()
{
	string errors = "";
	BEGIN_SAFE(tasks_mutex)
		for(list<ServerComponentTask *>::iterator ptask = current_tasks.begin(); ptask!= current_tasks.end(); ptask++)
		{
			try
			{
				delete *ptask;
			}
			catch(string exc)
			{
				errors += "Failed to terminate task: " + exc + "\n";
			}
		}
	current_tasks.clear();
	END_SAFE(tasks_mutex)
	if(errors != "")
		throw errors;
}

void ServerComponent::cleanup_tasks()
{
	BEGIN_SAFE(tasks_mutex)
			do
			{
				list<ServerComponentTask *>::iterator ptask;
				for(ptask = current_tasks.begin(); ptask!= current_tasks.end(); ptask++)
					if((*ptask)->is_dead())
						break;
				if(ptask == current_tasks.end())
					break;
				else
				{
					delete *ptask;
					current_tasks.erase(ptask);
				}
			}
			while(1);
	END_SAFE(tasks_mutex)
}

Message * ServerComponent::processAsync(Message *msg_in)
{
	cleanup_tasks();
	BEGIN_SAFE(tasks_mutex)
	current_tasks.push_back(
			new ServerComponentTask(this,msg_in)
	);
	END_SAFE(tasks_mutex)
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
	processIncomingMessage(message);
}

bool ServerComponent::isConnected()
{
	return true;
}

void ServerComponent::getMessage(Message *)
{
}

void ServerComponent::sendMessage(Message *message)
{
	doCall(message, NULL, false, 0);
}
