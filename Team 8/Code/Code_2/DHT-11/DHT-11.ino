#define pin 3
int expectPulse(bool lvl)
{
  int count=0;
   while(digitalRead(pin)==lvl) count++;
  return count;
  }
  void setup() {
 
  Serial.begin(9600); 
}

void loop () 
{
  unsigned char RH_int=0,RH_dec=0,T_int=0, T_dec=0, sum=0;
  unsigned char pulse [80];
  pinMode(pin,OUTPUT);
  digitalWrite(pin,HIGH);
  delay(1000);
  digitalWrite(pin,LOW);
  delay(18);
  digitalWrite(pin,HIGH);
  delayMicroseconds(40);
  pinMode(pin,INPUT);
  if (digitalRead(pin)==LOW)
  {
    while(digitalRead (pin) == LOW)
      {
        //delayMicroseconds(80);
      }
    while(digitalRead (pin) == HIGH)
    {
      //delayMicroseconds(80);
    }
  }
  else
  {
    Serial.println("NOT RESPONDING!!!");
    return;
  }
    for (int i=0; i<80;i++) 
    pulse[i]=expectPulse(i%2);
    for (int i=0; i<40; i++)
    {
      unsigned char lowCycle = pulse[i*2];
      unsigned char highCycle = pulse[(i*2)+1];

      if (i<8)
      {
        RH_int<<=1;
        if(highCycle>lowCycle)  RH_int|=1;
      }
        if (i<16)
        {
          RH_dec<<=1;
          if(highCycle>lowCycle)  RH_dec|=1;
        }
          if (i<24)
          {
            T_int<<=1;
            if(highCycle>lowCycle)  T_int|=1;
          }
            if (i<32)
            {
              T_dec<<=1;
              if(highCycle>lowCycle)  T_dec|=1;
            }
              if (i<40)
              {
                sum<<=1;
                if(highCycle>lowCycle)  sum|=1;
              }
     }
        if (sum!= RH_int+RH_dec+T_int+T_dec)
        {
          Serial.println("ERROR !!!");
        }
        else
        {
          Serial.println("DHT Readings");
          Serial.println("");
          Serial.print("Relative Humidity : ");  
          Serial.print(RH_int); Serial.print(".");Serial.print(RH_dec);Serial.println(" %");
          Serial.print("Temperature : ");  Serial.print(T_int);Serial.print(".");Serial.println(T_dec);
          Serial.println("");
          Serial.println("");
                  
        }
}
