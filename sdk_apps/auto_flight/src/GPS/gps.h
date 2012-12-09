#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct gps_coordinate{
   double latitude; 
   double longitude;
};

typedef struct gps_error_t {
  int sat_number;
  double hdop;
} gps_error;

int initialisation_gps(struct gps_coordinate *depart,struct gps_coordinate *dest, struct gps_coordinate *error);

void extract_coord( char str_gps[], struct gps_coordinate *point );

void extract_error(char *gpgga_string, gps_error *g_error);

void navigation(struct gps_coordinate *depart,struct gps_coordinate *dest, double *distance, double *angle , struct gps_coordinate *error_gps);


