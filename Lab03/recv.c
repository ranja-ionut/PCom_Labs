#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

unsigned char byte_parity(unsigned char x){
	unsigned char rez = 0;
	for(int i = 0; i<8; i++){
		if(x&(1<<i))
			rez^=1;
	}
	return rez;
}	
int main(void)
{
	msg r;
	int i, res;
	int bune=0, rele=0;
	printf("[RECEIVER] Starting.\n");
	init(HOST, PORT);
	
	for (i = 0; i < COUNT; i++) {
		/* wait for message */
		res = recv_message(&r);
		//printf("Mersi boss am primit pachetu.\n");
		
		unsigned char par = 0;
		for(int j = 0;j<r.len;j++){
			par ^= byte_parity(r.payload[j]);
		}
		
		printf("Par=%u si r.parity=%u.\n",par,r.parity);
		
		if(par!=r.parity){
			rele++;
		}else
			bune++;
		if (res < 0) {
			perror("[RECEIVER] Receive error. Exiting.\n");
			return -1;
		}
		/*
		int fd = open("output", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if(!write(fd, r.payload, r.len)){
			perror("N-a mers scrierea frate.\n");
			return -1;
		}
		close(fd);
		*/
		/* send dummy ACK */
		res = send_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Send ACK error. Exiting.\n");
			return -1;
		}
	}
	printf("Au fost trimise %i mesaje bune si %i mesaje rele.\n",bune,rele);
	printf("[RECEIVER] Finished receiving..\n");
	return 0;
}
