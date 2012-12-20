#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include <ardrone_api.h>
#include <ardrone_tool/ardrone_tool.h>
#include <ardrone_tool/UI/ardrone_input.h>
#include <ardrone_testing_tool.h>

#include "../Auto/auto.h"
#include "../Comm/gps_comm.h"
#include "../Control/drone_control.h"


void turn_angle2(float target_angle, float tol) {

  float angle_360 = sauv_ndata.psi_current;
  float angle_inf, angle_sup;

  angle_inf = target_angle - tol;
  angle_sup = target_angle + tol;

  printf("inf [%f], sup[%f]\n",angle_inf,angle_sup);

  if (sauv_ndata.psi_current < 0) angle_360 = 360 + sauv_ndata.psi_current;
  if (target_angle < 0) target_angle += 360;

  if (target_angle > angle_360) {
    (target_angle - angle_360) < 180 ? send_order(turn_left,NULL) : send_order(turn_right,NULL);
  } else {
    (angle_360 - target_angle) < 180 ? send_order(turn_right,NULL) : send_order(turn_left,NULL);
  }

  while (!(sauv_ndata.psi_current > angle_inf && sauv_ndata.psi_current < angle_sup)) {
    printf("tourne [%f]!\n\n",sauv_ndata.psi_current);
    usleep(100);
  }
  
  send_order(stop,NULL);
  sleep(1);
  printf("kikoo\n");
    
}

void go_target() {
  int tag_config_ok = 0;
  int landed = 0;
  int calibration = 0;
  int mission = 0;
  comm_datas datas;
  comm_datas_target datas_target;
  struct gps_coordinate depart;
  //struct gps_coordinate relatif_error;
  //gps_error error_dest, error_depart;
  double distance,angle;
  while (1) {
  	
    //check state of uav and battery level
    if ((is_landed(sauv_ndata.ctrl_state_current) > 0) && sauv_ndata.bat_level_current > 0) {
      if (!tag_config_ok) {
	sleep(3);
	printf("ok");
	tag_configurate('b');
	tag_config_ok = 1;
      }
    }
    //tag detection
    if (sauv_ndata.tag_detected > 0) {

      if (landed == 0){
	send_order(take_off,NULL);
	landed = 1;
	//calibrate_magneto();
      } else {
	send_order(land,NULL);
	sleep(1);
	landed = 0;
      }
	  
      if ( (landed == 1) && (calibration == 0) ){
	sleep(5);
	printf("start calibration\n");
	calibrate_magneto(NULL);
	sleep(4);
	calibration = 1;
      }
      
      if ( (landed == 1) && (calibration == 1) && (mission = 0) ) {	  
	datas = get_comm_datas();	
	extract_coord(datas.gprmc_string,&depart);
	//extract_error(datas.gpgga_string,&error_depart);
		
	datas_target = get_comm_datas_target();
	//extract_coord(datas_target.gprmc_string,&dest);
	//extract_error(datas_target.gpgga_string,&error_dest);			
		
	navigation(&depart, &datas_target.dest, &distance, &angle, NULL); //&relatif_error
		
	turn_angle2(angle ,5.0);
		
	sleep(1);
	if (distance > 4.0){
	  send_order(forward,NULL);
	  sleep(3);
	} else { send_order(land,NULL);
	  mission = 1;}
		
      }
    }//if (sauv_ndata.tag_detected
  }//while
}

   
DEFINE_THREAD_ROUTINE(target, data) {

  while (1) {
    if (target_ready) {
      printf("GO Target launched !\n");
      go_target();
      usleep(10000);
    }
    usleep(10000);
  }

  return (THREAD_RET) 0;
}

