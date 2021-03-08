#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10001


int main(int argc,char** argv){
  msg r,t;
  init(HOST,PORT);

  if (recv_message(&r)<0){
    perror("Receive message");
    return -1;
  }

  //for(int i=0;i<4;i++){
	  printf("[recv] Got msg with payload: <%s>, sending ACK...\n", r.payload);
	  char *new_name=malloc(LEN);
	  strcpy(new_name,"copy_");
	  strcat(new_name,r.fn);
	  FILE *f=fopen(new_name,"w");
	  fwrite(r.payload,sizeof(char),r.len-1,f);
	  fclose(f);
	  // Send ACK:
	  sprintf(r.payload,"%s", "ACK");
	  r.len = strlen(r.payload) + 1;
	  send_message(&r);
	  printf("[recv] ACK sent\n");
	  recv_message(&t);
  //}
  return 0;
}
