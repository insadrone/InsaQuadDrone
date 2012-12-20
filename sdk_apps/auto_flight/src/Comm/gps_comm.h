#ifndef GPS_COMM_H
#define GPS_COMM_H

#include <VP_Api/vp_api_thread_helper.h>
#include "../GPS/gps.h"
typedef struct comm_datas_t {
  double srfl;
  double srfr;
  char gprmc_string[100];
  char gpgga_string[100];
} comm_datas;

//extern int srf_received;

void init_comm();
void stop_comm();
comm_datas get_comm_datas();
int record_data(char *buf);
int start_comm(void);



PROTO_THREAD_ROUTINE(receive_gps, data);

#endif // GPS_COMM_H
