#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <string>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#include "SparkFunLSM6DSO.h"
#include "Wire.h"
//#include "SPI.h"

#include <iostream>
#include <sstream>
using namespace std;

LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B
BLECharacteristic *pCharacteristic;
 
void setup() {
 Serial.begin(115200);
 Serial.println("Starting BLE work!");
 
 BLEDevice::init("CS147");
 BLEServer *pServer = BLEDevice::createServer();
 BLEService *pService = pServer->createService(SERVICE_UUID);
 pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_WRITE
                                      );
 
 pCharacteristic->setValue("Counting starts!");
 pService->start();
 BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
 pAdvertising->addServiceUUID(SERVICE_UUID);
 pAdvertising->setScanResponse(true);
 pAdvertising->setMinPreferred(0x0); 
 pAdvertising->setMinPreferred(0x12);
 BLEDevice::startAdvertising();

int count = 0;
  
  Wire.begin();
  delay(10);
  if( myIMU.begin() )
    Serial.println("Ready.");
  else { 
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  if( myIMU.initialize(BASIC_SETTINGS) )
    Serial.println("Loaded Settings.");


  while (1) {
    Serial.print("\nAccelerometer:\n");
    Serial.print(" X = ");
    Serial.println(myIMU.readFloatAccelX(), 3);


    while (myIMU.readFloatAccelX() < 0.2) {
    }
    count++;
    char buffer[5];
    pCharacteristic->setValue(itoa(count, buffer, 10));
    Serial.println(count);
    delay(2000);
  }
  
}

void loop() {
  delay(500);
}