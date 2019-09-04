// modified on 16h 12/8/2019
//{"Type":6,"Data":"20"}
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "command.h"
SoftwareSerial MasterSerial(D7, D8); // RX, TX

#define DEBUGER

/* Set Wifi Configuration */
// server CMD: {"Type":20,"Data":"20"}

String ssid = "";
String password = "";
String ip = "";


const uint16_t port = 9999;

unsigned long Timeout_ID = 4000;

unsigned long last_time;
unsigned long last_time_2;
unsigned long last_time_3;
unsigned long last_time_4;
unsigned long last_time_5;
unsigned long last_time_6;
unsigned long last_time_7;

int i = 0;

float battery[] = {0, 0, 0};
String InputString = "";
String DisplayString = "";
boolean StringComplete = false;
bool SerialRecv = false;
int serial_counter = 0;
char cmd;

int MovingSpeed = 0; //float
int ChargingThreshold = 0;
int SpinnerSpeed = 0;
int MaxPower = 0;
int MinPower = 0;

int ACK_ID = 0; //1byte
bool config_network = 0; //true fasle
int StrPanel = 1;
int PanPos = 1;
int CAMStrPanel = 1;
int CAMPanPos = 1;
int Status = 0;  //run stop
int Direction = 1; 
int PanelStatus = 1;
unsigned long ServerTimeout = millis() ;
unsigned long WifiTimeout ;

bool RecheckSocket = 0 ;
bool RecheckWifi = 0 ;

char jsonBattery[] = "{\"Type\":30,\"Data\":\"{'current':%.2f,'voltage':%.2f,'energy':%d}\"}\r\n";
char jsonParameter[] = "{\"Type\":31,\"Data\":\"{'status':'%d','direction':'%d','row':%d, 'collumn':%d}\"}\r\n";
char jsonPanel[] = "{\"Type\":32,\"Data\":\"{'row':%d, 'collumn':%d,'status':'%d'}\"}\r\n";

/* event callbacks */
static void handleData(void* arg, AsyncClient* client, void *data, size_t len) 
{
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
      case typemovingSpeed:
        Serial.print("Moving Speed: ");
        Serial.println(String(data));
        //MasterSerial.println(String("0x01") + String(data));
        break;
      case typechargingThreshold:
        Serial.print("Charging Threshold: ");
        Serial.println(String(data));
        break;
      case typespinnerSpeed:
        Serial.print("Spinner Speed: ");
        Serial.println(String(data));
        break;
      case typeIDError:
        Serial.print("ID Error: ");
        Serial.println(String(data));
        break;
      case typeFixedID:
        Serial.print("FixedID: ");
        Serial.println(String(data));
        UpdatetoMaster(String(setFixedID), String(data));
        break;
      case typeMode:
        if (String(data) == "7") {
          UpdatetoMaster(String(setStop), String(data));
#ifdef DEBUGER
          Serial.print("setStop: ");
          Serial.println(String(data));
#endif
        }
        else {
          if (String(data) == "8") {
            UpdatetoMaster(String(setContinue), String(data));
#ifdef DEBUGER
            Serial.print("setContinue: ");
            Serial.println(String(data));
#endif
          }
          else
          {
            UpdatetoMaster(String(setMode), String(data));
#ifdef DEBUGER
            Serial.print("setMode: ");
            Serial.println(String(data));
#endif
          }
        }

        break;
      case typeHandshake:
#ifdef DEBUGER
        ACK_ID = 1;
        Serial.print("ACK: ");
        Serial.println(String(data));
#endif
        UpdatetoMaster(String(ACKSERVERCmd), String(ACK_ID));
        break;
      case typeStop:
        UpdatetoMaster(String(setStop), String(data));
#ifdef DEBUGER
        Serial.print("Stop: ");
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
  Serial.printf("\n client has been connected to %s on port %d \n", ip.c_str(), port);
#endif
  
  RegisterClient(client, ID);
  isAllowCheck = true;
  isReconnecting = false;
}
void RegisterClient(void* arg, String IDReg) {

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
    case typeupdateBattery:
      sprintf(message, jsonBattery, battery[0], battery[1], 100);
      break;
    case typeupdateMachineStatus:
      sprintf(message, jsonParameter, Status, Direction, StrPanel, PanPos);
      break;
    case typeupdatePanel:
      sprintf(message, jsonPanel, CAMStrPanel, CAMPanPos, PanelStatus);
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
    while ((String(ssid) == "") || (String(password) == "") || (String(ip) == "") || (String(port) == "")) {
      if ( (unsigned long) (millis() - last_time_6) > 2000)
      {
        MasterSerial.print(String(Start) + String(IDError) + String("No Wifi Installed!") + String(End));
        Serial.println(String(Start) + String(IDError) + String("No Wifi Installed!") + String(End));
        last_time_6 = millis();
      }
      Serial_ID();
    }
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    CheckWifi();
#ifdef DEBUGER
    Serial.println("Wifi Connected!");
#endif
    bool isAllowCheck = true;
    bool isReconnecting = false;
    for (i = 0 ; i < 5; i++) {
      client->onData(&handleData, client);
      client->onConnect(&onConnect, client);
      client->connect(ip.c_str(), port);
      last_time_5 = millis();
      while (!ACK_ID) {
        delay(10);
        if ((millis() - last_time_5) > Timeout_ID) {
          Serial.println("Timeout!!!");
          Serial.println("Retry!!!");
          break;
        }
      }
      if (ACK_ID && (millis() - last_time_5) < Timeout_ID) {
        config_network = 1;
        UpdatetoMaster(String(ACKSERVERCmd), String("OK"));
        break;
      }
    }
    if (i == 5) {
      Serial.println("Error Send ID to Server!!!");
      ID = "";
      ACK_ID = 0;
    }
  }
  Serial.println("Socket Connected!!!");
  last_time_3 = millis();
  WifiTimeout = millis() ;
}


void loop() {
  CAMStrPanel = StrPanel;
  CAMPanPos = PanPos;
  CheckWifi();
  if ( (unsigned long) (millis() - last_time_3) > 2000)
  {
    StartCheckSocket();
    last_time_3 = millis();
  }
  Serial_Wifi();
  if ( (unsigned long) (millis() - last_time) > 500)
  {
    battery[2] = 100;
    SendClient(client, typeupdateBattery);

    last_time = millis();
  }
  if ( (unsigned long) (millis() - last_time_4) > 800)
  {
    SendClient(client, typeupdateMachineStatus);
    last_time_4 = millis();
  }
    if ( (unsigned long) (millis() - last_time_7) > 1300)
  {
    SendClient(client, typeupdatePanel);
    last_time_7 = millis();
  }
}
