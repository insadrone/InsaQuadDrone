#include "keyboard.h"
#include <VP_Os/vp_os_types.h>
#include <ardrone_tool/UI/ardrone_input.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <ardrone_tool/ardrone_tool_configuration.h>

input_device_t gamepad = {"clavier",
			  init_game_pad,
			  update_game_pad,
			  shutdown_game_pad};

static struct termios old, new;

C_RESULT init_game_pad() {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    new.c_lflag &= ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
    ardrone_tool_input_reset();
    return C_OK;
}

void print_state(input_state_pcmd_t state) {
  printf("===================================\n");
  printf("FLAG = %d\n",state.flag);
  printf("PHI = %4.3f  || THETA = %4.3f\n",state.phi,state.theta);
  printf("GAZ = %4.3f  || YAW = %4.3f\n",state.gaz,state.yaw);
  printf("PSI = %4.3f  || PSI_A = %4.3f\n",state.psi,state.psi_accuracy);
  printf("\033[5A");
}

/* void ack(int32_t success){ */
  
/*   if (success) */
/*     printf("kikoo acquitement\n"); */
/*   else */
/*     printf("Fail\n"); */
/* } */



C_RESULT update_game_pad() {
  char ch = '0';
  //input_state_t *state;

  static float pitch=0.0f,roll=0.0f,gaz=0.0f,yaw=0.0f,psi=0.0f;
  static int fly=0;
    
  //state = ardrone_tool_input_get_state(); 
  //print_state(state->pcmd);

  ch = getchar();
  if (ch == 't') {
    ardrone_tool_set_ui_pad_select(1);
    ardrone_tool_set_ui_pad_start(1);
    printf("decollageee");
  } else if (ch == 'l') {
    ardrone_tool_set_ui_pad_start(0);
  } else if (ch == 'a') {
    printf("gaaz\n");
    fly = 0;
    gaz = (gaz < 0.9) ? gaz + 0.1 : 1.;
  } else if (ch == 'e') {
    printf("gaz\n");
    fly = 0;
    gaz = (gaz > -0.9) ? gaz - 0.1 : -1.;
  } else if (ch == 'q') {
    fly = 1;
    pitch=0.;
    roll = 0.3;
  } else if (ch == 'd') {
    fly = 1;
    pitch=0.;
    roll = -0.3;
  } else if (ch == 'z') {
    fly = 1;
    pitch=0.3;
    roll = 0;
  } else if (ch == 's') {
    fly = 1;
    pitch=-0.3;
    roll = 0;
  } else if (ch == 'w') {
    fly = 0;
    pitch=0.;
    yaw = 0.5;
    roll = 0.;
  } else if (ch == 'c') {
    fly = 0;
    pitch=0.;
    yaw = -0.5;  
    roll = 0;
  /* } else if (ch == '&') { */
  /*   /\*code pourrave*\/ */
  /*   /\* bool_t lol = TRUE; *\/ */
  /*   /\* ARDRONE_TOOL_CONFIGURATION_ADDEVENT(navdata_demo, &lol, ack); *\/ */

  /*   printf("setting tag color %d\n",   ARDRONE_DETECTION_COLOR_ORANGE_YELLOW); */
  /*   int32_t enemyColors = ARDRONE_DETECTION_COLOR_ORANGE_YELLOW; */
  /*   ARDRONE_TOOL_CONFIGURATION_ADDEVENT (enemy_colors, &enemyColors, ack); */
    
  /*   printf("setting shell\n"); */
  /*   int32_t activated = 0; */
  /*   ARDRONE_TOOL_CONFIGURATION_ADDEVENT (enemy_without_shell, &activated, ack); */

  /*   printf("setting detection mode\n"); */
  /*   int32_t detectType = CAD_TYPE_MULTIPLE_DETECTION_MODE; */
  /*   ARDRONE_TOOL_CONFIGURATION_ADDEVENT (detect_type, &detectType, ack); */

  /*   printf("setting detection h_cam\n"); */
  /*   int32_t detectH = 1;//TAG_TYPE_MASK (TAG_TYPE_SHELL_TAG); */
  /*   ARDRONE_TOOL_CONFIGURATION_ADDEVENT (detections_select_v, &detectH, ack); */
    
  /*   printf("CALIBRATION\n"); */
  /*   ardrone_at_set_calibration(ARDRONE_CALIBRATION_DEVICE_MAGNETOMETER); */
  /*   } else if (ch == 'y') { */
    
  /*   printf("setting navdataoptions\n"); */
  /*   int32_t enemyColors2 = NAVDATA_OPTION_FULL_MASK; */
  /*   ARDRONE_TOOL_CONFIGURATION_ADDEVENT (navdata_options, &enemyColors2, ack); */
    
    } else if (ch == 'g') {
    fly = 0;
    pitch=0.0f;
    roll=0.0f;
    gaz=0.0f;
    yaw=0.0f;
    psi = 1.0f;
    printf("psi test\n");   
  } else {
    fly = 0;
    pitch=0.0f;
    roll=0.0f;
    gaz=0.0f;
    yaw=0.0f;
    psi=0.0f;
  }

  ardrone_tool_set_progressive_cmd( fly,/*roll*/roll,/*pitch*/pitch,/*gaz*/gaz,/*yaw*/yaw,psi,psi);
  return C_OK;
}

C_RESULT shutdown_game_pad() {
  tcsetattr(0, TCSANOW, &old);
  return C_OK;
}
