#ifndef LIB
#define LIB

#define MAX_LEN 1400
#define LEN 150
typedef struct {
  int len;
  char fn[LEN];
  char payload[MAX_LEN];
} msg;

void init(char* remote,int remote_port);
void set_local_port(int port);
void set_remote(char* ip, int port);
int send_message(const msg* m);
int recv_message(msg* r);

#endif

