#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SAMPLE_NB_AVERAGE 10	// number of sample used for average calculation of target postion
#define TOLERANCE 15.0/(30.9*3600.0)	// distance max that new position of target can be from its average position
/*1 degree = 111.2km     1 minute = 1853m   1 second = 30.9m  => x (m) = x/(30.9 *3600) (degree)
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


/*
 *	Calculate average position of the target
 *
 * Input: struct gps_coordinate, new GPS coord of the target
 * Output: struct gps_coordinate, average of last X position of the target (X defined in the .h file)
 * Return: 1 upon success, -1 otherwise
*/
int average_target_pos(struct gps_coordinate *new_pos, struct gps_coordinate *average_pos);


/*
 *	Init array of last X position of the target
 *
 * Fill the array containing target positions with values equal to -1
 * Has to be called once at the beggining of the program
*/
void init_array_target_pos(void);


/*
 *	Check if a GPS coordinate is ready to process
 *
 * Input: struct gps_coordinate, GPS coord to check
 * Return: 1 upon success, -1 otherwise (=> struct null of values equal -1)
*/
int check_gps_coord_struc(struct gps_coordinate *check_me);
