/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-02     Tomato       the first version
 */
#include "rtthread.h"
#include <rtdevice.h>
#include "drv_flash.h"
#include <lcd_sample.h>
#include <adc_sample.h>
#include <stdio.h>
#include <string.h>

#define STM32_FLASH_BASE_ADDR 0x8000000
#define DEST_ADDDR STM32_FLASH_BASE_ADDR+510*1024

rt_device_t dev_uart2;
struct serial_configure uart2_config;

rt_thread_t alarm_thread,AT_thread,phone_change,recv_thread;
rt_event_t alarm_event;
rt_mailbox_t AT_mb;
char recv_str[40]={0};
char recv_loc[50]={0};
char phone_number[11];//="18389212757";//="15779615671";
char flag0=0,index0=0,index_temp=0,count=0;
char flag_0=0,flag_1=0;

char sheng[]="½­";

rt_err_t uart_rcall2(rt_device_t dev, rt_size_t size){
    if(flag0) rt_event_send(alarm_event, 0x08);
 /*   char buffer1;
   if(flag0){
        rt_device_read(dev_uart2, 0, &buffer1, 1);
      //  recv_loc[index0++]=buffer1;
        if(buffer1==0x0a) count++;
        if(count==6) {
         //   rt_kprintf("count%d\n",count);
            //rt_device_write(dev_uart2, 0, "now recv\n", 9);
            recv_loc[index0++]=buffer1;
            rt_kprintf("%d--",buffer1);
        //    rt_event_send(alarm_event, 0x02);
        }//2 SHI GET+6
        //3 OK
       if(count==7){
           index_temp=index0;
           recv_loc[index0]='\0';
          for(char k=0;k<index_temp;k++){
                if(recv_loc[k]==0x0a||recv_loc[k]==0x0d) recv_loc[k]=0x41;
            }
           rt_kprintf("recv:%s\n",recv_loc);
           rt_kprintf("index:%d\n",index_temp);

        //rt_device_write(dev_uart2, 0, recv_loc, index_temp);
        flag0=0;
       }
    }*/
    return RT_EOK;
}

static void recv_entry(void *p){
   char buffer1;
    while(1){
        while(rt_device_read(dev_uart2, 0, &buffer1, 1)!=1){
        rt_event_recv(alarm_event, 0x08, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, 0);
        }
        rt_kprintf("buffer:%x\n",buffer1);
        if(flag_0&&flag_1){
            recv_loc[2+index0++]=buffer1;
            rt_kprintf("%x--",buffer1);
            if(buffer1==0x0a) {
                flag_1=0;flag_0=0;
                recv_loc[2+index0]='\0';index_temp=index0+2;index0=0;
                rt_kprintf("\nrecv:%s\n",recv_loc);
                rt_device_write(dev_uart2, 0, recv_loc, index_temp);
            }
        }
        else {
        if(buffer1==sheng[0])//0xbd)
            flag_0=1;
        if(flag_0&&(buffer1==sheng[1])){//0xad) {
            flag_1=1;recv_loc[0]=0xbd;recv_loc[1]=0xad;
            rt_kprintf("%x--%x--",recv_loc[0],recv_loc[1]);
        }
        }
    }
}

static void phone_change_entry(void *p){
    while(1){
        rt_event_recv(lcd_event, 0x04, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, 0);
        stm32_flash_erase(DEST_ADDDR, 11);
        stm32_flash_write(DEST_ADDDR,phone_real,11);
        strcpy(phone_number, phone_real);
        rt_kprintf("change number %s\n",phone_number);
    }
}
void AT_getloc(rt_uint8_t flag){
    memset(recv_loc,0,sizeof(recv_loc));
    rt_device_write(dev_uart2, 0,"AT+GTKEY=\"994aa7368555e73682493174b9b560b5\"\r\n",strlen("AT+GTKEY=\"994aa7368555e73682493174b9b560b5\"\r\n"));
    rt_thread_mdelay(100);
    flag0=flag;index0=0;count=0;
    rt_device_write(dev_uart2, 0,"AT+GTGIS=6\r\n",strlen("AT+GTGIS=6\r\n"));


}


