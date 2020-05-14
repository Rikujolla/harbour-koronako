#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QtQml>

#include "device.h"
#include "client.h"


int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/harbour-koronako.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).
    qmlRegisterType<Device>("harbour.koronako.koronascan", 1, 0, "Koronascan");
    qmlRegisterType<Client>("harbour.koronako.koronaclient", 1, 0, "Koronaclient");

    return SailfishApp::main(argc, argv);
}
