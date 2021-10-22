
/////////////////////////////////
////////SD//////////////////
#include <SPI.h>
#include <SD.h>
File READINGS ;
////////////////////
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif


MPU6050 mpu;






#define OUTPUT_READABLE_YAWPITCHROLL





bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
#include <Wire.h>

#include "Adafruit_VEML6070.h"
Adafruit_VEML6070 uv = Adafruit_VEML6070();
// GPS FUCTION DECLERATION ;
char MID[6];
char UTC[11];
char LaT[10];
char NSInd[2];
char Long[11];
char EWInd[2];
char PosFix[2];
char sat[3] ;
char ALtitude[7];

short int latit [10];
short int Longit[11];

int c;

int Latitude ,longtitude , latitude_1=0 ,latitude_2=0 ,longtitude_1 =0,longtitude_2=0 ;

void GPS_LOOP(){

if (Serial.read() == '$')
{
  
  Serial.readBytesUntil(',',MID ,6);
  MID[5]= '\0';
 
if(MID[2] == 'G'&& MID[3] == 'G' && MID[4]=='A') {
  c += 1;
  Serial.readBytesUntil(',',UTC ,11);
  Serial.readBytesUntil(',',LaT ,10);
  Serial.readBytesUntil(',',NSInd ,2);
  Serial.readBytesUntil(',',Long ,11);
  Serial.readBytesUntil(',',EWInd ,2);
  Serial.readBytesUntil(',',PosFix ,2);
  Serial.readBytesUntil(',',sat ,3);
  Serial.readBytesUntil(',',ALtitude ,3);
  Serial.readBytesUntil(',',ALtitude ,7);

  UTC[10] ='\0';
  LaT[10]= '\0';
  NSInd[1]= '\0';
  Long[10]='\0';
  EWInd[1] ='\0';
  PosFix[1] ='\0';
  sat[2] ='\0';
  ALtitude[6] ='\0';
 
for(int i=0;i<11;i++)  // to convert latitude to crrect form :
{
  latit[i] =LaT[i]-'0' ;
  }

for(int n=0;n<2;n++)
 {
latitude_1+= latit[n]* pow(10,n);
}

for(int s =2;s<11;s++)
 {
latitude_2 += latit[s]* pow(10,s);
}

Latitude = latitude_1 +latitude_2/60;

for( int l=0;l<11;l++) // to convert longitude to correct form :
{
  Longit[l] =Long[l]-'0' ;
  }

for(int j=0;j<3;j++)
 {
longtitude_1+= latit[j]* pow(10,j);
}

for(int k=3;k<11;k++)
 {
longtitude_2 += latit[k]* pow(10,k);
}

longtitude = longtitude_1 +longtitude_2/60;


 Serial.print("GPS");
 Serial.print("/");

 Serial.print(MID); 
 Serial.print(",");
 Serial.print(UTC);        Serial.print(",");      
 Serial.print(Latitude);
 Serial.print(",");  
 Serial.print(NSInd);
 Serial.print(","); 
 Serial.print(longtitude);        Serial.print(",");      
 Serial.print(EWInd);
 Serial.print(",");     
 Serial.print(PosFix); 
  Serial.print(",");  
 Serial.print(sat);
 Serial.print(",");      
 Serial.print(ALtitude);
 Serial.println("/");

}
}
}

// DHT FUCTION DECLERATION ;

#define pin 13

int expectPulse (bool lvl)
{
  int count = 0;
  while (digitalRead (pin) == lvl)
    count++;
  return count;
}
unsigned char RH_int = 0, RH_dec = 0, T_int = 0, T_dec = 0, sum = 0;
void DHT_LOOP ()
{
unsigned char RH_int = 0, RH_dec = 0, T_int = 0, T_dec = 0, sum = 0;
  unsigned char pulse[80];
  pinMode (pin, OUTPUT);
  digitalWrite (pin, HIGH);
  delay (1500);
  digitalWrite (pin, LOW);
  delay (18);
  digitalWrite (pin, HIGH);
  delayMicroseconds (40);
  pinMode (pin, INPUT);
  if (digitalRead (pin) == LOW)
    {
      while (digitalRead (pin) == LOW)
  {
    delayMicroseconds(55);
  }
      while (digitalRead (pin) == HIGH)
  {
    delayMicroseconds(80);
  }
    }
  
  for (int i = 0; i < 80; i++)
    pulse[i] = expectPulse (i % 2);
  for (int i = 0; i < 40; i++)
    {
      unsigned char lowCycle = pulse[i * 2];
      unsigned char highCycle = pulse[(i * 2) + 1];
      if (i < 8)
  {
    RH_int <<= 1;
    if (highCycle > lowCycle)
      RH_int |= 1;
  }
      if (i < 16)
  {
    RH_dec <<= 1;
    if (highCycle > lowCycle)
      RH_dec |= 1;
  }
      if (i < 24)
  {
    T_int <<= 1;
    if (highCycle > lowCycle)
      T_int |= 1;
  }
      if (i < 32)
  {
    T_dec <<= 1;
    if (highCycle > lowCycle)
      T_dec |= 1;
  }
      if (i < 40)
  {
    sum <<= 1;
    if (highCycle > lowCycle)
      sum |= 1;
  }
    }
  if (sum != RH_int + RH_dec + T_int + T_dec)
    {
      Serial.println ("ERROR !!!");
    }
  else
    {

      Serial.print ("DHT");
      Serial.print ("/");
      Serial.print (RH_int);
      Serial.print (".");
      Serial.print (RH_dec);
      Serial.print (",");
      Serial.print (T_int);
      Serial.print (".");
      Serial.print (T_dec);
      Serial.println ("/");
    }
  return 0;
}

