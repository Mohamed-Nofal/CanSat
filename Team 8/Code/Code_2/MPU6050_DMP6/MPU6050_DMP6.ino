#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


MPU6050 mpu;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64]; // FIFO storage buffer


Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
int16_t pqr[3];         // gyro x , gyro y , gyro z
int16_t temperature;           // temperature


void setup() 
{
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock.
  Serial.begin(9600);
    
  mpu.initialize();
    
  devStatus = mpu.dmpInitialize(); 

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) 
  {
    // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);
   
    dmpReady = true;
    
    packetSize = mpu.dmpGetFIFOPacketSize();
  }   
  
  else 
  {
   //Serial.println(""); 
  }
}

void loop() 
{
  if (!dmpReady) 
  {
    //Serial.println("");
  }
    // read a packet from FIFO
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) 
    {      
      mpu.dmpGetGyro(pqr , fifoBuffer);
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetAccel(&aa, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);       
      temperature = mpu.getTemperature();

      Serial.println("MPU Readings");
      Serial.println("");
            
      Serial.print("gyro x\t"); Serial.print("gyro y\t");  Serial.print("gyro z\t");  
      Serial.print("roll\t"); Serial.print("pitch\t"); Serial.print("yaw\t");
      Serial.print("accel_x\t"); Serial.print("accel_y\t"); Serial.print("accel_z\t");  Serial.println("Temperature");
            
      Serial.print(pqr[0]/16.4); Serial.print("\t");
      Serial.print(pqr[1]/16.4); Serial.print("\t");
      Serial.print(pqr[2]/16.4); Serial.print("\t");
            
      Serial.print(ypr[2] * 180/M_PI);  Serial.print("\t");
      Serial.print(ypr[1] * 180/M_PI);  Serial.print("\t");
      Serial.print(ypr[0] * 180/M_PI);  Serial.print("\t");   


      Serial.print(aa.x/8192.0);  Serial.print("\t"); 
      Serial.print(aa.y/8192.0);  Serial.print("\t"); 
      Serial.print(aa.z/8192.0);  Serial.print("\t"); 
      Serial.println(temperature/340.0 + 36.53);
            
      Serial.println("");
      Serial.println("");      
             
   }
}
