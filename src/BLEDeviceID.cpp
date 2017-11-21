/*
 BLEDeviceID.cpp - Bluetooth Low Energy Device ID Profile implementation.
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

#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)
#include <stdlib.h>

#include "BLEDeviceID.h"

BLEDeviceID::BLEDeviceID(
        BLEServer* server,
        std::string systemId,           // 0x2A23, 64 bits, use strtoull format "0x..."
        std::string modelNumber,        // 0x2A24
        std::string serialNumber,       // 0x2A25
        std::string firmwareRevision,   // 0x2A26
        std::string hardwareRevision,   // 0x2A27
        std::string softwareRevision,   // 0x2A28
        std::string manufacturerName,   // 0x2A29
        std::string ieee11073_20601,    // 0x2A2A, standard is behind paywall.
        // 0x2A50 PnP ID values:
        uint8_t vendorIdSource,         // 1=Bluetooth SIG, 2=USB Impl. Forum
        uint16_t vendorId,
        uint16_t productId,
        uint16_t productVersion) {
    // Need space for 20 handles to hold all the characteristics we have.
    service = server->createService(BLEUUID((uint16_t)0x180A), 20);

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.system_id.xml
    // Use strtoull to convert string to 64-bit value.
    // systemId should probably be of format "0x0123456789ABCDEF"
    unsigned long long systemIdLong = strtoull(systemId.c_str(), NULL, 0);
    // Store 64 bits as byte stream; string is convenient
    systemId = std::string((char*)&systemIdLong, sizeof(systemIdLong));
    // Need to reverse string to correct for bigendian
    systemId = std::string(systemId.rbegin(), systemId.rend());
    service->createCharacteristic(
        BLEUUID((uint16_t)0x2A23),
        BLECharacteristic::PROPERTY_READ)
            ->setValue(systemId);

    // Straightforward string values.
    service->createCharacteristic(
        BLEUUID((uint16_t)0x2A24),
        BLECharacteristic::PROPERTY_READ)
            ->setValue(modelNumber);
    service->createCharacteristic(
        BLEUUID((uint16_t)0x2A25),
        BLECharacteristic::PROPERTY_READ)
            ->setValue(serialNumber);
    service->createCharacteristic(
        BLEUUID((uint16_t)0x2A26),
        BLECharacteristic::PROPERTY_READ)
            ->setValue(firmwareRevision);
    service->createCharacteristic(
        BLEUUID((uint16_t)0x2A27),
        BLECharacteristic::PROPERTY_READ)
            ->setValue(hardwareRevision);
    service->createCharacteristic(
        BLEUUID((uint16_t)0x2A28),
        BLECharacteristic::PROPERTY_READ)
            ->setValue(softwareRevision);
    service->createCharacteristic(
        BLEUUID((uint16_t)0x2A29),
        BLECharacteristic::PROPERTY_READ)
            ->setValue(manufacturerName);
    // Format is mysterious.
    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.ieee_11073-20601_regulatory_certification_data_list.xml
    service->createCharacteristic(
        BLEUUID((uint16_t)0x2A2A),
        BLECharacteristic::PROPERTY_READ)
            ->setValue(ieee11073_20601);

    // Per spec, order these from least-significant octet to most.
    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.pnp_id.xml
    uint8_t val2a50[7];
    val2a50[6] = vendorIdSource;
    val2a50[5] = vendorId;
    val2a50[4] = vendorId >> 8;
    val2a50[3] = productId;
    val2a50[2] = productId >> 8;
    val2a50[1] = productVersion;
    val2a50[0] = productVersion >> 8;
    service->createCharacteristic(
        BLEUUID((uint16_t)0x2A50),
        BLECharacteristic::PROPERTY_READ)
            ->setValue(val2a50, sizeof(val2a50));
}

void BLEDeviceID::start() {
    service->start();
}

#endif // CONFIG_BT_ENABLED
