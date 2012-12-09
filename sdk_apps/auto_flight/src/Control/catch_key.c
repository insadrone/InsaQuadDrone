#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <ardrone_tool/ardrone_tool.h>
#include <VP_Os/vp_os_print.h>

#include "catch_key.h"

static struct termios term;
int joy_dev, fd;

int open_clavier(void)
{
  return C_OK;
  int res = C_FAIL;
  struct termios raw;
  int keyboard = STDIN_FILENO;
  
  printf ("Keyboard found.\n");
  joy_dev = keyboard;
  res = C_OK;
  tcgetattr(STDIN_FILENO, &term);
  raw.c_iflag = ICANON;
  cfmakeraw(&raw);
  tcsetattr(keyboard, 0, &raw);
  fd = keyboard;
  //open("/tmp/lol", O_WRONLY | O_CREAT);
  
  if (fd == -1){
    return(C_FAIL);
  }
  
  return res;
}


int update_clavier(void)
{
  return C_OK;
  ssize_t res;
  res = read(joy_dev, &comm, 1);
  if (res != 0){
    if (comm == 'e') {
      fprintf(stderr, "exit\n\n");
      return C_FAIL;
    } else {
      fprintf(stderr, "new order: %c caught\n\n", comm);
    }
  }
  
  return C_OK;
}

int close_clavier(void)
{
  return C_OK;
  tcsetattr(joy_dev, 0, &term);
  //close(fd);
}

/* DEFINE_THREAD_ROUTINE(catch_key, data) */
/* { */
 
  /* PRINT("Catch_key thread init\n\n"); */
  
  /* if (open_clavier() == C_OK){ */
  /*   while (update_clavier() == C_OK) */
  /*     {} */
  /*   close_clavier(); */
  /* } else { */
  /*   printf("fail\n"); */
  /* } */
  
  /* PRINT("Catch_key thread ended\n\n"); */

/*   return (THREAD_RET)0; */
/* } */
