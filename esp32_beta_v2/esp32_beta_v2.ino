// Integrated 20A current sensor and acceleromter
// Using MySQL_MariaDB_Generic library
// Board: ESP32 Dev Kit
// Edited: January 11, 2023

/* I. Libraries */
#include <Wire.h>                 // I2C 
#include <MySQL_Generic.h>        // khoih-prog/MySQL_MariaDB_Generic 1.7.2
#include <ACS712.h>               // RobTillaart/ACS712 0.3.1
#include <Adafruit_ADXL345_U.h>   // adafruit/Adafruit_ADXL345 1.3.2
#include <SHT31.h>                // RobTillaart/SHT31
#include "Credentials.h"
#include <math.h>

/* II. Definitions */
#define   SHT31_ADDRESS 0x44
#define   IOUT          39    // input for current sensor Vout
#define   VOUT          35    // input for freon sensor Vout
#define   VREF          33    // input for freon sensor Vref
#define   MYSQL_DEBUG_PORT      Serial
#define   _MYSQL_LOGLEVEL_      3

// const int ledPin =  2; // indicator that device connects and uploads

/* III. Required variables for digital and analog sensors */
// A) SHT31 temperature humidity sensor
SHT31 sht;
bool enableHeater = false;
uint32_t start;
uint32_t stop;
float tempReading_float, humidityReading_float;

// B) ACS712 analog current sensor
//ACS712  ACS(IOUT, 3.3, 4095, 185);
float currentReading_float;

// C) ADXL345 digital accelerometer
//Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
float x2, x3, y2, y3, z2, z3, r, r2;
float d = 0;      // determining condition
float dx, dy, dz, dr;
float t, h;

// D) FCM2630-C01 freon sensor
float vout_adc, vref_adc; // initial values from analogRead
float vout_float, vref_float;  // converted to volts

// E) Device ID
byte device_id = 1;

// F) MySQL Connector
// 1 Initialize MySQL_MariaDB_Generic library
MySQL_Connection conn((Client *)&client);
MySQL_Query *query_mem;
// 2 using INSERT INTO
char INSERT_TEMPHMD[] = "INSERT INTO %s.%s (device_id, temp_data, hmd_data) VALUES (%d, %s, %s)";
char INSERT_ACC[] = "INSERT INTO %s.%s (device_id, r_data) VALUES (%d, %s)";
char INSERT_CURRENT[] = "INSERT INTO %s.%s (device_id, amp_data) VALUES (%d, %s)";
char INSERT_GAS[] = "INSERT INTO %s.%s (device_id, vout_data, vref_data, vout_status, vref_status, alarm_status) VALUES (%d, %s, %s, %d, %d, %d)";
//byte vout_status, vref_status, alarm_status;
char query1[100];
char query2[100];
char query3[100];
char query4[100];
char temp_char[10];
char hmd_char[10];
char r_char[10];
char current_char[10];
char vout_char[10];
char vref_char[10];

void setup() {
  Serial.begin(115200);     // 115200 or 9600
  Wire.begin();             // start I2C
  Wire.setClock(100000);
  sht.begin(SHT31_ADDRESS); // start SHT31
  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  //  Serial.println();
  //  if (!accel.begin()) {     // start accelerometer
  //    Serial.println("No ADXL345 detected");
  //    while (1);
  //  }
  //  accel.setRange(ADXL345_RANGE_16_G);
  //  ACS.autoMidPoint();       // start current sensor
}

void loop() {
  //  MYSQL_DISPLAY("Connecting to server");
  //  if (conn.connectNonBlocking(server_addr, server_port, user, password) != RESULT_FAIL) {
  //    MYSQL_DISPLAY("\nConnect success");
  //
  // functions
  tempHumidity();
  //    accelerometer();
  //    current();
  //    gasSensor();
  //    runInsert();
  //    conn.close();
  //  }

  //  else MYSQL_DISPLAY("\nConnect failed");
  //  delay(10000);   // retry after 10 seconds
  //  End of loop
}

void tempHumidity() {
  start = micros();
  sht.read();
  stop = micros();

  Serial.print("\t");
  Serial.print(stop - start);
  Serial.print("\t");
  float float_tempReading = sht.getTemperature();
  Serial.print(float_tempReading, 1);
  Serial.print("\t");
  float float_humidityReading = sht.getHumidity();
  Serial.println(float_humidityReading, 1);
  delay(100);
}

