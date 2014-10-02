#ifndef ZMQ_CONTEXT_H
#define ZMQ_CONTEXT_H

namespace Rhoban
{
	/* ZMQ recommends one context per process */
	extern void * get_zmq_context();
}

#endif