/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-01     Tomato       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <lcd_sample.h>
#include <adc_sample.h>
#include <AT_sample.h>

rt_device_t dev_uart3;
struct serial_configure uart3_config;
char p[50];
rt_uint8_t lenth=0;
rt_err_t uart_rcall(rt_device_t dev, rt_size_t size){
    char buffer0;
    if(page==1){
        rt_device_read(dev_uart3, 0, &buffer0, 1);
        p[lenth]=buffer0;
        if(p[lenth]=='@') {
            for(int i=0;p[i]!='@';i++)
                rt_kprintf("%c",p[i]);
            rt_kprintf("\n");
            lenth=0;
            rt_event_send(lcd_event, 0x01);
            rt_mb_send(lcd_mb,(rt_uint32_t *)&p);
            rt_mb_send(AT_mb,(rt_uint32_t *)&p);
            lenth--;
        }
        lenth++;
    }
    // rt_device_write(dev_uart3, 0,p, lenth);
}

int uart_sample(){
    rt_uint8_t result0;
    dev_uart3=rt_device_find("uart3");//查找设备
    //  *p=rt_malloc(size)
    if(dev_uart3==RT_NULL) rt_kprintf("SearchFailed\n");
    result0=rt_device_open(dev_uart3, RT_DEVICE_FLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
    if(result0<0) rt_kprintf("openFailed\n");
    uart3_config.baud_rate=BAUD_RATE_115200;//波特率
    uart3_config.data_bits=DATA_BITS_8;//数据位
    uart3_config.stop_bits=STOP_BITS_1;//停止位
    uart3_config.parity=PARITY_NONE;//优先级
    uart3_config.bit_order=BIT_ORDER_LSB;
    uart3_config.invert=NRZ_NORMAL;
    uart3_config.bufsz=RT_SERIAL_RB_BUFSZ;
    uart3_config.reserved=0;//配置串口结构体
    rt_device_control(dev_uart3, RT_DEVICE_CTRL_CONFIG, (void *)&uart3_config);
    rt_device_set_rx_indicate(dev_uart3, uart_rcall);
    rt_device_write(dev_uart3, 0,"uart3 working\n", rt_strlen("uart3 working\n"));
    return RT_EOK;
}


