    
void CheckWifi()
{
  while (WiFi.status() != WL_CONNECTED)
  {
#ifdef DEBUGER
    Serial.println(".");
#endif
    delay(500);
    if ( RecheckWifi == 0)
    {
      RecheckWifi = 1 ;
      MasterSerial.print(String(Start) + String(NetworkError) + String("No Wifi Installed!") + String(End));
      Serial.println(String(Start) + String(NetworkError) + String("No Wifi Installed!") + String(End));
    }
    if (client->connected()) client->close(true);
  }
  if ( RecheckWifi == 1 )
  {
    RecheckWifi = 0 ;
    MasterSerial.print(String(Start) + String(NetworkOK) + String("Wifi Installed!") + String(End));
    Serial.println(String(Start) + String(NetworkOK) + String("Wifi Installed!") + String(End));
  }
}


void CheckSocket()                                          // cu 2s thi lap lai ham nay 1 lan
{ // toan them luc 7h58pm 21/8/19
  Serial.print("Check Socket: ");
  Serial.println(client->connected());
  if (!client->connected())
  {
    if ( RecheckSocket == 0) {
      MasterSerial.print(String(Start) + String(ServerError) + String("Connect Server Failed") + String(End));  // toan them luc 7h30pm 22/8/19
    }
    Serial.println("Connect Server Failed!");
    isAllowCheck = false;
    ServerTimeout = millis() ;
    RecheckSocket = 1 ;                         // toan them luc 7h58pm 21/8/19
    ReconnectSocket();
  }
  else {
    isReconnecting = false;
    if ( RecheckSocket == 1)
    {
      RecheckSocket = 0;   // toan them luc 7h58pm 21/8/19
      MasterSerial.print(String(Start) + String(ServerOK) + String("Connect Server OK") + String(End));
    }
    Serial.println("Connect Server Successed !");
  }
}

void StartCheckSocket()
{
  if (isAllowCheck) {
    CheckSocket();
  }
  else if (!isReconnecting) {
    ReconnectSocket();
  }
}
void ReconnectSocket()
{
  client->close();
  client = new AsyncClient;
  isReconnecting = true;
  Serial.println("Reconnecting socket !");
  client->onData(&handleData, client);
  client->onConnect(&onConnect, client);
  client->onError(&handleError, NULL);
  client->connect(ip.c_str(), port);
  
}
static void handleError(void* arg, AsyncClient* client, int8_t error) {
#ifdef DEBUGER
  Serial.printf("\n timeout...");
#endif
  isAllowCheck = true;
  isReconnecting = false;
}

void Serial_ID()
{
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

void Serial_Wifi()
{
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
        Status = InputString.toInt();
#ifdef DEBUGER
        Serial.print("Set Status Parameter: ");
        Serial.println(Status);
#endif
        break;
      case updateDirectionParameter:
        Direction = InputString.toInt();
#ifdef DEBUGER
        Serial.print("Set Direction Parameter: ");
        Serial.println(Direction);
#endif
        break;
      case updateStringPanelParameter:
        StrPanel = InputString.toInt();
#ifdef DEBUGER
        Serial.print("Set String: ");
        Serial.println(StrPanel);
#endif
        break;
      case updateCollumnPanelParameter:
        PanPos = InputString.toInt();
#ifdef DEBUGER
        Serial.print("Set Collumn: ");
        Serial.println(PanPos);
#endif
        break;
      case updateStatusPanel:
        PanelStatus = InputString.toInt();
#ifdef DEBUGER
        Serial.print("Set PanelStatus: ");
        Serial.println(PanelStatus);
#endif
        break;
      case updateStringPanelCamera:
        CAMStrPanel = InputString.toInt();
#ifdef DEBUGER
        Serial.print("Set Camera String: ");
        Serial.println(CAMStrPanel);
#endif
        break;
      case updateCollumnPanelCamera:
        CAMPanPos = InputString.toInt();
#ifdef DEBUGER
        Serial.print("Set Camera Collumn: ");
        Serial.println(CAMPanPos);
#endif
        break;
      default:
        Serial.println("Unknown cmd!!!");
    }
    InputString = "";
    StringComplete = false;
  }
}



void UpdatetoMaster(String Command, String data)
{
  MasterSerial.print(String(Start) + Command + data + String(End));
}
