/* ESP32_FTPServer_SD.ino */
/* Test ftp connection to SD card on MagOD, adapted from
   https://github.com/robo8080/ESP32_FTPServer_SD
This works with FileZilla. Settings:
In File/Site Manager add new site
General: 
  Protocol: FTP
  Host: check output serial port
  Encryption: Only use plain FTP
  Logon Type: Normal
  User: esp32
  Password: esp32
Transfer Settings:
  Passive
  Limit number of simultaneous connections to 1
Rest default.

Also works with gnu ftp on mac. Set to communication to passive.
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include "ESP32FtpServer.h"
#include <SD.h>
#include <SPI.h>

/* Make sure password.h lives in current directory with contents: */
/*
const char* ssid = "YourWifiNetwork SSID";
const char* password = "YourPassword";
*/

/* set #define FTP_DEBUG in ESP32FtpServer.h to see ftp verbose on serial */

#include "password.h"

FtpServer ftpSrv;   

//SD card options
int chipSelect = 15;

void setup(void){
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("");
  pinMode(chipSelect, INPUT_PULLUP); 

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /////FTP Setup, ensure SD is started before ftp;  /////////

  /* Initialize SD card */
  Serial.println("Initializing card ..");
  if (!SD.begin(chipSelect))
    {
      Serial.println("SD Card not found");
    }
  else
    {
      uint8_t cardType = SD.cardType();
	
      if(cardType == CARD_NONE){
	Serial.println("Card Type not recognized");
	return;
      }
      
      Serial.print("SD Card Type: ");
      if(cardType == CARD_MMC){
	Serial.println("MMC");
      } else if(cardType == CARD_SD){
	Serial.println("SDSC");
      } else if(cardType == CARD_SDHC){
	Serial.println("SDHC");
      } else {
	Serial.println("UNKNOWN");
      }
      
      uint64_t cardSize = SD.cardSize() / (1024 * 1024);
      Serial.printf("SD Card Size: %lluMB\n", cardSize);
      
      if( ! SD.exists("/")) {
	Serial.println( "root path '/' not found");
      }      
      
      ftpSrv.begin("esp32","esp32");    //username, password for ftp.  set ports in ESP32FtpServer.h  (default 21, 50009 for PASV)
     }
}

void loop(void){
  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!   
}
