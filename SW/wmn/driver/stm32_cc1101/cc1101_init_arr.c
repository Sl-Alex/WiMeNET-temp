// CC1101 RF settings array
    0x07,  // IOCFG2              GDO2 Output Pin Configuration
    0x2E,  // IOCFG1              GDO1 Output Pin Configuration
    0x1B,  // IOCFG0              GDO0 Output Pin Configuration
    0x07,  // FIFOTHR             RX FIFO and TX FIFO Thresholds
    0xA7,  // SYNC1               Sync Word, High Byte
    0x58,  // SYNC0               Sync Word, Low Byte
    0x12,  // PKTLEN              Packet Length
    0x0C,  // PKTCTRL1            Packet Automation Control
    0x44,  // PKTCTRL0            Packet Automation Control
    0x00,  // ADDR                Device Address
    0x00,  // CHANNR              Channel Number
    0x08,  // FSCTRL1             Frequency Synthesizer Control
    0x00,  // FSCTRL0             Frequency Synthesizer Control
    0x21,  // FREQ2               Frequency Control Word, High Byte
    0x62,  // FREQ1               Frequency Control Word, Middle Byte
    0x76,  // FREQ0               Frequency Control Word, Low Byte
    0x5B,  // MDMCFG4             Modem Configuration
    0xF8,  // MDMCFG3             Modem Configuration
    0x13,  // MDMCFG2             Modem Configuration
    0x22,  // MDMCFG1             Modem Configuration
    0xF8,  // MDMCFG0             Modem Configuration
    0x47,  // DEVIATN             Modem Deviation Setting
    0x07,  // MCSM2               Main Radio Control State Machine Configuration
    0x2F,  // MCSM1               Main Radio Control State Machine Configuration
    0x18,  // MCSM0               Main Radio Control State Machine Configuration
    0x1D,  // FOCCFG              Frequency Offset Compensation Configuration
    0x1C,  // BSCFG               Bit Synchronization Configuration
    0xC7,  // AGCCTRL2            AGC Control
    0x00,  // AGCCTRL1            AGC Control
    0xB2,  // AGCCTRL0            AGC Control
    0x87,  // WOREVT1             High Byte Event0 Timeout
    0x6B,  // WOREVT0             Low Byte Event0 Timeout
    0xFB,  // WORCTRL             Wake On Radio Control
    0xB6,  // FREND1              Front End RX Configuration
    0x10,  // FREND0              Front End TX Configuration
    0xEA,  // FSCAL3              Frequency Synthesizer Calibration
    0x2A,  // FSCAL2              Frequency Synthesizer Calibration
    0x00,  // FSCAL1              Frequency Synthesizer Calibration
    0x1F,  // FSCAL0              Frequency Synthesizer Calibration
    0x41,  // RCCTRL1             RC Oscillator Configuration
    0x00,  // RCCTRL0             RC Oscillator Configuration
    0x59,  // FSTEST              Frequency Synthesizer Calibration Control
    0x7F,  // PTEST               Production Test
    0x3F,  // AGCTEST             AGC Test
    0x88,  // TEST2               Various Test Settings
    0x31,  // TEST1               Various Test Settings
    0x09,  // TEST0               Various Test Settings
    0x00,  // PARTNUM             Chip ID
    0x04,  // VERSION             Chip ID
    0x00,  // FREQEST             Frequency Offset Estimate from Demodulator
    0x00,  // LQI                 Demodulator Estimate for Link Quality
    0x00,  // RSSI                Received Signal Strength Indication
    0x00,  // MARCSTATE           Main Radio Control State Machine State
    0x00,  // WORTIME1            High Byte of WOR Time
    0x00,  // WORTIME0            Low Byte of WOR Time
    0x00,  // PKTSTATUS           Current GDOx Status and Packet Status
    0x00,  // VCO_VC_DAC          Current Setting from PLL Calibration Module
    0x00,  // TXBYTES             Underflow and Number of Bytes
    0x00,  // RXBYTES             Overflow and Number of Bytes
    0x00,  // RCCTRL1_STATUS      Last RC Oscillator Calibration Result
    0x00,  // RCCTRL0_STATUS      Last RC Oscillator Calibration Result
// End of the CC1101 RF settings array