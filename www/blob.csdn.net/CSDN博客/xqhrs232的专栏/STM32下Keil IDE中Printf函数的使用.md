# STM32下Keil IDE中Printf函数的使用 - xqhrs232的专栏 - CSDN博客
2013年04月24日 23:04:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1202
原文地址::[http://zggcj.blog.163.com/blog/static/191275229201111822229703/](http://zggcj.blog.163.com/blog/static/191275229201111822229703/)
**请在MDK（keil）工程属性的“Target“-》”Code Generation“中勾**
**选”Use MicroLIB**
前提是你有一个完整keil的工程 比如ADC的调试的时候很多时候用到串口 这里教你怎么样使
用Printf 函数
红色字句为重点！！！！！
**若使得在程序中使用Printf需做一下几步**1,输入输出文件包含#include <stdio.h>
2,在程序下添加宏定义代码
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
         /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf set to 'Yes') calls __io_putchar() */
       #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
     #else
      #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
    #endif /* __GNUC__ */
/* Private functions ---------------------------------------------------------*/
3，添加如下2个函数 usart配置 和 重定向 C库的printf函数
 /* System Clocks Configuration */
void RCC_Configuration(void)
{    
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK | USARTz_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
#ifndef USE_STM3210C_EVAL
  /* Enable USARTy Clock */
  RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE); 
#else
  /* Enable USARTy Clock */
  RCC_APB1PeriphClockCmd(USARTy_CLK, ENABLE); 
#endif
  /* Enable USARTz Clock */
  RCC_APB1PeriphClockCmd(USARTz_CLK, ENABLE);  
}
/* System Clocks Configuration */
/* Configure the GPIO ports */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
#ifdef USE_STM3210C_EVAL
  /* Enable the USART3 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);  
#elif defined(USE_STM3210B_EVAL) || defined(USE_STM32100B_EVAL)
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#endif
  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USARTy_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);
  /* Configure USARTz Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);  
  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USARTy_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);
  /* Configure USARTz Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure); 
}
/* Configure the GPIO ports */
/* USARTy configuration ------------------------------------------------------*/
void USART_Configuration()
{
USART_InitTypeDef USART_InitStructure;
 USART_InitStructure.USART_BaudRate = 230400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* Configure USARTy */
  USART_Init(USARTy, &USART_InitStructure);
  /* Configure USARTz */
  USART_Init(USARTz, &USART_InitStructure);
}
/* USARTy configuration ------------------------------------------------------*/
/*  Retargets the C library printf function to the USART.  */
PUTCHAR_PROTOTYPE
{
USART_SendData(USART1, (uint8_t) ch);
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
{}
return ch;
}
/*  Retargets the C library printf function to the USART.  */
3，然后就可以在main（）调用
printf("The is a example!" );
printf("%s%c%c%c%c%c%s",
           "#**",
           Value/256,Value%6,
          '&',
           Value_2/256,Value_2%6,
          "**%");
之类的输出函数
