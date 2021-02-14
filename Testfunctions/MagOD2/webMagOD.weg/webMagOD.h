/* webMagOD.h
 MagOD libary 
 December 2020
 Commands for webinterface on ESP32
 Rob Krawinkel, Leon Abelmann 
*/
#ifndef webMagOD_h
#define webMagOD_h

// Load Wi-Fi library
#include <WiFi.h>
#include <WiFiClient.h>
// Load SD card tools
#include <SD.h>
#include <SPI.h>

#include <WebServer.h>
#include <ESPmDNS.h>
#include <SD.h>

#include "../../MagOD.h"

//#warning "webMagOD.h loaded"

class webMagOD
{
 public:
  webMagOD();
  void returnOK();
  static void handleFileUpload();
  static void handleNotFound();
 private:
};

#endif
