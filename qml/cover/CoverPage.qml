import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Label {
        id: label
        anchors.centerIn: parent
        //: App name is originally in Finnish "Koronako" which is like question "Corona?" or "Is it corona?" Feel free to translate.
        text: qsTr("Koronako")
    }

    /*CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-pause"
        }
    }*/
}
