#ifndef simpleWebserver_h
#define simpleWebserver_h

// Load http server
#include <WebServer.h>
#include <ESPmDNS.h>
#include "src/webMagOD/webMagOD.h"

//extern webMagOD myweb;

extern WebServer server;
extern File uploadFile;

#endif
