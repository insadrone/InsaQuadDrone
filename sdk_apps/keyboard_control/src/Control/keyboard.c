#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>

#include <ardrone_api.h>
#include <ardrone_tool/ardrone_tool_configuration.h>
#include <VP_Os/vp_os_types.h>
#include <ardrone_tool/UI/ardrone_input.h>

#include "keyboard.h"

#define CONTROL_LATENCY 20
#define POWER 0.5

static int32_t joy_dev = 0;
static struct termios term, raw;

input_device_t keyboard = {
	"Keyboard",
	open_keyboard,
	update_keyboard,
	close_keyboard
};

C_RESULT open_keyboard(void)
{
	C_RESULT res = C_FAIL;
	tcgetattr(STDIN_FILENO, &term);
	raw.c_iflag = ICANON;
	cfmakeraw(&raw);
	joy_dev = STDIN_FILENO;
	res = (joy_dev == -1) ? C_FAIL : C_OK;
	if (res == C_OK){
	  tcsetattr(joy_dev, 0, &raw);
	  printf ("Keyboard found, joy_dev = %d.\r\n", joy_dev);
	}
	return res;
}


/*Translate keys and generate appropriate orders to send to the f***g drone via setprogressive_bite*/
C_RESULT control_command(char c, cmd_param_t *param)
{
  param->fly = 1;
  
  switch (c)
    {
    case 'q':
      param->yaw = -POWER;
      fprintf(stderr, " -> turn-\033[K");
      break;
    case 'd':
      param->yaw = POWER;
      fprintf(stderr, " -> turn+\033[K");
      break;
    case 'o':
      param->pitch = -POWER;
      fprintf(stderr, " -> front\033[K");
      break;
    case 'l':
      param->pitch = POWER;
      fprintf(stderr, " -> back\033[K");
      break;
    case 'k':
      param->roll = -POWER;
      fprintf(stderr, " -> left\033[K");
      break;
    case 'i':
      param->pitch = POWER / 2;
      param->roll = -POWER / 2;
      fprintf(stderr, " -> left+ diag\033[K");
      break;
    case ';':
      param->pitch = -POWER / 2;
      param->roll = -POWER / 2;
      fprintf(stderr, " -> left- diag\033[K");
      break;
    case 'm':
      param->roll = POWER;
      fprintf(stderr, " -> right\033[K");
      break;
    case 'p':
      param->pitch = POWER / 2;
      param->roll = POWER / 2;
      fprintf(stderr, " -> right+ diag\033[K");
      break;
    case '/':
      param->pitch = -POWER / 2;
      param->roll = POWER / 2;
      fprintf(stderr, " -> right- diag\033[K");
      break;
    case'z':
      param->gaz = POWER;
      fprintf(stderr, " -> up\033[K");
      break;
    case 'x':
      param->gaz = -POWER;
      fprintf(stderr, " -> down\033[K");
      break;
    case ' ':
      param->fly = 0;
      param->gaz = 0.0;
      param->yaw = 0.0;
      param->pitch = 0.0;
      param->roll = 0.0;
      fprintf(stderr, " -> hovering mode !\033[K");
      break;
    default:
      param->fly = 0;
      return C_FAIL;
    }
  
  return C_OK;
}


/*All these ack functions are very useless except one of them*/
void ack_hoover(bool_t success){
 if (success == 0){
   fprintf(stderr, "Fail hoovering!\n\r");
 }else{
    fprintf(stderr, "Drone now on hoovering mode!\n\r");
 }
}

void ack1(bool_t success){
  int32_t value = FLYING_MODE_HOVER_ON_TOP_OF_ROUNDEL;
  if (success == 0){
    fprintf(stderr, "Fail sent detectingroundel!\n\r");
  }else{
    fprintf(stderr, "enabling hoover on top!\n\r");
    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (flying_mode, &value, ack_hoover);
  }
}


void ack2(bool_t success){
  
  if (success == 0){
    fprintf(stderr, "Fail travelling mode!\n\r");
  }else{
    fprintf(stderr, "Travelling_mode_enable sent to the drone!\n\r");
  }
}

void ack3(bool_t success){
  
  if (success == 0){
    fprintf(stderr, "Fail sent default travelling mode!\n\r");
  }else{
    fprintf(stderr, "Drone now follow default travelling mode!\n\r");
  }
}

void ack(bool_t success){
  
  if (success == 0){
    fprintf(stderr, "Fail!\n\r");
  }else{
    fprintf(stderr, "Command succefully sent to the drone, check data/config.ini!\n\r");
  }
}

