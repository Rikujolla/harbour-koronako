/****************************************************************************
**
** Copyright (C) 2020 Riku Lahtinen
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "device.h"

#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QDebug>
#include <QList>
#include <QMetaEnum>
#include <QTimer>
#include <QDateTime>

Device::Device(QObject *parent)
    :   QObject(parent), localDevice(new QBluetoothLocalDevice)
{
    // devicename can be changed devel-su hciconfig hci0 name newname
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

    //connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
    //        this, SLOT(addDevice(QBluetoothDeviceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));

    connect(localDevice, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)),
            this, SLOT(hostModeStateChanged(QBluetoothLocalDevice::HostMode)));

    hostModeStateChanged(localDevice->hostMode());

    //timer.connect(&timer, SIGNAL(timeout()), this, SLOT(startScan()));
    //timer.setSingleShot(false);
    //timer.start(dTimer);

}

Device::~Device()
{
    delete discoveryAgent;
}

void Device::startScan()
{
    discoveryAgent->start();
}

void Device::scanFinished()
{
    for (int i=0 ; i < discoveryAgent->discoveredDevices().count();i++){
        QBluetoothDeviceInfo::MajorDeviceClass dev_cla =  discoveryAgent->discoveredDevices().at(i).majorDeviceClass();
        qint16 dev_rssi = discoveryAgent->discoveredDevices().at(i).rssi();
        int own_name_len = localDevice->name().length();
        QString dev_name = discoveryAgent->discoveredDevices().at(i).name();
        int dev_name_len = discoveryAgent->discoveredDevices().at(i).name().length();
        if (dev_cla == QBluetoothDeviceInfo::PhoneDevice){
            qDebug() << "Own/other device" << localDevice->address()<< localDevice->name() << discoveryAgent->discoveredDevices().at(i).address() << discoveryAgent->discoveredDevices().at(i).name()<< discoveryAgent->discoveredDevices().at(i).rssi();
            if (dev_rssi < 0 && dev_rssi > -80 && own_name_len > 6 && dev_name_len > 6 && !isInList(dev_name)){
                QString mybt = localDevice->name().right(7);
                uint mybtint;
                QString yourbt = discoveryAgent->discoveredDevices().at(i).name().right(7);
                uint yourbtint;
                QString sumbt;
                uint sumbtint;
                QString sumbttotal = QDateTime::currentDateTime().toString("dd") + ":";
                QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(discoveryAgent->discoveredDevices().at(i).address());
                if (pairingStatus == QBluetoothLocalDevice::Unpaired)
                {
                    for ( int j =0 ; j < 7; j = j + 1 ) {
                        mybtint = mybt.at(j).toLatin1();
                        yourbtint = yourbt.at(j).toLatin1();
                        sumbtint = (mybtint + yourbtint)/2;
                        // https://doc.qt.io/qt-5/qstring.html#arg
                        // https://forum.qt.io/topic/28890/convert-from-int-to-hex/8
                        sumbt = QString("%1").arg(sumbtint, 2, 16, QLatin1Char( '0' ));
                        if (sumbt.length() < 2) {sumbt = ":" + sumbt;}
                        sumbttotal = sumbttotal + sumbt ;
                    }
                    qDebug() << "Phone" << discoveryAgent->discoveredDevices().at(i).name() << dev_rssi << sumbttotal;
                    myBtDevice = sumbttotal;
                    btDeviceChanged(myBtDevice);
                }
            }
        }
    }
}

bool Device::setDiscoverable()
{
    if (localDevice->hostMode() != QBluetoothLocalDevice::HostDiscoverable) {
        //localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        myBtVisible = false;
        btVisibleChanged(myBtVisible);
        qDebug() << "Device not discoverable" << localDevice->hostMode();
        return false;
    }
    else
    {
        qDebug("Device was discoverable");
        return true;
    }
}

bool Device::getName()
{
    myOwnDevice = localDevice->name();
    ownDeviceChanged(myOwnDevice);
    if (myOwnDevice.length() < 7) {
        qDebug() << "Too short name" << myOwnDevice;
        return false;
    }
    else if (isInList(myOwnDevice)){
        qDebug() << "Phone name general" << myOwnDevice;
        return false;
    }
    else {
        qDebug() << "Phone name length and generality is ok" << myOwnDevice;
        return true;
    }

}

void Device::hostModeStateChanged(QBluetoothLocalDevice::HostMode mode)
{
}

bool Device::isInList(QString _phone)
{
    bool ret = false;
    // Phonelist edited from https://en.wikipedia.org/wiki/List_of_Android_smartphones
    QList<QString> phonelist = {
        "Asus PadFone",
        "Asus ROG Phone",
        "Asus ROG Phone II",
        "Asus ZenFone 2",
        "Asus ZenFone 3",
        "Asus ZenFone 4",
        "Asus ZenFone 4 (2014)",
        "Asus ZenFone 4.5 (2014)",
        "Asus ZenFone 5",
        "Asus ZenFone 5 (2014)",
        "Asus ZenFone 5 (2014)",
        "Asus ZenFone 5 (2015)",
        "Asus ZenFone 6",
        "Asus ZenFone 6 (2014)",
        "BlackBerry DTEK50",
        "BlackBerry DTEK60",
        "BlackBerry Key2",
        "BlackBerry KeyOne",
        "BlackBerry Motion",
        "BlackBerry Priv",
        "Essential PH-1",
        "Galaxy Nexus",
        "Honor 4X",
        "Honor 5X",
        "Honor 6",
        "Honor 7",
        "Honor 8",
        "Honor 8 Pro",
        "Honor 8A 2020",
        "Honor 8A Prime",
        "Honor 8S 2020",
        "Honor 8X",
        "Honor 9",
        "Honor 9C/9S/9A",
        "Honor 9X",
        "Honor 9X (China)",
        "Honor 9X Lite",
        "Honor 9X Pro",
        "Honor 20",
        "Honor 20 lite (China)",
        "Honor 20 Pro",
        "Honor 20e",
        "Honor 20S",
        "Honor 30 Lite",
        "Honor 30/30 Pro/30 Pro+",
        "Honor 30S",
        "Honor Play 3/3e",
        "Honor Play 4T/4T Pro",
        "Honor Play 9A",
        "Honor Play4/4 Pro",
        "Honor V30/30 Pro",
        "Honor X10",
        "Honor X10 Max",
        "HTC 10",
        "HTC Amaze 4G",
        "HTC Aria",
        "HTC Butterfly",
        "HTC ChaCha",
        "HTC Desire",
        "HTC Desire 12s",
        "HTC Desire 19+",
        "HTC Desire 19s",
        "HTC Desire 20 Pro",
        "HTC Desire 620",
        "HTC Desire Eye",
        "HTC Desire HD",
        "HTC Desire S",
        "HTC Desire X",
        "HTC Desire Z",
        "HTC Dream",
        "HTC Droid Incredible",
        "HTC Droid Incredible 4G LTE",
        "HTC Evo 3D",
        "HTC Evo 4G",
        "HTC Evo 4G LTE",
        "HTC Evo Design 4G",
        "HTC Evo Shift 4G",
        "HTC Exodus 1",
        "HTC Exodus 1s",
        "HTC Explorer",
        "HTC Hero",
        "HTC Incredible S",
        "HTC Inspire 4G",
        "HTC Legend",
        "HTC Magic",
        "HTC One (M7)",
        "HTC One (M8)",
        "HTC One M9",
        "HTC One Max",
        "HTC One Mini",
        "HTC One Mini 2",
        "HTC One S",
        "HTC One V",
        "HTC One X",
        "HTC Raider 4G",
        "HTC Rezound",
        "HTC Salsa",
        "HTC Sensation",
        "HTC Tattoo",
        "HTC ThunderBolt",
        "HTC U Ultra",
        "HTC U11",
        "HTC U11+",
        "HTC U12 life",
        "HTC U12+",
        "HTC U19e",
        "HTC U20 5G",
        "HTC Wildfire",
        "HTC Wildfire R70",
        "HTC Wildfire S",
        "HTC Wildfire X",
        "Huawei Ascend Mate",
        "Huawei Ascend Mate 2 4G",
        "Huawei Ascend Mate 7",
        "Huawei Ascend P1",
        "Huawei Ascend P2",
        "Huawei Ascend P6",
        "Huawei Ascend P7",
        "Huawei Mate 8",
        "Huawei Mate 9",
        "Huawei Mate 10",
        "Huawei Mate 20",
        "Huawei Mate 30",
        "Huawei Mate S",
        "Huawei P8",
        "Huawei P9",
        "Huawei P10",
        "Huawei P20",
        "Huawei P30",
        "Huawei P40",
        "LG G Flex",
        "LG G Flex 2",
        "LG G Pro 2",
        "LG G Pro Lite",
        "LG G2",
        "LG G2 Mini",
        "LG G3",
        "LG G3 Stylus",
        "LG G4",
        "LG G5",
        "LG G6",
        "LG G7 ThinQ",
        "LG G8 ThinQ",
        "LG GW620",
        "LG Gx",
        "LG K10 (2016)",
        "LG K10 (2017)",
        "LG K10 (2018)",
        "LG Optimus",
        "LG Optimus 2X",
        "LG Optimus 3D",
        "LG Optimus 4X HD",
        "LG Optimus Black",
        "LG Optimus Chic",
        "LG Optimus G",
        "LG Optimus L9",
        "LG Optimus LTE (LU6200)",
        "LG Optimus LTE (SU640)",
        "LG Optimus One",
        "LG Optimus Q",
        "LG Optimus Slider",
        "LG Optimus Vu",
        "LG V10",
        "LG V20",
        "LG V30",
        "LG V35 ThinQ",
        "LG V40 ThinQ",
        "LG V50 ThinQ",
        "LG V60 ThinQ",
        "LG Velvet",
        "Meizu 15/Pro/Lite",
        "Meizu 16s",
        "Meizu 16s Pro",
        "Meizu 16T",
        "Meizu 16th/Plus",
        "Meizu 16X",
        "Meizu 16Xs",
        "Meizu 17/Pro",
        "Meizu M1 Metal",
        "Meizu M1/1 Note",
        "Meizu M2",
        "Meizu M2 Note",
        "Meizu M3",
        "Meizu M3 Max",
        "Meizu M3 Note",
        "Meizu M3E",
        "Meizu M3s",
        "Meizu M3x",
        "Meizu M5",
        "Meizu M5 Note",
        "Meizu MX",
        "Meizu MX 4-core",
        "Meizu MX2",
        "Meizu MX3",
        "Meizu MX4",
        "Meizu MX4 Pro",
        "Meizu MX5",
        "Meizu MX5e",
        "Meizu MX6",
        "Meizu PRO 5",
        "Meizu PRO 6",
        "Meizu PRO 6 Plus",
        "Meizu PRO 6s",
        "Meizu PRO 7/Plus",
        "Meizu U10/20",
        "Moto C",
        "Moto E (1st generation)",
        "Moto E (2nd generation)",
        "Moto E3",
        "Moto E4",
        "Moto E5",
        "Moto G (1st generation)",
        "Moto G (2nd generation)",
        "Moto G (3rd generation)",
        "Moto G4",
        "Moto G5",
        "Moto G6",
        "Moto G7",
        "Moto G8",
        "Moto X (1st generation)",
        "Moto X (2nd generation)",
        "Moto X Play",
        "Moto X Style",
        "Moto X4",
        "Moto Z",
        "Moto Z Play",
        "Moto Z2 Force",
        "Moto Z2 Play",
        "Moto Z3",
        "Moto Z3 Play",
        "Moto Z4",
        "Motorola Atrix 2",
        "Motorola Atrix 4G",
        "Motorola Atrix HD",
        "Motorola Backflip",
        "Motorola Charm",
        "Motorola Cliq",
        "Motorola Defy",
        "Motorola Droid",
        "Motorola Droid 2",
        "Motorola Droid 3",
        "Motorola Droid 4",
        "Motorola Droid Bionic",
        "Motorola Droid Maxx",
        "Motorola Droid Mini",
        "Motorola Droid Pro",
        "Motorola Droid Razr",
        "Motorola Droid Razr HD",
        "Motorola Droid Razr M",
        "Motorola Droid Turbo",
        "Motorola Droid X",
        "Motorola Edge/Edge+",
        "Motorola Flipout",
        "Motorola One Action",
        "Motorola One Fusion+",
        "Motorola One Hyper",
        "Motorola One Macro",
        "Motorola One Vision",
        "Motorola One Zoom",
        "Motorola One/One Power",
        "Motorola Photon Q",
        "Nextbit Robin",
        "Nexus 4",
        "Nexus 5",
        "Nexus 5X",
        "Nexus 6",
        "Nexus 6P",
        "Nexus One",
        "Nexus S",
        "Nokia 1",
        "Nokia 1 Plus",
        "Nokia 1.3",
        "Nokia 2",
        "Nokia 2.1",
        "Nokia 2.2",
        "Nokia 2.3",
        "Nokia 3",
        "Nokia 3.1",
        "Nokia 3.1 Plus",
        "Nokia 3.2",
        "Nokia 4.2",
        "Nokia 5",
        "Nokia 5.1",
        "Nokia 5.1 Plus",
        "Nokia 5.3",
        "Nokia 6",
        "Nokia 6.1",
        "Nokia 6.1 Plus",
        "Nokia 6.2",
        "Nokia 7",
        "Nokia 7 Plus",
        "Nokia 7.1",
        "Nokia 7.2",
        "Nokia 8",
        "Nokia 8 Sirocco",
        "Nokia 8.1",
        "Nokia 8.3 5G",
        "Nokia 9 PureView",
        "Nokia C1",
        "Nokia C2",
        "Nokia X",
        "Nokia X2",
        "Nokia XL",
        "Nubia Red Magic",
        "Nubia Red Magic 3",
        "Nubia Red Magic 3s",
        "Nubia Red Magic 5G",
        "Nubia Red Magic Mars",
        "Nubia X",
        "Nubia Z20",
        "OnePlus 2",
        "OnePlus 3",
        "OnePlus 3T",
        "OnePlus 5",
        "OnePlus 5T",
        "OnePlus 6",
        "OnePlus 6T",
        "OnePlus 7",
        "OnePlus 7 Pro",
        "OnePlus 7T",
        "OnePlus 7T Pro",
        "OnePlus 8",
        "OnePlus 8 Pro",
        "OnePlus One",
        "OnePlus X",
        "Oppo A9 (2020)",
        "Oppo F1",
        "Oppo F7",
        "Oppo F9",
        "Oppo Find X",
        "Oppo Find X2/Pro",
        "Oppo N1",
        "Oppo R7",
        "Oppo R11",
        "Oppo R15 Pro",
        "Oppo R17",
        "Oppo Reno",
        "Oppo Reno2",
        "Panasonic P100",
        "Pixel 2/XL",
        "Pixel 3/XL",
        "Pixel 3a/XL",
        "Pixel 4/XL",
        "Pixel/XL",
        "Razer Phone",
        "Razer Phone 2",
        "Realme 1",
        "Realme 2",
        "Realme 2 Pro",
        "Realme 3",
        "Realme 3 Pro",
        "Realme 3i",
        "Realme 5",
        "Realme 5 Pro",
        "Realme 5i",
        "Realme 6",
        "Realme 6 Pro",
        "Realme 6i",
        "Realme C1",
        "Realme C2",
        "Realme C3",
        "Realme C3i",
        "Realme C11",
        "Realme Narzo",
        "Realme U1",
        "Realme X",
        "Realme X2",
        "Realme X2 Pro",
        "Realme X3",
        "Realme X3 SuperZoom",
        "Realme X50",
        "Realme X50 Pro",
        "Realme X50 Pro Player",
        "Realme XT",
        "Redmi 1",
        "Redmi 1S",
        "Redmi 2/Prime/Pro",
        "Redmi 2A",
        "Redmi 3",
        "Redmi 3 Pro",
        "Redmi 3S/3X",
        "Redmi 4/Pro/Prime",
        "Redmi 4A",
        "Redmi 4X",
        "Redmi 5/5 Plus",
        "Redmi 5A",
        "Redmi 6/6A/6 Pro",
        "Redmi 7",
        "Redmi 7A",
        "Redmi 8",
        "Redmi 8A",
        "Redmi 9",
        "Redmi 9A/9C",
        "Redmi 10X/Pro",
        "Redmi K20/Pro",
        "Redmi K30 Pro/Pro Zoom",
        "Redmi K30/5G",
        "Redmi Note (3G)",
        "Redmi Note 2",
        "Redmi Note 3/Pro",
        "Redmi Note 4/4X",
        "Redmi Note 4G",
        "Redmi Note 5",
        "Redmi Note 5 Pro",
        "Redmi Note 5A/Prime",
        "Redmi Note 6 Pro",
        "Redmi Note 7",
        "Redmi Note 7 Pro",
        "Redmi Note 8/Pro",
        "Redmi Note 9",
        "Redmi Note 9 Pro/9S",
        "Redmi Note Prime",
        "Redmi Pro",
        "Redmi S2/Y2",
        "Redmi Y3",
        "Samsung Galaxy",
        "Samsung Galaxy Grand Prime",
        "Samsung Galaxy Note",
        "Samsung Galaxy Note 2",
        "Samsung Galaxy Note 3",
        "Samsung Galaxy Note 3 Neo",
        "Samsung Galaxy Note 4",
        "Samsung Galaxy Note 5",
        "Samsung Galaxy Note 7",
        "Samsung Galaxy Note 8",
        "Samsung Galaxy Note 9",
        "Samsung Galaxy Note 10/+",
        "Samsung Galaxy Note Edge",
        "Samsung Galaxy Note FE",
        "Samsung Galaxy S1",
        "Samsung Galaxy S2",
        "Samsung Galaxy S3",
        "Samsung Galaxy S4",
        "Samsung Galaxy S4 Zoom",
        "Samsung Galaxy S5",
        "Samsung Galaxy S6 Edge+",
        "Samsung Galaxy S6/Edge",
        "Samsung Galaxy S7/Edge",
        "Samsung Galaxy S8/+",
        "Samsung Galaxy S9/+",
        "Samsung Galaxy S10 5G",
        "Samsung Galaxy S10/e/+",
        "Samsung Galaxy S20/+/Ultra",
        "Sony Ericsson Xperia Play",
        "Sony Ericsson Xperia pro",
        "Sony Ericsson Xperia X8",
        "Sony Ericsson Xperia X10",
        "Sony Xperia 1",
        "Sony Xperia 1 II",
        "Sony Xperia 5",
        "Sony Xperia 8",
        "Sony Xperia 10 II",
        "Sony Xperia 10/Plus",
        "Sony Xperia L1",
        "Sony Xperia L2",
        "Sony Xperia L3",
        "Sony Xperia L4",
        "Sony Xperia X Compact",
        "Sony Xperia X/Performance",
        "Sony Xperia XA",
        "Sony Xperia XA Ultra",
        "Sony Xperia XA1",
        "Sony Xperia XA1 Plus",
        "Sony Xperia XA1 Ultra",
        "Sony Xperia XA2 Plus",
        "Sony Xperia XA2/Ultra",
        "Sony Xperia XZ",
        "Sony Xperia XZ1/Compact",
        "Sony Xperia XZ2 Premium",
        "Sony Xperia XZ2/Compact",
        "Sony Xperia XZ3",
        "Sony Xperia XZs/Premium",
        "Sony Xperia Z",
        "Sony Xperia Z Ultra",
        "Sony Xperia Z1",
        "Sony Xperia Z1 Compact",
        "Sony Xperia Z2",
        "Sony Xperia Z3/Compact",
        "Sony Xperia Z4",
        "Sony Xperia Z5 Premium",
        "Sony Xperia Z5/Compact",
        "TCL 10L/10 Pro",
        "TCL Plex",
        "Vivo NEX",
        "Vivo NEX 3",
        "Vivo NEX 3S",
        "Vivo NEX Dual Display",
        "Vivo V9",
        "Vivo X7",
        "Vivo X30",
        "Vivo X50",
        "Xiaomi Black Shark",
        "Xiaomi Black Shark 2",
        "Xiaomi Black Shark 2 Pro",
        "Xiaomi Black Shark 3/Pro",
        "Xiaomi Black Shark Helo",
        "Xiaomi Mi 1",
        "Xiaomi Mi 2",
        "Xiaomi Mi 2S/A",
        "Xiaomi Mi 3/3 TD",
        "Xiaomi Mi 4",
        "Xiaomi Mi 4c",
        "Xiaomi Mi 4i",
        "Xiaomi Mi 4S",
        "Xiaomi Mi 5",
        "Xiaomi Mi 5c",
        "Xiaomi Mi 5s/5s Plus",
        "Xiaomi Mi 5X/Mi A1",
        "Xiaomi Mi 6",
        "Xiaomi Mi 6X/Mi A2",
        "Xiaomi Mi 8",
        "Xiaomi Mi 8 EE/8 Pro",
        "Xiaomi Mi 8 Lite",
        "Xiaomi Mi 8 SE",
        "Xiaomi Mi 9",
        "Xiaomi Mi 9 Lite",
        "Xiaomi Mi 9 Pro/Pro 5G",
        "Xiaomi Mi 9 SE",
        "Xiaomi Mi 9T",
        "Xiaomi Mi 9T Pro",
        "Xiaomi Mi 10 Lite",
        "Xiaomi Mi 10 Youth/Lite Zoom",
        "Xiaomi Mi 10/Pro",
        "Xiaomi Mi Max 2",
        "Xiaomi Mi Max 3",
        "Xiaomi Mi Max/Pro",
        "Xiaomi Mi MIX",
        "Xiaomi Mi MIX 2",
        "Xiaomi Mi MIX 2S",
        "Xiaomi Mi MIX 3",
        "Xiaomi Mi MIX 3 5G",
        "Xiaomi Mi Note",
        "Xiaomi Mi Note 2",
        "Xiaomi Mi Note 3",
        "Xiaomi Mi Note 10 Lite",
        "Xiaomi Mi Note 10/Pro",
        "Xiaomi Mi Note Pro",
        "Xiaomi Mi Play",
        "Xiaomi Pocophone F1"
    };
    for (int i=0 ; i < phonelist.size();i++){
        if (phonelist.at(i) == _phone){ ret = true;}
    }
    return ret;
}