//void accelerometer() { // c/o Melvin, do not touch
//  sensors_event_t event;
//  accel.getEvent(&event);
//  if (d <= 0) {
//    x2 = event.acceleration.x;      //Initial Position of X
//    y2 = event.acceleration.y;      //Initial Position of y
//    z2 = event.acceleration.z;      //Initial Position of z
//    r = 0;
//  }
//
//  dx = event.acceleration.x - x2;  //Changes in position of x
//  dy = event.acceleration.y - y2;  //Changes in position of y
//  dz = event.acceleration.z - z2;  //Changes in position of z
//
//  x3 = powf((dx), 2);    //acceleration of x with respect to the initial calibrated position
//  y3 = powf((dy), 2);    //acceleration of y with respect to the initial calibrated position
//  z3 = powf((dz), 2);    //acceleration of z with respect to the initial calibrated position
//  if (d > 0) {
//    r = sqrtf(x3 + y3 + z3);
//    if (r <= 0.2) { //if the change is very minimal or negligible set value to 0
//      r = 0;
//    }
//  }
//  d++;
//  delay(250);
//  Serial.print("R = ");
//  Serial.print(r);
//  Serial.println();
//}
//
//void current() {
//  currentReading_float = ACS.mA_AC() / 1000;
//  Serial.print("A: ");
//  delay(100);
//}
//
//void gasSensor() {
//  vout_adc = analogRead(VOUT);
//  vref_adc = analogRead(VREF);
//  Serial.print("VOUT = ");
//  Serial.print(vout_float);
//  Serial.print("\t\tVREF");
//  Serial.print(vref_float);
//  delay(250);
//
//  //  dtostrf(vout_float, 4, 2, vout_char);
//  //  dtostrf(vref_float, 4, 2, vref_char);
//}
//
//void runInsert() {
//  // Initiate the query class instance
//  MySQL_Query query_mem = MySQL_Query(&conn);
//
//  if (conn.connected()) {
//    // Convert floats to strings before insert
//    // dtostf == double to string
//    dtostrf(tempReading_float, 4, 2, temp_char);
//    dtostrf(humidityReading_float, 4, 2, hmd_char);
//    dtostrf(r, 4, 2, r_char);
//    dtostrf(currentReading_float, 4, 2, current_char);
//    dtostrf(vout_float, 4, 2, vref_char);
//    dtostrf(vref_float, 4, 2, vref_char);
//
//    // Insert char strings to placeholders in single query string
//    // sprintf == string print
//    sprintf(query1, INSERT_TEMPHMD, database, table1, device_id, temp_char, hmd_char);
//    sprintf(query2, INSERT_ACC, database, table2, device_id, r_char);
//    sprintf(query3, INSERT_CURRENT, database, table3, device_id, current_char);
//    sprintf(query4, INSERT_GAS, database, table4, device_id, vout_char, vref_char);
//    /* char INSERT_GAS[] = "INSERT INTO %s.%s (device_id, vout_data, vref_data, vout_status, vref_status, alarm_status) VALUES (%d, %s, %s, %d, %d, %d)"; */
//
//    // Execute query1
//    MYSQL_DISPLAY1("Query1", query1);
//    // KH, check if valid before fetching
//    if ( !query_mem.execute(query1) ) {
//      MYSQL_DISPLAY("Query 1: Insert error");
//    }
//    else {
//      MYSQL_DISPLAY("Query 1: Data Inserted.");
//    }
//
//    // Execute query2
//    MYSQL_DISPLAY1("Query2", query2);
//    // KH, check if valid before fetching
//    if ( !query_mem.execute(query2) ) {
//      MYSQL_DISPLAY("Query 2: Insert error");
//    }
//    else {
//      MYSQL_DISPLAY("Query 2: Data Inserted.");
//    }
//
//    // Execute query3
//    MYSQL_DISPLAY1("Query3", query3);
//    // KH, check if valid before fetching
//    if ( !query_mem.execute(query3) ) {
//      MYSQL_DISPLAY("Query 3: Insert error");
//    }
//    else {
//      MYSQL_DISPLAY("Query 3: Data Inserted.");
//    }
//
//    // Execute query4
//    MYSQL_DISPLAY1("Query4", query4);
//    // KH, check if valid before fetching
//    if ( !query_mem.execute(query4) ) {
//      MYSQL_DISPLAY("Query 4: Insert error");
//    }
//    else {
//      MYSQL_DISPLAY("Query 4: Data Inserted.");
//    }
//  }
//  else MYSQL_DISPLAY("Disconnected from Server. Can't insert.");
//}
