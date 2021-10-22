class GPS{
private:

public:
//For saving in SD_card
File myFile;

//Data Type for GPS functions 
char sign [5];
char Raw_Data [54];
char check=0;

void GPS_Readings(){
  char min_Lat [9];
  float Decimal_Lat,Decimal_Lon;
  // String Decimal_lat,Decimal_lon;
  char min_Lon [9];
  A:
if (Serial.read()=='$'){
  Serial.readBytesUntil(',' ,sign,6);
 
  
  if ( sign [2] =='G' && sign[3] =='G'&& sign[4] =='A'){
    

     
     Serial.readBytesUntil('M' ,Raw_Data,53);



     check++;

}
}
if (check<2){goto A;}
check=0;
min_Lat [0]=Raw_Data[12];
min_Lat [1]=Raw_Data[13];
min_Lat [2]=Raw_Data[15];
min_Lat [3]=Raw_Data[16];
min_Lat [4]=Raw_Data[17];
min_Lat [5]=Raw_Data[18];
min_Lat [6]=Raw_Data[19];
min_Lat [7]='\0';


min_Lon [0]=Raw_Data[26];
min_Lon [1]=Raw_Data[27];
min_Lon [2]=Raw_Data[29];
min_Lon [3]=Raw_Data[30];
min_Lon [4]=Raw_Data[31];
min_Lon [5]=Raw_Data[32];
min_Lon [6]=Raw_Data[33];
min_Lon [7]=Raw_Data[34];
min_Lon [7]='\0';

Decimal_Lat=(String(min_Lat).toFloat())/60.0;
Decimal_Lon=(String(min_Lon).toFloat())/60.0;


dtostrf(Decimal_Lat, 4, 2, min_Lat);
dtostrf(Decimal_Lon, 4, 2, min_Lon);

Raw_Data[12]='.';
Raw_Data[13]=min_Lat[0];
Raw_Data[14]=min_Lat[1];
Raw_Data[15]=min_Lat[2];
Raw_Data[16]=min_Lat[3];
Raw_Data[17]=min_Lat[4];
Raw_Data[18]=min_Lat[6];
Raw_Data[19]=min_Lat[7];


Raw_Data[26]='.';
Raw_Data[27]=min_Lon[0];
Raw_Data[28]=min_Lon[1];
Raw_Data[29]=min_Lon[2];
Raw_Data[30]=min_Lon[3];
Raw_Data[31]=min_Lon[4];
Raw_Data[32]=min_Lon[6];
Raw_Data[33]=min_Lon[7];

Serial.print(F("GPS:$GPGGA,"));
Serial.print(String (Raw_Data));
Serial.println(F("M,"));


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
    myFile.print(F("GPS:$GPGGA"));
    myFile.println(String (Raw_Data));
    myFile.println(F("**************************"));  
    
       
        
          
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
