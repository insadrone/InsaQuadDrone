#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gps.h"

#define RAYON 6371.0*1000  //m

struct gps_coordinate pos_target[SAMPLE_NB_AVERAGE];
	// array of last SAMPLE_NB_AVERAGE position of target
	// used to calculate its average position
int position_array_pos_target;
	// Integer used to scan the array above (0< & <SAMPLE_NB_AVERAGE)
struct gps_coordinate average_pos_target;
	// average position of target
struct gps_coordinate variance_pos_target;
	// variance of last SAMPLE_NB_AVERAGE target positions

double pos_obstacle_left[OBS_NB_AVERAGE];
	// array of last OBS_NB_AVERAGE position of target
	// used to calculate its average position
double pos_obstacle_right[OBS_NB_AVERAGE];
	// array of last OBS_NB_AVERAGE position of target
	// used to calculate its average position
int position_array_pos_obstacle;
	// Integer used to scan the array above (0< & <SAMPLE_NB_AVERAGE)
double average_pos_obstacle_left;
	// average position of obstacle
double average_pos_obstacle_right;
	// average position of obstacle


double pos_obstacle_left2[OBS_NB_AVERAGE];
	// array of last OBS_NB_AVERAGE position of target
	// used to calculate its average position
int position_array_pos_obstacle2;
	// Integer used to scan the array above (0< & <SAMPLE_NB_AVERAGE)
double average_pos_obstacle_left2;
	// average position of obstacle


int initialisation_gps(struct gps_coordinate *depart,struct gps_coordinate *dest, struct gps_coordinate *error)
{
  if  (depart->latitude == -1.0 && depart->longitude == -1.0)  {
    printf("Coordinates depart undertermined \n");
    return -1;
  }
  else if ( dest->latitude == -1.0 && dest->longitude == -1.0 ){
    printf("Coordinates destination undertermined \n");
    return -1;
  }
  else{
    error->latitude = dest->latitude - depart->latitude;
    error->longitude = dest->longitude - depart->longitude;

    printf("erreur lat %f, erreur long %f\n",error->latitude,error->longitude);
    return 0;
  }
}


/*Extract coordinate gps from a string gprmc given by gps device                                         
 * INPUT: string gprmc given by gps device (char [])                                                      
 * OUTPUT: coordinate gps (struct gps_coordinate)                                                        
 */
void extract_coord( char str_gps[], struct gps_coordinate *point )
{
  char delims[] = ",";
  char *result;
  result = malloc(sizeof(char));
  int j,i = 0;
  result = strtok( str_gps, delims );

  char *degree ;
  degree = malloc(sizeof(char));
  char *minute ;
  minute = malloc(sizeof(char));
	
  while( i<6 ) 
    {
      i++;  
      result = strtok( NULL, delims );	     		        
      //printf( "result is %d \"%s\"\n", i, result );   	    	     
      if (!strcmp(result, "V") )//sortie la boucle quand GPS ne fonctionne pas
	{ i = 7; 
	  point->longitude = -1.0;	
	  point->latitude =  -1.0;	
	  printf( "coordinates undetermined \n" );
	} 
		
      if (i==3) //effecter la valeur de latitude
	{	
	  for (j=0;j<2;j++) //extract degree 
	    {
	      degree[j] = result[j]; 	
	    }
		
	  while (j< strlen(result)) //extract minute
	    {
	      minute[j-2] = result[j];
	      j++;		
	    }
	  printf( "latitude (degree minute) %s %s\n", degree , minute);
	  point->latitude = atof(degree) + atof(minute)/60.0 ;	//convert into degree   
	  printf( "latitude %f\n", point->latitude);
	}

      if (i==5) //effecter la valeur de longitude
	{	
	  for (j=0;j<3;j++) //extract degree
	    {
	      degree[j] = result[j]; 	
	    }
		
	  while (j< strlen(result)) //extract minute
	    {
	      minute[j-3] = result[j];
	      j++;		
	    }
	  printf( "longitude (degree minute) %s %s\n", degree , minute);
	  point->longitude = atof(degree) + atof(minute)/60.0 ; //convert into degree
	  printf( "longitude  %f\n\n", point->longitude);
       	}	     	
    }
}

