#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include<Wire.h>
#include<SPI.h> 
#include<Adafruit_Sensor.h>
#include<Adafruit_BMP280.h>
#include<EEPROM.h>

// GPS pins
const int RXPIN = 4, TXPIN = 3;

// EEPROM writing adress
long int addr = 0;

TinyGPSPlus gps;
SoftwareSerial ss(RXPIN, TXPIN);

//BMP280pins 
#define BMP_SCL 13
#define BMP_SDO 12 
#define BMP_SDA 11
#define BMP_CSB 10

Adafruit_BMP280 bmp(BMP_SCL,BMP_SDO,BMP_SDA,BMP_CSB);

typedef struct data
{
  long int timer;
 float lati;     //Read the latitude 
float longi;    //Read the longitude 
float alt;  //Read the altitude 
float qual;       //Quality of the measure
  //Time Variables
float hou;        //Read the hour of the clock 
float min;    //Read the minutes of the clock 
float sec;    //Read the seconds of the clock 

float course;   //Course of the spacecraft 
float speed;    //Speed course of the spacecraft in knots

float pres;    //Read the pressure in Pa 
float temp;

float Voltage;

float intensity;
}data;

typedef struct delta
{
  long int timer;
long int latitude;     //Read the latitude 
long int longitude;    //Read the longitude 
long int altitudeValue;  //Read the altitude 
long int quality;       //Quality of the measure
  //Time Variables
long int hou;        //Read the hour of the clock 
long int min;    //Read the minutes of the clock 
long int sec;    //Read the seconds of the clock 

long int course;   //Course of the spacecraft 
long int speed;    //Speed course of the spacecraft in knots

long int pressure;    //Read the pressure in Pa 
long int temperature;

long int voltage;

long int intensity;
}delta;

delta diff;

bool writeEnable = true;

long int timer, counter;


long int EEaddr = 0;
byte EECurrentByteData;

//********************************************SETUP************************************************************
void setup() {

Serial.begin(9600);
ss.begin(9600);
bmp.begin(9600);




counter = 0;
timer = gps.time.hour() * 10000 + gps.time.minute() * 100 + gps.time.second();
float latitude = gps.location.lat();
float longitude = gps.location.lng();
float altitudeValue =  gps.altitude.meters();
float quality = gps.hdop.hdop();
float temperature = bmp.readTemperature();
float pressure = bmp.readPressure();
float voltage = readVolt();
float intensity = readint();




//==============COUNTER===============
//writing the counter in bits 1 to 7
for (int i = 0; i < 7; i++){
  bitWrite(EECurrentByteData,i+1,bitRead((long int)counter,i));
}

//writing timer bit  in bits 8 to 24


//==============TIMER===============
  int a = 31;
  while(bitRead((long int)timer,a)==0 && a > 17){
    a--;
  }
  Serial.println(EECurrentByteData);
 bitWrite(EECurrentByteData,0,bitRead((long int)timer,a));
 EEPROM.write(EEaddr,EECurrentByteData);
 EEaddr++;

  //building a byte
  a--;
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)timer,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;
 
  a -= 8;

  //byte 2
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)timer,a-i));
  }
  //writing byte 2
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  //==============LATITUDE===============

  //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)(latitude*10000),a)==0 && a > 21){
    a--;
  }
  
  //building byte 1
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)(latitude*10000),a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;
 
  a -= 8;

  //byte 2
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)(latitude*10000),a-i));
  }
  //writing byte 2
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

    a -= 8;

  //byte 3
  for (int i = 0; i<5 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)(latitude*10000),a-i));
  }


  //===============LONGITUDE=====================
  //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)longitude,a)==0 && a > 22){
    a--;
  }
  
  //building byte 1
  for (int i = 0; i<3 ; i++){
    bitWrite(EECurrentByteData,2-i,bitRead((long int)longitude,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;
 
  a -= 3;

  //byte 2
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)longitude,a-i));
  }
  //writing byte 2
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

    a -= 8;

  //byte 3
  for (int i = 0; i<3 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)longitude,a-i));
  }

//=====================ALTITUDE=====================================

    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)altitudeValue,a)==0 && a > 11){
    a--;
  }
  
  //building byte 1
  for (int i = 0; i<5 ; i++){
    bitWrite(EECurrentByteData,4-i,bitRead((long int)altitudeValue,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;
 
  a -= 5;

  //byte 2
  for (int i = 0; i<6 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)altitudeValue,a-i));
  }
//byte is still incomplete

//==============================QUALITY=====================

    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)quality,a)==0 && a > 10){
    a--;
  }

    //building byte 1
  for (int i = 0; i<2 ; i++){
    bitWrite(EECurrentByteData,1-i,bitRead((long int)quality,a-i));
  }

  a -= 2;
    
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;


  //byte 2
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)quality,a-i));
  }
  //writing byte 2
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  //========================TEMPERATURE======================

    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)temperature,a)==0 && a > 10){
    a--;
  }

  //byte 1
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)temperature,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  a -= 8;
  
  //byte 2
  for (int i = 0; i<2 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)temperature,a-i));
  }
