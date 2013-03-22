#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>  /* The main library that used in this program*/
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sniffer_l1.h"


/* Prototyping of sniffer level 1 */
void sniffer_l1(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);

int sniffer_l0(int argc, char * argv[]) {
	
	char *dev;  						/* The device to sniff on */
	char errbuf[PCAP_ERRBUF_SIZE];		/* Error string */
	pcap_t * descr;						/* Session handle */
	struct bpf_program fp;				/* compiled filter */
	char filter_exp[] = "port 80";		/* filter expresion */
	bpf_u_int32 maskp;					/* subnet mask address  */
	bpf_u_int32 netp;					/* ip network address */
	/*struct pcap_pkthdr pcaphdr; 		 :pcap.h -> packet header of pcap */
	//u_char *packet;						/* The actual packet */
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

	switch(pcap_loop(descr, -1, sniffer_l1, (u_char*)descr)) {

	    //printf("pcap_datalink = %i\n", pcap_datalink(descr));
	    ///printf("pcap_datalink = %i\n", pcap_datalink(descr));
        //return 0;
		case (-1) : 
			fprintf(stderr, "There was an error occured during capturing :%s \n", pcap_geterr(descr));
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
int main(int argc, char** argv) {
    sniffer_l0(argc, argv);
}
