#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "command.h"
SoftwareSerial MasterSerial(D7, D8); // RX, TX

#define DEBUGER

/* Set Wifi Configuration */
const char *ssid = "FreeWifi";
const char *password = "freecaicc";
const char *ip = "192.168.43.167";
const uint16_t port = 9999;


unsigned long last_time;
unsigned long last_time_2;
int i = 0;
int battery[] = {0, 0, 0};
String InputString = "";
String DisplayString = "";
boolean StringComplete = false;
bool SerialRecv = false;
int serial_counter = 0;
char cmd;
int MovingSpeed = 0;
int ChargingThreshold = 0;
int SpinnerSpeed = 0;
int MaxPower = 0;
int MinPower = 0;
String ID = "";

char jsonBattery[] = "{\"Type\":30,\"Data\":\"{'current':%d,'voltage':%d,'energy':%d}\"}\r\n";
char jsonParameter[] = "{\"Type\":31,\"Data\":{\"status\":\"run\",\"direction\":\"forward\",\"location\":{\"X\":1,\"Y\":3}}}\r\n";
char jsonPanel[] = "{\"Type\":32,\"Data\":{\"location\":{\"X\":2,\"Y\":4},\"status\":\"fine\"}}\r\n";

/* event callbacks */
static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
#ifdef DEBUGER
  Serial.printf("\n data received from %s \n", client->remoteIP().toString().c_str());
#endif

  //  Serial.write((uint8_t*)data, len);
  const char *char_pointer = (char*)((uint8_t*)data);


#ifdef DEBUGER
  Serial.println(char_pointer);
#endif
  // Allocate the JsonDocument
  StaticJsonDocument<500> doc;

  // Parse the JSON input
  DeserializationError err = deserializeJson(doc, char_pointer);

  if (err) {
#ifdef DEBUGER
    Serial.println("parseObject() failed");
#endif

    return;
  } else {
    /*****************************************************/
    int type = doc["Type"];
    const char* data = doc["Data"];
    switch (type) {
      case movingSpeed:

        //Serial.println(String("0x01") + String(data));
        //MasterSerial.println(String("0x01") + String(data));
        break;

      case chargingThreshold:
        break;

      case spinnerSpeed:
        break;

      case maxPower:

        break;

      case minPower:
        break;

      default:
        Serial.println("Unknown receive cmd");
    }
    /*****************************************************/
    //const char* ID = doc["ID"];
    //Serial.println(ID);
    int current = doc["battery"]["current"];
    Serial.println(String("0x01") + String(current));
    MasterSerial.println(String("0x01") + String(current));

    int voltage = doc["battery"]["voltage"];
    Serial.println(String("0x02") + String(voltage));
    MasterSerial.println(String("0x02") + String(voltage));
    //MasterSerial.println(char_pointer);
  }
}

void onConnect(void* arg, AsyncClient* client) {
#ifdef DEBUGER
  Serial.printf("\n client has been connected to %s on port %d \n", ip, port);
#endif
  RegisterClient(client, ID);
}
void RegisterClient(void* arg, String IDReg) {
  //Get client ID;
  char clientID[] = "clientidtest";
  char connectJson[] = "{\"SocketClientType\":2,\"Code\":5,\"TargetID\":\"\",\"SendID\":\"%s\",\"CmdToken\":null,\"FirstParameter\":null,\"SecondParameter\":null}\r\n";
  AsyncClient* client = reinterpret_cast<AsyncClient*>(arg);
  if (client->space() > 300 && client->canSend()) {
    char message[300];
    sprintf(message, connectJson, IDReg.c_str());
    client->add(message, strlen(message));
    client->send();
  }
}

void SendClient(void* arg, int type) {
  char message[300];
  switch (type) {
    case updateBattery:
      sprintf(message, jsonBattery, battery[0], battery[1], battery[2]);
      break;
    default:
      Serial.println("Unknown send client cmd");
      return;
  }
  AsyncClient* client = reinterpret_cast<AsyncClient*>(arg);

  // send reply
  if (client->space() > 300 && client->canSend()) {
    client->add(message, strlen(message));
    client->send();
  }
}

AsyncClient* client = new AsyncClient;
void setup() {
  Serial.begin(9600);
  MasterSerial.begin(9600);
  delay(20);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  CheckWifi();
#ifdef DEBUGER
  Serial.println("Wifi Connected!");
#endif
  while (ID == "") {
    if ( (unsigned long) (millis() - last_time_2) > 2000)
    {
      MasterSerial.print(String(Start) + String(IDError) + String("No ID Installed!") + String(End));
      last_time_2 = millis();
    }
    Serial_ID();
  }
  client->onData(&handleData, client);
  client->onConnect(&onConnect, client);
  client->connect(ip, port);
}

void loop() {
  CheckWifi();
  Serial_Wifi();
  if ( (unsigned long) (millis() - last_time) > 2000)
  {
    battery[0] = random(9);
    battery[1] = random(10, 24);
    battery[2] = random(99);
    //replyData(client, json);
    SendClient(client, updateBattery);
    //Test
    Serial.println(String(Start) + String(setMovingSpeed) + String(battery[0]) + String(End));
    MasterSerial.print(String(Start) + String(setMovingSpeed) + String(battery[0]) + String(End));
    last_time = millis();
  }

}
