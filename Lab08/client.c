#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helpers.h"

void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, n, ret;
	struct sockaddr_in serv_addr;
	char buffer[BUFLEN];

	if (argc < 3) {
		usage(argv[0]);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	ret = inet_aton(argv[1], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	fd_set set, temp;
	int fdmax = sockfd;

	FD_ZERO(&set);
	FD_ZERO(&temp);

	FD_SET(STDIN_FILENO, &set);
	FD_SET(sockfd, &set);

	while (1) {
		temp = set;

		ret = select(fdmax + 1, &temp, NULL, NULL, NULL);

		if(FD_ISSET(STDIN_FILENO, &temp)){
			// se citeste de la tastatura
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);

			if (strncmp(buffer, "exit", 4) == 0) {
				break;
			}

			// se trimite mesaj la server
			n = send(sockfd, buffer, strlen(buffer), 0);
			DIE(n < 0, "send");
		}

		if(FD_ISSET(sockfd, &temp)){
			memset(buffer, 0, BUFLEN);
			int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
			DIE(bytes_received < 0, "eroare la receptie");
			puts(buffer);
			memset(buffer, 0, BUFLEN);
		}
	}

	close(sockfd);

	return 0;
}
