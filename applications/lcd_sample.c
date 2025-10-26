#include <pic.h>
#include <lcd_init.h>
#include <lcd.h>
#include <adc_sample.h>
#include "stdlib.h"
#include "string.h"
rt_thread_t lcd_thread;
rt_mailbox_t lcd_mb;
rt_event_t lcd_event;
rt_int8_t lcd_recv[40]={0},len[100]={0},*show[100]={NULL},len_sum=0,len_sum_temp;;
lcd_page_t lcd_page[page_num];
lcd_object_t *lcd_object[page_num*6],*t;
rt_uint8_t count0=0;
//lcd_object_t *t;
rt_uint8_t pageflag=0,Page=0,show_index=0,show_delete=0;
void str_cpy(char *str1,char *str2){
    while(*str2!='\0'){
        *str1=*str2;
        str1++;str2++;
    }
    *str1='\0';
}
rt_size_t str_len(char *s){
    rt_size_t len=0;
    while(*s!='\0'){
        len++;
        s++;
        if(*s==0xa0) len-=3;
    }
    return len;
}
rt_uint8_t tcb_init(lcd_page_t *L){
    L->free=6;
    L->head.addr=NULL;
    L->head.index=0;
    L->head.loc=0;
    L->head.len=0;
    L->head.next=NULL;
    L->head.pre=NULL;
    return RT_EOK;
}

rt_uint8_t open_cartoon(){//120-249616
    int a,d=0;
 /*   int a,b,c,d=0;rt_uint8_t flag=0;//240*320 120 180
    for(c=0;c<16;c++){
        LCD_DrawRectangle(120-18-6*c,160-24-9*c,120+18+6*c,160+24+9*c,WHITE);
        rt_kprintf("%d\n",c);
    }
    for(c=16;c;c--){
        LCD_DrawRectangle(120-18-6*c,160-24-9*c,120+18+6*c,160+24+9*c,BLACK);
    }
    a=36;
    for(b=0;b<=120-18;b++){
        LCD_DrawRectangle(120-18-b,160-24-b,120+18+b,160+24+b,WHITE);//36 48
        LCD_DrawPoint(120+18+b,160+24+b,WHITE);
        if(b==48) flag=1;
        if(flag){
            a--;
            LCD_DrawLine(120-18,160-24,120-18+a,160+24,WHITE);
        }
      //  if(a==0) LCD_ShowPicture(x, y, length, width, pic)
        else
        {
            LCD_DrawLine(120-18,160-24,120+18,160-24+b,WHITE);
        }
    }*/
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
    while(1)    //120-50
    {  // Display_Arial_String(120-34-16*4,150-34+10-5-32,16,"loading...",BLACK, WHITE);
        LCD_ShowString(120-34, 150-34+10-5-16,"loading...",BLACK, WHITE, 16, 0);
        LCD_DrawRectangle(68,180+20-1,69+102,180+20+7,BLACK);//199
        for(a=0;a<10;a++)
        {   d++;
        LCD_ShowPicture(120-34, 150-34+10-5, 68, 68, gImage[a]);//184
        rt_thread_mdelay(10);
        LCD_DrawLine(69+d,180+21,69+d,180+21+5,BLACK);
        LCD_ShowIntNum(96, 180+20+9, d, 3, BLACK, WHITE, 16);
        LCD_ShowChar(120+8,180+20+9,'%', BLACK, WHITE, 16, 0);
        if(d==100)  return 1;
        }
    }
}

