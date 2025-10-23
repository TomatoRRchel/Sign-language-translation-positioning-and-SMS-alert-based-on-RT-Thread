#ifndef __LCD_H
#define __LCD_H

#include "rtthread.h"

void LCD_Fill(rt_uint16_t xsta,rt_uint16_t ysta,rt_uint16_t xend,rt_uint16_t yend,rt_uint16_t color);//指定区域填充颜色
void LCD_DrawPoint(rt_uint16_t x,rt_uint16_t y,rt_uint16_t color);//在指定位置画一个点
void LCD_DrawLine(rt_uint16_t x1,rt_uint16_t y1,rt_uint16_t x2,rt_uint16_t y2,rt_uint16_t color);//在指定位置画一条线
void LCD_DrawRectangle(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2,rt_uint16_t color);//在指定位置画一个矩形
void Draw_Circle(rt_uint16_t x0,rt_uint16_t y0,rt_uint8_t r,rt_uint16_t color);//在指定位置画一个圆

void LCD_ShowChinese(rt_uint16_t x,rt_uint16_t y,rt_uint8_t *s,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey,rt_uint8_t mode);//显示汉字串
void LCD_ShowChinese12x12(rt_uint16_t x,rt_uint16_t y,rt_uint8_t *s,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey,rt_uint8_t mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(rt_uint16_t x,rt_uint16_t y,rt_uint8_t *s,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey,rt_uint8_t mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(rt_uint16_t x,rt_uint16_t y,rt_uint8_t *s,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey,rt_uint8_t mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(rt_uint16_t x,rt_uint16_t y,rt_uint8_t *s,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey,rt_uint8_t mode);//显示单个32x32汉字

void LCD_ShowChar(rt_uint16_t x,rt_uint16_t y,rt_uint8_t num,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey,rt_uint8_t mode);//显示一个字符
void LCD_ShowString(rt_uint16_t x,rt_uint16_t y,const rt_uint8_t *p,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey,rt_uint8_t mode);//显示字符串
rt_uint32_t mypow(rt_uint8_t m,rt_uint8_t n);//求幂
void LCD_ShowIntNum(rt_uint16_t x,rt_uint16_t y,rt_uint16_t num,rt_uint8_t len,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey);//显示整数变量
void LCD_ShowFloatNum1(rt_uint16_t x,rt_uint16_t y,float num,rt_uint8_t len,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey);//显示两位小数变量

void LCD_ShowPicture(rt_uint16_t x,rt_uint16_t y,rt_uint16_t length,rt_uint16_t width,const rt_uint8_t pic[]);//显示图片

//字库函数
void ZK_command(rt_uint8_t dat);
rt_uint8_t get_data_from_ROM(void);
void get_n_bytes_data_from_ROM(rt_uint8_t AddrHigh,rt_uint8_t AddrMid,rt_uint8_t AddrLow,rt_uint8_t *pBuff,rt_uint8_t DataLen);
void Display_GB2312(rt_uint16_t x,rt_uint16_t y,rt_uint8_t zk_num,rt_uint16_t fc,rt_uint16_t bc);
rt_uint8_t Display_GB2312_String(rt_uint16_t x,rt_uint16_t y,rt_uint8_t zk_num,rt_uint8_t text[],rt_uint16_t fc,rt_uint16_t bc);
void Display_Asc(rt_uint16_t x,rt_uint16_t y,rt_uint8_t zk_num,rt_uint16_t fc,rt_uint16_t bc);
void Display_Asc_String(rt_uint16_t x,rt_uint16_t y,rt_uint16_t zk_num,rt_uint8_t text[],rt_uint16_t fc,rt_uint16_t bc);
void Display_Arial_TimesNewRoman(rt_uint16_t x,rt_uint16_t y,rt_uint8_t zk_num,rt_uint16_t fc,rt_uint16_t bc);
void Display_Arial_String(rt_uint16_t x,rt_uint16_t y,rt_uint16_t zk_num,rt_uint8_t text[],rt_uint16_t fc,rt_uint16_t bc);
void Display_TimesNewRoman_String(rt_uint16_t x,rt_uint16_t y,rt_uint16_t zk_num,rt_uint8_t text[],rt_uint16_t fc,rt_uint16_t bc);

//画笔颜色
#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED                   0XFFE0
#define GBLUE                  0X07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN                0XBC40 //棕色
#define BRRED                0XFC07 //棕红色
#define GRAY                 0X8430 //灰色
#define DARKBLUE         0X01CF //深蓝色
#define LIGHTBLUE        0X7D7C //浅蓝色
#define GRAYBLUE         0X5458 //灰蓝色
#define LIGHTGREEN       0X841F //浅绿色
#define LGRAY                0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

typedef struct kiki{
    rt_uint8_t *addr;
    rt_uint8_t index;
    rt_uint8_t loc;
    rt_uint8_t len;
    struct kiki *next;
    struct kiki *pre;
}lcd_object_t;

typedef struct lcd_struct{
  rt_int8_t free;
  lcd_object_t head;
}lcd_page_t;

#define page_num    50
#endif

