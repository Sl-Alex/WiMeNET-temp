/**
* @file wmn_config.h
* @brief WiMeNet system-wide configuration
*
* @author Oleksii Slabchenko, alexslabchenko@gmail.com
*/

#ifndef WMN_CONFIG_H
#define WMN_CONFIG_H

// HW-DEFINED PARAMETERS
#define WMN_CONFIG_PACKET_LENGTH    62  ///< Single packet size (hardware-limited)
#define	WMN_CONFIG_QUEUE_SIZE       10  ///< Number of packets in RX/TX queue. Maximum is 255

#endif // WMN_CONFIG_H
