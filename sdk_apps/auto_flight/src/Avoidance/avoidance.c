#include <stdio.h>
#include <unistd.h>

#include <ardrone_api.h>
#include <ardrone_tool/ardrone_tool.h>
#include <ardrone_testing_tool.h>

#include "../Avoidance/avoidance.h"
#include "../Auto/auto.h"
#include "../Comm/gps_comm.h"
#include "../Control/drone_control.h"

void go_up_threshold(float thresh) {
  float alt = get_alt();
  mov speed;
  speed.power = 4;

  printf("UP to %f\n",thresh);
  while (alt < thresh) {
    send_fast_order(up,(void *)&speed);
    alt = get_alt();
    usleep(100);
  }
  printf("REACH %f going UP\n",alt);
}

void go_down_threshold(float thresh) {
  float alt = get_alt();
  mov speed;
  speed.power = 4;
  printf("DOWN to %f\n",thresh);
  while (alt > thresh) {
    send_fast_order(down,(void *)&speed);
    alt = get_alt();
    usleep(100);
  }
  printf("REACH %f going DOWN\n",alt);
}
    
void avoid_obstacles(float thresh) {
  comm_datas datas;
  
  datas = get_comm_datas();
  
  double dangerThreshold=200;
  int going_up = 0;

  mov speed;

  printf("ALT %f\n",get_alt());

  if (/*(datas.srfl > dangerThreshold) && */(datas.srfr > dangerThreshold)) {
    //Path is clear
    //Basic forward movement a control mvt law will be done in sprint 3
    //CMD : Move Forward

    if (/*datas.srfl + */datas.srfr > 2*dangerThreshold) {
      fprintf(stderr,"[Drone move forward With speed]\n");
      speed.power = 1;
      send_fast_order(forward,(void *)&speed);      
    } else {
      speed.power = 1;
      send_fast_order(forward,(void *)&speed);
      fprintf(stderr,"[Drone move forward with a small step]\n");
    }
    if (going_up) {
      go_down_threshold(thresh);
      going_up = 0;
    }
  } else {
    //Path is Blocked
      //CMD: Stop
    /* fprintf(stderr,"[Drone Stop]\n"); */
    /* small_move(stop); */
    /* if(datas.srfl > dangerThreshold) { */
    /*   //Right is less obstructed */
    /*   //CMD: Turn Right */
    /*   /\* fprintf(stderr,"[Drone turn left]\n"); *\/ */
    /*   /\* small_move(turn_left); *\/ */

    /*   speed.power = 1; */
    /*   send_fast_order(forward,(void *)&speed); */


    /* } else */
    /* if (datas.srfr > dangerThreshold) { */
    /*   //Left is less obstructed */
    /*   //CMD: Turn Left */
    /*   /\* fprintf(stderr,"[Drone turn right]\n"); *\/ */
    /*   /\* small_move(turn_right); *\/ */

    /*   speed.power = 1; */
    /*   send_fast_order(forward,(void *)&speed); */
    /* } else { */
      //If both are equally obstructed
      //Memorise current Altitude
    //get_nav_datas_altitude();
      //CMD: Go up
      fprintf(stderr,"[Drone Go up ]\n");
      speed.power = 4;
      send_fast_order(up,(void *)&speed);
      going_up = 1;
      //}
  } 
}
 
  
DEFINE_THREAD_ROUTINE(avoidance, data) {

  float thresh = 0.5;

  while (1) {
    if (auto_ready) {
      printf("thread auto lauched\n");
      go_up_threshold(thresh);
      printf("SEUIL ATTEINT\n");
      avoid_obstacles(thresh);
      usleep(100000);
    }
    usleep(100000);
  }

  return (THREAD_RET) 0;
}
