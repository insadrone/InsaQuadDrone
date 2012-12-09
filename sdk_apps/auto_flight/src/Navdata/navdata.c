#include <ardrone_tool/Navdata/ardrone_navdata_client.h>

#include <Navdata/navdata.h>
#include <stdio.h>
#define CTRL_STATES_STRING
#include "control_states.h"
#include "../Auto/auto.h"

#define MAX_STR_CTRL_STATE 2048

/* Initialization local variables before event loop  */
inline C_RESULT demo_navdata_client_init( void* data )
{
  return C_OK;
}

const char* ctrl_state_str(uint32_t ctrl_state)
{
  static char str_ctrl_state[MAX_STR_CTRL_STATE];

  ctrl_string_t* ctrl_string;
  uint32_t major = ctrl_state >> 16;
  uint32_t minor = ctrl_state & 0xFFFF;

  if( strlen(ctrl_states[major]) < MAX_STR_CTRL_STATE )
  {
    vp_os_memset(str_ctrl_state, 0, sizeof(str_ctrl_state));

    strcat(str_ctrl_state, ctrl_states[major]);
    ctrl_string = control_states_link[major];

    if( ctrl_string != NULL && (strlen(ctrl_states[major]) + strlen(ctrl_string[minor]) < MAX_STR_CTRL_STATE) )
    {
      strcat( str_ctrl_state, " | " );
      strcat( str_ctrl_state, ctrl_string[minor] );
    }
  }
  else
  {
    vp_os_memset( str_ctrl_state, '#', sizeof(str_ctrl_state) );
  }

  return str_ctrl_state;
}

/* Receving navdata during the event loop */
inline C_RESULT demo_navdata_client_process( const navdata_unpacked_t* const navdata )
{
  //const navdata_demo_t*nd = &navdata->navdata_demo;
  //const navdata_vision_detect_t *nv = &navdata->navdata_vision_detect;
   /* printf("=====================\nNavdata for flight demonstrations =====================\n\n"); */

   /* printf("Control state : [%s]\n\n\n",ctrl_state_str(nd->ctrl_state)); */
   /* printf("Battery level : %i mV\n",nd->vbat_flying_percentage); */
   /* printf("Orientation   : [Theta] %4.3f  [Phi] %4.3f  [Psi] %4.3f\n",nd->theta,nd->phi,nd->psi/1000); */
   /* printf("Altitude      : %i\n",nd->altitude); */
   /* printf("Speed         : [vX] %4.3f  [vY] %4.3f  [vZPsi] %4.3f\n",nd->theta,nd->phi,nd->psi); */
   
   /* int i = 0; */
   /* printf("cam_type : ["); */
   /* for (i = 0; i < 3; ++i){ */
   /*   printf("%d,", nv->camera_source[i]); */
   /* } */
   /* printf("]\n"); */
   /* printf("Nb_detected : [%d]\n",nv->nb_detected);  */
   /* printf("\033[10A"); */
  return C_OK;
}

/* Relinquish the local resources after the event loop exit */
inline C_RESULT demo_navdata_client_release( void )
{
  return C_OK;
}

/* Registering to navdata client */
BEGIN_NAVDATA_HANDLER_TABLE
  /* NAVDATA_HANDLER_TABLE_ENTRY(demo_navdata_client_init, demo_navdata_client_process, demo_navdata_client_release, NULL) */
 NAVDATA_HANDLER_TABLE_ENTRY(auto_navdata_client_init, auto_navdata_client_process, auto_navdata_client_release, NULL)
END_NAVDATA_HANDLER_TABLE

