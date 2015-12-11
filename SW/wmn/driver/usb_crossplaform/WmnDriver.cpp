#include "WmnDriver.h"


//using namespace QtConcurrent;

#include "stdio.h"

WmnDriver::WmnDriver()
{
}

WmnDriver::~WmnDriver()
{
//    libusb_exit(mLibUsbContext);
}

void WmnDriver::emitStatusChanged()
{
    emit statusChanged();
}

int WmnDriver::Init(void)
{
    /* Get the first device with the matching Vendor ID and Product ID. If
     * intending to allow multiple demo boards to be connected at once, you
     * will need to use libusb_get_device_list() instead. Refer to the libusb
     * documentation for details. */
//    if (mDevHandle != NULL){
//        DeInit();
//    }
//    mDevHandle = libusb_open_device_with_vid_pid(mLibUsbContext, WMN_DRIVER_VID, WMN_DRIVER_PID);
//    if (mDevHandle != NULL)
//    {
//        libusb_claim_interface(mDevHandle, 0);
//        StartRx();
        /// @todo update state
//        return 1;
//    }
//    else
//    {
        /// @todo update state
        return 0;
//    }
}

void WmnDriver::DeInit(void)
{
//    if (mDevHandle == NULL) return;

//    RxThreadState = false;

//    int tmp = mFuture.result();
//    tmp++;

//    libusb_release_interface(mDevHandle, 0);
//    libusb_close(mDevHandle);

//    mDevHandle = NULL;
}
