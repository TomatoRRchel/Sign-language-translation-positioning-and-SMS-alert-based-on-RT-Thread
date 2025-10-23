#ifndef _LCD_SAMPLE_H
#define _LCD_SAMPLE_H

int lcd_sample();
extern rt_mailbox_t lcd_mb;
extern rt_event_t lcd_event;
extern rt_uint8_t Page,pageflag;
#endif
