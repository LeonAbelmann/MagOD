
#include "simpleWebServer.h"

/* To connect to your wifi network, we expect file a 'password.h' in
local directory with contents: 
const char* ssid = "MyWifiNetwork"; //WiFi SSID 
const char* password = "PasswordForThatNetwork"; //WiFi Password
*/
#include "src/password.h" //expected in local directory

/* hostname that may appear on your network */
const char* host = "magod";

/* User settings : */
#define chipSelect 15 //SDcard CS port.

// Set web server port number to 80
WebServer server(80);
bool serverStarted = false;

webMagOD myweb;

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

File uploadFile;




void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  if (MDNS.begin(host)) {
    MDNS.addService("http", "tcp", 80);
    Serial.println("[WIFI] MDNS responder started");
    Serial.print("[WIFI] You can now connect to http://");
    Serial.print(host);
    Serial.println(".local");
  }

  server.on("/edit", HTTP_POST, []() {
      myweb.returnOK();
    }, myweb.handleFileUpload);
  
            
  server.onNotFound(myweb.handleNotFound);
  
  server.begin();
  Serial.println("[WIFI] HTTP server started");
  
  serverStarted = true;

  
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
      }
}

void loop(){
        server.handleClient();
	delay(10);
}

