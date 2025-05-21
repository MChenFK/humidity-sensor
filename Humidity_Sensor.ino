#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

bool PLX_flag = true;
bool enableHeater = false;
uint8_t loopCnt = 0;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(9600);
  delay(10000);

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  if (! PLX_flag){
    Serial.println("SHT31 test");
  }
  
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    if (! PLX_flag){
      Serial.println("Couldn't find SHT31");
    }
    while (1) delay(1);
  }

  if (! PLX_flag){
      Serial.print("Heater Enabled State: ");
    if (sht31.isHeaterEnabled())
      Serial.println("ENABLED");
    else
      Serial.println("DISABLED");
  }

  if (PLX_flag) {
    Serial.println("LABEL, time, temp (C), humidity (%)");
  }
  
}


void loop() {
  if (PLX_flag) {
    Serial.print("DATA,TIME,");
  }
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (! isnan(t)) {  // check if 'is not a number'
    if (! PLX_flag) {
      Serial.print("Temp *C = ");
    }
    Serial.print(t); 
    if (! PLX_flag) {
      Serial.print("\t\t");
    }
    if (PLX_flag) {
      Serial.print(", ");
    }
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    if (! PLX_flag) {
      Serial.print("Hum. % = ");
      Serial.println(h);
    } else {
      Serial.println(h);
    }
    
  } else { 
    Serial.println("Failed to read humidity");
  }

  delay(1000);

  // Toggle heater enabled state every 30 seconds
  // An ~3.0 degC temperature increase can be noted when heater is enabled
  if (loopCnt >= 30) {
    enableHeater = !enableHeater;
    sht31.heater(enableHeater);
    if (! PLX_flag) {
      Serial.print("Heater Enabled State: ");
      if (sht31.isHeaterEnabled())
        Serial.println("ENABLED");
      else
        Serial.println("DISABLED");
    }

    loopCnt = 0;
  }
  loopCnt++;
}