#ifndef __ADC_SAMPLE_H_
#define __ADC_SAMPLE_H_

extern rt_uint8_t page0_index;
extern rt_uint8_t page2_index;
extern rt_uint8_t page;
extern rt_uint8_t show_page;
extern rt_uint8_t choic2;
extern rt_uint8_t phone[11],phone_real[11],phone_index,exit_count;
extern rt_int8_t reflag,follow,return_flag,show_flag;
int adc_sample();

#endif
