#include "WmnDriverUsb.h"

#include <QApplication>
#include <QDateTime>

WmnDriverUsb::WmnDriverUsb(uint16_t vid, uint16_t pid)
{
//    libusb_init(&mLibUsbContext);
    mVid = vid;
    mPid = pid;
}

WmnDriverUsb::~WmnDriverUsb()
{
//    libusb_exit(mLibUsbContext);
}

volatile bool rx_cb_done;
volatile bool tx_cb_done;
volatile bool rx_success;
volatile bool tx_success;

static void CALLBACK usb_rx_cb(struct libusb_transfer *xfr)
{
    rx_cb_done = true;
    switch(xfr->status)
    {
        case LIBUSB_TRANSFER_COMPLETED:
            // Success here, data transfered are inside
            // xfr->buffer
            // and the length is
            // xfr->actual_length
            rx_success = true;
            break;
        case LIBUSB_TRANSFER_CANCELLED:
            break;
        case LIBUSB_TRANSFER_NO_DEVICE:
            ((WmnDriverUsb *) xfr->user_data)->mConnected = false;
            break;
        case LIBUSB_TRANSFER_TIMED_OUT:
            break;
        case LIBUSB_TRANSFER_ERROR:
            ((WmnDriverUsb *) xfr->user_data)->mConnected = false;
            break;
        case LIBUSB_TRANSFER_STALL:
            ((WmnDriverUsb *) xfr->user_data)->mConnected = false;
            break;
        case LIBUSB_TRANSFER_OVERFLOW:
            ((WmnDriverUsb *) xfr->user_data)->mConnected = false;
            break;
    }
}

static void CALLBACK usb_tx_cb(struct libusb_transfer *xfr)
{
    tx_cb_done = true;
    switch(xfr->status)
    {
        case LIBUSB_TRANSFER_COMPLETED:
            // Success here, data transfered are inside
            // xfr->buffer
            // and the length is
            // xfr->actual_length
            tx_success = true;
            break;
        case LIBUSB_TRANSFER_CANCELLED:
            break;
        case LIBUSB_TRANSFER_NO_DEVICE:
            ((WmnDriverUsb *) xfr->user_data)->mConnected = false;
            break;
        case LIBUSB_TRANSFER_TIMED_OUT:
            break;
        case LIBUSB_TRANSFER_ERROR:
            ((WmnDriverUsb *) xfr->user_data)->mConnected = false;
            break;
        case LIBUSB_TRANSFER_STALL:
            ((WmnDriverUsb *) xfr->user_data)->mConnected = false;
            break;
        case LIBUSB_TRANSFER_OVERFLOW:
            ((WmnDriverUsb *) xfr->user_data)->mConnected = false;
            break;
    }
}

WmnDriverUsb::DriverPacket mTxPacket;
WmnDriverUsb::DriverPacket mRxPacket;


void WmnDriverUsb::updateConnection(int value)
{
    switch (value)
    {
        case LIBUSB_SUCCESS:
        case LIBUSB_ERROR_TIMEOUT:
            return;
    }

    mConnected = false;
}

void WmnDriverUsb::Disconnect()
{
    mConnected = false;
    libusb_cancel_transfer(mRxTransfer);
    libusb_cancel_transfer(mTxTransfer);
    libusb_free_transfer(mRxTransfer);
    libusb_free_transfer(mTxTransfer);
    libusb_release_interface(mDevHandle, 0);
    libusb_close(mDevHandle);
    libusb_exit(mLibUsbContext);
}

