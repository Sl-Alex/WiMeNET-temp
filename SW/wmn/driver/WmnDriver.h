#ifndef WMN_DRIVER_H
#define WMN_DRIVER_H

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

typedef enum {
    WMN_DRIVER_SLEEP,
    WMN_DRIVER_TX,
    WMN_DRIVER_RX,
} WmnDriverState;

#ifdef __cplusplus
class WmnDriver
{
  public:
    WmnDriver();
    ~WmnDriver();

    void Sleep(void);
    void Wake(void);
    WmnDriverState getState(void);

    typedef void (*RxCallback)(void *);
    typedef int (*TestCallback)(void *, int);

    void setRxCallback(void * obj, WmnDriver::RxCallback value);

    void setTestCallback(void * obj, WmnDriver::TestCallback value);

    int runTestCallback(int);

  private:
    WmnDriverState mState;

    RxCallback mRxCallback;
    void * mRxCallbackObj;

    TestCallback mTestCallback;
    void * mTestCallbackObj;
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void CWmnDriver;

CWmnDriver *   CWmnDriver_Create(void);
void           CWmnDriver_Destroy(CWmnDriver * driver);
void           CWmnDriver_Sleep(CWmnDriver * driver);
void           CWmnDriver_Wake(CWmnDriver * driver);
WmnDriverState CWmnDriver_getState(CWmnDriver * driver);

#ifdef __cplusplus
}
#endif


#endif // WMN_DRIVER_H