void extract_error(char *gpgga_string, gps_error *g_error) {
  
  char *delims = ",";
  char *res;
  res = strtok(gpgga_string, delims);
  int i = 0;

  while (i < 8) {
    res = strtok(NULL, delims);

    if (i == 6) {
      g_error->sat_number = atoi(res);
    }

    if (i == 7) {
      g_error->hdop = atof(res);
    }

    i++;
  }
  
}



/* Calculate the distance and bearing between 2 coordinate gps                                           
 * INPUT: 2 coordinates gps (struct gps_coordinate)                                                       
 * OUTPUT: distance(m) and angle(m) (double)                                                              
 */
void navigation(struct gps_coordinate *depart,struct gps_coordinate *dest, double *distance, double *angle , struct gps_coordinate *error)
{
  double y,x;
  if  (depart->latitude == -1.0 && depart->longitude == -1.0)
    printf("Coordinates depart undertermined \n");
  else if ( dest->latitude == -1.0 && dest->longitude == -1.0 )
    printf("Coordinates destination undertermined \n");

  else
    {
      // convert to radian                                                                             
      if (!error) {//if error is null
      	depart->latitude = depart->latitude * M_PI/ 180.0 ;
      	depart->longitude = depart->longitude * M_PI/ 180.0 ;
      	dest->latitude = dest->latitude * M_PI/ 180.0;	
      	dest->longitude = dest->longitude * M_PI/ 180.0;
      }
      else	{ //                                                                            
      	depart->latitude = depart->latitude * M_PI/ 180.0 + error->latitude * M_PI/ 180.0;
      	depart->longitude = depart->longitude * M_PI/ 180.0 + error->longitude * M_PI/ 180.0;
      	dest->latitude = dest->latitude * M_PI/ 180.0;	
      	dest->longitude = dest->longitude * M_PI/ 180.0;}
      //==============//
      // calcul angle //
      //==============//
	
      //calcul direction // North
      y = sin(dest->longitude - depart->longitude) * cos(dest->latitude);
      x = cos(depart->latitude)*sin(dest->latitude) - sin(depart->latitude)*cos(dest->latitude)*cos(dest->longitude - depart->longitude);
      *angle = (atan2(y,x) * 180.0/M_PI);
 
      //================// 
      // calcul distance //
      //================//
     
      //     methode haverine
      x = sin((dest->latitude - depart->latitude)/2) * sin((dest->latitude - depart->latitude)/2) + sin((dest->longitude - depart->longitude)/2)*sin((dest->longitude - depart->longitude)/2) * cos(depart->latitude)*cos(dest->latitude);
      y = 2 * atan2(sqrt(x),sqrt(1-x));
      *distance = RAYON *y;

      printf("angle(degree) distance(m) %f %f \n", *angle , *distance);
    }
}


/* ================================================================================================= */
/* ================================================================================================= */

