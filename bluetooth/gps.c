#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gps.h"

//const double R = 6371.0*1000; //m

void extract_coord( char str_gps[], struct gps_coordinate *point )
{
     	char delims[] = ",";
     	char *result;
	result = malloc(sizeof(char));
     	int j,i = 0;
	result = strtok( str_gps, delims );
//	double x;
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
/*
void navigation(struct gps_coordinate *depart,struct gps_coordinate *dest, double *distance, double *angle )
{
     double y,x,z;
    if  (depart->latitude == -1.0 && depart->longitude == -1.0)  
	printf("Coordinates depart undertermined \n");
    else if ( dest->latitude == -1.0 && dest->longitude == -1.0 )
	printf("Coordinates destination undertermined \n");        
 
    else
    {	
	// convert to radian
     depart->latitude = depart->latitude * M_PI/ 180.0;
     depart->longitude = depart->longitude * M_PI/ 180.0;
     dest->latitude = dest->latitude * M_PI/ 180.0;	
     dest->longitude = dest->longitude * M_PI/ 180.0;
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
     *distance = R*y;

     printf("angle(degree) distance(m) %f %f \n", *angle , *distance);
	}
}

int main ()
{//coordonne de depart
    char str_gps[] = "$GPRMC,220516,A,4851.600,N,00220.4000,W,173.8,231.8,130694,004.2,W*70";
    struct gps_coordinate *depart;
    depart = malloc(sizeof(struct gps_coordinate));
    printf("%s\n", str_gps);
    extract_coord(str_gps,depart);
   //coordonnne de dest 
    struct gps_coordinate *dest;
    dest = malloc(sizeof(struct gps_coordinate));
    char str_gps_dest[] =  "$GPRMC,220516,A,5038.400,N,00304.200,E,173.8,231.8,130694,004.2,W*70";	
    printf("%s\n", str_gps_dest);
    extract_coord(str_gps_dest,dest);

//    depart->latitude = 	43.570;
//    depart->longitude = 1.465  ;  
//    dest->latitude = 43.570	;
//    dest->longitude = 	1.465;
    double *distance, *angle;
    angle = malloc(sizeof(double));
    distance = malloc(sizeof(double));
    navigation(depart, dest, distance, angle);
    printf("angle distance %f %f \n", *angle , *distance);
    
return 0;
}*/