/*if the fucking command success, this fucking callback lauch the command mode*/
void ak_navdata_full(bool_t success){
  
  if (success == 0){
    fprintf(stderr, "Fail to send fullmask navdata!\n");
  }else{
    fprintf(stderr, "Drone now send fullmask navdata!\n");
  }
}

C_RESULT update_keyboard(void)
{
        static char start = 0;
	char comm = 0;
	ssize_t res = read(joy_dev, &comm, 1);
	cmd_param_t cmd = {0, 0.0, 0.0, 0.0, 0.0};
	
	if (res > 0 ){
	  fprintf(stderr, "            [%c, %d]\033[K", comm, res);
	  
	  if (comm == 's'){
	    if (((++start)%2) == 1){
	      fprintf(stderr, " -> take off!\033[K");
	      ardrone_tool_set_ui_pad_start(1);
	      ardrone_tool_set_ui_pad_start(1);
	      ardrone_tool_set_ui_pad_start(1);
	    } else {
	      fprintf(stderr, " -> landing...\033[K");
	      ardrone_tool_set_ui_pad_start(0);
	    }
	  } else if (comm == '1'){
	    int32_t type = TAG_TYPE_ROUNDEL;
	    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (detect_type, &type, ack1);
	  }
	  else if (comm == '2'){
	    int32_t value = TRUE;
	    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (travelling_enable, &value, ack2);
	  } else if (comm == '3'){
	    char str[] = ARDRONE_DEFAULT_TRAVELLING_MODE;
	    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (travelling_mode, str, ack3);
	  } else if (comm == '4'){ 
	    /*lauch all the stuff for the detection of tags*/
	    fprintf(stderr,"setting tag color %d\n",   ARDRONE_DETECTION_COLOR_ORANGE_BLUE);
	    int32_t enemyColors = ARDRONE_DETECTION_COLOR_ORANGE_BLUE;
	    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (enemy_colors, &enemyColors, ack);
	    
	    fprintf(stderr,"setting shell\n");
	    int32_t activated = 0;
	    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (enemy_without_shell, &activated, ack);
	    
	    fprintf(stderr,"setting detection mode\n");
	    int32_t detectType = CAD_TYPE_MULTIPLE_DETECTION_MODE;
	    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (detect_type, &detectType, ack);
    
	    fprintf(stderr,"setting detection h_cam to %d\n", TAG_TYPE_MASK (TAG_TYPE_SHELL_TAG));
	    int32_t detect_v = 1;//
	    ARDRONE_TOOL_CONFIGURATION_ADDEVENT (detections_select_h, &detect_v, ack);

	    /* fprintf(stderr,"setting detection v_cam\n"); */
	    /* int32_t detect_v = 1;//TAG_TYPE_MASK (TAG_TYPE_SHELL_TAG); */
	    /* ARDRONE_TOOL_CONFIGURATION_ADDEVENT (detections_select_h, &detect_v, ack); */
	    

	    
	  } else if (comm == '5'){  
	    int32_t navdata_option = NAVDATA_OPTION_FULL_MASK;
	    ARDRONE_TOOL_CONFIGURATION_ADDEVENT(navdata_options, &navdata_option, ak_navdata_full);
	    
	  } else {
	    if (control_command(comm, &cmd) != C_OK){
	      if (comm =='g'){
		tcsetattr(joy_dev, 0, &term);
		ardrone_tool_set_ui_pad_start(0);
		printf("EXIT\n");
	      }
	      if (comm =='r'){
		char param[20];
		snprintf (param, sizeof (param), "%d,%d", ARDRONE_ANIM_WAVE, MAYDAY_TIMEOUT[ARDRONE_ANIM_WAVE]);
		ARDRONE_TOOL_CONFIGURATION_ADDEVENT (flight_anim, param, NULL);
	      }
	    }
	  }
	  
	  /*send the command to the drone*/
	  ardrone_tool_set_progressive_cmd(cmd.fly,
					   cmd.roll,
					   cmd.pitch,
					   cmd.gaz,
					   cmd.yaw,
					   0.0,0.0);	  
	} else {
	  fprintf(stderr, "nothing typed\n\r");
	} 
	

	return C_OK;
}

C_RESULT close_keyboard(void)
{
        tcsetattr(joy_dev, 0, &term);
	close(joy_dev);
  
	return C_OK;
}
