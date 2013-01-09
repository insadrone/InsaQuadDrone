#include <stdio.h>
#include <string.h>
#include <ardrone_tool/UI/ardrone_input.h>
#include <ardrone_tool/ardrone_tool_configuration.h>

#define CTRL_STATES_STRING 
#include <control_states.h>
#include "drone_control.h"
#include <time.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>

/**
 * \file drone_control.c
 * \brief Drone API using the ARDrone SDK
 */

/*struct to pass movements orders to the drone*/
static input_state_pcmd_t drone_state = INPUT_STATE_NULL;

static int ack_number = 0;

/*
  Example measuring time:
 
  struct timeval start, end;
  gettimeofday(&start, NULL);
  section_critique
  clock_gettime(&end, NULL);
  time = diff(&end, &start);
*/

/**
 * \fn long int diff(struct timeval *timeA_p, struct timeval *timeB_p)
 * \param    Start time, End time
 * \brief    Calculation of an interval time between two time values
 * \return   The difference between the start time and the end time
 */

long int diff(struct timeval *timeA_p, struct timeval *timeB_p)
{
    return (long int)((timeA_p->tv_sec * 1000000) + timeA_p->tv_usec) -
	((timeB_p->tv_sec * 1000000) + timeB_p->tv_usec);
}

/**
 * \fn float p2speed(int power)
 * \param    Power (1 < int < 10)
 * \brief    Translate the power into speed (hand calculated)
 * \return   speed in cm/s
 */

float p2speed(int power){
  
    float speed = 0.0;
    if (power == 1){
	speed = 57.0;//(cm/s)
    } else if (power == 2){
	speed = 84.0;//(cm/s)
    } else {
	printf("This speed wasn't quatified, set to infinite value\n");
	speed = 10000000.0;
    }
    return speed;
}

/**
 * \fn int32_t d2time(int power, int32_t distance)
 * \param    Power (0.0 < float < 1.0), Distance (cm)
 * \brief    
 * \return   Time in usec
 */

int32_t d2time(int power, int32_t distance){
  
    int32_t usec = 0;
    float speed = p2speed(power);
  
    if ( (distance / speed * 1000000.0) < INT_MAX ){
	usec = (int) ceil(distance / speed * 1000000.0);
    } else {
	printf("Warning time overflow\n");
	usec = -1;
    }
    return usec;
}

/**
 * \fn int is_landed(int ctrl_state)
 * \param
 * \brief    
 * \return
 */

int is_landed(int ctrl_state) {
    int major = ctrl_state >> 16;
    ctrl_string_t* ctrl_string = control_states_link[major];
    int ret = 0;
  
    if (ctrl_string != NULL) {
	ret = -1;
    } else {
	ret = (strcmp(ctrl_states[major],"CTRL_LANDED") == 0); 
    } 

    return ret;
}

void recover_emergency() {
  ardrone_tool_set_ui_pad_select(1);
}

/**
 * \fn char* print_state_string(int ctrl_state)
 * \param    
 * \brief    
 * \return   
 */

char* print_state_string(int ctrl_state) {
    int major = ctrl_state >> 16;
    return ctrl_states[major]; 
}

/**
 * \fn C_RESULT small_move( C_RESULT (*function)(void*) )
 * \param    
 * \brief    API for small moves
 * \return   
 */

