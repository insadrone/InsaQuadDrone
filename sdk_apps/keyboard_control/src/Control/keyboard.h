#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

extern input_device_t keyboard;

typedef struct _cmd_param_t{

  int fly;
  float roll;
  float pitch;
  float gaz;
  float yaw;
  
} cmd_param_t;


C_RESULT open_keyboard(void);
C_RESULT update_keyboard(void);
C_RESULT close_keyboard(void);


#endif // _KEYBOARD_H_