/*-----------------------------------------------------------------------------------------
  ----------------------------------------------------------------------------------------- */
//BMP SENSOR function declaration  :
#include<Wire.h>
#define BMP 0x77
#define AC1 0xAA
#define AC2 0xAC
#define AC3 0xAE
#define AC4 0xB0
#define AC5 0xB2
#define AC6 0xB4
#define B1 0xB6
#define B2 0xB8
#define MB 0xBa
#define MC 0xBC
#define MD 0xBE

char oss = 0;

long UT, UP, X1, X2, X3, B_3, B_5, B_6;
unsigned long B_4, B_7, T, P;

short AC_1, AC_2, AC_3, B_1, B_2, M_C, M_D, M_B;
unsigned short AC_4, AC_5, AC_6;

void BMP_SETUP ()
{
  AC_1 = Read_data (BMP, AC1, 2);
  AC_2 = Read_data (BMP, AC2, 2);
  AC_3 = Read_data (BMP, AC3, 2);
  AC_4 = Read_data (BMP, AC4, 2);
  AC_5 = Read_data (BMP, AC5, 2);
  AC_6 = Read_data (BMP, AC6, 2);
  B_1 = Read_data (BMP, B1, 2);
  B_2 = Read_data (BMP, B2, 2);
  M_B = Read_data (BMP, MB, 2);
  M_C = Read_data (BMP, MC, 2);
  M_D = Read_data (BMP, MD, 2);
}



long Read_data (int slave, int reg, int num)
{
  long temp;
  Wire.beginTransmission (slave);
  Wire.write (reg);
  Wire.endTransmission ();
  Wire.requestFrom (slave, num);

  if (Wire.available () == num)
    {
      temp = Wire.read ();  // temp =00000000 ,0101010101
      for (int i = 0; i < num - 1; i++)
  {
    temp = temp << 8;
    // temp = 01010101 ,00000000
    temp = temp | (Wire.read ());
    // from the second wire temp =01010101|new data 

  }

      return temp;
    }
}

long Temperature ()
{

  X1 = (UT - AC_6) * AC_5 / pow (2, 15);
  X2 = (M_C * pow (2, 11)) / (X1 + M_D);
  B_5 = X1 + X2;
  T = (B_5 + 8) / pow (2, 4);
  return T;

}

long Pressure ()
{

  B_6 = B_5 - 4000;
  X1 = (B_2 * (B_6 * B_6 / pow (2, 12))) / pow (2, 11);
  X2 = AC_2 * B_6 / pow (2, 11);
  X3 = X1 + X2;
  B_3 = (((AC_1 * 4 + X3) << oss) + 2) / 4;
  X1 = AC_3 * B_6 / pow (2, 13);
  X2 = (B_1 * (B_6 * B_6 / pow (2, 12))) / pow (2, 16);
  X3 = ((X1 + X2) + 2) / pow (2, 2);
  B_4 = AC_4 * (unsigned long) (X3 + 32768) / pow (2, 15);
  B_7 = ((unsigned long) UP - B_3) * (50000 >> oss);
  if (B_7 < 0x80000000)
    {
      P = (B_7 * 2) / B_4;
    }
  else
    {
      P = (B_7 / B_4) * 2;
    }
  X1 = (P / pow (2, 8)) * (P / pow (2, 8));
  X1 = (X1 * 3038) / pow (2, 16);
  X2 = (-7357 * P) / pow (2, 16);
  P = P + (X1 + X2 + 3791) / pow (2, 4);
                                                                                                                                                       
 return P;

}

