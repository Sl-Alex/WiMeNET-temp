/**
* @file wmn_driver.c
* @brief CC1101 transceiver driver for WiMeNet
*
* @author Oleksii Slabchenko, alexslabchenko@gmail.com
*/

#include "wmn_driver.h"
#include "wmn_config.h"
#include "stm32f10x_spi.h"
#include "cc1101_regs.h"
#include "delay_hw.h"

// Maximum number of bytes per packet is 64 for CC1101
// But actual usable number is 62, because last 2 bytes
// contain LQI and RSSI values
#if WMN_CONFIG_PACKET_LENGTH > 62
    #error Maximum CC1101 packet length exceeded
#endif

static uint8_t rssi_raw;
static uint8_t lqi_raw;

static wmn_driver_rx_cb_t rx_callback;
static wmn_driver_tx_cb_t tx_callback;

static wmn_packet_t rx_packet;

static uint8_t  chip_status = 0;
// Rf settings for CC1101
static uint8_t dev_settings[] = {
    #include "cc1101_init_arr.c"
/*
    // 0x07 - Signal is asserted when a packet has been received with CRC OK and is de-asserted
    //    when the first byte is read from the RX FIFO.
    0x07,  // IOCFG2              GDO2 Output Pin Configuration
    // 0x2E - Tri-state
    0x2E,  // IOCFG1              GDO1 Output Pin Configuration
    // Asserts when sync word has been sent / received, and de-asserts at the end of the packet.
    0x06,  // IOCFG0              GDO0 Output Pin Configuration
    0x07,  // FIFOTHR             RX FIFO and TX FIFO Thresholds
    0xA7,  // SYNC1               Sync Word, High Byte
    0x58,  // SYNC0               Sync Word, Low Byte
    // 16 Bytes packets
    WMN_CONFIG_PACKET_LENGTH,     // PKTLEN              Packet Length
    // CRC_AUTOFLUSH and APPEND_STATUS are enabled, ADR_CHK is disabled
    0x0C,  // PKTCTRL1            Packet Automation Control
    // WHITE_DATA and CRC_EN are enabled, fixed size packets
    0x44,  // PKTCTRL0            Packet Automation Control
    0x00,  // ADDR                Device Address
    0x00,  // CHANNR              Channel Number

    //  Transceiver settings (freq, mod, dev, etc.)
    0x0C,  // FSCTRL1             Frequency Synthesizer Control
    0x00,  // FSCTRL0             Frequency Synthesizer Control
    0x10,  // FREQ2               Frequency Control Word, High Byte
    0xB1,  // FREQ1               Frequency Control Word, Middle Byte
    0x3B,  // FREQ0               Frequency Control Word, Low Byte
    0x2D,  // MDMCFG4             Modem Configuration
    0x3B,  // MDMCFG3             Modem Configuration
    0x13,  // MDMCFG2             Modem Configuration

    // MSK, 30/32 sync bits should be detected
    0x22,  // MDMCFG1             Modem Configuration
    // FEC is on, 4 preamble bytes
    0xF8,  // MDMCFG0             Modem Configuration
    0x62,  // DEVIATN             Modem Deviation Setting

    // Main state machine configuration
    0x07,  // MCSM2               Main Radio Control State Machine Configuration
    // CCA is always on, IDLE after TX, IDLE after RX
    0x00,  // MCSM1               Main Radio Control State Machine Configuration
    // Calibrate each time when RX->IDLE or TX->IDLE, XOSC startup ~149-155us
    0x18,  // MCSM0               Main Radio Control State Machine Configuration

    0x1D,  // FOCCFG              Frequency Offset Compensation Configuration
    0x1C,  // BSCFG               Bit Synchronization Configuration
    0xC7,  // AGCCTRL2            AGC Control
    0x00,  // AGCCTRL1            AGC Control
    0xB0,  // AGCCTRL0            AGC Control
    0x87,  // WOREVT1             High Byte Event0 Timeout
    0x6B,  // WOREVT0             Low Byte Event0 Timeout
    0xF8,  // WORCTRL             Wake On Radio Control
    0xB6,  // FREND1              Front End RX Configuration
    0x10,  // FREND0              Front End TX Configuration
    0xEA,  // FSCAL3              Frequency Synthesizer Calibration
    0x2A,  // FSCAL2              Frequency Synthesizer Calibration
    0x00,  // FSCAL1              Frequency Synthesizer Calibration
    0x1F,  // FSCAL0              Frequency Synthesizer Calibration
    0x41,  // RCCTRL1             RC Oscillator Configuration
    0x00   // RCCTRL0             RC Oscillator Configuration*/
};

