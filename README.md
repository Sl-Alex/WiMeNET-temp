# WiMeNET structure

This repository contains source code for the wireless network based on CC1101 tracsceiver and STM32F103C8T6 microcontroller.

> WiMeNET is a short of the "**Wi**reless **Me**sh **Net**work".
> Probably "Mesh" will not be implemented, see below in `ToDo` section.

## Description

This repository is under development. Currently the following is implemented:

* CC1101 driver for STM32. Implementation uses GPIO interrupts for both "RX done" and "TX done" events, so polling is not necessary. Network operates at 868MHz.
* USB communication with the host PC via libusb (tested on Windows, should work on Linux as well)
* LCD driver (for debug output). Supports pcd8544 LCD (84x48 pixels), which is cheap and easy to find.

## ToDo

Of course, a lot of things should be done. Here they are:

* Network structure documentation. Will be documented in Dia.
* HW documentation. What and how to connect to make it work. At the moment the following hardware is used: [dev. board][1], [transceiver][2], [display][3]
* The network itself. At the moment only a draft driver is available
* Secure communication. AES-128 is planned, but any custom  Test implementation takes about 1.5ms for the 64-byte packet encryption. Decryption takes roughly the same time.
* Mesh implementation.

*Note: I'm not sure that mesh implementetion is necessary here. It seems that the network without mesh is more than enough for the smart home, because the communication on such a small distances is stable enough. Moreover, 868MHz frequency range is clear enough, unlike of 433MHz range*

  [1]: http://www.ebay.com/itm/STM32F103C8T6-ARM-STM32-Minimum-System-Development-Board-Module-For-Arduino-NEW-/161469482910
  [2]: http://www.ebay.com/itm/1PCS-CC1101-wireless-module-Long-Distance-Transmission-Antenna-868MHZ-NEW-/262159509635
  [3]: http://www.ebay.com/itm/84-48-LCD-Module-White-backlight-adapter-pcb-for-Nokia-5110-NEW-S3-/252056501566
