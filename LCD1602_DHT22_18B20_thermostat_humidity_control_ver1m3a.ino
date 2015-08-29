/*
THERMOSTAR with HUMIDITY CONTROL
ver.1m0 - 28.08.2015, Craiova - Romania, Europe, Earth
ver.1m3a - 29.08.2015

niq_ro write this sketch using parts of other sketches :)
http://www.tehnic.go.ro
http://nicuflorica.blogspot.ro/
http://arduinotehniq.blogspot.com/
*/

#include <LiquidCrystal.h>  //this library is included in the Arduino IDE
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/*                                    -------------------
                                      |  LCD  | Arduino |
                                      -------------------
 LCD RS pin to digital pin 12         |  RS   |   D12   |
 LCD Enable pin to digital pin 11     |  E    |   D11   |
 LCD D4 pin to digital pin 5          |  D4   |   D6    |
 LCD D5 pin to digital pin 4          |  D5   |   D4    |
 LCD D6 pin to digital pin 3          |  D6   |   D3    |
 LCD D7 pin to digital pin 2          |  D7   |   D2    |
 LCD R/W pin to ground                |  R/W  |   GND   |
                                      -------------------
*/



// http://arduino.cc/en/Reference/LiquidCrystalCreateChar
byte grad[8] = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
};


// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include "DHT.h"
#define DHTPIN A0     // what pin we're connected to A0
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// if is just sensor:
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);


// for DS18B20 - http://arduinoprojects.ru/2014/08/%D0%BF%D1%80%D0%BE%D1%81%D1%82%D0%BE%D0%B9-%D1%82%D0%B5%D1%80%D0%BC%D0%BE%D1%81%D1%82%D0%B0%D1%82-%D0%BD%D0%B0-arduino-%D0%B8-%D1%86%D0%B8%D1%84%D1%80%D0%BE%D0%B2%D0%BE%D0%BC-%D1%82%D0%B5%D1%80%D0%BC/
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 10
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);



// define pins for relay or leds
#define temppeste A1
#define umidpeste A2

// define variable

float te, t1, tes, dete;   // variable for temperature
int has, hass, dehas;        // variable for humiditiy

byte heat = 1;  // is 1 for heater and 0 for cooler
byte dry = 1;   // is 1 for dryer and 0 for wetter 

// part for menu: http://nicuflorica.blogspot.ro/2015/05/termostat-cu-afisaj-led.html
#define BUT1 6    // - switch
#define BUT2 7    // + switch
#define BUT3 8    // MENU switch

byte meniu = 0; // if MENIU = 0 is clasical 
                // if MENIU = 1 is for temperature set (tes)
                // if MENIU = 2 is for dt temperature (dete)
                // if MENIU = 3 is for humidity set (hass)
                // if MENIU = 4 is for dh humidity (dehas)
                // if MENIU = 5 is for HEAT/COOL
                // if MENIU = 6 id for DRY/WET

void setup()
{

// Initialize LCD display  
 lcd.begin(16, 2);
// create custom symbol
lcd.createChar(0, grad);

// Initialize DHT sensor
  dht.begin();

// for DS18B20 sensor
sensors.begin(); 

// define outputs
 pinMode(temppeste, OUTPUT);  
 pinMode(umidpeste, OUTPUT);

// set the default state for outputs
  digitalWrite(temppeste, LOW);
  digitalWrite(umidpeste, LOW);

// set push buttons for menu
  pinMode(BUT1, INPUT);
  pinMode(BUT2, INPUT);
  pinMode(BUT3, INPUT);
  
  digitalWrite(BUT1, HIGH); // pull-ups on
  digitalWrite(BUT2, HIGH);
  digitalWrite(BUT3, HIGH);

  lcd.setCursor(0,0);  
  lcd.print("thermostat with");
  
  lcd.setCursor(0,1);  
  lcd.print("humidity control");

delay(3000);
lcd.clear();

  lcd.setCursor(0,0);  
  lcd.print("original sketch");
  
  lcd.setCursor(0,1);  
  lcd.print("ver.1.3a- niq_ro");
delay(3000);
lcd.clear();

// preset value for temperature and humidity
tes = 28.9;   
dete = 0.2;
hass = 58;
dehas = 2;
  
}

