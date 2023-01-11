// Integrated 20A current sensor and acceleromter
// Using MySQL_MariaDB_Generic library
// Board: ESP32 Dev Kit
// Edited: January 7, 2023

#include <Wire.h>                 // I2C 
#include <MySQL_Generic.h>        // khoih-prog/MySQL_MariaDB_Generic 1.7.2
#include <ACS712.h>               // RobTillaart/ACS712 0.3.1
#include <Adafruit_ADXL345_U.h>   // adafruit/Adafruit_ADXL345 1.3.2
#include "SHT31.h"                // RobTillaart/SHT31
#include "Credentials.h"
#include <math.h>

#define   SHT31_ADDRESS 0x44
#define   IOUT          39    // input for current sensor Vout
#define   VOUT          35    // input for freon sensor Vout
#define   VREF          33    // input for freon sensor Vref
#define   MYSQL_DEBUG_PORT      Serial
#define   _MYSQL_LOGLEVEL_      3

// const int ledPin =  2; // indicator that device connects and uploads
/* Required variables for digital and analog sensors */
// A) SHT31 temperature current
SHT31 sht;
bool enableHeater = false;
uint32_t start;
uint32_t stop;
// B) ACS712 current
float mA_float;
ACS712  ACS(IOUT, 3.3, 4095, 185);
// C) ADXL345 accelerometer
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
float x2, x3, y2, y3, z2, z3, r, r2;
float d = 0;      // determining condition
float dx, dy, dz, dr;
float t, h;
// D) Device ID
byte device_id = 1;
// E) MySQL Connector
// Initialize MySQL_MariaDB_Generic library
MySQL_Connection conn((Client *)&client);
MySQL_Query *query_mem;
// using INSERT INTO
char INSERT_TEMPHMD[] = "INSERT INTO %s.%s (device_id, temp_data, hmd_data) VALUES (%d, %s, %s)";
char INSERT_GAS[] = "INSERT INTO %s.%s (device_id, vout_data, vref_data, vout_status, vref_status, alarm_status) VALUES (%d, %s, %s, %d, %d, %d)";
// byte vout_status, vref_status, alarm_status;
char INSERT_MA[] = "INSERT INTO %s.%s (device_id, amp_data) VALUES (%d, %s)";
char INSERT_ACC[] = "INSERT INTO %s.%s (device_id, xa_data, ya_data, za_data) VALUES (%d, %s, %s, %s)";

void setup() {
  Serial.begin(115200);
  Wire.begin();
  sht.begin(SHT31_ADDRESS);
  sht.begin();
  Wire.setClock(100000);
  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();
  if (!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("No ADXL345 detected");
    while (1);
  }
  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);
  ACS.autoMidPoint();
}

void loop() {
  tempHumidity();
  accelerometer();
  current();
  gasSensor();
  //  uploadData();
  //  clearData();
}

void tempHumidity() {
  start = micros();
  sht.read();
  stop = micros();

  //  Serial.print("\t");
  //  Serial.print(stop - start);
  //  Serial.print("\t");
  float tempReading = sht.getTemperature();
  //  Serial.print(tempReading, 1);
  //  Serial.print("\t");
  float humidityReading = sht.getHumidity();
  //  Serial.println(humidityReading, 1);
  delay(250);

//  if (! isnan(t)) {  // check if 'is not a number'
//    Serial.print("Temp *C = ");
//    Serial.print(tempReading);
//    Serial.print("\t\t");
//  }
//  else {
//    Serial.println("Failed to read temperature");
//  }
//  if (! isnan(h)) {  // check if 'is not a number'
//    Serial.print("Hum. % = ");
//    Serial.println(humidityReading);
//  }
//  else {
//    Serial.println("Failed to read humidity");
//  }

  //  char vout_char[10];
  //  char vref_char[10];
  //  dtostrf(vout_float, 4, 2, vout_char);
  //  dtostrf(vref_float, 4, 2, vref_char);
}


void accelerometer() // c/o Melvin, do not touch
{
  sensors_event_t event;
  accel.getEvent(&event);
  if (d <= 0) {
    x2 = event.acceleration.x;      //Initial Position of X
    y2 = event.acceleration.y;      //Initial Position of y
    z2 = event.acceleration.z;      //Initial Position of z
    r = 0;
  }

  dx = event.acceleration.x - x2;  //Changes in position of x
  dy = event.acceleration.y - y2;  //Changes in position of y
  dz = event.acceleration.z - z2;  //Changes in position of z

  x3 = powf((dx), 2);    //acceleration of x with respect to the initial calibrated position
  y3 = powf((dy), 2);    //acceleration of y with respect to the initial calibrated position
  z3 = powf((dz), 2);    //acceleration of z with respect to the initial calibrated position
  if (d > 0) {
    r = sqrtf(x3 + y3 + z3);
    if (r <= 0.2) { //if the change is very minimal or negligible set value to 0
      r = 0;
    }
  }
  d++;
  delay(250);
  Serial.print("R = ");
  Serial.print(r);
  Serial.println();
}

void current() {
  Serial.print("A: ");
  Serial.println(mA_float / 1000);
  delay(250);
}

void gasSensor() {

  float vout_float = analogRead(VOUT);
  float vref_float = analogRead(VREF);
  Serial.print("VOUT = ");
  Serial.print(vout_float);
  Serial.print("\t\tVREF");
  Serial.print(vref_float);
  delay(250);


  //  char vout_char[10];
  //  char vref_char[10];
  //  dtostrf(vout_float, 4, 2, vout_char);
  //  dtostrf(vref_float, 4, 2, vref_char);


}

//void uploadtoDatabase() {
//
//
//}
