import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
    ///Commonstart
    property string version : "0.1.3"
    property string covidStartDate : "" // Start date for my corona
    property string covidEndDate : "" // End date for my corona
    property int minHits : 5 // minimum amount of hits to give exposure, related to discoveryTimer
    property int discoveryTimer : 60000 // discoveryTimer default value, discovered once per minute
    property string serverAddress : "" //Default server address
    property int serverPort: 4243 // Default server port
    property string lastUsed : "" // To be utilized later to remove old data if the ap has been used seldom
    property int coronaExposureSince : -1 // Days from last corona exposure
    property bool developer : true // Switch false before publishing the app
    property bool discoveryRunning : false //Defaulting app not working before tests
    ///Commonend
}
