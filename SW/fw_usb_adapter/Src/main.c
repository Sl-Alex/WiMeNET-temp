/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    09-September-2013
  * @brief   RHID demo main file
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "pcd8544.h"
#include "delay_hw.h"
#include "wmn_driver.h"
#include "wmn_queue.h"
#include "WmnPacket.h"

/* Private variables ---------------------------------------------------------*/
__IO uint8_t PrevXferComplete = 1;

WmnPacket packet;
WmnPacket tx_packet;
wmn_queue_t rx_queue;
wmn_queue_t tx_queue;
uint8_t rx_cnt = 0;
extern volatile uint8_t tx_done;

/* Private functions ---------------------------------------------------------*/

void Delay(volatile uint32_t nCount) {
	for (; nCount != 0; nCount--);
}

// New packet has arrived, can be read via the function parameter
void wireless_rx_cb(WmnPacket * rx_packet)
{
    wmn_queue_write(&rx_queue, rx_packet, 1);
    rx_cnt++;
}

extern uint8_t usb_buff_tx[64];
extern uint8_t usb_tx;
/*******************************************************************************
* Function Name  : main.
* Description    : main routine.
* Input          : None.
* Output         : None.
* Return         : ************************************************************/
int main(void)
{
    uint8_t x, y;
    static char tstr[32];
    GPIO_InitTypeDef GPIO_InitStructure;

   Set_System();

  USB_Interrupts_Config();

  Set_USBClock();

  USB_Init();

    //CCat* cat = getCat(20);
    //setCatAge(cat, 100);


    delay_hw_init();
    wmn_driver_set_rx_callback(wireless_rx_cb);
    wmn_driver_init();
    wmn_queue_init(&rx_queue);
    wmn_queue_init(&tx_queue);

  SetEPRxStatus(ENDP2, EP_RX_VALID);

    GPIO_InitStructure.GPIO_Speed           = GPIO_Speed_50MHz;
    // Initialize GDO pins
	GPIO_InitStructure.GPIO_Pin 			= GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode 			= GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    delay_hw_ms(100);
    lcd8544_init();
    lcd8544_refresh();
    delay_hw_ms(500);
    lcd8544_clear();
  while (1)
  {

    if (bDeviceState == CONFIGURED)
    {
        #define RECEIVER
        #ifdef RECEIVER
        if (usb_tx == 0)
        {
            if (wmn_queue_read(&rx_queue, (WmnPacket *)&usb_buff_tx, 0))
            {
                for (y = 0; y < 8; y++)
                {
                    for (x = 0; x < 8; x++)
                    {
                       sprintf(tstr, "%2X", usb_buff_tx[8 * y + x]);
                        lcd8544_putstr(x*(7+3), y*6, tstr, 0, 1);
                    }
                }
                usb_tx = 1;
                USB_SIL_Write(EP1_IN, usb_buff_tx, 0x40);
                SetEPTxValid(ENDP1);
            }
        }
        if (tx_done)
        {
            if (wmn_queue_read(&tx_queue, &tx_packet, 0))
            {
                if (tx_packet.raw[0] == 0)
                {
                    GPIO_SetBits(GPIOC,GPIO_Pin_13);
                }
                else
                {
                    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
                }

                wmn_driver_transmit(&tx_packet);
            }
        }
            //sprintf(tstr,"Q %2u",rx_queue.count);
            //sprintf(tstr,"Q %2u CNT %3u",0,cnt);
            //lcd8544_putstr(0,33,tstr,0, 0);
            //sprintf(tstr,"RS %3i LQ %3u",-packet.packet.rssi, packet.packet.lqi);
            //lcd8544_putstr(0,40,tstr,0, 0);*/
            lcd8544_refresh();
        #endif
    }
  }
}

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
