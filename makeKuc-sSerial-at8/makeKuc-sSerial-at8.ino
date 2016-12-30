/*
 SoftwareSerial
 ATmega328: Moisture Sensor, 
 OLED display Driver
*/
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

SoftwareSerial mySerial(5, 6); /* RX:D5, TX:D6 */
//OLED
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
  
//define
const int mVoutPin = 0;
uint32_t mTimerTmp;

//
long convert_Map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//
// reading Moisture
int getMoisture(){
  int iRet=0;
  float fSen  = 0;
  unsigned long reading  = 0;   
  for (int i=0; i<10; i++) {
    int  iTmp = analogRead(mVoutPin);
//Serial.print("get.iTmp=");
//Serial.println(iTmp);
    reading  += iTmp; 
    delay(100);
  }
  int SValue= reading / 10;
  int voltage=convert_Map(SValue, 0, 1000, 0,3300);  // V
Serial.print("SValue=");
Serial.print(SValue);
Serial.print(" , voltage=");
Serial.println(voltage);
  int iMaxVoltage= 2048;
  //float fMoi =(voltage * 100) / iMaxVoltage;
  float fMoi = (float)voltage / (float)iMaxVoltage;  
Serial.print( "f.fMoi= " );
Serial.println( fMoi );   
  fMoi= fMoi * 100 ;
  int iMoi= (int)fMoi;  
Serial.print( "Moisture Value= " );
Serial.println( iMoi ); 
  iRet= iMoi;
  return iRet;  
}
//
//void display_OLED(String sTmp, String sHum ){
void display_OLED(String sTmp ){
  //sTmp="Temp:"+ sTmp+ " %";
  sTmp=sTmp+ " %";
  
  display.setTextSize(2);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.println( "Moisture" );
  display.println( sTmp );
  display.display();
  //delay(2000);
  delay(500);  
  display.clearDisplay();
}

//
void setup() {
  Serial.begin(9600);
  mySerial.begin( 9600 );
  Serial.println("# Start-setup");
    //Sensor
  pinMode(mVoutPin, INPUT);
  //
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display();
  delay(2000);
  display.clearDisplay();  
}

//
void loop() {
  delay( 100 );
  int iNum=0;
  if (millis() > mTimerTmp) {
      mTimerTmp = millis()+ 500;
      int itemp= getMoisture();
      //OLED
      String sTemp=String(itemp);
      display_OLED(sTemp );
      //sSerial
     int iD1=int(itemp );
     char cD1[8+1];
     sprintf(cD1 , "d1%06d", iD1);       
     mySerial.print( cD1 );
Serial.print("cD1=");
Serial.println(cD1  );      
  }

}

