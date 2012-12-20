#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

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
int auto_ready = 0;
int target_ready = 0;
FILE *redir_sortie = NULL;

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
  sauv_ndata.alt = nd->altitude / 1000.0;
  
  //printf("alt %f\n",sauv_ndata.alt);
  //printf("angle %f\n",sauv_ndata.psi_current);

  //printf("\033[2A");
  return C_OK;
}

/* Relinquish the local resources after the event loop exit */
inline C_RESULT auto_navdata_client_release( void )
{
  return C_OK;
}

float get_alt() {
  return sauv_ndata.alt;
}

fdata get_ndata() {
  return sauv_ndata;
}

void control() {

  char c;

  while (1) {
    usleep(100000);
    scanf("%c", &c);
    printf("%c\n",c);
    switch(c){
    case 'f':
      small_move(forward);
      break;
    case 'b':
      small_move(backward);
      break;
    case 'u':
      small_move(up);
      break;
    case 'd':
      small_move(down);
      break;
    case 'o':
      //small_move(left);
      printf("Batt :%d\n",sauv_ndata.bat_level_current);
      break;
    case 't':
      small_move(take_off);
      break;
    case 'c':
      target_ready = 1;
      //small_move(turn_left);
      break;
    case 'a':
      auto_ready = 1;
      //small_move(turn_left);
      break;
    case 'x':
      //small_move(turn_right);
      printf("OFF emergency mode\n");
      recover_emergency();
      break;
    case '\n':
      break;
    default:
      small_move(land);
      printf("KIKOO LAND\n");
    }
    //printf("kikoo\n");
  }
}

   
DEFINE_THREAD_ROUTINE(auto_control, data) {
  redir_sortie = fopen("lol","a+");

  control();

  return (THREAD_RET) 0;
}

