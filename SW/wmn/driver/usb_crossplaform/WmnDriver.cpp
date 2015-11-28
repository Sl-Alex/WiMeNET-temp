#include "WmnDriver.h"

WmnDriver::WmnDriver()
{
    mState = WMN_DRIVER_RX;
}

WmnDriver::~WmnDriver()
{

}

void WmnDriver::Sleep(void)
{
    mState = WMN_DRIVER_SLEEP;
}

void WmnDriver::Wake(void)
{
    mState = WMN_DRIVER_RX;
}

WmnDriverState WmnDriver::getState(void)
{
    /// @todo: Implement
    return mState;
}

void WmnDriver::setRxCallback(void * obj, WmnDriver::RxCallback value)
{
    mRxCallbackObj = obj;
    mRxCallback = value;
}

void WmnDriver::setTestCallback(void * obj, WmnDriver::TestCallback value)
{
    mTestCallbackObj = obj;
    mTestCallback = value;
}


int WmnDriver::runTestCallback(int value)
{
    return mTestCallback(mTestCallbackObj, value);
}

// C style functions
extern "C" {

CWmnDriver * CWmnDriver_Create(void)
{
    WmnDriver * out = new WmnDriver();
    return ((CWmnDriver * )out);
}

void CWmnDriver_Destroy(CWmnDriver * driver)
{
    delete ((WmnDriver *)driver);
}

void CWmnDriver_Sleep(CWmnDriver * driver)
{
    ((WmnDriver *)driver)->Sleep();
}

void CWmnDriver_Wake(CWmnDriver * driver)
{
    ((WmnDriver *)driver)->Wake();
}


WmnDriverState CWmnDriver_getState(CWmnDriver * driver)
{
    return ((WmnDriver *)driver)->getState();
}

}
