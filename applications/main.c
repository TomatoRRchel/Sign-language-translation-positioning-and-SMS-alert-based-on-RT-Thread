/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-06-29     RT-Thread    first version
 */
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "lcd_sample.h"
#include "uart_sample.h"
#include "AT_sample.h"
#include "adc_sample.h"

int main(void)
{  // rt_uint8_t i,j;float t;
    //char *ptr="�ܳϴ�";
   // rt_thread_mdelay(5000);
    lcd_sample();
    uart_sample();
    AT_sample();
    adc_sample();
   // while(1)
    //{
      /*  LCD_ShowChinese32x32(30+32,40,"��",RED,WHITE,32,0);
        LCD_ShowChinese32x32(30+64,40,"",RED,WHITE,32,0);*/

     /*   LCD_ShowChinese(30+32,40,"",RED,WHITE,32,0);
        LCD_ShowChinese(30+64,40,"",RED,WHITE,32,0);*/
     //  LCD_ShowString(12,64,"SCDDDDDDDDDDDDDDDDDDD",RED,WHITE,16,1);
       /* LCD_ShowIntNum(134,80,LCD_W,3,RED,WHITE,16);
        LCD_ShowString(32,100,"Increaseing Nun:",RED,WHITE,16,0);
        LCD_ShowFloatNum1(160,100,t,4,RED,WHITE,16);
        t+=0.11;*/

   //   LCD_ShowPicture(80,80,80,80,gImage_1);
     /*   for(j=0;j<2;j++)
        {
            for(i=0;i<3;i++)
            {
                LCD_ShowPicture(80*i,10+j*80,80,80,gImage_1);
            }
        }*/
   // }

    return RT_EOK;
}
