#include <QApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QSurfaceFormat>
#include "transceiver.h"
#include "libusb.h"
#include <QTranslator>

#include "WmnDriver.h"
#include "testclass.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WmnDriver * drv = new WmnDriver();

    TestClass * tst1 = new TestClass(10);
    TestClass * tst2 = new TestClass(20);

    drv->Sleep();


    int tmp = 0;

    drv->setTestCallback(tst1, TestClass::onRxCallback);
    tmp = drv->runTestCallback(1);
    drv->setTestCallback(tst2, TestClass::onRxCallback);
    tmp = drv->runTestCallback(3);

    drv->setTestCallback(tst1, TestClass::onRxCallback);
    tmp = drv->runTestCallback(5);
    drv->setTestCallback(tst2, TestClass::onRxCallback);
    tmp = drv->runTestCallback(7);

    QTranslator translator;
    translator.load("wmn_tester_ru.qm", app.applicationDirPath()+"/lang");

    app.installTranslator(&translator);

    // Register our component type with QML.
    qmlRegisterType<Transceiver>("com.ics.demo", 1, 0, "Transceiver");

    QQmlApplicationEngine engine;



    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    return app.exec();
}
