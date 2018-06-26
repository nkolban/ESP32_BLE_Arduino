/*
 BLEDeviceID.h - Bluetooth Low Energy Device ID Profile implementation.
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

#ifndef MAIN_BLEDeviceID_H_
#define MAIN_BLEDeviceID_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)
#include <inttypes.h>
#include <string>

#include "BLEService.h"

class BLEDeviceID {
public:
    // Specify server and all values needed to implement the Device ID spec:
    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.device_information.xml
    BLEDeviceID(
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
        uint16_t productVersion);
    void start();

protected:
    BLEService* service;
};

#endif // CONFIG_BT_ENABLED
#endif /* MAIN_BLEDeviceID_H_ */
