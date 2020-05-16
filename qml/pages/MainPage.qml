import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.LocalStorage 2.0
import harbour.koronako.koronascan 1.0
import harbour.koronako.koronaclient 1.0
import "./databases.js" as Mydb

Page {
    id: page


    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }
        }

        contentHeight: column.height

        Column {
            id: column
            property bool poweri : false
            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Main page")
            }

            SectionHeader { text: qsTr("Device exposures") }
            Text {
                id: exposuresText
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                wrapMode: Text.WordWrap
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                text: ""
            }

            SectionHeader { text: qsTr("Korona exposures") }
            Text {
                id: koronaExposuresText
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                wrapMode: Text.WordWrap
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                text: ""
            }

            Button {
                id:sendMyKorona
                text:"Check korona exposures"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    Mydb.checkMyExposures()
                    koronaClient.sport = serverPort
                    koronaClient.sipadd = serverAddress
                    koronaClient.exchangeDataWithServer()
                }
            }
            SectionHeader { text: qsTr("My korona disease") }
            Text {
                id: koronaDiseaseText
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                wrapMode: Text.WordWrap
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                text: qsTr("By sending my corona infection dates and exposure data to the server, I will help others to prevent of spreading the disease.")
            }
            Row{
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    id: koronaStart
                    text: "Start date"

                    onClicked: {
                        var dialog = pageStack.push(pickerComponent, {
                                                        date: new Date('2020/05/09')
                                                    })
                        dialog.accepted.connect(function() {
                            koronaStart.text = "Start: " + dialog.dateText
                        })
                    }
                }

                Button {
                    id: koronaEnd
                    text: "End date"

                    onClicked: {
                        var dialog = pageStack.push(pickerComponent, {
                                                        date: new Date('2020/05/09')
                                                    })
                        dialog.accepted.connect(function() {
                            koronaEnd.text = "End: " + dialog.dateText
                        })
                    }
                }
            }
            Component {
                id: pickerComponent
                DatePickerDialog {}
            }

            Button {
                text: "Send my disease data"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked:{
                    console.log("Sent data")
                }
            }




        }
    }

    Timer{
        //interval: 120000
        interval: 20000 //for testing
        running: true
        repeat: true
        onTriggered: {
            Mydb.findHits(current_day());
            //koronaScan.startScan();
            //devicesSeen.text = qsTr("Devices seen today") + ": " + koronaList.get(0).devices
        }
    }

    /*Timer{
        interval: 70000
        running: true
        repeat: true
        onTriggered: Mydb.findHits();
    }*/

    ListModel {
        id: koronaList
        ListElement {
            day:"today"
            devices: 0
            exposures: 0
        }
    }

    Koronascan {
        id:koronaScan
        onBtDeviceChanged:{
            //console.log(current_day(),btDevice, btDevice.substring(0, 2))
            Mydb.addHits(btDevice, btDevice.substring(0, 2))
        }
    }

    Koronaclient {
        id: koronaClient
        onCmoveChanged: console.log(cmove, "test")
    }


    property string current_date

    function current_day() {

        var d = new Date();
        var n = d.getDate()
        if (n < 10) {n = "0"+n}
        current_date = n
        return current_date
    }

    Component.onCompleted: {
        Mydb.findHits(current_day());
        koronaScan.ctime = discoveryTimer;
        Mydb.deleteOldData(current_day())
    }
}
