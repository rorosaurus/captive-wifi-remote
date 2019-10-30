#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"

DNSServer dnsServer;
AsyncWebServer server(80);

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    if (request->url() == "/NEXT") {
        Serial.println("NEXT PRESSED");
    }
    else if (request->url() == "/PREVIOUS") {
        Serial.println("PREVIOUS PRESSED");
    }
    
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->print("<!DOCTYPE html><html><head><title>Captive Portal</title></head><body>");
    response->printf("<a href='/NEXT'><button style='width: 100%;height: 50%;font-size: 10vw;'>NEXT .gif</button></a><br>");
    response->printf("<a href='/PREVIOUS'><button style='width: 100%;height: 50%;font-size: 10vw;'>PREVIOUS .gif</button></a><br>");
    response->print("</body></html>");
    request->send(response);
  }
};


void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");
  
  //your other setup stuff...
  WiFi.softAP("myLEDPanel");
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP
  //more handlers...
  server.begin();
}

void loop(){
  dnsServer.processNextRequest();
}
