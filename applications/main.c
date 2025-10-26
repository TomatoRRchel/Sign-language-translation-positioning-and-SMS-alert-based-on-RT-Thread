/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"

#include "AT_sample.h"
#include "mpu_app.h"
#include "adc_sample.h"
#include <lcd_sample.h>
/*
#define ADC_DEV_NAME        "adc1"
#define ADC_DEV_CHANNEL     0
#define REFER_VOLTAGE       330
#define CONVERT_BITS        (1 << 16)


#define LED_PIN GET_PIN(I, 8)
   rt_uint16_t value1,value2;
   rt_adc_device_t adc_dev;
int adc_vol()
{


    rt_err_t ret = RT_EOK;

    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }


    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);
    ret = rt_adc_enable(adc_dev, 9);



   // vol = value * REFER_VOLTAGE / CONVERT_BITS;
  //  rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);


  //  ret = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);

 //   return ret;
}
*/
int main(void)
{
    adc_sample();
    lcd_sample();
    AT_sample();
    mpu_sample();
    /*rt_uint32_t count = 1;
    LCD_Init();
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
    LCD_ShowString(120,180,"FIGHT",RED,BLACK,32,1);
    adc_vol();
    while(1)
    {
        rt_thread_mdelay(1000);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(1000);
        rt_pin_write(LED_PIN, PIN_LOW);
        value1 = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
        value2 = rt_adc_read(adc_dev, 9);
        rt_kprintf("the value1 is :%d \n", value1);
        rt_kprintf("the value2 is :%d \n", value2);
    }
*/
    return RT_EOK;
}

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);