//incomplete byte

//========================PRESSURE======================

    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)pressure,a)==0 && a > 17){
    a--;
  }

     //byte 1
  for (int i = 0; i<6 ; i++){
    bitWrite(EECurrentByteData,5-i,bitRead((long int)pressure,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  a -= 6;

    //byte 2
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)pressure,a-i));
  }
  //writing byte 2
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  a -= 8;

    //byte 3
  for (int i = 0; i<3 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)pressure,a-i));
  }
//incomplete byte

//========================VOLTAGE======================


    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)voltage,a)==0 && a > 9){
    a--;
  }

     //byte 1
  for (int i = 0; i<5 ; i++){
    bitWrite(EECurrentByteData,4-i,bitRead((long int)voltage,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  a -= 5;

     //byte 2
  for (int i = 0; i<4 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)voltage,a-i));
  }
//incomplete byte

//=========================intENSITY=========================

    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)intensity,a)==0 && a > 7){
    a--;
  }

     //byte 1
  for (int i = 0; i<4 ; i++){
    bitWrite(EECurrentByteData,3-i,bitRead((long int)intensity,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  a -= 4;

     //byte 2
  for (int i = 0; i<3 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)intensity,a-i));
  }
for (int i = 0; i<5 ; i++){
    bitWrite(EECurrentByteData,4-i,0);//filling the last byte with zeros
  }
  
  //writing byte 2
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;



}//SETUP

//********************************************LOOP************************************************************
void loop() {
  
data dataSet,old;
  
//Variables are FLOATS

//first data gather is 17 Bytes
//all following data are 12 Bytes
//stored data are as follow:
//FIRST MEASUREMENT
//COUNTER |   TIMER | LATITUDE | LONGITUDE | ALTITUDE | QUALITY | TEMPERATURE | PRESSURE | VOLTAGE | intENSITY
//7 BITS  | 17 BITS | 21 BITS  | 22 BITS   | 11 BITS  | 10 BITS | 10 BITS     | 17 BITS  | 9 BITS  | 7 BITS
//OTHER MESUREMENTS
//COUNTER | TIMER  | LATITUDE | LONGITUDE | ALTITUDE | QUALITY | TEMPERATURE | PRESSURE | VOLTAGE | intENSITY
//7 BITS  | 3 BITS | 10 BITS  | 10 BITS   | 11 BITS  | 10 BITS | 8 BITS      | 16 BITS  | 9 BITS  | 7 BITS


  //========================GPS=================================================
  //Position Variables
  dataSet.lati=gps.location.lat();     //Read the latitude 
  dataSet.longi=gps.location.lng();    //Read the longitude 
  dataSet.alt=gps.altitude.meters();  //Read the altitude 
  dataSet.qual=gps.hdop.hdop();       //Quality of the measure
  //Time Variables
  dataSet.hou=gps.time.hour();        //Read the hour of the clock 
  dataSet.min = gps.time.minute();    //Read the minutes of the clock 
  dataSet.sec = gps.time.second();    //Read the seconds of the clock 

  dataSet.course = gps.course.deg();   //Course of the spacecraft 
  dataSet.speed = gps.speed.knots();    //Speed course of the spacecraft in knots
  //========================GPS=================================================

  dataSet.timer = dataSet.hou * 10000 + dataSet.min * 100 + dataSet.sec;

  //========================TEMPERATURE & PRESSURE==============================
  dataSet.pres=bmp.readPressure();    //Read the pressure in Pa 
  dataSet.temp=bmp.readTemperature(); //Read the temperature
  //========================TEMPERATURE & PRESSURE==============================



  //========================VOLTAGE & intENSITY READING==============================
  dataSet.Voltage = readVolt();

  dataSet.intensity = readint();
  //========================VOLTAGE & intENSITY READING==============================


  if (writeEnable == true){
      CalculateDeltas(dataSet,old);
      EEWrite(dataSet);
      if (EEaddr == 127){
        writeEnable = false;
      }
  }
  
  //saving data in memory for next cycle
  old = dataSet;
  old.timer = timer;

  //preparing for next data collection
  counter++;

  //=========================== WAIT TIME ======================================

  delay (3000); //wait for 3 seconds, mesures occur every 3 seconds approx.

  //=========================== WAIT TIME ======================================

}

float readVolt(){
  long int valueV= analogRead(A5);       // Read the value in the analog pin 
  float voldef= (valueV/ 1023) * 5; // Convert the value read in the  voltage we have measured 
  return voldef;                    // Return the voltage 
}

float readint() {
  long int valueI= analogRead(A4);         // Read the value in the analog pin 
  float intdef= (valueI/ 1023) * 5;    // Convert the value read in a voltage value 
  return intdef/10;                    // Return the correct intensitvalue (relation to measure: 0,1A/1V) MAX is 0.5A
  }

