#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <lcd_sample.h>
#include <lcd.h>
#include <string.h>
#include <lcd_init.h>
#include <AT_sample.h>

#define CHANNAL5 5
#define CHANNAL4 4
#define KEY GET_PIN(A,6)

rt_adc_device_t adc_dev=RT_NULL;
rt_thread_t adc_thread,key_thread;
rt_uint8_t page0_index=1,page2_index=0,page=0,show_page=0;
rt_uint8_t key_val=0,key_old=0,key_temp=0,key_down;
rt_int8_t phone[11],phone_index;
rt_int8_t phone_real[11]="15779615671";//{0};//"18174167698";
rt_uint8_t exit_count=0;
rt_int8_t end='@',reflag=0,return_flag=0,follow=1,show_flag=1;

static void key_entry(void *p){
    while(1)
    {
        if(rt_pin_read(KEY)==0x00){
            key_temp=1;
        }
        else key_temp=0;
        key_val = key_temp;
        key_down = key_val & (key_old ^ key_val);//²¶×½°´¼üÏÂ½µÑØ
        key_old = key_val;
        if(key_down==1)
        {
            if(page0_index==1){
                if(page==1) {
                    page=0;
                    show_flag=1;
                    return_flag=1;
                    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
                    rt_mb_send(lcd_mb, (rt_uint32_t *)&end);
                }
                else {
                    return_flag=0;
                    reflag=1;
                    page=1;
                    LCD_Fill(0, 0,LCD_W, LCD_H, BROWN);
                    rt_mb_send(lcd_mb, (rt_uint32_t *)&end);
                }
            }
            if(page0_index==2){
                if(page==2) {
                    if(page2_index){
                        page2_index=0;
                        rt_event_send(alarm_event, 0x01);
                    }
                    page=0;
                    show_flag=1;
                    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
                }
                else {
                    page=2;
                    LCD_Fill(50,160-45, 190, 160+45, BLACK);
                }
            }
            if(page0_index==3){
                if(page==3) {
                    page=6;
                    show_flag=1;
                    strcpy(phone_real,phone);
                    rt_event_send(lcd_event, 0x04);
                }
                else {
                    page=3;
                    for(unsigned char i=0;i<11;i++) {
                        phone[i]=phone_real[i];
                        phone_index=0;
                    }
                    LCD_Fill(0, 0,LCD_W, LCD_H, BLACK);
                }
            }
            rt_event_send(lcd_event, 0x01);
        }
        rt_thread_mdelay(15);
    }
}

static void adc_entry(void *par){
    rt_uint16_t adc_datax,adc_datay;
    while(1){
        adc_datax=rt_adc_read(adc_dev, CHANNAL5);
        adc_datay=rt_adc_read(adc_dev, CHANNAL4);
        //        if(choic2!=1){
        if(adc_datay==4095) {//DOWM
            if(page==0){
                if(++page0_index==4) page0_index=1;
            }
            if(page==1){
                if(show_page<Page) {
                    show_page++;
                    if(pageflag) follow=((show_page==Page-1)?1:0);
                    else follow=((show_page==Page)?1:0);
                //    rt_kprintf("follow:%d\n",follow);
                    rt_event_send(lcd_event, 0x01);
                    rt_mb_send(lcd_mb, (rt_uint32_t *)&end);
                    LCD_Fill(6+2,20+5,240-10,320-20-16-5,BROWN);
                    rt_thread_mdelay(300);
                }
            }
            if(page==3){
                exit_count=0;
                if(--phone[phone_index]=='/')     phone[phone_index]='9';
            }
            rt_event_send(lcd_event, 0x01);
            rt_thread_mdelay(300);
        }
        if(adc_datay==0) {//UP
            if(page==0){
                if(--page0_index==0) page0_index=3;
            }
            if(page==1){
                if(show_page>0) {
                    show_page--;
                    follow=0;
                    rt_kprintf("follow:%d\n",follow);
                    rt_event_send(lcd_event, 0x01);
                    rt_mb_send(lcd_mb, (rt_uint32_t *)&end);
                    LCD_Fill(6+2,20+5,240-10,320-20-16-5,BROWN);
                    rt_thread_mdelay(300);
                }
            }
            if(page==3){
                exit_count=0;
                if(++phone[phone_index]==':')     phone[phone_index]='0';
            }
            rt_event_send(lcd_event, 0x01);
            rt_thread_mdelay(300);
        }
        //     }
        if(adc_datax==4095){ //LEFT
            if(page==2)   page2_index^=1;
            if(page==3){
                if(++exit_count==2){
                    exit_count=0;
                    page=0;
                    show_flag=1;
                    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
                }
            }
            rt_event_send(lcd_event, 0x01);
            rt_thread_mdelay(300);
        }
        if(adc_datax==0){ //RIGHT
            if(page==2) page2_index^=1;
            if(page==3){
                exit_count=0;
                if(++phone_index==11) phone_index=0;
            }
            rt_event_send(lcd_event, 0x01);
            rt_thread_mdelay(300);
        }
        rt_thread_mdelay(200);
    }
}
///max4095 0
int adc_sample(){
    adc_dev=rt_device_find("adc1");
    if(adc_dev==RT_NULL) rt_kprintf("AdcSearchFail\n");
    rt_adc_enable(adc_dev, CHANNAL5);
    rt_adc_enable(adc_dev, CHANNAL4);
    adc_thread=rt_thread_create(
            "adc", adc_entry, RT_NULL, 512, 18, 5);
    rt_thread_startup(adc_thread);

    rt_pin_mode(KEY, PIN_MODE_INPUT_PULLUP);
    key_thread=rt_thread_create("key",key_entry, RT_NULL, 512, 17, 5);
    rt_thread_startup(key_thread);
    return RT_EOK;
}
