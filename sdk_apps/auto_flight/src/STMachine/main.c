#include <stdio.h>
#include <stdlib.h>
#include "IAvoid.h"

inC_IAvoid input;
outC_IAvoid output;


void print_state(outC_IAvoid in) {
  printf("UP : %d\n",in.up);
  printf("DOWN : %d\n",in.down);
  printf("STOP : %d\n",in.stop);
  printf("AUTO : %d\n",in.auto1);
  printf("FORWARD : %d\n",in.forward);
  printf("===================\n");
}


int main() {
  int rc = 0, cycle = 0;
  IAvoid_reset(&output);
  input.obstacle_detected = 0;
  input.latence1 = 5;
  input.latence2 = 5;
  input.latence3 = 5;
  input.latence4 = 5;
    
  while (1) {
    IAvoid(&input,&output);
    print_state(output);
    usleep(1000000);
    cycle++;
    if (cycle == 3) {
      input.obstacle_detected = 1;
    }

    if (output.stop && !output.up) {
      rc = 0;
    }

    if (output.up == 1) {
      rc = cycle;
      printf("rc %d\n",rc);
      input.obstacle_detected = 0;
    }
    
  }

  return 0;
}
