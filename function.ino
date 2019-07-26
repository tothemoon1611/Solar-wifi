void CheckWifi() {
  while (WiFi.status() != WL_CONNECTED) {
#ifdef DEBUGER
    Serial.println("Wifi disconnected!!!");
#endif
    delay(500);
  }
}
void CheckSocket() {
  Serial.print("Check Socket: ");
  Serial.println(client->connected());
  if (!client->connected()) {
    client->onData(&handleData, client);
    client->onConnect(&onConnect, client);
    client->connect(ip, port);
  }
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
        battery[0] = InputString.toInt();
#ifdef DEBUGER
        Serial.print("Set Current Battery: ");
        Serial.println(battery[0]);
#endif
        break;
      case updateVoltageBattery:
        battery[1] = InputString.toInt();
#ifdef DEBUGER
        Serial.print("Set Voltage Battery: ");
        Serial.println(battery[1]);
#endif
        break;
      case updateEnergyBattery:
#ifdef DEBUGER
        Serial.print("Set Moving Speed: ");
        Serial.println(MaxPower);
#endif
        break;
      case updateStatusParameter:
#ifdef DEBUGER
        Serial.print("Set Moving Speed: ");
        Serial.println(MinPower);
#endif
        break;
      case updateDirectionParameter:
#ifdef DEBUGER
        Serial.print("Set Moving Speed: ");
        Serial.println(MinPower);
#endif
        break;
      case updateLocationParameter:
#ifdef DEBUGER
        Serial.print("Set Moving Speed: ");
        Serial.println(MinPower);
#endif
        break;
      case updateLocationPanel:
#ifdef DEBUGER
        Serial.print("Set Moving Speed: ");
        Serial.println(MinPower);
#endif
        break;
      case updateStatusPanel:
#ifdef DEBUGER
        Serial.print("Set Moving Speed: ");
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
