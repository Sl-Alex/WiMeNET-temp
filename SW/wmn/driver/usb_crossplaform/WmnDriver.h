#ifndef WMN_DRIVER_H
#define WMN_DRIVER_H

#include <QObject>
#include <QtConcurrent>

#include "libusb.h"

/*
 * Usage: C++
 *   WmnDriver drv;
 *   drv.Sleep();
 *   WmnDriverState tmp = drv.getState();
 *
 * Usage: C
 *   CWmnDriver * drv = CWmnDriver_Create();
 *   CWmnDriver_Sleep(drv);
 *   WmnDriverState tmp = CWmnDriver_getState(drv);
 *
 **/

#include <QObject>

typedef enum {
    WMN_DRIVER_SLEEP,
    WMN_DRIVER_TX,
    WMN_DRIVER_RX,
} WmnDriverState;

#define WMN_DRIVER_VID  0x2345
#define WMN_DRIVER_PID  0x0000

#ifdef __cplusplus
class WmnDriver : public QObject
{
    Q_OBJECT

  public:
    WmnDriver();
    ~WmnDriver();

    void emitStatusChanged();

    int Init(void);
    void DeInit(void);
//    void Sleep(void);
//    void Wake(void);
//    WmnDriverState getState(void);

    typedef void (*RxCallback)(void *);
    typedef int (*TestCallback)(void *, int);

//    void setRxCallback(void * obj, WmnDriver::RxCallback value);

//    void setTestCallback(void * obj, WmnDriver::TestCallback value);

//    int runTestCallback(int);

    void StartRx(void);

    volatile bool RxThreadState;
    libusb_device_handle * mDevHandle;

signals:
  statusChanged();

  private:
    WmnDriverState mState;

    RxCallback mRxCallback;
    void * mRxCallbackObj;

    TestCallback mTestCallback;
    void * mTestCallbackObj;


    QFuture<int> mFuture;
    //static void CALLBACK RxThread(void * obj);
};
#endif

//#ifdef __cplusplus
//extern "C" {
//#endif

//typedef void CWmnDriver;

//CWmnDriver *   CWmnDriver_Create(void);
//void           CWmnDriver_Destroy(CWmnDriver * driver);
//void           CWmnDriver_Sleep(CWmnDriver * driver);
//void           CWmnDriver_Wake(CWmnDriver * driver);
//WmnDriverState CWmnDriver_getState(CWmnDriver * driver);

//#ifdef __cplusplus
//}
//#endif


#endif // WMN_DRIVER_H
