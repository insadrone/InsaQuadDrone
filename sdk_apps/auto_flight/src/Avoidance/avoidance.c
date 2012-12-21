#include <stdio.h>
#include <unistd.h>

#include <ardrone_api.h>
#include <ardrone_tool/ardrone_tool.h>
#include <ardrone_testing_tool.h>

#include "../Avoidance/avoidance.h"
#include "../Auto/auto.h"
#include "../Comm/gps_comm.h"
#include "../Control/drone_control.h"
#include "../STMachine/IAvoid.h"
#include "../GPS/gps.h"

inC_IAvoid input;
outC_IAvoid output;

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
  fprintf(redir_sortie,"REACH %f going UP\n",alt);
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
  fprintf(redir_sortie,"REACH %f going DOWN\n",alt);
}
    

void command(outC_IAvoid comm) {
  mov speed;

  if (comm.up) {
    speed.power = 6;
    send_fast_order(up,(void *)&speed);
  } else if (comm.down) {
    speed.power = 3;
    send_fast_order(down,(void *)&speed);
  } else if (comm.stop) {
    small_move(stop);
  } else if (comm.auto1) {
    speed.power = 1;
    printf("avance\n");
    send_fast_order(forward,(void *)&speed);
  } else if (comm.forward) {
    speed.power = 1;
    printf("avance\n");
    send_fast_order(forward,(void *)&speed);
  }

}
  
DEFINE_THREAD_ROUTINE(avoidance, data) {
  
  comm_datas datas;
  double dangerThreshold=100;
  int detection;
  double *average_left;

  init_array_obstacle_pos2();

  IAvoid_reset(&output);
  input.obstacle_detected = 0;
  input.latence1 = 100;
  input.latence2 = 100;
  input.latence3 = 100;
  input.latence4 = 100;

 while (1) {
   printf("in auto1\n");
   if (auto_ready) {
     //get srf datas
     printf("in auto2\n");
     datas = get_comm_datas();

      average_obstacle_pos2(&datas.srfr, average_left);

     //check threshold
     if (datas.srfr > dangerThreshold) {
       detection = 1;
     } else {
       detection = 0;
     } 
   
     input.obstacle_detected = detection;     
     IAvoid(&input,&output);
     command(output);
     usleep(10000);    
   }
   //   usleep(100);
 }
 
  return (THREAD_RET) 0;
}
