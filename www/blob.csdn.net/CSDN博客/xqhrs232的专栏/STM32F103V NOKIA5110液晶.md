# STM32F103V NOKIA5110液晶 - xqhrs232的专栏 - CSDN博客
2018年03月15日 17:47:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：654
原文地址::[http://www.eeworld.com.cn/mcu/article_2016090629148.html](http://www.eeworld.com.cn/mcu/article_2016090629148.html)
相关文章
1、PIC16F917 NOKIA5110液晶测试程序----[http://www.eeworld.com.cn/mcu/article_2016090529092.html](http://www.eeworld.com.cn/mcu/article_2016090529092.html)
2、PIC16F917 电压表----[http://www.eeworld.com.cn/mcu/article_2016090529091.html](http://www.eeworld.com.cn/mcu/article_2016090529091.html)
3、STM8S103F2 NOKIA5110+AD采集----[http://www.eeworld.com.cn/mcu/article_2016090629157.html](http://www.eeworld.com.cn/mcu/article_2016090629157.html)
4、STM8S103F---Nokia5110液晶显示----[http://www.eeworld.com.cn/mcu/article_2016090629161.html](http://www.eeworld.com.cn/mcu/article_2016090629161.html)
5、stm32驱动nokia5110液晶屏程序----[https://download.csdn.net/download/u010340095/8203629](https://download.csdn.net/download/u010340095/8203629)

  ////////////LCD NOKIA5110引脚定义/////////
#define NOKIA_SCLK            GPIO_Pin_14
#define NOKIA_SDIN            GPIO_Pin_13
#define NOKIA_DC              GPIO_Pin_12
#define NOKIA_RST             GPIO_Pin_10
#define NOKIA_SCE             GPIO_Pin_11
#define NOKIA_VCC       GPIO_Pin_15
#define NOKIA_GND       GPIO_Pin_9
#define NOKIA_**[LED](http://www.eeworld.com.cn/LED/)**       GPIO_Pin_8
//////////////////////////////////////////
Nokia_5110.h头文件
#ifndef __Nokia_5110__
#define __Nokia_5110__
#include"HZ_12x12_pixel.h"
#include"english_6x8_pixel.h"
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
#define true 1
#define flase 0
/////////////引脚定义//////////////
void Delay_1uS(void);
void LCD_Init(void);
void LCD_set_XY(unsigned char X, unsigned char Y);
/*-----------------------------------------------------------------------
LCD_write_english_String  : 英文字符串显示函数
输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
void TestLCD_Nokia5110(void);
unsigned char mask_table[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
unsigned char  Hzk1212[][24];
void displayonechar1212(uchar row,uchar page,uchar dd); //显示 12*12 点阵
void write_byte(uchar dd,uchar a); //写数据或指令 0:指令 1：数据
#define c_Lcd_Max_x 84
#define c_Lcd_Max_y 6   //以字节为单位
#define c_LCD_Max_Dot_X 84
#define c_LCD_Max_Dot_Y 48
#define COM  0  //指令
#define DAT  1  //数据
//-------------------
#define STR  1  //字符
#define HAN  2  //汉字
//--------------------------LPH7366指令集---------------------------
#define Nokia5110_basic_fun_set  0x20 //基本功能设置
#define Nokia5110_expand_fun_set 0x21 //扩展功能设置
//  D7  D6  D5  D4  D3  D2  D1  D0
//  0   0   1   0   0   PD  V   H
//                      |   |   |_______1->扩展指令集  0->基本指令集
//                      |   |___________1->垂直寻址    0->水平寻址
//                      |_______________1->**[低功耗](http://www.eeworld.com.cn/qrs/)**模式  0->正常模式
//-------------------------------------------------------------------
//基本指令集
#define Nokia5110_dis_set  0x0c//显示模式设置
#define Nokia5110_DispMode_AllLightOff 0x08
#define Nokia5110_DispMode_AllLightOn 0x09
#define Nokia5110_DispMode_Reverse 0x0d
//  D7  D6  D5  D4  D3  D2  D1  D0
//  0   0   0   0   1   D   0   E
//                      |       |_______0    0        1    1
//                      |_______________0    1        0    1
//                                      白屏 正常显示 全显 反转
//-------------------------------------------------------------------
#define Nokia5110_add_Yset  0x40 //设置Y地址
//  D7  D6  D5  D4  D3  D2  D1  D0
//  0   1   0   0   0   Y1  Y2  Y3
//-------------------------------------------------------------------
#define Nokia5110_add_Xset  0x80 //设置X地址
//  D7  D6  D5  D4  D3  D2  D1  D0
//  0   1   X6  X5  X4  X3  X2  X1
//-------------------------------------------------------------------
//扩展指令集
#define Nokia5110_tem_set  0x06   //温度系数设置
//  D7  D6  D5  D4  D3  D2  D1  D0
//  0   0   0   0   0   1  TC1  TC0
//-------------------------------------------------------------------
//#define Nokia5110_vot_set  0xD7  //电压系数设置
#define Nokia5110_vot_set  0xc0  //电压系数设置
//  D7  D6  D5  D4  D3  D2  D1  D0
//  1   vo6 vo5 vo4 vo3 vo2 vo1 vo0
//-------------------------------------------------------------------
/*****************************************************************/
void delay_1us(void)                 //1us延时函数
  {
   unsigned int i;
  for(i=0;i<1000;i++);
  }
/*****************************************************************/
/*-----------------------------------------------------------------------
LCD_write_byte    : 使用SPI接口写数据到LCD
输入参数：data    ：写入的数据；
          command ：写数据/命令选择；
-----------------------------------------------------------------------*/
void LCD_write_CMD(unsigned char ucWriteData)
{
    unsigned char uci;
    //SCE = 0;
    //DC = 0;
    GPIO_ResetBits(GPIOD, NOKIA_SCE);
    GPIO_ResetBits(GPIOD, NOKIA_DC);
 for(uci=0;uci<8;uci++)
 {
  if(ucWriteData & 0x80)   //最搞位为1时候就是写，所以要与0x80
  {
   //SDIN = 1;
                        GPIO_SetBits(GPIOD, NOKIA_SDIN);
    }
  else
  {
   //SDIN = 0;
                         GPIO_ResetBits(GPIOD, NOKIA_SDIN);
    }
  //SCLK = 0;
                 GPIO_ResetBits(GPIOD, NOKIA_SCLK);
  ucWriteData = ucWriteData << 1;
  //SCLK = 1;
                GPIO_SetBits(GPIOD, NOKIA_SCLK);
 }
   //SCE = 1;
         GPIO_SetBits(GPIOD, NOKIA_SCE);
}
/*-----------------------------------------------------------------------
LCD_write_byte    : 使用SPI接口写数据到LCD
输入参数：data    ：写入的数据；
          command ：写数据/命令选择；-------------------------*/
void LCD_write_Data(unsigned char ucWriteData)
{
    unsigned char uci;
    //SCE = 0;
    //DC = 1;
    GPIO_ResetBits(GPIOD, NOKIA_SCE);
    GPIO_SetBits(GPIOD, NOKIA_DC);
 for(uci=0;uci<8;uci++)
 {
  if(ucWriteData & 0x80)
  {
   //SDIN = 1;
                        GPIO_SetBits(GPIOD, NOKIA_SDIN);
    }
  else
  {
   //SDIN = 0;
                        GPIO_ResetBits(GPIOD, NOKIA_SDIN);
    }
  //SCLK = 0;
                GPIO_ResetBits(GPIOD, NOKIA_SCLK);
  ucWriteData = ucWriteData << 1;
  //SCLK = 1;
                 GPIO_SetBits(GPIOD, NOKIA_SCLK);
 }
 //SCE = 1;
        GPIO_SetBits(GPIOD, NOKIA_SCE);
}
/*-----------------------------------------------------------------------
LCD_set_XY        : 设置LCD坐标函数
输入参数：X     -83
          Y       ：0－5----------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
{
    LCD_write_CMD(0x40 | (Y & 0X07));  // column
    LCD_write_CMD(0x80 | (X & 0X7F));           // row
}
void setadd(unsigned char a,unsigned char d)//设定地址
{
write_byte((a|0x80),0);
write_byte((d|0x40),0);
}
/*-----------------------------------------------------------------------
LCD_clear         : LCD清屏函数
-----------------------------------------------------------------------*/
void LCD_Clear(void)
{
 uint uii;
// LCD_write_CMD(0x0c);
// LCD_write_CMD(0x80);
 LCD_set_XY(0,0);
 for(uii=0; uii<c_lcd_max_x *="" c_lcd_max_y;="" uii++)
  {
  LCD_write_Data(0x00);
 }
}
void LCD_Init(void)
  {
  // 产生一个让LCD复位的低电平脉冲
 //RST = 0;
        GPIO_ResetBits(GPIOD, NOKIA_RST);
    delay_1us();
    //RST = 1;
        GPIO_SetBits(GPIOD, NOKIA_RST);
    //SCE = 0;
    //DC = 0;
     GPIO_ResetBits(GPIOD, NOKIA_SCE);
     GPIO_ResetBits(GPIOD, NOKIA_DC);
  // 关闭LCD
    delay_1us();  //延时必须加
  // 使能LCD
    delay_1us();
    LCD_write_CMD(0x21); // 使用扩展命令设置LCD模式,PD=0,V=0,H=1
    LCD_write_CMD(0xc0); // 设置偏置电压
    LCD_write_CMD(0x06); // 温度校正
    LCD_write_CMD(0x13); // 1:48
    LCD_write_CMD(0x20); // 使用基本命令
    LCD_write_CMD(0x0c); // 设定显示模式，正常显示
  //  LCD_write_CMD(Nokia5110_DispMode_Reverse);
           // 关闭LCD
 //SCE = 1;
        GPIO_SetBits(GPIOD, NOKIA_SCE);
  }
/*-----------------------------------------------------------------------
LCD_write_char    : 显示英文字符
输入参数：c       ：显示的字符；
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
{
    unsigned char line;
    c -= 32;
    for (line=0; line<6; line++)
 {
       LCD_write_Data(font6x8[c][line]);
   }
}
void write_byte(uchar dd,uchar a)//写一字节 0:指令 1：数据
{
char i;
//SCE=0;
//DC=a;
GPIO_ResetBits(GPIOD, NOKIA_SCE);
   if(a==0)GPIO_ResetBits(GPIOD, NOKIA_DC);
   if(a==1)GPIO_SetBits(GPIOD, NOKIA_DC);        
for(i=0;i<8;i++)
{
if(dd&mask_table[i])
//SDIN=1;
        GPIO_SetBits(GPIOD, NOKIA_SDIN);
else
//SDIN=0;
//SCLK=0;
//SCLK=1;
        GPIO_ResetBits(GPIOD, NOKIA_SDIN);
        GPIO_ResetBits(GPIOD, NOKIA_SCLK);
        GPIO_SetBits(GPIOD, NOKIA_SCLK);
}
//DC=1;
//SCE=1;
//SDIN=1;
 GPIO_SetBits(GPIOD, NOKIA_DC);
 GPIO_SetBits(GPIOD, NOKIA_SCE);
 GPIO_SetBits(GPIOD, NOKIA_SDIN);
}
/*-----------------------------------------------------------------------
LCD_write_english_String  : 英文字符串显示函数
输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
{
    LCD_set_XY(X,Y);
    while (*s)
    {
   LCD_write_char(*s);
   s++;
   }
}
//****************************************************/
//显示12（宽）*12（高）点阵列汉字
void displayonechar1212(uchar row, uchar page,uchar dd) //row:列 page:页 dd:字符
{
uchar row_i,xx,num=0;
for(xx=0;xx<2;xx++)
{
setadd(row,page);// 列，页
for(row_i=num; row_i<num+12;row_i++) write_byte(hzk1212[dd][row_i],1);
 num=num+12;page++;
}
}
#endif
#### 关键字：[STM32F103V](http://www.eeworld.com.cn/tags/STM32F103V)[NOKIA5110液晶](http://www.eeworld.com.cn/tags/NOKIA5110%E6%B6%B2%E6%99%B6)

