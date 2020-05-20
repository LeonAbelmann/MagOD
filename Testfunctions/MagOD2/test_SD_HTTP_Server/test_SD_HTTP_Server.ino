/*
    Simulate the writing to the SD card on one core at 8Hz, measure the performance
    Have the WiFi accessible SD card on the other core, measure performance
    Run both simulatanuously and measure performance

    Designed and tested by Rob Kräwinkel 20-05-2020
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "SD.h"
#include "SPI.h"

#define measurementFrequency 8 // In hertz

bool doMeasurementFlag = false;

/*
    SD webserver
*/

const char* ssid = "****";                  //WiFi SSID
const char* password = "****";              //WiFi Password
const char* host = "esp32sd";

WebServer server(80);

bool serverStarted = false;

File uploadFile;

void returnOK() {
    server.send(200, "text/plain", "");
}

void returnFail(String msg) {
    server.send(500, "text/plain", msg + "\r\n");
    Serial.println(msg);
}

void loadFromSdCard(String path) {
    
    File requestedFile = SD.open((char *)path.c_str());
    if (requestedFile.isDirectory()) {
        printDirectory(requestedFile);
        requestedFile.close();
        return;
    }

    
    String dataType = "text/plain";
    if (path.endsWith("/")) {
        path += "index.htm";
    }

    if (path.endsWith(".src")) {
        path = path.substring(0, path.lastIndexOf("."));
    } else if (path.endsWith(".htm")) {
        dataType = "text/html";
    } else if (path.endsWith(".css")) {
        dataType = "text/css";
    } else if (path.endsWith(".js")) {
        dataType = "application/javascript";
    } else if (path.endsWith(".png")) {
        dataType = "image/png";
    } else if (path.endsWith(".gif")) {
        dataType = "image/gif";
    } else if (path.endsWith(".jpg")) {
        dataType = "image/jpeg";
    } else if (path.endsWith(".ico")) {
        dataType = "image/x-icon";
    } else if (path.endsWith(".xml")) {
        dataType = "text/xml";
    } else if (path.endsWith(".pdf")) {
        dataType = "application/pdf";
    } else if (path.endsWith(".zip")) {
        dataType = "application/zip";
    }

    if (server.hasArg("download") && server.arg("download") == "1") {
        dataType = "application/octet-stream";
    }

    if (server.streamFile(requestedFile, dataType) != requestedFile.size()) {
        Serial.println("Sent less data than expected!");
    }
}

void handleFileUpload() {
    if (server.uri() != "/edit") {
        return;
    }

    if (!server.hasArg("dir")) {
        return returnFail("BAD ARGS");
    }
    String path = server.arg("dir");

    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        if (SD.exists((char *)path.c_str())) {
            SD.remove((char *)path.c_str());
        }
        uploadFile = SD.open(path.c_str(), FILE_WRITE);
        Serial.print("Upload: START, filename: "); Serial.println(upload.filename);

    } else if (upload.status == UPLOAD_FILE_WRITE) {
        uploadFile.write(upload.buf, upload.currentSize);
        Serial.print("Upload: WRITE, Bytes: "); Serial.println(upload.currentSize);

    } else if (upload.status == UPLOAD_FILE_END) {
        uploadFile.close();
        Serial.print("Upload: END, Size: "); Serial.println(upload.totalSize);
    }
}

void handleDelete(String path) {
    deleteRecursive(path);
    
    path.remove(path.lastIndexOf("/"), path.length());
    server.sendHeader("Location", "http://esp32sd.local" + path, true);
    server.send(303, "text/plain", "");
}

void handleCreate(String path) {
    if (path.indexOf('.') > 0) {
        File file = SD.open((char *)path.c_str(), FILE_WRITE);
        if (file) {
            file.write(0);
            file.close();
        }
    } else {
        SD.mkdir((char *)path.c_str());
    }
    
    path.remove(path.lastIndexOf("/"), path.length());
    server.sendHeader("Location", "http://esp32sd.local" + path, true);
    server.send(303, "text/plain", "");
}

