#include "pid.h"

double somme, previous_val;


void init_pid(void) {
  somme = 0;
  previous_val = 0;
}


double calcul_commande_pid(double consigne, double val_capt) {
  double erreur, derive;

  erreur = consigne - val_capt;
  
  // MAJ intégrale
  somme += erreur;

  // Derivée
  derive = (erreur - previous_val)/T_ECH;

  // MAJ dérivé
  previous_val = erreur;

  return P*(erreur + I*somme + D*derive);
}
