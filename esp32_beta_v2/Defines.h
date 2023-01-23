#ifndef Defines
#define Defines

/* I. Definitions */
#define   SHT31_ADDRESS 0x44
#define   IOUT          39    // input for current sensor Vout
#define   VOUT          35    // input for freon sensor Vout
#define   VREF          33    // input for freon sensor Vref
#define   MYSQL_DEBUG_PORT      Serial
#define   _MYSQL_LOGLEVEL_      3
int LED_BUILTIN = 2;
float vin = 5.00;     // Vin for ADC

#define ENABLE_TEMPHUMIDITY     true
#define ENABLE_ACCELEROMETER    true
#define ENABLE_CURRENT          true
#define ENABLE_GAS              true
#define ENABLE_UPLOAD           true

#include <Wire.h>                 // I2C 
#include <SHT31.h>                // RobTillaart/SHT31 0.3.7
#include <Adafruit_ADXL345_U.h>   // adafruit/Adafruit_ADXL345 1.3.2
#include <ACS712.h>               // RobTillaart/ACS712 0.3.4
#include <MySQL_Generic.h>        // khoih-prog/MySQL_MariaDB_Generic 1.7.2

/* II. Required variables for digital and analog sensors */

#if ENABLE_TEMPHUMIDITY
// A) SHT31 temperature humidity sensor
SHT31 sht;
bool enableHeater = false;
uint32_t start;
uint32_t stop;
float tempReading_float, humidityReading_float;
#endif

#if ENABLE_ACCELEROMETER
// B) ADXL345 digital accelerometer
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
float x2, x3, y2, y3, z2, z3, r, r2;
float d = 0;      // determining condition
float dx, dy, dz, dr;
float t, h;
#endif

#if ENABLE_CURRENT
// C) ACS712 analog current sensor
ACS712  ACS(IOUT, 3.3, 4095, 185);
float currentReading_float;
#endif

#if ENABLE_GAS
// D) FCM2630-C01 freon sensor
float vout_adc, vref_adc; // initial values from analogRead
float vout_float, vref_float;  // converted to volts
byte vout_status, vref_status, alarm_status;
#endif

// E) Device ID
byte device_id = 1;

#if ENABLE_UPLOAD
// F) MySQL Connector
// 1 Initialize MySQL_MariaDB_Generic library
MySQL_Connection conn((Client *)&client);
MySQL_Query *query_mem;
// 2 using INSERT INTO
char INSERT_TEMPHMD[] = "INSERT INTO %s.%s (device_id, temp_data, hmd_data) VALUES (%d, %s, %s)";
char INSERT_ACC[] = "INSERT INTO %s.%s (device_id, r_data) VALUES (%d, %s)";
char INSERT_CURRENT[] = "INSERT INTO %s.%s (device_id, amp_data) VALUES (%d, %s)";
char INSERT_GAS[] = "INSERT INTO %s.%s (device_id, vout_data, vref_data, vout_status, vref_status, alarm_status) VALUES (%d, %s, %s, %d, %d, %d)";

// char variables for sql script
char query1[200];   // INSERT_TEMPHMD[]
char query2[200];   // INSERT_ACC[]
char query3[200];   // INSERT_CURRENT[]
char query4[200];   // INSERT_GAS[]

// char variables for dtostrf type conversion
char temp_char[50];
char hmd_char[50];
char r_char[50];
char current_char[50];
char vout_char[50];
char vref_char[50];

char table1[]  = "data_temp_hmd";
char table2[]  = "data_vibration";    // table renamed at database
char table3[]  = "data_amp";
char table4[]  = "data_gas";
#endif

#endif Defines