C_RESULT small_move( C_RESULT (*function)(void*) ){
  
    C_RESULT ret = C_FAIL;
    int32_t usec = 0;
    /*default struct, hardcoded values*/
    mov def_mov = {10, 0, 350};

    /*for up and dow, the movement is a bit different*/
    if (function == up || function == down){
	def_mov.time = 400;
    } 
    usec = 1000 * def_mov.time;
  
    null_state(&drone_state);
    ret = (*function)(&def_mov);
    /* Send the command to the drone */
    if ( ret == C_OK ){
	ardrone_tool_set_progressive_cmd( drone_state.flag,
					  drone_state.phi,
					  drone_state.theta,
					  drone_state.gaz,
					  drone_state.yaw,
					  drone_state.psi,
					  drone_state.psi_accuracy);
	if (usec != 0){
	    if (usec > 0){
		usleep(usec);
	    } else {
		//if the time is not valid (negative value) stop the drone!
		printf("usec was < 0, stop the drone immediatly\n");
	    }
	    //stop the drone
	    ardrone_tool_set_progressive_cmd(0,0,0,0,0,0,0);
	}
	ret = C_OK;
    } else {
	printf("Fail send order\n");
    }
  
    return ret;
}

/**
 * \fn C_RESULT send_order( C_RESULT (*function)(void*), void *arg)
 * \param    
 * \brief  This is not the definitive format but, thanks to this
 *  command, you can pass orders to the drone like forward,
 *  backward...   
 * \return   
 */

C_RESULT send_order( C_RESULT (*function)(void*), void *arg) {

    C_RESULT ret = C_FAIL;
    int32_t usec = 0;
  
    if (arg != NULL){
	mov *mv = (mov*)arg;
	//fprintf(stderr, "argument passé à l'api power = %f\n", mv->power);
	usec = mv->time * 1000;//d2time(mv->power, mv->distance);
    }
  
    null_state(&drone_state);
    ret = (*function)(arg);
    /* Send the command to the drone */
    if ( ret == C_OK ){
	ardrone_tool_set_progressive_cmd( drone_state.flag,
					  drone_state.phi,
					  drone_state.theta,
					  drone_state.gaz,
					  drone_state.yaw,
					  drone_state.psi,
					  drone_state.psi_accuracy);
	if (usec != 0){
	    printf("passage dans usleep\n");
	    if (usec > 0){
		usleep(usec);
	    } else {
		//if the time is not valid (negative value) stop the drone!
		printf("usec was < 0, stop the drone immediatly\n");
	    }
	    //stop the drone
	    ardrone_tool_set_progressive_cmd(0,0,0,0,0,0,0);
	}
	ret = C_OK;
    } else {
	printf("Fail send order\n");
    }
  
    return ret;
}

C_RESULT send_fast_order( C_RESULT (*function)(void*), void *arg) {

    C_RESULT ret = C_FAIL;
    int32_t usec = 0;

    null_state(&drone_state);
    ret = (*function)(arg);

    /* Send the command to the drone */
    if ( ret == C_OK ){
	ardrone_tool_set_progressive_cmd( drone_state.flag,
					  drone_state.phi,
					  drone_state.theta,
					  drone_state.gaz,
					  drone_state.yaw,
					  drone_state.psi,
					  drone_state.psi_accuracy);
	ret = C_OK;
    } else {
	printf("Fail send order\n");
    }
  
    return ret;
}
 
/**
 * \fn void ack(int32_t success)
 * \param    
 * \brief    Generic ack, if you don't want to redefine your own
 *  callback function when you are using parrot's api you could use
 *  this generic ack that print the ack number on success
 */

void ack(int32_t success){
    ack_number++;
    if (success)
	printf("ACK N°%d recieved\n", ack_number);
    else
	printf("Fail\n");
}

/**
 * \fn void null_state(input_state_pcmd_t *i)
 * \param   
 * \brief   Init an struct for configuring drone movements
 */

void null_state(input_state_pcmd_t *i){
    vp_os_memset(i, 0, sizeof(input_state_pcmd_t));
}

/**
 * \fn C_RESULT take_off(void *arg)
 * \param 
 * \brief       Take of the drone
 *  TODO: exit from the anoying emergency state
 * \return
 */

C_RESULT take_off(void *arg) {
  ardrone_tool_set_ui_pad_select(0);
  ardrone_tool_set_ui_pad_start(1);
  return C_OK;
}

/**
 * \fn C_RESULT land(void *arg) 
 * \param    
 * \brief       Gently land the drone
 * \return   
 */

