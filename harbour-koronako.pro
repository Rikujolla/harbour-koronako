# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-koronako

CONFIG += sailfishapp
# Uncomment next line for release
#DEFINES += QT_NO_DEBUG_OUTPUT

QT += bluetooth

SOURCES += src/harbour-koronako.cpp \
    src/client.cpp \
    src/device.cpp

DISTFILES += qml/harbour-koronako.qml \
    qml/cover/CoverPage.qml \
    qml/pages/About.qml \
    qml/pages/MainPage.qml \
    qml/pages/Settings.qml \
    qml/pages/databases.js \
    rpm/harbour-koronako.changes \
    rpm/harbour-koronako.changes.run \
    rpm/harbour-koronako.spec \
    rpm/harbour-koronako.yaml \
    translations/*.ts \
    harbour-koronako.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-koronako-de.ts \
                translations/harbour-koronako-fi.ts
HEADERS += \
    src/client.h \
    src/device.h
