/**
  ******************************************************************************
  * @file    usb_endp.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Endpoint routines
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_bot.h"
#include "usb_istr.h"

#include "wmn_queue.h"
#include "WmnPacket.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    : EP1 IN Callback Routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t usb_buff_rx[0x40];
uint8_t usb_buff_tx[0x40];

extern wmn_queue_t rx_queue;
extern wmn_queue_t tx_queue;

volatile uint8_t usb_tx = 0;

void EP1_IN_Callback(void)
{
    //Bulk_Data_Buff[0]++;
    /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
//    USB_SIL_Write(EP1_IN, Bulk_Data_Buff, 0x40);
    /* Enable endpoint for transmission */
    if (wmn_queue_read(&rx_queue, (WmnPacket *)&usb_buff_tx, 1))
    {
        USB_SIL_Write(EP1_IN, usb_buff_tx, 0x40);
        SetEPTxValid(ENDP1);
        usb_tx = 1;
    }
    else
    {
        usb_tx = 0;
    }
}

/*******************************************************************************
* Function Name  : EP2_OUT_Callback.
* Description    : EP2 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_OUT_Callback(void)
{
  //BitAction Led_State;

    // Read received data
    USB_SIL_Read(EP2_OUT, usb_buff_rx);

    // Write to the queue
    wmn_queue_write(&tx_queue, (WmnPacket *)&usb_buff_rx, 1);

    SetEPRxStatus(ENDP2, EP_RX_VALID);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

