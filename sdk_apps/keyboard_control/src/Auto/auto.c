#include <stdio.h>

/* #include <ardrone_tool/Navdata/ardrone_navdata_client.h> */
 #include <ardrone_tool/Navdata/ardrone_general_navdata.h> 
/* #include <utils/ardrone_gen_ids.h> */


#include <ardrone_api.h>
#include <ardrone_tool/ardrone_tool.h>
#include "ardrone_tool/UI/ardrone_input.h"
#include "ardrone_testing_tool.h"
#include <ardrone_tool/Navdata/ardrone_navdata_client.h>
#include "auto.h"
#include "../Control/drone_control.h"
#include <unistd.h>

float psi_current = 0.0f;
int bat_level_current = 0;
int ctrl_state_current = 0;
int tag_detected = 0;
int *tag_tab = NULL;

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

  printf("Battery level : .%i. mV\n",nd->vbat_flying_percentage); 
  psi_current = nd->psi / 1000;
  bat_level_current = nd->vbat_flying_percentage;
  ctrl_state_current = nd->ctrl_state;
  tag_detected = nv->nb_detected;
  tag_tab = nv->camera_source;

  /* int i = 0; */
  /* printf("cam_type : ["); */
  /* for (i = 0; i < 3; ++i){ */
  /*   printf("%d,", nv->camera_source[i]); */
  /*  } */
  /* printf("]\n"); */
  /* printf("Nb_detected : [%d]\n",nv->nb_detected); */
  /* printf("\033[2A"); */
  
  printf("\033[1A");
  return C_OK;
}

/* Relinquish the local resources after the event loop exit */
inline C_RESULT auto_navdata_client_release( void )
{
  return C_OK;
}


DEFINE_THREAD_ROUTINE(auto_control, data) {
  // Code de la function

  /* int take_off_ok = 0; */
  /* int magneto_calibrated = 0; */
  
  int tag_config_ok = 0;
  int first_time = 0;
  int landed = 0;
  
  while (1) {
    if ((is_landed(ctrl_state_current) > 0) && bat_level_current > 0) {
      if (!tag_config_ok) {
	sleep(3);
	tag_configurate('b');
	tag_config_ok = 1;
      }
    }
    
    if (tag_detected > 0) {
      int i = 0;
      printf("tag detected [");
      for (i = 0; i < tag_detected; ++i){
	printf("%d,", tag_tab[i]);
      }
      printf("\033[D]\n");
      
      if (tag_tab[0] == 1){
	printf("tag au sol\n");
	if (first_time == 0){
	  printf("send forward\n");
	  send_order(forward);
	  sleep(1);
	  first_time = 1;
	} else {
	  printf("STOP!!!!!!!!!!!!!!");
	  send_order(stop);
	  sleep(4);
	  calibrate_magneto();
	  first_time = 0;
	}
      } else if (tag_tab[0] == 0){
	printf("tag vertical\n");
	if (landed == 0){
	  send_order(take_off);
	  sleep(1);
	  landed = 1;
	} else {
	  send_order(land);
	  sleep(1);
	  landed = 0;
	}
      }
      
    } 
  }
  
  return (THREAD_RET) 0;
}
