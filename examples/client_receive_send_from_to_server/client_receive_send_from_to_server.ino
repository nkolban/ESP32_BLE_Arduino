#include <BLEDevice.h>

std::string rxValue;
String rxValueArduino ="";
static BLEUUID SERVICE_UUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEUUID    CHARACTERISTIC_UUID_RX("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEUUID    CHARACTERISTIC_UUID_TX("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean deviceConnected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;

BLERemoteService* pRemoteService;
//static void notifyCallback(
//BLERemoteCharacteristic* pBLERemoteCharacteristic,
//uint8_t* pData,
//size_t length,
//bool isNotify) {
//Serial.print("Notify callback for characteristic ");
//Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
//}

bool connectToServer(BLEAddress pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());
  BLEClient*  pClient  = BLEDevice::createClient();
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");
  pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(SERVICE_UUID.toString().c_str());
    return false;
  }

  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  //pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID_RX);
  //if (pRemoteCharacteristic == nullptr) {
  //Serial.print("Failed to find our characteristic UUID: ");
  //Serial.println(CHARACTERISTIC_UUID_RX.toString().c_str());
  //return false;
  //}
  //Serial.println(" - Found our characteristic");
  // Read the value of the characteristic.
  //pRemoteCharacteristic->registerForNotify(notifyCallback);
}

class MyAdvertisedDeviceCallbacks: 
public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(SERVICE_UUID)) {
      Serial.print("Found our device!  address: ");
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
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
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);

  Serial.begin(9600);
}

void loop()
{
  delay(20);
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      deviceConnected = true;
    } 
    else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
  if (deviceConnected) {
    pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID_RX);
    rxValue = pRemoteCharacteristic->readValue();
    rxValueArduino ="";
    for (int i = 0; i < rxValue.length(); i++) {
      rxValueArduino += rxValue[i];
    }
  }

  txStringArduino += String(	random( 80 , 90 )) +'|';
  txStringArduino += String(	random( 100 , 110 )) +'|';
  txStringArduino += String(	random( 110 , 120 )) +'|';
  txStringArduino += String(	random( 120 , 130 )) +'|';
  if (deviceConnected) {
    pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID_TX);
    pRemoteCharacteristic->writeValue(txStringArduino.c_str() ,txStringArduino.length() );
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

