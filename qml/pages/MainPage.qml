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

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }
            /*MenuItem {
                text: qsTr("DeviceDiscovery")
                onClicked: koronaScan.startScan();
            }*/
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column
            property bool poweri : false
            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Main page")
            }

            SectionHeader { text: qsTr("Exposures") }
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
                onClicked: {
                    Mydb.checkMyExposures()
                    koronaClient.sport = 32653
                    koronaClient.sipadd = "172.28.172.2"
                    koronaClient.requestNewFortune()
                    //Mydb.deleteOldData(current_day())
                    //koronaScan.on_power_clicked(column.poweri);
                    //!column.poweri
                }
            }
            SectionHeader { text: qsTr("My korona") }
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
                text: "My korona disease started 9.5.2020"
            }
            Row{
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

            /*Label {
                id: devicesSeen
                x: Theme.horizontalPageMargin
                text: qsTr("Devices seen today") + ": " + koronaList.get(0).devices
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            Label {
                id: exposuresMet
                x: Theme.horizontalPageMargin
                text: qsTr("Device exposures today") + ": " + koronaList.get(0).exposures
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }*/

            Button {
                text: "Send my disease data"
            }

            Button {
                id:power
                text:"Clear today"
                onClicked: {
                    Mydb.deleteOldData(current_day())
                    //koronaScan.on_power_clicked(column.poweri);
                    //!column.poweri
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
    }
}
