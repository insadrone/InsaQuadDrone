#include <stdio.h>
#include <unistd.h>

#include <ardrone_tool/Navdata/ardrone_general_navdata.h> 
#include <ardrone_api.h>
#include <ardrone_tool/ardrone_tool.h>
#include <ardrone_tool/UI/ardrone_input.h>
#include <ardrone_testing_tool.h>
#include <ardrone_tool/Navdata/ardrone_navdata_client.h>

#include "auto.h"
#include "../Comm/gps_comm.h"
#include "../Control/drone_control.h"

fdata sauv_ndata;

/* Initialization local variables before event loop  */
inline C_RESULT auto_navdata_client_init( void* data )
{
  return C_OK;
}

/* Receving navdata during the event loop */
inline C_RESULT auto_navdata_client_process( const navdata_unpacked_t* const navdata )
{
  const navdata_demo_t*nd = &navdata->navdata_demo;
  const navdata_vision_detect_t *nv = &navdata->navdata_vision_detect;

  //printf("Battery level : .%i. mV\n",nd->vbat_flying_percentage); 
  sauv_ndata.psi_current = nd->psi / 1000;
  sauv_ndata.bat_level_current = nd->vbat_flying_percentage;
  sauv_ndata.ctrl_state_current = nd->ctrl_state;
  sauv_ndata.tag_detected = nv->nb_detected;
  sauv_ndata.tag_tab = nv->camera_source;
  
  //printf("angle %f\n",sauv_ndata.psi_current);

  //printf("\033[2A");
  return C_OK;
}

/* Relinquish the local resources after the event loop exit */
inline C_RESULT auto_navdata_client_release( void )
{
  return C_OK;
}



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

void flight_demo() {
  int tag_config_ok = 0;
  int landed = 0;
    
  while (1) {
    if ((is_landed(sauv_ndata.ctrl_state_current) > 0) && sauv_ndata.bat_level_current > 0) {
      if (!tag_config_ok) {
	sleep(3);
	printf("ok");
	tag_configurate('b');
	tag_config_ok = 1;
      }
    }
    
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
      
      if (landed == 1) {
	sleep(5);
	printf("start turn\n");
	calibrate_magneto(NULL);
	sleep(4);
	small_move(up);
	sleep(1);
	small_move(up);
	sleep(1);
	small_move(up);
	sleep(1);
	turn_angle2(0,5.0);
	sleep(1);
	turn_angle2(90,5.0);
	sleep(1);
	turn_angle2(180,5.0);
	sleep(1);
	turn_angle2(-90,5.0);
	sleep(1);
	turn_angle2(0,5.0);
	sleep(1);
	//hoola_hoop();
	send_order(backward,NULL);
	sleep(5);
	send_order(stop,NULL);
	sleep(3);
	send_order(forward,NULL);
	sleep(1);
	small_move(up);
	sleep(1);
	hoola_hoop();
	sleep(3);
	send_order(land,NULL);
      }	
    }      
  }
}

    
void avoid_obstacles() {
  comm_datas datas;
  while (1) {
    datas = get_comm_datas();
    printf("SL %f\nSR %f\n",datas.srfl,datas.srfr);
    usleep(100000);
  }
}

   
DEFINE_THREAD_ROUTINE(auto_control, data) {
  
  avoid_obstacles();
  return (THREAD_RET) 0;
}
