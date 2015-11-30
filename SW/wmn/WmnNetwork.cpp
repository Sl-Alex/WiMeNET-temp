#include "WmnNetwork.h"

// Creator
WmnNetwork::WmnNetwork(void)
{
}

/**
 * @brief WmnNetwork::mRxCallback
 * @param value : Wireless packet has been received
 */
void WmnNetwork::mRxCallback(WmnPacket * value)
{

}

/**
 * @brief WmnNetwork::onRxCallback
 * @param obj
 * @param value
 * @return
 */
void WmnNetwork::onRxCallback(void *obj, WmnPacket * value)
{
    return ((WmnNetwork *)obj)->mRxCallback(value);
}
