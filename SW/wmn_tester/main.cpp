#include <QApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QSurfaceFormat>
#include "transceiver.h"
#include "libusb.h"
#include <QTranslator>

#include "WmnDriver.h"
#include "testclass.h"

#include "WmnCipher.h"

#include "WmnDriverUsb.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //WmnDriver * drv = new WmnDriver();

    TestClass * tst1 = new TestClass(10);
    TestClass * tst2 = new TestClass(20);

    //drv->Sleep();


    int tmp = 0;

    //drv->setTestCallback(tst1, TestClass::onRxCallback);
    //tmp = drv->runTestCallback(1);
    //drv->setTestCallback(tst2, TestClass::onRxCallback);
    //tmp = drv->runTestCallback(3);

    //drv->setTestCallback(tst1, TestClass::onRxCallback);
    //tmp = drv->runTestCallback(5);
    //drv->setTestCallback(tst2, TestClass::onRxCallback);
    //tmp = drv->runTestCallback(7);

    WmnCipher * cph = new WmnCipher();

    uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    uint8_t in[]  = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
    uint8_t out[] = {0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97};
    uint8_t buffer[16];

    //AES128_ECB_encrypt(in, key, buffer);

    printf("ECB decrypt: ");

    //AES128_CBC_encrypt_buffer(buffer, in, 64, key, iv);
    cph->encryptBuffer(in,buffer,key);

    if(0 == strncmp((char*) out, (char*) buffer, 16))
    {
      printf("SUCCESS!\n");
    }
    else
    {
      printf("FAILURE!\n");
    }

    memcpy(out,buffer,16);

    cph->decryptBuffer(out,buffer,key);

    if(0 == strncmp((char*) in, (char*) buffer, 16))
    {
      printf("SUCCESS!\n");
    }
    else
    {
      printf("FAILURE!\n");
    }

    QTranslator translator;
    translator.load("wmn_tester_ru.qm", app.applicationDirPath()+"/lang");

    app.installTranslator(&translator);

    // Register our component type with QML.
    qmlRegisterType<Transceiver>("com.ics.demo", 1, 0, "Transceiver");

    QQmlApplicationEngine engine;

//    WmnDriverUsb * drvusb = new WmnDriverUsb(0x2345, 0x0000);
//    drvusb->start();

//    engine.rootContext()->setContextProperty("drvusb", drvusb);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    app.exec();

//    drvusb->quit();
//    drvusb->wait();

    return 0;
}
