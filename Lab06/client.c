/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	client mini-server de backup fisiere
*/

#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#include "helpers.h"

void usage(char*file)
{
	fprintf(stderr,"Usage: %s ip_server port_server file\n",file);
	exit(0);
}

/*127.0.0.1
*	Utilizare: ./client ip_server port_server nume_fisier_trimis
*/
int main(int argc,char**argv)
{
	if (argc!=4)
		usage(argv[0]);
	
	int fd;
	struct sockaddr_in to_station;
	char buf[BUFLEN];
	memset(&buf, 0, sizeof(buf));


	/*Deschidere socket*/
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	/* Deschidere fisier pentru citire */
	DIE((fd=open(argv[3],O_RDONLY))==-1,"open file");
	
	/*Setare struct sockaddr_in pentru a specifica unde trimit datele*/
	memset(&to_station, 0, sizeof(to_station));
	to_station.sin_family = AF_INET;
	to_station.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1], &to_station.sin_addr);
	
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din fisier
	*		trimite pe socket
	*/	
	int byteswrite;
	int count = 1;
	while(count > 0){	// citeste pana cand nu mai e nimic de citit
		count = read(fd, buf, sizeof(buf));
		DIE(count < 0, "eroare la citire din fisier");
		byteswrite = 
				sendto(sockfd, buf, count, 0, (struct sockaddr *) &to_station, sizeof(to_station));
		DIE(byteswrite < 0, "eroare la trimiterea datelor");
		memset(&buf, 0, sizeof(buf)); // clean buffer
		usleep(500);
	}

	/*Inchidere socket*/
	close(sockfd);
	
	/*Inchidere fisier*/
	close(fd);
	
	return 0;
}
