#ifndef Credentials_h
#define Credentials_h

#define USING_HOST_NAME           false

#if USING_HOST_NAME
  // char server_addr[] = "raspberrypi.local";
  char server_addr[] = "asusx505za.local";
#else
  IPAddress server_addr(192, 168, 254, 104);
#endif

uint16_t server_port = 3306;

char ssid[] = "HelloKitty";             // your network SSID (name)
char pass[] = "JVA102123";         // your network password

char user[]         = "arduino";              // MySQL user login username
char password[]     = "arduino";          // MySQL user login password

char database[] = "adnu_acrms_4";

#endif    //Credentials_h
