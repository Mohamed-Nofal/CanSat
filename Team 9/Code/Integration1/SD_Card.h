class SD_Card{
  private:
  
  public:
//For reading from SD_card
File myFile;

void SD_Reading_Data(){
  Serial.print(F("Initializing SD card..."));
  myFile = SD.open("Data11.txt");
  if (myFile) {
  Serial.println(F("Data.txt:"));

  // read from the file until there's nothing else in it:
  while (myFile.available()) {
  Serial.write(myFile.read());
  }
  // close the file:
  myFile.close();
  } else {
  // if the file didn't open, print an error:
  Serial.println(F("Error Opening SD_Card Data"));
  }
}
};
