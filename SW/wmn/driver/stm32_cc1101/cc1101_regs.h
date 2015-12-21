/**
* @file cc1101_regs.h
* @brief Registers definition for CC1101 transceiver
*
* @author Oleksii Slabchenko, alexslabchenko@gmail.com
*/

#ifndef CC1101_REGS_H
#define CC1101_REGS_H

/****** Registers *****************/
#define CC1101_REG_IOCFG2           0x00
#define CC1101_REG_IOCFG1           0x01
#define CC1101_REG_IOCFG0           0x02
#define CC1101_REG_FIFOTHR          0x03
#define CC1101_REG_SYNC1            0x04
#define CC1101_REG_SYNC0            0x05
#define CC1101_REG_PKTLEN           0x06
#define CC1101_REG_PKTCTRL1         0x07
#define CC1101_REG_PKTCTRL0         0x08
#define CC1101_REG_ADDR             0x09
#define CC1101_REG_CHANNR           0x0A
#define CC1101_REG_FSCTRL1          0x0B
#define CC1101_REG_FSCTRL0          0x0C
#define CC1101_REG_FREQ2            0x0D
#define CC1101_REG_FREQ1            0x0E
#define CC1101_REG_FREQ0            0x0F
#define CC1101_REG_MDMCFG4          0x10
#define CC1101_REG_MDMCFG3          0x11
#define CC1101_REG_MDMCFG2          0x12
#define CC1101_REG_MDMCFG1          0x13
#define CC1101_REG_MDMCFG0          0x14
#define CC1101_REG_DEVIATN          0x15
#define CC1101_REG_MCSM2            0x16
#define CC1101_REG_MCSM1            0x17
#define CC1101_REG_MCSM0            0x18
#define CC1101_REG_FOCCFG           0x19
#define CC1101_REG_BSCFG            0x1A
#define CC1101_REG_AGCCTRL2         0x1B
#define CC1101_REG_AGCCTRL1         0x1C
#define CC1101_REG_AGCCTRL0         0x1D
#define CC1101_REG_WOREVT1          0x1E
#define CC1101_REG_WOREVT0          0x1F
#define CC1101_REG_WORCTRL          0x20
#define CC1101_REG_FREND1           0x21
#define CC1101_REG_FREND0           0x22
#define CC1101_REG_FSCAL3           0x23
#define CC1101_REG_FSCAL2           0x24
#define CC1101_REG_FSCAL1           0x25
#define CC1101_REG_FSCAL0           0x26
#define CC1101_REG_RCCTRL1          0x27
#define CC1101_REG_RCCTRL0          0x28
#define CC1101_REG_FSTEST           0x29
#define CC1101_REG_PTEST            0x2A
#define CC1101_REG_AGCTEST          0x2B
#define CC1101_REG_TEST2            0x2C
#define CC1101_REG_TEST1            0x2D
#define CC1101_REG_TEST0            0x2E
#define CC1101_REG_PARTNUM          0x30
#define CC1101_REG_VERSION          0x31
#define CC1101_REG_FREQTEST         0x32
#define CC1101_REG_LQI              0x33
#define CC1101_REG_RSSI             0x34
#define CC1101_REG_MARCSTATE        0x35
#define CC1101_REG_WORTIME1         0x36
#define CC1101_REG_WORTIME0         0x37
#define CC1101_REG_PKTSTATUS        0x38
#define CC1101_REG_VCO_VC_DAC       0x39
#define CC1101_REG_TXBYTES          0x3A
#define CC1101_REG_RXBYTES          0x3B
#define CC1101_REG_RCCTRL1_STATUS   0x3C
#define CC1101_REG_RCCTRL0_STATUS   0x3D
/****** Strobes *******************/
#define CC1101_STROBE_SRES      0x30    // Reset chip.
#define CC1101_STROBE_SFSTXON   0x31    // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1). If in RX (with CCA):
                                        // Go to a wait state where only the synthesizer is running (for quick RX / TX turnaround).
#define CC1101_STROBE_SXOFF     0x32    // Turn off crystal oscillator.
#define CC1101_STROBE_SCAL      0x33    // Calibrate frequency synthesizer and turn it off. SCAL can be strobed from IDLE mode without
                                        // setting manual calibration mode (MCSM0.FS_AUTOCAL=0)
#define CC1101_STROBE_SRX       0x34    // Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL=1.
#define CC1101_STROBE_STX       0x35    // In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL=1.
                                        // If in RX state and CCA is enabled: Only go to TX if channel is clear.
#define CC1101_STROBE_SIDLE     0x36    // Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable.
#define CC1101_STROBE_SWOR      0x38    // Start automatic RX polling sequence (Wake-on-Radio) as described in Section 19.5 if
                                        // WORCTRL.RC_PD=0.
#define CC1101_STROBE_SPWD      0x39    // Enter power down mode when CSn goes high.
#define CC1101_STROBE_SFRX      0x3A    // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states.
#define CC1101_STROBE_SFTX      0x3B    // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states.
#define CC1101_STROBE_SWORRST   0x3C    // Reset real time clock to Event1 value.
#define CC1101_STROBE_SNOP      0x3D    // No operation. May be used to get access to the chip status byte.
/****** PATABLE and FIFOs *********/
#define CC1101_REG_PATABLE      0x3E
#define CC1101_REG_RXTX_FIFO    0x3F

#endif // CC1101_REGS_H
