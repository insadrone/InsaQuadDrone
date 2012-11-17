#include "config_bluetooth_connexion.h"


/* tests */
/*int main(void)
{
*/	// test read file to extract channel number
	/*int i=-3;

	i = read_file("/home/flo/tmp0.txt");
	printf("\n%d\n\n",i);*/


	/* test all */	
/*	if(config() < 0)
		return 1;
	else
		return 0;
}*/



/* ============================================================================	*/
/*					Config					*/
/*	Configure and connect to the bluetooth device corresponding to the MAC	*/
/*	address defined in the file .h						*/
/*	The device must have the application ShareGPS activated for its		*/
/*	corresponding channel will be searched					*/
/*										*/
/*	Return 0 if success, -1 otherwise					*/
/* ============================================================================	*/
int config()
{
	int channel = -2;
	char str[4];
	char cmd[40] = "rfcomm bind 0 ";
	
	
	// kill all bluetooth
	printf("Kill all bluetooth com...\t");
	if(system("rfcomm release all") < 0)
	{
		perror("Error killing bluetooth connection");
		return -1;
	}
	printf("Done\n");


	// saving all available bluetooth channels
	printf("Looking for all available channels...\n");
	if(system("sdptool records "MAC_ADDR">> /home/tmp.txt") < 0)
	{
		perror("Error saving bluetooth channels");
		return -1;
	}
	printf("Done\n");

	// extract channel of GPS coordinate
	printf("Looking for ShareGPS channel...\t");
	if( (channel = read_file("/home/tmp.txt")) < 0)
	{
		printf("Error finding ShareGPS channel\n");
		return -1;
	}
	printf("Done, channel: %d\n", channel);

	// remove temporary file
	if(system("rm /home/tmp.txt") < 0)
	{
		perror("Error removing temporary file");
	}
	
	// convert channel number to string
	printf("Binding device to rfcomm0...\t");
	sprintf(str, "%d", channel);
	strcat(cmd, MAC_ADDR);
	strcat(cmd, " ");
	strcat(cmd, str);

	// bind to the device using its MAC @ and the channel found
	if(system(cmd) < 0)
	{
		perror("Error binding to device");
		return -1;
	}
	printf("Done\n");

	return 0;
}



/* ============================================================================	*/
/*				Read_File					*/
/*	Read a file containing all bluetooth channels of a defined		*/
/*	bluetooth device.							*/
/*										*/
/*	Return the channel of the application ShareGPS, -1 if not found		*/
/* ============================================================================	*/
int read_file(char *file_name)
{
	FILE *fd;
	char str[100];
	int i = 0;

	/* Open file */
	if((fd = fopen(file_name,"r")) == NULL)
	{
		// error
		perror("\nError opening file\n");
	}

	/* read file to extract ShareGPS channel */
	while(fgets(str, sizeof(str), fd) != NULL)
	{
		/* looking for each Service Name line */
		if(str != NULL && str[0] == 'S' && str[8] == 'N')	// line Service Name
		{
			sscanf(str, "Service Name: %s", str);

			if(strcmp(str,"ShareGPS") == 0)	// look for ShareGPS channel
			{
				for(i=0; i < 7; i++)
					fgets(str, sizeof(str), fd); // read 7 lines, the 7th contains the channel number

				/* extract channel number from line */
				sscanf(str, "%s %d", str, &i);

				if(strcmp(str,"Channel:") == 0) // double check it's the channel line
				{
					fclose(fd);
					return i;
				}
			} // end if look channel
		} // end if
	} // end while read file

	/* Close file */
	fclose(fd);

	return -1;
}
