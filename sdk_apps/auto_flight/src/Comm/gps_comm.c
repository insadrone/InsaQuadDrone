#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

#include "udp_server.h"
#include "gps_comm.h"
//#include "../GPS/gps.h"

#define UDP_UAV 6444

char buf_uav[512];

int start_listen;

int srf_received = 0;

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

/* char* get_coords_target() { */
  

/*   struct gps_coordinate depart; */
/*   printf("%s\n", buf_uav); */
/*   extract_coord(buf_uav ,&depart); */

/*   struct gps_coordinate dest; */
/*   printf("%s\n", buf_target); */
/*   extract_coord(buf_target,&dest); */

/*   double distance, angle; */

/*   struct gps_coordinate error; */

/*   initialisation_gps(&depart,&dest, &error); */

/*   navigation(&depart, &dest, &distance, &angle, &error); */

/*   printf("angle distance %f %f \n", angle , distance); */
/*   return NULL; */
/* } */

int record_data(char *buf) {
  char *gprmc_begin = "$GPRMC";
  char *gpgga_begin = "$GPGGA";
  char *srfl_begin = "$SRFL";
  char *srfr_begin = "$SRFR";

  int ret_val = 0;

  if (!strncmp(gprmc_begin,buf,6)) {
    strncpy(ret_datas.gprmc_string,buf_uav,sizeof(ret_datas.gprmc_string));
    //    printf("UAV%s",ret_datas.gprmc_string);
    ret_val = 1;
  } else if (!strncmp(gpgga_begin,buf,6)) {
    strncpy(ret_datas.gpgga_string,buf_uav,sizeof(ret_datas.gpgga_string));
  } else if (!strncmp(srfl_begin,buf,5)) {
    ret_datas.srfl = atof(buf+6*sizeof(char));
  } else if (!strncmp(srfr_begin,buf,5)) {
    ret_datas.srfr = atof(buf+6*sizeof(char));
    //printf("SRF%f\n",ret_datas.srfr);
  } else {
    printf("ERROR NO STRING DETECTED\n");
  }
  //printf("GPS %s\n SRFL : %f\n SRFR : %f\n",ret_datas.gps_string,ret_datas.srfl,ret_datas.srfr);
  return ret_val;
}

int start_comm(void)
{
    udp_struct udp_uav;
    int msglen_uav;
  
    if(udpserver_init(&udp_uav,UDP_UAV,1)) diep("udp_UAV init");
  
    while (start_listen) {
      //printf("listen\n");
	do {
	    msglen_uav = udpserver_receive(&udp_uav, buf_uav, 512);	   
	} while(msglen_uav<=0);
	
	//printf("udp uav received\n");

       record_data(buf_uav);

    }
  
    udpserver_close(&udp_uav);
    return 0;
}


DEFINE_THREAD_ROUTINE(receive_gps, data) {

  init_comm();

  start_comm();

  stop_comm();

  return C_OK;
}
