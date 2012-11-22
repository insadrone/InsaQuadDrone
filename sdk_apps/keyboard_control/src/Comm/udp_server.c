#include <sys/time.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "udp_server.h"

int udpserver_init(udp_struct *udp, int port, int blocking)
{
    udp->slen=sizeof(udp->si_other);
 
    if ((udp->s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) return 1;
 
    if(blocking==0) {
	// Set socket to be non-blocking. All of the sockets for
	// the incoming connections will also be non-blocking since
	// they will inherit that state from the listening socket.
	int x;
	x = fcntl(udp->s, F_GETFL, 0);
	fcntl(udp->s, F_SETFL, x | O_NONBLOCK);
    }
 
    //Bind the socket
    memset((char *) &udp->si_me, 0, sizeof(udp->si_me));
    udp->si_me.sin_family = AF_INET;
    udp->si_me.sin_port = htons(port);
    udp->si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(udp->s, (const struct sockaddr*)&udp->si_me, sizeof(udp->si_me))==-1) return 3;

    return 0;
}

//returns size of packet received or returns -1 and errno=EWOULDBLOCK if no data available
int udpserver_receive(udp_struct *udp, char* buf, int len)
{
    return recvfrom(udp->s, buf, len, 0, (struct sockaddr*)&udp->si_other, &udp->slen);
}
  
void udpserver_close(udp_struct *udp)
{
    close(udp->s);
}
