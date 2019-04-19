# 如何在MDK环境下使用printf以及使用vsprintf - xqhrs232的专栏 - CSDN博客
2017年04月14日 10:26:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1402
原文地址::[http://blog.csdn.net/vincent_fang/article/details/41348781](http://blog.csdn.net/vincent_fang/article/details/41348781)
相关文章
1、[STM32+Keil 如何使用printf函数？](http://blog.csdn.net/lanmanck/article/details/17411921)----[STM32+Keil
 如何使用printf函数？](STM32+Keil)[STM32+Keil 如何使用printf函数？](%E5%A6%82%E4%BD%95%E4%BD%BF%E7%94%A8printf%E5%87%BD%E6%95%B0%EF%BC%9F)
2、**mdk中printf函数使用的一点理解----[http://www.openedv.com/posts/list/44609.htm](http://www.openedv.com/posts/list/44609.htm)**
3、[STM32
 MDK工程中使用printf](http://www.cnblogs.com/zyqgold/archive/2013/05/20/3088416.html)----[http://www.cnblogs.com/zyqgold/archive/2013/05/20/3088416.html](http://www.cnblogs.com/zyqgold/archive/2013/05/20/3088416.html)
   这一年来一直在用STM32以及LPC17XX系列，自然而然也一直在使用MDK环境。在使用串口调试过程中，越发觉得下位机的串口输出函数没有[C语言](http://lib.csdn.net/base/c)库中的printf好用。于是就去查找学习了下如何将printf函数给引进过来。此文目的于此-----总结记录。
     首先使用printf函数，代码如下：
#include "stdio.h"     
**[cpp]**[view
 plain](http://blog.csdn.net/vincent_fang/article/details/41348781#)[copy](http://blog.csdn.net/vincent_fang/article/details/41348781#)
[print](http://blog.csdn.net/vincent_fang/article/details/41348781#)[?](http://blog.csdn.net/vincent_fang/article/details/41348781#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/527752/fork)
- <span style="font-size:14px;">#define <span style="color:#ff0000;">NOT_USE_MICROLIB</span> 0  
- 
- 
- #if <span style="color:#ff0000;">NOT_USE_MICROLIB</span>
- //=========不用选择use MicroLIB的方法=====================
- #pragma import(__use_no_semihosting)             
- //标准库需要的支持函数                 
- struct __FILE   
- {   
- int handle;   
- 
- };   
- 
- FILE __stdout;         
- //定义_sys_exit()以避免使用半主机模式    
- _sys_exit(int x)   
- {   
-     x = x;   
- }   
- //重定义fputc函数 
- int fputc(int ch, FILE *f)  
- {        
- while((USART1->SR&0X40)==0);//循环发送,直到发送完毕  <span style="font-family: Arial, Helvetica, sans-serif;">USART1</span><span style="font-family: Arial, Helvetica, sans-serif;"> 为STM32串口1的寄存器，后续需要根据实际MCU进行更改</span>
-     USART1->DR = (u8) ch;        
- return ch;  
- }  
- #else
- //=========使用use MicroLIB的方法=====================
- 
- int fputc(int ch, FILE *f)  
- {  
-     USART_SendData(USART1, (uint8_t) ch);  
- 
- while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}    
- 
- return ch;  
- }  
- int GetKey (void)  {   
- 
- while (!(USART1->SR & USART_FLAG_RXNE));  
- 
- return ((int)(USART1->DR & 0x1FF));  
- }  
- #endif</span>
   解释下：
    想要在MDK中使用printf，需要同时重定义fputc函数和避免使用semihosting（半主机模式），标准库函数的默认输出设备是显示器，要实现在串口或LCD输出，必须重定义标准库函数里调用的与输出设备相关的函数。所以需要将printf需要调用的fput里面的输出指向串口（重定向）。
     另外printf之类的函数，使用了半主机模式。使员工标准库会导致程序无法运行，解决方法有2个：
     方法1.使用微库，因为使用微库的话，不会使用半主机模式。
![](https://img-blog.csdn.net/20141121151552500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmluY2VudF9mYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    方法2.添加一段代码
**[cpp]**[view
 plain](http://blog.csdn.net/vincent_fang/article/details/41348781#)[copy](http://blog.csdn.net/vincent_fang/article/details/41348781#)
[print](http://blog.csdn.net/vincent_fang/article/details/41348781#)[?](http://blog.csdn.net/vincent_fang/article/details/41348781#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/527752/fork)
- #pragma import(__use_no_semihosting)             
- //标准库需要的支持函数                 
- struct __FILE   
- {   
- int handle;   
- 
- };   
- 
- FILE __stdout;         
- //定义_sys_exit()以避免使用半主机模式    
- _sys_exit(int x)   
- {   
-     x = x;   
- }   
    semihosting的作用，介绍如下  
    Semihosting is a mechanism for ARM targets to communicate input/outputrequests from application code to a host computer running a debugger. This mechanismcould be used, for example, to allow functions in the C library, such as printf() andscanf(), to use
 the screen and keyboard of the host rather than having a screenand keyboard on the target system.  
    This is useful because development hardware often does not have all the inputand output facilities of the final system. Semihosting allows the host computer toprovide these facilities.  
    Semihosting is implemented by a set of defined software interrupt (SWI)operations.  
    The application invokes the appropriate SWI and the debug agent then handlesthe SWI exception. The debug agent provides the required communication with thehost.  
    In many cases, the semihosting SWI will be invoked by code within libraryfunctions. The application can also invoke the semihosting SWI directly. Referto the C library descriptions in the ADS Compilers and Libraries Guide for moreinformation on support
 for semihosting in the ARM C library.  
     按我的理解，这个模式是用来调试的，通过仿真器，使用主机的输入输出代替单片机自己的，也就是说即便单片机没有输出口也能printf到电脑上。反过来，由于这个模式更改了printf（）等的实现方式，输入输出就不走单片机的外设了，所以只重定义fputc不起作用。 
用代码关闭此模式后，需要同时更新一下__stdout 和__stdin 的定义，所以有后面的语句。 
    以上仅为个人理解，如有错误请指正。  
    另外，勾选microlib之后，也许编译的时候就不把开启semihosting的文件包进去了，所以没事。 
     以上方法就成功的将printf引进过来直接调用了，但是个人觉得还是比较麻烦，比如在STM32和LPC环境下就必须更改底层串口接口寄存器。如此就引来了另外一个C标准库函数vsprintf()。
函数名: vsprintf 
     功 能: 送格式化输出到串中 
     用 法: int vsprintf(char *string, char *format, va_list param); 
     代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/vincent_fang/article/details/41348781#)[copy](http://blog.csdn.net/vincent_fang/article/details/41348781#)
[print](http://blog.csdn.net/vincent_fang/article/details/41348781#)[?](http://blog.csdn.net/vincent_fang/article/details/41348781#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/527752/fork)
- #include <stdio.h>
- #include <stdarg.h>
- 
- void  UART_PrintStr (const uint8_t * str)  
- {  
- 
- while ((*str) != 0) {  
- if (*str == '\n') {  
-          UART_PrintChar(*str++);  
-          UART_PrintChar('\r');  
-       } else {  
-          UART_PrintChar(*str++);  
-       }      
-    }  
- }  
- 
- /*********************************************************************//**
-  * @brief     print formatted string. This function takes variable length arguments
-  * @param[in]     variable length arguments
-  * @return    None.
-  **********************************************************************/
- void  UART_Printf (constvoid *format, ...)  
- {  
- static  uint8_t  buffer[40 + 1];  
- va_list     vArgs;  
- 
-     va_start(vArgs, format);  
-     vsprintf((char *)buffer, (charconst *)format, vArgs);  
-     va_end(vArgs);  
-     UART_PrintStr((uint8_t *) buffer);  
- }  
    VA_LIST的用法（ VA_LIST 是在C语言中解决变参问题的一组宏）：
  （1）首先在函数里定义一具VA_LIST型的变量，这个变量是指向参数的指针
  （2）然后用VA_START宏初始化变量刚定义的VA_LIST变量，这个宏的第二个参数是第一个可变参数的前一个参数，是一个固定的参数。（如在运行VA_START(ap,v)以后，ap指向第一个可变参数在堆栈的地址。）
  （3）然后用VA_ARG返回可变的参数，VA_ARG的第二个参数是你要返回的参数的类型。
  （4）最后用VA_END宏结束可变参数的获取。然后你就可以在函数里使用第二个参数了。
    如果函数有多个可变参数的，依次调用VA_ARG获取各个参数。
     vsprintf的功能就是将为止大小的输入进行重组，之后使用串口输出，底层还是运用单片机的串口输出函数。
     综上所诉，个人更倾向于使用vsprintf进行修改，方便快捷。
