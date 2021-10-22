



#include "library.h"
#include <library_dht.h>
#include <my_library_uuuuu.h>



#include <SPI.h>
#include <SD.h>



File myFile;

void setup() {
  

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  } 
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("DATA.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
   
    
   Serial.print("FILE OPENED");
    myFile.print (" bmp ");
      myFile.print ("Temperature : ");
   myFile.println ( T / 10.0 );
  myFile.print ("Pressure : ");
  //Serial.println ( P /100.0 *0.18376 );
   myFile.println ( P );
 myFile.print ("Altitude : ");
   myFile.println (Alt);

   myFile.print("dht");
   


   
    // close the file:
    








    
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  void loop)
  {}
