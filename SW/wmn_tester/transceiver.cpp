#include "transceiver.h"

Transceiver::Transceiver()
{
    libusb_init(0);
    m_status =  0;
    dev_handle = NULL;
}

Transceiver::~Transceiver()
{
    libusb_exit(0);
}

void Transceiver::connect()
{
    /* Get the first device with the matching Vendor ID and Product ID. If
     * intending to allow multiple demo boards to be connected at once, you
     * will need to use libusb_get_device_list() instead. Refer to the libusb
     * documentation for details. */
    if (dev_handle != NULL) return;
    dev_handle = libusb_open_device_with_vid_pid(0, 0x2345, 0x0000);
    if (dev_handle)
    {
        libusb_claim_interface(dev_handle, 0);
        m_status = 1;
    }
    else
    {
        m_status = 0;
    }
    emit statusChanged();
}

void Transceiver::disconnect()
{
    if (dev_handle == NULL) return;
    libusb_release_interface(dev_handle,0);
    libusb_close(dev_handle);
    dev_handle = NULL;
    m_status = 0;
    emit statusChanged();
}

int Transceiver::send(int num)
{
    unsigned char data[64];
    int act = 0;

    for (uint8_t x = 0; x < 8; x++)
        for (uint8_t y = 0; y < 8; y++)
        {
            data[y * 8 + x] = y * 8 + x;
        }

    data[0] = num;

    if (dev_handle == NULL)
        return 0;

    libusb_bulk_transfer(dev_handle,(2 | LIBUSB_ENDPOINT_OUT),data,0x40,&act,2000);
    return act;
}

int Transceiver::read(void)
{
    unsigned char data[64];
    QString txt;
    int act = 0;

    if (dev_handle == NULL)
        return 0;

    libusb_bulk_transfer(dev_handle,(1 | LIBUSB_ENDPOINT_IN),data,0x40,&act,2000);
    for (uint8_t x = 0; x < 8; x++)
        for (uint8_t y = 0; y < 8; y++)
        {
            if (y != 7)
                txt += QString("%1 ").arg(data[y * 8 + x],2,16,QChar('0')).toUpper();
            else
                if (x != 7)
                    txt += QString("%1\n").arg(data[y * 8 + x],2,16,QChar('0')).toUpper();
                else
                    txt += QString("%1").arg(data[y * 8 + x],2,16,QChar('0')).toUpper();
        }
    emit packetReceived(txt);
    return data[0];
}

int Transceiver::status()
{
    return m_status;
}
