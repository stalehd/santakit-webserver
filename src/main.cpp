#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncWebServer.h>
#include <IRRemote.hpp>

#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif
#ifndef WIFI_PASS
#define WIFI_PASS ""
#endif

#define IR_SEND_LED 14

const uint16_t address = 0;

const uint8_t onCommand = 0x45;
const uint8_t offCommand = 0x47;

const uint8_t partyMode1 = 0x44;
const uint8_t partyMode2 = 0x43;
const uint8_t partyMode3 = 0x7;
const uint8_t partyMode4 = 0x9;
const uint8_t partyMode5 = 0x16;
const uint8_t partyMode6 = 0xd;
const uint8_t partyMode7 = 0xc; 
const uint8_t partyMode8 = 0x5e;

const uint8_t repeats = 1;

AsyncWebServer webServer(80);


void treePartyMode(const int mode) {
    Serial.print("party mode");
    Serial.println(mode);

switch (mode) {
    case 1:
      IrSender.sendNEC(address, partyMode1, repeats);
      break;
    case 2:
      IrSender.sendNEC(address, partyMode2, repeats);
      break;
    case 3:
      IrSender.sendNEC(address, partyMode3, repeats);
      break;
    case 4:
      IrSender.sendNEC(address, partyMode4, repeats);
      break;
    case 5:
      IrSender.sendNEC(address, partyMode5, repeats);
      break;
    case 6:
      IrSender.sendNEC(address, partyMode6, repeats);
      break;
    case 7:
      IrSender.sendNEC(address, partyMode7, repeats);
      break;
    case 8:
      IrSender.sendNEC(address, partyMode8, repeats);
      break;
    default:
        Serial.print("Don't know how to mode ");
        Serial.println(mode);
    }
}

void sendPage(AsyncWebServerRequest *request) {
     AsyncResponseStream *response = request->beginResponseStream("text/html");
     response->print("<html>");
     response->print("<head>");
     response->print("<title>Santamote</title>");
     response->print("<style>body{font-size:5em;}</style>");
     response->print("</head>");
     response->print("<body>");
     response->print("<a href=/on>On</a></br>");
     response->print("<a href=/off>Off</a></br>");
     response->print("<a href=/party/1>Party mode 1</a></br>");
     response->print("<a href=/party/2>Party mode 2</a></br>");
     response->print("<a href=/party/3>Party mode 3</a></br>");
     response->print("<a href=/party/4>Party mode 4</a></br>");
     response->print("<a href=/party/5>Party mode 5</a></br>");
     response->print("<a href=/party/6>Party mode 6</a></br>");
     response->print("<a href=/party/7>Party mode 7</a></br>");
     response->print("<a href=/party/8>Party mode 8</a></br>");
     response->print("</body>");
     response->print("</html>");
     request->send(response);
}

int cmd = 0;

void sendCmd(int c) {
    cmd = c;
}

#define ON 100
#define OFF 200
#define PARTY1 11
#define PARTY2 12
#define PARTY3 13
#define PARTY4 14
#define PARTY5 15
#define PARTY6 16
#define PARTY7 17
#define PARTY8 18

const char *ssid = WIFI_SSID;
const char *pass = WIFI_PASS;

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Santamote");
    Serial.println();
    Serial.println("WiFi setup....");

    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi connect failed");
        return;
    }

    Serial.println("mDNS setup");
    MDNS.begin("santamote");
    MDNS.announce();
    
    Serial.println("IR setup");
    IrSender.begin(IR_SEND_LED, false, 0); 

    Serial.println("web server setup...");
    webServer.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Not found");
    });

    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        sendPage(request);
    });

    webServer.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
        sendCmd(ON);
        sendPage(request);
    });
    
    webServer.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
        sendCmd(OFF);
        sendPage(request);
    });
    
    webServer.on("/party/1", HTTP_GET, [](AsyncWebServerRequest *request){
        sendCmd(PARTY1);
        sendPage(request);
    });
    webServer.on("/party/2", HTTP_GET, [](AsyncWebServerRequest *request){
        sendCmd(PARTY2);
        sendPage(request);
    });
    webServer.on("/party/3", HTTP_GET, [](AsyncWebServerRequest *request){
        sendCmd(PARTY3);
        sendPage(request);
    });
    webServer.on("/party/4", HTTP_GET, [](AsyncWebServerRequest *request){
        sendCmd(PARTY4);
        sendPage(request);
    });
    webServer.on("/party/5", HTTP_GET, [](AsyncWebServerRequest *request){
        sendCmd(PARTY5);
        sendPage(request);
    });
    webServer.on("/party/6", HTTP_GET, [](AsyncWebServerRequest *request){
        sendCmd(PARTY6);
        sendPage(request);
    });
    webServer.on("/party/7", HTTP_GET, [](AsyncWebServerRequest *request){
        sendCmd(PARTY7);
        sendPage(request);
    });
    webServer.on("/party/8", HTTP_GET, [](AsyncWebServerRequest *request){
        sendCmd(PARTY8);
        sendPage(request);
    });

    Serial.println("web server @ http://santamote.local/");
    Serial.print("local IP:");
    Serial.println(WiFi.localIP().toString().c_str());
    webServer.begin();

}


void loop() {
    if (cmd > 0) {
        Serial.print("Cmd = ");
        Serial.println(cmd);
        switch (cmd) {
            case ON:
                IrSender.sendNEC(address, onCommand, repeats);
                break;
            case OFF:
                IrSender.sendNEC(address, offCommand, repeats);
                break;
            case PARTY1:
                IrSender.sendNEC(address, partyMode1, repeats);
                break;
            case PARTY2:
                IrSender.sendNEC(address, partyMode2, repeats);
                break;
            case PARTY3:
                IrSender.sendNEC(address, partyMode3, repeats);
                break;
            case PARTY4:
                IrSender.sendNEC(address, partyMode4, repeats);
                break;
            case PARTY5:
                IrSender.sendNEC(address, partyMode5, repeats);
                break;
            case PARTY6:
                IrSender.sendNEC(address, partyMode6, repeats);
                break;
            case PARTY7:
                IrSender.sendNEC(address, partyMode7, repeats);
                break;
            case PARTY8:
                IrSender.sendNEC(address, partyMode8, repeats);
                break;
        }
        cmd = 0;
    }
    // Process mDNS queries
    MDNS.update();

    delay(500);
}

