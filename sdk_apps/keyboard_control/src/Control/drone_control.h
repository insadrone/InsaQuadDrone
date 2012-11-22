#ifndef DRONE_CONTROL_H
#define DRONE_CONTROL_H

#include <ardrone_tool/UI/ardrone_input.h>

#define INPUT_STATE_NULL {0, 0, 0, 0, 0, 0, 0}
#define GAZ_POWER 0.8
#define THETA_POWER 0.05
#define PHI_POWER 0.05
#define YAW_POWER 0.3

int is_landed(int ctrl_state);

char* print_state_string();

void null_state(input_state_pcmd_t *);

C_RESULT send_order( C_RESULT (*function)());

/*functions supported by the above function*/
C_RESULT take_off();

C_RESULT land();

C_RESULT turn_left();

C_RESULT turn_right();

C_RESULT calibrate_magneto();

C_RESULT forward();

C_RESULT backward();

C_RESULT stop();
  
/*function that should be called à la mano*/
void hoola_hoop();

void looping();

void tag_configurate( const char);

void set_travelling();



#endif
