/*
 BLESerial.h - Bluetooth Low Energy Serial library.
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

#ifndef BLESerial_h
#define BLESerial_h

#include <inttypes.h>
#include <string>

#include "BLECharacteristic.h"
#include "BLEService.h"
#include "Stream.h"

class BLESerial: public Stream, public BLECharacteristicCallbacks {
public:
    BLESerial();

    void begin(BLEServer* server, BLEUUID service_uuid, BLEUUID characteristic_uuid);
    void end();
    int available(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);

    inline size_t write(const char * s) {
        return write((uint8_t*) s, strlen(s));
    }
    inline size_t write(unsigned long n) {
        return write((uint8_t) n);
    }
    inline size_t write(long n) {
        return write((uint8_t) n);
    }
    inline size_t write(unsigned int n) {
        return write((uint8_t) n);
    }
    inline size_t write(int n) {
        return write((uint8_t) n);
    }
    uint32_t baudRate();
    operator bool() const;

    void setDebugOutput(bool);

    void onWrite(BLECharacteristic* characteristic);

protected:
    BLEService* service;
    BLECharacteristic* characteristic;
    std::string buffer;
};

#endif
