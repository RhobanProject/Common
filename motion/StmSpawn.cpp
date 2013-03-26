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

void StmSpawner::execute()
{
	while(true)
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
		syst_wait_ms(500);
	}
}
