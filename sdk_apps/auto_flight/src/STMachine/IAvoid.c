/* $*************** KCG Version 6.1.3 (build i6) ****************
** Command: s2c613 -config U:/Windows/Bureau/avoidance/KCG\kcg_s2c_config.txt
** Generation date: 2012-12-20T22:13:40
*************************************************************$ */

#include "IAvoid.h"

void IAvoid_reset(outC_IAvoid *outC)
{
  outC->init = 1;
  outC->init2 = 1;
  outC->init3 = 1;
  outC->init4 = 1;
  outC->init5 = 1;
}

/* IAvoid */
void IAvoid(inC_IAvoid *inC, outC_IAvoid *outC)
{
  int times4;
  int times2;
  int times3;
  int times1;
  int times;
  /* IAvoid::AvoidMachine::Hovering */ int br_2_guard_AvoidMachine_Hovering;
  /* IAvoid::AvoidMachine::Hovering */ int br_1_guard_AvoidMachine_Hovering;
  /* IAvoid::AvoidMachine::Avoidance_up */ int br_2_guard_AvoidMachine_Avoidance_up;
  /* IAvoid::AvoidMachine::Avoidance_restore */ int _5_br_1_guard_AvoidMachine_Avoidance_restore;
  /* IAvoid::AvoidMachine::Avoidance_fwd */ int br_1_guard_AvoidMachine_Avoidance_fwd;
  /* IAvoid::AvoidMachine */ SSM_ST_AvoidMachine AvoidMachine_state_sel;
  /* IAvoid::AvoidMachine */ SSM_ST_AvoidMachine AvoidMachine_state_act;
  /* IAvoid::AvoidMachine */ int AvoidMachine_reset_prv;
  
  if (outC->init5) {
    AvoidMachine_state_sel = SSM_st_Travelling_AvoidMachine;
  }
  else {
    AvoidMachine_state_sel = outC->AvoidMachine_state_nxt;
  }
  switch (AvoidMachine_state_sel) {
    case SSM_st_Avoidance_up_AvoidMachine :
      times3 = !inC->obstacle_detected;
      break;
    
  }
  if (outC->init5) {
    AvoidMachine_reset_prv = 0;
  }
  else {
    AvoidMachine_reset_prv = outC->AvoidMachine_reset_act;
  }
  switch (AvoidMachine_state_sel) {
    case SSM_st_Travelling_AvoidMachine :
      if (inC->obstacle_detected) {
        AvoidMachine_state_act = SSM_st_Hovering_AvoidMachine;
      }
      else {
        AvoidMachine_state_act = SSM_st_Travelling_AvoidMachine;
      }
      break;
    case SSM_st_Hovering_AvoidMachine :
      if (AvoidMachine_reset_prv) {
        outC->init = 1;
      }
      if (outC->init) {
        times4 = inC->latence1;
      }
      else {
        times4 = outC->times8;
      }
      if (times4 < 0) {
        outC->times8 = times4;
      }
      else if (inC->obstacle_detected) {
        outC->times8 = times4 - 1;
      }
      else {
        outC->times8 = times4;
      }
      br_1_guard_AvoidMachine_Hovering = inC->obstacle_detected &
        (outC->times8 == 0);
      br_2_guard_AvoidMachine_Hovering = !inC->obstacle_detected;
      if (br_1_guard_AvoidMachine_Hovering) {
        AvoidMachine_state_act = SSM_st_Avoidance_up_AvoidMachine;
      }
      else if (br_2_guard_AvoidMachine_Hovering) {
        AvoidMachine_state_act = SSM_st_Travelling_AvoidMachine;
      }
      else {
        AvoidMachine_state_act = SSM_st_Hovering_AvoidMachine;
      }
      break;
    case SSM_st_Avoidance_up_AvoidMachine :
      if (AvoidMachine_reset_prv) {
        outC->init2 = 1;
      }
      if (outC->init2) {
        times2 = inC->latence2;
      }
      else {
        times2 = outC->times7;
      }
      if (times2 < 0) {
        outC->times7 = times2;
      }
      else if (times3) {
        outC->times7 = times2 - 1;
      }
      else {
        outC->times7 = times2;
      }
      br_2_guard_AvoidMachine_Avoidance_up = times3 & (outC->times7 == 0);
      if (inC->obstacle_detected) {
        AvoidMachine_state_act = SSM_st_Avoidance_up_AvoidMachine;
      }
      else if (br_2_guard_AvoidMachine_Avoidance_up) {
        AvoidMachine_state_act = SSM_st_Avoidance_fwd_AvoidMachine;
      }
      else {
        AvoidMachine_state_act = SSM_st_Avoidance_up_AvoidMachine;
      }
      break;
    case SSM_st_Avoidance_restore_AvoidMachine :
      if (AvoidMachine_reset_prv) {
        outC->init3 = 1;
      }
      if (outC->init3) {
        times1 = inC->latence4;
      }
      else {
        times1 = outC->times6;
      }
      if (times1 < 0) {
        outC->times6 = times1;
      }
      else {
        outC->times6 = times1 - 1;
      }
      _5_br_1_guard_AvoidMachine_Avoidance_restore = 1 & (outC->times6 ==
          0);
      if (_5_br_1_guard_AvoidMachine_Avoidance_restore) {
        AvoidMachine_state_act = SSM_st_Hovering_AvoidMachine;
      }
      else {
        AvoidMachine_state_act = SSM_st_Avoidance_restore_AvoidMachine;
      }
      break;
    case SSM_st_Avoidance_fwd_AvoidMachine :
      if (AvoidMachine_reset_prv) {
        outC->init4 = 1;
      }
      if (outC->init4) {
        times = inC->latence3;
      }
      else {
        times = outC->times;
      }
      if (times < 0) {
        outC->times = times;
      }
      else {
        outC->times = times - 1;
      }
      br_1_guard_AvoidMachine_Avoidance_fwd = 1 & (outC->times == 0);
      if (br_1_guard_AvoidMachine_Avoidance_fwd) {
        AvoidMachine_state_act = SSM_st_Avoidance_restore_AvoidMachine;
      }
      else if (inC->obstacle_detected) {
        AvoidMachine_state_act = SSM_st_Avoidance_up_AvoidMachine;
      }
      else {
        AvoidMachine_state_act = SSM_st_Avoidance_fwd_AvoidMachine;
      }
      break;
    
  }
  switch (AvoidMachine_state_act) {
    case SSM_st_Travelling_AvoidMachine :
      outC->AvoidMachine_state_nxt = SSM_st_Travelling_AvoidMachine;
      outC->forward = 0;
      outC->auto1 = 1;
      outC->stop = 0;
      outC->down = 0;
      outC->up = 0;
      break;
    case SSM_st_Hovering_AvoidMachine :
      outC->AvoidMachine_state_nxt = SSM_st_Hovering_AvoidMachine;
      outC->forward = 0;
      outC->auto1 = 0;
      outC->stop = 1;
      outC->down = 0;
      outC->up = 0;
      break;
    case SSM_st_Avoidance_up_AvoidMachine :
      outC->AvoidMachine_state_nxt = SSM_st_Avoidance_up_AvoidMachine;
      outC->forward = 0;
      outC->auto1 = 0;
      outC->stop = 0;
      outC->down = 0;
      outC->up = 1;
      break;
    case SSM_st_Avoidance_restore_AvoidMachine :
      outC->AvoidMachine_state_nxt = SSM_st_Avoidance_restore_AvoidMachine;
      outC->forward = 0;
      outC->auto1 = 0;
      outC->stop = 0;
      outC->down = 1;
      outC->up = 0;
      break;
    case SSM_st_Avoidance_fwd_AvoidMachine :
      outC->AvoidMachine_state_nxt = SSM_st_Avoidance_fwd_AvoidMachine;
      outC->forward = 1;
      outC->auto1 = 0;
      outC->stop = 0;
      outC->down = 0;
      outC->up = 0;
      break;
    
  }
  switch (AvoidMachine_state_sel) {
    case SSM_st_Travelling_AvoidMachine :
      outC->AvoidMachine_reset_act = inC->obstacle_detected;
      break;
    case SSM_st_Hovering_AvoidMachine :
      if (br_1_guard_AvoidMachine_Hovering) {
        outC->AvoidMachine_reset_act = 1;
      }
      else {
        outC->AvoidMachine_reset_act = br_2_guard_AvoidMachine_Hovering;
      }
      outC->init = 0;
      break;
    case SSM_st_Avoidance_up_AvoidMachine :
      if (inC->obstacle_detected) {
        outC->AvoidMachine_reset_act = 1;
      }
      else {
        outC->AvoidMachine_reset_act = br_2_guard_AvoidMachine_Avoidance_up;
      }
      outC->init2 = 0;
      break;
    case SSM_st_Avoidance_restore_AvoidMachine :
      outC->AvoidMachine_reset_act =
        _5_br_1_guard_AvoidMachine_Avoidance_restore;
      outC->init3 = 0;
      break;
    case SSM_st_Avoidance_fwd_AvoidMachine :
      if (br_1_guard_AvoidMachine_Avoidance_fwd) {
        outC->AvoidMachine_reset_act = 1;
      }
      else {
        outC->AvoidMachine_reset_act = inC->obstacle_detected;
      }
      outC->init4 = 0;
      break;
    
  }
  outC->init5 = 0;
}

/* $*************** KCG Version 6.1.3 (build i6) ****************
** IAvoid.c
** Generation date: 2012-12-20T22:13:40
*************************************************************$ */

