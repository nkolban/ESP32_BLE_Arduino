#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
std::string rxValue;
String rxValueArduino ="";
bool deviceConnected = false;
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
class MyServerCallbacks: 
public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };


  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};
class MyCallbacks: 
public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    rxValue = pCharacteristic->getValue();
    rxValueArduino ="";
    for (int i = 0; i < rxValue.length(); i++) {
      rxValueArduino += rxValue[i];
    }
  }
};
String txStringArduino ="";
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 
    0, -1   };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void setup()
{
  BLEDevice::init("ESP32");
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
  CHARACTERISTIC_UUID_TX,
  BLECharacteristic::PROPERTY_NOTIFY
    );

  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
  CHARACTERISTIC_UUID_RX,
  BLECharacteristic::PROPERTY_WRITE
    );
  pCharacteristic->setCallbacks(new MyCallbacks());
  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();
  Serial.println("Waiting a client connection to notify...");
  Serial.begin(9600);
}

void loop()
{
  txStringArduino += String(	random( 0 , 10 )) +'|';
  txStringArduino += String(	random( 20 , 30 )) +'|';
  txStringArduino += String(	random( 40 , 50 )) +'|';
  txStringArduino += String(	random( 60 , 70 )) +'|';
  if (deviceConnected) {
    uint8_t txLen = txStringArduino.length();
    char txString[ txLen ];
    for(uint8_t i=0; i<txLen; i++){
      txString[i] = txStringArduino[i];
    }
    pCharacteristic->setValue(txString);
    pCharacteristic->notify(); // Send the value to the app!
  }
  txStringArduino ="";
  Serial.print(getValue(rxValueArduino,'|', 0).toInt());
  Serial.print(" ");
  Serial.print(getValue(rxValueArduino,'|', 1).toInt());
  Serial.print(" ");
  Serial.print(getValue(rxValueArduino,'|', 2).toInt());
  Serial.print(" ");
  Serial.print(getValue(rxValueArduino,'|', 3).toInt());
  Serial.print(" ");
  Serial.println();
}

