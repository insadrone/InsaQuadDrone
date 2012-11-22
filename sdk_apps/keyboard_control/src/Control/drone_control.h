#ifndef DRONE_CONTROL_H
#define DRONE_CONTROL_H

#include <ardrone_tool/UI/ardrone_input.h>
#include <time.h>

#define GAZ_POWER 0.8
#define THETA_POWER 0.2
#define PHI_POWER   0.2

#define YAW_POWER   0.5


#define INPUT_STATE_NULL  {0, 0, 0, 0, 0, 0, 0}


typedef struct mov_t{
    int32_t power;
    int32_t distance;
    int32_t time;
}mov;

int32_t d2time(int32_t, int32_t);

int is_landed(int ctrl_state);

char* print_state_string();

void null_state(input_state_pcmd_t *);

C_RESULT send_order( C_RESULT (*function)(void*), void* );
C_RESULT small_move( C_RESULT (*function)(void*) );

/*functions supported by the above function*/
C_RESULT take_off(void*);

C_RESULT land(void*);

C_RESULT turn_left(void*);

C_RESULT turn_right(void*);

C_RESULT calibrate_magneto(void*);

C_RESULT forward(void*);

C_RESULT backward(void*);

C_RESULT stop(void*);
  
C_RESULT up(void *);

C_RESULT down(void *);

C_RESULT left(void *);

C_RESULT right(void *);

/*function that should be called à la mano*/
void hoola_hoop();

void looping();

void tag_configurate( const char);

void set_travelling();

long int diff(struct timeval *, struct timeval *);

#endif
