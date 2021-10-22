 #include "gps.h"
#include "dht.h"
#include "bmp.h"
#include "mq.h"
#include "mmm.h"
#include <SPI.h> //Library for SPI communication (Pre-Loaded into Arduino)
#include <SD.h> //Library for SD card (Pre-Loaded into Arduino)
dht DHT ;
bmp BMP;
gps GPS;
mq5 MQ2;
mmm Mpu;
const int chipSelect = 4;
void setup()
{
    Serial.begin(9600);
Initialize_SDcard();
   BMP.set_BMP();
 Mpu.set_mpu();
    

}  

void loop()
{
 
Write_SDcard();
DHT.READ_DHT();
BMP.READ_BMP();
MQ2.READ_MQ();
GPS.READ_GPS();
Mpu.read_gyro();
//read_SDcard();

}

void Initialize_SDcard()
{
  
 if (!SD.begin(chipSelect))
  {
    //Serial.println("Card failed, or not present");
    return;
  }
  File myfile = SD.open("testaaaa.txt", FILE_WRITE);
  if (myfile)
  {
   
    myfile.close();
  }
  
}
void Write_SDcard()
{
 File myfile = SD.open("testaaaa.txt", FILE_WRITE);
  if (myfile)
  {
     myfile.print("DHT,");
     myfile.print( DHT.read_Humidity () );
     myfile.print(",");
     myfile.print(DHT.read_Temperature ());
     myfile.println(",");
     myfile.print("MQ,");
     myfile.print( MQ2.read_MQ());
     myfile.println(",");
     myfile.print("BMP,");
     myfile.print( BMP.read_T() );
     myfile.print(",");
     myfile.print(BMP.read_P());
     myfile.print(",");
     myfile.print(BMP.read_A());
     myfile.println(",");
   myfile.print("GPS,");
     myfile.print( GPS.get_long() );
     myfile.print(",");
     myfile.print(GPS.get_lat());
     myfile.print(",");
     myfile.print(GPS.read_num());
     myfile.print(",");
     myfile.print(GPS.read_hours());
     myfile.print(":");
     myfile.print( GPS.read_min());
     myfile.print(":");
     myfile.print(GPS.read_sec()); 
     myfile.println("/");
     myfile.close();
     Serial.println("SD,T,");
    
  }
  else
  {
    
    Serial.println("SD,F,");
  }
  
}
void read_SDcard()
{
   File myfile = SD.open("testaaa.txt");

  if (myfile) 
  {
    while (myfile.available()) 
    {
      Serial.write(myfile.read());
    }
    myfile.close();
  }
  
  else {
    Serial.println("error opening datalog.txt");
  }
 }
 
