/**
 ******************************************************************************
 * @file    usb_hal.c
 * @author  Satish Nair
 * @version V1.0.0
 * @date    05-Nov-2014
 * @brief   USB Virtual COM Port and HID device HAL
 ******************************************************************************
  Copyright (c) 2013-14 Spark Labs, Inc.  All rights reserved.

  Copyright 2012 STMicroelectronics
  http://www.st.com/software_license_agreement_liberty_v2

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_hal.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "delay_hal.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#ifdef USB_CDC_ENABLE
USB_OTG_CORE_HANDLE USB_OTG_dev;
#endif

/* Extern variables ----------------------------------------------------------*/
#ifdef USB_CDC_ENABLE
extern uint8_t USB_DEVICE_CONFIGURED;
extern uint8_t USB_Rx_Buffer[];
extern uint8_t APP_Rx_Buffer[];
extern uint32_t APP_Rx_ptr_in;
extern uint16_t USB_Rx_length;
extern uint16_t USB_Rx_ptr;
extern uint8_t  USB_Tx_State;
extern uint8_t  USB_Rx_State;
#endif

#if defined (USB_CDC_ENABLE) || defined (USB_HID_ENABLE)
/*******************************************************************************
 * Function Name  : SPARK_USB_Setup
 * Description    : Spark USB Setup.
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void SPARK_USB_Setup(void)
{
    USBD_Init(&USB_OTG_dev,
              USB_OTG_HS_CORE_ID,
              &USR_desc,
              &USBD_CDC_cb,
              &USR_cb);
}

/*******************************************************************************
 * Function Name  : Get_SerialNum.
 * Description    : Create the serial number string descriptor.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void Get_SerialNum(void)
{
    //Not required. Retained for compatibility
}
#endif

#ifdef USB_CDC_ENABLE
/*******************************************************************************
 * Function Name  : USB_USART_Init
 * Description    : Start USB-USART protocol.
 * Input          : baudRate.
 * Return         : None.
 *******************************************************************************/
void USB_USART_Init(uint32_t baudRate)
{
    SPARK_USB_Setup();
}

/*******************************************************************************
 * Function Name  : USB_USART_Available_Data.
 * Description    : Return the length of available data received from USB.
 * Input          : None.
 * Return         : Length.
 *******************************************************************************/
uint8_t USB_USART_Available_Data(void)
{
    if(USB_DEVICE_CONFIGURED)
    {
        if(USB_Rx_State == 1)
        {
            return (USB_Rx_length - USB_Rx_ptr);
        }
    }

    return 0;
}

/*******************************************************************************
 * Function Name  : USB_USART_Receive_Data.
 * Description    : Return data sent by USB Host.
 * Input          : None
 * Return         : Data.
 *******************************************************************************/
int32_t USB_USART_Receive_Data(void)
{
    if(USB_DEVICE_CONFIGURED)
    {
        if(USB_Rx_State == 1)
        {
            if((USB_Rx_length - USB_Rx_ptr) == 1)
            {
                USB_Rx_State = 0;

                /* Prepare Out endpoint to receive next packet */
                //To Do
            }

            return USB_Rx_Buffer[USB_Rx_ptr++];
        }
    }

    return -1;
}

/*******************************************************************************
 * Function Name  : USB_USART_Send_Data.
 * Description    : Send Data from USB_USART to USB Host.
 * Input          : Data.
 * Return         : None.
 *******************************************************************************/
void USB_USART_Send_Data(uint8_t Data)
{
    if(USB_DEVICE_CONFIGURED)
    {
        APP_Rx_Buffer[APP_Rx_ptr_in] = Data;

        APP_Rx_ptr_in++;

        /* To avoid buffer overflow */
        if(APP_Rx_ptr_in == APP_RX_DATA_SIZE)
        {
            APP_Rx_ptr_in = 0;
        }

        //Delay 100us to avoid losing the data
        HAL_Delay_Microseconds(100);
    }
}
#endif

#ifdef USB_HID_ENABLE
/*******************************************************************************
 * Function Name : USB_HID_Send_Report.
 * Description   : Send HID Report Info to Host.
 * Input         : pHIDReport and reportSize.
 * Output        : None.
 * Return value  : None.
 *******************************************************************************/
void USB_HID_Send_Report(void *pHIDReport, size_t reportSize)
{
    //To Do
}
#endif

