
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>  /* The main library that used in this program*/
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


void pcap_pkdescr(u_char*, const struct pcap_pkthdr *, const u_char *);