void WmnDriverUsb::run()
{
    int libusb_status = LIBUSB_SUCCESS;
    struct timeval zero_tv;
    zero_tv.tv_sec = 0;
    zero_tv.tv_usec = 0;

    mQuitRequest = false;
    mDevHandle = NULL;
    mConnected = false;

    while (mQuitRequest == false)
    {
        // Process events
        QApplication::processEvents();

        // Check if we have to connect
        if (mConnected == false)
        {
            // Sleep for a while (In this way we will offload CPU
            // while device is disconnected)
            msleep(10);

            mLibUsbContext = NULL;
            libusb_init(&mLibUsbContext);

            // Try to connect
            mDevHandle = libusb_open_device_with_vid_pid(mLibUsbContext, mVid, mPid);
            // Skip the rest if failed
            if (mDevHandle == NULL)
            {
                libusb_exit(mLibUsbContext);
                continue;
            }

            //msleep(200);

            // At this step we are connected, so we can proceed with the
            // connection, prepare and start our transfers
            if (libusb_claim_interface(mDevHandle, 0) != 0)
            {
                libusb_close(mDevHandle);
                libusb_exit(mLibUsbContext);
                continue;
            }

            mConnected = true;

            // Prepare transfers
            mRxTransfer = libusb_alloc_transfer(0);
            mTxTransfer = libusb_alloc_transfer(0);
            libusb_fill_bulk_transfer(mRxTransfer,
                                      mDevHandle,
                                      (1 | LIBUSB_ENDPOINT_IN), // Endpoint ID
                                      (unsigned char *)&mRxPacket,
                                      0x40,
                                      usb_rx_cb,
                                      this, // <-- Pointer to the object
                                      5000
                                      );
            libusb_fill_bulk_transfer(mTxTransfer,
                                      mDevHandle,
                                      (2 | LIBUSB_ENDPOINT_OUT), // Endpoint ID
                                      (unsigned char *)&mTxPacket,
                                      0x40,
                                      usb_tx_cb,
                                      this, // <-- Pointer to the object
                                      5000
                                      );

            rx_cb_done = true;
            tx_cb_done = true;
            tx_success = true;

            emit connected();
        }
        if (mConnected == false) continue;
        // We are already connected at this point
        if (rx_cb_done)
        {
            if (rx_success)
            {
                rx_success = false;
                mRxMutex.lock();
                mRxQueue.enqueue(mRxPacket);
                mRxMutex.unlock();
                emit PacketReceived(mRxQueue.count());
            }
            rx_cb_done = false;
            if(libusb_submit_transfer(mRxTransfer) < 0)
            {
                Disconnect();
            }
        }
        if (mConnected == false) continue;
        // Check if tx callback is done
        if (tx_cb_done)
        {
            // Check if tx callback is successful
            if (tx_success)
            {
                // Get the size of the tx queue
                mTxMutex.lock();
                bool isTxEmpty = mTxQueue.isEmpty();
                mTxMutex.unlock();
                // If there is something to transmit
                if (!isTxEmpty)
                {
                    // Start transmission
                    tx_success = false;
                    mTxMutex.lock();
                    mTxPacket = mTxQueue.dequeue();
                    mTxMutex.unlock();
                    tx_cb_done = false;
                    if(libusb_submit_transfer(mTxTransfer) < 0)
                    {
                        Disconnect();
                    }
                }
            }
            else
            {
                // Start transmission again
                tx_cb_done = false;
                if(libusb_submit_transfer(mTxTransfer) < 0)
                {
                    Disconnect();
                }
            }
        }
        if (mConnected == false) continue;

        libusb_status = libusb_handle_events_timeout_completed(
                    mLibUsbContext,
                    &zero_tv,
                    NULL
                    );
        updateConnection(libusb_status);

        // Error handling (connection/disconnection and so on)
        if (mConnected == false)
        {
            Disconnect();
            emit disconnected();
        }
    }
    if (mConnected)
    {
        Disconnect();
        emit disconnected();
    }
}

void WmnDriverUsb::quit()
{
    mQuitRequest = true;
}

void WmnDriverUsb::writePacket(DriverPacket value)
{
    mTxMutex.lock();
    mTxQueue.enqueue(value);
    mTxMutex.unlock();
}

WmnDriverUsb::DriverPacket WmnDriverUsb::readPacket(void)
{
    DriverPacket pkt;
    mRxMutex.lock();
    if (!mRxQueue.isEmpty())
        pkt = mRxQueue.dequeue();
    mRxMutex.unlock();
    return pkt;
}
