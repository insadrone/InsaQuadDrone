#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udp_client.h"

void diep(const char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    udp_struct udp;
    int i;

    int msglen;
    char buf[512];
  
    if(udpclient_init(&udp,"192.168.1.3",6444)) diep("udpclient_init");
  
    for (i=0; i<10; i++) {
	printf((char*)"Sending packet %d\n", i);
	msglen=sprintf(buf, (char*)"This is packet %d\n", i);
	if (udpclient_send(&udp, buf, msglen)) diep("send");
	usleep(100000);
    }
  
    udpclient_close(&udp);
    return 0;
}
