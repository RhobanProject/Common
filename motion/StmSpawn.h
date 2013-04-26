/*
 * StmSpawn.h
 *
 *  Created on: 31 janv. 2013
 *      Author: Hugo
 */

#ifndef STMSPAWN_H_
#define STMSPAWN_H_

#include <stdlib.h>
#include <string>
#include <logging/log.h>
#include <threading/TimedThread.h>
#include <communication/Server.h>

/**
 * Server log level
 * 1: Caution
 * 2: Messages
 * 3: Debug
 */
#define STM_LOG_LEVEL 2

#define STM_CAUTION(...)     LOG_CPP(1, STM_LOG_LEVEL, "server:caution", __VA_ARGS__)
#define STM_MSG(...)         LOG_CPP(2, STM_LOG_LEVEL, "server", __VA_ARGS__)
#define STM_DEBUG(...)       LOG_CPP(3, STM_LOG_LEVEL, "server:debug", __VA_ARGS__)

using namespace std;

class StmSpawner : public SlowTimedThread
{
public:
	StmSpawner(ServerHub * hub, string path_to_py_server, ui32 port, string command_store);

	virtual void step(void);

private:

	bool check_stmloader_already_exists();
	void launch_stmloader();

	string path_to_py_server;
	ui32 port;
	string path_to_command_store;
	ServerHub * hub;
};

#endif /* STMSPAWN_H_ */
