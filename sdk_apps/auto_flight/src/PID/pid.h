#define P 0.1
#define I 0.0
#define D 0.0

#define T_ECH 1


void init_pid(void);


double calcul_commande_pid(double consigne, double val_capt);
