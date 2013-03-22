
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>  /* The main library that used in this program*/
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Including protocols headers */
#include "ip_hdr.h"
#include "tcp_hdr.h"
#include "ethernet_hdr.h"


void sniffer_l1(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet) {

		
	const struct ethernet_hdr *ethernet; /* The ethernet header */
	const struct ip_hdr *ip; /* The IP header */
	const struct tcp_hdr *tcp; /* The TCP header */
	const u_char *payload; /* Packet payload */
    pcap_t* descr = (pcap_t*) args;

	u_int size_ip;
	u_int size_tcp;

	//printf("1:done");
	ethernet = (struct ethernet_hdr*)(packet);
	printf("ether_type value = %x\n", ntohs(ethernet->ether_type));
	printf("ether_type value = %s\n", ethernet->ether_dhost);
	printf("ether_type value = %s\n", ethernet->ether_shost);
	printf("pcap_datalink value = %s\n", pcap_datalink_val_to_name(pcap_datalink(descr)));
	printf("pcap_datalink value = %s\n", pcap_datalink_val_to_description(pcap_datalink(descr)));
	printf("pcap_pkthdr->len = %i\n", pkthdr->len);
	printf("pcap_pkthdr->len = %i\n", pkthdr->caplen);
	switch (ntohs(ethernet->ether_type)) {
		case (0x0800):fprintf(stdout, "ether_type = IP\n");break;
		case (0x0806):fprintf(stdout, "ether_type = ARP\n");break;
		case (0x8035):fprintf(stdout, "ether_type = RARP\n");break;
		case (0x880b):fprintf(stdout, "ether_type = PPP\n");break;
		case (0x8863):fprintf(stdout, "ether_type = PPPOED\n");break;
		case (0x8864):fprintf(stdout, "ether_type = PPPOES\n");break;
		default:fprintf(stdout, ">>>>>There is no any known protocol ancapsulated in ETHERNET\n");
	}	
		
	//printf("2:done");  
	ip = (struct ip_hdr*)(packet + SIZE_ETHERNET );
	//printf("3:done");
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}
	
	//printf("4:done");
	tcp = (struct tcp_hdr*)(packet + SIZE_ETHERNET +  size_ip);
	size_tcp = TH_OFF(tcp)*4;
	if (size_tcp < 20) {
		printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
		return;
	}
	//printf("5:done");
	
	payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
	//printf("6:done");
	fprintf(stdout, "payload FROM address \"%s\" with MAC \"%X\" TO \
	   address \"%s\" with MAC \"%X\" contain :%s>>\n", \
		inet_ntoa(ip->ip_src), ether_ntoa(ethernet->ether_shost), \
		 inet_ntoa(ip->ip_dst), ether_ntoa(ethernet->ether_dhost), payload);
	//fflush(stdout);
    ip = NULL;
}


