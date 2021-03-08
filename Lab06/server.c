/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	mini-server de backup fisiere
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
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd;

	if (argc!=3)
		usage(argv[0]);
	
	struct sockaddr_in my_sockaddr, from_station ;
	char buf[BUFLEN];
	memset(&buf, 0, sizeof(buf));

	/*Deschidere socket*/
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	/*Setare struct sockaddr_in pentru a asculta pe portul respectiv */
	my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_port = htons(8080);
	my_sockaddr.sin_addr.s_addr = INADDR_ANY;
	
	/* Legare proprietati de socket */
	int enable = 1;
	
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
		perror("setsocketopt");
		exit(1);
	}
	
	bind(sockfd, (struct sockaddr*) &my_sockaddr, sizeof(my_sockaddr));
	
	socklen_t station_len = sizeof(from_station);
	
	/* Deschidere fisier pentru scriere */
	DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");
	
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din socket
	*		pune in fisier
	*/
	int bytesread = 1;
	int count;
	while(bytesread > 0){ // cand am citit tot recvfrom ar trebui sa dea 0 si programul sa se inchida
		bytesread = 
			recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &from_station, &station_len);
		DIE(bytesread < 0, "eroare la citirea datelor");
		count = write(fd, buf, bytesread); // scrie in fisier atat cat s-a putut citi
		DIE(count < 0, "eroare la scrierea in fisier");
		memset(&buf, 0, sizeof(buf));
	}


	/*Inchidere socket*/	
	close(sockfd);
	
	/*Inchidere fisier*/
	close(fd);
	
	return 0;
}
