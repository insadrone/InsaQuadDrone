#define P 0.1
#define I 0.0
#define D 0.0

#define T_ECH 1

/*
                      ===== PID =====

    commande(n) = P * [ erreur + (T_ECH * somme_erreur / I) + (D * (erreur(n)-erreur(n-1)) / T_ECH ) ]
<=>
    commande(n) = commande(n-1) + P * (erreur(n)-erreur(n-1)) + I * T_ECH * erreur(n)
                  + D / T_ECH * (erreur(n) - 2*erreur(n-1) + erreur(n-2))

  with erreur = consigne - val_capt

    init_pid function reset calculations (i.e. the sum and derivative parts)
*/
void init_pid(void);

double calcul_commande_pid(double consigne, double val_capt);
