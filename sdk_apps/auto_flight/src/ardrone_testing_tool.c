/**
 * @file main.c
 * @author sylvain.gaeremynck@parrot.com
 * @date 2009/07/01
 */
#include <ardrone_testing_tool.h>

//ARDroneLib
#include <utils/ardrone_time.h>
#include <ardrone_tool/Navdata/ardrone_navdata_client.h>
#include <ardrone_tool/Control/ardrone_control.h>
#include <ardrone_tool/UI/ardrone_input.h>

//Common
#include <config.h>
#include <ardrone_api.h>

//VP_SDK
#include <ATcodec/ATcodec_api.h>
#include <VP_Os/vp_os_print.h>
#include <VP_Api/vp_api_thread_helper.h>
#include <VP_Os/vp_os_signal.h>

//Local project
#include <Video/video_stage.h>
#include "Auto/auto.h"
#include "Target/target.h"
#include "Comm/gps_comm.h"
#include "Comm_target/target_comm.h"
#include "Avoidance/avoidance.h"

//Keyboard managment
#include "Keyboard/keyboard.h"

static int32_t exit_ihm_program = 1;


/* Implementing Custom methods for the main function of an ARDrone application */
int main(int argc, char** argv)
{
  return ardrone_tool_main(argc, argv);
}


/* The delegate object calls this method during initialization of an ARDrone application */
C_RESULT ardrone_tool_init_custom(void)
{
  /* Registering for a new device of game controller */
  //ardrone_tool_input_add( &gamepad );
  
  /* Start all threads of your application */
  //START_THREAD( video_stage, NULL );
  START_THREAD( auto_control, NULL );
  START_THREAD( receive_gps, NULL );
  START_THREAD( avoidance, NULL );
  START_THREAD( target, NULL );
  START_THREAD( gps_target, NULL );
  
  return C_OK;
}

/* The delegate object calls this method when the event loop exit */
C_RESULT ardrone_tool_shutdown_custom(void)
{
  
  /* Relinquish all threads of your application */
  //JOIN_THREAD( video_stage );
  JOIN_THREAD( auto_control );
  JOIN_THREAD( receive_gps );
  JOIN_THREAD( avoidance );
  JOIN_THREAD( target );
  JOIN_THREAD( gps_target );

  /* Unregistering for the current device */
  //ardrone_tool_input_remove( &gamepad );

  return C_OK;
}

/* The event loop calls this method for the exit condition */
bool_t ardrone_tool_exit()
{
  return exit_ihm_program == 0;
}

C_RESULT signal_exit()
{
  exit_ihm_program = 0;

  return C_OK;
}

/* Implementing thread table in which you add routines of your application and those provided by the SDK */
BEGIN_THREAD_TABLE
  THREAD_TABLE_ENTRY( ardrone_control, 2 )
  THREAD_TABLE_ENTRY( navdata_update, 20 )
  THREAD_TABLE_ENTRY( auto_control, 20 )
  THREAD_TABLE_ENTRY( receive_gps, 50 )
  THREAD_TABLE_ENTRY( avoidance, 60 )
  THREAD_TABLE_ENTRY( target, 60 )
  THREAD_TABLE_ENTRY( gps_target, 60 )
END_THREAD_TABLE

