/****************************************************************************
**
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

// Better source https://code.qt.io/cgit/qt/qtconnectivity.git/tree/examples/bluetooth/lowenergyscanner?h=5.14
// https://code.qt.io/cgit/qt/qtconnectivity.git/tree/examples/bluetooth/btscanner?h=5.14
#ifndef DEVICE_H
#define DEVICE_H

#include <qbluetoothglobal.h> //Not needed??
#include <qbluetoothlocaldevice.h>
#include <QObject>
#include <QVariant>
#include <QList>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>
#include "deviceinfo.h"
#include "serviceinfo.h"
#include "characteristicinfo.h"


QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceInfo)

QT_USE_NAMESPACE

class Device : public QObject
{
    Q_OBJECT

    //Q_PROPERTY(QVariantList bluetoothDeviceList READ bluetoothDeviceList)
    Q_PROPERTY(QString btDevice READ btDevice WRITE setBtDevice NOTIFY btDeviceChanged)

public:
    Device(QObject *parent = 0);
    ~Device();
    Q_INVOKABLE void startScan();
    Q_INVOKABLE void itemActivated(QString item);
    Q_INVOKABLE void changeOwnDeviceName();
    Q_INVOKABLE void setServices();
    //QVariantList bluetoothDeviceList() {return m_bluetoothDeviceList;}
    QString btDevice(){return myBtDevice;}
    void setBtDevice(QString tee1){
      myBtDevice = tee1;
      btDeviceChanged(myBtDevice);
    }
signals:
    void btDeviceChanged(QString tee1);

public slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void on_power_clicked(bool clicked);
    void on_discoverable_clicked(bool clicked);
    //void displayPairingMenu(const QPoint &pos);
    //void pairingDone(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing);
private slots:
    //void startScan();
    void scanFinished();
    void setGeneralUnlimited(bool unlimited);
    //void itemActivated(QString item);
    void hostModeStateChanged(QBluetoothLocalDevice::HostMode);

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    //    Ui_DeviceDiscovery *ui;
     QVariantList m_bluetoothDeviceList;
     QString myBtDevice;
     QBluetoothServiceInfo *btService3;

};

#endif