static uint8_t PA_table[8] = {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define CC1101_HEADER_READ      (1 << 7) // Read (1) or write (0) access
#define CC1101_HEADER_BURST     (1 << 6) // Burst (1) or single (0) access
#define CC1101_ADDRESS_MASK     0x3F

// Pinout
// GDO0 - PA2  CC1101_GPIO
// GDO2 - PA3  CC1101_GPIO
// CSN  - PA4  CC1101_GPIO
// SCK  - PA5  CC1101_SPI
// SO   - PA6  CC1101_SPI
// SI   - PA7  CC1101_SPI
#define RCC_CC1101_GPIO     RCC_APB2Periph_GPIOA
#define RCC_CC1101_SPI      RCC_APB2Periph_SPI1

#define CC1101_SPI          SPI1

#define CC1101_SPI_PRESCALER    SPI_BaudRatePrescaler_16     ///< 6.5MHz MAX

#define CC1101_GPIO_GPIO    GPIOA
#define CC1101_GPIO_SPI     GPIOA

// Pins definitions
#define CC1101_PIN_GDO0     GPIO_Pin_2  // PA2
#define CC1101_PIN_GDO2     GPIO_Pin_3  // PA3
#define CC1101_PIN_CSN      GPIO_Pin_4  // PA4
#define CC1101_PIN_SCK      GPIO_Pin_5  // PA5
#define CC1101_PIN_SO       GPIO_Pin_6  // PA6
#define CC1101_PIN_SI       GPIO_Pin_7  // PA7

#define GDO2_IRQHandler EXTI3_IRQHandler
#define GDO2_IRQLine    EXTI_Line3
#define GDO2_PortSource GPIO_PortSourceGPIOA
#define GDO2_PinSource  GPIO_PinSource3
#define GDO2_IRQn       EXTI3_IRQn

#define GDO0_IRQHandler EXTI2_IRQHandler
#define GDO0_IRQLine    EXTI_Line2
#define GDO0_PortSource GPIO_PortSourceGPIOA
#define GDO0_PinSource  GPIO_PinSource3
#define GDO0_IRQn       EXTI2_IRQn

static void spi_init(void);
static void spi_select(void);
static void spi_select_wait(void);
static void spi_deselect(void);
static uint8_t spi_transfer(uint8_t data);
static void powerup_reset(void);
static uint8_t strobe(uint8_t strobe);
static void read(uint8_t address, uint8_t * data, uint8_t length);
static void write(uint8_t address, uint8_t * data, uint8_t length);
static void driver_irq_setup(void);

/// @brief Initialize CC1101
void wmn_driver_init(void)
{
    // Initialize SPI
    spi_init();
    // Perform power-up reset
    powerup_reset();

    // Replace packet length with the correct length
    dev_settings[CC1101_REG_PKTLEN] = WMN_CONFIG_PACKET_LENGTH;
    // Should be in IDLE state now (see chapter 19.1.1)
    // Transfer configuration data
    write(0x00, &dev_settings[0], sizeof(dev_settings));
    // Transfer PA table
    write(CC1101_REG_PATABLE, &PA_table[0], sizeof(PA_table));
    // Default mode is RX
    strobe(CC1101_STROBE_SCAL);
    delay_hw_ms(1);
    strobe(CC1101_STROBE_SRX);
    driver_irq_setup();
    wmn_driver_irq_enable();
}

/// @brief Driver deinitialization
/// @details Does nothing for the embedded driver
void wmn_driver_deinit(void)
{

}

/// @brief Switch driver to the sleep mode
/// @details Does nothing for the embedded driver
void wmn_driver_sleep(void)
{

}

/// @brief Switch driver to the active mode
/// @details Does nothing for the embedded driver
void wmn_driver_wake(void)
{

}

/// @brief Set reception callback (new packet has arrived and can be processed)
void wmn_driver_set_rx_callback(wmn_driver_rx_cb_t cb)
{
    rx_callback = cb;
}

/// @brief Set transmission callback (last transmission is done, new packet can be sent)
void wmn_driver_set_tx_callback(wmn_driver_tx_cb_t cb)
{
    tx_callback = cb;
}

/// @brief Write bytes to CC1101, starting from specified address
static void write(uint8_t address, uint8_t * data, uint8_t length)
{
    uint8_t header = address & 0x3F;

    if (length == 0)
        return;

    if (length > 1)
        header |= CC1101_HEADER_BURST;

    spi_select_wait();
    spi_transfer(header);

    while (length > 0)
    {
        spi_transfer(*data);
        data++;
        length--;
    }

    spi_deselect();
}

static void spi_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	// Enable clock for GPIO pins (CSN, GDO0, GDO2)
	RCC_APB2PeriphClockCmd(RCC_CC1101_GPIO, ENABLE);
	// Enable clock for SPI pins
	RCC_APB2PeriphClockCmd(RCC_CC1101_SPI, ENABLE);

    // Initialize pins
    GPIO_InitStructure.GPIO_Speed           = GPIO_Speed_50MHz;
    // Initialize GDO pins
	GPIO_InitStructure.GPIO_Pin 			= CC1101_PIN_GDO0 | CC1101_PIN_GDO2;
	GPIO_InitStructure.GPIO_Mode 			= GPIO_Mode_IPU;
	GPIO_Init(CC1101_GPIO_GPIO, &GPIO_InitStructure);
    // Initialize GPIO pins
	GPIO_InitStructure.GPIO_Pin 			= CC1101_PIN_CSN;
	GPIO_InitStructure.GPIO_Mode 			= GPIO_Mode_Out_PP;
	GPIO_Init(CC1101_GPIO_GPIO, &GPIO_InitStructure);
    // Initialize SPI pins
	GPIO_InitStructure.GPIO_Pin 			= CC1101_PIN_SCK | CC1101_PIN_SI;
	GPIO_InitStructure.GPIO_Mode 			= GPIO_Mode_AF_PP;
	GPIO_Init(CC1101_GPIO_SPI, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 			= CC1101_PIN_SO;
	GPIO_InitStructure.GPIO_Mode 			= GPIO_Mode_IPU;
	GPIO_Init(CC1101_GPIO_SPI, &GPIO_InitStructure);

    SPI_Cmd(CC1101_SPI, DISABLE);

	SPI_InitStructure.SPI_Mode        		= SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction   		= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize 			= SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL 				= SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA 				= SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS 				= SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = CC1101_SPI_PRESCALER;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;

	SPI_Init(CC1101_SPI, &SPI_InitStructure);
	SPI_Cmd(CC1101_SPI, ENABLE);
}

static void spi_select(void)
{
    GPIO_ResetBits(CC1101_GPIO_GPIO, CC1101_PIN_CSN);
}

static void spi_select_wait(void)
{
    spi_select();
    while (GPIO_ReadInputDataBit(CC1101_GPIO_SPI, CC1101_PIN_SO) != 0);
}

static void spi_deselect(void)
{
    GPIO_SetBits(CC1101_GPIO_GPIO, CC1101_PIN_CSN);
}

static uint8_t spi_transfer(uint8_t data)
{
	//while(SPI_I2S_GetFlagStatus(CC1101_SPI, SPI_I2S_FLAG_RXNE) == SET)
	//    SPI_I2S_ReceiveData(CC1101_SPI);

	SPI_I2S_SendData(CC1101_SPI, data);
	while(SPI_I2S_GetFlagStatus(CC1101_SPI, SPI_I2S_FLAG_TXE) == RESET);
	while(SPI_I2S_GetFlagStatus(CC1101_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(CC1101_SPI);
}

/// @brief send strobe
static uint8_t strobe(uint8_t strobe)
{
    uint8_t ret;
    spi_select_wait();
    ret = spi_transfer(strobe);
    spi_deselect();
    return ret;
}

/// @brief Perform power-up reset
static void powerup_reset(void)
{
    spi_select();
    delay_hw_us(1);
    spi_deselect();
    delay_hw_us(50);
    strobe(CC1101_STROBE_SRES);
}

void wmn_driver_transmit(wmn_packet_t * packet)
{
    write(CC1101_REG_RXTX_FIFO, (uint8_t *)packet, WMN_CONFIG_PACKET_LENGTH);
    strobe(CC1101_STROBE_STX);
    /// @todo Implement waiting if necessary
    // Wait for TX finish (check GDO0)
}

uint8_t wmn_driver_receive(wmn_packet_t * packet, uint16_t timeout)
{
    uint8_t rssi_dec;
    int16_t rssi_dBm;
    uint8_t rssi_offset = 74; // for 100kbit @868MHz

    delay_hw_us_clear();
    strobe(CC1101_STROBE_SRX);
    while (delay_hw_us_is_elapsed(timeout) == 0)
    {
        // If GDO2 is 1 then read data and return 1;
        /// @todo Rewrite using interrupts
        /// Interrupt should be configured to the rising edge
        /// In the interrupt we read the packet as long as the input is high
        if (GPIO_ReadInputDataBit(CC1101_GPIO_GPIO, CC1101_PIN_GDO2) != 0)
        {
            read(CC1101_REG_RXTX_FIFO, (uint8_t *)packet, WMN_CONFIG_PACKET_LENGTH);
            rssi_dec = rssi_raw;
            if (rssi_dec >= 128)
                rssi_dBm = (int16_t)((int16_t)(rssi_dec - 256) / 2) - rssi_offset;
            else
                rssi_dBm = (rssi_dec / 2) - rssi_offset;
//            packet->packet.rssi = rssi_raw + 0x80; // Converting range -128..127 to range 0..255
            packet->packet.rssi = -rssi_dBm; // Converting range -128..127 to range 0..255
            packet->packet.lqi = lqi_raw & 0x7F;   // Remove CRC_OK field
            return 1;
        }
    }
    return 0;
}

/// @brief Read data from CC1101
/// @param address - Register address
/// @param data - pointer to data buffer to read to
/// @param length - amount of data to read
static void read(uint8_t address, uint8_t * data, uint8_t length)
{
    uint8_t header = address & 0x3F;

    header |= CC1101_HEADER_READ;
    header |= CC1101_HEADER_BURST;

    if (length == 0)
        return;

    spi_select_wait();
    chip_status = spi_transfer(header);

    // Read data
    while (length > 0)
    {
        *data = spi_transfer(0);
        data++;
        length--;
    }
    // If we have to read extra data (RSSI and LQI values in the end of the received packet)
    if (address == CC1101_REG_RXTX_FIFO)
    {
        rssi_raw = spi_transfer(0);
        lqi_raw  = spi_transfer(0);
    }

    spi_deselect();
}

static void driver_irq_setup(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);

    // Connect GDO0 Line to EXTI */
    GPIO_EXTILineConfig(GDO0_PortSource, GDO0_PinSource);
    // Connect GDO2 Line to EXTI */
    GPIO_EXTILineConfig(GDO2_PortSource, GDO2_PinSource);

    // Both GDO lines should generate interrupt on rising edge
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;

    // Configure GDO0 line with the same parameters
    EXTI_InitStructure.EXTI_Line = GDO0_IRQLine;
    EXTI_Init(&EXTI_InitStructure);

    // Configure GDO2 line for interrupt
    EXTI_InitStructure.EXTI_Line = GDO2_IRQLine;
    EXTI_Init(&EXTI_InitStructure);
}

void wmn_driver_irq_disable()
{
    NVIC_InitTypeDef   NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = GDO2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void wmn_driver_irq_enable()
{
    NVIC_InitTypeDef   NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = GDO2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// GDO2 interrupt
// Receive is complete, data are ready
void GDO2_IRQHandler(void)
{
    uint8_t rssi_dec;
    int16_t rssi_dBm;
    uint8_t rssi_offset = 74; // for 100kbit @868MHz

    static uint8_t tmp = 0;
    tmp = 1 - tmp;

    if(EXTI_GetITStatus(GDO2_IRQLine) != RESET)
    {
        /// @TODO Remove debug statement
        if (tmp == 0)
        {
           GPIO_SetBits(GPIOC,GPIO_Pin_13);
        }
        else
        {
           GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        }
        /* Clear the  EXTI line 3 pending bit */
        EXTI_ClearITPendingBit(GDO2_IRQLine);

        read(CC1101_REG_RXTX_FIFO, (uint8_t *)&rx_packet, WMN_CONFIG_PACKET_LENGTH);
        rssi_dec = rssi_raw;
        if (rssi_dec >= 128)
            rssi_dBm = (int16_t)((int16_t)(rssi_dec - 256) / 2) - rssi_offset;
        else
            rssi_dBm = (rssi_dec / 2) - rssi_offset;
//            packet->packet.rssi = rssi_raw + 0x80; // Converting range -128..127 to range 0..255
        rx_packet.packet.rssi = -rssi_dBm; // Converting range -128..127 to range 0..255
        rx_packet.packet.lqi = lqi_raw & 0x7F;   // Remove CRC_OK field
        if (rx_callback != 0)
            rx_callback(&rx_packet);
        // Check chip status for RX overflow
        if ((chip_status & 0x70) == 0x60)
        {
            // Flush RX buffer
            strobe(CC1101_STROBE_SFRX);
            // Switch to RX mode
            strobe(CC1101_STROBE_SRX);
        }
    }
}
