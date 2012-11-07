#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const double R = 6371.0*1000; //m
struct gps_coordinate{
   double latitude; 
   double longitude;
};

int gps( char* str_gps, struct gps_coordinate *p );
void navigation(struct gps_coordinate *depart,struct gps_coordinate *dest, double *distance, double *angle );
