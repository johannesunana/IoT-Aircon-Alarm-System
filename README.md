## Instructions on how to use and deploy software to ESP32:

1. Setup Wi-Fi connectivity in Raspberry Pi.

   - Download [wpa_supplicant.conf](https://github.com/johannesunana/IoT-Aircon-Alarm-System/blob/main/wpa_supplicant.conf) and edit the following sections:
   ```ini
   ssid="WIFI_SSID" = "********";		# WiFI SSID
   psk="WIFI_PASSWORD" = "********";	# WiFi password
   ```
   to the Wi-Fi connection in use.
   
2. Save and copy the file to the root folder of microSD card containing the installed Raspberry PI OS.

3. Setup Wi-Fi connectivity in ESP32.

   - Download the latest commit **[here](https://github.com/johannesunana/IoT-Aircon-Alarm-System/archive/master.zip)**.

   - On <u>Credentials.h</u>, edit the following sections:

    ```c
    char server_addr[] = "********";	// IP address of server in local network
    ```
   ```c
   char ssid[] = "********";		// WiFI SSID
   char pass[] = "********";		// WiFi password
   
   char user[] = "********";		// MySQL server username
   char password[] = "********";	// MySQL server password
   char database[] = "********";	// MySQL database name
   ```

4. On <u>Defines.h</u>, edit the following section as needed:

   ```c
   // E) Device ID
   byte device_id = 1;		// device id of module
   ```

5. Download required libraries using Arduino IDE to <u>%USERPROFILE%\Documents\Arduino\libraries</u>.

6. Upload <u>esp32_beta_v2.ino</u> to ESP32 Dev Module.
