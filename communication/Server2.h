#ifndef SERVER2_H
#define SERVER2_H

#include "rhobanProtocol.h"
#include <logging/log.h>
#include <threading/Thread.h>
#include <communication/ServerHub.h>

using namespace std;
using namespace Rhoban;


/**
 * Server log level
 * 1: Caution
 * 2: Messages
 * 3: Debug
 */
#define SERVER2_LOG_LEVEL 2

#define SERVER2_CAUTION(...)     LOG_CPP(1, SERVER2_LOG_LEVEL, "server:caution", __VA_ARGS__)
#define SERVER2_MSG(...)         LOG_CPP(2, SERVER2_LOG_LEVEL, "server", __VA_ARGS__)
#define SERVER2_DEBUG(...)       LOG_CPP(3, SERVER2_LOG_LEVEL, "server:debug", __VA_ARGS__)

namespace Rhoban
{


class Server2 : public Callable, protected Thread
{
public:

	/* Constructor of a server. To sart the server use run.*/
	Server2(ServerHub * hub);

	~Server2();

	/* Waits until server stops */
	void wait();

	/* kills server*/
	void shutdown();

	/* Runs the server on the given port with given name , if not started yet.
	Returns once the server has been created and started. */
	void run(int port, string name = "RhobanServer");


		/**
		* Handles incoming messages whose target is the server
		* itself, like ping, component tregistration, etc...
		* */
		virtual Message *callSync(Message *msg_in, Message *msg_out, int timeout = 1000);

		const ui16 virtual DestinationID() const { return MSG_TYPE_SERVER; }


protected:

	Server2();

	ServerHub * hub;

	virtual void execute();

#ifdef WIN32
	static BOOL CtrlHandler(DWORD fdwCtrlType);
	static list<Server2 *> servers;
#endif

	int port;
	string name;

};


}

#endif // SERVER_H
