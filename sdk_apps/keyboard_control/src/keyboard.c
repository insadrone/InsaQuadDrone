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

C_RESULT update_game_pad() {
  char ch = '0';
  ch = getchar();
  if (ch == 't') {
    ardrone_tool_set_ui_pad_select(1);
    if (ardrone_tool_set_ui_pad_start(1) == C_OK) {
      printf ("décollageeeeeeee !!");
    } else {
      printf("probleme\n");
    }
  } else if (ch == 'l') {
    printf ("atterissage !!");
    ardrone_tool_set_ui_pad_start(0);
  } else if (ch == 'q') {
    ardrone_tool_set_ui_pad_ab(0);
    ardrone_tool_set_ui_pad_ag(1);
  } else if (ch == 'd') {
    ardrone_tool_set_ui_pad_ab(1);
    ardrone_tool_set_ui_pad_ag(0);
  } else if (ch == 'z') {
    ardrone_tool_set_ui_pad_ad(1);
    ardrone_tool_set_ui_pad_ah(0);
  } else if (ch == 's') {
    ardrone_tool_set_ui_pad_ad(0);
    ardrone_tool_set_ui_pad_ah(1);
  }

  return C_OK;
}

C_RESULT shutdown_game_pad() {
  tcsetattr(0, TCSANOW, &old);
  return C_OK;
}
