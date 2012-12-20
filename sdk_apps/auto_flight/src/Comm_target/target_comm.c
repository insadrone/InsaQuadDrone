#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

#include "udp_server.h"
#include "target_comm.h"

#define UDP_TARGET 6445

char buf_target[512];

comm_datas_target ret_datas_target;


void record_data_target(char *buf) {
  char *gprmc_begin = "$GPRMC";
  char *gpgga_begin = "$GPGGA";
  struct gps_coordinate dest;

  if (!strncmp(gprmc_begin,buf,6)) {
    strncpy(ret_datas_target.gprmc_string,buf_target,sizeof(ret_datas_target.gprmc_string));
    printf("TARGET%s",ret_datas_target.gprmc_string);
	extract_coord(ret_datas_target.gprmc_string,&dest);
	average_target_pos(&dest, &ret_datas_target.dest);
	
  } else if (!strncmp(gpgga_begin,buf,6)) {
    strncpy(ret_datas_target.gpgga_string,buf_target,sizeof(ret_datas_target.gpgga_string));
	printf("%s",ret_datas_target.gpgga_string);
	//extract_error(datas_target.gpgga_string,&(datas_target.error));
	
  } else {
    //printf("ERROR NO STRING TARGET DETECTED\n");
  }
  //printf("GPRMC:\n%s\n  GPGGA:\n%s\n ",ret_datas_target.gprmc_string,ret_datas_target.gpgga_string );

}

comm_datas_target get_comm_datas_target() {
  return ret_datas_target;
}

int start_comm_target(void)
{
    udp_struct udp_target;

    int msglen_target;
  
    if(udpserver_init(&udp_target,UDP_TARGET,1)) perror("udp_target init");

      //printf("listen\n");

    while (1) {    
      do {
	msglen_target = udpserver_receive(&udp_target, buf_target, 512);
      } while(msglen_target<=0);

      record_data_target(buf_target);

    }
  
    udpserver_close(&udp_target);
    return 0;
}



DEFINE_THREAD_ROUTINE(gps_target, data) {

  start_comm_target();

  return C_OK;
}
