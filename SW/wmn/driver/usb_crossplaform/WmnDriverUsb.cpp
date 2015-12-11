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

volatile bool rx_done;
volatile bool tx_done;

static void CALLBACK usb_rx_cb(struct libusb_transfer *xfr)
{
    rx_done = true;
    ((WmnDriverUsb *) xfr->user_data)->updateConnection(xfr->status);
    switch(xfr->status)
    {
        case LIBUSB_TRANSFER_COMPLETED:
            // Success here, data transfered are inside
            // xfr->buffer
            // and the length is
            // xfr->actual_length
            break;
        case LIBUSB_TRANSFER_CANCELLED:
        case LIBUSB_TRANSFER_NO_DEVICE:
        case LIBUSB_TRANSFER_TIMED_OUT:
        case LIBUSB_TRANSFER_ERROR:
        case LIBUSB_TRANSFER_STALL:
        case LIBUSB_TRANSFER_OVERFLOW:
            // Various type of errors here
            break;
    }
}

static void CALLBACK usb_tx_cb(struct libusb_transfer *xfr)
{
    tx_done = true;
    ((WmnDriverUsb *) xfr->user_data)->updateConnection(xfr->status);
    switch(xfr->status)
    {
        case LIBUSB_TRANSFER_COMPLETED:
            // Success here, data transfered are inside
            // xfr->buffer
            // and the length is
            // xfr->actual_length
            break;
        case LIBUSB_TRANSFER_CANCELLED:
        case LIBUSB_TRANSFER_NO_DEVICE:
        case LIBUSB_TRANSFER_TIMED_OUT:
        case LIBUSB_TRANSFER_ERROR:
        case LIBUSB_TRANSFER_STALL:
        case LIBUSB_TRANSFER_OVERFLOW:
            // Various type of errors here
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

void WmnDriverUsb::run()
{
    int libusb_status = LIBUSB_SUCCESS;

    mQuitRequest = false;
    mDevHandle = NULL;
    mConnected = false;

    while (mQuitRequest == false)
    {
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

            rx_done = false;
            tx_done = false;

            emit connected();

            if(libusb_submit_transfer(mRxTransfer) < 0)
            {
                /// @todo Error
            }

            /// @todo Implement
            //if (have_untransmitted_data)
            if(libusb_submit_transfer(mTxTransfer) < 0)
            {
                /// @todo Error
            }
        }
        // We are already connected at this point
        if (rx_done)
        {
            mRxMutex.lock();
            mRxQueue.enqueue(mRxPacket);
            mRxMutex.unlock();
            emit PacketReceived(mRxQueue.count());
            rx_done = false;
            if(libusb_submit_transfer(mRxTransfer) < 0)
            {
                // Error
            }
        }
        if (tx_done)
        {
            /// @todo Implement
            //if (have_untransmitted_data)
                tx_done = false;
            mTxPacket.data[0] = 1 - mTxPacket.data[0];
            if(libusb_submit_transfer(mTxTransfer) < 0)
            {
                // Error
            }
        }
        libusb_status = libusb_handle_events(mLibUsbContext);
        /// @todo Implement error handling
        //if(libusb_handle_events(mLibUsbContext) != LIBUSB_SUCCESS) break;

        updateConnection(libusb_status);

        // Error handling (connection/disconnection and so on)
        if (mConnected == false)
        {
            libusb_cancel_transfer(mRxTransfer);
            libusb_cancel_transfer(mTxTransfer);
            libusb_free_transfer(mRxTransfer);
            libusb_free_transfer(mTxTransfer);
            libusb_release_interface(mDevHandle, 0);
            libusb_close(mDevHandle);
            libusb_exit(mLibUsbContext);
            emit disconnected();
        }

        QApplication::processEvents();
        //usleep(1);
    }
    if (mConnected)
    {
        libusb_cancel_transfer(mRxTransfer);
        libusb_cancel_transfer(mTxTransfer);
        //libusb_handle_events_completed(mLibUsbContext, NULL);
        libusb_free_transfer(mRxTransfer);
        libusb_free_transfer(mTxTransfer);
        libusb_release_interface(mDevHandle, 0);
        libusb_close(mDevHandle);
        libusb_exit(mLibUsbContext);

        mConnected = false;
        emit disconnected();
    }
}

void WmnDriverUsb::quit()
{
    mQuitRequest = true;
}

void WmnDriverUsb::timerHit()
{
/*
    QString newTime= QDateTime::currentDateTime().toString("ddd MMMM d yy, hh:mm:ss");
    if(m_lastTime != newTime ){
        m_lastTime = newTime;
        emit sendTime(newTime) ;
    }
*/
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
    pkt = mRxQueue.dequeue();
    mRxMutex.unlock();
    return pkt;
}
