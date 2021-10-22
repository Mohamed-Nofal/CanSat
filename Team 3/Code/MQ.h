#define MQ2pin   A0
int sensorValue     ;
#define T 1
#define F 0
class mq5
{
   public:
   void READ_MQ()
 {
     int sensorValue;
     sensorValue = analogRead(MQ2pin);
     if(sensorValue > 100)
   {

      Serial.println("MQ,1.00,");
   }
     else
  {
      Serial.println("MQ,0.00,");

   }
  }
 float read_MQ()
{

  int sensorValue;
  sensorValue = analogRead(MQ2pin);


  if(sensorValue > 100)
 {

  return (T);
 }
  else
  {
  return (F);

  }
 } 
};

 
