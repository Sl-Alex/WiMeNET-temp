/**
* @file wmn_packet.h
* @brief WiMeNet packet definition
*
* @author Oleksii Slabchenko, alexslabchenko@gmail.com
*/

#ifndef WMN_PACKET_H
#define WMN_PACKET_H

#include <stdint.h>
#include "wmn_config.h"

/// @todo remove all WiMeNet packet-related definition from the driver
typedef struct __attribute__((packed))
{
    /// @todo Add reference to the connect protocol
	uint32_t network_id;	///< 0 means unconnected. Unconnected device can use only connect protocol
	uint32_t src;			///< 0 means anonymous (probably will be dropped)
	uint32_t dest;			///< 0 means broadcast
	uint8_t counter;		///< retransmission counter (decremented before each transmission. Will not be transmitted if equals to zero)
	uint8_t type;			///< Packet type
	uint8_t number;			///< Packet number (in case of several packet transmission)
	uint8_t total;			///< Total number of packets
	uint8_t length;			///< Length of the packet
} wmn_packet_header_t;

#define WMN_PACKET_HEADER_SIZE    (sizeof(wmn_packet_header_t)) ///< Calculate size of the header

typedef union __attribute__((packed))
{
	// RAW data
	uint8_t raw[WMN_CONFIG_PACKET_LENGTH + 2]; // Complete package in RAW form plus 2 status bytes

	// CC1101 packet
	struct __attribute__((packed))
	{
		// Header
		wmn_packet_header_t header;	// Protocol header

		// Data
		union __attribute__((packed))
		{
			uint8_t raw[WMN_CONFIG_PACKET_LENGTH - WMN_PACKET_HEADER_SIZE]; // Protocol data in RAW form
		} data;

        // Signal quality
        uint8_t lqi;
        uint8_t rssi;
	} packet;

} wmn_packet_t;

#endif // WMN_PACKET_H
