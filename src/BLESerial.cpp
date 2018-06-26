/*
 BLESerial.cpp - Bluetooth Low Energy Serial library.
 Copyright (c) 2017 by Grant Patterson.  All rights reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "BLESerial.h"

BLESerial::BLESerial() {
}

// From BLECharacteristicCallbacks
void BLESerial::onWrite(BLECharacteristic* characteristic) {
    buffer += characteristic->getValue();
}

void BLESerial::begin(BLEServer* server, BLEUUID service_uuid, BLEUUID characteristic_uuid)
{
    service = server->createService(service_uuid);
    buffer = "";
    characteristic = service->createCharacteristic(characteristic_uuid,
                                                   BLECharacteristic::PROPERTY_READ   |
                                                   BLECharacteristic::PROPERTY_WRITE  |
                                                   BLECharacteristic::PROPERTY_NOTIFY);
    BLEDescriptor* desc = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
    desc->setValue("Serial");
    characteristic->addDescriptor(desc);
    characteristic->setCallbacks(this);
    service->start();
}

void BLESerial::end() {
}

void BLESerial::setDebugOutput(bool en) {
}

int BLESerial::available(void) {
    return buffer.size();
}

int BLESerial::peek(void) {
    if (available()) {
        return (int)buffer[0];
    }
    return -1;
}

int BLESerial::read(void) {
    if (available()) {
        int result = (int)buffer[0];
        buffer.erase(0, 1);
        return result;
    }
    return -1;
}

void BLESerial::flush() {
    // Pretty sure this is supposed to go both ways in Arduino.
    buffer.clear();
}

size_t BLESerial::write(uint8_t c) {
    characteristic->setValue(&c, 1);
    characteristic->notify();
    return 1;
}

size_t BLESerial::write(const uint8_t *buffer, size_t size) {
    characteristic->setValue((uint8_t*)buffer, size);
    characteristic->notify();
    return size;
}

uint32_t  BLESerial::baudRate() {
    return 0;
}

BLESerial::operator bool() const {
    return true;
}
