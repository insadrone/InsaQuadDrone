#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "../Comm/gps_comm.h"
#include "../Comm_target/target_comm.h"

double distance_comp = 0.0;
double angle_comp = 0.0;

double compute_distance() {
  comm_datas datas;
  comm_datas_target datas_target;
  struct gps_coordinate depart;
  
  datas = get_comm_datas();	
  extract_coord(datas.gprmc_string,&depart);
  
  datas_target = get_comm_datas_target();

  if ((check_gps_coord_struc(&depart) > 0) && (check_gps_coord_struc(&datas_target.dest) > 0)) {     
    navigation(&depart, &datas_target.dest, &distance_comp, &angle_comp, NULL); //&relatif_error
  }
  return distance_comp;
}

   
DEFINE_THREAD_ROUTINE(data_comp, data) {

  while (1) {
    compute_distance();
    usleep(100);
  }

  return (THREAD_RET) 0;
}

