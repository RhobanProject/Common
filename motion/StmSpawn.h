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
#include <threading/Thread.h>

using namespace std;

class StmSpawner : public Thread
{
public:
	StmSpawner(string path_to_py_server, ui32 port, string command_store) : path_to_py_server(path_to_py_server), port(port), path_to_command_store(command_store) {};

	virtual void execute(void);

private:
	string path_to_py_server;
	ui32 port;
	string path_to_command_store;
};

#endif /* STMSPAWN_H_ */
