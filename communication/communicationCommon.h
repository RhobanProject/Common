#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <sys/types.h>

#ifdef _WIN32
#include <Winsock.h>
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

#include <unistd.h>

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#endif // SERVER_COMMON_H
