#include "CurieBle.h"

static const char* bluetoothDeviceName = "MyDevice";

static const char* serviceUuid = "180C"; 
static const char* characteristicUuid = "2A56";
static const int   characteristicTransmissionLength = 20;

char bleMessage[characteristicTransmissionLength];
char decryptedMessage[characteristicTransmissionLength];
unsigned int bleMessageLength;
const char* bleMessageCharacteristicUUID;
bool bleDataWritten = false; 

static const char* bleReadReceiptMessage = "ready\0"; // send this after write
static const int   bleReadReceiptMessageLength = 6;

BLEService service(serviceUuid);
BLECharacteristic characteristic(
  characteristicUuid,
  BLEWrite | BLERead | BLENotify,
  characteristicTransmissionLength
);

BLEPeripheral blePeripheral;

void encryptMessage(char* encryptedMessage, char* plainMessage, int messageLength) {
  for (int i=0; i < messageLength; i++) {
    if      (plainMessage[i] >= 'a' && plainMessage[i] <= 'm') encryptedMessage[i] = plainMessage[i] + 13;
    else if (plainMessage[i] >= 'A' && plainMessage[i] <= 'M') encryptedMessage[i] = plainMessage[i] + 13;
    else if (plainMessage[i] >= 'n' && plainMessage[i] <= 'z') encryptedMessage[i] = plainMessage[i] - 13;
    else if (plainMessage[i] >= 'N' && plainMessage[i] <= 'Z') encryptedMessage[i] = plainMessage[i] - 13;  
  }
  
}
void decryptMessage(char* plainMessage, char* encryptedMessage, int messageLength) {
  encryptMessage(plainMessage, encryptedMessage, messageLength);
}

void onBleCharacteristicWritten(BLECentral& central, 
  BLECharacteristic &characteristic) {
  bleDataWritten = true;
  
  bleMessageCharacteristicUUID = characteristic.uuid();
  bleMessageLength = characteristic.valueLength();
  
  strncpy(bleMessage, 
    (char*) characteristic.value(), characteristic.valueLength());
}

void sendBleReadReceipt() {
  // encrypt and send "ready" read receipt
  char encryptedMessage[characteristicTransmissionLength] = {0};
  encryptMessage(encryptedMessage, (char*)bleReadReceiptMessage, characteristicTransmissionLength);
  characteristic.setValue((const unsigned char*) encryptedMessage, characteristicTransmissionLength);
}


void setup() {
  Serial.begin(9600);
  while (!Serial) {;}
  blePeripheral.setLocalName(bluetoothDeviceName);
  
  blePeripheral.setAdvertisedServiceUuid(service.uuid());
  blePeripheral.addAttribute(service);
  blePeripheral.addAttribute(characteristic);

  characteristic.setEventHandler(
    BLEWritten,
    onBleCharacteristicWritten
  );

  Serial.println("Starting BLE Device");
  blePeripheral.begin();
}

void loop() {
  if (bleDataWritten) {
    bleDataWritten = false; // ensure only happens once
    
    Serial.print(bleMessageLength);
    Serial.print(" encrypted bytes sent to characteristic ");
    Serial.print(bleMessageCharacteristicUUID);
    Serial.print(": ");

    // decrypt message and print onscreen
    decryptMessage(decryptedMessage, bleMessage, characteristicTransmissionLength);
    Serial.println(decryptedMessage);
    
    // clear decrypted message for next time
    for (int i=0; i<characteristicTransmissionLength; i++) {
      decryptedMessage[i] = 0;
    }

    // send back read receipt
    Serial.println("Ready for more data");
    sendBleReadReceipt();

  }
}
