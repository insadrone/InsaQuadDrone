#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct gps_coordinate{
   double latitude; 
   double longitude;
};

void extract_coord( char str_gps[], struct gps_coordinate *point );
//void navigation(struct gps_coordinate *depart,struct gps_coordinate *dest, double *distance, double *angle );
