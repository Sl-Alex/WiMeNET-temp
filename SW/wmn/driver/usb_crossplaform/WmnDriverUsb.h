#ifndef WMNDRIVERUSB_H
#define WMNDRIVERUSB_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <stdint.h>

#include "libusb.h"

class WmnDriverUsb : public QThread
{
    Q_OBJECT
protected:
    void run();
public:
    WmnDriverUsb(uint16_t vid, uint16_t pid);
    ~WmnDriverUsb();

    void quit(void);

    typedef struct {
        uint8_t data[0x40];
    } DriverPacket;

    struct libusb_context *mLibUsbContext;
    libusb_device_handle * mDevHandle;
    volatile bool mConnected;
    void updateConnection(int value);

    void writePacket(DriverPacket value);
    DriverPacket readPacket(void);
signals:
    void sendTime(QString time);
    void PacketReceived(int value);
    void connected(void);
    void disconnected(void);
private:
    QString m_lastTime;

    uint16_t mVid, mPid;
    volatile bool mQuitRequest;
    struct libusb_transfer *mRxTransfer;
    struct libusb_transfer *mTxTransfer;

    void Disconnect(void);

    QQueue<DriverPacket> mRxQueue;
    QQueue<DriverPacket> mTxQueue;

    QMutex mRxMutex;
    QMutex mTxMutex;
};

#endif // WMNDRIVERUSB_H
