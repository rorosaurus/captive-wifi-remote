#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

DNSServer dnsServer;
AsyncWebServer server(80);

String processor(const String& var){
  if(var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
  return String();
}

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
    
//    AsyncResponseStream *response = request->beginResponseStream("text/html");
//    response->print("<!DOCTYPE html><html style='width: 100%; height: 100%'><head><title>Wifi Remote</title></head><body style='width: 100%; height: 100%'>");
//    response->print("<a href='/NEXT'><button style='width: 100%;height: 50%;font-size: 12vw;'>NEXT .gif</button></a>");
//    response->print("<a href='/PREVIOUS'><button style='width: 100%;height: 50%;font-size: 12vw;'>PREVIOUS .gif</button></a>");
//    response->print("</body></html>");
//    request->send(response);

    //Send index.htm with template processor function
    request->send(SPIFFS, "/www/index.htm", "text/html", false, processor);
//    request->send(SPIFFS, "/www/index.htm", "text/html");
  }
};


void setup(){
  Serial.begin(115200);
  Serial.println();

  SPIFFS.begin();
  
  Serial.println("Configuring access point...");
  
  //your other setup stuff...
  WiFi.softAP("test");
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler());//only when requested from AP
  //more handlers...
  server.begin();
  Serial.print("Server started. IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void loop(){
  dnsServer.processNextRequest();
}
