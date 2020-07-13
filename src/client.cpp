/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
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

#include <QtNetwork>
#include <QDebug>
#include <QDataStream>

#include "client.h"

Client::Client()
    : tcpSocket(new QSslSocket(this))
{}

Client::~Client()
{

}

void Client::exchangeDataWithServer(QString _toTcp)
{
    qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
    QString pairData = "";
    //qDebug() << _toTcp << "TCP";
    pairData = myExpdata + "-";
    qDebug() << "Pairdata" << pairData;

    tcpSocket->setProtocol(QSsl::TlsV1_0);

    tcpSocket->connectToHostEncrypted(mySipadd,mySport);

    tcpSocket->ignoreSslErrors();

    if (tcpSocket->waitForEncrypted(5000)) {
        qDebug() << "Crypted!" << tcpSocket->errorString() << tcpSocket->sslErrors().length();
        const QSslCipher cipher = tcpSocket->sessionCipher();
        const QString cipherInfo = QString("%1, %2 (%3/%4)").arg(cipher.authenticationMethod())
                                           .arg(cipher.name()).arg(cipher.usedBits())
                                           .arg(cipher.supportedBits());;
        //qDebug() << cipherInfo;
        int values = pairData.length();
        uint iterator = 0;
        std::string part = pairData.toStdString();
        QString result;
        while (values > 0) {
            std::string temp = part.substr(iterator, 1024);
            char temp2[1025];
            strcpy(temp2, temp.c_str());
            tcpSocket->write(temp2,temp.length());
            QString string(temp2);
            result = result + string;
            qDebug() << result;
            values = values - 1024;
            iterator = iterator + 1024;
        }
        qDebug() << "Data sent" << mySipadd << mySport << result;

        if(tcpSocket->waitForReadyRead()){
            //qDebug() << "Reading: " << tcpSocket->bytesAvailable();

            QByteArray temp2 = tcpSocket->readAll();
            QString nextFortune = QString::fromStdString(temp2.toStdString());
            if (nextFortune.left(10) == "EXPOSUREDD"){
                myMsg = 2; // Exposure message
                msgChanged(myMsg);
                myMsg2 = nextFortune.right(2).toInt();
                msg2Changed(myMsg2);
                qDebug() << "Server answers: " << nextFortune << myMsg << myMsg2;
            }
            else if (nextFortune.left(10) == "NOEXPOSURE"){
                qDebug() << "Server answers: " << nextFortune;
                myMsg = 3; // Noexposure message
                msgChanged(myMsg);
            }
            else if (nextFortune.left(10) == "SENTCOVIDD") {
                qDebug() << "Server answers: " << nextFortune;
                myMsg = 4; // Sent data message
                msgChanged(myMsg);
            }
            else if (nextFortune.left(10) == "ERRORERROR") {
                qDebug() << "Server answers error: " << nextFortune;
                myMsg = 5; // Error message
                msgChanged(myMsg);
            }
            else {
                qDebug() << "Server answers error: " << nextFortune;
                myMsg = 6; // Error message
                msgChanged(myMsg);
            }
            tcpSocket->disconnectFromHost();
        }
        else {
            tcpSocket->abort();
            qDebug("Data could not be read");
        }
    }
    else {
        tcpSocket->abort();
        myMsg = 1; // No connection message
        msgChanged(myMsg);
        qDebug("Not connected!");
    }
}
