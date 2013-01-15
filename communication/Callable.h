/*
 * Callable.h
 *
 *  Created on: 15 janv. 2013
 *      Author: Hugo
 */

#ifndef CALLABLE_H_
#define CALLABLE_H_

#include "Message.h"

using namespace Rhoban;

namespace  Rhoban
{
class Callable
{
public:
	Callable(){};
	virtual ~Callable(){};

	/*!
	 * a callable object process a message and outputs an answer
	 * the caller is in charge of disposing the returned message
	 */
	virtual Message * call(Message * msg_in){ return call(msg_in, new Message()); };

	/*!
	 * if a msg_out is given,
	 * they the component will use it
	 * to place the answer message
	 *
	 * in case large messages are output frequently,
	 * this can be used to save ressources:
	 * memory allocation is done once for all by the caller
	 * and the same msg out is used again and again
	 *
	 */

	virtual Message * call(Message * msg_in, Message * msg_out) = 0;
};

}


#endif /* CALLABLE_H_ */
