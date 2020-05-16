import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    property string covidStartDate : "" // Start date for my corona
    property string covidEndDate : "" // End date for my corona
    property int minHits : 15 // minimum amount of hits to give exposure, related to discoveryTimer
    property int discoveryTimer : 60000 // discoveryTimer default value, discovered once per minute
    property string serverAddress : "172.28.172.2" //Default server address
    property int serverPort: 32658 // Default server port
}
