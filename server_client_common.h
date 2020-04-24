#ifndef _SRC_SERVER_CLIENT_COMMON_H_
#define _SRC_SERVER_CLIENT_COMMON_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>

#define SOCKET_ERROR (-1)
#define DOMAIN_FAMILY (AF_INET)
#define PORT (8084)
#define BUFFER_SIZE (1024)

void checkSocketErrorExit(int checkVal, const int errorVal, std::string err_msg);

#endif /*_SRC_SERVER_CLIENT_COMMON_H_*/