#ifndef WMNPACKETDISPATCHER_H
#define WMNPACKETDISPATCHER_H

#include "WmnPacket.h"

class WmnPacketDispatcher
{
  public:
    WmnPacketDispatcher(void);

    static void onRxCallback(void * obj, WmnPacket * value);
  private:
    int mValue;
    void mRxCallback(WmnPacket * value);
};

#endif // WMNPACKETDISPATCHER_H
