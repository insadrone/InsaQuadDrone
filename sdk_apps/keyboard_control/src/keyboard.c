#include "keyboard.h"
#include <VP_Os/vp_os_types.h>
#include <ardrone_tool/UI/ardrone_input.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

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


C_RESULT update_game_pad() {
  char ch = '0';
  input_state_t *state;
  static float pitch=0.0f,roll=0.0f,gaz=0.0f,yaw=0.0f;
  static int fly=0;
    
  state = ardrone_tool_input_get_state(); 
  print_state(state->pcmd);

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
  } else {
    fly = 0;
    pitch=0.0f;
    roll=0.0f;
    gaz=0.0f;
    yaw=0.0f;
  }

  ardrone_tool_set_progressive_cmd( fly,/*roll*/roll,/*pitch*/pitch,/*gaz*/gaz,/*yaw*/yaw,0.0,0.0);
  return C_OK;
}

C_RESULT shutdown_game_pad() {
  tcsetattr(0, TCSANOW, &old);
  return C_OK;
}
