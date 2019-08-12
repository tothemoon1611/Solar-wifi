const char Start = 0x84;
const char End = 0x95;
/*--------------------SERVER UPDATE TO CLIENT------------------------------*/
//char updateMode =1;
const int typeFixedID = 2;
const int typemovingSpeed = 3;
const int typechargingThreshold = 4;
const int typespinnerSpeed = 5;
const int typeStop = 6;
const int typeContinue = 7;
const int typeControl = 8;
const int typeMode = 9;
const int typeIDError = 10;
const int typeHandshake = 20;

/*--------------------UPDATE TO SERVER------------------------------*/
const int typeupdateID = 29;
const int typeupdateBattery = 30;
const int typeupdateMachineStatus = 31;
const int typeupdatePanel = 32;
const int typeupdateError = 33 ;
/*--------------------UPDATE TO WIFI SLAVE ------------------------------*/
const char IDCmd = 0x01;
const char DataCmd = 0x02;
const char WifiCmd = 0x03;
const char WifiSSID = 0x0A;
const char WifiPass = 0x0B;
const char WifiIP = 0x0C;
const char WifiPort = 0x0D;
const char ACKSERVERCmd = 0x0E;
const char ACKNetworkCmd = 0x0F;

const char updateCurrentBattery = 0x11;
const char updateVoltageBattery = 0x12;
const char updateEnergyBattery = 0x13;

const char updateStatusParameter = 0x16;
const char updateDirectionParameter = 0x17;
const char updateCollumnPanelParameter = 0x18;
const char updateStringPanelParameter = 0x19;

const char updateLocationPanel = 0x20;
const char updateStatusPanel = 0x21;
/*--------------------UPDATE WIFI TO MASTER------------------------------*/
const char AssignedIDCmd = 0x04;
const char DataRecCmd = 0x05;
const char TimeoutError = 0x30;
const char IDError = 0x31;
const char NetworkError = 0x32;
const char ServerError = 0x33 ;

const char setFixedID = 0x40;
const char setMovingSpeed = 0x41;
const char setSpinnerSpeed =0x42;
const char setChargeThreshold = 0x43;
const char setMinPower = 0x44;
const char setMaxPower = 0x45;

const char setMode = 0x50;
const char setBuildMap = 0x51;
const char setCleanMode = 0x52;
const char setMeasureMode = 0x53;
const char setChargeMode = 0x54;
const char setStop = 0x55;
const char setContinue = 0x56;

/*--------------------UPDATE TO CAMERA SLAVE ------------------------------*/
const char CapImg = 0x08;
const char StreamCmd = 0x09;
