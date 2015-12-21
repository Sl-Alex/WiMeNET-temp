#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include <QObject>
#include <QString>
#include "libusb.h"

#include "WmnDriverUsb.h"

class Transceiver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int status READ status NOTIFY statusChanged)

public:
    Transceiver();
    ~Transceiver();

    int status();

public slots:
    void connect();
    void disconnect();
    int send(int num);
    int read(void);

signals:
    void statusChanged();
    void packetReceived(QString txt);
    void test();

private:
    int mStatus;
    WmnDriverUsb * mDrv;
private slots:
    void RxDone(int value);
    void connected(void);
    void disconnected(void);
};

#endif // TRANSCEIVER_H
