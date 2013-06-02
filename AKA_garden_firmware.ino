



/*
This sketch will read:
 - liquid level sensor
 - thermistors in bed and reservoir
 - photocell in bed
 
 ...and post to Cosm.
 
 As of 06/02/2013, all sensors are working but writing to the Serial Display still doesn't work.
 To Do:
 * fix serial display
 * add LED status indicator for liquid level? (Or LED countdown to next flood?)
 * add UI affordance: click RIGHT to indicate one gallon of water added (this gets logged in separate field in DB)
 
 
 A0 = thermistor 1 (reservoir) (needs voltage divider 10K)
 A1 = thermistor 2 (bed / environment) (needs voltage divider 10K)
 A2 = reservoir level (needs voltage divider 560ohm, provided)
 A3 = photocell (needs voltage divider 10K)
 A4 and A5 - reserved for I2C channel
 D7 - pump relay.
 
 D8 and D9 - serial to openLog
 
 D10 and D11 - serial to LCD temperature display?
 
 */

#include "SoftwareSerial.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <Math.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define LED_PIN 13 // board's status LED

int RESERVOIR_TEMP = A0; // 10k NTC thermistor
int BED_TEMP = A1; // 10k NTC thermistor
int RESERVOIR_LEVEL = A2; // 8-inch eTape liquid level sensor
int PHOTOCELL = A3; // junk photocell I had laying around
#define RELAY_1 7

#define DISPLAY_TX 8
#define DISPLAY_RX 9
#define LOG_RX 10
#define LOG_TX 11

boolean isPumping = false;
boolean firstTimeLogging = true;

unsigned long lastLogged = 0;
unsigned long lastAction = 0;
double intervalDuration = 0;
double loggingInterval = (1.0 * 60 * 1000); // 1 minute
double pumpDuration = (2.0 * 60 * 1000); // 2 minutes
double drainDuration = (6.0 * 60 * 1000); // 6 minutes = 360000 ms

SoftwareSerial logSerial = SoftwareSerial(LOG_RX, LOG_TX);
SoftwareSerial displaySerial = SoftwareSerial(DISPLAY_RX, DISPLAY_TX);

unsigned long currentTime = 0;
float bed_temp;
float reservoir_temp;
float reservoir_level;
float photocell;
int PUMP = 0;
int gals_added = 0;
int log_count = 0;

float sensors[4];
char* sensorLabels[] = {
  "Bed tC: ","Res tC: ","ResLev: ","Photo: "};

int sensorIndex = 0; // used to keep track of which val to display on LCD
int digit[4];
int displayCounter = 100000;

void setup()
{

  pinMode(LED_PIN,OUTPUT);
  pinMode(RELAY_1,OUTPUT);
  pinMode(RESERVOIR_TEMP, INPUT);
  pinMode(RESERVOIR_LEVEL, INPUT);
  pinMode(PHOTOCELL, INPUT);
  pinMode(BED_TEMP, INPUT);

  lcd.begin(16, 2);
  lcd.setBacklight(RED);
  displaySerial.begin(9600);
  displaySerial.write('v');
  logSerial.begin(9600);
  displaySerial.write(0x7A);
  displaySerial.write(0xFF);
  
  intervalDuration = pumpDuration;

  Serial.begin(9600);
}

void loop() {
    
  currentTime = millis();

  readSensors();

  // LOG STUFF
  if(currentTime-lastLogged > loggingInterval){
    logValues();
    log_count++;
    lastLogged = currentTime;
  }

  // WRITE TO SERIAL DISPLAY
  displayPrint(displayCounter);

  // ACTUATE PUMP
  if( (currentTime-lastAction) > (intervalDuration) ){

    if(!isPumping){
      digitalWrite(RELAY_1,HIGH);
      intervalDuration = pumpDuration;
    }
    else {
      digitalWrite(RELAY_1,LOW); 
      intervalDuration = drainDuration;
    }

    lastAction = currentTime;
    displayCounter = intervalDuration;
    isPumping = !isPumping;
  } // end of trigger-on-interval loop


// CHECK UI
  uint8_t buttons = lcd.readButtons();

  if (buttons) {

    if (buttons & BUTTON_UP) {
      sensorIndex += 2;
    }
    if (buttons & BUTTON_DOWN) {
      sensorIndex += 3; // since we do mod 4, this is equiv to -1
    }
    if (buttons & BUTTON_LEFT) {
      // nothing
    }
    if (buttons & BUTTON_RIGHT) {
      gals_added++;
      // nothing
    }
    if (buttons & BUTTON_SELECT) {
      // nothing
    }
  } 
  else { // default display mode

  }

  // WRITE TO UI
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(sensorLabels[sensorIndex%4]);
  lcd.print(" ");
  lcd.print(sensors[sensorIndex%4]);
  lcd.setCursor(0,1);
  lcd.print(sensorLabels[(sensorIndex+1)%4]);
  lcd.print(" ");
  lcd.print(sensors[(sensorIndex+1)%4]);


  displayCounter--;
  delay(100);

} // end of main loop


