/*
 *	Calculate average position of the target
 * if new coord > tolerance (~10-15m) data won't be take into account
 *
 * Input: struct gps_coordinate, new GPS coord of the target
 * Output: struct gps_coordinate, average of last X position of the target (X defined in the .h file)
 * Return: 1 upon success, -1 otherwise
*/
int average_target_pos(struct gps_coordinate *new_pos, struct gps_coordinate *average_pos)
{
	int i = 0,j = 0;

	// check if new position is valid
	if(check_gps_coord_struc(new_pos) < 0)
		return -1;

	// check if an average has already been calculated
	if(check_gps_coord_struc(&average_pos_target) < 0)
	{	// if no (array empty), easy
		pos_target[position_array_pos_target].latitude = new_pos->latitude;
		pos_target[position_array_pos_target].longitude = new_pos->longitude;

		average_pos_target.latitude = new_pos->latitude;
		average_pos_target.longitude = new_pos->longitude;
		variance_pos_target.latitude = 0;
		variance_pos_target.longitude = 0;
	}
	// else scan array and calculate average and variance
	else
	{
		// add value to the array of positions if tolerance OK
		if((new_pos->latitude < (average_pos_target.latitude - TOLERANCE))
			|| (new_pos->latitude > (average_pos_target.latitude + TOLERANCE))
			|| (new_pos->longitude < (average_pos_target.longitude - TOLERANCE))
			|| (new_pos->longitude > (average_pos_target.longitude + TOLERANCE)))
		{
			return -1;
		}
		else 
		{
			pos_target[position_array_pos_target].latitude = new_pos->latitude;
			pos_target[position_array_pos_target].longitude = new_pos->longitude;
		}

		// init varaibles for calulation
		average_pos_target.latitude = 0;
		average_pos_target.longitude = 0;
		variance_pos_target.latitude = 0;
		variance_pos_target.longitude = 0;

		//scan array
		for(i = 0; i < SAMPLE_NB_AVERAGE; i++)
		{	// calculate if values OK
			if(check_gps_coord_struc(&pos_target[i]) == 1)
			{
				// Sum(Xi)
				average_pos_target.latitude += pos_target[i].latitude;
				average_pos_target.longitude += pos_target[i].longitude;
				// Sum(Xi²)
				variance_pos_target.latitude += (pos_target[i].latitude * pos_target[i].latitude);
				variance_pos_target.longitude += (pos_target[i].longitude * pos_target[i].longitude);
				j++;
			} // end if calcul with apporpriate values
		} // end for scan array

		// average = E(X) = Sum(Xi)/nb_sample
		average_pos_target.latitude = (average_pos_target.latitude / j);
		average_pos_target.longitude = (average_pos_target.longitude / j);
		// var = E(X²)-E(X)²
		variance_pos_target.latitude = ((variance_pos_target.latitude / j) - (average_pos_target.latitude * average_pos_target.latitude));
		variance_pos_target.longitude = ((variance_pos_target.longitude / j) - (average_pos_target.longitude * average_pos_target.longitude));
	} // end else from if no average has already been calculated

	// Update next position to write int he array (cyclic operating)
	if(position_array_pos_target == (SAMPLE_NB_AVERAGE-1))
		position_array_pos_target = 0;
	else
		position_array_pos_target++;

	// write results in the output structure
	average_pos->latitude = average_pos_target.latitude;
	average_pos->longitude = average_pos_target.longitude;

	return 1;	
}


/*
 *	Init array of last X position of the target
 *
 * Fill the array containing target positions with values equal -1
 * Has to be called once at the beggining of the program
*/
void init_array_target_pos(void)
{
	for(position_array_pos_target = 0; position_array_pos_target < SAMPLE_NB_AVERAGE; position_array_pos_target++)
	{
		pos_target[position_array_pos_target].latitude = -1;
		pos_target[position_array_pos_target].longitude = -1;
	}
	// init other variables
	position_array_pos_target = 0;
	average_pos_target.latitude = -1;
	average_pos_target.longitude = -1;
	variance_pos_target.latitude = -1;
	variance_pos_target.longitude = -1;
}


/*
 *	Check if a GPS coordinate is ready to process
 *
 * Input: struct gps_coordinate, GPS coord to check
 * Return: 1 upon success, -1 otherwise (=> struct null of values equal -1)
*/
int check_gps_coord_struc(struct gps_coordinate *check_me)
{
	if((check_me == NULL) || (check_me->latitude == -1) || (check_me->longitude == -1))
		return -1;
	else return 1;
}


/* ================================================================================================= */
/* ================================================================================================= */

