/**
* @file wmn_driver.h
* @brief Header file for wmn_driver.c
*
* @author Oleksii Slabchenko, alexslabchenko@gmail.com
*/

#ifndef WMN_DRIVER_H
#define WMN_DRIVER_H

#include <stdint.h>
#include "wmn_packet.h"

// Types definition
typedef void (*wmn_driver_rx_cb_t)(wmn_packet_t *); // New packet has arrived, can be read via the function parameter
typedef void (*wmn_driver_tx_cb_t)(void);           // Transmission is done, transmitter is ready

void wmn_driver_init(void);     // Driver initialization
void wmn_driver_deinit(void);   // Driver deinitialization
void wmn_driver_sleep(void);    // Switch driver to the sleep mode
void wmn_driver_wake(void);     // Switch driver to the active mode
void wmn_driver_set_rx_callback(wmn_driver_rx_cb_t cb);	// Set reception callback (new packet has arrived and can be processed)
void wmn_driver_set_tx_callback(wmn_driver_tx_cb_t cb);	// Set transmission callback (last transmission is done, new packet can be sent)
void wmn_driver_transmit(wmn_packet_t * packet);	// Transmit a packet immediately
/// @TODO remove this function
uint8_t wmn_driver_receive(wmn_packet_t * packet, uint16_t timeout);

#endif // WMN_DRIVER_H