void printDirectory(File requestedFile) {
    
    requestedFile.rewindDirectory();
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/json", "");
    WiFiClient client = server.client();

    server.sendContent("[\r\n");
    for (int cnt = 0; true; ++cnt) {
        File entry = requestedFile.openNextFile();
        if (!entry) {
            break;
        }

        String output;
        output += "{Type: ";
        output += (entry.isDirectory()) ? "dir" : "file";
        output += ", Location: ";
        output += entry.name();
        output += "}\r\n";
        server.sendContent(output);
        entry.close();
    }
    server.sendContent("]");
    requestedFile.close();
}

void handleNotFound() {
    String path = server.uri();
    
    if (server.args() == 0 || (server.hasArg("download") && server.arg("download") == "1")) {       //http://esp32sd.local/folder or http://esp32sd.local/folder/file.txt or http://esp32sd.local/folder/file.txt?download=1
        if (path != "/" && !SD.exists((char *)path.c_str())) {
            return returnFail("BAD PATH");
        }
        loadFromSdCard(path);
        
    } else if (server.hasArg("delete") && server.arg("delete") == "1") {                            //http://esp32sd.local/folder?delete=1 or http://esp32sd.local/folder/file.txt?delete=1
        if (path == "/") {
            return returnFail("Cannot delete root folder");
        } else if (!SD.exists((char *)path.c_str())) {
            return returnFail("File does not exist");
        } 
        handleDelete(path);
        
    } else if (server.hasArg("create") && server.arg("create") == "1") {                            //http://esp32sd.local/folder?create=1 or http://esp32sd.local/folder/file.txt?create=1
        if (path == "/") {
            return returnFail("Cannot overwrite root folder");
        } else if (SD.exists((char *)path.c_str())) {
            return returnFail("File already exists");
        } 
        handleCreate(path);
        
    } else {
        return returnFail("BAD ARGS");
    }
}

/*
    SD file access
*/

void createDir(const char * path) {
    Serial.printf("[SD] Creating Dir: %s\n", path);
    if (SD.mkdir(path)) {
        Serial.println("[SD] Dir created");
    } else {
        Serial.println("[SD] mkdir failed");
    }
}

void deleteRecursive(String path) {
    File file = SD.open((char *)path.c_str());
    if (!file.isDirectory()) {
        file.close();
        SD.remove((char *)path.c_str());
        Serial.print("[SD] Deleted "); Serial.println(path);
        return;
    }

    file.rewindDirectory();
    while (true) {
        File entry = file.openNextFile();
        if (!entry) {
            break;
        }
        String entryPath = entry.name(); //path + "/" + entry.name();
        if (entry.isDirectory()) {
            entry.close();
            deleteRecursive(entryPath);
        } else {
            entry.close();
            SD.remove((char *)entryPath.c_str());
            Serial.print("[SD] Deleted "); Serial.println(entryPath);
        }
        
        
        yield();
    }

    SD.rmdir((char *)path.c_str());
    Serial.print("[SD] Deleted folder "); Serial.println(path);
    file.close();
}

/*
    Tasks
*/

void TaskWiFiSD( void *pvParameters );
void TaskKeepWiFiAlive( void *pvParameters );

TaskHandle_t TaskWiFiSDHandle;
TaskHandle_t TaskKeepWiFiAliveHandle;

void TaskWiFiSD(void *pvParameters) {
    (void) pvParameters;

    while (WiFi.status() != WL_CONNECTED){
        Serial.println("[SD Server] Waiting for WiFi connection...");
        vTaskDelay(pdMS_TO_TICKS(1000));
        yield();
    }
    while (serverStarted == false){
        Serial.println("[SD Server] Waiting for server initialization...");
        vTaskDelay(pdMS_TO_TICKS(1000));
        yield();
    }

    Serial.println("[SD Server] Server operational");

    for (;;) {
        //loop
        server.handleClient();
        vTaskDelay(pdMS_TO_TICKS(10));          //Some delay is neccessary to give the CPU time to tailor to other tasks and keep the watchdog in check
    }
}

