/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <string>
#include "websocketconnection.h"

// assign functionpointer name of function executed on incoming data
void (*receiveDataPtr)(UInt8Ptr data, size_t length) = receiveDataCbk; 

// create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

char buffer[]= "00000000";

char* num2bin(uint32_t n) {
  int bin[8];
  int length = 0;
  
  for (int i=0; i<8; i++)
    buffer[i]= '0';

  while (n > 0) {
    ++length;
    bin[length] = n % 2;
    n = n / 2;
  }

  while (length > 0) {
    if (bin[length]==1) {
      buffer[8-length]= '1';
    } 
    else {
      buffer[8-length]= '0';
    }

    --length;
  }

  return buffer;
}

void sendData(uint8_t* data, size_t length) {
  if (isLog) {
    Serial.print("Data sent: ");
    for(int i=0; i<length; i++){
        Serial.print(num2bin(data[i]));
        Serial.print(' ');
    }
    Serial.println();
  }

  ws.binaryAll(data, length);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t length) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;

  if (isLog) {
    Serial.print("Data received: ");
    for(int i=0; i<length; i++){
        Serial.print(num2bin(data[i]));
        Serial.print(' ');
    }
    Serial.println();
  }

  if (info->final && info->index == 0 && 
     info->len == length && info->opcode == WS_BINARY) {
    // execute callback function
    receiveDataPtr(data, length);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t length) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, length);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void startConnection(const char* ssid, const char* password){
  // access point initialization (ESP32 as router)
  WiFi.softAP(ssid, password);

  // print ESP local ip address
  Serial.print("ESP32 local IP-address: ");
  Serial.println(WiFi.softAPIP());

  initWebSocket();

  // start server
  server.begin();
}