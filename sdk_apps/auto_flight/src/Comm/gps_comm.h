#ifndef GPS_COMM_H
#define GPS_COMM_H

#include <VP_Api/vp_api_thread_helper.h>


typedef struct comm_datas_t {
  double srfl;
  double srfr;
  char gprmc_string[100];
  char gpgga_string[100];
} comm_datas;

typedef struct comm_datas_t_target {
  char gprmc_string[100];
  char gpgga_string[100];
} comm_datas_target;

//extern int srf_received;

void init_comm();
void stop_comm();
comm_datas get_comm_datas();
comm_datas_target get_comm_datas_target();
void record_data(char *buf);
void record_data_target(char *buf);
char* get_coords_target();
char* get_coords_uav();
int start_comm(void);



PROTO_THREAD_ROUTINE(receive_gps, data);

#endif // GPS_COMM_H
