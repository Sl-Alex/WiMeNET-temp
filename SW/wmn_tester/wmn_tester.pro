TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    transceiver.cpp \
    testclass.cpp \
    ../wmn/driver/usb_crossplaform/WmnDriver.cpp \
    ../wmn/WmnNetwork.cpp

lupdate_only{
SOURCES += *.qml \
          content/*.qml \
}

TRANSLATIONS    = wmn_tester_en.ts \
                  wmn_tester_ru.ts

LIBS +=          $$PWD\libs\libusb-1.0.a
INCLUDEPATH +=   $$PWD/libs/inc/ \
                 $$PWD/../wmn/driver/

DEPENDPATH +=    $$PWD\libs\inc\
# PRE_TARGETDEPS+= $$PWD\libs\libusb-1.0.a

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    transceiver.h \
    testclass.h \
    ../wmn/driver/WmnDriver.h \
    ../wmn/WmnPacket.h \
    ../wmn/WmnNetwork.h

FORMS +=
