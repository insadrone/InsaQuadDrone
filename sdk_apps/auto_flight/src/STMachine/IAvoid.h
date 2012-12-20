/* $*************** KCG Version 6.1.3 (build i6) ****************
** Command: s2c613 -config U:/Windows/Bureau/avoidance/KCG\kcg_s2c_config.txt
** Generation date: 2012-12-20T22:13:40
*************************************************************$ */
#ifndef _IAvoid_H_
#define _IAvoid_H_

typedef enum {
  SSM_TR_no_trans_AvoidMachine,
  SSM_TR_Travelling_1_AvoidMachine,
  SSM_TR_Hovering_1_AvoidMachine,
  SSM_TR_Hovering_2_AvoidMachine,
  SSM_TR_Avoidance_up_1_AvoidMachine,
  SSM_TR_Avoidance_up_2_AvoidMachine,
  SSM_TR_Avoidance_restore_1_AvoidMachine,
  SSM_TR_Avoidance_fwd_1_AvoidMachine,
  SSM_TR_Avoidance_fwd_2_AvoidMachine
} SSM_TR_AvoidMachine;
/* IAvoid::AvoidMachine */
typedef enum {
  SSM_st_Travelling_AvoidMachine,
  SSM_st_Hovering_AvoidMachine,
  SSM_st_Avoidance_up_AvoidMachine,
  SSM_st_Avoidance_restore_AvoidMachine,
  SSM_st_Avoidance_fwd_AvoidMachine
} SSM_ST_AvoidMachine;

/* ========================  input structure  ====================== */
typedef struct {
  int /* IAvoid::obstacle_detected */ obstacle_detected;
  int /* IAvoid::latence1 */ latence1;
  int /* IAvoid::latence2 */ latence2;
  int /* IAvoid::latence3 */ latence3;
  int /* IAvoid::latence4 */ latence4;
} inC_IAvoid;

/* ========================  context type  ========================= */
typedef struct {
  /* ---------------------------  outputs  --------------------------- */
  int /* IAvoid::up */ up;
  int /* IAvoid::down */ down;
  int /* IAvoid::stop */ stop;
  int /* IAvoid::auto */ auto1;
  int /* IAvoid::forward */ forward;
  /* -----------------------  no local probes  ----------------------- */
  /* -------------------- initialization variables  ------------------ */
  int init5;
  int init4;
  int init3;
  int init2;
  int init;
  /* ----------------------- local memories  ------------------------- */
  int times8;
  int times7;
  int times6;
  int times;
  SSM_ST_AvoidMachine /* IAvoid::AvoidMachine */ AvoidMachine_state_nxt;
  int /* IAvoid::AvoidMachine */ AvoidMachine_reset_act;
  /* -------------------- no sub nodes' contexts  -------------------- */
  /* ----------------- no clocks of observable data ------------------ */
} outC_IAvoid;

/* ===========  node initialization and cycle functions  =========== */
/* IAvoid */
extern void IAvoid(inC_IAvoid *inC, outC_IAvoid *outC);

extern void IAvoid_reset(outC_IAvoid *outC);

#endif /* _IAvoid_H_ */
/* $*************** KCG Version 6.1.3 (build i6) ****************
** IAvoid.h
** Generation date: 2012-12-20T22:13:40
*************************************************************$ */

