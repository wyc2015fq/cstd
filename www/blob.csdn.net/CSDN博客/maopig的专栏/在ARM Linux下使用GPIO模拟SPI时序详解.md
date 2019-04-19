# 在ARM Linux下使用GPIO模拟SPI时序详解 - maopig的专栏 - CSDN博客
2017年03月11日 10:54:44[maopig](https://me.csdn.net/maopig)阅读数：3145
Author：杨正  Data：2016.1.1  Mail：[yz2012ww@gmail.com](mailto:yz2012ww@gmail.com)
一、       概述
SPI是英文SerialPeripheral Interface的缩写，顾名思义就是串行外围设备接口。SPI是一种高速、全双工、同步通信总线，标准的SPI有4个引脚，常用于单片机和EEPROM、FLASH、实时时钟、数字信号处理等器件的通信。SPI通信原理要比I2C简单，它主要是主从方式通信，这种通信方式通常只有一个主机和一个或多个从机，标准的SPI是4根线，分别是SSEL（片选）、SCLK（时钟，也写作SCK）、MOSI（主机输入，从机输出）和MISO（主机输入，从机输出）。
SSEL：从设备片选使能信号。譬如从设备是低电平使能的话，当拉低这个引脚，从设备就会被选中，主机和这个被选中的从设备通信。
SCLK：时钟信号，即主机产生。
MOSI：主机给从机发送指令或者数据的通道。
MISO：主机读取从机的状态或者数据的通道。
在某些情况下，也可以用3根或者2根线的SPI进行通信。譬如，主机只给从机发送指令，从机不需要回复数据的时候，MISO就可以不要；而主机只读从机的数据，不需要给从机发送数据的时候，MOSI就可以不要；当一个主机与一个从机通信时，从机的片选有时可以固定为有效电平而一直处于使能状态，那么SSEL可以不要，此时如果主机只给从机发数据，那么SSEL和MISO都可以不要；如果主机只读取从机发送来的数据，那么SSEL和MOSI都可以不要。当然一般情况下说的SPI都指的是标准的SPI，有4根线进行通信。
二、  时序分析
众所周知，SPI时序有4种模式，在讲解4种模式之前先学习两个单词，即Polarity和Phase，这两个单词分别是SPI的时钟极性（Polarity）和相位（Phase），最常见的写法CPOL和CPHA，也有其它的一些写法，如：
(1) CKPOL (Clock Polarity) = CPOL = POL = Polarity =（时钟）极性
(2) CKPHA (Clock Phase)   = CPHA = PHA = Phase = （时钟）相位
那么时钟极性是什么概念呢？SPI通信过程分为空闲状态和通信状态，空闲状态是指SCLK在数据发送之前和发送完之后的状态，通信状态当然就是指发送数据的状态。CPOL=1，那么空闲状态SCLK为高电平，反之，CPOL=0，则空闲状态SCLK为低电平。
SPI是一个环形的总线结构，主要是在SCLK的控制下，两个双向移位寄存器进行数据交换。那么主机和从机在进行交换数据的时候就设计到一个问题，即主机在什么时刻输出到MOSI上而从机在什么时刻采样这个数据，或者从机什么时刻输出到MISO上而主机什么时刻采样这个数据。同步通信的一个特点就是所有数据的变化和采样都是伴随着时钟沿进行的，也就是说数据总是在时钟的边沿附近变化或被采样，而一个完整的时钟周期必定包含了一个上升沿和一个下降沿，这是周期的定义所决定的，只是这两个沿的先后并无规定。又因为数据从产生到它稳定是需要一定的时间，那么如果主机在上升沿输出数据到MOSI，从机就只能在下降沿去采样这个数据了。反之，如果一方在下降沿输出数据，那么另一方就必须在上升沿采样这个数据。那么由谁来决定上升沿采样还是下降沿采样，上升沿输出还是下降沿输出呢？
当CPHA=1时，表示数据的输出在一个时钟周期的第一个沿上，至于这个沿是上升沿还是下降沿，就得由CPOL来决定。CPOL=1那就是下降沿，反之就是上升沿。那么数据采样自然就是第二个沿上了。
当CPHA=0时，表示数据采样在一个时钟周期的第一个沿上，至于这个沿是上升沿还是下降沿，就得由CPOL来决定。CPOL=1那就是下降沿，反之就是上升沿。那么数据输出自然就是第二个沿上了。
通过以上的学习，SPI的4中模式其实已经浮现出来了，下面对此做一总结：
|**Mode0**|**CPOL=0，CPHA=0**|
|----|----|
|**Mode1**|**CPOL=0，CPHA=1**|
|**Mode2**|**CPOL=1，CPHA=0**|
|**Mode3**|**CPOL=1，CPHA=1**|
图形比文字更容易看懂，所以SPI4种模式的时序图如下：
![](https://img-blog.csdn.net/20160106173035004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
再进一步说明模式1。当SPI处于使能状态，而且发送FIFO内有有效数据时，设置SSEL信号为低，表示开始传输数据。来自Slave的数据立刻发送到Master的接收数据线MISO。半个SCLK时钟周期之后，有效的Master数据传输到MOSI。此时Master和Slave数据都已经有效，SCLK管脚在接下来的半个SCLK时钟周期之后变为高电平。数据在SCLK时钟的上升沿被捕获(采样)，在时钟的下降沿被传送（输出），传输完成后SSEL恢复为高电平。
再进一步说明模式2。当SPI为使能状态，而且发送FIFO内有有效数据时，设置SSEL信号为低表示开始传输数据。半个SCLK时钟周期之后，Master和Slave的有效数据分别在各自的传输线上有效。同时，SCLK从第一个上升沿开始有效。数据在SCLK时钟的下降沿被捕获，在时钟的上升沿被传送，传输完成后SSEL恢复为高电平。
再进一步的说明模式3。当SPI为使能状态，而且发送FIFO内有有效数据时，设置SSEL信号为低表示开始传输数据。此时Slave的数据立刻发送到Master的接收数据线MISO。半个SCLK周期之后，Master的有效数据传送到MOSI。再过半个SCLK时钟周期之后，SCLK管脚设置为低。这表示数据在SCLK时钟的下降沿被捕获，在SCLK时钟的上升沿被传送，传输完成后SSEL恢复为高电平。
再进一步的说明模式4。当SPI为使能状态，而且发送FIFO内有有效数据时，设置SSEL信号为低表示开始传输数据。半个SCLK时钟周期后，Master和Slave数据在各自的传输线上有效。同时，时钟SCLK从1个下降沿开始有效。数据在SCLK时钟的上升沿被捕获，在时钟的下降沿被传送，传输完成后SSEL恢复为高电平。
三、  SPI访问寄存器模式
常见的SPI访问寄存器模式有3种，分别是SingleAccess（单次访问）、Burst Access（突发访问）、FIFO Access（FIFO访问）。
SingleAccess：通过写访问发送一个地址字节后跟一个数据字节，或者发送一个地址字节，再通过读访问接收一个数据字节。发送帧时SSEL为低，发送完最后一个字节后SSEL变为高。如下图，将0x0A写到寄存器0x02上，再从该寄存器上读其值。
![](https://img-blog.csdn.net/20160106184720685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
W + addr. 0x02 Data: 0x0A                      R + addr. 0x02 Data:
注意：SSEL第一次为低电平使能时，MISO变为高电平，然后立即变为低电平，这表示从设备芯片已经装备好（Thefirst time
 CSn goes low, MISO goes high and then low again immediately,indicating that the chip is ready.）。
![](https://img-blog.csdn.net/20160106184958384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Single Byte Access (Write and Read)
BurstAccess：一个地址字节后跟多个数据字节。在每个数据字节之间地址字节会在内部自动增加，并且片选信号SSEL在发送帧时为低电平，在每个数据字节之间也保持低电平，当发送完最后一个字节后变为高电平。如：
BYTE xdata regValues[] = {1,2,3};
halSpiWriteBurstReg(0x00, regValues,sizeof(regValues));
halSpiReadReg(0x00);
halSpiReadReg(0x01);
halSpiReadReg(0x02)；
**![](https://img-blog.csdn.net/20160106184931221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**BurstWrite Followed by Single Read**
FIFO Access：如果地址字节对应的是FIFO地址，那么随后的数据字节将针对FIFO。该地址字节没有自动递增而被存储，并且不需要在每个数据字节之间发送该地址。片选信号SSEL在发送帧时为低电平，在每个数据字节之间也保持低电平，当发送完最后一个字节后变为高电平。
**特别说明**：不同的SPI从设备地址字节的内容不一定相同，需要查看相关手册，才能知道地址字节的构成是什么样的。例如：RFM69H这个模块SPI的地址字节的组成如下：
The first byteis the address byte. It is made of:
u  wnr bit, whichis 1 for write access and 0 for read access
u 7 bits of address, MSB first
又如CCXX00的地址字节组成如下：
The R/W bit in the address header controlsif the register should be written or read, and the burst bit controls if it
is a single access or a burst access.
|R/W|Burst|A5|A4|A3|A2|A1|A0|
**注**：其实地址位只有5位，最高位为读写位，1为读，0为写；后面一位是burstAccess的使能位，当为1时使能。
四、  例程解析
**特别注意**：在主设备这边配置SPI接口时钟的时候一定要弄清楚从设备的时钟要求，因为主设备这边的时钟极性和相位都是以从设备为基准的。因此在时钟极性的配置上一定要搞清楚从设备是在时钟的上升沿还是下降沿接收数据，是在时钟的下降沿还是上升沿输出数据。但要注意的是，由于主设备的SDO连接从设备的SDI，从设备的SDO连接主设备的SDI，从设备SDI接收的数据是主设备的SDO发送过来的，主设备SDI接收的数据是从设备SDO发送过来的，所以主设备这边SPI时钟极性的配置（即SDO的配置）跟从设备的SDI接收数据的极性是相反的，跟从设备SDO发送数据的极性是相同的。
通过手册得知（MOSI is generated by the master on the falling edge ofSCK and is sampled by the slave (i.e. this SPI interface) on the rising edge ofSCK. MISO is generated by the slave on
 the falling edge of SCK.A transferalways starts by the NSS pin going low. MISO is high impedance when NSS ishigh.），本例程使用的从设备的SPI工作在Mode1，即CPOL=
 0 and CPHA = 0，且SSEL低电平使能，当SSEL为高电平时，MISO为高。故主机的MOSI在空闲状态时为高。
```cpp
/*********************************************************************************
 *     Copyright:  (C) 2015 YangZheng<yz2012ww@gmail.com> 
 *                  All rights reserved.
 *
 *      Filename:  dev_rfm69h.c
 *   Description:  This file
 *                
 *       Version:  1.0.0(12/28/2015~)
 *        Author:  Yang Zheng<yz2012ww@gmail.com>
 *     ChangeLog:  1, Release initialversion on "12/28/2015 07:46:07 PM"
 *                
 ********************************************************************************/
#include<linux/kernel.h>
#include<linux/version.h>
#include<linux/module.h>
#include<linux/types.h>
#include<linux/errno.h>
#include<linux/fcntl.h>
#include<linux/mm.h>
#include<linux/proc_fs.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/init.h>
#include<asm/uaccess.h>
#include<asm/io.h>
#include <asm/system.h>
#include<linux/miscdevice.h>
#include<linux/delay.h>
#include<linux/sched.h>
 
#include<linux/proc_fs.h>
#include<linux/poll.h>
 
#include<asm/bitops.h>
#include<asm/uaccess.h>
#include<asm/irq.h>
 
#include<linux/moduleparam.h>
#include <linux/ioport.h>
#include<linux/interrupt.h>
#include<linux/cdev.h>
#include<linux/semaphore.h>
#include<linux/wait.h>
 
#define IOCONFIG4_4                                 IO_ADDRESS(0x200f0000+ 0x054)          //复用管脚gpio4_4
#define IOCONFIG4_5                                 IO_ADDRESS(0x200f0000+ 0x050)          //复用管脚gpio4_5
#define IOCONFIG4_6                                 IO_ADDRESS(0x200f0000+ 0x04c)          //复用管脚gpio4_6
#define IOCONFIG4_7                                 IO_ADDRESS(0x200f0000+ 0x048)          //复用管脚gpio4_7
 
#define GPIO4_BASE                                    0x20180000                        //gpio基地址
#define GPIO4_SIZE                                     0x10000             // 64KB
#define GPIO4_DIR                                      IO_ADDRESS(GPIO4_BASE + 0x400)     //gpio4方向寄存器
#defineGPIO_SPI_CS_REG                         IO_ADDRESS(GPIO4_BASE+ 0x40)          //gpio4_4数据寄存器
#defineGPIO4_SPI_SCK_REG                    IO_ADDRESS(GPIO4_BASE + 0x80)          //gpio4_5数据寄存器
#defineGPIO4_SPI_MOSI_REG                 IO_ADDRESS(GPIO4_BASE+ 0x100)     //gpio4_6数据寄存器
#defineGPIO4_SPI_MISO_REG                 IO_ADDRESS(GPIO4_BASE+ 0x200)     //gpio4_7数据寄存器
#define GPIO4_4                                           (1<< 4)   //gpio4_4
#define GPIO4_5                                           (1<< 5)   //gpio4_5
#define GPIO4_6                                           (1<< 6)   //gpio4_6
#define GPIO4_7                                           (1<< 7)   //gpio4_7
 
#define DATA_LENGTH                                21
 
#define DRV_AUTHOR                                "Yang Zheng<yz2012ww@gmail.com>"
#define DRV_DESC                                       "spi driver"                                                                                
#define DRV_VERSION                                "v0.0.1"
 
#if 0
#defineDBG(x...)   printk(x)
#define DBG_PRINT
#else
#defineDBG(x...)   do {} while (0)
#endif
 
staticDECLARE_WAIT_QUEUE_HEAD(spi_waitq);
unsigned char                                       read_tmp_data[DATA_LENGTH] = {0};
void __iomem                                       *reg_ssp_base_va;
 
typedef unsignedchar                       byte;
typedef unsignedshort                      word;
 
static int                                                read_flag = 0;
 
 
/******************************************************************************
**函数名称：Set_nCS
**函数功能：禁用片选
**输入参数：无
**输出参数：无
**注意：   高电平为禁用片选，低电平为使能片选
******************************************************************************/
void Set_nCS(void)
{
         unsigned char regvalue;
 
         writel(0, IOCONFIG4_4);   //gpio模式
 
         regvalue = readl(GPIO4_DIR);  //设置gpio输出
         regvalue |= GPIO4_4;
         writel(regvalue, GPIO4_DIR);
 
        
         regvalue = readl(GPIO_SPI_CS_REG); 
         regvalue |= GPIO4_4;
         writel(regvalue, GPIO_SPI_CS_REG);  //禁用片选
}
 
 
/******************************************************************************
**函数名称：Clr_nCS
**函数功能：使能片选
**输入参数：无
**输出参数：无
**注意：   高电平为禁用片选，低电平为使能片选
******************************************************************************/
void Clr_nCS(void)
{
         unsigned char regvalue;
 
         writel(0, IOCONFIG4_4);   //gpio模式
 
         regvalue = readl(GPIO4_DIR);  //设置gpio输出
         regvalue |= GPIO4_4;
         writel(regvalue, GPIO4_DIR);
 
         regvalue = readl(GPIO_SPI_CS_REG); 
         regvalue &= ~GPIO4_4;
         writel(0, GPIO_SPI_CS_REG);  //使能片选
 
}
 
 
/******************************************************************************
**函数名称：Set_SCK
**函数功能：SCK为高电平
**输入参数：无
**输出参数：无
**注意：    
******************************************************************************/
void Set_SCK(void)
{
         unsigned char regvalue;
 
         writel(0, IOCONFIG4_5);   //gpio模式
 
         regvalue = readl(GPIO4_DIR);  //设置gpio输出
         regvalue |= GPIO4_5;
         writel(regvalue, GPIO4_DIR);
 
         regvalue =readl(GPIO4_SPI_SCK_REG);  //设置gpio输出高电平
         regvalue |= GPIO4_5;
         writel(regvalue, GPIO4_SPI_SCK_REG);
}
 
 
/******************************************************************************
**函数名称：Set_SCK
**函数功能：SCK为高电平
**输入参数：无
**输出参数：无
**注意：    
******************************************************************************/
void Clr_SCK(void)
{
         unsigned char regvalue;
 
         writel(0, IOCONFIG4_5);   //gpio模式
 
         regvalue = readl(GPIO4_DIR);  //设置gpio输出
         regvalue |= GPIO4_5;
         writel(regvalue, GPIO4_DIR);
 
         regvalue =readl(GPIO4_SPI_SCK_REG);  //设置gpio输出低电平
         regvalue &= ~GPIO4_5;
         writel(regvalue,GPIO4_SPI_SCK_REG); 
 
}
 
 
/******************************************************************************
**函数名称：Set_MOSI
**函数功能：MOSI为高电平
**输入参数：无
**输出参数：无
**注意：    
******************************************************************************/
void Set_MOSI(void)
{
         unsigned char regvalue;
 
         writel(0, IOCONFIG4_6);   //gpio模式
 
         regvalue = readl(GPIO4_DIR);  //设置gpio输出
         regvalue |= GPIO4_6;
         writel(regvalue, GPIO4_DIR);
 
         regvalue = readl(GPIO4_DIR);  //设置gpio输出高电平
         regvalue |= GPIO4_6;
         writel(regvalue, GPIO4_SPI_MOSI_REG);
 
}
 
 
/******************************************************************************
**函数名称：Set_MOSI
**函数功能：MOSI为低电平
**输入参数：无
**输出参数：无
**注意：    
******************************************************************************/
void Clr_MOSI(void)
{
         unsigned char regvalue;
 
         writel(0, IOCONFIG4_6);   //gpio模式
 
         regvalue = readl(GPIO4_DIR);  //设置gpio输出
         regvalue |= GPIO4_6;
         writel(regvalue, GPIO4_DIR);
 
         regvalue = readl(GPIO4_DIR);  //设置gpio输出低电平
         regvalue &= ~GPIO4_6;
         writel(regvalue, GPIO4_SPI_MOSI_REG);
 
}
 
/******************************************************************************
**函数名称：MISO_H
**函数功能：读取MISO_H的值
**输入参数：无
**输出参数：无
**注意：    
******************************************************************************/
unsigned charMISO_H(void)
{
         unsigned char regvalue;
 
         writel(0, IOCONFIG4_7);   //gpio模式
 
         regvalue = readl(GPIO4_DIR);  //设置gpio输入
         regvalue &= ~GPIO4_7;
         writel(regvalue, GPIO4_DIR);
 
         regvalue =readl(GPIO4_SPI_MISO_REG);  //读取输入值
         //DBG("regvalue = %x\n",regvalue);
 
         return regvalue;
}
 
/******************************************************************************
**函数名称：SPICmd8bit
**函数功能：SPI写入参数8bit
**输入参数：WrPara
**输出参数：无
**注意：   保留nCS输出Low
******************************************************************************/
voidSPICmd8bit(byte WrPara)
{
         byte bitcnt;     
         Clr_nCS();
         Clr_SCK();
 
         for(bitcnt = 8; bitcnt != 0; bitcnt--)
         {
                   Clr_SCK();
                   if(WrPara&0x80)
                   {
                            Set_MOSI();
                   }
                   else
                   {
                            Clr_MOSI();
                   }
                   Set_SCK();
                   WrPara <<= 1;
         }
         Clr_SCK();
         Set_MOSI();
 //Set_nCS();            //*此处不关闭nCS，使用连续模式*
}
 
/******************************************************************************
**函数名称：SPIRead8bitt
**函数功能：SPI读取参数8bit
**输入参数：读取8bit数据——RdPara
**输出参数：无
**注意：   保留nCS输出Low
******************************************************************************/
byte SPIRead8bit(void)
{
         byte RdPara = 0;
         byte bitcnt;
          
         Clr_nCS();
         Set_MOSI();               //读FIFO，维持SDI为H 
 
         for(bitcnt=8; bitcnt!=0; bitcnt--)
         {
                   Clr_SCK();
                   RdPara <<= 1;
                   Set_SCK();
                   if(MISO_H())
                   {
                            RdPara |= 0x01;
                   }
                   else
                   {
                            ;
                   }
         }
 
         Clr_SCK();
 
         //Set_nCS();               //*此处不关闭nCS，使用连续模式*
         //DBG("RdPara = %x\n",RdPara);
         return(RdPara);
}
 
/******************************************************************************
**函数名称：SPIRead
**函数功能：SPI读取一个地址数据
**输入参数：adr
**输出参数：无
******************************************************************************/
byte SPIRead(byteadr)
{
         byte tmp;         
 
         SPICmd8bit(adr);               //发送要读取的地址
         tmp = SPIRead8bit();    //读取数据    
         Set_nCS();
 
         return(tmp);
}
 
/******************************************************************************
**函数名称：SPIWrite
**函数功能：SPI写入一个16数据（高8位地址，低8位数据）
**输入参数：WrPara
**输出参数：无
******************************************************************************/
void SPIWrite(wordWrPara)                                                                    
{                                                      
         byte bitcnt;   
          
         Clr_SCK();                           //注意SCK先清0，保持低
         Clr_nCS();
          
         WrPara |= 0x8000;           //写数据高位置1
          
         for(bitcnt=16; bitcnt!=0; bitcnt--)
         {
                   Clr_SCK();
                   if(WrPara&0x8000)
                   {
                            Set_MOSI();
                   }
                   else
                   {
                            Clr_MOSI();
                   }
 
                   Set_SCK();
                   WrPara <<= 1;
         }
          
         Clr_SCK();
         Set_MOSI();
         Set_nCS();
}         
 
/******************************************************************************
**函数名称：SPIBurstRead
**函数功能：SPI连续读取模式
**输入参数：adr——读取地址
**          ptr——存储数据指针
**          length 读取长度
**输出参数：无，数据存在ptr中
******************************************************************************/
voidSPIBurstRead(byte adr, byte *ptr, byte length)
{
         byte i;
         if(length<=1)                       //读取长度必须大于1
         {
                   return;
         }
        else
        {
                 Clr_SCK();                          //注意SCK先清0，保持低
                 Clr_nCS();
                 SPICmd8bit(adr);               //读取地址
                 for(i=0;i<length;i++)
                   {
                         ptr[i]= SPIRead8bit();
                   }
                 Set_nCS();        
         }
}
 
/******************************************************************************
**函数名称：SPIBurstWrite
**函数功能：SPI连续写入模式
**输入参数：adr——写入地址
**          ptr——存储数据指针
**          length 写入长度
**输出参数：无
******************************************************************************/
voidBurstWrite(byte adr, byte *ptr, byte length)
{
         byte i;
 
         if(length<=1)                       //读取长度不为0或1
         {
                   return;
         }
         else 
         {      
                   Clr_SCK();                           //注意SCK先清0，保持低
                   Clr_nCS();                                             
                   SPICmd8bit(adr|0x80);    //连续写
                   for(i=0;i<length;i++)
                   {
                            SPICmd8bit(ptr[i]);
                   }
                   Set_nCS();        
         }
}
 
longspi_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         ……
 
         switch(cmd)
         {
                   case SSP_READ_ALT:
                             …...
                            val = *(unsigned int*)arg;
                            addr = (unsignedchar)(val&0xff);
                            data =SPIRead(addr);
                            ……
                            *(unsigned int *)arg= (unsigned int)(data&0x0000ff);
                            break;
 
 
                   case SSP_READ_BURST:
                            {
                                     unsignedchar          addr = 0x00;
 
                                     ……
 
                                     /*读取数据 */
                                     SPIBurstRead(addr,read_tmp_data, DATA_LENGTH);
 
                                     …...
 
                                     break;
                            }
 
                   case SSP_WRITE_ALT:
                            data = 0;
                            val  = *(unsigned int *)arg;
                            tmp = (unsignedshort)((val&0xff0000)>>8);      
                            tmp |= (unsignedshort)((val&0x0000ff)>>0);
                            ……
                            SPIWrite(tmp);
                            break; 
 
                   case SSP_WRITE_BURST:
                            res =copy_from_user(tmp_str, (unsigned char *)arg, sizeof(tmp_str));
                            if (res != 0)
                            {
                                     printk("copydata from the user space error\n");
                            }
                           
                            ……
                           
                            BurstWrite(addr,data, DATA_LENGTH);
                                                                                                      
                            break;
 
                   default:
                            {
                                     printk("Nosuch spi command %#x!\n", cmd);
                                     return -1;
                            }
         }
         return 0;
}
 
int spi_open(structinode * inode, struct file * file)
{
         ……
 
         return 0;
}
 
intspi_close(struct inode * inode, struct file * file)
{
         return 0;
}
 
 
static structfile_operations spi_fops = {
         .owner          =THIS_MODULE,
         .unlocked_ioctl        = spi_ioctl,
         .open           =spi_open,
         .release           =spi_close
};
 
 
static structmiscdevice spi_dev = {
         .minor                =MISC_DYNAMIC_MINOR,                                                                                      
         .name               =DEV_NAME,
         .fops                           =&spi_fops,
};
 
static int __initspi_gpio_init(void)
{
         int ret;
 
         ret = misc_register(&spi_dev);
         if (ret < 0)
         {
                   printk("registerspi_gpio device failed!\n");
 
                   return -1;
         }
 
         printk("SPI driver initializesuccessful! .\n");
 
         return 0;
}
 
static void __exitspi_gpio_exit(void)
{
         misc_deregister(&spi_dev);
 
         printk("SPI driver exit!\n");
}
module_init(spi_gpio_init);
module_exit(spi_gpio_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRV_AUTHOR);
MODULE_DESCRIPTION(DRV_DESC);
```
