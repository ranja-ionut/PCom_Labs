#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

unsigned char byte_parity(unsigned char x){
	unsigned char rez = 0;
	for(int i = 0; i<8; i++){
		if(x&(1<<i))
			rez^=1;
	}
	return rez;
}	

int main(int argc, char *argv[])
{
	msg t, ack;
	//int res,i;
	//int total =1;
	printf("[SENDER] Starting.\n");	
	init(HOST, PORT);

	printf("[SENDER]: BDP=%d\n", atoi(argv[1]));
	int bdp = atoi(argv[1]);
	int w = (bdp*1000) / (MSGSIZE*8);
	
	printf("Pachete: %i\n",w);
/*	
	for (i = 0; i < COUNT; i++) {
		// cleanup msg 
		*/memset(&t, 0, sizeof(msg));
		/*
		int fd = open(argv[2], O_RDONLY);
		if(!(t.len=read(fd, t.payload, MSGSIZE))){
			perror("N-a mers citirea frate.\n");
			return -1;
		}
		close(fd);
		
		// send msg
		res = send_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
		
		// wait for ACK
		res = recv_message(&t);
		if (res < 0) {
			perror("[SENDER] Receive error. Exiting.\n");
			return -1;
		}
	}
*/
	t.len=MSGSIZE;
	unsigned char par = 0;
	for(int i = 0;i<t.len;i++){
		par ^= byte_parity(t.payload[i]);
	}
	t.parity = par;
	printf("Par=%u\n",par);
	for(int i=0;i<w;i++){
		send_message(&t);
		//printf("Am trimis pachetu %i boss.\n",total++);
	}
	for(int i=0;i<COUNT-w;i++){
		recv_message(&ack);
		send_message(&t);
		//printf("Am trimis pachetu %i boss.\n",total++);
	}
	for(int i=0;i<w;i++){
		recv_message(&ack);
	}
	printf("[SENDER] Job done, all sent.\n");
		
	return 0;
}