static void alarm_entry(void *p){
    rt_uint8_t end=0x1a;
    char string[]={0xE4,0xBD,0xA0,0xE5,0xA5,0xBD};
    while(1){
        //  rt_device_write(dev_uart2, 0, "AT+GTTS=1,\"¹ãºÍÍ¨ \",1\r\n", rt_strlen("AT+GTTS=1,\"¹ãºÍÍ¨ \",1\r\n"));
        rt_event_recv(alarm_event, 0x01, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, 0);
     //   AT_getloc(1);
        rt_thread_mdelay(2000);
     //   rt_device_write(dev_uart2, 0, recv_loc, index_temp);
       //   rt_event_recv(alarm_event, 0x02, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, 0);
        rt_device_write(dev_uart2, 0, "AT+CAVIMS=1\r\n", rt_strlen("AT+CAVIMS=1\r\n"));
        rt_thread_mdelay(100);
        rt_device_write(dev_uart2, 0, "AT+CPMS=\"SM\"\r\n", rt_strlen("AT+CPMS=\"SM\"\r\n"));
        rt_thread_mdelay(100);
        rt_device_write(dev_uart2, 0, "AT+CNMI=2,1,0,0,0\r\n", rt_strlen("AT+CNMI=2,1,0,0,0\r\n"));
        rt_thread_mdelay(100);
        rt_device_write(dev_uart2, 0, "AT+CMGF=1\r\n", rt_strlen("AT+CMGF=1\r\n"));
        rt_thread_mdelay(100);
        rt_device_write(dev_uart2, 0, "AT+CSMP=17,167,0,0\r\n", rt_strlen("AT+CSMP=17,167,0,0\r\n"));
        rt_thread_mdelay(100);
        rt_device_write(dev_uart2, 0, "AT+CMGS=\"",rt_strlen("AT+CMGS=\""));
        rt_device_write(dev_uart2, 0,&phone_number[0],11);
        rt_device_write(dev_uart2, 0, "\"\r\n",rt_strlen("\"\r\n"));
        //    rt_device_write(dev_uart2, 0, "AT+CMGS=\"15779615671\"\r\n", rt_strlen("AT+CMGS=\"15779615671\"\r\n"));
        rt_thread_mdelay(100);
        rt_device_write(dev_uart2, 0, string, 6);
   //     rt_device_write(dev_uart2, 0, recv_loc, index_temp);
        //       rt_device_write(dev_uart2, 0,"help",rt_strlen("help"));
        //                rt_thread_mdelay(100);
        //rt_device_write(dev_uart2, 0,&end,1);
        rt_thread_mdelay(2000);
        rt_device_write(dev_uart2, 0,&end,1);

    }
}//15779615671

static void AT_entry(void *p){
    rt_uint8_t i;
    rt_uint8_t *recv_box;
    AT_getloc(0);
    while(1){
        memset(recv_str,0,sizeof(recv_str));
        rt_mb_recv(AT_mb, (rt_uint32_t *)&recv_box, RT_WAITING_FOREVER);
        rt_kprintf("AT mail get\n");
        for(i=0;recv_box[i]!='@';i++) {
            recv_str[i]=recv_box[i];
        }
        recv_str[i]='\0';
        rt_device_write(dev_uart2, 0,"AT+GTTS=1,\"",strlen("AT+GTTS=1,\""));
        rt_device_write(dev_uart2, 0,recv_str,strlen(recv_str));
        rt_device_write(dev_uart2, 0,"\",1\r\n",strlen("\",1\r\n"));
    }
}

int AT_sample(){
    rt_uint8_t result0;
  //  stm32_flash_erase(DEST_ADDDR, 11);
 //  stm32_flash_write(DEST_ADDDR,phone_real,11);
    stm32_flash_read(DEST_ADDDR ,phone_real,11);
    strcpy(phone_number, phone_real);
    //  char *p="ÙÜ³Ï´ïÄãºÃ";
    dev_uart2=rt_device_find("uart2");//ï¿½ï¿½ï¿½ï¿½ï¿½è±¸
    //  *p=rt_malloc(size)
    if(dev_uart2==RT_NULL) rt_kprintf("SearchFailed\n");
    result0=rt_device_open(dev_uart2, RT_DEVICE_FLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
    if(result0<0) rt_kprintf("openFailed\n");
    uart2_config.baud_rate=BAUD_RATE_115200;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    uart2_config.data_bits=DATA_BITS_8;//ï¿½ï¿½ï¿½ï¿½Î»
    uart2_config.stop_bits=STOP_BITS_1;//Í£Ö¹Î»
    uart2_config.parity=PARITY_NONE;//ï¿½ï¿½ï¿½È¼ï¿½
    uart2_config.bit_order=BIT_ORDER_LSB;
    uart2_config.invert=NRZ_NORMAL;
    uart2_config.bufsz=RT_SERIAL_RB_BUFSZ;
    uart2_config.reserved=0;//ï¿½ï¿½ï¿½Ã´ï¿½ï¿½Ú½á¹¹ï¿½ï¿½
    rt_device_control(dev_uart2, RT_DEVICE_CTRL_CONFIG, (void *)&uart2_config);
    rt_device_set_rx_indicate(dev_uart2, uart_rcall2);
    //    rt_device_write(dev_uart2, 0,p, rt_strlen(p));

    alarm_event=rt_event_create("alarm", RT_IPC_FLAG_FIFO);
    AT_mb=rt_mb_create("AT",48, RT_IPC_FLAG_FIFO);

    alarm_thread=rt_thread_create("alarm",alarm_entry, RT_NULL,1024, 15, 5);
    if(alarm_thread!=RT_NULL) rt_thread_startup(alarm_thread);

    AT_thread=rt_thread_create("AT",AT_entry, RT_NULL,1024, 19, 5);

    recv_thread=rt_thread_create("recv",recv_entry, RT_NULL,1024, 14, 5);
    if(recv_thread!=RT_NULL) rt_thread_startup(recv_thread);

    phone_change=rt_thread_create("phone_change",phone_change_entry, RT_NULL,512, 14, 5);
    if(AT_thread!=RT_NULL) rt_thread_startup(AT_thread);
    if(phone_change!=RT_NULL) rt_thread_startup(phone_change);
    rt_device_write(dev_uart2, 0,"uart2 working\r\n",strlen("uart2 working\r\n"));
    //rt_device_write(dev_uart2, 0,"½­Î÷Ê¡",4);
  //  AT_getloc(0);
    //  rt_event_send(alarm_event, 0x01);
    return RT_EOK;
}
