#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udp_server.h"

void diep(const char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    udp_struct udp;
    int i;
    int cnt=0;

    int msglen;
    char buf[512];
  
    if(udpserver_init(&udp,6444,1)) diep("udpServer_Init");
  
    while (1) {
	do {
	    msglen = udpserver_receive(&udp, buf, 512);
	    cnt++;
	} while(msglen<=0);
	printf("%s", buf);
    }
  
    udpserver_close(&udp);
    return 0;
}
