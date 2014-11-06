#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <iostream>

#include <string.h>
#include <set>

#include <xml/XMLTools.h>
#include <util.h>

#include <threading/Thread.h>

#include "ServerHub.h"


namespace Rhoban
{

	ServerHub::ServerHub(bool thread_safe) : thread_safe(thread_safe), fallbackComponent(NULL)
	{
	}

	ServerHub::~ServerHub()
	{
		removeAllComponents();
	}

	Message* ServerHub::call(Message* msg_in, Message * msg_out)
	{
		return doCall(msg_in, msg_out, false, 0);
	}

	Message* ServerHub::callSync(Message* msg_in, Message * msg_out, int timeout)
	{
		return doCall(msg_in, msg_out, true, timeout);
	}

	Message* ServerHub::doCall(Message* msg_in, Message * msg_out, bool sync, int timeout)
	{
		Callable *component;
		ui16 comp_nb = msg_in->destination;

		component = getComponent(comp_nb);

		if (component == NULL) {
			//SERVER_CAUTION("Unable to find the component " << comp_nb);
			//SERVER_CAUTION("Null component: " << comp_nb);
			return NULL;
		}

		Message * answer = 0;
		if (thread_safe) {
			Mutex * mut = mutexes[comp_nb];
			BEGIN_SAFE((*mut))
			if (sync) {
				answer = component->callSync(msg_in, msg_out, timeout);
			}
			else {
				answer = component->call(msg_in, msg_out);
			}
			END_SAFE((*mut))
		}
		else {
			if (sync) {
				answer = component->callSync(msg_in, msg_out, timeout);
			}
			else {
				answer = component->call(msg_in, msg_out);
			}
		}

		return answer;
	}

	
	/**
	* Registering a new component, specifying the type
	*/
	void ServerHub::registerComponent(ServerComponent *component)
	{
		registerComponent(component->DestinationID(), component);
		component->setHub(this);
	}

	void ServerHub::registerComponent(ui16 type, Callable *component)
	{
		removeComponent(type);
		BEGIN_SAFE(components_mutex);
		components[type] = component;
		mutexes[type] = new Mutex();
		if (type == 0)
			fallbackComponent = component;
		END_SAFE(components_mutex);
	}

	

	/**
	* Registering a new internal component
	*/
	Callable *ServerHub::getComponent(ui16 type)
	{
		Callable * comp = NULL;
		BEGIN_SAFE(components_mutex);
		map<ui16, Callable *>::iterator it = components.find(type);

		
		if (it != components.end())
			comp = it->second;
		END_SAFE(components_mutex);

		if (comp != NULL)
			return comp;
		else
			return fallbackComponent;
	}

	/**
	* Registering a new internal component
	*/
	void ServerHub::removeComponent(ui16 type)
	{
		BEGIN_SAFE(components_mutex);
		map<ui16, Callable *>::iterator it = components.find(type);
		if (it != components.end() && it->first != MSG_TYPE_SERVER) {
			//delete it->second;
			SERVER_MSG("Unregistering component " << type);
			components.erase(it);
		}

		mutexes.erase(type);
		END_SAFE(components_mutex);
	}


	void ServerHub::removeAllComponents()
	{
		BEGIN_SAFE(components_mutex)
		for (map<ui16, Callable *>::iterator it = components.begin(); it != components.end(); it++)
		if (it->first != MSG_TYPE_SERVER && it->second != NULL)
		{
			delete it->second;
			it->second = 0;
		}
		END_SAFE(components_mutex)
	}

	/**
	* Remove a component
	*/
	void ServerHub::removeComponent(Callable *component)
	{
		vector<ui16> types;
		map<ui16, Callable*>::iterator it;

		BEGIN_SAFE(components_mutex);
		for (it = components.begin(); it != components.end(); it++) {
			if (component == it->second) {
				types.push_back((*it).first);
			}
		}

		for (vector<ui16>::iterator vit = types.begin(); vit != types.end(); vit++) {
			components.erase(*vit);
			mutexes.erase(*vit);
		}
		END_SAFE(components_mutex);

	}

	/**
	* Returns the registered components
	*/
	vector<ui16> ServerHub::getComponents()
	{
		vector<ui16> result;
		BEGIN_SAFE(components_mutex);
		for (map<ui16, Callable*>::iterator it = components.begin(); it != components.end(); it++)
			result.push_back(it->first);
		END_SAFE(components_mutex);

		return result;
	}

}
