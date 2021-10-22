class BMP_280{
  private:
  #define BMP280_ADDRESS 0x76

  public:
//Data Type for BMP280 functions                            
unsigned long int temp_raw,pres_raw;
float Temp_act , Press_act;
signed long int t_fine;
uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;
uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;

//For saving in SD_card
File myFile;

  void BMP280_Setup(){
    
    uint8_t osrs_t = 1;             //Temperature oversampling x 1
    uint8_t osrs_p = 1;             //Pressure oversampling x 1
    uint8_t mode = 3;               //Normal mode
    uint8_t t_sb = 5;               //Tstandby 1000ms
    uint8_t filter = 0;             //Filter off
    uint8_t spi3w_en = 0;           //3-wire SPI Disable
   
    uint8_t ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
    uint8_t config_reg    = (t_sb << 5) | (filter << 2) | spi3w_en;

    
    writeReg(0xF4,ctrl_meas_reg);
    writeReg(0xF5,config_reg);
    readTrim();               
      
      }

void BMP280_Readings(){
double temp_act = 0.0, press_act = 0.0;
signed long int temp_cal;
unsigned long int press_cal;
    readData();
   
    temp_cal = calibration_T(temp_raw);
    press_cal = calibration_P(pres_raw);
    temp_act = (double)temp_cal / 100.0;
    press_act = (double)press_cal / 100.0;
    Temp_act=temp_act;
    Press_act=press_act*100.0;
    Serial.print("BMP280:");
    Serial.print(temp_act);
    Serial.print(",");
    Serial.print(press_act);
    Serial.println(","); 
SD_Writing_Data();

    
}
void SD_Writing_Data(){
    if (!SD.begin(4)) {
    Serial.println(F("SD:0,"));
    }
    else{ 
    myFile = SD.open("Data11.txt", FILE_WRITE);

  // if the file opened okay, write to it:
    if (myFile) {
    myFile.print("BMP280: TEMP : ");
    myFile.print(Temp_act);
    myFile.print(" DegC      PRESS : ");
    myFile.print(Press_act);
    myFile.println(" Pa "); 
   
    
       
        
          
    // close the file:
    myFile.close();
    Serial.println(F("SD:1,"));
    }
    else {
    // if the file didn't open, print an error:
    Serial.println(F("SD:0,"));
    }}
}

//function for BMP280 Readings
void readTrim()
{
    uint8_t data[32],i=0;                      // Fix 2014/04/06
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(0x88);
    Wire.endTransmission();
    Wire.requestFrom(BMP280_ADDRESS,24);       // Fix 2014/04/06
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
   
    Wire.beginTransmission(BMP280_ADDRESS);    // Add 2014/04/06
    Wire.write(0xA1);                          // Add 2014/04/06
    Wire.endTransmission();                    // Add 2014/04/06
    Wire.requestFrom(BMP280_ADDRESS,1);        // Add 2014/04/06
    data[i] = Wire.read();                     // Add 2014/04/06
    i++;                                       // Add 2014/04/06
   
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(0xE1);
    Wire.endTransmission();
    Wire.requestFrom(BMP280_ADDRESS,7);        // Fix 2014/04/06
    while(Wire.available()){
        data[i] = Wire.read();
        i++;   
    }
    dig_T1 = (data[1] << 8) | data[0];
    dig_T2 = (data[3] << 8) | data[2];
    dig_T3 = (data[5] << 8) | data[4];
    dig_P1 = (data[7] << 8) | data[6];
    dig_P2 = (data[9] << 8) | data[8];
    dig_P3 = (data[11]<< 8) | data[10];
    dig_P4 = (data[13]<< 8) | data[12];
    dig_P5 = (data[15]<< 8) | data[14];
    dig_P6 = (data[17]<< 8) | data[16];
    dig_P7 = (data[19]<< 8) | data[18];
    dig_P8 = (data[21]<< 8) | data[20];
    dig_P9 = (data[23]<< 8) | data[22];
}

//function for BMP280 Readings
void writeReg(uint8_t reg_address, uint8_t data)
{
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(reg_address);
    Wire.write(data);
    Wire.endTransmission();   
}


//function for BMP280 Readings
void readData()
{
    int i = 0;
    uint32_t data[8];
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(0xF7);
    Wire.endTransmission();
    Wire.requestFrom(BMP280_ADDRESS,8);
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
    pres_raw = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    temp_raw = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
}


//function for BMP280 Readings
signed long int calibration_T(signed long int adc_T)
{
   
    signed long int var1, var2, T;
    var1 = ((((adc_T >> 3) - ((signed long int)dig_T1<<1))) * ((signed long int)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((signed long int)dig_T1)) * ((adc_T>>4) - ((signed long int)dig_T1))) >> 12) * ((signed long int)dig_T3)) >> 14;
   
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

//function for BMP280 Readings
unsigned long int calibration_P(signed long int adc_P)
{
    signed long int var1, var2;
    unsigned long int P;
    var1 = (((signed long int)t_fine)>>1) - (signed long int)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11) * ((signed long int)dig_P6);
    var2 = var2 + ((var1*((signed long int)dig_P5))<<1);
    var2 = (var2>>2)+(((signed long int)dig_P4)<<16);
    var1 = (((dig_P3 * (((var1>>2)*(var1>>2)) >> 13)) >>3) + ((((signed long int)dig_P2) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((signed long int)dig_P1))>>15);
    if (var1 == 0)
    {
        return 0;
    }   
    P = (((unsigned long int)(((signed long int)1048576)-adc_P)-(var2>>12)))*3125;
    if(P<0x80000000)
    {
       P = (P << 1) / ((unsigned long int) var1);   
    }
    else
    {
        P = (P / (unsigned long int)var1) * 2;   
    }
    var1 = (((signed long int)dig_P9) * ((signed long int)(((P>>3) * (P>>3))>>13)))>>12;
    var2 = (((signed long int)(P>>2)) * ((signed long int)dig_P8))>>13;
    P = (unsigned long int)((signed long int)P + ((var1 + var2 + dig_P7) >> 4));
    return P;
}


};
