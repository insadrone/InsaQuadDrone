#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include <ardrone_api.h>
#include <ardrone_tool/ardrone_tool.h>
#include <ardrone_tool/UI/ardrone_input.h>
#include <ardrone_testing_tool.h>

#include "../Auto/auto.h"
#include "../Comm/gps_comm.h"
#include "../Comm_target/target_comm.h"
#include "../Control/drone_control.h"

/*
INPUT: angle   (float)
       tolerance (float)
This function will make uav turn to the direction of target with an error of +- tolerance     
*/
void turn_angle2(float target_angle, float tol) {

  fdata sauv_ndata = get_ndata();
  float angle_360 = sauv_ndata.psi_current;
  float angle_inf, angle_sup;

  angle_inf = target_angle - tol;
  angle_sup = target_angle + tol;

  printf("inf [%f], sup[%f]\n",angle_inf,angle_sup);

  if (sauv_ndata.psi_current < 0) angle_360 = 360 + sauv_ndata.psi_current;
  if (target_angle < 0) target_angle += 360;

  if (target_angle > angle_360) {
    // (target_angle - angle_360) < 180 ? send_order(turn_left,NULL) : send_order(turn_right,NULL);
  } else {
    //(angle_360 - target_angle) < 180 ? send_order(turn_right,NULL) : send_order(turn_left,NULL);
  }

  while (!(sauv_ndata.psi_current > angle_inf && sauv_ndata.psi_current < angle_sup)) {
    printf("tourne [%f]!\n\n",sauv_ndata.psi_current);
    usleep(100);
  }
  
  //  send_order(stop,NULL);
  sleep(1);
  printf("finish turning\n");
    
}
/*
All steps for reaching target
1 Take off and Calibration
2 Receive coordinates gps and calculate direction
3 Turn to direction, then move forward in 3s
4 Return to step 2, if distance < 4 m , Landing ...
*/
void go_target() {
  //1 DONE, 0 not yet	
  static int landed = 0;
  static int calibration = 0;
  static int mission = 0;
  comm_datas datas;
  comm_datas_target datas_target;
  struct gps_coordinate depart;
  struct gps_coordinate relatif_error;
  //gps_error error_dest, error_depart;
  static double distance,angle;
  char msg = "N";	
  distance = 1000.0;	
      while(distance!=0.0){
  	datas = get_comm_datas();	
	extract_coord(datas.gprmc_string,&depart);
	datas_target = get_comm_datas_target();
	printf("INIT\n");	
	initialisation(&depart, &datas_target.dest, &relatif_error);
        navigation(depart,dest,&distance,&angle,&relatif_error);
        printf("INIT : distance %f \n", distance);       
        
      }
      while(c != "Y"){
	    scanf("Is your target ready  ? [Y/N] : %c \n", &msg);
    	    printf("%c\n",msg);
      }
      
      if (landed == 0){
      	printf("TAKE OFF \n");
      	small_move(take_off);
      	landed = 1;
      	sleep(1);
      }
      
      if ( (landed == 1) && (calibration == 0) ){
	sleep(5);
	printf("start calibration\n");
	calibrate_magneto(NULL);
	sleep(4);
	calibration = 1;
	printf("Calibration done\n");
      }
      
      if ( (landed == 1) && (calibration == 1) && (mission = 0) ) {	  
	datas = get_comm_datas();	
	extract_coord(datas.gprmc_string,&depart);
	//extract_error(datas.gpgga_string,&error_depart);
		
	datas_target = get_comm_datas_target();
	//extract_coord(datas_target.gprmc_string,&dest);
	//extract_error(datas_target.gpgga_string,&error_dest);			
		
	navigation(&depart, &datas_target.dest, &distance, &angle, NULL); //&relatif_error
		
	printf("turn angle %f, &angle",angle);
	turn_angle2(angle ,5.0);
		
	sleep(1);
	if (distance > 4.0){
	  //send_order(forward,NULL);
	  printf("FORWARD  \n");
	  sleep(3);
	} else { 
	  //send_order(land,NULL); 
	  printf("LANDING \n");
	  mission = 1;
	}	
      }
}

   
DEFINE_THREAD_ROUTINE(target, data) {

  while (1) {
    if (target_ready) {
      //printf("GO Target launched !\n");
      go_target();
      usleep(100);
    }
    usleep(100);
  }

  return (THREAD_RET) 0;
}

