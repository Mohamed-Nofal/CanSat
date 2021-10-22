//Protocol example message
//$GPGGA,082206.00,3001.47897,N,03112.68931,E,1,08,1.29,65.8,M,15.3,M,,*6F

char MID[6];
char UTC[10];
char LAT[11];
char NSInd[2];
char Long[12]; 
char EWInd[2];
char PosFix[2];
char Sat[3];
char Altitude[5];

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() 
{
  if (Serial.read()== '$')
  {
    Serial.readBytesUntil(',',MID, 6);
    MID[5] = '\0';
    if (MID[2]=='G' && MID[3]== 'G' && MID[4]== 'A')
    {
      Serial.readBytesUntil(',' , UTC , 10);
      Serial.readBytesUntil(',' , LAT , 11);
      Serial.readBytesUntil(',',NSInd, 2);
      Serial.readBytesUntil(',' , Long , 12);  
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

             
      Serial.print("ID : ");
      Serial.println(MID);
              
      Serial.print("UTC : ");
      Serial.println(UTC);
              
      Serial.print("Latitude : ");
      Serial.println(int(atof(LAT)/100)+(atof(LAT)-int(atof(LAT)/100)*100)/60.0 , 6);
              
      Serial.print("North/South : ");
      Serial.println(NSInd);
              
      Serial.print("Longitude : ");
      Serial.println(int(atof(Long)/100)+(atof(Long)-int(atof(Long)/100)*100)/60.0 , 6 );
              
      Serial.print("East/West : ");
      Serial.println(EWInd);
              
      Serial.print("Position Fix : ");
      Serial.println(PosFix);
              
      Serial.print("Number of Satellites : ");
      Serial.println(Sat);
              
      Serial.print("Altitude : ");
      Serial.println(Altitude);
              
      Serial.println("");
      Serial.println("");
    }  
   
   } 

}