C_RESULT land(void *arg) {
    ardrone_tool_set_ui_pad_start(0);
    return C_OK;
}

/**
 * \fn C_RESULT turn_left(void *arg)
 * \param 
 * \brief       Make the drone rotate on the left (X and Y are not changed,
 *  only the orientation of the drone change).
 * \return
 */

C_RESULT turn_left(void *arg) {
  printf("PUTE LEFT\n");
    float yaw = YAW_POWER;
    if (arg != NULL){
	yaw = (float)((mov*)arg)->power / 10.0;
    }
    drone_state.yaw = yaw;
    return C_OK;
}

/**
 * \fn C_RESULT turn_right(void *arg) 
 * \param 
 * \brief       Make the drone rotate on the right (X and Y are not changed,
 *  only the orientation of the drone change).
 * \return
 */

C_RESULT turn_right(void *arg) {
  printf("PUTE RIGHT\n");
    float yaw = YAW_POWER;
    if (arg != NULL){
	yaw = (float)((mov*)arg)->power / 10.0;
    }
    drone_state.yaw = -yaw;
    return C_OK;
}

/**
 * \fn C_RESULT forward(void *arg)
 * \param  
 * \brief       Make the drone move forward (assuming that the front of the
 *  drone is the side with the frontal cam)
 * \return 
 */

C_RESULT forward(void *arg) {
  printf("PUTE FORWARD\n");
    float theta = THETA_POWER;
    if (arg != NULL){
	theta = (float)((mov*)arg)->power / 10.0;
    }
    fprintf(stderr, "go forward with power = %f\n", theta);
    drone_state.flag = 1;
    drone_state.theta = -theta;
    return C_OK;
}

/**
 * \fn C_RESULT backward(void *arg)
 * \param 
 * \brief       Make the drone move backward (assuming that the front of the
 *  drone is the side with the frontal cam)
 * \return
 */

C_RESULT backward(void *arg) {
    float theta = THETA_POWER;
    if (arg != NULL){
	theta = (float)((mov*)arg)->power / 10.0;
    }
    printf("GO BACKWARD\n");
    drone_state.flag = 1;
    drone_state.theta = theta;
    return C_OK;
}

/**
 * \fn C_RESULT up(void *arg)
 * \param       
 * \brief       Make the drone move up on the z axis
 * \return 
 */

C_RESULT up(void *arg) {
    float gaz = GAZ_POWER;
    if (arg != NULL){
	gaz = (float)((mov*)arg)->power / 10.0;
    }
    printf("GO UP\n");
    drone_state.flag = 1;
    drone_state.gaz = gaz;
    return C_OK;
}

/**
 * \fn C_RESULT down(void *arg)
 * \param Make the drone move down on the z axis
 * \brief    
 * \return
 */

C_RESULT down(void *arg) {
    float gaz = -GAZ_POWER;
    if (arg != NULL){
	gaz = -(float)((mov*)arg)->power / 10.0;
    }
    printf("GO UP\n");
    drone_state.flag = 1;
    drone_state.gaz = gaz;
    return C_OK;
}

/**
 * \fn C_RESULT right(void *arg)
 * \param 
 * \brief       Make the drone move right on the plan xOy
 * \return
 */

C_RESULT right(void *arg) {
    float phi = PHI_POWER;
    if (arg != NULL){
	phi = (float)((mov*)arg)->power / 10.0;
    }
    printf("GO UP\n");
    drone_state.flag = 1;
    drone_state.phi = phi;
    return C_OK;
}

/**
 * \fn C_RESULT left(void *arg)
 * \param
 * \brief       Make the drone move left on the plan xOy
 * \return
 */

C_RESULT left(void *arg) {
    float phi = -PHI_POWER;
    if (arg != NULL){
	phi = -(float)((mov*)arg)->power / 10.0;
    }
    printf("GO LEFT\n");
    drone_state.flag = 1;
    drone_state.phi = phi;
    return C_OK;
}

