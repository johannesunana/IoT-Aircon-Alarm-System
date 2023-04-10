// Integrated sketch for data gathering module
// Board: ESP32 Dev Module
// Using espressif/arduino-esp32 2.0.7
// Updated: April 2023
// Created by: Johannes Unana, Melvin Sta. Rosa Jr., Bon Joey Pintant

#include "Credentials.h"
#include "Defines.h"

void setup() {
  Serial.begin(115200);
  pinMode (LED_BUILTIN, OUTPUT);

  // start SHT31
  sht.begin(SHT31_ADDRESS);

  // start I2C
  Wire.begin();
  Wire.setClock(100000);

  //  start accelerometer
  if (!accel.begin()) {
    Serial.println("No ADXL345 detected");
    while (1);
  }
  accel.setRange(ADXL345_RANGE_16_G);

  // start current sensor
ACS.autoMidPoint();

  WiFi.begin(ssid, pass);
//  WiFi.mode(WIFI_STA);      // station mode, ESP32 connects to an access point
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    MYSQL_DISPLAY0(".");
  }
}

void loop() {
  MYSQL_DISPLAY("Connecting to server");
  if (conn.connectNonBlocking(server_addr, server_port, user, password) != RESULT_FAIL) {
    MYSQL_DISPLAY("\nConnect success");
    // functions to be executed in the program
    tempHumidity();
    accelerometer();
    current();
    gasSensor();
    runInsert();
    conn.close();
  }
  else MYSQL_DISPLAY("\nConnect failed");
  delay(3000);   // repeat loop after 3 seconds
  //  End of loop function
}

#if ENABLE_TEMPHUMIDITY
void tempHumidity() {
  start = micros();
  sht.read();
  stop = micros();
  float tempReading_float = sht.getTemperature();
  Serial.print("T = ");
  //  tempReading_float = 36.5;         // sample reading for test upload, comment when using sensor
  Serial.print(tempReading_float, 1);
  Serial.print("C\tH = ");
  float humidityReading_float = sht.getHumidity();
  // humidityReading_float = 67.0;      // sample reading for test upload, comment when using sensor
  Serial.print(humidityReading_float, 1);
  Serial.println("%");
  delay(100);
  //  End of tempeHumidity function
}
#endif

void accelerometer() { // c/o Melvin, do not modify if unsure
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
  delay(100);
  Serial.print("R = ");
  Serial.println(r);
  //  End of accelerometer function
}

void current() {
  currentReading_float = ACS.mA_AC() / 1000;
  Serial.print("A: ");
  Serial.println(currentReading_float);
  delay(100);
  // End of current function
}

void gasSensor() {
  vout_adc = analogRead(VOUT);
  vout_float = (vout_adc * vin) / 1023;
  vref_adc = analogRead(VREF);
  vref_float = (vref_adc * vin) / 1023;

  if (vout_float < 0.05 || vout_float > 4.95) {
    vout_status = 1;        // malfunction
  }
  else vout_status = 0;     // normal

  if (vref_float < 2.50 || vref_float > 3.70) {
    vref_status = 1;        // malfunction
  }
  else vref_status = 0;     // normal

  if (vout_status == 1 || vref_status == 1) {
    alarm_status = 2;       // malfunction state
  }
  else if (vout_status == 0 || vref_status == 0) {
    if (vout_float >= vref_float) {
      alarm_status = 1;     // alarm state
    }
    else alarm_status = 0;  // normal state
  }
  Serial.print("VOut = ");
  Serial.print(vout_float);
  Serial.print("\tVRef");
  Serial.println(vref_float);
  delay(100);
  //  End of gasSensor function
}

void runInsert() {  // connect and upload to a mysql database
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);

  if (conn.connected()) {
    digitalWrite(LED_BUILTIN, HIGH);
    //  Convert floats to strings before insert
    //  dtostf == double to string function
    dtostrf(tempReading_float, 4, 2, temp_char);
    dtostrf(humidityReading_float, 4, 2, hmd_char);
    dtostrf(r, 4, 2, r_char);
    dtostrf(currentReading_float, 4, 2, current_char);
    dtostrf(vout_float, 4, 2, vout_char);
    dtostrf(vref_float, 4, 2, vref_char);

    // Insert char strings to placeholders in single query string
    // sprintf == string print
    sprintf(query1, INSERT_TEMPHMD, database, table1, device_id, temp_char, hmd_char);
    sprintf(query2, INSERT_ACC, database, table2, device_id, r_char);
    sprintf(query3, INSERT_CURRENT, database, table3, device_id, current_char);
    sprintf(query4, INSERT_GAS, database, table4, device_id, vout_char, vref_char, vout_status, vref_status, alarm_status);

    //     Execute query1 INSERT_TEMPHMD[]
    MYSQL_DISPLAY1("Query1", query1);
    // KH, check if valid before fetching
    if (!query_mem.execute(query1)) {
      MYSQL_DISPLAY("Query 1: Insert error");
    }
    else MYSQL_DISPLAY("Query 1: Data Inserted.");

    //   Execute query2 INSERT_ACC[]
    MYSQL_DISPLAY1("Query2", query2);
    if (!query_mem.execute(query2)) {
      MYSQL_DISPLAY("Query 2: Insert error");
    }
    else MYSQL_DISPLAY("Query 2: Data Inserted.");

    // Execute query3 INSERT_CURRENT[]
    MYSQL_DISPLAY1("Query3", query3);
    if (!query_mem.execute(query3)) {
      MYSQL_DISPLAY("Query 3: Insert error");
    }
    else MYSQL_DISPLAY("Query 3: Data Inserted.");

    // Execute query4 INSERT_GAS[]
    MYSQL_DISPLAY1("Query4", query4);
    if (!query_mem.execute(query4)) {
      MYSQL_DISPLAY("Query 4: Insert error");
    }
    else MYSQL_DISPLAY("Query 4: Data Inserted.");
    digitalWrite(LED_BUILTIN, LOW);
  }
  else MYSQL_DISPLAY("Disconnected from Server. Can't insert.");
  //  End of runInsert function
}
