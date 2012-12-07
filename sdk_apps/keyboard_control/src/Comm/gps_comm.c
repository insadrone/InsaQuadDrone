#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "udp_server.h"
#include "gps_comm.h"
#include "../GPS/gps.h"

#define UDP_UAV 6444
#define UDP_TARGET 6445

char buf_uav[512];
char buf_target[512];

int start_listen;

comm_datas ret_datas;

void diep(const char *s)
{
    perror(s);
    exit(1);
}

void init_comm() {
  printf("start comm true\n");
  start_listen = 1;
}

void stop_comm() {
  printf("stop comm true\n");
  start_listen = 0;
}

comm_datas get_comm_datas() {
  return ret_datas;
}

char* get_coords_target() {
  

  struct gps_coordinate depart;
  printf("%s\n", buf_uav);
  extract_coord(buf_uav ,&depart);

  struct gps_coordinate dest;
  printf("%s\n", buf_target);
  extract_coord(buf_target,&dest);

  double distance, angle;

  struct gps_coordinate error;

  initialisation_gps(&depart,&dest, &error);

  navigation(&depart, &dest, &distance, &angle, &error);

  printf("angle distance %f %f \n", angle , distance);
  return NULL;
}

void record_data(char *buf) {
  char *gps_begin = "$GPR";
  char *srfl_begin = "$SRFL";
  char *srfr_begin = "$SRFR";

  if (!strncmp(gps_begin,buf,4)) {
    strncpy(ret_datas.gps_string,buf_uav,sizeof(ret_datas.gps_string));
  } else if (!strncmp(srfl_begin,buf,5)) {
    ret_datas.srfl = atof(buf+6*sizeof(char));
  } else if (!strncmp(srfr_begin,buf,5)) {
    ret_datas.srfr = atof(buf+6*sizeof(char));
  } else {
    printf("ERROR NO STRING DETECTED\n");
  }
  //printf("GPS %s\n SRFL : %f\n SRFR : %f\n",ret_datas.gps_string,ret_datas.srfl,ret_datas.srfr);

}


char* get_coords_uav() {
  return NULL;
}


int start_comm(void)
{
    udp_struct udp_uav;
    udp_struct udp_target;

    int msglen_uav,msglen_target;
  
    if(udpserver_init(&udp_uav,UDP_UAV,1)) diep("udp_UAV init");
    if(udpserver_init(&udp_target,UDP_TARGET,1)) diep("udp_target init");
  
    printf("start listen\n");

    while (start_listen) {
      printf("listen\n");
	do {
	    msglen_uav = udpserver_receive(&udp_uav, buf_uav, 512);	   
	} while(msglen_uav<=0);
	
	printf("udp uav received\n");

	record_data(buf_uav);
	/* do { */
	/*     msglen_target = udpserver_receive(&udp_target, buf_target, 512);	    */
	/* } while(msglen_target<=0);        */
	
	//get_coords_target();
    }
  
    udpserver_close(&udp_uav);
    udpserver_close(&udp_target);
    return 0;
}


DEFINE_THREAD_ROUTINE(receive_gps, data) {

  init_comm();

  start_comm();

  stop_comm();

  return C_OK;
}
