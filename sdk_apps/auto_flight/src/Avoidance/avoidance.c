#include <stdio.h>
#include <unistd.h>

#include <ardrone_api.h>
#include <ardrone_tool/ardrone_tool.h>
#include <ardrone_testing_tool.h>

#include "avoidance.h"
#include "../Comm/gps_comm.h"
#include "../Control/drone_control.h"

    
void avoid_obstacles() {
  comm_datas datas;
  
  datas = get_comm_datas();
  
  double dangerThreshold=100;
  double currentAltitude;
  double avoidanceAltitude;
  
  if ((datas.srfl > dangerThreshold) && (datas.srfr > dangerThreshold)) {
    //Path is clear
    //Basic forward movement a control mvt law will be done in sprint 3
    //CMD : Move Forward
    if (datas.srfl + datas.srfl > 4*dangerThreshold) {
      fprintf(stderr,"[Drone move forward With speed]\n");
    } else {
      fprintf(stderr,"[Drone move forward with a small step]\n");
    }
  } else {
    //Path is Blocked
      //CMD: Stop
    fprintf(stderr,"[Drone Stop]\n");
    //small_move(stop);
    if(datas.srfl > dangerThreshold) {
      //Right is less obstructed
      //CMD: Turn Right
      fprintf(stderr,"[Drone turn left]\n");
      //small_move(turn_left);
    } else if (datas.srfr > dangerThreshold) {
      //Left is less obstructed
      //CMD: Turn Left
      fprintf(stderr,"[Drone turn right]\n");
      //small_move(turn_right);
    } else {
      //If both are equally obstructed
      //Memorise current Altitude
      currentAltitude= 0;//get_nav_datas_altitude();
      //CMD: Go up
      fprintf(stderr,"[Drone Go up ]\n");
      //small_move(up);
    }
  } 
}
 
  
DEFINE_THREAD_ROUTINE(avoidance, data) {

  while (1) {
    avoid_obstacles();
    usleep(100000);
  }

  return (THREAD_RET) 0;
}
