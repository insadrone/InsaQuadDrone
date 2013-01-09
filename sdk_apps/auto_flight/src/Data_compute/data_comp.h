#ifndef DATA_H
#define DATA_H

#include <VP_Api/vp_api_thread_helper.h>

double compute_distance();

PROTO_THREAD_ROUTINE(data_comp, data);

#endif // DATA_H
