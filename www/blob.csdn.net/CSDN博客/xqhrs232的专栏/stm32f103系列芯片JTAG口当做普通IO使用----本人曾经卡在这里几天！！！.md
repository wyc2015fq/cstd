# stm32f103系列芯片JTAG口当做普通IO使用----本人曾经卡在这里几天！！！ - xqhrs232的专栏 - CSDN博客
2018年02月06日 17:06:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：390
原文地址::[http://blog.csdn.net/hot__rain/article/details/54585507](http://blog.csdn.net/hot__rain/article/details/54585507)
使用的芯片是stm32f103c8t6.
今天在调试板子时发现在PA15口上的LED灯无论输出高还是低都只有一个中间电平2.3V，后来百度说这个口(PA15--JTDI)默认用作JTAG调试口，需要配置下成普通IO口，于是按照那人的办法写了2句：RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
不写还好，一写呵呵哒，结果连原本用来下载、调试程序用的SWD口都不能用了。。。后来在STM32库中找到了正确的写法：RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);这个的意思才是不使用JTAG，保留SWD，前面那个JTAG、SWD都不保留。
问题倒是发现了，可怎么办呢？现在连SWD都不能用了。。。用串口下载吧，一看板子傻眼了(自己画的)，BOOT1悬空，BOOT0接地。。。没办法接线跳线吧，把BOOT1接地，BOOT0通过4K7上拉3V3，再用MCUISP那个串口下载(用过野火开发板的应该知道)软件先擦除芯片再写入新HEX文件(新HEX文件是上面第二条代码，只保留SWD)。
下载成功，然后把BOOT0接上拉(BOOT1还是接地没关系，这个数据手册上写的很清楚)，接上ST-LINK后SWD模式能正常下载，而且这个PA15口也能正常控制灯。
由此可见：1.如果引脚资源不是很紧张还是将BOOT1脚拉低；2.在上面第一条代码下JTAG原本占用的5个IO都可用作普通IO口，只不过以后只能通过串口下载程序了。
**[html]**[view
 plain](http://blog.csdn.net/hot__rain/article/details/54585507#)[copy](http://blog.csdn.net/hot__rain/article/details/54585507#)
- void LED_GPIO_INIT(void)  
- {  
-     GPIO_InitTypeDef GPIO_InitStructure;  
- 
-     RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE ); //PA clock enable  
- 
- GPIO_InitStructure.GPIO_Pin = LED_PIN;  
- GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //PULL PUSH   
- GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
- 
-     GPIO_Init( LED_PORT, &GPIO_InitStructure );  
- 
-     // for PA15 JTDI REMAP  
-     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  
- //  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);     // JTAG&SWD DISABLE  
-     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); // JTAG DISABLE,SWD ENABLE  
- 
-     GPIO_ResetBits( LED_PORT, LED_PIN );    //TURN OFF LED  
- }  
- 
//=======================================================================
本人备注::
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO , ENABLE); 
  /**
  *  WR->PA15
  */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//--------------------------------------------------
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO , ENABLE); 
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  /**
  *  RD->PB3, A0->PB4, CS->PB5
  */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