/*
 *	Calculate average position of the obstacle
 * if new pos > tolerance (~10-15m) data won't be take into account
 *
 * Input: double double, new left & right obstacle position 
 * Output: double double, average of last X position of the obstacle (X defined in the .h file)
 * Return: 1 upon success, -1 otherwise
*/
int average_obstacle_pos(double *left, double *right, double *average_left, double *average_right)
{
	int i = 0,j = 0;

	// check if new position is valid
	if((*left < 0) || (*right < 0))
		return -1;

	// check if an average has already been calculated
	if((average_pos_obstacle_left < 0) || (average_pos_obstacle_right < 0))
	{	// if no (array empty), easy
		pos_obstacle_left[position_array_pos_obstacle] = *left;
		pos_obstacle_right[position_array_pos_obstacle] = *right;

		average_pos_obstacle_left = *left;
		average_pos_obstacle_right = *right;
	}
	// else scan array and calculate average and variance
	else
	{
		// add value to the array of positions if tolerance OK
		if((*left < (average_pos_obstacle_left - SENSOR_TOLERANCE))
			|| (*left > (average_pos_obstacle_left + SENSOR_TOLERANCE))
			|| (*right < (average_pos_obstacle_right - SENSOR_TOLERANCE))
			|| (*right > (average_pos_obstacle_right + SENSOR_TOLERANCE)))
		{
			return -1;
		}
		else 
		{
			pos_obstacle_left[position_array_pos_obstacle] = *left;
			pos_obstacle_right[position_array_pos_obstacle] = *right;
		}

		// init variables for calulation
		average_pos_obstacle_left = 0;
		average_pos_obstacle_right = 0;

		//scan array
		for(i = 0; i < OBS_NB_AVERAGE; i++)
		{	// calculate if values OK
			if((pos_obstacle_left[i] > 0) && (pos_obstacle_right[i] > 0))
			{
				// Sum(Xi)
				average_pos_obstacle_left += pos_obstacle_left[i];
				average_pos_obstacle_right += pos_obstacle_right[i];
				j++;
			} // end if calcul with apporpriate values
		} // end for scan array

		// average = E(X) = Sum(Xi)/nb_sample
		average_pos_obstacle_left = (average_pos_obstacle_left / j);
		average_pos_obstacle_right = (average_pos_obstacle_right / j);

	} // end else from if no average has already been calculated

	// Update next position to write int he array (cyclic operating)
	if(position_array_pos_obstacle == (OBS_NB_AVERAGE-1))
		position_array_pos_obstacle = 0;
	else
		position_array_pos_obstacle++;

	// write results in the output structure
	*average_left = average_pos_obstacle_left;
	*average_right = average_pos_obstacle_right;

	return 1;	
}


int average_obstacle_pos2(double *left, double *average_left)
{
	int i = 0,j = 0;

	// check if new position is valid
	if(*left < 0)
		return -1;

	// check if an average has already been calculated
	if(average_pos_obstacle_left2 < 0)
	{	// if no (array empty), easy
		pos_obstacle_left2[position_array_pos_obstacle2] = *left;

		average_pos_obstacle_left2 = *left;
	}
	// else scan array and calculate average and variance
	else
	{
		// add value to the array of positions if tolerance OK
		if((*left < (average_pos_obstacle_left2 - SENSOR_TOLERANCE))
			|| (*left > (average_pos_obstacle_left2 + SENSOR_TOLERANCE)))
		{
			return -1;
		}
		else 
		{
			pos_obstacle_left2[position_array_pos_obstacle2] = *left;
		}

		// init variables for calulation
		average_pos_obstacle_left2 = 0;

		//scan array
		for(i = 0; i < OBS_NB_AVERAGE; i++)
		{	// calculate if values OK
			if(pos_obstacle_left2[i] > 0)
			{
				// Sum(Xi)
				average_pos_obstacle_left2 += pos_obstacle_left2[i];
				j++;
			} // end if calcul with apporpriate values
		} // end for scan array

		// average = E(X) = Sum(Xi)/nb_sample
		average_pos_obstacle_left2 = (average_pos_obstacle_left2 / j);

	} // end else from if no average has already been calculated

	// Update next position to write int he array (cyclic operating)
	if(position_array_pos_obstacle2 == (OBS_NB_AVERAGE-1))
		position_array_pos_obstacle2 = 0;
	else
		position_array_pos_obstacle2++;

	// write results in the output structure
	*average_left = average_pos_obstacle_left2;

	return 1;	
}


