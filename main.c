#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    FILE* fp;
    char *str;
    char tval1,tval2,tval3,tval4;
    char sval1,sval2,sval3,sval4;
    size_t num_of_bytes = 6;
    float percentage;
    long int period = 20000L;
    char sd=0;
    float korak;

    
    fp = fopen("/dev/led", "w");    
    if(fp == NULL)
    {
        printf("Problem pri otvaranju /dev/led\n");
        return -1;
    }
    fputs("0x00\n", fp);
    if(fclose(fp))
    {
        printf("Problem pri zatvaranju /dev/led\n");
        return -1;
    }


    while(1) 
    {

//Citanje vrednosti tastera
		fp = fopen ("/dev/switch", "r");
		if(fp==NULL)
		{
			puts("Problem pri otvaranju /dev/switch");
			return -1;
		}


		str = (char *)malloc(num_of_bytes+1); 
		getline(&str, &num_of_bytes, fp); 

		if(fclose(fp))
		{
			puts("Problem pri zatvaranju /dev/switch");
			return -1;
		}


		sval1 = str[2] - 48;
		sval2 = str[3] - 48;
		sval3 = str[4] - 48;
		sval4 = str[5] - 48;
		free(str);

	if(sval1 == 0 && sval2 == 0) korak =0.05;		
	if(sval1 == 0 && sval2 == 1)  korak =0.1;	
	if(sval1 == 1 && sval2 == 0) korak =0.15;	
	if(sval1 == 1 && sval2 == 1) korak =0.2;	
       
        fp = fopen ("/dev/button", "r");
	if(fp==NULL)
	{
	    puts("Problem pri otvaranju /dev/button");
	    return -1;
	}
        
	str = (char *)malloc(num_of_bytes+1); 
	getline(&str, &num_of_bytes, fp); 

	tval1 = str[2] - 48;
	tval2 = str[3] - 48; 
	free(str);

	if(tval1 == 1 && sd ==0)
	{
	    sd = 1;
	    if(percentage >0) 
                percentage-=korak;  
	}

	
	if(tval2 == 1 && sd ==0)
	{
	    sd = 1;
	    if(percentage < 1) 
                percentage += korak;  
	}

	if(tval1 == 0 && tval2 == 0)
	    sd = 0;

	if(fclose(fp))
	{
            puts("Problem pri zatvaranju /dev/button");
	    return -1;
	}
       
		// Upali diode
		fp = fopen("/dev/led", "w");
		if(fp == NULL)
		{
			printf("Problem pri otvaranju /dev/led\n");
			return -1;
		}
		fputs("0x0F\n", fp);
		if(fclose(fp))
		{
			printf("Problem pri zatvaranju /dev/led\n");
			return -1;
		}
		usleep(percentage*period);

		// Ugasi diode
		fp = fopen("/dev/led", "w");
		if(fp == NULL)
		{
			printf("Problem pri otvaranju /dev/led\n");
			return -1;
		}
		fputs("0x00\n", fp);
		if(fclose(fp))
		{
			printf("Problem pri zatvaranju /dev/led\n");
			return -1;
		}
		usleep((1-percentage)*period);	

    }
    return 0;
}
