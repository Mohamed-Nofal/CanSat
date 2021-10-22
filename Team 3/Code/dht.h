#define sens 8
int pulseExp (bool value)
{

  int counter = 0;
  while (digitalRead(sens) == value) counter++;
  return counter;
}

class dht
{
  private:
    unsigned char RH_I = 0, RH_D = 0, T_I = 0, T_D = 0, SUM = 0;  unsigned char pulse [80];

  public :
    void set_dht()
    {
       pinMode (sens, OUTPUT);
      digitalWrite(sens, 1);
      delay(1000);
      digitalWrite (sens, 0);
      delay(18);
      digitalWrite (sens, 1);
      delayMicroseconds(40);
      pinMode (sens, INPUT);
      if (digitalRead(sens) == 0)
      {
        while (digitalRead(sens) == 0);
        while (digitalRead(sens) == 1);
      }
      else
      {
        Serial.println("ERROR");
        return;
      }
      for (int i = 0; i < 80; i++)
      {
        pulse[i] = pulseExp(i % 2);
      }
      for (int i = 0; i < 40; i++)
      {
        unsigned char low = pulse[i * 2];
        unsigned char high = pulse [(i * 2) + 1];
        if (i < 8)
        {
          RH_I <<= 1;
          if (high > low) RH_I |= 1;
        }
        if (i < 16)
        {
          RH_D <<= 1;
          if (high > low) RH_D |= 1;
        }
        if (i < 24)
        {
          T_I <<= 1;
          if (high > low) T_I |= 1;
        }
        if (i < 32)
        {
          T_D <<= 1;
          if (high > low) T_D |= 1;
        }
        if (i < 40)
        {
          SUM <<= 1;
          if (high > low) SUM |= 1;
        }

      }
     }
    float read_Humidity ()
    {
        set_dht();
      if (SUM != RH_I + RH_D + T_I + T_D)
      {
        Serial.println("ERORR");
      }
      else
      {

      
        return (float(RH_I+RH_D));

      }


    }

    
      float read_Temperature ()
      {
        set_dht();
         if (SUM != RH_I + RH_D + T_I + T_D)
      {
        Serial.println("ERORR");
      }
      else
      {

      
        return (float(T_I+T_D));

      }
      

}
     void READ_DHT()
     {
      set_dht();
       if (SUM != RH_I + RH_D + T_I + T_D)
      {
        Serial.println("ERORR");
      }
      else
      {

        Serial.print("DHT,");
        Serial.print(RH_I); Serial.print(".");

        Serial.print(RH_D); Serial.print("%");
        Serial.print(",");

        Serial.print(T_I); Serial.print(".");

        Serial.print(T_D); Serial.println(",");
     

      }
      
     }
};
