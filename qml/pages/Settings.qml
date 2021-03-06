/*Copyright (c) 2019, Riku Lahtinen
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.2
import Sailfish.Silica 1.0
import QtQuick.LocalStorage 2.0
import "./databases.js" as Mydb
import harbour.koronako.koronaclient 1.0
import harbour.koronako.koronascan 1.0


Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent


        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Settings page")
            }

            Text {
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                wrapMode: Text.WordWrap
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                text: {
                    qsTr("If you end to this page when starting the app, check the settings are OK.")
                }
            }

            SectionHeader {
                id: btVisibility
                visible: false
                text: qsTr("Check bluetooth visibility")
            }
            Text {
                id: btVisibilityText
                visible: false
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                wrapMode: Text.WordWrap
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                text: {
                    qsTr("Set bluetooth on and visible from the phone settings. Restart the app.")
                }
            }

            SectionHeader { text: qsTr("Server settings") }
            Text {
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                wrapMode: Text.WordWrap
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                text: {
                    qsTr("Insert here address info of your koronako-server")
                }
            }

            Row {
                TextField {
                    id: iipee
                    text: serverAddress
                    placeholderText: qsTr("IP address")
                    label: qsTr("IP address")
                    width: page.width*3/4
                    inputMethodHints: Qt.ImhNoPredictiveText
                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: {
                        koronaClient.sipadd = text;
                        serverAddress = text;
                        Mydb.saveSettings(0);
                        focus = false;
                    }
                }

                IconButton {
                    visible: iipee.text != ""
                    icon.source: "image://theme/icon-m-clear?" + (pressed
                                                                  ? Theme.highlightColor
                                                                  : Theme.primaryColor)
                    onClicked: {
                        iipee.text = ""
                        Mydb.saveSettings(0);
                    }
                }
            }

            Row {
                TextField {
                    id: portti
                    text: serverPort
                    placeholderText: qsTr("Port number")
                    label: qsTr("Port number")
                    width: page.width*3/4
                    inputMethodHints: Qt.ImhDigitsOnly
                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: {
                        koronaClient.sport = text
                        serverPort = text;
                        Mydb.saveSettings(0);
                        focus = false;
                    }
                }

                IconButton {
                    visible: portti.text != ""
                    icon.source: "image://theme/icon-m-clear?" + (pressed
                                                                  ? Theme.highlightColor
                                                                  : Theme.primaryColor)
                    onClicked: portti.text = ""
                }
            }

            Button {
                text:qsTr("Use common server")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    serverPort = "4243";
                    koronaClient.sport = serverPort;
                    portti.text = serverPort;
                    serverAddress = "77.240.23.45";
                    koronaClient.sipadd = serverAddress;
                    iipee.text = serverAddress;
                    Mydb.saveSettings(0);
                }
            }

            SectionHeader {
                id : phoneNameValidity
                visible: false
                text: qsTr("Phone name is not valid")
            }

            Text {
                id: phoneNameValidityText
                visible: false
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                wrapMode: Text.WordWrap
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                text: {
                    qsTr("The phone name is '%1'. The app will utilize last seven characters of the phone name. If the phone name is too short or too general, the app will not work. If you see this text, change the name from the device settings.").arg(koronaScan.ownDevice)
                }
            }

            Koronaclient {
                id: koronaClient
                onKorodataChanged:  {}
            }

            Koronascan {
                id: koronaScan
            }
        }
    }

    Component.onCompleted: {
        if (koronaScan.setDiscoverable()) {
            if (!koronaScan.getName()) {
                phoneNameValidity.visible = true
                phoneNameValidityText.visible = true
                if (developer){console.log ("The nametest did not succeed")}
            }
            else {
                phoneNameValidity.visible = false
                phoneNameValidityText.visible = false
                if (developer){console.log ("The nametest succeeded")}
            }
        }
        else {
            btVisibility.visible = true
            btVisibilityText.visible = true
            if (developer){console.log ("Bluetooth is off or not discoverable, nametest was not done")}
        }
    }
}
