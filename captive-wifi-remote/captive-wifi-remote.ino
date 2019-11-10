#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

DNSServer dnsServer;
AsyncWebServer server(80);

bool nextFlag = false;
bool prevFlag = false;

#define defaultBrightness 25
#define minBrightness 0
#define maxBrightness 210

int currentBrightness = defaultBrightness;

String processor(const String& var){
  if(var == "MIN_BRIGHTNESS") return String(minBrightness);
  if(var == "MAX_BRIGHTNESS") return String(maxBrightness);
  if(var == "CURRENT_BRIGHTNESS") return String(currentBrightness);
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
    if (request->url() == "/styles.css"){
      request->send(SPIFFS, "/www/styles.css", "text/css");
    }
    else {
      if (request->url() == "/NEXT") {
          Serial.println("NEXT PRESSED");
          nextFlag = true;
      }
      else if (request->url() == "/PREVIOUS") {
          Serial.println("PREVIOUS PRESSED");
          prevFlag = true;
      }

      if(request->hasParam("brightness")){
        AsyncWebParameter* p = request->getParam("brightness");
        currentBrightness = p->value().toInt();
          Serial.print("New brightness: ");
          Serial.println(currentBrightness);
      }
      
      //Send index.htm with template processor function
      request->send(SPIFFS, "/www/index.htm", "text/html", false, processor);
    }
  }
};


void setup(){
  Serial.begin(115200);
  Serial.println();

  SPIFFS.begin();
  
  Serial.println("Configuring access point...");
  WiFi.softAP("test");
  
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler());
  //more handlers...
  server.begin();
  Serial.print("Server started. IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void loop(){
  dnsServer.processNextRequest();
}
