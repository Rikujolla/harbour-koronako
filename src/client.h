/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** Modified for app Fast Chess 2016 by Riku Lahtinen
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

#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QSslSocket>

class QTcpSocket;
class QNetworkSession;

class Client : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sipadd READ sipadd WRITE setSipadd NOTIFY sipaddChanged) //Server address
    Q_PROPERTY(int sport READ sport WRITE setSport NOTIFY sportChanged) // Server port
    Q_PROPERTY(QString korodata READ korodata WRITE setKorodata NOTIFY korodataChanged) //opponents move
    Q_PROPERTY(int msg READ msg WRITE setMsg NOTIFY msgChanged) // For messages
    Q_PROPERTY(int msg2 READ msg2 WRITE setMsg2 NOTIFY msg2Changed) // For messages
    Q_PROPERTY(QString expdata READ expdata WRITE setExpdata NOTIFY expdataChanged)

public:
    explicit Client();
    ~Client();
    QString sipadd(){return mySipadd;}
    void setSipadd(QString tee1){
      mySipadd = tee1;
      sipaddChanged(mySipadd);
    }
    int sport(){return mySport;}
    void setSport(int tee2){
      mySport = tee2;
      sportChanged(mySport);
    }
    QString korodata(){return myKorodata;}
    void setKorodata(QString tee3){
      myKorodata = tee3;
      korodataChanged(myKorodata);
    }
    int msg(){return myMsg;}
    void setMsg(int tee4){
      myMsg = tee4;
      msgChanged(myMsg);
    }
    int msg2(){return myMsg2;}
    void setMsg2(int tee5){
      myMsg2 = tee5;
      msgChanged(myMsg2);
    }
    QString expdata(){return myExpdata;}
    void setExpdata(QString tee6){
      myExpdata = tee6;
      expdataChanged(myExpdata);
    }
    Q_INVOKABLE void exchangeDataWithServer(QString toTcp);
    Q_INVOKABLE void startClient();
    QString getExposures();

signals:
    void sipaddChanged(QString tee1);
    void sportChanged(int tee2);
    void korodataChanged(QString tee3);
    void msgChanged(int tee4);
    void msg2Changed(int tee5);
    void expdataChanged(QString tee6);

private slots:
    //void requestNewFortune();
    void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    void sessionOpened();

private:
    QSslSocket *tcpSocket;
    QDataStream in;
    QString currentFortune;
    QNetworkSession *networkSession;
    QString mySipadd;
    int mySport;
    QString myKorodata;
    int myMsg;
    int myMsg2;
    QString myExpdata;

};

#endif
