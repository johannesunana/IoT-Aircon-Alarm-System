#ifndef Defines
#define Defines

/* I. Definitions */
#define   SHT31_ADDRESS 0x44
#define   IOUT          39    // input for current sensor Vout
#define   VOUT          35    // input for freon sensor Vout
#define   VREF          33    // input for freon sensor Vref
#define   MYSQL_DEBUG_PORT      Serial
#define   _MYSQL_LOGLEVEL_      3

/* II. Required variables for digital and analog sensors */
// A) SHT31 temperature humidity sensor
SHT31 sht;
bool enableHeater = false;
uint32_t start;
uint32_t stop;
float tempReading_float, humidityReading_float;

//// B) ACS712 analog current sensor
//ACS712  ACS(IOUT, 3.3, 4095, 185);
//float currentReading_float;
//
//// C) ADXL345 digital accelerometer
//Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
//float x2, x3, y2, y3, z2, z3, r, r2;
//float d = 0;      // determining condition
//float dx, dy, dz, dr;
//float t, h;
//
//// D) FCM2630-C01 freon sensor
//float vout_adc, vref_adc; // initial values from analogRead
//float vout_float, vref_float;  // converted to volts

// E) Device ID
byte device_id = 1;

// F) MySQL Connector
// 1 Initialize MySQL_MariaDB_Generic library
MySQL_Connection conn((Client *)&client);
MySQL_Query *query_mem;
// 2 using INSERT INTO
char INSERT_TEMPHMD[] = "INSERT INTO %s.%s (device_id, temp_data, hmd_data) VALUES (%d, %s, %s)";
//char INSERT_ACC[] = "INSERT INTO %s.%s (device_id, r_data) VALUES (%d, %s)";
//char INSERT_CURRENT[] = "INSERT INTO %s.%s (device_id, amp_data) VALUES (%d, %s)";
//char INSERT_GAS[] = "INSERT INTO %s.%s (device_id, vout_data, vref_data, vout_status, vref_status, alarm_status) VALUES (%d, %s, %s, %d, %d, %d)";
//byte vout_status, vref_status, alarm_status;

// char variables for sql script
char query1[200];   // INSERT_TEMPHMD[]
//char query2[200];   // INSERT_ACC[]
//char query3[200];   // INSERT_CURRENT[]
//char query4[200];   // INSERT_GAS[]

// char variables for dtostrf type conversion
char temp_char[10];
char hmd_char[10];
//char r_char[10];
//char current_char[10];
//char vout_char[10];
//char vref_char[10];

#endif Defines
