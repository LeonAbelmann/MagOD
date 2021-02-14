/* webMagOD.cpp
 MagOD libary 
 December 2020
 Commands for webinterface on ESP32
 Rob Krawinkel, Leon Abelmann 
*/

#include "webMagOD.h"

//Constructor
webMagOD::webMagOD(){
};


/*   SD file access */

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


/*   Web access */

void webMagOD::returnOK() {
    server.send(200, "text/plain", "");
}

void returnFail(String msg) {
    server.send(500, "text/plain", msg + "\r\n");
    Serial.println(msg);
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

void webMagOD::handleFileUpload() {
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


void webMagOD::handleNotFound() {
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
