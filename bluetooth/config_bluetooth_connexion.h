/* ****************************************************************************	*/
/*			Config bluetooth connexion				*/
/*										*/
/*	This file will allow you to bind a bluetooth device to the port rfcomm0	*/
/*										*/
/*	You have to know the device's MAC address, ShareGPS should be running	*/
/*	and your bluetooth connexion must be turned ON				*/
/*										*/
/* ****************************************************************************	*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// MAC address of the device sharing its GPS coordinate via bluettooth and using the application ShareGPS
#define MAC_ADDR "90:C1:15:65:D2:02"


/* ============================================================================	*/
/*					Config					*/
/*	Configure and connect to the bluetooth device corresponding to the MAC	*/
/*	address above								*/
/*	The device must have the application ShareGPS activated for its		*/
/*	corresponding channel will be searched					*/
/*										*/
/*	Return 0 if success, -1 otherwise					*/
/* ============================================================================	*/
/*
	Input: none

	Output: none, return 0 if success, -1 otherwise
*/
int config();


/* ============================================================================	*/
/*				Read_File					*/
/*	Read a file containing all bluetooth channels of a defined		*/
/*	bluetooth device							*/
/*										*/
/*	Called by config()							*/
/*										*/
/*	Return the channel of the application ShareGPS, -1 if not found		*/
/* ============================================================================	*/
/*
	Input: - char*: string of the path to the file to read

	Output: none, return the channel found (int), -1 if nothing found
*/
int read_file(char *file_name);



