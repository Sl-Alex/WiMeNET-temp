/**
* @file wmn_driver.h
* @brief Header file for wmn_driver.c
*
* @author Oleksii Slabchenko, alexslabchenko@gmail.com
*/

#ifndef WMN_DRIVER_H
#define WMN_DRIVER_H

#include <stdint.h>
#include "WmnPacket.h"

// Types definition
typedef void (*wmn_driver_rx_cb_t)(WmnPacket*); // New packet has arrived, can be read via the function parameter
typedef void (*wmn_driver_tx_cb_t)(void);           // Transmission is done, transmitter is ready

typedef enum {
    WMN_DRIVER_STATE_RX,
    WMN_DRIVER_STATE_TX
} WmnDriverState_t;

void wmn_driver_init(void);     // Driver initialization
void wmn_driver_deinit(void);   // Driver deinitialization
void wmn_driver_sleep(void);    // Switch driver to the sleep mode
void wmn_driver_wake(void);     // Switch driver to the active mode
void wmn_driver_set_rx_callback(wmn_driver_rx_cb_t cb);	// Set reception callback (new packet has arrived and can be processed)
void wmn_driver_set_tx_callback(wmn_driver_tx_cb_t cb);	// Set transmission callback (last transmission is done, new packet can be sent)
void wmn_driver_transmit(WmnPacket * packet);	// Transmit a packet immediately
void wmn_driver_retransmit(void);
WmnDriverState_t wmn_driver_get_state(void);
uint8_t wmn_driver_work(void); // Main function, should be called in the main loop

#endif // WMN_DRIVER_H
