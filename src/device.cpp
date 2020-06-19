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
    //qDebug() << "Finished" << discoveryAgent->discoveredDevices().count() ;
    for (int i=0 ; i < discoveryAgent->discoveredDevices().count();i++){
        //qDebug() << "Device" << discoveryAgent->discoveredDevices().at(i).address() << discoveryAgent->discoveredDevices().at(i).rssi() << discoveryAgent->discoveredDevices().at(i).majorDeviceClass();
        QBluetoothDeviceInfo::MajorDeviceClass dev_cla =  discoveryAgent->discoveredDevices().at(i).majorDeviceClass();
        if (dev_cla == QBluetoothDeviceInfo::PhoneDevice || dev_cla == QBluetoothDeviceInfo::MiscellaneousDevice || dev_cla == QBluetoothDeviceInfo::UncategorizedDevice){
            if (discoveryAgent->discoveredDevices().at(i).rssi() < 0 && discoveryAgent->discoveredDevices().at(i).rssi() > -80){
                QString mybt;
                uint mybtint;
                QString yourbt;
                uint yourbtint;
                QString sumbt = QDateTime::currentDateTime().toString("dd") +":";
                uint sumbtint;
                QString sumbttotal = QDateTime::currentDateTime().toString("dd");
                QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(discoveryAgent->discoveredDevices().at(i).address());
                if (pairingStatus == QBluetoothLocalDevice::Unpaired)
                {
                    for ( int j =3 ; j < 16; j = j + 3 ) {
                        mybt = localDevice->address().toString().mid(j,1);
                        // https://forum.qt.io/topic/31737/solved-convert-ascii-hex-to-int
                        bool bStatus = false;
                        mybtint = mybt.toUInt(&bStatus,16);
                        yourbt = QString("%1").arg(discoveryAgent->discoveredDevices().at(i).address().toString()).mid(j,1);
                        yourbtint = yourbt.toUInt(&bStatus,16);
                        sumbtint = mybtint + yourbtint;
                        // https://doc.qt.io/qt-5/qstring.html#arg
                        // https://forum.qt.io/topic/28890/convert-from-int-to-hex/8
                        sumbt = QString("%1").arg(sumbtint, 2, 16, QLatin1Char( '0' ));
                        sumbttotal = sumbttotal + ":" + sumbt ;
                    }

                    //qDebug() << "Phone" << discoveryAgent->discoveredDevices().at(i).address() << discoveryAgent->discoveredDevices().at(i).rssi() << sumbttotal;
                    myBtDevice = sumbttotal;
                    btDeviceChanged(myBtDevice);
                }

            }
        }
    }
}

void Device::setGeneralUnlimited(bool unlimited)
{
    if (unlimited)
        discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);
    else
        discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::LimitedInquiry);
}

void Device::on_discoverable_clicked(bool clicked)
{
    if (clicked)
        localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    else
        localDevice->setHostMode(QBluetoothLocalDevice::HostConnectable);
}

void Device::hostModeStateChanged(QBluetoothLocalDevice::HostMode mode)
{
    if (mode != QBluetoothLocalDevice::HostPoweredOff)
        ;
    else
        //timer.stop();

        if (mode == QBluetoothLocalDevice::HostDiscoverable)
            //ui->discoverable->setChecked(true)
            ;
        else
            //ui->discoverable->setChecked(false);

            bool on = !(mode == QBluetoothLocalDevice::HostPoweredOff);

}
