#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include <QObject>
#include <QString>
#include "libusb.h"

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
//    void generateKey();

signals:
    void statusChanged();
    void packetReceived(QString txt);

private:
    libusb_device_handle * dev_handle;
    int m_status;
};

#endif // TRANSCEIVER_H