/*
 *	Init array of last X position of the obstacle
 *
 * Fill the array containing obstacle positions with values equal -1
 * Has to be called once at the beggining of the program
*/
void init_array_obstacle_pos(void)
{
	for(position_array_pos_obstacle = 0; position_array_pos_obstacle < OBS_NB_AVERAGE; position_array_pos_obstacle++)
	{
		pos_obstacle_left[position_array_pos_obstacle] = -1;
		pos_obstacle_right[position_array_pos_obstacle] = -1;
	}
	// init other variables
	position_array_pos_obstacle = 0;
	average_pos_obstacle_left = -1;
	average_pos_obstacle_right = -1;
}


void init_array_obstacle_pos2(void)
{
	for(position_array_pos_obstacle2 = 0; position_array_pos_obstacle2 < OBS_NB_AVERAGE; position_array_pos_obstacle2++)
	{
		pos_obstacle_left2[position_array_pos_obstacle2] = -1;
	}
	// init other variables
	position_array_pos_obstacle2 = 0;
	average_pos_obstacle_left2 = -1;
}


/* ================================================================================================= */
/*	basic test for functions average_target_pos, init_array_target_pos and check_gps_coord_struc

int main()
{
struct gps_coordinate *new_pos, *average_pos;
new_pos = malloc(sizeof(struct gps_coordinate));
average_pos = malloc(sizeof(struct gps_coordinate));

new_pos->latitude = 100;
new_pos->longitude = 50;

printf("\ninit...\n");
init_array_target_pos();

printf("\ncall 1\n");
average_target_pos(new_pos, average_pos);
printf("\nresults : %f - %f\n", average_pos->latitude, average_pos->longitude);

printf("\n\ncall 2\n");
new_pos->latitude = 110;
new_pos->longitude = 55;
average_target_pos(new_pos, average_pos);
printf("\nresults : %f - %f\n", average_pos->latitude, average_pos->longitude);

printf("\n\ncall 2\n");
new_pos->latitude = 200;
new_pos->longitude = 50;
average_target_pos(new_pos, average_pos);
printf("\nresults : %f - %f\n", average_pos->latitude, average_pos->longitude);

printf("\n\nresults : %f - %f - %f - %f\n", average_pos_target.latitude, average_pos_target.longitude, variance_pos_target.latitude, variance_pos_target.longitude);

return 0;
}*/



//	basic test for functions average_target_obstacle, init_array_target_obstacle
/*
int main()
{
double *new_pos_left, *new_pos_right, *average_pos_l, *average_pos_r;
new_pos_left = malloc(sizeof(double));
average_pos_l = malloc(sizeof(double));
new_pos_right = malloc(sizeof(double));
average_pos_r = malloc(sizeof(double));

*new_pos_left = 100;
*new_pos_right = 50;

printf("\ninit...\n");
init_array_obstacle_pos();

printf("\ncall 1\n");
average_obstacle_pos(new_pos_left, new_pos_right, average_pos_l, average_pos_r);
printf("\nresults : %f - %f\n", *average_pos_l, *average_pos_r);

printf("\n\ncall 2\n");
*new_pos_left = 101;
*new_pos_right = 49;
average_obstacle_pos(new_pos_left, new_pos_right, average_pos_l, average_pos_r);
printf("\nresults : %f - %f\n", *average_pos_l, *average_pos_r);

printf("\n\ncall 2\n");
*new_pos_left = 200;
*new_pos_right = 100;
average_obstacle_pos(new_pos_left, new_pos_right, average_pos_l, average_pos_r);
printf("\nresults : %f - %f\n", *average_pos_l, *average_pos_r);

printf("\n\ncall 3\n");
*new_pos_left = 101;
*new_pos_right = 50;
average_obstacle_pos(new_pos_left, new_pos_right, average_pos_l, average_pos_r);
printf("\nresults : %f - %f\n", *average_pos_l, *average_pos_r);

printf("\n\nresults : %f - %f\n",  *average_pos_l, *average_pos_r);

return 0;
}*/


