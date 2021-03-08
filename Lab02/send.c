#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10000


int main(int argc,char** argv){
  init(HOST,PORT);
  msg t, p;
  if(argc==1){
	exit(-1);
  }
  
  for(int i=1; i<argc;i++){
	  FILE *f=fopen(argv[1],"r");
	  t.len=fread(t.payload,sizeof(char),1400,f)+1;
	  
		  //t.payload[strlen(t.payload)]='\0';
		  //Send message:
		printf("[send] Sending %s...\n",argv[1]);
		
		strcpy(t.fn,argv[1]);
		send_message(&t);
		//recv_message(&p);
  }
  
  // Check response:
  if (recv_message(&t)<0){
    perror("Receive error ...");
    return -1;
  }
  else {
    printf("[send] Got reply with payload: %s\n", t.payload);
  }

  return 0;
}
