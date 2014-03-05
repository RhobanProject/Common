#ifndef SERVER_HUB_H
#define SERVER_HUB_H


#include "Callable.h"
#include "rhobanProtocol.h"
#include "ServerComponent.h"

/**
* Server log level
* 1: Caution
* 2: Messages
* 3: Debug
*/
#define SERVER_LOG_LEVEL 2

#define SERVER_CAUTION(...)     LOG_CPP(1, SERVER_LOG_LEVEL, "server:caution", __VA_ARGS__)
#define SERVER_MSG(...)         LOG_CPP(2, SERVER_LOG_LEVEL, "server", __VA_ARGS__)
#define SERVER_DEBUG(...)       LOG_CPP(3, SERVER_LOG_LEVEL, "server:debug", __VA_ARGS__)

/**
*  The ServerHub encapsulates all components of the server
*  and routes requests to the right components,
*  and create the component if needed
*
*  it is given by the server to its internal clients
*  so they handle requests
*/

namespace Rhoban
{

	class ServerHub : public Callable
	{
	public:
		/**
		* Creates a thread save server
		*/
		ServerHub(bool thread_safe = true);

		/**
		* Delete the server hub
		*/
		virtual ~ServerHub();

		/**
		* Registering a component to the server
		*/
		void registerComponent(ServerComponent *component);

		/**
		* Registering a component by giving the id
		*/
		void registerComponent(ui16 type, ServerComponent *component);

		/**
		* Removing a component
		*/
		void removeComponent(ServerComponent *component);

		/**
		* Retreive a component
		*/
		ServerComponent *getComponent(ui16 type);

		/*
		* Whether a component has been registered
		*/
		bool hasComponent(ui16 type){ return getComponent(type) != fallbackComponent; };

		/**
		* Removes the component from the hub
		*/
		void removeComponent(ui16 type);

		/*
		* Remove all components
		*/
		void removeAllComponents();

		/**
		* Run a call
		*/
		Message *doCall(Message *msg_in, Message *msg_out, bool sync, int timeout);
		Message *call(Message * msg_in, Message * msg_out);
		Message *callSync(Message * msg_in, Message * msg_out, int timeout);

		/**
		* Returns the components
		*/
		vector<ServerComponent *> getComponents();

		/**
		* Component to use when no other one exists
		*/
		ServerComponent *fallbackComponent;

	protected:
		/**
		* The internal client process the message
		*/
		void processMessage(Message * msg);

		/**
		* Components
		*/
		map<ui16, Callable *> components;
		map<ui16, Mutex *> mutexes;

		bool thread_safe;
		Mutex components_mutex;
	};

}
#endif