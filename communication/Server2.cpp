#include "Server2.h"
#include "Message.h"
#include <ticks.h>

#include <zhelpers.h>


#ifdef WIN32
#include <Windows.h>
#endif



void Rhoban::Server2::cleanup()
{
	/* delete all components */
	removeAllComponents();
}

#ifdef  WIN32
list<Rhoban::Server2 *> Rhoban::Server2::servers;
#endif

Rhoban::Server2::Server2(int port)
: port(port)
{

#ifdef WIN32
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
	servers.push_back(this);
#endif
	start();
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

	cout << "Binding REP Socket to address " << address << endl;
	zmq_bind(socket, address);

	Message request;
	Message answer;

	Message length_error_message;
	length_error_message.destination = MSG_TYPE_ERROR;
	length_error_message.command = MSG_ERROR_COMMAND;

	zmq_msg_t header_msg;
	zmq_msg_init(&header_msg);

	zmq_msg_t data_msg;
	zmq_msg_init(&data_msg);

	zmq_msg_t empty_msg;
	zmq_msg_init(&empty_msg);
	zmq_msg_init_size(&empty_msg, 0);

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
//				cout << "Received " << size << " bytes" << endl;

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
					local_answer = call(&request, &answer);
				}
				else
				{
					local_answer = &length_error_message;
				}
				if (local_answer != NULL)
				{
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
				else
				{
					if (zmq_sendmsg(socket, &empty_msg, 0) == -1)
						throw std::runtime_error("Failed to send empty message through zmq socket");
				}
			}

			zmq_msg_close(&zmsg);
		}
		catch (std::exception exc)
		{
			cout << exc.what() << endl;
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