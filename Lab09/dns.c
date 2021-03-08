// Protocoale de comunicatii
// Laborator 9 - DNS
// dns.c

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int usage(char* name)
{
	printf("Usage:\n\t%s -n <NAME>\n\t%s -a <IP>\n", name, name);
	return 1;
}

// Receives a name and prints IP addresses
void get_ip(char* name)
{
	int ret;
	struct addrinfo hints, *result;
	struct in_addr *p1;
	char ip4[INET_ADDRSTRLEN];
	struct in6_addr *p2;
	char ip6[INET6_ADDRSTRLEN];

	// TODO: set hints
	hints.ai_flags = 0;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_addrlen = 0;
	hints.ai_addr = 0;
	hints.ai_canonname = 0;
	hints.ai_next = 0;
	// TODO: get addresses
	ret = getaddrinfo(name, NULL, &hints, &result);
	if(ret < 0){
		gai_strerror(ret);
	}
	// TODO: iterate through addresses and print them
	while(result != NULL){
		switch(result->ai_family){
			case AF_INET:
				p1 = &((struct sockaddr_in *) result->ai_addr)->sin_addr;
				inet_ntop (result->ai_family, p1, ip4, sizeof(ip4));
				printf("IPv4 address: %s\n", ip4);
				break;
			case AF_INET6:
				p2 = &((struct sockaddr_in6 *) result->ai_addr)->sin6_addr;
				inet_ntop (result->ai_family, p2, ip6, sizeof(ip6));
				printf("IPv6 address: %s\n", ip6);
		}
		
		result = result->ai_next;
	}
	// TODO: free allocated data
	freeaddrinfo(result);
}

// Receives an address and prints the associated name and service
void get_name(char* ip)
{
	int ret;
	struct sockaddr_in addr;
	char host[1024];
	char service[20];

	// TODO: fill in address data
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	inet_aton(ip, &addr.sin_addr);

	// TODO: get name and service
	ret = getnameinfo((struct sockaddr*)&addr, (socklen_t) sizeof(addr), host,(socklen_t) 1024, service, (socklen_t) 20, 0);
	if(ret < 0){
		gai_strerror(ret);
	}
	// TODO: print name and service

	printf("Name: %s Service: %s\n", host, service);
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		return usage(argv[0]);
	}

	if (strncmp(argv[1], "-n", 2) == 0) {
		get_ip(argv[2]);
	} else if (strncmp(argv[1], "-a", 2) == 0) {
		get_name(argv[2]);
	} else {
		return usage(argv[0]);
	}

	return 0;
}
