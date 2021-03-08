#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <bits/ioctls.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>

#include "arp.h"
#include "utils.h"

#define TIMEOUT_SEC 5
#define MAX_TTL 30


void init_eth_hdr(int sockfd, struct ether_header *eth_hdr)
{
	uint32_t dg_addr = get_default_gateway_ip();
	arp_resolve(sockfd, dg_addr, eth_hdr->ether_dhost);
	get_interface_mac(sockfd, IFNAME, eth_hdr->ether_shost);
	eth_hdr->ether_type = htons(ETHERTYPE_IP);
}

void ping(int sockfd, struct in_addr ip_addr, int count)
{
	struct packet pkt;
	init_packet(&pkt);

	struct ether_header *eth_hdr = (struct ether_header *)pkt.payload;
	struct iphdr *ip_hdr = (struct iphdr *)(pkt.payload + IP_OFF);
	struct icmphdr *icmp_hdr = (struct icmphdr *)(pkt.payload + ICMP_OFF);

	/* Init Ethernet header. */
	init_eth_hdr(sockfd, eth_hdr);

	char *ip_s = inet_ntoa(ip_addr);
	printf("PING %s ...\n", ip_s);

	/* TODO 2 implementați un ciclu astfel încât să dați ping de count ori. */
	for(unsigned char i = 0; (i < count) || (count == -1); i++){
		/* TODO 1 Dacă vreți ca pachetul de ICMP să conțină date in
		 * payload, updatați dimensiunea totală a pachetului). */
		pkt.size = sizeof(struct ether_header) + sizeof(struct iphdr)
			+ sizeof(struct icmphdr);

		/* TODO 1 Inițializați headerul de IPv4 cu informațiile necesare.
		 * Calculați checksum. Folosiți funcția 'checksum' din utils.h
		 */
		ip_hdr->version = 4;
		ip_hdr->ihl = 5;
		ip_hdr->tos = 0;
		ip_hdr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
		ip_hdr->id = htons(getpid());
		ip_hdr->frag_off = 0;
		ip_hdr->ttl = 64;
		ip_hdr->protocol = IPPROTO_ICMP;
		get_interface_ip(sockfd, IFNAME, &ip_hdr->saddr);
		memcpy(&ip_hdr->daddr, &ip_addr.s_addr, sizeof(ip_addr.s_addr));
		ip_hdr->check = 0;
		ip_hdr->check = checksum(ip_hdr, sizeof(struct iphdr));

		/* TODO 1 Inițializați headerul de ICMP cu informațiile necesare.
		 * Calculați checksum. Folosiți funcția 'checksum' din utils.h
		 */
		icmp_hdr->code = 0;
		icmp_hdr->type = ICMP_ECHO;
		icmp_hdr->un.echo.id = htons(getpid());
		icmp_hdr->un.echo.sequence = htons(i);
		icmp_hdr->checksum = 0;
		icmp_hdr->checksum = checksum(icmp_hdr, sizeof(struct icmphdr));

		struct packet reply;
		double time_elapsed = time_packet(sockfd, &pkt, &reply);

		// normalize to miliseconds
		time_elapsed = time_elapsed * 1000;

		struct iphdr *rip_hdr = (struct iphdr *)(reply.payload + IP_OFF);
		struct icmphdr *ricmp_hdr = (struct icmphdr *)(reply.payload + ICMP_OFF);

		/* TODO 1 afișați informațiile relevante din pachetul reply */
		if(time_elapsed < 0){
			printf("TIMEOUT\n");
		}
		else{
			printf("%li bytes from %s: icmp_seq=%i ttl=%i time=%f ms\n",
				pkt.size, ip_s, icmp_hdr->un.echo.sequence / 256, ip_hdr->ttl, time_elapsed);
		}
	}
}

