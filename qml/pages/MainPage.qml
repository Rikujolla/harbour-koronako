import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.LocalStorage 2.0
import harbour.koronako.koronascan 1.0
import harbour.koronako.koronaclient 1.0
import "./databases.js" as Mydb

Page {
    id: page

    property var messages: [{mesg:""},
        {mesg:qsTr("Not connected to server!")},
        {mesg:qsTr("Exposured!")},
        {mesg:qsTr("No exposure!")},
        {mesg:qsTr("Sent corona data!")},
        {mesg:qsTr("Wrong app version!")},
        {mesg:qsTr("Other error!")}
    ]

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
                visible: developer
                text: qsTr("Show data")
                onClicked: pageStack.push(Qt.resolvedUrl("ShowData.qml"))
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
                title: qsTr("Today")
            }
            BackgroundItem {
                SectionHeader { text: qsTr("Phones close of my phone: %1").arg(koronaList.get(0).devices) }
                onClicked: {
                    closeText.visible = !closeText.visible
                    Mydb.saveSettings(1)
                }
            }
            Text {
                id: closeText
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                wrapMode: Text.WordWrap
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                text: qsTr("Number of the phones the Koronako app has detected today close of your phone.")
            }

            BackgroundItem {
                SectionHeader { text: qsTr("Phone exposures: %1").arg(koronaList.get(0).exposures) }
                onClicked: {
                    exposuresText.visible = !exposuresText.visible
                    Mydb.saveSettings(1)
                }
            }
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
                text: qsTr("Number of the those phones that have exceeded determined exposure time.")
            }

            BackgroundItem {
                SectionHeader {
                    id:headDaysCorona
                    text: qsTr("Days from last corona exposure: %1").arg(koronaList.get(0).coronaExposureSince < 0 ? "-" : koronaList.get(0).coronaExposureSince)
                }
                onClicked: {
                    koronaExposuresText.visible = !koronaExposuresText.visible
                    Mydb.saveSettings(1)
                }
            }

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
                text: qsTr("By sending your exposure data to the server, you can check if somebody has exposured you to coronavirus.")
            }

            Text {
                id: exposuresCheckedText
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                wrapMode: Text.WordWrap
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                text: qsTr("Exposures checked from server: %1").arg("NOT KNOWN")
            }

            Text {
                id: msgRow1
                visible:false
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
                id:checkMyKorona
                text:qsTr("Check corona exposures")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    enabled = false;
                    msgRow1.visible = false
                    //Mydb.checkMyExposures()
                    koronaClient.sport = serverPort
                    koronaClient.sipadd = serverAddress
                    koronaClient.exchangeDataWithServer(Mydb.checkMyExposures())
                }
            }
            BackgroundItem {

                SectionHeader { text: qsTr("My korona infection") }
                onClicked: {
                    koronaDiseaseText.visible = !koronaDiseaseText.visible
                    Mydb.saveSettings(1)
                }
            }
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
                    text: qsTr("Start date")

                    onClicked: {
                        var dialog = pageStack.push(pickerComponent, {
                                                        date: covidStartDate != "" ? covidStartDate : new Date('2020/05/01')
                                                    })
                        dialog.accepted.connect(function() {
                            // Not accepting future date
                            if((new Date(dialog.date)-new Date())/24/3600/1000 >0.5){
                                covidStartDate = new Date()
                                koronaStart.text = new Date(covidStartDate).toLocaleDateString(Qt.locale(),Locale.ShortFormat)
                            }
                            // Start date cannot be newer than end date
                            else if((new Date(dialog.date)-new Date(covidEndDate))/24/3600/1000 >0.5){
                                covidStartDate = covidEndDate
                                koronaStart.text = new Date(covidStartDate).toLocaleDateString(Qt.locale(),Locale.ShortFormat)

                            }
                            else {
                                koronaStart.text = dialog.dateText
                                covidStartDate = dialog.date
                            }
                            Mydb.saveSettings(1);
                        })
                    }
                }

                Button {
                    id: koronaEnd
                    text: qsTr("End date")

                    onClicked: {
                        var dialog = pageStack.push(pickerComponent, {
                                                        date: covidEndDate != "" ? covidEndDate : new Date()
                                                    })
                        dialog.accepted.connect(function() {
                            // Not accepting future date
                            if((new Date(dialog.date)-new Date())/24/3600/1000 >0.5){
                                covidEndDate = new Date()
                                koronaEnd.text = new Date(covidEndDate).toLocaleDateString(Qt.locale(),Locale.ShortFormat)
                            }
                            // End day cannot be earlier than start date
                            else if ((new Date(dialog.date)-new Date(covidStartDate))/24/3600/1000 < 0.5) {
                                covidEndDate = covidStartDate
                                koronaEnd.text = new Date(covidStartDate).toLocaleDateString(Qt.locale(),Locale.ShortFormat)
                            }
                            else {
                                koronaEnd.text = dialog.dateText
                                covidEndDate = dialog.date
                            }
                            Mydb.saveSettings(1);
                        })
                    }
                }
            }
            Component {
                id: pickerComponent
                DatePickerDialog {}
            }

            Text {
                id: exposuresSentText
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                wrapMode: Text.WordWrap
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                text: qsTr("Infection data sent to the server: %1").arg("NOT KNOWN")
            }


            Button {
                id: sendMyKorona
                text: qsTr("Send my infection data")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked:{
                    enabled = false
                    koronaStart.enabled = false
                    koronaEnd.enabled = false
                    //Mydb.readMyKorona()
                    koronaClient.sport = serverPort
                    koronaClient.sipadd = serverAddress
                    //koronaClient.exchangeDataWithServer("MyKoronaData")
                    koronaClient.exchangeDataWithServer(Mydb.readMyKorona())
                }
            }

            Text {
                id: msgRow2
                visible:false
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
        }
    }

    Timer{
        interval: discoveryTimer
        running: discoveryRunning
        repeat: true
        onTriggered: {
            koronaScan.setDiscoverable();
            Mydb.findHits(current_day());
            koronaScan.startScan();
        }
    }


    ListModel {
        id: koronaList
        ListElement {
            day:"today"
            devices: 0
            exposures: 0
            coronaExposureSince: -1
        }
    }

    Koronascan {
        id:koronaScan
        onBtDeviceChanged:{
            Mydb.addHits(btDevice, btDevice.substring(0, 2))
        }
    }

    Koronaclient {
        id: koronaClient
        onKorodataChanged:  {}
        onMsgChanged: {
            switch (msg) {
            case 1: // No connection to server
                if (checkMyKorona.enabled == false){
                    msgRow1.visible = true
                    msgRow1.text = qsTr("Exposure status: ") + messages[msg].mesg
                    checkMyKorona.enabled = true
                }
                else {
                    msgRow2.visible = true
                    msgRow2.text = qsTr("Data sent status: ") + messages[msg].mesg
                    sendMyKorona.enabled = true
                    koronaStart.enabled = true
                    koronaEnd.enabled = true
                }
                break;
            case 2: // Exposured
                msgRow1.visible = true
                msgRow1.text = qsTr("Exposure status: ") + messages[msg].mesg
                checkMyKorona.enabled = true
                exposuresCheckedText.text = qsTr("Exposures checked from server: %1").arg(new Date().toLocaleString(Qt.locale(),Locale.ShortFormat))
                break;
            case 3: // Not exposured
                msgRow1.visible = true
                msgRow1.text = qsTr("Exposure status: ") + messages[msg].mesg
                checkMyKorona.enabled = true
                exposuresCheckedText.text = qsTr("Exposures checked from server: %1").arg(new Date().toLocaleString(Qt.locale(),Locale.ShortFormat))
                break;
            case 4: //Data sent
                Mydb.removeMyKorona()
                msgRow2.visible = true
                msgRow2.text = qsTr("Data sent status: ") + messages[msg].mesg
                sendMyKorona.enabled = true
                koronaStart.enabled = true
                koronaEnd.enabled = true
                exposuresSentText.text = qsTr("Infection data sent to the server: %1").arg(new Date().toLocaleString(Qt.locale(),Locale.ShortFormat))
                break;
            case 5: // Wrong app version
                if (checkMyKorona.enabled == false){
                    msgRow1.visible = true
                    msgRow1.text = qsTr("Exposure status: ") + messages[msg].mesg
                    checkMyKorona.enabled = true
                }
                else {
                    msgRow2.visible = true
                    msgRow2.text = qsTr("Data sent status: ") + messages[msg].mesg
                    sendMyKorona.enabled = true
                    koronaStart.enabled = true
                    koronaEnd.enabled = true
                }
                break;
            case 6: // Other error
                if (checkMyKorona.enabled == false){
                    msgRow1.visible = true
                    msgRow1.text = qsTr("Exposure status: ") + messages[msg].mesg
                    checkMyKorona.enabled = true
                }
                else {
                    msgRow2.visible = true
                    msgRow2.text = qsTr("Data sent status: ") + messages[msg].mesg
                    sendMyKorona.enabled = true
                    koronaStart.enabled = true
                    koronaEnd.enabled = true
                }
                break;
            default:
                break;
            }

        }
        onMsg2Changed: {
            if (msg2 > new Date().getDate()){
                //rough estimate
                koronaList.set(0,{"coronaExposureSince": new Date().getDate() + 31 - msg2})
            }
            else {
                koronaList.set(0,{"coronaExposureSince": new Date().getDate() - msg2})
            }

            headDaysCorona.text = qsTr("Days from last corona exposure: %1").arg(koronaList.get(0).coronaExposureSince < 0 ? "-" : koronaList.get(0).coronaExposureSince)
        }
    }


    property string current_date

    function current_day() {

        var d = new Date();
        var n = d.getDate()
        if (n < 10) {n = "0"+n}
        current_date = n
        return current_date
    }

    Timer {
        id: delay
        interval: 160
        running: false
        repeat: false
        onTriggered: {
            if (!discoveryRunning || serverAddress==""){pageStack.push(Qt.resolvedUrl("Settings.qml"))}
        }
    }

    Component.onCompleted: {
        Mydb.findHits(current_day());
        koronaScan.ctime = discoveryTimer;
        Mydb.deleteOldData(current_day())
        Mydb.loadSettings()
        covidStartDate != "" ? koronaStart.text = new Date(covidStartDate).toLocaleDateString(Qt.locale(),Locale.ShortFormat) : koronaStart.text = qsTr("Start date")
        covidEndDate != "" ? koronaEnd.text = new Date(covidEndDate).toLocaleDateString(Qt.locale(),Locale.ShortFormat) : koronaEnd.text = qsTr("End date")
        koronaScan.setDiscoverable();
        if (!discoveryRunning || serverAddress==""){delay.start()}
    }
}
