#ifndef HANDLE_REQUEST
#define HANDLE_REQUEST
/* Include headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>
#include <err.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "handle-request.h"



/* define globals and functions*/
#define MAX_CONNECTIONS 100
int build_server(int port_num);
void sigint_wrapper(int signal);


#endif