void TaskKeepWiFiAlive(void *pvParameters){
    (void) pvParameters;
    
    for(;;){
        if(WiFi.status() == WL_CONNECTED){
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }

        Serial.println("[WIFI] Re-connecting");
        WiFi.begin(ssid, password);

        unsigned long startAttemptTime = millis();

        // Keep looping while we're not connected and haven't reached the timeout
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 20000){
            vTaskDelay(pdMS_TO_TICKS(10));
            yield();
        }

        // When we couldn't make a WiFi connection (or the timeout expired)
          // sleep for a while and then retry.
        if(WiFi.status() != WL_CONNECTED){
            Serial.println("[WIFI] FAILED");
            vTaskDelay(30000 / portTICK_PERIOD_MS);
            continue;
        }
        
        Serial.print("[WIFI] Connected: ");
        Serial.println(WiFi.localIP());
        
        if (serverStarted == false) {
            if (MDNS.begin(host)) {
                MDNS.addService("http", "tcp", 80);
                Serial.println("[WIFI] MDNS responder started");
                Serial.print("[WIFI] You can now connect to http://");
                Serial.print(host);
                Serial.println(".local");
            }
        
            server.on("/edit", HTTP_POST, []() {
                returnOK();
            }, handleFileUpload);
            
            server.onNotFound(handleNotFound);
        
            server.begin();
            Serial.println("[WIFI] HTTP server started");
            
            serverStarted = true;
        }
    }
}

uint32_t currentTime;
uint32_t writeTime = 0;

int fileNum = 0;
int lineNum = 0;
String fileName = "/measurementData/measurement_0.csv";  

void MagOD2Simulator() {
    currentTime = millis();

    if (lineNum > 999) {
        fileNum++;
        lineNum = 0;
        fileName = "/measurementData/measurement_" + (String)fileNum + ".csv";
    } 

    File dataFile = SD.open(fileName, FILE_APPEND);
    
    if (!dataFile) {
        Serial.print(fileName); Serial.println(" failed to open");
    } else {
        dataFile.print(writeTime); dataFile.print(",");
        
        for (int i=0; i<15; i++) {
            dataFile.print("12345,");
        }
        dataFile.println("12345");
        dataFile.close();
    }
    
    lineNum++;
    
    writeTime = millis() - currentTime;

    Serial.print("\t Write time: ");
    Serial.println(writeTime);
}

/* 
 *  See the IRAM_ATTR this makes sure the interrupt can actually run on while an SPI transfer occurs 
 *  in another task due to flash access being blocked by the SPI transfer this function should 
 *  always live in RAM
 */
void IRAM_ATTR measEvent() {        
    doMeasurementFlag = true;
}

void setup() {      //Runs on core 1
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.print("\n");
    
    WiFi.mode(WIFI_STA);

    while(!SD.begin(SS, SPI, 4000000, "/sd", 5)) {
        Serial.println("[SD] No SD card found");
        delay(3000);
    }
    
    Serial.println("[SD] SD Card initialized");

    deleteRecursive("/measurementData");    //Delete old data on the SD card
    createDir("/measurementData");          //Create folder for new data to be stored in

    // Setup a task to monitor the WiFi connection and to reconnect if it disconnects.
    xTaskCreatePinnedToCore(
        TaskKeepWiFiAlive
        ,  "keepWiFiAlive"  // A name just for humans
        ,  4096             // This stack size can be checked & adjusted by reading the uxTaskGetStackHighWaterMark();
        ,  NULL
        ,  2                // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,  &TaskKeepWiFiAliveHandle
        ,  0);              // Core
    
    delay(1000);

    // Now set up task to have the SD be WiFi accessible
    xTaskCreatePinnedToCore(
        TaskWiFiSD
        ,  "TaskWiFiSD"     // A name just for humans
        ,  4096             // This stack size can be checked & adjusted by reading the uxTaskGetStackHighWaterMark();
        ,  NULL
        ,  1                // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,  &TaskWiFiSDHandle
        ,  0);              // Core

    delay(1000);

    // Finally finalize setup for the MagOD2 simulator
    hw_timer_t * timer0 = NULL;                                         // Initialize a timer
    timer0 = timerBegin(0, 80, true);                                   // Attach the timer to hardware timer 0 with prescaler 80
    timerAttachInterrupt(timer0, measEvent, true);                      // Attach interrupt routine to the timer if it triggers
    timerAlarmWrite(timer0, 1000000/measurementFrequency, true);        // Set time between interrupts
    timerAlarmEnable(timer0);                                           // Enable timer
}

void loop() {   //Runs on core 1
    if (doMeasurementFlag == true) {
        doMeasurementFlag = false;
        MagOD2Simulator();
    }
}
