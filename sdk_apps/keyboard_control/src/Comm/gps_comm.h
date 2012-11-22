#ifndef GPS_COMM_H
#define GPS_COMM_H

#include <VP_Api/vp_api_thread_helper.h>

void init_comm();
void stop_comm();
char* get_coords_target();
char* get_coords_uav();
int start_comm(void);

PROTO_THREAD_ROUTINE(receive_gps, data);

#endif // GPS_COMM_H
