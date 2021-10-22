#include <Wire.h>

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

char oss = 0 ;
short AC_1, AC_2, AC_3, B_1, B_2, M_B, M_C, M_D ;
float Alt;
unsigned short AC_4, AC_5, AC_6 ;
long UT,UP, X1, X2, X3, T, B_3, B_5, B_6, P;
unsigned char UP_packet[3];
unsigned char UT_packet[2];
unsigned long B_4 , B_7 ;

short Read_data(unsigned char slave , unsigned char reg )
{
  Wire.beginTransmission(slave);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(slave , 2);

  while (Wire.available() < 2);
  return (short) Wire.read() << 8 | Wire.read()  ;
   
}




void setup() 
{
  Wire.begin();
  Serial.begin(9600);

  // Read the data coefficients
  
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
}


void loop() 
{
  // Read uncompensated Temperature

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
    // 0xF6 corresponds to 11110110
    // 0xF7 corresponds to 11110111

  UT_packet[0] = Wire.read();
    // 0xF6

  UT_packet[1] = Wire.read();
    // 0xF7

  UT = (long) ( UT_packet[0] * 256 + UT_packet[1] ) ;
  
  // Read uncompensated Pressure

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

  X1 = ((UT - (long) AC_6) *  (long) AC_5) / pow(2, 15);
  
  X2 =  M_C * pow(2, 11) / (X1 + M_D);
  
  B_5 = X1 + X2 ;
  
  T = (B_5 + 8) / pow(2, 4);
  


// Calculating Pressure
  
  B_6 = B_5 - 4000 ;
  
  X1 = (B_2 * ((B_6 * B_6) / pow(2, 12))) / pow(2, 11);
  
  X2 = ( AC_2 * B_6 / pow(2, 11) );
  
  X3 = X1 + X2;
  
  B_3 = ((((long) AC_1 * 4  + X3 ) << oss) + 2 ) / 4;
  
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

  Alt = 44330.0 * (1 - (pow( P/101325.0 , (1 / 5.255))));

  Serial.print ("Temperature : ");
  Serial.println ( T / 10.0 );
  
  Serial.print ("Pressure : ");
  Serial.println ( P );

  Serial.print ("Altitude : ");
  Serial.println (Alt);

}
