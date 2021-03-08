#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    memcpy(line, HOST, HOST_SIZE);
    strcat(line, host);
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
       for(int i = 0; i < cookies_count; i++){
            memset(line, 0, LINELEN);
            memcpy(line, COOKIE, COOKIE_SIZE);
            strcat(line, cookies[i]);
            compute_message(message, line);
       }
    }
    // Step 4: add final new line
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
                           char **cookies, int cookies_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host
    memset(line, 0, LINELEN);
    memcpy(line, HOST, HOST_SIZE);
    strcat(line, host);
    compute_message(message, line);

    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    memset(line, 0, LINELEN);
    memcpy(line, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
    sprintf(&line[CONTENT_LENGTH_SIZE], "%li", strlen(body_data));
    compute_message(message, line);

    memset(line, 0, LINELEN);
    memcpy(line, CONTENT_TYPE, CONTENT_TYPE_SIZE);
    strcat(line, content_type);
    compute_message(message, line);

    // Step 4 (optional): add cookies
     if (cookies != NULL) {
       for(int i = 0; i < cookies_count; i++){
            memset(line, 0, LINELEN);
            memcpy(line, COOKIE, COOKIE_SIZE);
            strcat(line, cookies[i]);
            compute_message(message, line);
       }
    }
    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    memset(line, 0, LINELEN);
    memcpy(line, body_data, strlen(body_data));
    compute_message(message, line);

    free(line);
    return message;
}
