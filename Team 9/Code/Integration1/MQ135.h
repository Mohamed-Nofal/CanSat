class mq_135
{

  
  private:
#define mqs A0


  public:
  float ppm;
  
//For saving in SD_card
File myFile;


void MQ135_Setup(){
pinMode(mqs,INPUT);
}

  
void MQ135_Readings(){
  ppm=analogRead(A0);
          Serial.print(F("MQ_135:"));
          Serial.print(ppm);
          Serial.println(F(",ppm"));
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
 
    if (ppm >=0 && ppm <350)
        {
          myFile.print(F("MQ_135: Air Quality = "));
          myFile.print(ppm);
          myFile.print(F(" PPM"));
          myFile.println(F("Air Quality is good for health"));
        }
        else if (ppm >=350 && ppm <600)
        {
          myFile.print(F("MQ_135:Air Quality = "));
          myFile.print(ppm);
          myFile.print(F(" PPM"));
          myFile.println(F("Air Quality not okay *Take Care* "));          
        }
        else if (ppm > 600)
        {
          myFile.print(F("MQ_135:Air Quality = "));
          myFile.print(ppm);
          myFile.print(F(" PPM"));
          myFile.println(F("Air Quality is *TOXIC* XO "));             
        }
        else
        {
          myFile.print(F("MQ_135:Air Quality = "));
          myFile.print(ppm);
          myFile.print(F(" PPM"));
          myFile.println(F("ERROR in Calibration"));   
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
