//These code didn't use any external libraries ... just DataSheet :)

/*
The distinctive in our project :

1- We used new module called 10 dof imu 
     These module contain BMP280 & MPU9250 in one chip
     That save space so we could make all the project in 3 PCBs single layer 
     BMP280 data is more acurate than BMP180
     MPU9250 has a magnometer so we have bonus data (compass)
     
2-We have Bonus Sensor (MQ135) that would bd in the roof of the structre  testing air Quality as insitu data
     MQ135 gas sensor has high sensitivity to ammonia gas, sulfide, benzene series steam, also can monitor smoke and other toxic gases well. 
     
3-We used OOP (classes & functions) and added alot of comments to make the code more readable 
     
*/
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>
#include <I2C.h>

#include "MPU9250.h"
#include "MQ135.h"
#include "BMP280.h"
#include "DHT.h"
#include "GPS.h"
#include "SD_Card.h"

MPU_9250 mpu;
mq_135 gas;
BMP_280 bmp;

DHT dht;
GPS gps;
SD_Card sd_card;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
      
  mpu.MPU9250_Setub();
  bmp.BMP280_Setup();
  gas.MQ135_Setup();
}

void loop() {
//put your main code here, to run repeatedly:
  mpu.MPU9250_Readings();
  bmp.BMP280_Readings();
  dht.DHT_Readings();
  gas.MQ135_Readings();
  gps.GPS_Readings();
//sd_card.SD_Reading_Data();
}
