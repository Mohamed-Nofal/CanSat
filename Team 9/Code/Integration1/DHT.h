class DHT{
private:
#define dhtPin  6 

 public:
//For saving in SD_card
File myFile;
//Data Type for DHT functions 
unsigned char pulse [80];
unsigned char RH_int=0, RH_dec=0, T_int=0, T_dec=0, sum=0;

void DHT_Readings(){
  pinMode(dhtPin,OUTPUT);
  digitalWrite(dhtPin, HIGH); delay(250);
  digitalWrite (dhtPin,LOW); delay(18);
  digitalWrite(dhtPin, HIGH); delayMicroseconds(40);
  
// then flip the mode to INPUT and set the output conditions
  pinMode(dhtPin,INPUT);
  
  if(digitalRead(dhtPin) == LOW)
   {
   while(digitalRead(dhtPin) == LOW){}
    //delayMicroseconds(80);
  
   while (digitalRead(dhtPin) == HIGH){}
   //delayMicroseconds(80);
   }
  else
    {Serial.print(F("Not Responding...!"));
      return;}
  // here is all the magic 
  // first we do a for loop for all the 80 pulses and fill it into the array with its 0 or 1 value according to the % operator which if its odd number it will put [1]( as its true for the remaining on the dividing)and put [0] ( as its false there is no remaining )       
    for(int i = 0; i<80; i++)
      pulse[i]=expectPulse(i%2);
      
  // then for every cycle it consists of 2 digits [ byte ] (cycles) 0 and 1 ( low [ the even numbers ] and high [ the odd numbers ]) so the 80 pulses will come together into a 40 bite array    
    for(int i=0; i<40; i++)
     {unsigned char lowCyc = pulse[i*2];
      unsigned char highCyc = pulse[(i*2)+1];

  // for the first 8-bits (from b-0 to b-7) [ RH_int ]  shift the value of the reading by 1       
      if (i<8)
       {RH_int<<=1;
  //  here this is to check every bit if its has a value then set it by 1 if not leave it 0 as it is [[at declaring we set it RH_int = 0 [00000000]]   
        if (highCyc>lowCyc)RH_int|=1;}

      if (i<16)
       {RH_dec<<=1;
        if (highCyc>lowCyc)RH_dec|=1;}

      if (i<24)
       {T_int<<=1;
        if (highCyc>lowCyc)T_int|=1;}

      if (i<32)
       {T_dec<<=1;
        if (highCyc>lowCyc)T_dec|=1;}

      if (i<40)
       {sum<<=1;
        if (highCyc>lowCyc)sum|=1;}
     }   
         if(sum != (RH_int+RH_dec+T_int+T_dec))
         {Serial.println(F("ERROR..!"));}
     
        else 
      {Serial.print(F("DHT:"));
      Serial.print(RH_int);
      Serial.print(F("."));
      Serial.print(RH_dec);
      Serial.print (F(","));
      Serial.print(T_int);
      Serial.print(F("."));
      Serial.print(T_dec);
      Serial.println (F(","));
//SD_Writing_Data();


}
}

void SD_Writing_Data(){
    if (!SD.begin(4)) {
    Serial.println(F("SD:0,"));
    } 
    else{
    myFile = SD.open("Data11.txt", FILE_WRITE);

  // if the file opened okay, write to it:
    if (myFile) {
    myFile.print(F("DHT:R.H: "));
    myFile.print(RH_int);
    myFile.print(F("."));
    myFile.print(RH_dec);
    myFile.print (F(" %"));
    myFile.print(F("Temperature: "));
    myFile.print(T_int);
    myFile.print(F("."));
    myFile.print(T_dec);
    myFile.println (F(" C"));

   
    
       
        
          
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
// setting a counter to calculate the pulses for DHT functions
int expectPulse (bool lvl)  
{
   int count = 0;
   while (digitalRead(dhtPin) == lvl) count++;
   return count;
}   

};
