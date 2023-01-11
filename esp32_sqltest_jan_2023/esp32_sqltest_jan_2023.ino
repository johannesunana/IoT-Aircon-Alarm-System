#if ! (ESP8266 || ESP32 )
#error This code is intended to run on the ESP8266/ESP32 platform! Please check your Tools->Board setting
#endif

#define SHT31_ADDRESS 0x44
#define MYSQL_DEBUG_PORT      Serial
#define _MYSQL_LOGLEVEL_      4

#include <MySQL_Generic.h>        // khoih-prog/MySQL_MariaDB_Generic 1.7.2
#include "SHT31.h"                // RobTillaart/SHT31
#include "Credentials.h"

/* Required variables for digital and analog sensors */
// A) SHT31 temperature current
SHT31 sht;
bool enableHeater = false;
uint32_t start;
uint32_t stop;
// B) MySQL
char INSERT_TEMPHMD[] = "INSERT INTO %s.%s (device_id, temp_data, hmd_data) VALUES (%d, %s, %s)";
float float_tempReading, float_humidityReading;
char query1[100];
char temp_char[10];
char hmd_char[10];

byte device_id = 1;
// Initialize MySQL_MariaDB_Generic library
MySQL_Connection conn((Client *)&client);
MySQL_Query *query_mem;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  sht.begin(SHT31_ADDRESS);
  sht.begin();
  Wire.setClock(100000);
  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();

  MYSQL_DISPLAY1("\nStarting Complex_Insert_WiFi on", ARDUINO_BOARD);
  MYSQL_DISPLAY(MYSQL_MARIADB_GENERIC_VERSION);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    MYSQL_DISPLAY0(".");
  }


  MYSQL_DISPLAY1("Connected to network. My IP address is:", WiFi.localIP());
  MYSQL_DISPLAY3("Connecting to SQL Server @", server_addr, ", Port =", server_port);
  MYSQL_DISPLAY5("User =", user, ", PW =", password, ", DB =", database);
}

void runInsert() {
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);

  if (conn.connected()) {
    // Convert floats to strings before insert
    dtostrf(float_tempReading, 4, 2, temp_char);
    dtostrf(float_humidityReading, 4, 2, hmd_char);

    // Insert char strings to placeholders in single query string
    sprintf(query1, INSERT_TEMPHMD, database, table1, device_id, temp_char, hmd_char);

    // Execute the query
    MYSQL_DISPLAY1("Query1", query1);

    // KH, check if valid before fetching
    if ( !query_mem.execute(query1) ) {
      MYSQL_DISPLAY("Query 1: Insert error");
    }
    else {
      MYSQL_DISPLAY("Query 1: Data Inserted.");
    }
  }
  else MYSQL_DISPLAY("Disconnected from Server. Can't insert.");
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
}

void loop() {
  MYSQL_DISPLAY("Connecting to server");
  if (conn.connectNonBlocking(server_addr, server_port, user, password) != RESULT_FAIL) {
    MYSQL_DISPLAY("\nConnect success");

    // functions
    tempHumidity();
    runInsert();
    conn.close();
  }

  else MYSQL_DISPLAY("\nConnect failed");
  delay(10000);
  // End of loop

}
