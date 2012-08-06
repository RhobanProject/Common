/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <sys/time.h>
#include <sys/types.h>


#include <stdlib.h>
#include <iostream>
#include <string>
#include "string.h"

#include <errno.h>
#include <time.h>

#include "util.h"
#include "Encodings.h"

#include "Client.h"
#include "Message.h"
#include "Server.h"

using namespace std;

#define FD_ZERO2(p)     memset((p), 0, sizeof(*(p)))

namespace Rhoban
{
    Client::Client()
    {
        sock = -1;
        connected = 0;
        header_buffer.alloc(MSG_HEADER_SIZE);

        for(int i =0;i < MSG_TYPE_MAX_NB; i++)
        {
            in_msgs.push_back(new Message());
            out_msgs.push_back(new Message());
        }
    }

    Client::~Client()
    {
        for(int i =0;i < MSG_TYPE_MAX_NB; i++)
        {
            if(in_msgs[i])
            {
                delete(in_msgs[i]);
                in_msgs[i] = 0;
            }
            if(out_msgs[i])
            {
                delete(out_msgs[i]);
                out_msgs[i] = 0;
            }
        }
    }
    Message * Client::read_one_msg(uint timeout_ms)
    {
        fd_set rdfs;
        struct timeval tv;
        FD_ZERO2(&rdfs);
        FD_SET(sock, &rdfs);

        int res = 0;
        if(timeout_ms)
        {
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = 1000 * (timeout_ms % 1000);
            res = select(sock + 1, &rdfs, NULL, NULL, &tv);
        }
        else
            res = select(sock + 1, &rdfs, NULL, NULL, NULL);

        if(res == 0)
        {
            SERVER_DEBUG_MSG("Client " << this << " waited " << timeout_ms <<"ms without receiving any message",1);
            return NULL;
        }
        else if( res == -1)
        {
            SERVER_CAUTION_MSG("Select error");
            connected = false;
#ifdef _WIN32
            int err = WSAGetLastError();
            throw string("Client : select error (WSA error code "+my_itoa(err)+ ")");
#else
            throw string ("Client : select error");
#endif
        }
        else
            return  read_one_msg_now();
    }

    Message * Client::read_one_msg_now()
    {
        int received = 0;
        int size = MSG_HEADER_SIZE;
        char * p = header_buffer.buffer;
        SERVER_DEBUG_MSG("Client trying to read one message from socket "<< sock,2);
        do
        {
            received = recv(sock, p, size, 0);
            if(!received)
            {
                string msg = "Client socket closed (waiting for header but received 0 byte)";
                SERVER_DEBUG_MSG(msg,1);
                throw msg;
            }
            if(received == -1)
            {
                string msg = "Client socket closed (Rcv failure while waiting for headers)";
                SERVER_DEBUG_MSG(msg,1);
                throw msg;
            }
            p += received;
            size -= received;
        } while(received > 0 && size > 0);

        if(size>0)
            throw string("Failed to receive entire header, "+ my_itoa(size)+" bytes missing");

        ui32 type = Encodings::decode_uint(header_buffer.buffer + Header::destination_offset); // Destination

        if(type>MSG_TYPE_MAX_NB)
            throw string("Msgs type too large "+my_itoa(type));

        Message * in_msg = in_msgs[type];
        in_msg->clear();
        in_msg->read_header(header_buffer.buffer);
        size= in_msg->length;

        if(size)
        {
            in_msg->alloc(size + MSG_HEADER_SIZE);
            in_msg->setSize(size + MSG_HEADER_SIZE);
            char * p = in_msg->buffer + MSG_HEADER_SIZE;

            do
            {
                received = recv(sock, p, size, 0);
                p += received;
                size -= received;
            } while(received > 0 && size > 0);

            if(received == -1)
            {
                //throw string("Rcv failure received header but socket error while receiving message");
                throw string("Rcv failure (received header and still "+my_itoa(size-1)+"/"+my_itoa(in_msg->length)+" to receive).");
                //return 0;
            }
            if(size>0)
            {
                //throw string("Failed to receive entire message.");
                throw string("Failed to receive entire message, "+ my_itoa(size)+" bytes missing over "+my_itoa(in_msg->length)+" in total.");
                //return 0;
            }
        }

        return in_msg;
    }


    void Client::init_connection(const char *address, int port)
    {
        if(sock!=0) close(sock);
        sock = socket(AF_INET, SOCK_STREAM, 0);
        SOCKADDR_IN sin = { 0 };
        struct hostent *hostinfo;

        if(sock == INVALID_SOCKET)
        {
            connected = false;
            throw string("Could not create socket");
        }

        hostinfo = gethostbyname(address);
        if (hostinfo == NULL)
        {
            connected = false;
            throw string("Unknown host " + string(address));
        }

        sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
        sin.sin_port = htons(port);
        sin.sin_family = AF_INET;

        if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
        {
            connected = false;
            throw string("Could not connect");
        }
        connected = true;
    }

    void Client::send_msg(Message *msg)
    {
        send_msg(msg, sock);
    }


    void Client::send_msg(Message *msg, SOCKET sock)
    {
        if(!msg) return ;

        msg->length = msg->getSize() - MSG_HEADER_SIZE;
        msg->write_header(msg->buffer);

        SERVER_DEBUG_MSG("Client --> message l" << msg->length << " d"<< msg->destination << " c"<< msg->command << "("<<msg->uid  <<" --> remote "<<sock,3);
        char * p = msg->buffer;
        int size = msg->getSize();
        int ret=0;
        do
        {
            ret = send(sock ,p ,size,0);
            if(ret<=0) break;
            size -=ret;
            p+= ret;
        } while(size > 0 && ret>0);

        if(ret<=0 || size>0)
            throw string("failed to send msg");
        if(ret<0)
            throw string("send error while sending message, "+ my_itoa(size)+" bytesmissing over "+my_itoa(msg->length)+" in total.");
        if(size>0)
            throw string("failed to send full message, "+ my_itoa(size)+" bytes missing over "+my_itoa(msg->length)+" in total.");

    }

    void Client::shutdown()
    {
        if(connected)
        {
            connected = false;
            SERVER_DEBUG_MSG("Client shutting down itself "<< (intptr_t) this,1);
            close(sock);
            sock = 0;
        }
    }

    void Client::log_error(string str)
    {
        SERVER_CAUTION_MSG(str);
    }
}
