#ifndef _WEBSOCKETCONNECTION_H
#define _WEBSOCKETCONNECTION_H

// import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

typedef enum {TRUE=1, FALSE=0} BOOL;
typedef uint8_t* UInt8Ptr;

extern AsyncWebSocket ws;

extern const BOOL isLog;

void startConnection(const char* ssid, const char* password);

// receiving and sending data
void receiveDataCbk(UInt8Ptr data, size_t length);
void sendData(UInt8Ptr data, size_t length);

#endif // _WEBSOCKETCONNECTION_H