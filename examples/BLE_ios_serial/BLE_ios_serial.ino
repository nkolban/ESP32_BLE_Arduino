/*
 * This example illustrates use of BLEDeviceID which makes the device
 * discoverable on iOS, which requires all Bluetooth devices to implement the
 * Device ID Profile:
 * https://developer.apple.com/hardwaredrivers/BluetoothDesignGuidelines.pdf
 *
 * It also shows usage of BLESerial which implements a Stream-compliant serial
 * protocol.
 *
 * This imitates a DFRobot Bluno BLE device which uses a simple serial protocol
 * for communication. BLESerial on ESP32 can be used equivalently with
 * Serial on Bluno BLE devices.
 *
 * Connect to "Bluno ESP" using LightBlue or another BLE test app.
 * The DFB1 characteristic in the DFB0 service is rewritten every
 * two seconds, and values written to this characteristc will be displayed
 * on the ESP's serial console.
 */

#include <BLEDevice.h>
#include <BLEDeviceID.h>
#include <BLESerial.h>
#include <BLEServer.h>

BLESerial bleSerial = BLESerial();

void setup() {
  Serial.begin(115200);

  BLEDevice::init("Bluno ESP");
  BLEServer *server = BLEDevice::createServer();

  bleSerial.begin(server, BLEUUID((uint16_t)0xDFB0), BLEUUID((uint16_t)0xDFB1));

  BLEDeviceID* did = new BLEDeviceID(
    server,
    "0x0BB9FD999969D238",
    "DF Bluno",
    "0123456789",
    "FW V1.97",
    "HW V1.7",
    "SW V1.97",
    "DFRobot",
    "",
    1,
    0x0010,
    0x0D00,
    0);
  did->start();
  server->getAdvertising()->start();
}

// This digit will be the first character of our message.
byte counter = 0;

void loop() {
  // The message we'll send to the client.
  // First byte will be an incrementing digit.
  std::string message = "  Hi!";
  // Convert counter digit to ASCII and put it in our message.
  message[0] = '0' + counter;
  counter = (counter + 1) % 10;
  bleSerial.write(message.c_str());

  if (bleSerial.available() > 0) {
    // Read up to 20 characters from the client, plus one for null-termination
    char buf[21];
    byte len = bleSerial.readBytesUntil('\0', buf, sizeof(buf) - 1);
    // Make sure the string is null-terminated.
    buf[len] = '\0';
    Serial.print("Got serial data: ");
    Serial.println(buf);
  } else {
    Serial.print(millis());
    Serial.println(" No serial data.");
  }
  delay(2000);
}
