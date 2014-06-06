
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>  /* The main library that used in this program*/
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "mysql/mysql.h"

/* Including protocols headers */
#include "ip_hdr.h"
#include "tcp_hdr.h"
#include "ethernet_hdr.h"

#include "sniff_sql.h"


void sniffer_l1(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet) {

		
	const struct ethernet_hdr *ethernet; /* The ethernet header */
	const struct ip_hdr *ip; /* The IP header */
	const struct tcp_hdr *tcp; /* The TCP header */
	const u_char *payload; /* Packet payload */
	MYSQL *con = (MYSQL*) args;

	u_int size_ip;
	u_int size_tcp;

	
	
	//printf("1:done");
	ethernet = (struct ethernet_hdr*)(packet);
	printf("ether_type value = %x\n", ntohs(ethernet->ether_type));
	printf("ether_dhost value = %s\n", ethernet->ether_dhost);
	printf("ether_shost value = %s\n", ethernet->ether_shost);
	printf("pcap_pkthdr->len = %i\n", pkthdr->len);
	printf("pcap_pkthdr->len = %i\n", pkthdr->caplen);
	char ptype[6];
	switch (ntohs(ethernet->ether_type)) {
		case (0x0800):fprintf(stdout, "ether_type = IP\n"); strcpy(ptype,"IP"); break;
		case (0x0806):fprintf(stdout, "ether_type = ARP\n"); strcpy(ptype,"ARP"); break;
		case (0x8035):fprintf(stdout, "ether_type = RARP\n"); strcpy(ptype,"RARP"); break;
		case (0x880b):fprintf(stdout, "ether_type = PPP\n"); strcpy(ptype,"PPP"); break;
		case (0x8863):fprintf(stdout, "ether_type = PPPOED\n"); strcpy(ptype,"PPPOED"); break;
		case (0x8864):fprintf(stdout, "ether_type = PPPOES\n"); strcpy(ptype,"PPPOES"); break;
		default:fprintf(stdout, ">>>>>There is no any known protocol ancapsulated in ETHERNET\n");
	}
	char ethr_query[70];
	sprintf(ethr_query, "INSERT INTO ethernet_hdr VALUES('%s','%s','%s')", ethernet->ether_dhost, ethernet->ether_shost, ptype);
	
	
	printf ("query: %s", ethr_query);
	if (mysql_query(con, ethr_query)) {
		printf ("Insert Into Table Ethernet Failed %s.\n", mysql_error(con));
	}
	
	//printf("2:done");  
	ip = (struct ip_hdr*)(packet + SIZE_ETHERNET );
	//printf("3:done");
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}
	printf("ip_total Length = %i\n", ip->ip_vhl);
	printf("ip_type_serveice value = %i\n", ip->ip_tos);
	printf("ip_length value = %i\n", ntohs(ip->ip_len));
	printf("ip_id value = %i\n", ntohs(ip->ip_id));
	printf("ip_fragment_offcet value = %i\n", ntohs(ip->ip_off));
	printf("ip_time_to_live value = %i\n", ip->ip_ttl);
	printf("ip_protocol value = %i\n", ip->ip_p);
	printf("ip_cecksum value = %i\n", ntohs(ip->ip_sum));
	char ip_query[70];
	// , ntohs(ip->ip_len), ntohs(ip->ip_id), ntohs(ip->ip_off), ip->ip_ttl, ip->ip_p, ntohs(ip->ip_sum)
	sprintf(ip_query, "INSERT INTO ip_hdr VALUES(%i,%i,%i,%i,%i,%i,%i,%i)", ip->ip_vhl, ip->ip_tos, ntohs(ip->ip_len), ntohs(ip->ip_id), ntohs(ip->ip_off), ip->ip_ttl, ip->ip_p, ntohs(ip->ip_sum));
	printf ("query: %s", ip_query);
	if (mysql_query(con, ip_query)) {
		printf ("Insert Into Table Ethernet Failed %s.\n", mysql_error(con));
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


