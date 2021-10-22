class MPU_9250{
   private:
#define MPU9250_AD 0x68
#define SELF_AD 0x18
#define I2C_FREQ 400
#define FIFO_EN_AD 0x23
#define PWR_MGMT_1_AD 0x6B
#define ACCEL_XOUT_H_AD 0x3B
#define GYRO_XOUT_H_AD 0x43
#define EXT_SENS_DATA_00_AD 0x49
#define ACCEL_CONFIG_1_AD 0x1C
#define ACCEL_CONFIG_2_AD 0x1D
#define GYRO_CONFIG_AD 0x1B
#define CONFIG_AD 0x1A
#define I2C_MST_CTRL_AD 0x24
#define I2C_SLV0_ADDR_AD 0x25
#define I2C_SLV0_REG_AD 0x26
#define I2C_SLV0_CTRL_AD 0x27
#define INT_BYPASS_CONFIG_AD 0x37
#define USER_CTRL_AD 0x6A
#define ACCEL_SENS 8192.0f
#define GYRO_SENS 131.0f

//Magnetometer
#define MAG_AD 0xC
#define WIA_AD 0x00
#define INFO 0x01
#define STATUS_1_AD 0x02
#define HXL_AD 0x03    //X-axis measurement data lower 8bit
#define HXH_AD 0x04    //X-axis measurement data higher 8bit
#define HYL_AD 0x05    //Y-axis measurement data lower 8bit
#define HYH_AD 0x06    //Y-axis measurement data higher 8bit
#define HZL_AD 0x07    //Z-axis measurement data lower 8bit
#define HZH_AD 0x08    //Z-axis measurement data higher 8bit
#define STATUS_2_AD 0x09
#define CNTL1_AD 0x0A   //control 1
#define CNTL2_AD 0x0B   //control 2
#define ASTC_AD 0x0C    //Self-Test Control
#define TS1_AD 0x0D    //test 1
#define TS2_AD 0x0E   //test 2
#define I2CDIS_AD 0x0F    //I2C disable
#define ASAX_AD 0x10    //Magnetic sensor X-axis sensitivity adjustment value
#define ASAY_AD 0x11    //Magnetic sensor Y-axis sensitivity adjustment value
#define ASAZ_AD 0x12    //Magnetic sensor Z-axis sensitivity adjustment value
#define MAGNE_SENS 6.67f
#define SCALE 0.1499f  // 4912/32760 uT/tick
#define DATA_READY 0x01
#define MAGIC_OVERFLOW 0x8
#define USER_CTRL_AD 0x6A
#define INT_BYPASS_CONFIG_AD 0x37



  public:
  
  //Data Type for MPU9250 functions  
volatile float accelX,accelY,accelZ,gyroX,gyroY,gyroZ,magneX,magneY,magneZ,asax,asay,asaz;
bool updated = false;

//For saving in SD_card
File myFile;


void MPU9250_Setub()   
    {      
      using namespace TWI;


    I2CSetup(SELF_AD,I2C_FREQ);

    startTrans(MPU9250_AD);
    write(PWR_MGMT_1_AD);
    write(0x01,true); //0000 0001 in binary, set the clock reference to X axis gyroscope to get a better accuracy
    
    startTrans(MPU9250_AD);
    write(ACCEL_CONFIG_1_AD);
    write(0x08,true); //0000 1000 in binary. Set the accel scale to 4g
    
    startTrans(MPU9250_AD);
    write(GYRO_CONFIG_AD);
    write(0x08,true); //0000 1000 in binary. Set the gyro scale to 500 °/s and FCHOICE_B
    
    startTrans(MPU9250_AD);
    write(ACCEL_CONFIG_2_AD);
    write(0x05,true);  //0000 0101 in binary. Turn on the internal low-pass filter for accelerometer with 10.2Hz bandwidth
    
    startTrans(MPU9250_AD);
    write(CONFIG_AD);
    write(0x05,true);//0000 0101 in binary. Turn on the internal low-pass filter for gyroscope with 10Hz bandwidth
     // Actually we don't need this step cause the reset value of the register 106 is 0x00
    startTrans(MPU9250_AD);
    write(USER_CTRL_AD);
    write(0x00,true);
    
    startTrans(MPU9250_AD);
    write(INT_BYPASS_CONFIG_AD);
    write(0x02,true);   //0000 0010 in binary, turn on the bypass multiplexer

      // setup the Magnetometer to fuse ROM access mode to get the Sensitivity Adjustment values and 16-bit output
    startTrans(MAG_AD);
    write(CNTL1_AD);
    write(0x1F,true);  //0001 1111 in binary
    
    delay(100);  //wait for the mode changes

        //read the Sensitivit Adjustment values
    startTrans(MAG_AD);
    write(ASAX_AD);
    requestFrom(MAG_AD,3,true);
    asax = (readBuffer()-128)*0.5/128+1;
    asay = (readBuffer()-128)*0.5/128+1;
    asaz = (readBuffer()-128)*0.5/128+1;

      //reset the Magnetometer to power down mode
    startTrans(MAG_AD);
    write(CNTL1_AD);
    write(0x00,true);

    //wait for the mode changes
    delay(100);

        //set the Magnetometer to continuous mode 2（100Hz) and 16-bit output
    startTrans(MAG_AD);
    write(CNTL1_AD);
    write(0x16,true);   //0001 0110 in binary

    //wait for the mode changes
    delay(100);
    }


void MPU9250_Readings(){
using namespace TWI;


    startTrans(MPU9250_AD);
    write(GYRO_XOUT_H_AD);
    requestFrom(MPU9250_AD,6,true);
    gyroX = (readBuffer()<<8) | readBuffer();
    gyroY = (readBuffer()<<8) | readBuffer();
    gyroZ = (readBuffer()<<8) | readBuffer();

    
    //read the accelerate
    startTrans(MPU9250_AD);
    write(ACCEL_XOUT_H_AD);
    requestFrom(MPU9250_AD,6,true);
    accelX = (readBuffer()<<8) | readBuffer();
    accelY = (readBuffer()<<8) | readBuffer();
    accelZ = (readBuffer()<<8) | readBuffer();

       startTrans(MAG_AD);
    write(STATUS_1_AD);
    requestFrom(MAG_AD,1,true);  //pull the DRDY bit
    if((readBuffer() & DATA_READY) == DATA_READY){
        startTrans(MAG_AD);
        write(HXL_AD);
        requestFrom(MAG_AD,7,true);
        byte* buffer = getBuffer();
        if(!(buffer[6] & MAGIC_OVERFLOW)){     //check whether the magnetic sensor is overflown
          magneX = buffer[0] | (buffer[1]<<8);
          magneY = buffer[2] | (buffer[3]<<8);
          magneZ = buffer[4] | (buffer[5]<<8);
        }
    }

    Serial.print(F("MPU9250_G:"));
    Serial.print(gyroX/GYRO_SENS);
    Serial.print(F(","));
    Serial.print(gyroY/GYRO_SENS);
    Serial.print(F(","));
    Serial.print(gyroZ/GYRO_SENS);
    Serial.println(F(","));

    Serial.print(F("MPU9250_A:"));
    Serial.print(accelX/ACCEL_SENS);
    Serial.print(F(","));
    Serial.print(accelY/ACCEL_SENS);
    Serial.print(F(","));
    Serial.print(accelZ/ACCEL_SENS);
    Serial.println(F(","));

    Serial.print(F("MPU9250_M:"));
    Serial.print(magneX*asax*SCALE);
    Serial.print(F(","));
    Serial.print(magneY*asay*SCALE);
    Serial.print(F(","));
    Serial.print(magneZ*asaz*SCALE);
    Serial.print(F(","));

    //Calulating North angel for the compass 
    if ((magneY*asay*SCALE>=0)&&(magneX*asax*SCALE>=0)){
    Serial.print(360-atan2(abs(magneY*asay*SCALE), abs(magneX*asax*SCALE))*(180.0/ PI )-180) ; // arc tangent of y/x
    }

        if ((magneY*asay*SCALE>=0)&&(magneX*asax*SCALE<0)){
    Serial.print(360-180+atan2(abs(magneY*asay*SCALE), abs(magneX*asax*SCALE))*(180.0/ PI )-180) ; // arc tangent of y/x
    }
    
        if ((magneY*asay*SCALE<0)&&(magneX*asax*SCALE<0)){
    Serial.print(360-180-atan2(abs(magneY*asay*SCALE), abs(magneX*asax*SCALE))*(180.0/ PI )-180+360) ; // arc tangent of y/x
    }
    
        if ((magneY*asay*SCALE<0)&&(magneX*asax*SCALE>=0)){
    Serial.print(360-360+atan2(abs(magneY*asay*SCALE), abs(magneX*asax*SCALE))*(180.0/ PI )-180+360) ; // arc tangent of y/x
    }

    Serial.println(F(","));
    
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
    myFile.println (F("<<<<<<SD_Card_Data>>>>>>"));
    
    myFile.print(F("MPU9250:GYRO_X: "));
    myFile.print(gyroX/GYRO_SENS);
    myFile.print(F(" GYRO_Y: "));
    myFile.print(gyroY/GYRO_SENS);
    myFile.print(F(" GYRO_Z: "));
    myFile.print(gyroZ/GYRO_SENS);
    myFile.print(F(" ACCEL_X: "));
    myFile.print(accelX/ACCEL_SENS);
    myFile.print(F(" ACCEL_Y: "));
    myFile.print(accelY/ACCEL_SENS);
    myFile.print(F(" ACCEL_Z: "));
    myFile.print(accelZ/ACCEL_SENS);
    myFile.print(F(" MAGNE_X: "));
    myFile.print(magneX*asax*SCALE);
    myFile.print(F(" MAGNE_Y: "));
    myFile.print(magneY*asay*SCALE);
    myFile.print(F(" MAGNE_Z: "));
    myFile.print(magneZ*asaz*SCALE);
    myFile.print(F("North angel for the compass:"));
        if ((magneY*asay*SCALE>=0)&&(magneX*asax*SCALE>=0)){
    myFile.println(360-atan2(abs(magneY*asay*SCALE), abs(magneX*asax*SCALE))*(180.0/ PI )-180) ; // arc tangent of y/x
    }

        if ((magneY*asay*SCALE>=0)&&(magneX*asax*SCALE<0)){
    myFile.println(360-180+atan2(abs(magneY*asay*SCALE), abs(magneX*asax*SCALE))*(180.0/ PI )-180) ; // arc tangent of y/x
    }
    
        if ((magneY*asay*SCALE<0)&&(magneX*asax*SCALE<0)){
    myFile.println(360-180-atan2(abs(magneY*asay*SCALE), abs(magneX*asax*SCALE))*(180.0/ PI )-180+360) ; // arc tangent of y/x
    }
    
        if ((magneY*asay*SCALE<0)&&(magneX*asax*SCALE>=0)){
    myFile.println(360-360+atan2(abs(magneY*asay*SCALE), abs(magneX*asax*SCALE))*(180.0/ PI )-180+360) ; // arc tangent of y/x
    }
    
    // close the file:
    myFile.close();
    Serial.println(F("SD:1,"));
    
    }
    else {
    // if the file didn't open, print an error:
    Serial.println(F("SD:0,"));
    
    }
    }
}
  };
