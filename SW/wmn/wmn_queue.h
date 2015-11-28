/**
* @file wmn_queue.h
* @brief Header file for wmn_queue.c
*
* @author Oleksii Slabchenko, alexslabchenko@gmail.com
*/

#ifndef WMN_QUEUE_H
#define WMN_QUEUE_H

#include <stdint.h>
#include "wmn_config.h"
#include "wmn_packet.h"

#define WMN_QUEUE_FLAG_IS_EMPTY	0x01    ///< Queue is empty
#define WMN_QUEUE_FLAG_IS_FULL	0x02    ///< Queue is full

/// @brief WiMeNET queue structure
typedef struct
{
	wmn_packet_t data[WMN_CONFIG_QUEUE_SIZE];   ///< Array of packets
	uint8_t read_index;                         ///< Read index
	uint8_t write_index;                        ///< Write index
	uint8_t flags;                              ///< Queue flags
	uint8_t count;                              ///< Number of elements in the queue
} wmn_queue_t;

void wmn_queue_init(wmn_queue_t * queue);
uint8_t wmn_queue_write(wmn_queue_t * queue, wmn_packet_t * packet, uint8_t in_irq);
uint8_t wmn_queue_read(wmn_queue_t * queue, wmn_packet_t * packet, uint8_t in_irq);

#endif // WMN_QUEUE_H
