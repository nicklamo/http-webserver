#include "web-server.h"

int main(int argc, char *argv[]) {
    // define variables
    int sockfd, optval, port, connection, status;
    pid_t pid;
    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    
    // handle ctrl-c and child processes
    signal(SIGINT, sigint_wrapper);
    signal(SIGCHLD, SIG_IGN);
    
    // read command line arg
    if (argc != 2) err(1, "usage: ./web-server <port-number>");

    // build server
    port = atoi(argv[1]);
    sockfd = build_server(port); 

    // loop forever
    for ( ; ; ) {
        // accept incoming requests
        bzero((char *) &clientaddr, sizeof(clientaddr));
        if ((connection = accept (sockfd, (struct sockaddr *) &clientaddr, &clientlen)) < 0) err(1, "Error making connection. error is %d:", errno);

        // fork process to handle request
        if ((pid = fork()) == 0) { // child process
            // function to handle request
            handle_request(connection);
            exit(0);
        } 
        printf("Request from %s:%d handled by process %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), pid);
        close(connection);
    }
}


/* Build the server base on port number */
int build_server(int port_num) {
    // define variables
    int sockfd, optval, connection, status;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t clientlen;

    /* create parent socket */
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) err(1, "Error opening socket. errno is %d", errno);
    

    // setsockopt: configure socket to reuse address
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    // build server 
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port_num);

    // bind socket
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) err(1, "Error binding socket. error is %d:", errno);

    // listen for requests
    if (listen(sockfd, MAX_CONNECTIONS) < 0) err(1 ,"Error in listening. errno is %d", errno); 
    printf("Listening... \n");
    return sockfd;
}

void sigint_wrapper(int signal) { 
    printf("\nExiting... \n"); 
    fflush(stdout); 
    exit(0);
} 
