#ifndef TARGET_COMM_H
#define TARGET_COMM_H

#include <VP_Api/vp_api_thread_helper.h>
#include "../Comm/gps_comm.h"

typedef struct comm_datas_t_target {
  char gprmc_string[100];
  struct gps_coordinate dest;
  char gpgga_string[100];
  gps_error error;
} comm_datas_target;

//extern int srf_received;

comm_datas_target get_comm_datas_target();
void record_data_target(char *buf);
char* get_coords_target();


PROTO_THREAD_ROUTINE(gps_target, data);

#endif // TARGET_COMM_H
