#include "transceiver.h"

Transceiver::Transceiver()
{
    mDrv = new WmnDriverUsb(0x2345, 0x0000);
    QObject::connect(mDrv, &WmnDriverUsb::PacketReceived, this, &Transceiver::RxDone);
    QObject::connect(mDrv, &WmnDriverUsb::connected, this, &Transceiver::connected);
    QObject::connect(mDrv, &WmnDriverUsb::disconnected, this, &Transceiver::disconnected);
}

Transceiver::~Transceiver()
{
    delete mDrv;
}

void Transceiver::connect()
{
    if (m_status == 0)
        mDrv->start(QThread::TimeCriticalPriority);
}

void Transceiver::connected()
{
    m_status = 1;
    emit statusChanged();
}

void Transceiver::disconnected()
{
    m_status = 0;
    emit statusChanged();
}

void Transceiver::RxDone(int value)
{
    WmnDriverUsb::DriverPacket pkt;
    QString txt;

    pkt = mDrv->readPacket();
    for (uint8_t y = 0; y < 8; y++)
        for (uint8_t x = 0; x < 8; x++)
        {
            if (x != 7)
                txt += QString("%1 ").arg(pkt.data[y * 8 + x],2,16,QChar('0')).toUpper();
            else
                if (y != 7)
                    txt += QString("%1\n").arg(pkt.data[y * 8 + x],2,16,QChar('0')).toUpper();
                else
                    txt += QString("%1").arg(pkt.data[y * 8 + x],2,16,QChar('0')).toUpper();
        }
    emit packetReceived(txt);
}

void Transceiver::disconnect()
{
    if (m_status)
    {
        mDrv->quit();
        mDrv->wait();
    }
}

int Transceiver::send(int num)
{
    WmnDriverUsb::DriverPacket pkt;
    pkt.data[0] = num;
    mDrv->writePacket(pkt);
    return num + 0x40;
}

int Transceiver::read(void)
{
    unsigned char data[64];

    return data[0];
}

int Transceiver::status()
{
    return m_status;
}