void loop()
{

if (digitalRead(BUT3) == LOW) 
{ meniu = meniu + 1;
lcd.clear();
delay(250);
}
if (meniu >= 7) meniu = 0;

if (meniu == 0)
{
 if (digitalRead(BUT3) == LOW) 
  { meniu = 1;
  delay(250);
  }  
  
   // Reading temperature or humidity 
 has = dht.readHumidity();

/*
 te = dht.readTemperature(); 
delay(250);
*/

 // 9 bit resolution by default 
  // Note the programmer is responsible for the right delay
  // we could do something usefull here instead of the delay
  int resolution = 11;
  sensors.setResolution(resolution);
  delay(750/ (1 << (12-resolution)));

// using DS18B20 ( http://arduinoprojects.ru/2014/08/%D0%BF%D1%80%D0%BE%D1%81%D1%82%D0%BE%D0%B9-%D1%82%D0%B5%D1%80%D0%BC%D0%BE%D1%81%D1%82%D0%B0%D1%82-%D0%BD%D0%B0-arduino-%D0%B8-%D1%86%D0%B8%D1%84%D1%80%D0%BE%D0%B2%D0%BE%D0%BC-%D1%82%D0%B5%D1%80%D0%BC/ )
sensors.requestTemperatures(); // запрос на получение температуры
float te=(sensors.getTempCByIndex(0)); 


// part from http://nicuflorica.blogspot.ro/2014/10/ceas-rtc-cu-ds1307-si-date-mediu-cu.html
 lcd.setCursor(0, 0);
  // lcd.print("t=");
 if (te < 0) 
 {t1=-te;}
 else t1=te;
  
    if ( t1 < 10)
   {
     lcd.print(" "); 
   }
   if (te>0) lcd.print("+"); 
   if (te==0) lcd.print(" "); 
   if (te<0) lcd.print("-");
   lcd.print(t1,1);
//   lcd.write(0b11011111);
   lcd.write(byte(0));
   lcd.print("C  ");

   lcd.setCursor(10, 0);
  // lcd.print("H=");
   lcd.print(has);
   lcd.print("%RH");


if (te > tes) 
 {
if (heat == 1) digitalWrite(temppeste, LOW); 
if (heat == 0) digitalWrite(temppeste, HIGH); 
   lcd.setCursor(0, 1);
//   lcd.print("OVER ");
lcd.print("H0T   ");
 } 
if (tes - dete > te)
  {
if (heat == 0) digitalWrite(temppeste, LOW); 
if (heat == 1) digitalWrite(temppeste, HIGH); 

   lcd.setCursor(0, 1);
//   lcd.print("BELOW");
      lcd.print("COLD ");
 } 
if ((te <= tes) and (tes - dete <= te))
 {
   lcd.setCursor(0, 1);
   lcd.print("OK !  ");
 } 


 if (has > hass) 
 {
if (dry == 1) digitalWrite(umidpeste, HIGH); 
if (dry == 0) digitalWrite(umidpeste, LOW); 

   lcd.setCursor(10, 1);
//   lcd.print("OVER ");
      lcd.print("WET ");
 } 
 if (has < hass - dehas) 
 {
if (dry == 0) digitalWrite(umidpeste, HIGH); 
if (dry == 1) digitalWrite(umidpeste, LOW); 
   lcd.setCursor(10, 1);
//   lcd.print("BELOW");
   lcd.print("DRY ");
 } 
 if ((has <= hass) and (has >= hass - dehas))
 {  lcd.setCursor(10, 1);
   lcd.print("OK !  ");
}
} // last line for MENIU = 0


if (meniu == 1) {
  while (meniu == 1) {
//   teset(tset);
   lcd.setCursor(0, 0);
   lcd.print("Temperature SET:");
   lcd.setCursor(0, 1);
//   lcd.print("+");
   lcd.print(tes,1);
//   lcd.write(0b11011111);
   lcd.write(byte(0));
   lcd.print("C  ");

   lcd.setCursor(8, 1);
   lcd.print("dt=");
   lcd.print(dete,1);
//   lcd.write(0b11011111);
   lcd.write(byte(0));
   lcd.print("C  ");


   if (digitalRead(BUT1) == LOW)
  { tes = tes - 0.1; 
  delay(250);
  }
   if (digitalRead(BUT2) == LOW)
  { tes = tes + 0.1;
  delay(250);
  }
  if (digitalRead(BUT3) == LOW) 
  { meniu = 2;
  delay(250);
  lcd.clear();
  }
//  delay(15);
     }
   delay (100);
}   // end loop for MENIU = 1


if (meniu == 2) {
 // if (tmax <= tmin) tmax = tmin + 10;
   while (meniu ==2) {
 //    maxim(dt/10); // this is number to diplay
 //    dete(dt); // this is number to diplay
   lcd.setCursor(0, 0);
   lcd.print("hyst.temp. SET:");
   lcd.setCursor(0, 1);
//   lcd.print("+");
   lcd.print(tes,1);
//   lcd.write(0b11011111);
   lcd.write(byte(0));
   lcd.print("C  ");

   lcd.setCursor(8, 1);
   lcd.print("dt=");
   lcd.print(dete,1);
//   lcd.write(0b11011111);
   lcd.write(byte(0));
   lcd.print("C  ");

    if (digitalRead(BUT1) == LOW) 
   { dete = dete - 0.1;
   delay(250);
  }
   if (digitalRead(BUT2) == LOW) 
   { dete = dete + 0.1;
   delay(250);
  }
   if (digitalRead(BUT3) == LOW) 
   { meniu = 3;
   delay(250);
   lcd.clear();
  }
//   delay(15);
if (dete < 0.1) dete = 0.1;
    }
}  // end loop for MENIU = 2


if (meniu == 3) {
  while (meniu == 3) {
//   teset(tset);
   lcd.setCursor(0, 0);
   lcd.print("Humidity SET:");
   lcd.setCursor(0, 1);
//   lcd.print("+");
   lcd.print(hass,1);
//   lcd.write(0b11011111);
//   lcd.write(byte(0));
   lcd.print("%RH  ");

   lcd.setCursor(7, 1);
   lcd.print("dh=");
   lcd.print(dehas);
//   lcd.write(0b11011111);
//   lcd.write(byte(0));
   lcd.print("%RH");


   if (digitalRead(BUT1) == LOW)
  { hass = hass - 1; 
  delay(250);
  }
   if (digitalRead(BUT2) == LOW)
  { hass = hass + 1;
  delay(250);
  }
  if (digitalRead(BUT3) == LOW) 
  { meniu = 4;
  delay(250);
  lcd.clear();
  }
//  delay(15);
     }
   delay (250);
}  // end loop for MENIU =3


if (meniu == 4) {
   while (meniu ==4) {
   lcd.setCursor(0, 0);
   lcd.print("hyst.humid. SET:");
   lcd.setCursor(0, 1);
//   lcd.print("+");
   lcd.print(hass);
//   lcd.write(0b11011111);
   lcd.write(byte(0));
   lcd.print("%RH");

   lcd.setCursor(7, 1);
   lcd.print("dh=");
   lcd.print(dehas);
//   lcd.write(0b11011111);
   lcd.write(byte(0));
   lcd.print("%RH");

 
   if (digitalRead(BUT1) == LOW) 
   { dehas = dehas - 1;
   delay(250);
  }
   if (digitalRead(BUT2) == LOW) 
   { dehas = dehas + 1;
   delay(250);
  }
   if (digitalRead(BUT3) == LOW) 
   { meniu = 5;
   delay(250);
   lcd.clear();
  }
//   delay(15);
if (dehas < 1) dehas = 1;
    }
}  // end loop for MENIU = 4


if (meniu == 5) {
   while (meniu ==5) {
   lcd.setCursor(0, 0);
   lcd.print("thermostat type:");
   lcd.setCursor(0, 1);

if (heat == 1) 
{
   lcd.print("HEATER ");
}
if (heat == 0) 
{
   lcd.print("COOLER ");
}

   if (digitalRead(BUT1) == LOW) 
   { heat = 0;
   delay(250);
  }
   if (digitalRead(BUT2) == LOW) 
   { heat = 1;
   delay(250);
  }
   if (digitalRead(BUT3) == LOW) 
   { meniu = 6;
   delay(250);
   lcd.clear();
  }
  }
}   // end loop for end MENIU = 5


if (meniu == 6) {
   while (meniu ==6) {
   lcd.setCursor(0, 0);
   lcd.print("Humidity control");
   lcd.setCursor(0, 1);

if (dry == 1) 
{
   lcd.print("DRYER ");
}
if (dry == 0) 
{
   lcd.print("WETTER ");
}

   if (digitalRead(BUT1) == LOW) 
   { dry = 0;
   delay(250);
  }
   if (digitalRead(BUT2) == LOW) 
   { dry = 1;
   delay(250);
  }
   if (digitalRead(BUT3) == LOW) 
   { meniu = 7;
   delay(250);
   lcd.clear();
  }
  }
}   // end loop for end MENIU = 6



}   // last line in main loop
