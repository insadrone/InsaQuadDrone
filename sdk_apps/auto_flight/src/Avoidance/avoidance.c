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
  printf("[%d,%d,%d,%d,%d]\n",comm.up,comm.down,comm.stop,comm.auto1,comm.forward);
  if (comm.up) {
    target_ready = 0;
    speed.power = 8;
    printf("ICIIIIIIIII\n");
    send_fast_order(up,(void *)&speed);
  } else if (comm.down) {
    target_ready = 0;
    speed.power = 3;
    send_fast_order(down,(void *)&speed);
  } else if (comm.stop) {
    target_ready = 0;
    send_fast_order(stop,NULL);
  } else if (comm.auto1) {
    target_ready = 1;
    //speed.power = 1;
    printf("auto\n");
    //send_fast_order(forward,(void *)&speed);
  } else if (comm.forward) {
    target_ready = 0;
    speed.power = 1;
    printf("avance\n");
    send_fast_order(forward,(void *)&speed);
  }

}
  
void command2(outC_IAvoid comm) {
  mov speed;

  if (comm.up) {
    speed.power = 8;
    send_fast_order(up,(void *)&speed);
  } else if (comm.down) {
    speed.power = 3;
    send_fast_order(down,(void *)&speed);
  } else if (comm.stop) {
    send_fast_order(stop,NULL);
  } else if (comm.auto1) {
    speed.power = 1;
    printf("auto\n");
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
  double average_left;
  int ret;

  init_array_obstacle_pos2();

  IAvoid_reset(&output);
  input.obstacle_detected = 0;
  input.latence1 = 100;
  input.latence2 = 100;
  input.latence3 = 200;
  input.latence4 = 200;

 while (1) {
   usleep(10);
   if (auto_ready || auto_ready_o) {
     //get srf datas
     printf("AVOIDANCE\n");
     datas = get_comm_datas();

     ret = average_obstacle_pos2(&datas.srfr, &average_left);
     printf("Moyenne capteur(%d) : %f\n", ret, average_left);

     //check threshold
     if (average_left < dangerThreshold) {
       detection = 1;
       printf("OBSTACLE\n");
     } else {
       detection = 0;
       printf("NO OBSTACLE\n");
     } 
   
     input.obstacle_detected = detection;     
     IAvoid(&input,&output);
     if (auto_ready) {
       command(output);
     } else if (auto_ready_o) {
       command2(output);
     }
     usleep(10000);    
   }
   //   usleep(100);
 }
 
  return (THREAD_RET) 0;
}
