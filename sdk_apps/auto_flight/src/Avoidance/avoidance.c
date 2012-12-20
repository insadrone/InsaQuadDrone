#include <stdio.h>
#include <unistd.h>

#include <ardrone_api.h>
#include <ardrone_tool/ardrone_tool.h>
#include <ardrone_testing_tool.h>

#include "../Avoidance/avoidance.h"
#include "../Auto/auto.h"
#include "../Comm/gps_comm.h"
#include "../Control/drone_control.h"

int going_up = 0;

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
    
void avoid_obstacles(float thresh) {
  comm_datas datas;
  
  datas = get_comm_datas();
  
  double dangerThreshold=100;

  mov speed;
  static struct timeval tv1,tv2;
  struct timezone tz;
  long long diff;
  static float last = 0.0;

  if (abs(last - datas.srfr) > 1.0) {
    gettimeofday(&tv2, &tz);
    diff=(tv2.tv_sec-tv1.tv_sec) * 1000000L +	\
      (tv2.tv_usec-tv1.tv_usec);  
    //  fprintf(redir_sortie,"durée=%d usec\n",diff);
  //  printf("ALT %f\n",get_alt());
    fprintf(redir_sortie,"SRFR : %f\n",datas.srfr);
    fflush(redir_sortie);
  }
  gettimeofday(&tv1, &tz);
  last = datas.srfr;

  if (/*(datas.srfl > dangerThreshold) && */(datas.srfr > dangerThreshold)) {
    //Path is clear
    //Basic forward movement a control mvt law will be done in sprint 3
    //CMD : Move Forward

    if (going_up) {
      speed.power = 1;
      send_fast_order(forward,(void *)&speed);
      sleep(2);
      //go_down_threshold(thresh);
      small_move(down);
      printf("go down\n");
      going_up = 0;
    }

    if (/*datas.srfl + */datas.srfr > 2*dangerThreshold) {
      fprintf(redir_sortie,"[Drone move forward With speed]\n");
      speed.power = 1;
      send_fast_order(forward,(void *)&speed);      
    } else {
      speed.power = 1;
      send_fast_order(forward,(void *)&speed);
      fprintf(redir_sortie,"[Drone move forward with a small step]\n");
    }

  }  else {
    /* //Path is Blocked */
    /*   //CMD: Stop */
    /* /\* fprintf(redir_sortie,"[Drone Stop]\n"); *\/ */
    /* /\* small_move(stop); *\/ */
    /* /\* if(datas.srfl > dangerThreshold) { *\/ */
    /* /\*   //Right is less obstructed *\/ */
    /* /\*   //CMD: Turn Right *\/ */
    /* /\*   /\\* fprintf(redir_sortie,"[Drone turn left]\n"); *\\/ *\/ */
    /* /\*   /\\* small_move(turn_left); *\\/ *\/ */

    /* /\*   speed.power = 1; *\/ */
    /* /\*   send_fast_order(forward,(void *)&speed); *\/ */


    /* /\* } else *\/ */
    /* /\* if (datas.srfr > dangerThreshold) { *\/ */
    /* /\*   //Left is less obstructed *\/ */
    /* /\*   //CMD: Turn Left *\/ */
    /* /\*   /\\* fprintf(redir_sortie,"[Drone turn right]\n"); *\\/ *\/ */
    /* /\*   /\\* small_move(turn_right); *\\/ *\/ */

    /* /\*   speed.power = 1; *\/ */
    /* /\*   send_fast_order(forward,(void *)&speed); *\/ */
    /* /\* } else { *\/ */
    /*   //If both are equally obstructed */
    /*   //Memorise current Altitude */
    /* //get_nav_datas_altitude(); */
    /*   //CMD: Go up */
  
  /* fprintf(redir_sortie,"[Drone Stop 1]\n"); */
  /* fprintf(redir_sortie,"[Drone Stop 2]\n"); */
  /* fprintf(redir_sortie,"[Drone Stop 3]\n"); */
  /* small_move(stop); */
  /* sleep(4); */
  //speed.power = 6;
  
    //  small_move(stop); 
  sleep(1);
  fprintf(redir_sortie,"[Drone Go up ]\n");
  speed.power = 8;
  send_fast_order(up,(void *)&speed);
  going_up = 1;
  }
}
 
  
DEFINE_THREAD_ROUTINE(avoidance, data) {

  float thresh = 0.5;

  while (1) {
    if (auto_ready) {
      //printf("thread auto lauched\n");
      //go_up_threshold(thresh);
      //printf("SEUIL ATTEINT\n");
      avoid_obstacles(thresh);
      usleep(10);
    }
    usleep(10);
  }

  return (THREAD_RET) 0;
}
