#ifndef _HELPERS_
#define _HELPERS_

#define BUFLEN 4096
#define LINELEN 1000
#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)
#define CONTENT_TYPE "Content-Type: "
#define CONTENT_TYPE_SIZE (sizeof(CONTENT_TYPE))
#define HOST "Host: "
#define HOST_SIZE (sizeof(HOST))
#define COOKIE "Cookie: "
#define COOKIE_SIZE (sizeof(COOKIE))
#define SET_COOKIE "Set-Cookie: "
#define SET_COOKIE_SIZE (sizeof(SET_COOKIE))

// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

#endif
