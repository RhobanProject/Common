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
 * component.h
 *
 *  Created on: 12 mars 2011
 *      Author: hugo
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <string>
#include <logging/log.h>
#include <configfile/ConfigFile.h>

#include "Callable.h"
#include <threading/Thread.h>

using namespace Rhoban;

/**
 * Component log level
 * 1: Caution
 * 2: Messages
 * 3: Debug
 */
#define COMPONENT_LOG_LEVEL 3

#define COMPONENT_CAUTION(...)     LOG_CPP(1, COMPONENT_LOG_LEVEL, "component:caution", __VA_ARGS__)
#define COMPONENT_MSG(...)         LOG_CPP(2, COMPONENT_LOG_LEVEL, "component", __VA_ARGS__)
#define COMPONENT_DEBUG(...)       LOG_CPP(3, COMPONENT_LOG_LEVEL, "component:debug", __VA_ARGS__)

/*
 * A component can be called
 *
 * Typically, the components are called by clients of the server
 * The server itself is a component
 */
namespace Rhoban
{

class ServerComponentTask;

    /*
     * A server component is callable.
     * It encapsulates a hub to connect to other components.
     */
    class ServerComponent : public Callable
    {
    	friend ServerComponentTask;

        public:
            ServerComponent() : hub(0){};
            virtual ~ServerComponent(){};

            /* The destination id of this component */
            const ui16 virtual DestinationID() const = 0;

            Message *doCall(Message *msg_in, Message *msg_out, bool sync, int timeout);

            /* IF the message can be processed by this component then process is used */
            /* otherwise the message is routed to the hub */
            Message *call(Message *msg_in, Message *msg_out);

            Message *callSync(Message *msg_in, Message *msg_out, int timeout);

            /* sets the hub used to connect to other components */
            void setHub(Callable * hub);


            /**
             * Reads the components config from the config file
             */
            virtual void loadConfig(ConfigFile &config);

            /**
             * Does the server component responds to this id ?
             */
            virtual bool respondTo(ui16 id);

        protected:
            /* Connection to other components */
            Callable * hub;

            virtual Message * process(Message * msg_in, Message * msg_out, bool sync = false, int timeout = 0)=0;
            virtual void processAnswer(Message * answer);

    };

    /*
     * This creates a parallel task to process a message and sends the answer in an asynchronous way
     */
    	class ServerComponentTask :  public Thread
    	{
    	public:
    		ServerComponentTask(ServerComponent * component, Message *msg_in, Callable * hub);


    	protected:
    		void execute();

    	private:
    		ServerComponent * component;
    		Message *msg_in;
    		Callable * hub;

    	};

}

#endif /* COMPONENT_H_ */
