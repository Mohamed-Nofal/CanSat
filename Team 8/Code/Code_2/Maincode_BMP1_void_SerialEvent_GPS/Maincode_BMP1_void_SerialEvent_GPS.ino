// Pre-setup of DHT sensor

  #define DHT_pin 3

  int expectPulse(bool lvl)
  {
    int count=0;
    while(digitalRead(DHT_pin)==lvl) count++;
    return count;
  }

  unsigned char RH_int=0,RH_dec=0,T_int=0, T_dec=0, sum=0; 

// Pre-setup of BMP sensor 

  #include <Wire.h>
  #define oss 0
  #define referencing_altitude -5.58 
  #define BMP 0x77
  #define AC1 0xAA
  #define AC2 0xAC
  #define AC3 0xAE
  #define AC4 0xB0
  #define AC5 0xB2
  #define AC6 0xB4
  #define B1 0xB6
  #define B2 0xB8
  #define MB 0xBA
  #define MC 0xBC
  #define MD 0xBE

  short AC_1, AC_2, AC_3, B_1, B_2, M_B, M_C, M_D ;
  float Alt;
  unsigned short AC_4, AC_5, AC_6 ;
  long UT, UP, X1, X2, X3, T, B_3, B_5, B_6, P;
  unsigned char UP_packet[3];
  unsigned char UT_packet[2];
  unsigned long B_4 , B_7 ;

  short Read_data(unsigned char slave , unsigned char reg)
  {
    Wire.beginTransmission(slave);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(slave , 2);
    while (Wire.available() < 2);
    return (short) (Wire.read() << 8 | Wire.read());
  }


//Pre-Setup of GPS module

  char MID[6];
  char UTC[10];
  char LAT[11];
  char NSInd[2];
  char Long[12];
  char EWInd[2];
  char PosFix[2];
  char Sat[3];
  char Altitude[5];



// Pre-setup of MPU sensor

  #include "I2Cdev.h"
  #include "MPU6050_6Axis_MotionApps20.h"
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
  #endif

  MPU6050 mpu;

  bool dmpReady = false;  // true if DMP initialization was successful
  uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
  uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
  uint8_t fifoBuffer[64]; // FIFO storage buffer

  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorInt16 aa;         // [x, y, z]            accel sensor measurements
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector respectively
  int16_t pqr[3];         // gyro x , gyro y , gyro z respectively
  int16_t temperature;    // temperature

// Pre-setup of SD 
  #include <SPI.h>
  #include <SD.h> 
  File myFile;
 

void setup() 
{  
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);       
  
  // Setup for BMP
  
  AC_1 = Read_data(BMP, AC1);
  AC_2 = Read_data(BMP, AC2);
  AC_3 = Read_data(BMP, AC3);
  AC_4 = Read_data(BMP, AC4);
  AC_5 = Read_data(BMP, AC5);
  AC_6 = Read_data(BMP, AC6);
  B_1 = Read_data(BMP, B1);
  B_2 = Read_data(BMP, B2);
  M_B = Read_data(BMP, MB);
  M_C = Read_data(BMP, MC);
  M_D = Read_data(BMP, MD);

  
  // MPU setup
  
  mpu.initialize();  
  devStatus = mpu.dmpInitialize();
  
  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
  
  // check message whether it is working or not
  if (devStatus == 0) 
  { 
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    // turn on the DMP, now that it's ready
    // Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  } 
  else 
  {
    //Serial.println("MPU is not connected");   // When MPU is not working 
  }
  
  
  // SD initialization
  Serial.print("Initializing SD card...");
  
  if (!SD.begin(4)) 
  {
    Serial.println("initialization failed!");
    // while (1);
  }
  
  Serial.println("initialization done.");
  
}

