#include "lcd_init.h"
#include "rtthread.h"

void LCD_GPIO_Init(void)
{   /*
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);  //使能A端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    GPIO_SetBits(GPIOG,GPIO_Pin_12);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_SetBits(GPIOD,GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE,GPIO_Pin_10|GPIO_Pin_12);*/

    GPIO_InitTypeDef GPIO_InitStruct0 = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct0.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9;
    GPIO_InitStruct0.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct0.Pull = GPIO_NOPULL;
    GPIO_InitStruct0.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9, GPIO_PIN_SET);

    GPIO_InitTypeDef GPIO_InitStruct1 = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct1.Pin = GPIO_PIN_8;
    GPIO_InitStruct1.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct1.Pull = GPIO_PULLUP;
    GPIO_InitStruct1.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct1);

    GPIO_InitTypeDef GPIO_InitStruct2 = {0};
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitStruct2.Pin = GPIO_PIN_0;
    GPIO_InitStruct2.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct2.Pull = GPIO_NOPULL;
    GPIO_InitStruct2.Speed = GPIO_SPEED_FREQ_LOW;
    // GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct2);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
  /*  rt_pin_mode(CLK,PIN_MODE_OUTPUT);
    rt_pin_mode(MOSI,PIN_MODE_OUTPUT);
    rt_pin_mode(RES,PIN_MODE_OUTPUT);
    rt_pin_mode(DC,PIN_MODE_OUTPUT);
    rt_pin_mode(BLK,PIN_MODE_OUTPUT);
    rt_pin_mode(MISO,PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(CS1,PIN_MODE_OUTPUT);
    rt_pin_mode(CS2,PIN_MODE_OUTPUT);*/

}


