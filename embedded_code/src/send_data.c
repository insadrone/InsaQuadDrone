#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udp_client.h"
#include "read_arduino.h"

void diep(const char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    udp_struct udp;
    int i;
    int res;

    int msglen;
    char tty_buf[255];
    char udp_buf[512];
    
    int *fd = malloc(sizeof(int));

    if(udpclient_init(&udp,"192.168.1.3",6444)) diep("udpclient_init");
  
    init_term(fd);
    printf("term opened\n");
    
    while (1) {       /* loop for input */
      res = read(*fd,tty_buf,255);   /* returns after 5 chars have been input */
      tty_buf[res]=0;               /* so we can printf... */
      printf("envoi de : %s", tty_buf);
      msglen=sprintf(udp_buf, tty_buf) + 1;
      if (udpclient_send(&udp, udp_buf, msglen)) diep("send");
      usleep(10);
    }
    
    close_term(fd);
    printf("term closed\n");
    
    /* for (i=0; i<10; i++) { */
    /* 	printf((char*)"Sending packet %d\n", i); */
    /* 	msglen=sprintf(buf, (char*)"This is packet %d", i) + 1; */
    /* 	if (udpclient_send(&udp, buf, msglen)) diep("send"); */
    /* 	usleep(100000); */
    /* } */
    
    udpclient_close(&udp);
    return 0;
}
