//{"Type":6,"Data":"20"}
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "command.h"
SoftwareSerial MasterSerial(D7, D8); // RX, TX

#define DEBUGER

/* Set Wifi Configuration */
// server CMD: {"Type":6,"Data":"20"}

const char *ssid = "FreeWifi";
const char *password = "freecaicc";
const char *ip = "192.168.43.167";
const uint16_t port = 9999;


unsigned long last_time;
unsigned long last_time_2;
unsigned long last_time_3;
unsigned long last_time_4;
unsigned long last_time_5;
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
bool ACK_ID = 0;
bool config_network=0;

char jsonBattery[] = "{\"Type\":30,\"Data\":\"{'current':%d,'voltage':%d,'energy':%d}\"}\r\n";
char jsonParameter[] = "{\"Type\":31,\"Data\":\"{'status':'%s','direction':'%s','panel':%d, 'collumn':%d}\"}\r\n";
char jsonPanel[] = "{\"Type\":32,\"Data\":\"{'panel':%d, 'collumn':%d,'status':'%s'}\"}\r\n";

/* event callbacks */
static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
#ifdef DEBUGER
  Serial.printf("\n data received from %s \n", client->remoteIP().toString().c_str());
#endif

  char *char_pointer = (char*)((uint8_t*)data);
  char_pointer[len - 1] = '\0';
#ifdef DEBUGER
  Serial.println(char_pointer);
#endif
  StaticJsonDocument<500> doc; // Allocate the JsonDocument
  DeserializationError err = deserializeJson(doc, char_pointer); // Parse the JSON input
  // char_pointer ="";

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
        Serial.print("Moving Speed: ");
        Serial.println(String(data));
        //MasterSerial.println(String("0x01") + String(data));
        break;
      case chargingThreshold:
        Serial.print("Charging Threshold: ");
        Serial.println(String(data));
        break;
      case spinnerSpeed:
        Serial.print("Spinner Speed: ");
        Serial.println(String(data));
        break;
      case maxPower:
        Serial.print("Max Power: ");
        Serial.println(String(data));
        break;
      case minPower:
#ifdef DEBUGER
        Serial.print("Min Power: ");
        Serial.println(String(data));
#endif
        break;
      case Handshake:
#ifdef DEBUGER
        ACK_ID = 1;
        Serial.print("ACK: ");
        Serial.println(String(data));
#endif
        break;
      default:
        Serial.println("Unknown receive cmd");
    }
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
    case updateMachineStatus:
      sprintf(message, jsonParameter, "run", "forward", 1, 2);
      break;
    case updatePanel:
      sprintf(message, jsonPanel, 3, 4, "fine");
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
  while (!config_network) {
    while (ID == "") {
      if ( (unsigned long) (millis() - last_time_2) > 2000)
      {
        MasterSerial.print(String(Start) + String(IDError) + String("No ID Installed!") + String(End));
        Serial.println(String(Start) + String(IDError) + String("No ID Installed!") + String(End));
        last_time_2 = millis();
      }
      Serial_ID();
    }
    client->onData(&handleData, client);
    client->onConnect(&onConnect, client);
    client->connect(ip, port);
    last_time_5 = millis();
    while (!ACK_ID) {
      delay(10);
      if ((millis() - last_time_5) > 4000) {
        Serial.println("Timeout!!!");
        break;
      }
    }
    if (ACK_ID && (millis() - last_time_5) < 4000) config_network = 1;
    else ID = "";
  }
  Serial.println("Socket Connected!!!");
  last_time_3 = millis();
}

void loop() {


  CheckWifi();
  if ( (unsigned long) (millis() - last_time_3) > 2000)
  {
    CheckSocket();
    last_time_3 = millis();
  }
  Serial_Wifi();
  if ( (unsigned long) (millis() - last_time) > 2000)
  {
    battery[2] = random(99);
    SendClient(client, updateBattery);
    SendClient(client, updateMachineStatus);
    // SendClient(client, updatePanel);
    //Test
    Serial.println(String(Start) + String(setMovingSpeed) + String(battery[0]) + String(End));
    UpdatetoMaster(String(setMovingSpeed), String(battery[0]));
    last_time = millis();
  }
  if ( (unsigned long) (millis() - last_time_4) > 3000)
  {
    SendClient(client, updateMachineStatus);
    last_time_4 = millis();
  }

}
