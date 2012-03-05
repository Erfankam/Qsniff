#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>  /* The main library that used in this program*/
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/* #define _BSD_SOURCE 1 */



/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6
/* Ethernet header */
struct sniff_ethernet {
	u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
	u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
	u_short ether_type; /* IP? ARP? RARP? etc */
	
};	
	
	
	
//#define PPPOE_HDRLEN 6	
/* PPPOE header */
/*struct sniff_pppoe {
	u_int16_t length;
	u_int16_t msg_type;
	u_int32_t magic_cookie;
	u_int16_t ctrl_msg_type;
	u_int16_t reserved0;
};
*/
	
	
	

/* IP header */
struct sniff_ip {
	u_char ip_vhl;		/* version << 4 | header length >> 2 */
	u_char ip_tos;		/* type of service */
	u_short ip_len;		/* total length */
	u_short ip_id;		/* identification */
	u_short ip_off;		/* fragment offset field */
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* dont fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
	u_char ip_ttl;		/* time to live */
	u_char ip_p;		/* protocol */
	u_short ip_sum;		/* checksum */
	struct in_addr ip_src,ip_dst; /* source and dest address */
};
/* ethernet headers are always exactly 14 bytes */
#define SIZE_ETHERNET 14


#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

typedef	u_int32_t tcp_seq;
/* TCP header */
struct sniff_tcp {
	u_short th_sport;	/* source port */
	u_short th_dport;	/* destination port */
	tcp_seq th_seq;		/* sequence number */
	tcp_seq th_ack;		/* acknowledgement number */
	u_char th_offx2;	/* data offset, rsvd */
#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
	u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
	u_short th_win;		/* window */
	u_short th_sum;		/* checksum */
	u_short th_urp;		/* urgent pointer */
};



void pcap_pkdescr(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet)	 {
		
	const struct sniff_ethernet *ethernet; /* The ethernet header */
	const struct sniff_ip *ip; /* The IP header */
	const struct sniff_tcp *tcp; /* The TCP header */
	const u_char *payload; /* Packet payload */

	u_int size_ip;
	u_int size_tcp;

	//printf("1:done");
	ethernet = (struct sniff_ethernet*)(packet);
	printf("ether_type value = %x\n", ntohs(ethernet->ether_type));
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
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET );
	//printf("3:done");
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}
	
	//printf("4:done");
	tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET +  size_ip);
	size_tcp = TH_OFF(tcp)*4;
	if (size_tcp < 20) {
		printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
		return;
	}
	//printf("5:done");
	
	payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
	//printf("6:done");
	fprintf(stdout, "payload FROM address \"%s\" with MAC \"%x\" TO \
	   address \"%s\" with MAC \"%x\" contain :%s>>\n", \
		inet_ntoa(ip->ip_src), ether_ntoa(ethernet->ether_shost), \
		 inet_ntoa(ip->ip_dst), ether_ntoa(ethernet->ether_dhost), payload);
	//fflush(stdout);
};


int main (int argc, char * argv[]) {
	
	
	char *dev;  						/* The device to sniff on */
	char errbuf[PCAP_ERRBUF_SIZE];		/* Error string */
	pcap_t * descr;						/* Session handle */
	struct bpf_program fp;				/* compiled filter */
	char filter_exp[] = "port 80";		/* filter expresion */
	bpf_u_int32 maskp;					/* subnet mask address  */
	bpf_u_int32 netp;					/* ip network address */
	/*struct pcap_pkthdr pcaphdr; 		 :pcap.h -> packet header of pcap */
	u_char *packet;						/* The actual packet */
	/*struct pcap_stat statp;				 pcap status structure */
	
	
	if (argc < 2) {
		printf("with out any interface ....>\n");
		/* defprintf("1:done");ine the device */
		if ((dev = pcap_lookupdev(errbuf)) == NULL) {
			fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
			return(2);
		}
	}
	else
		dev = argv[1];


	
	/* find the properties of the device */
	if (pcap_lookupnet(dev, &netp, &maskp, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device : %s\n", errbuf);
		netp = 0;
		maskp = 0;
		return(2);
	}
			
	/* open the session in promiscuous mode */
	if ((descr = pcap_open_live(dev, BUFSIZ, 0, 3000,errbuf)) == NULL) {
		fprintf(stderr, "coudn't open the session to sniff on dev \"%s\"  :>> %s\n", dev, errbuf);
		return(2);
	}
	
	/* compiled with the given filter ...non-optimized */
	if (pcap_compile(descr, &fp, filter_exp, 0, netp) == -1) {
		fprintf(stderr, "Coudn't parse filter \"%s\" :%s\n", filter_exp, pcap_geterr(descr));
		return(2);
	}
	
	
	/*apply the filter */
	if (pcap_setfilter(descr,&fp) == -1) {
		fprintf(stderr, "Coudn't install filter \"%s\" :%s\n", filter_exp, pcap_geterr(descr));
		return(2);
	}
	/*and at last:live capture of network */
	switch(pcap_loop(descr, atoi(argv[1]), pcap_pkdescr, packet)) {
		case (-1) : 
			fprintf(stderr, "There was an occured during capturing :%s \n", pcap_geterr(descr));
			return(2);
		
		case (-2) : 
			fprintf(stderr, "Capturing aborted by user or by any command.\n");
			return(0);
		
		case (0) : 
			fprintf(stderr, "counting exhausted >> number of packets sniffed :\n");
			pcap_close(descr);
			return(0);
	}
	return(0);

}
