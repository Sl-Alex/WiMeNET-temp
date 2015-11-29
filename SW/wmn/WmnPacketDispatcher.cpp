#include "WmnPacketDispatcher.h"

// Creator
WmnPacketDispatcher::WmnPacketDispatcher(void)
{
}

/**
 * @brief WmnPacketDispatcher::mRxCallback
 * @param value : Wireless packet has been received
 */
void WmnPacketDispatcher::mRxCallback(WmnPacket * value)
{

}

/**
 * @brief WmnPacketDispatcher::onRxCallback
 * @param obj
 * @param value
 * @return
 */
void WmnPacketDispatcher::onRxCallback(void *obj, WmnPacket * value)
{
    return ((WmnPacketDispatcher *)obj)->mRxCallback(value);
}
