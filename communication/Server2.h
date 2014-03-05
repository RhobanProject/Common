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


class Server2 : public ServerHub, protected Thread
{
public:
	Server2(int port = 7777);

	~Server2();

	virtual void execute();

	virtual void cleanup();

	void wait(){ Thread::wait(); };

protected:

#ifdef WIN32
	static BOOL CtrlHandler(DWORD fdwCtrlType);
	static list<Server2 *> servers;
#endif

	int port;

};


}

#endif // SERVER_H
