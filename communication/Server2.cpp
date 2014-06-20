#include "Server2.h"
#include "Message.h"
#include <ticks.h>

#include <zhelpers.h>


#ifdef WIN32
#include <Windows.h>
#endif



#ifdef  WIN32
list<Rhoban::Server2 *> Rhoban::Server2::servers;
#endif

/* Waits until server stops */
void Rhoban::Server2::wait()
{
	Thread::wait();
}

void Rhoban::Server2::shutdown()
{
	Thread::kill();
}

/* Runs the server on the given port, if not started yet */
void Rhoban::Server2::run(int port, string name)
{
	if (Thread::thread_state == Thread::ThreadState::Unborn)
	{
		this->port = port;
		this->name = name;
		start();
		wait_started();
	}
}

Rhoban::Server2::Server2() : hub(NULL)
{
#ifdef WIN32
//	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
	servers.push_back(this);
#endif

}

Rhoban::Server2::Server2(ServerHub * hub) : hub(hub)
{
	if (hub == NULL)
		throw new std::runtime_error("Cannnot create server with null hub");
}

Rhoban::Server2::~Server2()
{
#ifdef WIN32
	servers.remove(this);
#endif
}

void Rhoban::Server2::execute()
{

	/* starts server */
	void *context = zmq_init(1);

	int major, minor, patch;
	zmq_version(&major, &minor, &patch);
	printf("0MQ version is %d.%d.%d\n", major, minor, patch);

	void *socket = zmq_socket(context, ZMQ_REP);
	char address[256];
	sprintf(address, "tcp://*:%d", port);

	int hwm = 20;
	zmq_setsockopt(socket, ZMQ_RCVHWM, &hwm,sizeof hwm);
	zmq_setsockopt(socket, ZMQ_SNDHWM, &hwm, sizeof hwm);

	cout << "Binding REP Socket to address " << address << endl;
	zmq_bind(socket, address);

	Message request;
	Message answer;

	Message length_error_message;
	length_error_message.destination = MSG_TYPE_ERROR;
	length_error_message.command = MSG_ERROR_COMMAND;

	Message empty_message;

	zmq_msg_t header_msg;
	zmq_msg_init(&header_msg);

	zmq_msg_t data_msg;
	zmq_msg_init(&data_msg);

	int message_nb = 0;

	while (is_alive())
	{
		try
		{
			zmq_msg_t zmsg;
			zmq_msg_init(&zmsg);

			//cout << "Waiting for message " << ++message_nb << endl;
		int ok = zmq_recvmsg(socket, &zmsg, 0);

			if (ok == -1)
			{
				string err;
				switch (errno)
				{
				case EAGAIN: err = "EAGAIN " ; break;
				case EFSM: err = "EFSM "; break;
				case ETERM: err = "ETERM "; break;
				case ENOTSOCK: err = "ENOTSOCK "; break;
				case EINTR: err = "EINTR "; break;
				case EFAULT: err = "EFAULT "; break;
				default: err = "Unknown error"; break;
				}
				throw runtime_error("Failed to recv message: " + err);
			}
			else
			{
				int size = zmq_msg_size(&zmsg);
				//cout << "Received " << size << " bytes" << endl;

				request.clear();
				Message * local_answer = NULL;
				if (size >= (int)MSG_HEADER_SIZE)
				{
					const char * data_ptr = (const char*)zmq_msg_data(&zmsg);

					request.read_header(data_ptr);
					request.write(data_ptr, size);
					answer.clear();
					answer.command = request.command;
					answer.destination = request.source;
					answer.source = request.destination;
					answer.uid = request.uid;
					//cout << "Computing answer..." << endl;
					local_answer = hub->call(&request, &answer);
					if (local_answer == NULL)
					{
						empty_message.command = request.command;
						empty_message.source = request.destination;
						empty_message.destination = request.source;
						local_answer = &empty_message;
					}
					//cout << "...done." << endl;
				}
				else
				{
					cout << "Error, message too short" << endl;
					local_answer = &length_error_message;
				}
				local_answer->write_header();
				int length = local_answer->getLength();
				//cout << "Sending answer with content length " << length << " and total size " << local_answer->getSize() << endl;


				zmq_msg_init_size(&header_msg, MSG_HEADER_SIZE);
				memcpy(zmq_msg_data(&header_msg), local_answer->getRaw(), MSG_HEADER_SIZE);

				zmq_msg_init_size(&data_msg, length);
				memcpy(zmq_msg_data(&data_msg), local_answer->getRaw() + MSG_HEADER_SIZE, length);
				if (zmq_sendmsg(socket, &header_msg, ZMQ_SNDMORE) == -1)
					throw std::runtime_error("Failed to send header through zmq socket");
				if (zmq_sendmsg(socket, &data_msg, 0) == -1)
					throw std::runtime_error("Failed to send answer data through zmq socket");
			}

			zmq_msg_close(&zmsg);
		}
		catch (const std::exception & exc)
		{
			cout << "Caught exception in zmq server2" << string(exc.what()) << endl;
			syst_wait_ms(500);
		}
		catch(const string & exc)
		{
			cout << "Caught exception in zmq server2" << exc << endl;
			syst_wait_ms(500);
		}
		catch(...)
		{
			cout << "Caught unknown exception in zmq server2" << endl;
			syst_wait_ms(500);
		}


	}

	cout << "Closing server" << endl;
	zmq_close(socket);
	zmq_term(context);
	zmq_msg_close(&data_msg);
	cout << "Server closed" << endl;

}

#ifdef WIN32
BOOL Rhoban::Server2::CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		cout << "Ctrl-C event" << endl;
		while (servers.size() > 0)
		{
			try
			{
				servers.front()->kill();
			}
			catch (std::exception exc)
			{
				cout << "Failed to kill server :" << exc.what() << endl;
			}
			catch (...)
			{
				cout << "Failed to kill server :" << endl;
			}
			servers.pop_front();
		}
	}

	return true;
}
#endif
