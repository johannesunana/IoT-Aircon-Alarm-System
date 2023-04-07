#ifndef Credentials_h
#define Credentials_h

#define USING_HOST_NAME           false

#if USING_HOST_NAME
  char server_addr[] = "********";
#else
  IPAddress server_addr(192, 168, 254, 254);
#endif

uint16_t server_port = 3306;

char ssid[] = "********";             // your network SSID (name)
char pass[] = "********";         // your network password

char user[]         = "********";              // MySQL user login username
char password[]     = "********";          // MySQL user login password
char database[] = "********";

#endif    //Credentials_h
