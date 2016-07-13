#include <stdio.h>
#include <libnet.h>
#include <pcap/pcap.h>
#include <stdint.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

#define PROMISCUOUS 1
#define NONPROMISCUOUS 0

//==================handler=========================
void callback(u_char *useless ,const struct pcap_pkthdr *pkthdr ,const u_char *packet)
{

	struct libnet_ethernet_hdr *e_hdr = (struct libnet_ethernet_hdr *)packet;
	unsigned short e_type = ntohs(e_hdr->ether_type);
	uint8_t *Mac_s; 
	uint8_t *Mac_d;

	if(e_type == ETHERTYPE_IP)
	{
		struct libnet_ipv4_hdr *i_hdr = (struct libnet_ipv4_hdr *)(packet + 14);	//14byte offset
		printf("Src Address : %s \n",inet_ntoa(i_hdr->ip_src) );	
		printf("Dst Address : %s \n",inet_ntoa(i_hdr->ip_dst) );
		
		if(i_hdr->ip_p == IPPROTO_TCP)
		{
			struct libnet_tcp_hdr *t_hdr;
			t_hdr = (struct libnet_tcp_hdr *)(packet+14+(i_hdr->ip_hl*4));			//34byte offset
			printf("Src Port : %u\n",(u_short)ntohs(t_hdr->th_sport));
			printf("Dst Port : %u\n",(u_short)ntohs(t_hdr->th_dport));
		
			Mac_s = (uint8_t *)(e_hdr->ether_shost);
			Mac_d = (uint8_t *)(e_hdr->ether_dhost);
			printf("Mac Src: %02x:%02x:%02x:%02x:%02x:%02x\n",Mac_s[0],Mac_s[1],Mac_s[2],Mac_s[3],Mac_s[4],Mac_s[5]);
			printf("Mac Dst: %02x:%02x:%02x:%02x:%02x:%02x\n",Mac_d[0],Mac_d[1],Mac_d[2],Mac_d[3],Mac_d[4],Mac_d[5]);
		}
	}

	else
	{
		printf("NONE IP packet\n");
	}
	printf("====================\n");
	printf("\n");
}

int main(int argc, char **argv)
{
	char *dev;
	char *net;
	char *mask;

	bpf_u_int32 netp;
	bpf_u_int32 maskp;
	char errbuf[PCAP_ERRBUF_SIZE];
	int ret;
	struct pcap_pkthdr hdr;
	struct in_addr net_addr, mask_addr;
	struct ether_header *eptr;
	const u_char *packet;

	struct bpf_program fp;

	pcap_t *pcd;

	dev = pcap_lookupdev(errbuf);
	if(dev == NULL)
	{
		printf("%s\n",errbuf);
		exit(1);
	}

	ret = pcap_lookupnet(dev,&netp,&maskp,errbuf);
	if(ret == -1)
	{
		printf("%s\n",errbuf);
		exit(1);
	}

	pcd = pcap_open_live(dev,BUFSIZ,NONPROMISCUOUS,-1,errbuf);
	if(pcd == NULL)
	{
		printf("%s\n",errbuf);
		exit(1);
	}

	if(pcap_compile(pcd,&fp,argv[2],0,netp)==-1)
	{
		printf("compile error\n");
		exit(1);
	}

	if(pcap_setfilter(pcd,&fp)==-1)
	{
		printf("setfilter error\n");
		exit(0);
	}
	pcap_loop(pcd,atoi(argv[1]),callback,NULL);
}


