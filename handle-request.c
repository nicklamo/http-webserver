#include "handle-request.h"

/*********************************************************/
/*************** Handle incomming requests ***************/
/*********************************************************/
void handle_request(int connection) {
    int n;
    char buffer[BUFFER_SIZE], res[BUFFER_SIZE], command[LINE_SIZE], 
         file[LINE_SIZE], protocol[LINE_SIZE], err_content_header[LINE_SIZE];
    bzero(buffer, BUFFER_SIZE);

    // recieve message
    n=recv(connection, buffer, BUFFER_SIZE, 0);

    if ( n < 0) {
        err(1, "Error in recieves. error is %d:", errno);
    }
    else if (n == 0){
        err(1, "Connection closed suddenly %d:", errno);
    }
    else {
        // TODO parse the rest of the request
        sscanf(buffer, "%s %s %s", command, file, protocol);
        printf("Recieved request: %s, %s, %s \n", command, file, protocol);
        //printf("%s /n", buffer);
        // send response based on command
        if(!strcmp(command, "GET")){
            get_response(connection, file, protocol);
        }
        else if(!strcmp(command, "POST")){
            // TODO pass in the data to be saved into a file
            put_response(connection, file, protocol);
        }
        else {
            sprintf(err_content_header, "%s 500 Internal Server Error\r\n\r\n", protocol);
            send(connection, err_content_header, strlen(err_content_header), 0);
            printf("500 error server does not recognize commmand\n");
        } 
    }
    close(connection);
    return;
}

/*********************************************************/
/********** Send response for a  get request *************/
/*********************************************************/
void get_response(int connection, char * filename, char* protocol) {
    int filelen, bytes_sent;
    char final_path[LINE_SIZE], final_content_header[LINE_SIZE], err_content_header[LINE_SIZE];
    char *content_type;
    FILE * fp;
    if (!strcmp("/", filename)){
        strcat(filename, "index.html");
    }

    // generate file path 
    strcpy(final_path, "www");
    strcat(final_path, filename);

    fp = fopen(final_path, "r");
    if  ( fp == NULL )  { //file must exist
        sprintf(err_content_header, "%s 500 Internal Server Error\r\n\r\n", protocol);
        bytes_sent = send(connection, err_content_header, strlen(err_content_header), 0);
        printf("500 error could not find file %s \n", final_path);
        return;
    }
    // get file extension and content type 
    get_content_type(filename, &content_type);

    // read file into buffer
    fseek (fp, 0, SEEK_END);
    filelen = ftell(fp);
    fseek (fp, 0, SEEK_SET);
    char *buff = malloc(filelen+1);
    if (buff){
        fread (buff, 1, filelen, fp);
    }
    fclose (fp);

    // printf("Attemping to send file: %s with content type: %s of size: %d on connection: %d\n", final_path, content_type, filelen, connection);
    // format header and send header
    sprintf(final_content_header, "%s 200 OK\r\nContent-Length:%d\r\nContent-type: %s\r\n\r\n", protocol, filelen, content_type);
    send(connection, final_content_header, strlen(final_content_header), 0);
    // send data
    bytes_sent = send(connection, buff, filelen, 0);
    printf("File %s has sent %d bytes\n", final_path, bytes_sent);
    free(content_type);
    return;
}

/*********************************************************/
/************ Send respone for a put request *************/
/*********************************************************/
void put_response(int connection, char * filename, char * protocol) {
    char final_path[LINE_SIZE], err_header[LINE_SIZE];
    int bytes_sent;
    FILE * fp;
    // generate path to file
    strcpy(final_path, "www");
    strcat(final_path, filename);
    // if the file exists than don't put
    if ((fp = fopen(final_path, "r"))) {
        sprintf(err_header, "%s 500 Internal Server Error\r\n\r\n", protocol);
        bytes_sent = send(connection, err_header, strlen(err_header), 0);
        printf("This file already exists, cannot put\n");
        return;
    }
    printf("PUT request not implemented yet\n");
    sprintf(err_header, "%s 200 OK\r\n\r\n", protocol);
    bytes_sent = send(connection, err_header, strlen(err_header), 0);
    // read data into file and save file
    // fopen(final_path, "w")

    // send the file back to client
    // send(connection, header, strlen(header))
    // send(connection, data, datasize 0)
}

 /* get the content type based on the filename */
void get_content_type(char *filename, char **content_type){
    char* ext;
    int end, n;
    
    // get the extension
    end = strlen(filename)-1;
    n = end;
    while(filename[n] != '.') n--;
    ext = malloc(end-n);
    strcpy(ext,filename+n); 

    // return content_type based on extension
    *content_type = malloc(30);
    if (!strcmp( ext, ".html")){
        strcpy(*content_type, "text/html");
    }
    else if (!strcmp( ext, ".text")) {
        strcpy(*content_type, "text/plain");
    }
    else if (!strcmp( ext, ".css")){
        strcpy(*content_type, "text/css");
    }
    else if (!strcmp( ext, ".js")) {
        strcpy(*content_type, "application/javascript");
    }
    else if (!strcmp( ext, ".jpg")) {
        strcpy(*content_type, "image/jpg");
    }
    else if (!strcmp( ext, ".png")) {
        strcpy(*content_type, "image/png");
    }
    else if (!strcmp( ext, ".gif")) {
        strcpy(*content_type, "image/gif");
    }
    free(ext);
}