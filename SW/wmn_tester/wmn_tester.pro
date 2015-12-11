TEMPLATE = app

QT += concurrent qml quick widgets

SOURCES += main.cpp \
    transceiver.cpp \
    testclass.cpp \
    ../wmn/driver/usb_crossplaform/WmnDriver.cpp \
    ../wmn/WmnNetwork.cpp \
    ../wmn/WmnCipher.cpp \
    ../wmn/driver/usb_crossplaform/WmnDriverUsb.cpp

lupdate_only{
SOURCES += *.qml \
          content/*.qml \
}

TRANSLATIONS    = wmn_tester_en.ts \
                  wmn_tester_ru.ts

LIBS +=          $$PWD\libs\libusb-1.0.a
INCLUDEPATH +=   $$PWD/libs/inc/ \
                 $$PWD/../wmn/ \
                 $$PWD/../wmn/driver/ \
                 $$PWD/../wmn/driver/usb_crossplaform/

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
    ../wmn/WmnPacket.h \
    ../wmn/WmnNetwork.h \
    ../wmn/WmnCipher.h \
    ../wmn/driver/usb_crossplaform/WmnDriver.h \
    ../wmn/driver/usb_crossplaform/WmnDriverUsb.h

FORMS +=
