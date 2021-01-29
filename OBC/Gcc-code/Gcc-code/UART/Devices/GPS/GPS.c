/*
 * GPS.c
 *
 * Created: 1/21/2021 3:13:26 AM
 *  Author: mizo
 */ 
#include "GPS.h"
uint8_t readGPS(GPS *gps){
	uint8_t commas =0;
	char longi[13];
	char lati[13];
	char num[2];
	uint8_t i = 1;
	while(1){
		if (receiveByteUART()=='$'&&
			receiveByteUART()=='G'&&
			receiveByteUART()=='P'&&
			receiveByteUART()=='G'&&
			receiveByteUART()=='G'&&
			receiveByteUART()=='A')
		{	
			while(1){
			char temp = receiveByteUART();
			if(commas==2&&temp!=','){
				*(lati+i) = temp;
				i++;
			}
			if(commas==3&&temp!=','){
				if (temp=='N')
				{
					for (int u =0 ;u<12;u++)
					{
						lati[u]=lati[u+1];
					}
					lati[12]='0';
				}else{
					lati[0]='-';
				}
				
			}
			if(commas==4&&temp!=','){
				*(longi+i) = temp;
				i++;
			}
			if(commas==5&&temp!=','){
				if (temp=='E')
				{
					for (int u =0 ;u<12;u++)
					{
						longi[u]=longi[u+1];
					}
					longi[12]='0';
					}else{
					longi[0]='-';
				}
				
			}
			if(commas==7&&temp!=','){
				*(num+i-1) = temp;
				i++;
				if(i==2)
				goto ending;
			}
			if(temp==','){
				commas++;
				i=1;
				}
			}
			
		}
	}
	ending:
	sscanf(lati,"%f",&(*gps).latitude);
	sscanf(longi,"%f",&(*gps).longitude);
	sscanf(num,"%d",&(*gps).NoS);


}
