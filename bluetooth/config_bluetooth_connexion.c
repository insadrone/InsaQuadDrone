#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAC_ADDR "90:C1:15:65:D2:02"


int config()
{
	system("rfcomm release all");	// kill all bluetooth
	system("sdptool records "MAC_ADDR">> tmp.txt");	// looking for right channel

	
return 0;
}



int read_file(char *file_name)
{
	FILE *fd;
	char str[100], tmp[20];
	int i = 0;

	/* Open file */
	if((fd = fopen(file_name,"r")) == NULL)
	{
		// error
		perror("kikoo");
	}

	while(fscanf(fd,"%s\n", str) != EOF)
	{
		if(str != NULL && str[0] == 'S' && str[8] == 'N')	// line Service Name
		{
			sscanf(str, "Service Name: %s", tmp);	printf("%s\t%s\n",str,tmp);

			if(strcmp(tmp,"ShareGPS") == 0)	// look for its channel
			{
				for(i=0; i < 7; i++)
				{
					fscanf(fd,"%s",str); // read 7 lines, the 7th contains the channel number
printf("%s\n",str);
				}
				sscanf(str, "Channel: %d", &i); printf("%s\t%d\n",str,i);

				fclose(fd);

				return i;
			} // end if look ch			
		} // end if
	} // end while read file

	/* Close file */
	fclose(fd);

	return -1;
}



int main(void)
{
	int i=-3;

	i=read_file("/home/flo/tmp.txt");
	printf("\n%d\n\n",i);
	return 0;
}
