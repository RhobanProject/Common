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

#include "Callable.h"

using namespace Rhoban;

/*
 * A component can be called
 *
 * Typically, the components are called by clients of the server
 * The server itself is a component
 */
namespace Rhoban
{

/*
 * A server component is callable.
 * It encapsulates a hub to connect to other components.
 */
class ServerComponent : public Callable
{

public:
	ServerComponent() : hub(0){};
	virtual ~ServerComponent(){};

	/* The destination id of this component */
	const ui16 virtual DestinationID() const = 0;

	/* IF the message can be processed by this component then process is used */
	/* otherwise the message is routed to the hub */
	Message * call(Message * msg_in, Message * msg_out);

	/* sets the hub used to connect to other components */
	void setHub(Callable * hub){this->hub = hub; };

protected:
	/* Connection to other components */
	Callable * hub;

	virtual Message * process(Message * msg_in, Message * msg_out)=0;

};
}


#endif /* COMPONENT_H_ */
