# SD HTTP WebServer
Rob Kräwinkel, 20 May 2020


Program to try out an implementation of a HTTP webserver on the second core of the ESP32 microcontroller


Files and folders can be read, downloaded, created and deleted from the SD card through a internet browser using simple commands:

To read or download: http://esp32sd.local/folder or http://esp32sd.local/folder/file.txt or http://esp32sd.local/folder/file.txt?download=1

To delete: http://esp32sd.local/folder?delete=1 or http://esp32sd.local/folder/file.txt?delete=1

To create: http://esp32sd.local/folder?create=1 or http://esp32sd.local/folder/file.txt?create=1


Uploading a file is a bit harder as the internet browser can only send HTTP GET commands and not HTTP PUSH commands which are required for file uploading. The program curl can be used for this fairly simply (python could be used as well)
To upload: curl -F "data=@fileToUpload.txt" -X POST http://esp32sd.local/edit?dir=/folder/fileToUpload.txt
