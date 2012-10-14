#ifndef __UDP_H
#define __UDP_H
#include <netinet/in.h>

typedef struct 
{
    int s;
    struct sockaddr_in si_me;
    struct sockaddr_in si_other;
    socklen_t slen;
} udp_struct;

int udpserver_init(udp_struct *udp, int port, int blocking);
//returns size of packet received or -1 on error
int udpserver_receive(udp_struct *udp, char* buf, int len);
void udpserver_close(udp_struct *udp);


#endif

