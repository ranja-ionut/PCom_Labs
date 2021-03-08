#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "buffer.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;
        
    // Principal 1
    printf("Exercitiul principal 1: \n\n");
    message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                  "/api/v1/dummy",
                                  NULL, NULL, 0);
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n\n\n\n", response);

    // Principal 2
    printf("Exercitiul principal 2: \n\n");
    message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                  "/api/v1/dummy",
                                  "application/x-www-form-urlencoded",
                                  "a=5&b=6&c=7",
                                  NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n\n\n\n", response);

    // Principal 3
    printf("Exercitiul principal 3: \n\n");
    message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                  "/api/v1/auth/login",
                                  "application/x-www-form-urlencoded",
                                  "username=student&password=student",
                                  NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n\n\n\n", response);

    buffer buf = buffer_init();
    buffer_add(&buf, response, strlen(response));

    char *cookie = calloc(LINELEN + 10, sizeof(char));

    sscanf(&response[buffer_find(&buf, SET_COOKIE, SET_COOKIE_SIZE - 1) + SET_COOKIE_SIZE - 1], "%s", cookie);
    cookie[strlen(cookie) - 1] = '\0';

    // Principal 4
    printf("Exercitiul principal 4: \n\n");
    message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                  "/api/v1/weather/key",
                                  NULL, &cookie, 1);
    
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n\n\n\n", response);

    // BONUS 1
    printf("Exercitiul BONUS 1: \n\n");
    message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                  "/api/v1/auth/login",
                                  "application/x-www-form-urlencoded",
                                  "username=student&password=student",
                                  &cookie, 1);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n\n\n\n", response);


    // BONUS 2
    printf("Exercitiul BONUS 2: \n\n");
    int sockfd2;
    message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                  "/data/2.5/weather",
                                  "lat=36&lon=39&appid=b912dd495585fbf756dc6d8f415a7649", NULL, 0);
    sockfd2 = open_connection("188.166.16.132", 80, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd2, message);
    response = receive_from_server(sockfd2);
    printf("%s\n\n\n\n", response);
    

    // Principal 5
    printf("Exercitiul principal 5: \n\n");
    message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
                                  "/api/v1/auth/logout",
                                  NULL, NULL, 0);
    
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n\n\n\n", response);

    free(cookie);
    close_connection(sockfd);
    close_connection(sockfd2);
    return 0;
}
