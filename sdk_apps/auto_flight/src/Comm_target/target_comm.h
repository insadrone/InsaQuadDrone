#ifndef TARGET_COMM_H
#define TARGET_COMM_H

#include <VP_Api/vp_api_thread_helper.h>
#include "../Comm/gps_comm.h"

/* all datas sent from target will be stocked in this structure */
typedef struct comm_datas_t_target {
  char gprmc_string[100];
  struct gps_coordinate dest;
  char gpgga_string[100];
  gps_error error;
} comm_datas_target;

//extern int srf_received;

/* return value of ret_datas_target */ 
comm_datas_target get_comm_datas_target();

/*  Distribute each buffer into their corresponding value of ret_datas_target */
void record_data_target(char *buf);

/* Start listening the buffers sent from target */
int start_comm_target(void);

//?? not used ??
char* get_coords_target();


PROTO_THREAD_ROUTINE(gps_target, data);

#endif // TARGET_COMM_H
