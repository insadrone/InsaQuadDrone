#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config_bluetooth_connexion.h"
#include "gps.h"
#include "udp_client.h"

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/rfcomm0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

       
void diep(const char *s)
{
    perror(s);
    exit(1);
}


int main() {
	int fd, res, length;
	struct termios oldtio,newtio;
	char buf[255],buf_udp[255];
	struct gps_coordinate *coordinate = malloc(sizeof(struct gps_coordinate));
	udp_struct udp;

	if(udpclient_init(&udp,"192.168.1.2",6445)) diep("udpclient_init");

	if(config() < 0)
	{
		printf("Error bluetooth configuration");
		return 1;
	}

	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
	if (fd <0) {perror(MODEMDEVICE); exit(-1); }

	tcgetattr(fd,&oldtio); /* save current port settings */

	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag |= ICANON | ECHO | ECHONL;
	 
//	newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
//	newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);


	while (1){       /* loop for input */
	res = read(fd,buf,255);
	buf[res]='\0';

printf("\n======================================================\n");
printf("Raw data from device:\n");
printf("%s\n\n", buf);

if( (buf[1] = 'G') && (buf[2] = 'P') && (buf[3] = 'R') && (buf[4] = 'M') && (buf[5] == 'C') )
{
printf("\n%s\n",buf);
length = sprintf(buf_udp, buf);
/*printf("Printf inside function 'extract_coord' from gps.c made by Mathieu:\n");

	extract_coord(buf, coordinate);

printf("What I get after the function call:\n");
	printf("lat: %lf\tlong: %lf\n", coordinate->latitude, coordinate->longitude);
*/
	/*
		send UDP to station instead of printf
	*/
	if (udpclient_send(&udp, buf_udp, length+1)) diep("send");
	sleep(1);

}
else
printf("No GPRMC string\n");

	} //end while
	tcsetattr(fd,TCSANOW,&oldtio);

	udpclient_close(&udp);

	return 0;
}
