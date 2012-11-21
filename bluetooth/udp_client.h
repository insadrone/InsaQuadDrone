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

int udpclient_init(udp_struct *udp, const char* host, int port);
int udpclient_send(udp_struct *udp, char* buf, int len);
void udpclient_close(udp_struct *udp);

#endif

