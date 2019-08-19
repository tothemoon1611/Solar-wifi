void CheckWifi() {
  unsigned long WifiTimeout = millis() ;
  while (WiFi.status() != WL_CONNECTED) {
#ifdef DEBUGER
    Serial.println(".");
#endif
    delay(500);
    if ( (unsigned long) (millis() - WifiTimeout) > 7000)
      {
        MasterSerial.print(String(Start) + String(NetworkError) + String("Connect Server Failed") + String(End));
        Serial.println(String(Start) + String(NetworkError) + String("No Wifi Installed!") + String(End));
        WifiTimeout = millis() ;
        break ;
      }
  }
}

void CheckSocket() {
  if (!isAllowCheck){
    return;
  }
  Serial.print("Check Socket: ");
  Serial.println(client->connected());
  if (!client->connected()) {
   isAllowCheck = false;
   ReconnectSocket();
  } else {
    isReconnecting = false;
    isAllowCheck = true;
  }
}
void StartCheckSocket() {
  if (isAllowCheck) {
    CheckSocket();
  }
   else if (!isReconnecting) {
    ReconnectSocket();
   }
}
void ReconnectSocket() {
    isReconnecting = true; 
    client->onData(&handleData, client);
    client->onConnect(&onConnect, client);
    client->connect(ip.c_str(), port);
    isReconnecting = false;
}

void Serial_ID() {
  if (MasterSerial.available())
  {
    char inChar = (char)MasterSerial.read();
    if (inChar == Start) SerialRecv = true;
    if (inChar == End)
    {
      SerialRecv = false;
      serial_counter = 0;
      StringComplete = true;
    }
    if (SerialRecv)  serial_counter++;
    if (serial_counter == 2) cmd = inChar;
    if (serial_counter > 2) InputString += inChar;
  }
  if (StringComplete) {
    switch (cmd) {
      case IDCmd:
        ID = InputString;
        // RegisterClient(client, ID); /// dang ky ID moi
        Serial.print("ID: ");
        Serial.println(ID);
        break;
      case WifiSSID:
        ssid = InputString;
        Serial.print("ssid: ");
        Serial.println(ssid);
        break;
      case WifiPass:
        password = InputString;
        Serial.print("password: ");
        Serial.println(password);
        break;
      case WifiIP:
        ip = InputString;
        Serial.print("ip: ");
        Serial.println(ip);
        break;
      //      case WifiPort:
      //        port = atol(InputString.c_str());
      //        // RegisterClient(client, ID); /// dang ky ID moi
      //        Serial.print("port: ");
      //        Serial.println(port);
      //        break;
      //      case ACKIDCmd:
      //        ACK_ID = InputString.toInt();
      //        Serial.print("ACKID: ");
      //        Serial.println(ACK_ID);
      //        break;


    }
    InputString = "";
    StringComplete = false;
  }
}

void Serial_Wifi() {

  if (MasterSerial.available())
  {
    char inChar = (char)MasterSerial.read();
    if (inChar == Start) SerialRecv = true;
    if (inChar == End)
    {
      SerialRecv = false;
      serial_counter = 0;
      StringComplete = true;
    }
    if (SerialRecv)  serial_counter++;
    if (serial_counter == 2) cmd = inChar;
    if (serial_counter > 2) InputString += inChar;
  }
  if (StringComplete) {
    switch (cmd) {
      case IDCmd:
        ID = InputString;
#ifdef DEBUGER
        Serial.print("ID: ");
        Serial.println(ID);
#endif
        // RegisterClient(client, ID); /// dang ky ID moi
        break;
      case updateCurrentBattery:
        battery[0] = InputString.toFloat();
#ifdef DEBUGER
        Serial.print("Set Current Battery: ");
        Serial.println(battery[0]);
#endif
        break;
      case updateVoltageBattery:
        battery[1] = InputString.toFloat();
#ifdef DEBUGER
        Serial.print("Set Voltage Battery: ");
        Serial.println(battery[1]);
#endif
        break;
      case updateEnergyBattery:
#ifdef DEBUGER
        Serial.print("Set Energy Battery: ");
        Serial.println(MaxPower);
#endif
        break;
      case updateStatusParameter:
#ifdef DEBUGER
        Serial.print("Set Status Parameter: ");
        Serial.println(MinPower);
#endif
        break;
      case updateDirectionParameter:
#ifdef DEBUGER
        Serial.print("Set Direction Parameter: ");
        Serial.println(MinPower);
#endif
        break;
      case updateCollumnPanelParameter:
#ifdef DEBUGER
        Serial.print("Set Location Parameter: ");
        Serial.println(MinPower);
#endif
        break;
      case updateLocationPanel:
#ifdef DEBUGER
        Serial.print("Set Location Panel: ");
        Serial.println(MinPower);
#endif
        break;
      case updateStatusPanel:
#ifdef DEBUGER
        Serial.print("Set Status Panel: ");
        Serial.println(MinPower);
#endif
        break;
      default:
        Serial.println("Unknown cmd!!!");
    }
    InputString = "";
    StringComplete = false;
  }
}

void UpdatetoMaster(String Command, String data) {
  MasterSerial.print(String(Start) + Command + data + String(End));
}
