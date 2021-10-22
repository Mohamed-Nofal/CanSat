char mid[6];
char hour1[3];
char mun[3];
char sec[3];
char degl[3];
char degm[9];
char dir1[2];
char degl1[4];
char degm1[9]; 
char dir2[2]; char check[2]; char sat[3]; char alt[6]; double d, m, d1, m1; int c=1;float h,S,M,NUM;
class gps
{
  private :
  int k=0;
  public:
 
  void read_gps()
{
  do
 {
 
    
 c=1;
  if(Serial.read() == '$')
  {
 
    Serial.readBytesUntil(',', mid, 6);
    mid[5] = '\0';
    if (mid[2] == 'G' && mid[3] == 'G' && mid[4] == 'A')
    {
      c++;
      Serial.readBytesUntil(',', hour1, 2);
      Serial.readBytesUntil(',', mun, 2);
      Serial.readBytesUntil(',', sec, 3);
      Serial.readBytesUntil(',', degl, 4);
      Serial.readBytesUntil(',', degl, 2);
      Serial.readBytesUntil(',', degm, 9);
      Serial.readBytesUntil(',', dir1, 2);
      Serial.readBytesUntil(',', degl1, 3);
      Serial.readBytesUntil(',', degm1, 9);
      Serial.readBytesUntil(',', dir2, 2);
      Serial.readBytesUntil(',', check, 2);
      Serial.readBytesUntil(',', sat, 3);
      Serial.readBytesUntil(',', alt, 4);
      Serial.readBytesUntil(',', alt, 6);

      hour1[2] = '\0';    mun[2] = '\0';  sec[2] = '\0';  
      degl[2] = '\0';     degm[8] = '\0';
      dir1[1] = '\0';     degl1[3] = '\0';  degm1[8] = '\0';
      check[1] = '\0';    sat[2] = '\0';    dir2[1] = '\0';
      alt[5] = '\0';


      d = strtod (degl, 0 ); m = strtod (degm, 0 ); d1 = strtod (degl1, 0 ); m1 = strtod (degm1, 0 );
      h = strtod (hour1, 0 );   M = strtod (mun, 0 );   S = strtod (sec, 0 );  NUM = strtod (sat, 0 ); 
      switch(dir1[0])
     {
       case  'S':
       d= (-1*(d + (m / 60.000)));break;
       default :
       d = (d + (m / 60.000));break;
     }
       
       switch(dir2[0])
     {
       case 'W':
       d1=(-1*(d1 + (m1 / 60.000)));break;
       default :
       d1=(d1 + (m1 / 60.000));break;
     } 


  
    }
    
  
  }
   
}
  while(c==1);
 }
 
 void READ_GPS()
 {
         read_gps();
      Serial.print("GPS,");   Serial.print( d  ); Serial.print(",");  
      Serial.print( d1  );  Serial.print(",");  Serial.print(sat); Serial.print(",");
      Serial.print(h+2);Serial.print(":");Serial.print(mun);Serial.print(":");Serial.print(sec); Serial.println("/");
 }
 float get_long()
 {
  
   read_gps();
   
   return(d );
   
 }
 float get_lat()
 {
   read_gps();
   
   return(d1 );
   
 }
 float read_hours()
 {
   read_gps();
   return(h+2);
 }
 float read_min()
 {
   read_gps();
   return(M);
 }
 float read_sec()
 {
   read_gps();
   return(S);
 }
 float read_num()
 {
   read_gps();
   return(NUM);
 }
 

 
   
};
