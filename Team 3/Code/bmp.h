#include <Wire.h>
#define ADDRESS_SENSOR 0x77
int16_t  ac1, ac2, ac3, b1, b2, mb, mc, md;
uint16_t ac4, ac5, ac6;
const uint8_t oss = 3;
const uint8_t osd = 26;
float T, P;
int32_t read_pressure()
{
  int32_t value;
  Wire.beginTransmission(ADDRESS_SENSOR);
  Wire.write(0xf4);
  Wire.write(0x34 + (oss << 6));
  Wire.endTransmission();
  delay(osd);
  Wire.beginTransmission(ADDRESS_SENSOR);
  Wire.write(0xf6);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS_SENSOR, 3);
  if (Wire.available() >= 3)
  {
    value = (((int32_t)Wire.read() << 16) | ((int32_t)Wire.read() << 8) | ((int32_t)Wire.read())) >> (8 - oss);
  }
  return value;
}

uint8_t read_1_byte(uint8_t code)
{
  uint8_t value;
  Wire.beginTransmission(ADDRESS_SENSOR);
  Wire.write(code);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS_SENSOR, 1);
  if (Wire.available() >= 1)
  {
    value = Wire.read();
  }
  return value;
}


uint16_t read_2_bytes(uint8_t code)
{
  uint16_t value;
  Wire.beginTransmission(ADDRESS_SENSOR);
  Wire.write(code);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS_SENSOR, 2);
  if (Wire.available() >= 2)
  {
    value = (Wire.read() << 8) | Wire.read();
  }
  return value;
}
float pressure(int32_t b5)
{
  int32_t x1, x2, x3, b3, b6, p, UP;
  uint32_t b4, b7;

  UP = read_pressure();

  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6 >> 12)) >> 11;
  x2 = ac2 * b6 >> 11;
  x3 = x1 + x2;
  b3 = (((ac1 * 4 + x3) << oss) + 2) >> 2;
  x1 = ac3 * b6 >> 13;
  x2 = (b1 * (b6 * b6 >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (ac4 * (uint32_t)(x3 + 32768)) >> 15;
  b7 = ((uint32_t)UP - b3) * (50000 >> oss);
  if (b7 < 0x80000000) {
    p = (b7 << 1) / b4;
  } else {
    p = (b7 / b4) << 1;
  }
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  return ((p + ((x1 + x2 + 3791) >> 4)) / 100.0f);
}
int32_t temperature()
{
  int32_t x1, x2, b5, UT;

  Wire.beginTransmission(ADDRESS_SENSOR);
  Wire.write(0xf4);
  Wire.write(0x2e);
  Wire.endTransmission();
  delay(5);

  UT = read_2_bytes(0xf6);

  // Calcule la vrai température
  x1 = (UT - (int32_t)ac6) * (int32_t)ac5 >> 15;
  x2 = ((int32_t)mc << 11) / (x1 + (int32_t)md);
  b5 = x1 + x2;
  T  = (b5 + 8) >> 4;
  T = T / 10.0;
  return b5;
}




class bmp
{
  public:
    void set_BMP()
    {
      Wire.begin();
      ac1 = read_2_bytes(0xAA);
      ac2 = read_2_bytes(0xAC);
      ac3 = read_2_bytes(0xAE);
      ac4 = read_2_bytes(0xB0);
      ac5 = read_2_bytes(0xB2);
      ac6 = read_2_bytes(0xB4);
      b1  = read_2_bytes(0xB6);
      b2  = read_2_bytes(0xB8);
      mb  = read_2_bytes(0xBA);
      mc  = read_2_bytes(0xBC);
      md  = read_2_bytes(0xBE);

    }
    float read_T()
    {
      int32_t b5;
      b5 = temperature();
      return (T);
    }
    float read_P()
    {
      int32_t b5;
      b5 = temperature();
      P = pressure(b5);
      return (P);
    }
    float read_A()
    {

      return ((pow((1014.109 / ( read_P())), (1 / 5.257)) - 1) * (read_T() + 273.15) / (0.0065));


    }
    void READ_BMP()
    {
      int32_t b5;
      b5 = temperature();
      P = pressure(b5);
      Serial.print("BMP,");
      Serial.print(T);
      Serial.print(",");
      Serial.print(P);
      Serial.print(",");
      Serial.print(((pow((1013.09 / ( P)), (1 / 5.257)) - 1) * (T + 273.15) / (0.0065)));
      Serial.println(",");

    }


};
