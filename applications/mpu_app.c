/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-19     tangliwei       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "mpu6xxx.h"
#include "lcd_sample.h"
#include "AT_sample.h"
#include "adc_sample.h"

#include <rt_ai_network_model.h>
#include <rt_ai.h>
#include <rt_ai_log.h>


#define freq                   100
#define LED_PIN GET_PIN(I, 8)

#define MPU6XXX_DEVICE_NAME1  "i2c2"
#define MPU6XXX_DEVICE_NAME2  "i2c3"
#define MPU6XXX_ADDR1          0x68
#define MPU6XXX_ADDR2          0x69

#define sensor_num     3
#define value_num      3
#define value_len      3
#define collection_len 3
#define period_len     2.1
// data resizing settings
#define len_shift      7
#define mat_width      30
#define mat_height     30
#define samples        300

static rt_ai_t model = NULL;
struct mpu6xxx_device *dev1;
struct mpu6xxx_device *dev2;
struct mpu6xxx_device *dev3;
struct mpu6xxx_3axes accel1, gyro1;
struct mpu6xxx_3axes accel2, gyro2;
struct mpu6xxx_3axes accel3, gyro3;

int data_buffer[90][90];
int temp_buffer[300][9];
int i, m, n, p, o;
rt_ai_buffer_t *work_buffer;
//thread
#define priority 4
#define timeslices 5
rt_thread_t test_thread;//线程控制块
ALIGN(RT_ALIGN_SIZE)
//static rt_uint8_t rt_test_thread_stack[4096];//线程栈

int temp=0;

char p0[] = "下午好@";

char str[][5]={
        "你好@",
        "下午好@",
        "明天见@",
        {0xC4,0xDC,0xB0,0xEF,0xCF,0xC2,0xCE,0xD2,0xC2,0xF0,0x40},
        "再见@"
};

void ai_run_complete(void *arg){
    *(int*)arg = 1;
}

static void test_app(void)
{
//    rt_kprintf("test begin\n");

    /* Initialize mpu6xxx, The parameter is RT_NULL, means auto probing for i2c*/
    dev1 = mpu6xxx_init(MPU6XXX_DEVICE_NAME1, MPU6XXX_ADDR1);
    dev2 = mpu6xxx_init(MPU6XXX_DEVICE_NAME1, MPU6XXX_ADDR2);
    dev3 = mpu6xxx_init(MPU6XXX_DEVICE_NAME2, MPU6XXX_ADDR1);
//
//    if (dev1 == RT_NULL)
//    {
//        rt_kprintf("mpu6xxx1 init failed\n");
//        return -1;
//    }
//
//    if (dev2 == RT_NULL)
//    {
//        rt_kprintf("mpu6xxx2 init failed\n");
//        return -1;
//    }
//
//    if (dev3 == RT_NULL)
//    {
//        rt_kprintf("mpu6xxx3 init failed\n");
//        return -1;
//    }
    rt_kprintf("mpu6xxx s init succeed\n");
    rt_err_t result = RT_EOK;
        int ai_run_complete_flag = 0;
        work_buffer = rt_malloc(RT_AI_NETWORK_WORK_BUFFER_BYTES +
                                                RT_AI_NETWORK_IN_TOTAL_SIZE_BYTES +
                                                RT_AI_NETWORK_OUT_TOTAL_SIZE_BYTES);

        // find a registered model handle]
    //    rt_kprintf("try to find model\n");
        model = rt_ai_find(RT_AI_NETWORK_MODEL_NAME);
        if(!model) {
            rt_kprintf("ai model find err\r\n");
    //        return -1;
        }

        // init the model and allocate memory
    //    rt_kprintf("try to init model");
        result = rt_ai_init(model, work_buffer);
        if (result != 0) {
            rt_kprintf("ai init err\r\n");
    //        return -1;
        }

    while(1){
        if(page==1){

    // prepare input data
//    rt_kprintf("collecting data\n");

    for (i = 0; i <= samples-1; i++) {
        mpu6xxx_get_accel(dev1, &accel1);
        mpu6xxx_get_accel(dev2, &accel2);
        mpu6xxx_get_accel(dev3, &accel3);
        temp_buffer[i][0] = accel1.x;
        temp_buffer[i][1] = accel1.y;
        temp_buffer[i][2] = accel1.z;
        temp_buffer[i][3] = accel2.x;
        temp_buffer[i][4] = accel2.y;
        temp_buffer[i][5] = accel2.z;
        temp_buffer[i][6] = accel3.x;
        temp_buffer[i][7] = accel3.y;
        temp_buffer[i][8] = accel3.z;
        rt_thread_mdelay(40);
    }
    for (int p = 0; p < mat_height; p++) {
        for (int n = 0; n < sensor_num; n++) {
            for (int o = 0; o < value_num; o++) {
                int startIndex = n * mat_width;
                for (int i = 0; i < mat_width; i++) {
                    data_buffer[p + o * mat_height][i + startIndex] = temp_buffer[i + p * len_shift + startIndex][n * sensor_num + o];
                }
            }
        }
    }
//        rt_kprintf("running model\n");
        rt_memcpy(model->input[0], data_buffer, RT_AI_NETWORK_IN_1_SIZE_BYTES);
        result = rt_ai_run(model , ai_run_complete, &ai_run_complete_flag);
        if (result != 0) {
            rt_kprintf("ai model run err\r\n");
//            return -1;
        }
        int pred_num = 0;
        if(ai_run_complete_flag){
            float *out = (float *)rt_ai_output(model, 0);
            for(int i = 1 ; i < 5 ; i++){
                if(out[i] > out[pred_num]){
                    pred_num = i;
                }
            }
            if(pred_num!=temp)
                {
                rt_kprintf("%s",str[pred_num]);

                        rt_event_send(lcd_event, 0x01);
                        rt_mb_send(lcd_mb,(rt_uint32_t *)&str[pred_num]);
                        rt_mb_send(AT_mb ,(rt_uint32_t *)&str[pred_num]);

                    }

                rt_thread_mdelay(4);
                }
        rt_kprintf("pred:%d\n",pred_num);
        temp=pred_num;

        }
        else {
            rt_thread_mdelay(1000);
//            rt_kprintf("The prediction is : %d\n", pred_num);
//            rt_thread_mdelay(1000);
    }

}
//    rt_thread_mdelay(1000);
        rt_free(work_buffer);

}

int mpu_sample(void)
{
    rt_kprintf("mpu begin\n");
//    rt_thread_init(&test_thread,
//                   "test",
//                   test_app,
//                   RT_NULL,
//                   &rt_test_thread_stack[0],
//                   sizeof(rt_test_thread_stack),
//                   priority,
//                   timeslices);
    test_thread=rt_thread_create("test", test_app, RT_NULL, 4096, 13, 5);
    if(test_thread != RT_NULL){
        rt_thread_startup(test_thread);
    }else{
        rt_kprintf("failed\n");
    }
    return 1;
}



