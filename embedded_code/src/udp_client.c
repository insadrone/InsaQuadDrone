#include <sys/time.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "udp_client.h"

int udpclient_init(udp_struct *udp, const char* host, int port)
{
    udp->slen=sizeof(udp->si_other);
  
    if ((udp->s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) return 1;
  
    memset((char *) &udp->si_other, 0, sizeof(udp->si_other));
    udp->si_other.sin_family = AF_INET;
    udp->si_other.sin_port = htons(port);
    if (inet_aton(host, &udp->si_other.sin_addr)==0) return 2;

    return 0;
}

int udpclient_send(udp_struct *udp, char* buf, int len)
{
    if (sendto(udp->s, buf, len, 0, (const struct sockaddr *)&udp->si_other, udp->slen)==-1) return 1;
    return 0;
}

void udpclient_close(udp_struct *udp)
{
    close(udp->s);
}