void traceroute(int sockfd, struct in_addr ip_addr)
{
	struct packet pkt;
	init_packet(&pkt);

	struct ether_header *eth_hdr = (struct ether_header *)pkt.payload;
	struct iphdr *ip_hdr = (struct iphdr *)(pkt.payload + IP_OFF);
	struct icmphdr *icmp_hdr = (struct icmphdr *)(pkt.payload + ICMP_OFF);

	/* Init Ethernet header. */
	init_eth_hdr(sockfd, eth_hdr);

	char *ip_s = inet_ntoa(ip_addr);
	printf("TRACEROUTE %s ...\n", ip_s);

	/* TODO 3 Implementați funcționalitatea de traceroute. Porniți de la
	 * funcționalitatea implementată în funcția de ping. */
	for(unsigned char j = 1; j <= MAX_TTL; j++){
		pkt.size = sizeof(struct ether_header) + sizeof(struct iphdr)
			+ sizeof(struct icmphdr);

		ip_hdr->version = 4;
		ip_hdr->ihl = 5;
		ip_hdr->tos = 0;
		ip_hdr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
		ip_hdr->id = htons(getpid());
		ip_hdr->frag_off = 0;
		ip_hdr->ttl = j;
		ip_hdr->protocol = IPPROTO_ICMP;
		get_interface_ip(sockfd, IFNAME, &ip_hdr->saddr);
		memcpy(&ip_hdr->daddr, &ip_addr.s_addr, sizeof(ip_addr.s_addr));
		ip_hdr->check = 0;
		ip_hdr->check = checksum(ip_hdr, sizeof(struct iphdr));

		icmp_hdr->code = 0;
		icmp_hdr->type = ICMP_ECHO;
		icmp_hdr->un.echo.id = htons(getpid());
		icmp_hdr->un.echo.sequence = htons(j);
		icmp_hdr->checksum = 0;
		icmp_hdr->checksum = checksum(icmp_hdr, sizeof(struct icmphdr));

		struct packet reply;
		double time_elapsed = time_packet(sockfd, &pkt, &reply);

		// normalize to miliseconds
		time_elapsed = time_elapsed * 1000;

		struct iphdr *rip_hdr = (struct iphdr *)(reply.payload + IP_OFF);
		struct icmphdr *ricmp_hdr = (struct icmphdr *)(reply.payload + ICMP_OFF);

		struct in_addr path;
		memcpy(&path, &rip_hdr->saddr, sizeof(path));
		ip_s = inet_ntoa(path);


		if(ricmp_hdr->type == 0 && ricmp_hdr->code == 0){
			printf("%i Reached destination %s: %f ms\n", j, ip_s, time_elapsed);
			break;
		}
		if(time_elapsed < 0){
			printf("%i %s: %f ms\n", j, ip_s, time_elapsed);
		}
		else{
			printf("%i %s: %f ms\n", j, ip_s, time_elapsed);
		}
	}


}

int get_sock(const char *ifname)
{
	int res;
	int sockfd = socket(AF_PACKET, SOCK_RAW, 768);
	DIE(sockfd == -1, "socket");

	struct ifreq intf;
	strcpy(intf.ifr_name, ifname);
	res = ioctl(sockfd, SIOCGIFINDEX, &intf);
	DIE(res == -1, "ioctl SIOCGIFINDEX");

	struct sockaddr_ll addr;
	memset(&addr, 0, sizeof(addr));
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = intf.ifr_ifindex;

	struct timeval tv;
	tv.tv_sec = TIMEOUT_SEC;
	tv.tv_usec = 0;
	res = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	DIE(res == -1, "setsockopt");

	res = bind(sockfd, (struct sockaddr *)&addr , sizeof(addr));
	DIE(res == -1, "bind");
	return sockfd;
}

void show_usage(const char *pname)
{
	fprintf(stderr, "Usage:\n"
			"%1$s ping <ip> [<count>]\n"
			"OR\n"
			"%1$s traceroute <ip>\n", pname);
	exit(1);
}

enum run_mode {
	PING,
	TRACEROUTE,
};

enum run_mode validate(int argc, char **argv)
{
	int v = 1;
	enum run_mode mode;
	if (!strcmp(argv[1], "ping")) {
		mode = PING;
	} else if (!strcmp(argv[1], "traceroute")) {
		mode = TRACEROUTE;
	} else {
		show_usage(argv[0]);
	}

	if (!(argc == 3 || (mode == PING && argc == 4)))
		show_usage(argv[0]);

	return mode;
}

int main(int argc, char **argv)
{
	int res;
	enum run_mode mode = validate(argc, argv);

	char *ip_str;
	ip_str = dns_lookup(argv[2]);
	DIE(ip_str == NULL, "dns_lookup");
	struct in_addr ip_addr;
	res = inet_aton(ip_str, &ip_addr);
	DIE(!res, "inet_aton");

	int sockfd = get_sock(IFNAME);
	int count = -1;

	switch (mode) {
	case PING:
		if (argc == 4)
			count = atoi(argv[3]);

		ping(sockfd, ip_addr, count);
		break;
	case TRACEROUTE:
		traceroute(sockfd, ip_addr);
		break;
	}

	return 0;
}
