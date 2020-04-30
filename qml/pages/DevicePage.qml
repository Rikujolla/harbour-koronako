import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.koronako.koronascan 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All




    SilicaListView {

        PullDownMenu {
            /*MenuItem {
                text: qsTr("Show Page 2")
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }*/
            MenuItem {
                text: qsTr("DeviceDiscovery")
                onClicked: {
                    koronaScan.setServices();
                    koronaScan.changeOwnDeviceName();
                    deviceList.clear();
                    koronaScan.startScan();
                }
            }
        }

        id: listView
        model: deviceList
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Devicelist")
        }
        delegate: BackgroundItem {
            id: delegate

            Label {
                x: Theme.horizontalPageMargin
                text: device
                anchors.verticalCenter: parent.verticalCenter
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
            onClicked: {
                //console.log("Clicked " + index)
                koronaScan.itemActivated(device)
            }
        }
        VerticalScrollDecorator {}
    }
    Koronascan {
        id:koronaScan
        onBtDeviceChanged: {
            deviceList.append({"device":btDevice})
            //console.log("test")
        }
    }

    ListModel {
        id: deviceList
        ListElement {
        device:""
        }
    }

}