void CalculateDeltas(data dataSet,data old){
  diff.timer = dataSet.timer - old.timer;
  
  return ;
}

  void EEWrite(data dataSet){

  //==============COUNTER===============
//writing the counter in bits 1 to 7
for (int i = 0; i < 7; i++){
  bitWrite(EECurrentByteData,i+1,bitRead((long int)counter,i));
}

//writing timer bit  in bits 8 to 24


//==============TIMER===============
  long int a = 31;
  while(bitRead((long int)timer,a)==0 && a > 17){
    a--;
  }
  Serial.println(EECurrentByteData);
 bitWrite(EECurrentByteData,0,bitRead((long int)timer,a));
 EEPROM.write(EEaddr,EECurrentByteData);
 EEaddr++;

  //building a byte
  a--;
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)timer,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;
 
  a -= 8;

  //byte 2
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)timer,a-i));
  }
  //writing byte 2
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  //==============LATITUDE===============

  //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)(diff.latitude*10000),a)==0 && a > 21){
    a--;
  }
  
  //building byte 1
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)(diff.latitude*10000),a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;
 
  a -= 8;

  //byte 2
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)(diff.latitude*10000),a-i));
  }
  //writing byte 2
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

    a -= 8;

  //byte 3
  for (int i = 0; i<5 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)(diff.latitude*10000),a-i));
  }


  //===============LONGITUDE=====================
  //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)diff.longitude,a)==0 && a > 22){
    a--;
  }
  
  //building byte 1
  for (int i = 0; i<3 ; i++){
    bitWrite(EECurrentByteData,2-i,bitRead((long int)diff.longitude,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;
 
  a -= 3;

  //byte 2
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)diff.longitude,a-i));
  }
  //writing byte 2
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

    a -= 8;

  //byte 3
  for (int i = 0; i<3 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)diff.longitude,a-i));
  }

//=====================ALTITUDE=====================================

    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)diff.altitudeValue,a)==0 && a > 11){
    a--;
  }
  
  //building byte 1
  for (int i = 0; i<5 ; i++){
    bitWrite(EECurrentByteData,4-i,bitRead((long int)diff.altitudeValue,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;
 
  a -= 5;

  //byte 2
  for (int i = 0; i<6 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)diff.altitudeValue,a-i));
  }
//byte is still incomplete

//==============================QUALITY=====================

    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)diff.quality,a)==0 && a > 10){
    a--;
  }

    //building byte 1
  for (int i = 0; i<2 ; i++){
    bitWrite(EECurrentByteData,1-i,bitRead((long int)diff.quality,a-i));
  }

  a -= 2;
    
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;


  //byte 2
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)diff.quality,a-i));
  }
  //writing byte 2
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  //========================TEMPERATURE======================

    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)diff.temperature,a)==0 && a > 10){
    a--;
  }

  //byte 1
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)diff.temperature,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  a -= 8;
  
  //byte 2
  for (int i = 0; i<2 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)diff.temperature,a-i));
  }
//incomplete byte

//========================PRESSURE======================

    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)diff.pressure,a)==0 && a > 17){
    a--;
  }

     //byte 1
  for (int i = 0; i<6 ; i++){
    bitWrite(EECurrentByteData,5-i,bitRead((long int)diff.pressure,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  a -= 6;

    //byte 2
  for (int i = 0; i<8 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)diff.pressure,a-i));
  }
  //writing byte 2
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  a -= 8;

    //byte 3
  for (int i = 0; i<3 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)diff.pressure,a-i));
  }
//incomplete byte

//========================VOLTAGE======================


    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)diff.voltage,a)==0 && a > 9){
    a--;
  }

     //byte 1
  for (int i = 0; i<5 ; i++){
    bitWrite(EECurrentByteData,4-i,bitRead((long int)diff.voltage,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  a -= 5;

     //byte 2
  for (int i = 0; i<4 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)diff.voltage,a-i));
  }
//incomplete byte

//=========================INTENSITY=========================

    //this is to find the first significant bit
  a = 31;
  while(bitRead((long int)diff.intensity,a)==0 && a > 7){
    a--;
  }

     //byte 1
  for (int i = 0; i<4 ; i++){
    bitWrite(EECurrentByteData,3-i,bitRead((long int)diff.intensity,a-i));
  }
  //writing byte 1
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;

  a -= 4;

     //byte 2
  for (int i = 0; i<3 ; i++){
    bitWrite(EECurrentByteData,7-i,bitRead((long int)diff.intensity,a-i));
  }
for (int i = 0; i<5 ; i++){
    bitWrite(EECurrentByteData,4-i,0);//filling the last byte with zeros
  }
  
  //writing byte 2
  Serial.println(EECurrentByteData);
  EEPROM.write(EECurrentByteData,EEaddr);
  EEaddr++;
    
    return;
  }

