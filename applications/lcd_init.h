 #ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include <drv_common.h>
/*
#define CLK GET_PIN(B,3)
#define MOSI GET_PIN(B,4)
#define RES GET_PIN(B,5)
#define DC GET_PIN(B,6)
#define BLK GET_PIN(B,7)
#define MISO GET_PIN(B,8)
#define CS1 GET_PIN(B,9)
#define CS2 GET_PIN(E,0)
*/

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#define Chip_Selection 1   //设置芯片初始化 0为ILI9341  1为ST7789

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif



//-----------------LCD端口定义----------------
/*
#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_12)//SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(GPIOG,GPIO_Pin_12)

#define LCD_MOSI_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_5)//SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(GPIOD,GPIO_Pin_5)

#define LCD_RES_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_4)//RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_4)

#define LCD_DC_Clr()   GPIO_ResetBits(GPIOD,GPIO_Pin_15)//DC
#define LCD_DC_Set()   GPIO_SetBits(GPIOD,GPIO_Pin_15)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_1)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_1)

#define ZK_MISO        GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)//MISO  读取字库数据引脚

#define LCD_CS_Clr()   GPIO_ResetBits(GPIOE,GPIO_Pin_10)//CS1
#define LCD_CS_Set()   GPIO_SetBits(GPIOE,GPIO_Pin_10)

#define ZK_CS_Clr()    GPIO_ResetBits(GPIOE,GPIO_Pin_12)//CS2 字库片选
#define ZK_CS_Set()    GPIO_SetBits(GPIOE,GPIO_Pin_12)

*/
#define LCD_SCLK_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)//SCL=SCLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)//SDA=MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)

#define LCD_RES_Clr()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)

#define LCD_BLK_Clr()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)

#define ZK_MISO        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)//MISO  读取字库数据引脚

#define LCD_CS_Clr()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)//CS1
#define LCD_CS_Set()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)

#define ZK_CS_Clr()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET)//CS2 字库片选
#define ZK_CS_Set()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET)
/*
#define LCD_SCLK_Clr() rt_pin_write(CLK,PIN_LOW);
#define LCD_SCLK_Set() rt_pin_write(CLK,PIN_HIGH);

#define LCD_MOSI_Clr() rt_pin_write(MOSI,PIN_LOW);//SDA=MOSI
#define LCD_MOSI_Set() rt_pin_write(MOSI,PIN_HIGH);

#define LCD_RES_Clr()  rt_pin_write(RES,PIN_LOW);//RES
#define LCD_RES_Set()  rt_pin_write(RES,PIN_HIGH);

#define LCD_DC_Clr()   rt_pin_write(DC,PIN_LOW);//DC
#define LCD_DC_Set()   rt_pin_write(DC,PIN_HIGH);

#define LCD_BLK_Clr()  rt_pin_write(BLK,PIN_LOW);//BLK
#define LCD_BLK_Set()  rt_pin_write(BLK,PIN_HIGH);

#define ZK_MISO        rt_pin_read(MISO)//MISO  读取字库数据引脚

#define LCD_CS_Clr()   rt_pin_write(CS1,PIN_LOW);//CS1
#define LCD_CS_Set()   rt_pin_write(CS1,PIN_HIGH);

#define ZK_CS_Clr()    rt_pin_write(CS2,PIN_LOW);//CS2 字库片选
#define ZK_CS_Set()    rt_pin_write(CS2,PIN_HIGH);
*/
void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(rt_uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(rt_uint8_t dat);//写入一个字节
void LCD_WR_DATA(rt_uint16_t dat);//写入两个字节
void LCD_WR_REG(rt_uint8_t dat);//写入一个指令
void LCD_Address_Set(rt_uint16_t x1,rt_uint16_t y1,rt_uint16_t x2,rt_uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif
