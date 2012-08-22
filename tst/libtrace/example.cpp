#include <libtrace.h>
#include <iostream>
using namespace std;

void per_packet(libtrace_packet_t* pkt) {
	
	void *transport;
	uint32_t remaining;
    libtrace_tcp_t *tcp;
	uint8_t proto;
	  
    transport = trace_get_transport(pkt, &proto, &remaining);
    if (transport == NULL)
		return;
		
    if (proto == 6) // TCP protocol num
    {
		tcp = (libtrace_tcp_t *) transport;
		
		if (ntohs(tcp->source) == 80)
			cout << "\n< TCP packet Recieved :D" ; 
		
		if (ntohs(tcp->dest) == 80)
			cout << "\n> TCP packet Sent :D";
	}
}

int main (int argc, char* argv[])
{
	libtrace_t* trace = NULL;
	libtrace_packet_t* packet = NULL;
	
	packet = trace_create_packet();
	trace = trace_create(argv[1]);
	
	if (trace_start(trace) == -1) 
	{
		cout << "\n[Err]\n";
		return 1;
	}
	
	while (trace_read_packet(trace, packet)>0) {
		per_packet(packet);
	}
	
	trace_destroy(trace);
	trace_destroy_packet(packet);
	return 0;
}
