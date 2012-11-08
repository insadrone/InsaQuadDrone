#include "read_arduino.h"        

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyACM0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
       

int init_term(int *fd) {
        
  *fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
  if (*fd <0) {perror(MODEMDEVICE); exit(-1); }
        
  tcgetattr(*fd,&oldtio); /* save current port settings */
        
  bzero(&newtio, sizeof(newtio));
  newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;
        
  /* set input mode (non-canonical, no echo,...) */
  newtio.c_lflag |= ICANON | ECHO | ECHONL;
         
  newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */
        
  tcflush(*fd, TCIFLUSH);
  tcsetattr(*fd,TCSANOW,&newtio);
  return 0;
}


int close_term(int *fd) {
  tcsetattr(*fd,TCSANOW,&oldtio);
  return 0;
}
