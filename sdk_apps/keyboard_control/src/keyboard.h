#include <stdio.h>
#include <ardrone_tool/UI/ardrone_input.h>

extern input_device_t gamepad;

C_RESULT init_game_pad();
C_RESULT update_game_pad();
C_RESULT shutdown_game_pad();
