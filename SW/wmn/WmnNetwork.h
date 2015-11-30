#ifndef WMNNETWORK_H
#define WMNNETWORK_H

#include "WmnPacket.h"

class WmnNetwork
{
  public:
    WmnNetwork(void);

    static void onRxCallback(void * obj, WmnPacket * value);
  private:
    int mValue;
    void mRxCallback(WmnPacket * value);
};

#endif // WMNNETWORK_H