void BMP_LOOP ()
{
  //read uncompensated  temperature :
  Wire.beginTransmission (BMP);
  Wire.write (0xf4);
  Wire.write (0x2E);
  Wire.endTransmission (BMP);
  delay (4.5);

  UT = Read_data (BMP, 0xF6, 2);


//read uncompensated  pressure  :


  Wire.beginTransmission (BMP);
  Wire.write (0xF4);
  Wire.write (0x34 | (oss << 6));
  Wire.endTransmission (BMP);
  delay (5);      //wait 

  UP = Read_data (BMP, 0xF6, 3);
  UP = UP >> (8 - oss);

  Serial.print ("BMB");
  Serial.print ("/ ");
  Serial.print (Temperature()/ 10.0);
 
  Serial.print (",");
  Serial.print (Pressure()/ 100.0);
  
  Serial.println ("/");

  //delay (1000);
}


/*---------------------------------------------------------------------------
---------------------------------------------------------------------------*/
// mpu code function declaration

void mpu_ooh(){// if programming failed, don't try to do anything
    if (!dmpReady) return;
    // read a packet from FIFO
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet 
    
        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print("MPU/");
            Serial.print(ypr[2] * 180/M_PI);
            Serial.print(",");
            Serial.print(ypr[1] * 180/M_PI);     
            Serial.print(",");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.println("/");
        #endif

      
    }}


void UV_LOOP() 
{
Serial.print("UV/"); 

  Serial.print(uv.readUV());
  Serial.println("/"); 
  delay(1000);

  }
 void SD_card()     
{
  while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}

if (!SD.begin(10)) {
Serial.println("f");
while (1);
}
  

 READINGS = SD.open("DATA11.txt",FILE_WRITE) ;

if(READINGS){

READINGS.println(F("GPS"));
 
//READINGS.print(F("mid:"));
READINGS.print(MID); 


 
READINGS.print(F("Lat:"));
READINGS.print(Latitude);  

READINGS.print(F("NS:"));
READINGS.print(NSInd );
  
READINGS.print(F("Long"));
READINGS.print(longtitude);  

READINGS.print(F("EW"));
READINGS.print(EWInd);  

READINGS.print(F("pos:"));
READINGS.print( PosFix);  

READINGS.print(F("Sat"));
READINGS.print(sat);  

READINGS.print(F("Alt"));
READINGS.println(ALtitude);
  
READINGS.println(F("dht"));

//READINGS.println(F("Hum"));
READINGS.print(RH_int);
READINGS.print(F("."));
READINGS.println(RH_dec);

//READINGS.print(F("Temperature :"));
READINGS.print(T_int);
READINGS.print(F("."));
READINGS.println(T_dec);

READINGS.println(F("mpu"));

//READINGS.print(F("roll :"));
READINGS.println(ypr[2] * 180/M_PI);
//READINGS.println(F("pitch"));
READINGS.println(ypr[1] * 180/M_PI);
//READINGS.print(F("yaw"));
READINGS.println(ypr[0] * 180/M_PI);

READINGS.println(F("bmb"));

//READINGS.print(F("Temperature :"));
READINGS.print(Temperature()/10.0);
//READINGS.print(F("pressure :")) ;
READINGS.println(Pressure()/ 100.0);

READINGS.println(F("uv"));
READINGS.println(uv.readUV());
READINGS.close();}
else {

Serial.println("erro");
}



}
  void SD_cardREADING(){      // reading data from sd card 
  
  Serial.print(" READING  MODE");
  READINGS = SD.open ("Data.txt");
  if(READINGS){
    Serial.print(F("Data.txt:"));
    while(READINGS.available())
    {
      Serial.write(READINGS.read());
      }
    
    READINGS.close() ;
    }
  
 else
 {
  Serial.print(F("there is no stored data :)")) ;
  }
  
  }
void setup (){

  Wire.begin ();
  Serial.begin(9600);
  
 


 
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        
        Wire.setClock(400000); 
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
   
    
    mpu.initialize();
  

    // verify connection
   


    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();

        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
     
        
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } 
   
  BMP_SETUP ();
  delay(20);
uv.begin(VEML6070_1_T);  // pass in the integration time constant
Serial.println("Exit setup");
}
void loop(){
DHT_LOOP ();


BMP_LOOP();
UV_LOOP();
mpu_ooh();

A : GPS_LOOP() ;
if (c== 0)
  {goto A ;
  
  }
  c=0 ;

SD_card(); 
SD_cardREADING();
  }  
