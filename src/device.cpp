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
#include "service.h" //not needed later??
#include "serviceinfo.h" //not needed later??

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
    qDebug() << "devicename" << localDevice->name();
    qDebug() << "deviceaddress" << localDevice->address();
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(addDevice(QBluetoothDeviceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));

    connect(localDevice, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)),
            this, SLOT(hostModeStateChanged(QBluetoothLocalDevice::HostMode)));

    hostModeStateChanged(localDevice->hostMode());
}

Device::~Device()
{
    delete discoveryAgent;
    btService3->unregisterService();
    delete btService3;
}

void Device::addDevice(const QBluetoothDeviceInfo &info)
{
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
    //QString mybt = localDevice->address().toString();
    QString mybt;
    uint mybtint;
    //QString yourbt = QString("%1").arg(info.address().toString());
    QString yourbt;
    uint yourbtint;
    QString sumbt = QDateTime::currentDateTime().toString("dd") +":";
    uint sumbtint;
    // https://www.cplusplus.com/reference/ctime/tm/
    //time_t now = time(0);
    //tm *ltm = localtime(&now);
    QString sumbttotal = QDateTime::currentDateTime().toString("dd");
    //QList<QString *> items;
    //if (items.empty()) {
    QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
    if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
    {
        //myBtDevice = label;
        //qDebug() << "Paired " << label ;
        //btDeviceChanged(myBtDevice);
    }
    else
    {
        qDebug() << "Unpaired " << localDevice->address() << info.address() ;
        for ( int i =3 ; i < 16; i = i + 3 ) {
           mybt = localDevice->address().toString().mid(i,1);
           bool bStatus = false;
           mybtint = mybt.toUInt(&bStatus,16);
           yourbt = QString("%1").arg(info.address().toString()).mid(i,1);
           yourbtint = yourbt.toUInt(&bStatus,16);
           sumbtint = mybtint + yourbtint;
           // https://doc.qt.io/qt-5/qstring.html#arg
           QString sumbt = QString("%1").arg(sumbtint, 2, 16, QLatin1Char( '0' ));
           sumbttotal = sumbttotal + ":" + sumbt ;
           //qDebug() << mybtint << yourbtint << sumbtint << sumbttotal;
        }
        // https://forum.qt.io/topic/31737/solved-convert-ascii-hex-to-int
        QString sValue = "F";
        bool bStatus = false;
        uint nHex = sValue.toUInt(&bStatus,16);
        // https://forum.qt.io/topic/28890/convert-from-int-to-hex/8
        uint decimal = 14;
        QString hexvalue = QString("%1").arg(decimal, 8, 16, QLatin1Char( '0' ));

        //QString hexvalue = QString("0x%1").arg(decimal, 8, 16, QLatin1Char( '0' ));
        //qDebug() << nHex << hexvalue;
        qDebug() << "Unpaired short" << sumbttotal;
        myBtDevice = sumbttotal;
        btDeviceChanged(myBtDevice);
        //emit btDeviceChanged; //Have to study later if signal could be coded more simple way
    }
    // }

}

void Device::startScan()
{
    discoveryAgent->start();
    qDebug() << "Start " ;
}

void Device::scanFinished()
{
    qDebug() << "Finished , yes" ;
    //ui->scan->setEnabled(true);
    //ui->inquiryType->setEnabled(true);
}

void Device::setGeneralUnlimited(bool unlimited)
{
    if (unlimited)
        discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);
    else
        discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::LimitedInquiry);
}

void Device::itemActivated(QString item)
{
    qDebug() << "sss" << item;
    //QString text = item->text();

    int index = item.indexOf(' ');

    if (index == -1)
        return;
    qDebug() << "ssssseee " << item.left(index);

    QBluetoothAddress address(item.left(index));
    QString name(item.mid(index + 1));

    ServiceDiscoveryDialog d(name, address);
    //d.exec();
}

void Device::on_discoverable_clicked(bool clicked)
{
    if (clicked)
        localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    else
        localDevice->setHostMode(QBluetoothLocalDevice::HostConnectable);
}

void Device::on_power_clicked(bool clicked)
{
    if (clicked)
        localDevice->powerOn();
    else
        localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}

void Device::hostModeStateChanged(QBluetoothLocalDevice::HostMode mode)
{
    if (mode != QBluetoothLocalDevice::HostPoweredOff)
        //ui->power->setChecked(true)
        ;
    else
        //ui->power->setChecked( false);

        if (mode == QBluetoothLocalDevice::HostDiscoverable)
            //ui->discoverable->setChecked(true)
            ;
        else
            //ui->discoverable->setChecked(false);

            bool on = !(mode == QBluetoothLocalDevice::HostPoweredOff);

    //ui->scan->setEnabled(on);
    //ui->discoverable->setEnabled(on);
}
/*void DeviceDiscoveryDialog::displayPairingMenu(const QPoint &pos)
{
    if (ui->list->count() == 0)
        return;
    QMenu menu(this);
    QAction *pairAction = menu.addAction("Pair");
    QAction *removePairAction = menu.addAction("Remove Pairing");
    QAction *chosenAction = menu.exec(ui->list->viewport()->mapToGlobal(pos));
    QListWidgetItem *currentItem = ui->list->currentItem();

    QString text = currentItem->text();
    int index = text.indexOf(' ');
    if (index == -1)
        return;

    QBluetoothAddress address (text.left(index));
    if (chosenAction == pairAction) {
        localDevice->requestPairing(address, QBluetoothLocalDevice::Paired);
    } else if (chosenAction == removePairAction) {
        localDevice->requestPairing(address, QBluetoothLocalDevice::Unpaired);
    }
}*/
/*void DeviceDiscoveryDialog::pairingDone(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing)
{
    QList<QListWidgetItem *> items = ui->list->findItems(address.toString(), Qt::MatchContains);

    if (pairing == QBluetoothLocalDevice::Paired || pairing == QBluetoothLocalDevice::AuthorizedPaired ) {
        for (int var = 0; var < items.count(); ++var) {
            QListWidgetItem *item = items.at(var);
            item->setTextColor(QColor(Qt::green));
        }
    } else {
        for (int var = 0; var < items.count(); ++var) {
            QListWidgetItem *item = items.at(var);
            item->setTextColor(QColor(Qt::red));
        }
    }
}*/

void Device::changeOwnDeviceName()
{

    qDebug() << "Device name changed ";
    //qDebug() << "Device name changed " << QBluetoothLocalDevice::name();
    qDebug() << "Device name changed " << localDevice->name();

}

void Device::setServices()
{
    qDebug() << "Set service ";
    btService3 = new QBluetoothServiceInfo();
    btService3->setServiceName("koronako");
    btService3->setServiceDescription("proo");
    btService3->registerService();

}
