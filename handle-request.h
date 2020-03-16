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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "web-server.h"

/* define globals and functions*/
#define BUFFER_SIZE 20000
#define MAX_FILE_SIZE 65535
#define LINE_SIZE 1024
void handle_request(int connection);
void get_response(int connection, char * filename, char * protocol);
void put_response(int connection, char * file, char * protocol);
void get_content_type(char *ext, char **content_type);

#endif