## Instructions on how to use and deploy software to ESP32:

1. Setup Wi-Fi connectivity in Raspberry Pi.

   - Download [wpa_supplicant.conf](https://github.com/johannesunana/IoT-Aircon-Alarm-System/blob/main/wpa_supplicant.conf).
   - Edit the file by modifying
   ```c
   ssid="WIFI_SSID" = "********";
   ```
   and
   ```c
   psk="WIFI_PASSWORD" = "********";
   ```
   to the Wi-Fi connection in use.
   - Save and copy to root folder of microSD containing the installed Raspberry PI OS.

2. Setup WiFi connectivity in ESP32.

   - Download the latest release **[here](https://github.com/johannesunana/IoT-Aircon-Alarm-System/archive/master.zip)**.

   - On <u>Credentials.h</u>, edit the following sections:

    ```c
    char server_addr[] = "********";	// IP address of server in local network
    ```
   ```c
   char ssid[] = "********";		// WiFI SSID
   char pass[] = "********";		// Wifi password
   
   char user[] = "********";		// MySQL server username
   char password[] = "********";	// MySQL server password
   char database[] = "********";	// MySQL database name
   ```

3. On <u>Defines.h</u>, edit the following section:

   ```c
   // E) Device ID
   byte device_id = 1;
   ```

4. Copy files from <u>libraries</u> folder to <u>%USERPROFILE%\Documents\Arduino\libraries</u>.

5. Using Arduino IDE, upload <u>esp32_beta_v2.ino</u> to ESP32 Dev Module.