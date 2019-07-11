const char Start = 0x61;
const char End = 0x62;
/*--------------------UPDATE TO CLIENT------------------------------*/
//char updateMode =1;
//char changeSpeed =2;
const int movingSpeed = 3;
const int chargingThreshold = 4;
const int spinnerSpeed = 5;
const int maxPower = 6;
const int minPower = 7;

/*--------------------UPDATE TO SERVER------------------------------*/
const int updateID = 29;
const int updateBattery = 30;
const int updateMachineStatus = 31;
const int updatePanel = 32;

/*--------------------UPDATE TO SLAVE------------------------------*/
const char IDCmd = 0x01;
const char DataCmd = 0x02;
const char WifiCmd = 0x03;

const char updateCurrentBattery = 0x11;
const char updateVoltageBattery = 0x12;
const char updateEnergyBattery = 0x13;

const char updateStatusParameter = 0x16;
const char updateDirectionParameter = 0x17;
const char updateLocationParameter = 0x18;

const char updateLocationPanel = 0x20;
const char updateStatusPanel = 0x21;
/*--------------------UPDATE TO MASTER------------------------------*/
const char AssignedIDCmd = 0x04;
const char DataRecCmd = 0x05;
const char Timeout = 0x06;
const char IDError = 0x07;

const char setFixedID = 0x40;
const char setMovingSpeed = 0x41;
const char setSpinnerSpeed =0x42;
const char setChargeThreshold = 0x43;
const char setMinPower = 0x44;
const char setMaxPower = 0x45;

const char setBuildMap = 0x50;
const char setFullMode = 0x51;
const char setCleanMode = 0x52;
const char setMeasureMode = 0x53;
const char setChargeMode = 0x54;
const char setStop = 0x55;
const char setContinue = 0x56;
