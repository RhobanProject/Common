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
#include <threading/TimedThread.h>
#include <communication/Server.h>

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
