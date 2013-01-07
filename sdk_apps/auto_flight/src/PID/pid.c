#include "pid.h"


double u_1, e_2, e_1;


void init_pid(void) {
  u_1 = 0;
  e_2 = 0;
  e_1 = 0;
}


double calcul_commande_pid(double consigne, double val_capt) {
  double erreur, commande;

  erreur = consigne - val_capt;
  
  commande = u_1 + P * (erreur - e_1) + I * T_ECH * erreur + D / T_ECH * (erreur - 2 * e_1 + e_2);

  // update variables
  u_1 = commande;
  e_2 = e_1;
  e_1 = erreur;

  return commande;
}


/*double somme, previous_val;

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

  return P*(erreur + T_ECH*somme/I + D*derive);
}*/
