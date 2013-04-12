/*
 * StmSpawn.cpp
 *
 *  Created on: 31 janv. 2013
 *      Author: Hugo
 */


#include <stdlib.h>
#include <string>
#include <iostream>

#ifdef WIN32
#include <process.h>
#include <errno.h>
#else
#include <sys/mman.h>
#include <sys/wait.h>
#endif

#include <ticks.h>
#include "StmSpawn.h"
#include "util.h"

using namespace std;

StmSpawner::StmSpawner(ServerHub * hub, string path_to_py_server, ui32 port, string command_store) : hub(hub), path_to_py_server(path_to_py_server), port(port), path_to_command_store(command_store)
{
	init(1);
}

bool StmSpawner::check_stmloader_already_exists()
{
	int max_tries = 3;
	while(max_tries-- >= 0)
	{
		try
		{
			//give time to the machine to connect to the server
			syst_wait_ms(3000);
			Message msg_in,msg_out;
			msg_in.destination = MSG_TYPE_STM;
			msg_in.source = 0;
			msg_in.command = STM_PING;
			msg_in.append("ping");
			Message * answer = hub->callSync(&msg_in, &msg_out, 1000);
			if(answer == NULL)
			{
				throw string("no answer");
			}
			else
			{
				bool ok = (answer->read_string() == "ping");
				if(ok)
					cout << "StmSpawner found stmloader" << endl;
				else
					throw string("StmSpawner found stmloader but got wrong answer");
				return answer;
			}
		}
		catch(string exc)
		{
			cout << "StmSpawner found no stmloader, " << max_tries +1 << " to go : \n\t" << exc << endl;
		}
	}
	return false;
}

void StmSpawner::launch_stmloader()
{
	try
	{
		int ret;

		//todo detect if the stm machine is already running
		//todo pass listening port of the server by argument
		cout << "Creating stm server using py file '" << path_to_py_server.c_str() <<"' ..." << endl;
#ifdef WIN32
		//http://msdn.microsoft.com/en-us/library/20y988d2%28v=vs.71%29.aspx
		ret = _spawnlp(_P_WAIT,
				"python3","python3",path_to_py_server.c_str(),
				"-s", "\"localhost\"",
				"-p", my_itoa(port).c_str(),
				"-c", path_to_command_store.c_str(),
				NULL);
		if(ret == -1)
		{
			stringstream err;
			cout << "Failed to spawn stm process with path '" << path_to_py_server << "'" << endl << "\tErrno " << errno << endl << "\t";
			switch(errno)
			{
			case E2BIG:
				err << string("Argument list exceeds 1024 bytes"); break;
			case EINVAL:
				err <<  string("File or path is invalid"); break;
			case ENOENT:
				err <<  string("File or path is not found"); break;
			case ENOEXEC:
				err <<  string("Specified file is not executable or has invalid executable-file format"); break;
			case ENOMEM:
				err <<  string("Not enough memory is available to execute new process"); break;
			}
			throw err.str();
		}
		cout << "Server exited with code " << ret << endl;
#else
		switch(int pid=fork())
		{
		case -1:
			throw string("Error while forking");
		case 0:
			execl("python3", path_to_py_server.c_str(), NULL, NULL); break;
		default:
			waitpid(pid,NULL,0);
			cout << "stm_server spawned" << endl;
			break;
		}

#endif

	}
	catch(const string & exc)
	{
		cerr <<  "Could not create stm : \n\t" << exc << endl;
	}

}

void StmSpawner::step()
{
	if(!check_stmloader_already_exists())
		launch_stmloader();
}