/**
 * \fn C_RESULT stop(void *arg)
 * \param
 * \brief       Make the drone enter in hoovering mode    
 * \return
 */

C_RESULT stop(void *arg) {
    printf("STOP!\n");
    return C_OK;
}

/**
 * \fn C_RESULT calibrate_magneto(void *arg)
 * \param
 * \brief    Calibrate the magneto, the drone will turn on itself
 * \return
 */

C_RESULT calibrate_magneto(void *arg) {
    ardrone_at_set_calibration(ARDRONE_CALIBRATION_DEVICE_MAGNETOMETER);
    return C_OK;
}

/**
 * \fn void hoola_hoop
 * \brief   Amazing effect, but you cannot yet use this function via
 *  the api  
 */

void hoola_hoop() {
    char param[20];
    snprintf (param, sizeof (param), "%d,%d", ARDRONE_ANIM_WAVE, MAYDAY_TIMEOUT[ARDRONE_ANIM_WAVE]);
    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (flight_anim, param, NULL);
}

/**
 * \fn void looping
 * \brief   Same as above but this effect is a bit more ... funny 
 */

void looping() {
    char param[20];
    snprintf (param, sizeof (param), "%d,%d", ARDRONE_ANIM_FLIP_LEFT, MAYDAY_TIMEOUT[ARDRONE_ANIM_FLIP_LEFT]);
    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (flight_anim, param, NULL);
}

/**
 * \fn void tag_configurate( const char color)
 * \param   Color of the tag to detect
 * \brief   Given a color (char from 'b', 'g', 'y'), set up everything
 *  to be ready to use tag detection   
 */

void tag_configurate( const char color) {

    /*deactivate navdata demo*/
    bool_t demo = FALSE;
    ARDRONE_TOOL_CONFIGURATION_ADDEVENT(navdata_demo, &demo, ack);
  
    /*activate fullmask navdata*/
    int32_t options = NAVDATA_OPTION_FULL_MASK;
    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (navdata_options, &options, ack);
  
    /*setting the color*/
    int32_t enemyColors = ARDRONE_DETECTION_COLOR_ORANGE_BLUE;
    switch (color){
    case 'b':
	enemyColors = ARDRONE_DETECTION_COLOR_ORANGE_BLUE;
	break;
    case 'g':
	enemyColors = ARDRONE_DETECTION_COLOR_ORANGE_GREEN;
	break;
    case 'y':
	enemyColors = ARDRONE_DETECTION_COLOR_ORANGE_YELLOW;
	break;
    }
    printf("setting tag color %d\n", enemyColors);
    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (enemy_colors, &enemyColors, ack);
  
    printf("setting shell\n");
    int32_t activated = 0;
    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (enemy_without_shell, &activated, ack);
  
    printf("setting detection mode\n");
    int32_t detectType = CAD_TYPE_MULTIPLE_DETECTION_MODE;
    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (detect_type, &detectType, ack);
  
    /*detection on h_cam at 30fps*/
    printf("setting detection h_cam\n");
    int32_t detectH = TAG_TYPE_MASK (TAG_TYPE_SHELL_TAG);
    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (detections_select_h, &detectH, ack);
  
    /*detection on v_cam at 60fps*/
    printf("setting detection v_cam\n");
    int32_t detectV = TAG_TYPE_MASK (TAG_TYPE_ROUNDEL);
    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (detections_select_v, &detectV, ack);
    
}

/* void set_travelling() { */
/* char str[] = ARDRONE_DEFAULT_TRAVELLING_MODE; */
/* ARDRONE_TOOL_CONFIGURATION_ADDEVENT (travelling_mode, str, ack); */
/* sleep(1); */
/* int32_t value = TRUE; */
/* ARDRONE_TOOL_CONFIGURATION_ADDEVENT (travelling_enable, &value, ack); */
/* } */
