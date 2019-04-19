# STM32F103C8T6 can端口映射 - xqhrs232的专栏 - CSDN博客
2018年02月10日 17:31:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：740
原文地址::[https://www.cnblogs.com/craigtao/p/3645245.html](https://www.cnblogs.com/craigtao/p/3645245.html)
相关文章
1、[STM32F103C8T6
 CAN通信详解](http://www.cnblogs.com/craigtao/p/3645148.html)----[http://www.cnblogs.com/craigtao/p/3645148.html](http://www.cnblogs.com/craigtao/p/3645148.html)
2、关于stm32f103c8t6usb和can引脚复用问题!----[http://bbs.eeworld.com.cn/thread-253658-1-1.html](http://bbs.eeworld.com.cn/thread-253658-1-1.html)
3、STM32F1CUBE使用部分重映射怎么操作？----[http://www.stmcu.org/module/forum/thread-607831-1-1.html](http://www.stmcu.org/module/forum/thread-607831-1-1.html)
# [STM32F103C8T6 can端口映射](http://www.cnblogs.com/craigtao/p/3645245.html)
之前在网上找了一个STM32F103C8T6关于can通信的例子，直接烧录，can不能使用，
使用示波器在VP230前后端查看没有波形，
先介绍板子硬件资源：
HSE时钟：8MHz；
MCU ： STM32F103C8T6
CAN：一路；
在软件的配置中找不到任何原因，使用USB-CAN分析仪不能识别波特率，怀疑是不是因为波特率设置不对，
是不是硬件设计出现错误，
查找手册 《stm32f103c8t6》
![](https://images0.cnblogs.com/i/521709/201404/041504427039166.jpg)
can的发送接收引脚是在PA11 PA12，（注意：can和其他的复用）
我找的那个程序，我把原理找到看了一下，can的发送接收是在 PB8 PB9，怀疑是我的板子硬件出错了，
我的板子硬件原理图上是PA11 PA12，是按照手册来设计的，
既然硬件没有问题，应该是软件问题，看看GPIO的配置：
```
/*CAN GPIO  */
 void CAN_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  /* GPIOB */     
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);                                                                         
  /* CAN1  */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 
  /* Configure CAN pin: RX */     // PB8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     // 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */   // PB9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    //#define GPIO_Remap_CAN    GPIO_Remap1_CAN1 
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);    
  
}
```
明白了，原来是把can的发送接收端口映射到了PB8、9上面了，
修改后的代码：
```
void CAN_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  /* GPIOA */     
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);                                                                         
  /* CAN1  */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 
  /* Configure CAN pin: RX */     // PA11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     // 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */   // PA12
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //#define GPIO_Remap_CAN    GPIO_Remap1_CAN1 
  //GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);     //取消端口映射   craigtao 2014-4-4
  
}
```
这回编译，烧录，示波器检测，，哇，波形出来了，
耶稣爱你，    craigtao   2014-4-4