//char xing[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
static void lcd_entry(void *p){
    rt_uint8_t *recv_box;
    lcd_object_t *temp;
    char i=0;
    LCD_Init();
    for(int b=0;b<page_num;b++) tcb_init(&lcd_page[b]); //
 //   LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    open_cartoon();
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
    while(1)    //120-50
    {
        rt_event_recv(lcd_event, 0x01, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, 0);
       // rt_kprintf("page %d\n",page);
        if(page==0){
//            if(show_flag)
//            {   show_flag=0;
//                LCD_ShowPicture(0, 0, 240, 90, &ncu[0]);
//            }
            Display_GB2312_String(38,90+20,32,"翻译模式",BLACK,(page0_index==1?GREEN:WHITE));
            //  LCD_ShowCHNString(38,45,"翻译模式",BLACK,(page0_index==1?GREEN:WHITE),32,0);
            rt_thread_mdelay(10);
            Display_GB2312_String(38,90+50+20,32,"危险短信",BLACK,(page0_index==2?GREEN:WHITE));
            rt_thread_mdelay(10);
            Display_GB2312_String(38,90+100+20,32,"更改联系人",BLACK,(page0_index==3?GREEN:WHITE));
            rt_thread_mdelay(10);
        }
        if(page==1){
//            rt_kprintf("here 1\n");
            LCD_DrawRectangle(6,20,240-6,320-20-16-4,WHITE);
//            rt_kprintf("here 2\n");
            if(reflag){
//                rt_kprintf("here 3\n");
                reflag=0;
                show_page=Page;
                follow=1;
            }
            if(!follow){
//                rt_kprintf("here 4\n");
                temp=lcd_page[show_page].head.next;
                if(temp!=RT_NULL){
                while(temp->len!=0){
                    if(temp->index!=255)
                        LCD_ShowIntNum(8, 36+(temp->loc)*40, temp->index+1, 2, BLACK, BROWN, 32);
                    Display_GB2312_String(40, 36+(temp->loc)*40, 32,temp->addr, BLACK, BROWN);
                    temp=temp->next;
                }
                }
            }
            else{
                if(pageflag)    temp=lcd_page[Page-1].head.next;
                else temp=lcd_page[Page].head.next;
                if(temp!=RT_NULL){
                while(temp->len!=0){
                    if(temp->index!=255)
                        LCD_ShowIntNum(8, 36+(temp->loc)*40, temp->index+1, 2, BLACK, BROWN, 32);
                    Display_GB2312_String(40, 36+(temp->loc)*40, 32,temp->addr, BLACK, BROWN);
                    temp=temp->next;
                }}
            }
            rt_mb_recv(lcd_mb,(rt_uint32_t *)&recv_box, RT_WAITING_FOREVER);
            if(*recv_box!='@'){
                for(i=0;recv_box[i]!=0x40;i++) {
                    lcd_recv[i]=recv_box[i];
                }
                lcd_recv[i]='\0';

                len[show_index]=1+str_len(lcd_recv)/14;
                len_sum+=len[show_index];
                show[show_index]=rt_malloc(str_len(lcd_recv));
                str_cpy(show[show_index], lcd_recv);
                rt_kprintf("init string:%s-----len:%d\n",show[show_index],str_len(show[show_index]));
                lcd_object[show_index]=(lcd_object_t*)malloc(sizeof(lcd_object_t));
                lcd_object[show_index]->addr=RT_NULL;
                lcd_object[show_index]->addr=show[show_index];
                lcd_object[show_index]->index=show_index-show_delete;
                lcd_object[show_index]->loc=len_sum-len[show_index]-6*Page;
                lcd_object[show_index]->len=1+str_len(show[show_index])/14;

                rt_kprintf("str_len:%d****string:%s\n",str_len(show[show_index]),show[show_index]);
                if(pageflag)
                {
                    pageflag=0;
                    if(follow){
                    rt_thread_mdelay(500);
                    LCD_Fill(6+2,20+5,240-10,320-20-16-5,BROWN);
                }}
                if(lcd_page[Page].free >= lcd_object[show_index]->len){/// 如果要翻页 则进入这些程序 不翻页 直接插入链表
                    lcd_page[Page].free-=lcd_object[show_index]->len;
                    //插入链表
                    if(lcd_page[Page].head.next==NULL)  {
                        lcd_page[Page].head.next=lcd_object[show_index];
                        lcd_object[show_index]->pre=&(lcd_page[Page].head);
                    }
                    else {
                        lcd_page[Page].head.pre->next=lcd_object[show_index];
                        lcd_object[show_index]->pre=lcd_page[Page].head.pre;
                    }
                    lcd_object[show_index]->next=&(lcd_page[Page].head);
                    lcd_page[Page].head.pre=lcd_object[show_index];

                    if(lcd_page[Page].free==0)
                    {
                        Page++;
                        if(follow) show_page=Page;
                        pageflag=1;
                    }
                }
                else {
                    len_sum-=len[show_index];
                    len[show_index]=lcd_page[Page].free;//1+str_len(lcd_recv)/14-lcd_page[Page].free;
                    len_sum+=len[show_index];
                    lcd_page[Page+1].free-=(lcd_object[show_index]->len-lcd_page[Page].free);
                    lcd_object[show_index]->len=lcd_page[Page].free;

                    lcd_page[Page].head.pre->next=lcd_object[show_index];
                    lcd_object[show_index]->pre=lcd_page[Page].head.pre;
                    lcd_object[show_index]->next=&(lcd_page[Page].head);
                    lcd_page[Page].head.pre=lcd_object[show_index];

                    Page++;//1  2
                    if(follow) {show_page=Page;//36+40*6 240
                    //    lcd_page[Page].free-=(lcd_object[show_index]->len-lcd_page[Page-1].free);
                    LCD_ShowIntNum(8, 36+(lcd_object[show_index]->loc)*40, lcd_object[show_index]->index+1, 2, BLACK, BROWN, 32);
                    Display_GB2312_String(40, 36+(lcd_object[show_index]->loc)*40, 32,lcd_object[show_index]->addr , BLACK, BROWN);
                    }
                    rt_thread_mdelay(500);
                    LCD_Fill(6+2,20+5,240-10,320-20-16-5,BROWN);
                    //这里的在于进行对象编号++然后进行记录
                    show_index++;
                    show[show_index]=rt_malloc(str_len(&show[show_index-1][(lcd_page[Page-1].free)*12]));//找到断裂的地方得到该位置的地址
                    show[show_index]=&(show[show_index-1][(lcd_page[Page-1].free)*12]);
                    len[show_index]=1+str_len(&show[show_index-1][(lcd_page[Page-1].free)*12])/14;//(lcd_object[show_index]->len-lcd_page[Page-1].free);//1+str_len(&show[show_index-1][(lcd_page[Page-1].free)*12])/14;
                    len_sum+=len[show_index];
                    lcd_object[show_index]=(lcd_object_t*)malloc(sizeof(lcd_object_t));
                    lcd_object[show_index]->addr=&show[show_index-1][(lcd_page[Page-1].free)*12];
                    //    lcd_object[show_index]->addr[(lcd_page[Page-1].free)*12]='\0'
                    lcd_object[show_index]->index=255;
                    lcd_object[show_index]->loc=len_sum-len[show_index]-6*Page;
                    lcd_object[show_index]->len=1+str_len(&show[show_index-1][(lcd_page[Page-1].free)*12])/14;
                    show_delete++;
                    lcd_object[show_index]->next=&(lcd_page[Page].head);
                    lcd_page[Page].head.pre=lcd_object[show_index];
                    lcd_page[Page].head.next=lcd_object[show_index];
                    lcd_object[show_index]->pre=&(lcd_page[Page].head);

                    lcd_page[Page-1].free=-1; //该页已经用完

                }
                //     t=lcd_page[0].head.next;
                //    rt_kprintf("len:%d\n loc:%d\n index:%d\n",t->len,t->loc,t->index);
                //     rt_kprintf("pagefree:%d\nlen:%d\n loc:%d\n index:%d\n",
                //            lcd_page[Page].free,lcd_object[show_index]->len,lcd_object[show_index]->loc,lcd_object[show_index]->index);
                //   t=(lcd_object_t*)malloc(sizeof(lcd_object_t));
          /*      for(int a=0;a<=Page;a++){
                    //count0=6;
                    rt_kprintf("now is page:%d",a);
                    t=lcd_page[a].head.next;
                    while(t->len!=0)
                    {
                        rt_kprintf("Page:%dfree:%d\nlen:%d\n loc:%d\n index:%d\n",a,lcd_page[a].free,t->len,t->loc,t->index);
                        t=t->next;
                    }
                }*///36+32*6 192 228
                if(lcd_page[Page].free!=-1&&follow)
                {
                    if(lcd_object[show_index]->index!=255)
                        LCD_ShowIntNum(8, 36+(lcd_object[show_index]->loc)*40, lcd_object[show_index]->index+1, 2, BLACK, BROWN, 32);
                    Display_GB2312_String(40, 36+(lcd_object[show_index]->loc)*40, 32,lcd_object[show_index]->addr, BLACK, BROWN);
                    rt_kprintf("index:%d len:%d loc:%d\n",lcd_object[show_index]->index+1,lcd_object[show_index]->len,lcd_object[show_index]->loc);
                }
                /*   LCD_ShowIntNum(40-16-12, 44+(len_sum-len[show_index])*40, show_index+1, 1, BLACK, BROWN, 32);
                Display_GB2312_String(40-12, 44+(len_sum-len[show_index])*40, 32,show[show_index] , BLACK, GREEN);

                if(show_index>=1)
                    Display_GB2312_String(40-12, 44+(len_sum-len[show_index]-len[show_index-1])*40,32, show[show_index-1] , BLACK, BROWN);*/
                show_index++;
            }
        //    rt_kprintf("show_page:%d pageflag:%d\n",show_page,pageflag);
            if(!return_flag){
            LCD_ShowString(120-16*3, 320-20-16, "Page:", WHITE, BROWN, 16, 0);
            LCD_ShowIntNum(120+16, 320-20-16, ((pageflag&follow)? show_page:show_page+1), 2, WHITE, BROWN, 16);}
        }
        if(page==2){//
            Display_GB2312_String(50+70-24*2,160-12,24,"确认发送",WHITE,BLACK);
            Display_GB2312_String(50+16,160+45-20,16,"取消",BLACK,(page2_index==0?GREEN:WHITE));
            Display_GB2312_String(190-16*3,160+45-20,16,"确认",BLACK,(page2_index==1?GREEN:WHITE));
        }
        if(page==3){
            for(i=0;i<11;i++)
                LCD_ShowChar(40+16*i,120-16, phone[i],(phone_index==i?BLACK:WHITE), (phone_index==i?GREEN:BLACK),32,0);
            switch(exit_count){
            case 0: Draw_Circle(120, 200, 8, BLACK);break;
            case 1: Draw_Circle(120, 200, 8, RED);break;
            }
        }
        if(page==6){
            LCD_Fill(50,120+30, 190, 240, WHITE); //50 210 150 240 195-16
            Display_GB2312_String(120-16*2,195-8,16,"设置成功",BLACK,WHITE);
            rt_thread_mdelay(1000);
            LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
            page=0;
            rt_event_send(lcd_event, 0x01);
        }
    }
}
int lcd_sample(){
    lcd_thread=rt_thread_create("lcd", lcd_entry, RT_NULL, 2048, 20, 5);
    if(lcd_thread==RT_NULL) return 0;
    else rt_thread_startup(lcd_thread);

    lcd_mb=rt_mb_create("lcd_mb",12, RT_IPC_FLAG_FIFO);

    lcd_event=rt_event_create("lcd", RT_IPC_FLAG_FIFO);
    rt_event_send(lcd_event, 0x01);
    return RT_EOK;
}