void loop() 
{ 
  DHT();
  BMP1();
  MPU();

  // SD printing orders
 
  myFile = SD.open("CTP.csv", FILE_WRITE);
  if (myFile) 
  { 
    myFile.print(MID);    myFile.print (" , ");    myFile.print(UTC);  myFile.print (" , ");          myFile.print(int(atof(LAT)/100)+(atof(LAT)-int(atof(LAT)/100)*100)/60.0 , 6);   myFile.print (" , ");
    myFile.print(NSInd);  myFile.print (" , ");    myFile.print(int(atof(Long)/100)+(atof(Long)-int(atof(Long)/100)*100)/60.0 , 6 );  myFile.print (" , ");
    myFile.print(EWInd);  myFile.print (" , ");    myFile.print(Sat);  myFile.print (" , ");    myFile.print(Altitude);  myFile.print (" , ");

    myFile.print(RH_int); myFile.print("."); myFile.print(RH_dec); myFile.print(" %");   myFile.print (" , ");
    myFile.print(T_int); myFile.print("."); myFile.print(T_dec);     myFile.print (" , ");
  
    myFile.print (T / 10.0);  myFile.print (" , ");     myFile.print (P);  myFile.print (" , ");    myFile.print (Alt);  myFile.print (" , ");

    myFile.print(pqr[0]/16.4);  myFile.print(" , ");  
    myFile.print(pqr[1]/16.4);  myFile.print(" , ");
    myFile.print(pqr[2]/16.4);  myFile.print(" , ");
    
    myFile.print(ypr[2] * 180/M_PI);  myFile.print(" , ");
    myFile.print(ypr[1] * 180/M_PI);  myFile.print(" , ");
    myFile.print(ypr[0] * 180/M_PI);  myFile.print(" , ");

    myFile.print(aa.x/8192.0);        myFile.print(" , "); 
    myFile.print(aa.y/8192.0);        myFile.print(" , "); 
    myFile.print(aa.z/8192.0);        myFile.print(" , "); 
    myFile.println(temperature/340.0 + 36.53);

   // close the file:
    myFile.close();
  }
  else 
  {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

} 


void DHT()
{
  unsigned char pulse [80];
  
  pinMode (DHT_pin , OUTPUT);
  digitalWrite(DHT_pin , HIGH);
  delay(1000);
  digitalWrite (DHT_pin , LOW);
  delay(18);
  digitalWrite (DHT_pin , HIGH);
  delayMicroseconds(40);
  pinMode (DHT_pin , INPUT);
  if (digitalRead(DHT_pin) == LOW)
  {
    while(digitalRead(DHT_pin) == LOW)
    {
      //delayMicroseconds(80);
    }
    while(digitalRead(DHT_pin) == HIGH)
    {
      //delayMicroseconds(80);
    }
  }
  else
  {
    Serial.println("DHT,0,");  // wwhen DHT is not working
    return;
  }
    
  for (int i=0;i<80;i++)
  {
    pulse[i]=expectPulse(i%2);
  }
    
  for (int i=0;i<40;i++)
  {
    unsigned char lowCycle = pulse[i*2];
    unsigned char highCycle = pulse [(i*2)+1];
    if (i<8)
    {
      RH_int<<=1;
      if(highCycle>lowCycle)    RH_int|=1;
    }
        
    if (i<16)
    {
      RH_dec<<=1;
      if(highCycle>lowCycle)RH_dec|=1;
    }
           
    if (i<24)
    {
      T_int<<=1;
      if(highCycle>lowCycle)    T_int|=1;
    }
              
    if (i<32)
    {
      T_dec<<=1;
      if(highCycle>lowCycle)    T_dec|=1;
    }
                
    if(i<40)
    {
      sum<<=1;
      if (highCycle>lowCycle)   sum|=1;
    }
         
  } 
    
  if (sum!= RH_int+RH_dec+T_int+T_dec)
  {
    Serial.println("ERROR !!!");
  }
  else
  {   
    Serial.print("DHT,");
    
    //Serial.print("Relative Humidity : ");  
    Serial.print(RH_int);  Serial.print(".");  Serial.print(RH_dec);   Serial.print(",");
    
    //Serial.print("Temperature : ");  
    Serial.print(T_int);   Serial.print(".");   Serial.print(T_dec);     Serial.println(",");
    // Temperature is in degree celesius
       
  }   
         
}


// $GPGGA,082206.00,3001.47897,N,03112.68931,E,1,08,1.29,65.8,M,15.3,M,,*6F
 
void serialEvent()
{
  while(true)
  {  
    
    if (Serial.read()== '$')
    {
      Serial.readBytesUntil(',',MID, 6);
      MID[5] = '\0';
      if (MID[2]=='G' && MID[3]== 'G' && MID[4]== 'A')
      {
        Serial.readBytesUntil(',',UTC, 10);
        Serial.readBytesUntil(',',LAT, 11);
        Serial.readBytesUntil(',',NSInd, 2);
        Serial.readBytesUntil(',',Long, 12);
        Serial.readBytesUntil(',',EWInd, 2);
        Serial.readBytesUntil(',',PosFix, 2);
        Serial.readBytesUntil(',',Sat, 3);
        Serial.readBytesUntil(',',Altitude, 5);
        Serial.readBytesUntil(',',Altitude, 5);

 
        UTC[9] = '\0';
        LAT[10] = '\0';
        NSInd[1] = '\0';
        Long[11] = '\0';
        EWInd[1] = '\0';
        PosFix[1] = '\0';
        Sat[2] = '\0';
        Altitude[4] = '\0';

        if(PosFix[0] == '1') 
        {
          Serial.print("GPS,");
          
          //Serial.print("ID : ");
          Serial.print(MID);
          Serial.print(",");
              
          //Serial.print("UTC : ");
          Serial.print(UTC);
          Serial.print(",");
              
          //Serial.print("Latitude : ");
          Serial.print(int(atof(LAT)/100)+(atof(LAT)-int(atof(LAT)/100)*100)/60.0 , 6);   // degree + minutes / 60.0 (ddmmssssssss)
          Serial.print(",");
              
          //Serial.print("North/South : ");
          Serial.print(NSInd);
          Serial.print(",");
              
          //Serial.print("Longitude : ");
          Serial.print(int(atof(Long)/100)+(atof(Long)-int(atof(Long)/100)*100)/60.0 , 6 );   // degree + minutes / 60.0 (dddmmssssss)
          Serial.print(",");
              
          //Serial.print("East/West : ");
          Serial.print(EWInd);
          Serial.print(",");
              
          //Serial.print("Position Fix : ");
          Serial.print(PosFix);           // if it equals 1, it is valid 
          Serial.print(",");
              
          //Serial.print("Number of Satellites : ");
          Serial.print(Sat);
          Serial.print(",");
              
          //Serial.print(" Altitude : ");
          Serial.print(Altitude);     // altitude is in meters
          Serial.println(",");    
        }
        else
        {
          Serial.println("GPS,x,");        //when gps is connected but the readings are not accurate    
        }
         
    }
  }

 } 

}

 
void BMP1 ()
{

  char BMP_address;

  Wire.beginTransmission(0x77);
  Wire.write(0xD0);
  Wire.endTransmission(false);
  Wire.requestFrom(0x77, 1);
  if (Wire.available() > 0)
  {
    BMP_address = Wire.read();
  }


  if(BMP_address == 0x55)
  {
    /// Read uncompensated Temperature

    Wire.beginTransmission(BMP);
    Wire.write(0xF4);
    Wire.write(0x2E);
    Wire.endTransmission(BMP);
  
    delay(4.5);  // Can be 5 ms
    
    // 0xF6 corresponds to  11110110
    // 0xF7 corresponds to  11110111
  
    Wire.beginTransmission(BMP);
    Wire.write(0xF6);
    Wire.endTransmission(BMP);
    
    Wire.requestFrom(BMP , 2);
  
    while (Wire.available() < 2);
    
    UT_packet[0] = Wire.read();   // 0xF6
    UT_packet[1] = Wire.read();   // 0xF7
      
    UT = (long) ( UT_packet[0] * 256 + UT_packet[1] ) ;
  
    
    /// Read uncompensated Pressure
  
    Wire.beginTransmission(BMP);
    Wire.write(0xF4);
    Wire.write( (0x34) | (oss << 6) );
    Wire.endTransmission(BMP);
  
    delay(4.5);  // Can be 5 ms
  
  
  
    Wire.beginTransmission(BMP);
    Wire.write(0xF6);
    Wire.endTransmission(BMP);
    Wire.requestFrom(BMP , 3);
  
    while (Wire.available() < 3);
    
    // 0xF6 corresponds to 11110110
    // 0xF7 corresponds to 11110111
    // 0xF8 corresponds to 11111000
      
    UP_packet[0] = Wire.read();   // UP_packet[0] = 0xF6 
    UP_packet[1] = Wire.read();   // UP_packet[1] = 0xF7  
    UP_packet[2] = Wire.read();   // UP_packet[2] = 0xF8
         
    UP =  (long) ( ( (unsigned long) UP_packet[0] << 16 ) | ( (unsigned long) UP_packet[1] << 8 ) |  ( (unsigned long) UP_packet[2] )) >> (8 - oss) ;
  
  
    // Calculating Temperature
  
    X1 =   ( (UT - (long) AC_6) *  (long) AC_5  ) / pow(2, 15);
    X2 = ( M_C * pow(2, 11) ) / (X1 + M_D);
    B_5 = X1 + X2 ;
    T = ( B_5 + 8 ) / pow(2, 4);
  
    // Calculating Pressure
    
    B_6 = B_5 - 4000 ;
    X1 = ( B_2 * ( (B_6 * B_6) / pow(2, 12) ) ) / pow(2, 11);
    X2 = ( AC_2 * B_6 / pow(2, 11) );
    X3 = X1 + X2;
    B_3 = ((( (long) AC_1 * 4  + X3 ) << oss) + 2 ) / 4;
    X1 = (AC_3 * B_6) / pow(2, 13);
    X2 = ( B_1 * ( B_6 * B_6 / pow(2, 12) ) ) / pow(2, 16);
    X3 = ( (X1 + X2) + 2 ) / 4;
    B_4 = ( AC_4 * (unsigned long)(X3 + 32768) ) / pow(2, 15);
    B_7 = ( (unsigned long) UP - B_3 ) * (50000 >> oss);
  
    if (B_7 < 0x80000000)
    {
      P = (B_7 * 2 ) / B_4;
    }
    else
    {
      P = (B_7 / B_4) * 2;
    }
  
    X1 = ( P / pow(2, 8) ) * ( P / pow(2, 8) );  
    X1 = (X1 * 3038) / pow(2, 16);
    X2 = (-7357 * P ) / pow(2, 16);
    P = P + ( (X1 + X2 + 3791 ) / pow(2, 4) );    
  
    Alt = 44330.0 * (1 - (pow(P / 101325.0 , (1 / 5.255))));
  
    Serial.print ("BMP,");
    
    //Serial.print ("Temperature : ");
    Serial.print ( T / 10.0 );    Serial.print (",");
    
    //Serial.print ("Pressure : ");
    Serial.print ( P );         Serial.print (",");
  
    //Serial.print ("Altitude : ");
    Serial.print (Alt - referencing_altitude);         Serial.println (",");
  }
  else
  {
    Serial.println("BMP,0,");    // When BMP is not connected
  }

}

  
void MPU() 
{
    
    
  if (!dmpReady)    // check message to see if it is working or not.
  {
    Serial.println("MPU,-1,");   // when it is not working.
  }
      
    
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) // Get the Latest packet 
  {         
    mpu.dmpGetGyro(pqr , fifoBuffer);
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetAccel(&aa, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    temperature = mpu.getTemperature();
           
            
    Serial.print("MPU,");
            
            
    /*Serial.print("gyro x\t");   Serial.print("gyro y\t");   Serial.print("gyro z\t");  
    Serial.print("roll\t");     Serial.print("pitch\t");    Serial.print("yaw\t");
    Serial.print("accel_x\t");  Serial.print("accel_y\t");  Serial.print("accel_z\t");  Serial.println("Temperature"); */
            
    // angular velocities are in deg/sec
                   
    Serial.print(pqr[0]/16.4);  Serial.print(",");  
    Serial.print(pqr[1]/16.4);  Serial.print(",");
    Serial.print(pqr[2]/16.4);  Serial.print(",");

    // Yaw, pitch, roll are in degrees
            
    Serial.print(ypr[2] * 180/M_PI);    Serial.print(",");
    Serial.print(ypr[1] * 180/M_PI);    Serial.print(",");
    Serial.print(ypr[0] * 180/M_PI);    Serial.print(",");

    // acceleration is in g (m/sec^2/9.81)
            
    Serial.print(aa.x/8192.0);        Serial.print(","); 
    Serial.print(aa.y/8192.0);        Serial.print(","); 
    Serial.print(aa.z/8192.0);        Serial.print(","); 
    Serial.print(temperature/340.0 + 36.53);        //Temperature is in degree celesius
    Serial.println(",");
  }
    
}