/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(rt_uint8_t dat)
{
    rt_uint8_t i;
    LCD_CS_Clr();
    for(i=0;i<8;i++)
    {
        LCD_SCLK_Clr();
        if(dat&0x80)
        {
           LCD_MOSI_Set();
        }
        else
        {
           LCD_MOSI_Clr();
        }
        LCD_SCLK_Set();
        dat<<=1;
    }
  LCD_CS_Set();
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(rt_uint8_t dat)
{
    LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(rt_uint16_t dat)
{
    LCD_Writ_Bus(dat>>8);
    LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(rt_uint8_t dat)
{
    LCD_DC_Clr();//写命令
    LCD_Writ_Bus(dat);
    LCD_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(rt_uint16_t x1,rt_uint16_t y1,rt_uint16_t x2,rt_uint16_t y2)
{
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);//储存器写
}

/******************************************************************************
      函数说明：LCD初始化函数
      入口数据：无
      返回值：  无
******************************************************************************/
void LCD_Init(void)
{
  if(Chip_Selection==0)  //初始化ILI9341
    {
    LCD_GPIO_Init();//初始化GPIO

    LCD_RES_Clr();//复位
    rt_thread_mdelay(100);
    LCD_RES_Set();
    rt_thread_mdelay(100);

    LCD_BLK_Set();//打开背光
  rt_thread_mdelay(100);

    //************* Start Initial Sequence **********//
    LCD_WR_REG(0x11); //Sleep out
    rt_thread_mdelay(120);              //Delay 120ms
    //************* Start Initial Sequence **********//
    LCD_WR_REG(0xCF);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0xC1);
    LCD_WR_DATA8(0X30);
    LCD_WR_REG(0xED);
    LCD_WR_DATA8(0x64);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0X12);
    LCD_WR_DATA8(0X81);
    LCD_WR_REG(0xE8);
    LCD_WR_DATA8(0x85);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x79);
    LCD_WR_REG(0xCB);
    LCD_WR_DATA8(0x39);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x34);
    LCD_WR_DATA8(0x02);
    LCD_WR_REG(0xF7);
    LCD_WR_DATA8(0x20);
    LCD_WR_REG(0xEA);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_REG(0xC0); //Power control
    LCD_WR_DATA8(0x1D); //VRH[5:0]
    LCD_WR_REG(0xC1); //Power control
    LCD_WR_DATA8(0x12); //SAP[2:0];BT[3:0]
    LCD_WR_REG(0xC5); //VCM control
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x3F);
    LCD_WR_REG(0xC7); //VCM control
    LCD_WR_DATA8(0x92);
    LCD_WR_REG(0x3A); // Memory Access Control
    LCD_WR_DATA8(0x55);
    LCD_WR_REG(0x36); // Memory Access Control
    if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
    else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
    else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
    else LCD_WR_DATA8(0xA8);
    LCD_WR_REG(0xB1);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x12);
    LCD_WR_REG(0xB6); // Display Function Control
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0xA2);

    LCD_WR_REG(0x44);
    LCD_WR_DATA8(0x02);

    LCD_WR_REG(0xF2); // 3Gamma Function Disable
    LCD_WR_DATA8(0x00);
    LCD_WR_REG(0x26); //Gamma curve selected
    LCD_WR_DATA8(0x01);
    LCD_WR_REG(0xE0); //Set Gamma
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x22);
    LCD_WR_DATA8(0x1C);
    LCD_WR_DATA8(0x1B);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x48);
    LCD_WR_DATA8(0xB8);
    LCD_WR_DATA8(0x34);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x00);
    LCD_WR_REG(0XE1); //Set Gamma
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x23);
    LCD_WR_DATA8(0x24);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x10);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x38);
    LCD_WR_DATA8(0x47);
    LCD_WR_DATA8(0x4B);
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x06);
    LCD_WR_DATA8(0x30);
    LCD_WR_DATA8(0x38);
    LCD_WR_DATA8(0x0F);
    LCD_WR_REG(0x29); //Display on
 }
    else          //初始化ST7789
    {
        LCD_GPIO_Init();//初始化GPIO
        LCD_RES_Clr();  //复位
        rt_thread_mdelay(100);
        LCD_RES_Set();
        rt_thread_mdelay(100);
        LCD_BLK_Set();//打开背光
        rt_thread_mdelay(500);
        LCD_WR_REG(0x11);
        rt_thread_mdelay(100); //Delay 120ms
//************* Start Initial Sequence **********//
//------------------------------display and color format setting--------------------------------//

        LCD_WR_REG(0X36);// Memory Access Control
        if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
        else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
        else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
        else LCD_WR_DATA8(0xA0);
        LCD_WR_REG(0X3A);
        LCD_WR_DATA8(0X05);
    //--------------------------------ST7789S Frame rate setting-------------------------

        LCD_WR_REG(0xb2);
        LCD_WR_DATA8(0x0c);
        LCD_WR_DATA8(0x0c);
        LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0x33);
        LCD_WR_DATA8(0x33);
        LCD_WR_REG(0xb7);
        LCD_WR_DATA8(0x35);
        //---------------------------------ST7789S Power setting-----------------------------

        LCD_WR_REG(0xbb);
        LCD_WR_DATA8(0x35);
        LCD_WR_REG(0xc0);
        LCD_WR_DATA8(0x2c);
        LCD_WR_REG(0xc2);
        LCD_WR_DATA8(0x01);
        LCD_WR_REG(0xc3);
        LCD_WR_DATA8(0x13);
        LCD_WR_REG(0xc4);
        LCD_WR_DATA8(0x20);
        LCD_WR_REG(0xc6);
        LCD_WR_DATA8(0x0f);
        LCD_WR_REG(0xca);
        LCD_WR_DATA8(0x0f);
        LCD_WR_REG(0xc8);
        LCD_WR_DATA8(0x08);
        LCD_WR_REG(0x55);
        LCD_WR_DATA8(0x90);
        LCD_WR_REG(0xd0);
        LCD_WR_DATA8(0xa4);
        LCD_WR_DATA8(0xa1);
        //--------------------------------ST7789S gamma setting------------------------------
        LCD_WR_REG(0xe0);
        LCD_WR_DATA8(0xd0);
        LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0x06);
        LCD_WR_DATA8(0x09);
        LCD_WR_DATA8(0x0b);
        LCD_WR_DATA8(0x2a);
        LCD_WR_DATA8(0x3c);
        LCD_WR_DATA8(0x55);
        LCD_WR_DATA8(0x4b);
        LCD_WR_DATA8(0x08);
        LCD_WR_DATA8(0x16);
        LCD_WR_DATA8(0x14);
        LCD_WR_DATA8(0x19);
        LCD_WR_DATA8(0x20);
        LCD_WR_REG(0xe1);
        LCD_WR_DATA8(0xd0);
        LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0x06);
        LCD_WR_DATA8(0x09);
        LCD_WR_DATA8(0x0b);
        LCD_WR_DATA8(0x29);
        LCD_WR_DATA8(0x36);
        LCD_WR_DATA8(0x54);
        LCD_WR_DATA8(0x4b);
        LCD_WR_DATA8(0x0d);
        LCD_WR_DATA8(0x16);
        LCD_WR_DATA8(0x14);
        LCD_WR_DATA8(0x21);
        LCD_WR_DATA8(0x20);
        LCD_WR_REG(0x29);
}
}
