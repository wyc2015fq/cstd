# Keil MDK环境下使用printf函数的解决方法 - xqhrs232的专栏 - CSDN博客
2013年04月24日 23:04:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1498
原文地址::[http://yanceysky.spaces.eepw.com.cn/articles/article/item/94364](http://yanceysky.spaces.eepw.com.cn/articles/article/item/94364)
printf()函数可以直接向窗口格式化输出，给我们调试程序带来很大的方便，然而在Keil MDK环境下直接使用printf()函数就会陷入软件中断，导致死循环的产生；因而不能进入main()函数。
单步调试程序时会发现程序指针一直在 SWI_Handler     B       SWI_Handler 打转；
解决方法：
工程选择NXP的LPC21XX器件。
1.首先在Keil安装目录下面ARM——Startup找到Retarget.c文件将其复制到你的工程文件夹下面；并将其加入到工程中；
2. 在Keil安装目录下面ARM——Examples——Hello找到Serial.c文件将其复制到你的工程文件夹下面；并将其加入到工程中；
3.Serial.c代码如下：
/******************************************************************************/
/* SERIAL.C: Low Level Serial Routines                                        */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
#include <LPC21xx.H>                     /* LPC21xx definitions               */
#define CR     0x0D
void init_serial (void)  {               /* Initialize Serial Interface       */
  PINSEL0 = 0x00050000;                  /* Enable RxD1 and TxD1              */
  U1LCR = 0x83;                          /* 8 bits, no Parity, 1 Stop bit     */
  U1DLL = 97;                            /* 9600 Baud Rate @ 15MHz VPB Clock  */
  U1LCR = 0x03;                          /* DLAB = 0                          */
}
/* implementation of putchar (also used by printf function to output data)    */
int sendchar (int ch)  {                 /* Write character to Serial Port    */
  if (ch == '\n')  {
    while (!(U1LSR & 0x20));
    U1THR = CR;                          /* output CR */
  }
  while (!(U1LSR & 0x20));
  return (U1THR = ch);
}
int getkey (void)  {                     /* Read character from Serial Port   */
  while (!(U1LSR & 0x01));
  return (U1RBR);
}
4.在头文件中加入#include<stdio.h>。
5.在使用printf函数之前对串口进行初始化；可以直接使用Serial.c中的 init_serial ()函数对串口初始化。
