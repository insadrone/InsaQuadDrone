/*
 *  Ceci est votre programme de test.
 *  Il va tester une ou plusieurs fonctions dans des "test suites".

 * Pour compiler
 * gcc -L/usr/local/lib -o gps_test -lcunit -lncurses gps_test.c gps.c -lcunit -lm
 *
 */

#include <stdio.h>
#include "gps.h"
#include <CUnit/CUnit.h>

/*
 *  Cette procédure de test vérifie que navigation(,,,) donne de bons valeurs de l'angle et de la distance
 *  On test avec le point depart est le pole Nord et celui de destination est le pole Sud avec une variation de longitude
 */

void test1_de_navigation() {
  struct gps_coordinate *depart;
  depart = malloc(sizeof(struct gps_coordinate));
  depart->latitude = 90;//43.570546;
  //depart->longitude = 2;//1.468281;
  struct gps_coordinate *dest;
  dest = malloc(sizeof(struct gps_coordinate));
  dest->latitude = -90;//43.571157;
  //dest->longitude = 2;//1.467671;
  
  double distance;// = malloc(sizeof(double));
  double var;

  double angle; //= malloc(sizeof(double));
  navigation(depart, dest, &distance, &angle); 
  //CU_ASSERT( (*distance - M_PI*6371*1000) < 1e-3) ;
	//CU_ASSERT( (*angle - (180)) < 1e-3);  
   
  
  dest->longitude = -180.0;
  depart->longitude = -180.0;
  var  = -180.0;
  while (var < 180.0)
  {
	printf("test with longitude of %f \n", dest->longitude);
	navigation(depart, dest, &distance, &angle);  
	var = var + 1.0;
	
	depart->latitude = 90.0;
	dest->latitude = -90.0;
	depart->longitude = var;
	dest->longitude = var;
	CU_ASSERT( (distance - M_PI*6371*1000) < 1e-5) ;
	CU_ASSERT( (angle - (180)) < 1e-5);
	
  }
}

/*
 *  Cette procédure de test vérifie que mafonction(i,i) = 2*i
 *  pour i de 0 à 99.
 *  
 *  Elle vérifie aussi que mafonction(i,i) est différent de i (faux pour i = 0).
*/
/* 
void test2_de_mafonction() {
  int i ;
  for (i = 0 ; i < 100 ; i++) {
    CU_ASSERT_EQUAL( mafonction(i, i), 2 * i ) ;
    
    /* Ce test est incorrect pour i = 0, c'est expres pour le voir apparaitre dans le rapport de test. 
    CU_ASSERT_NOT_EQUAL( mafonction(i, i), i ) ;
  }
  
}*/

/*
 * Creation d'une ou plusieurs suite de tests.
 */

void init_suites() {

  // Suite 1 : tests de l'addition.
  printf("Creation de la suite 1...\n") ;
  CU_pSuite suite_add = CU_add_suite("Tests de la fonction navigation", NULL, NULL) ;
  CU_ADD_TEST(suite_add, test1_de_navigation) ;
  //CU_ADD_TEST(suite_add, test2_de_mafonction) ;

  // Autres suites de tests.
}


int main(char** args) {

  // En cas de problème de la librairie, le programme de test s'arrete et affiche un message (par exemple : memory allocation failed).
  CU_set_error_action(CUEA_ABORT) ;

  // Initialisation
  printf("Initialisation des tests.\n") ;
  CU_initialize_registry() ;
  init_suites() ;
  
  // Lancement
  printf("Lancement des tests.\n") ;

  // Variante 1 : fabriquer un fichier XML
  CU_automated_run_tests() ;
  printf("Pour observer les resultats, ouvrir le fichier .xml dans firefox\n") ;
  printf("Copier le fichier de style dans le meme repertoire : \n") ;
  printf("cp /usr/local/share/CUnit/CUnit-Run.xsl .\n\n") ;
  
  // Variante 2 : interface interactive dans le terminal.
  //CU_curses_run_tests() ;
  CU_basic_run_tests();
  // Nettoyage final
  CU_cleanup_registry() ;
  
}

