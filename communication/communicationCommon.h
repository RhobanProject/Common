/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <sys/types.h>

#ifdef _WIN32
#ifndef MSVC
#include <Winsock.h>
#else
#endif
#else
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SOCKET int
#endif

#ifndef MSVC
#include <unistd.h>
#endif

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#endif // SERVER_COMMON_H
