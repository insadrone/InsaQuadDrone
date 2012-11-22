#include <stdio.h>
#include <string.h>
#include <ardrone_tool/UI/ardrone_input.h>
#include <ardrone_tool/ardrone_tool_configuration.h>

#define CTRL_STATES_STRING 
#include "control_states.h"
#include "drone_control.h"

/*struct to pass movements orders to the drone*/
static input_state_pcmd_t drone_state = INPUT_STATE_NULL;

/** @function
 *  @param 
 *  @brief    Deobfuscation of the control_states mode
 *  backward...
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

char* print_state_string(int ctrl_state) {
  int major = ctrl_state >> 16;
  return ctrl_states[major]; 
}


/** @function
 *  @param 
 *  @brief    This is not teh definitive format but, thanks to this
 *  command, you can pass orders to the drone like forward,
 *  backward...
 */
C_RESULT send_order( C_RESULT (*function)()) {

  C_RESULT ret = C_FAIL;
  
  null_state(&drone_state);
  ret = (*function)();
  /* Send the command to the drone */
  if ( ret == C_OK ){
    ardrone_tool_set_progressive_cmd( drone_state.flag,
				      drone_state.phi,
				      drone_state.theta,
				      drone_state.gaz,
				      drone_state.yaw,
				      drone_state.psi,
				      drone_state.psi_accuracy);
  } else {
    printf("Fail send order\n");
  }
  
  return C_OK;
}


/** @function
 *  @
 *  @brief   Generic ack, if you don't want to redefine your own
 *  callback function when you are using parrot's api you could use
 *  this generic ack that print the ack number on sucess
 */
static int ack_number = 0;

void ack(int32_t success){
  ack_number++;
  if (success)
    printf("ACK N°%d recieved\n", ack_number);
  else
    printf("Fail\n");
}

/** @function
 *  @
 *  @brief Init an struct for configuring drone movements 
 */
void null_state(input_state_pcmd_t *i){
  i->flag         = 0;		  
  i->phi          = 0;		
  i->theta        = 0;	
  i->gaz          = 0;		
  i->yaw          = 0;		
  i->psi          = 0;		
  i->psi_accuracy = 0;
}


/** @function
 *  @
 *  @brief Take of the drone
 *  TODO: exit from the anoying emergency state
 */
C_RESULT take_off() {
  ardrone_tool_set_ui_pad_select(0);
  ardrone_tool_set_ui_pad_start(1);
  return C_OK;
}

/** @function
 *  @
 *  @brief Gently land the drone
 */
C_RESULT land() {
  ardrone_tool_set_ui_pad_start(0);
  return C_OK;
}

/** @function
 *  @
 *  @brief Make the drone rotate on the left (X and Y are not changed,
 *  only the orientation of the drone change).
 */
C_RESULT turn_left() {  
  drone_state.yaw = YAW_POWER;
  printf("turn left\n");
  return C_OK;
}

/** @function
 *  @
 *  @brief Make the drone rotate on the right (X and Y are not changed,
 *  only the orientation of the drone change).
 */
C_RESULT turn_right() {
  drone_state.yaw = -YAW_POWER;
  printf("turn right\n");
  return C_OK;
}

/** @function
 *  @
 *  @brief Make the drone move forward (assuming that the front of the
 *  drone is the side with the frontal cam)
 */
C_RESULT forward() {  
  printf("go forward\n");
  drone_state.flag = 1;
  drone_state.theta = -THETA_POWER;
  return C_OK;
}

/** @function
 *  @
 *  @brief Make the drone move backward (assuming that the front of the
 *  drone is the side with the frontal cam)
 */
C_RESULT backward() {
  printf("go backward\n");
  drone_state.flag = 1;
  drone_state.theta = THETA_POWER;
  return C_OK;
}

/** @function
 *  @
 *  @brief Make the drone enter in hoovering mode
 */
C_RESULT stop() {
  printf("stop!\n");
  return C_OK;
}

/** @function
 *  @
 *  @brief Calibrate the magneto, the drone will turn on itself
 */
C_RESULT calibrate_magneto() {
  ardrone_at_set_calibration(ARDRONE_CALIBRATION_DEVICE_MAGNETOMETER);
   return C_OK;
}

/** @function
 *  @
 *  @brief Amazing effect, but you cannot yet use this function via
 *  the api
 */
void hoola_hoop() {
  char param[20];
  snprintf (param, sizeof (param), "%d,%d", ARDRONE_ANIM_WAVE, MAYDAY_TIMEOUT[ARDRONE_ANIM_WAVE]);
  ARDRONE_TOOL_CONFIGURATION_ADDEVENT (flight_anim, param, NULL);
}

/** @function
 *  @
 *  @brief Same as above but this effect is a bit more ... funny
 */
void looping() {
  char param[20];
  snprintf (param, sizeof (param), "%d,%d", ARDRONE_ANIM_FLIP_LEFT, MAYDAY_TIMEOUT[ARDRONE_ANIM_FLIP_LEFT]);
  ARDRONE_TOOL_CONFIGURATION_ADDEVENT (flight_anim, param, NULL);
}

/** @function
 *  @
 *  @brief Given a color (char from 'b', 'g', 'y'), set up everything
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
  int32_t detectV = TAG_TYPE_MASK (TAG_TYPE_SHELL_TAG);
  ARDRONE_TOOL_CONFIGURATION_ADDEVENT (detections_select_v, &detectV, ack);
    
}

void set_travelling() {
  /* char str[] = ARDRONE_DEFAULT_TRAVELLING_MODE; */
  /* ARDRONE_TOOL_CONFIGURATION_ADDEVENT (travelling_mode, str, ack); */
  
  /* sleep(1); */
  
  /* int32_t value = TRUE; */
  /* ARDRONE_TOOL_CONFIGURATION_ADDEVENT (travelling_enable, &value, ack); */

}



