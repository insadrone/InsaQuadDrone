#ifndef GPS_COMM_H
#define GPS_COMM_H

#include <VP_Api/vp_api_thread_helper.h>
#include "../GPS/gps.h"

/* all datas sent from uav will be stocked in this structure */
typedef struct comm_datas_t {
  double srfl;
  double srfr;
  char gprmc_string[100];
  char gpgga_string[100];
} comm_datas;

//extern int srf_received;
/* initilisation the communication UDP between the target and the station */
void init_comm();

/* stop the communication UDP between the target and the station */
void stop_comm();

/* return value of ret_datas */ 
comm_datas get_comm_datas();

/*  Distribute each buffer into their corresponding value of ret_datas */
int record_data(char *buf);

/* Start listening the buffers sent from uav */
int start_comm(void);



PROTO_THREAD_ROUTINE(receive_gps, data);

#endif // GPS_COMM_H
