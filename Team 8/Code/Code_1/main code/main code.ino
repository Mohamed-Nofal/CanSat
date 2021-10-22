  #include "sd_card.h"
  
  #include "DHT.h"
  #include "BMP.h"
  #include "GPS.h"
  #include "MPU.h"

  DHT_CL dht ; 
  BMP_CL bmp;
  GPS_CL gps;
  MPU_CL mpu;
  SD_CL sd;
  

void setup() {
  
  Serial.begin(9600);
    
   dht.dht1();
    bmp.bmp1();
    gps.gps1();
    mpu.mpu1();
    sd.SD_card_1();
}


void loop() 
{
   
 for(int i=0; i<20; i++)  
 {  
   
   bmp.bmp2();
   mpu.mpu2();
   sd.SD_card_2(gps, dht, mpu, bmp);
 }
 dht.dht2();
}

void serialEvent()
{
  gps.gps2();
} 
