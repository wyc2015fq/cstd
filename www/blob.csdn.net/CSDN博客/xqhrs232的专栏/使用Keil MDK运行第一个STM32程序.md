# 使用Keil MDK运行第一个STM32程序 - xqhrs232的专栏 - CSDN博客
2012年12月16日 12:09:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1178
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://www.cnblogs.com/emouse/archive/2012/03/09/2388253.html](http://www.cnblogs.com/emouse/archive/2012/03/09/2388253.html)
## emouse原创文章，转载请注明出处[http://www.cnblogs.com/emouse/](http://www.cnblogs.com/emouse/)
应部分网友要求，最新加入固件库以及开发环境使用入门视频教程，同时提供例程模板，个人录制，欢迎指正。下载地址：
[http://115.com/file/e78l0xlo# emouse-STM32系列视频教程.rar](http://115.com/file/e78l0xlo#%20emouse-STM32%E7%B3%BB%E5%88%97%E8%A7%86%E9%A2%91%E6%95%99%E7%A8%8B.rar)
欢迎大家针对本博客文章提出宝贵意见。
关于使用固件库建立工程请参考
### [使用Keil MDK以及标准外设库创建STM32工程](http://www.cnblogs.com/emouse/archive/2012/03/01/2375146.html)
关于固件库的学习请参考
### [STM32固件库详解](http://www.cnblogs.com/emouse/archive/2011/11/29/2268441.html)
##### 1.1.1 使用Keil MDK运行第一个STM32F10X程序
在上一小节中已经详细介绍了使用Keil MDK和标准外设库创建一个工程的过程，下面将介绍基于这个工程来编写一个小程序，通过这个程序我们可以初步了解：
l STM32标准外设库的简单使用过程
l STM32外设的使用方法和大致流程
l 程序的编译、链接、下载步骤
l 利用Keil MDK的在线仿真功能进行软件仿真的简要步骤
###### 1. 程序的编写
（1）程序实现的功能
为了方便各位读者的入门和理解，这个小程序的功能非常简单，作为本书功能实践的第一个程序，其功能当然也是最为经典的“Hello World！”了，只不过不是简单的屏幕输出，而是利用硬件的串口进行输出，同时作为单片机类的第一个程序自然少不了LED闪烁的功能，这就是这个小程序的两个主要的功能：
l 利用串口1输出“Hello World！”字符。
l 控制两个LED闪烁。
（2）程序的实现
在基于标准外设库进行程序开发时一定要充分利用标准外设库下面的帮助文件stm32f10x_stdperiph_lib_um.chm以及库中自带的工程实例，同时结合STM32F10x系列的芯片手册来完成程序的开发，stm32f10x_stdperiph_lib_um.chm帮助文件如图 5‑25所示。此文件中已经包含了标准外设库的全部内容，并根据根据内容结构进行了重新的编排和整理，更加方便程序的阅读和理解。
STM32F10x_StdPeriph_Lib_V3.5.0\Project\STM32F10x_StdPeriph_Examples文件夹下包括了众多外设的使用例程，这里的例程多是针对官方的开发板而编写，然而这些例程却非常全面的展示了相关外设的各种使用方法，对我们做基于标准外设库的开发有着非常重要的意义。
![clip_image002[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005064126.png)
图 5‑25 标准外设库的帮助文档
我们的第一个程序就通过这些参考来完成，首先来看串口部分。串口部分的详细信息可以参考本书第8章，这儿只进行简单的说明和介绍简便的开发方法，首先先找一个与我们使用的功能最近的一个例程，STM32F10x_StdPeriph_Lib_V3.5.0\Project\STM32F10x_StdPeriph_Examples\USART文件夹下给出了多达12中的USART例程，这里选择较为相近的Interrupt文件夹下得例程，打开文件夹下面的mian.c文件，通过简单的浏览可以找到如下一段程序：
/* USARTy and USARTz configuration ------------------------------------------------------*/
/* USARTy and USARTz configured as follow:
- BaudRate = 9600 baud
- Word Length = 8 Bits
- One Stop Bit
- No parity
- Hardware flow control disabled (RTS and CTS signals)
- Receive and transmit enabled
*/
USART_InitStructure.USART_BaudRate = 9600;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
USART_InitStructure.USART_StopBits = USART_StopBits_1;
USART_InitStructure.USART_Parity = USART_Parity_No;
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
/* Configure USARTy */
USART_Init(USARTy, &USART_InitStructure);
/* Configure USARTz */
USART_Init(USARTz, &USART_InitStructure);
/* Enable USARTy Receive and Transmit interrupts */
USART_ITConfig(USARTy, USART_IT_RXNE, ENABLE);
USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);
/* Enable USARTz Receive and Transmit interrupts */
USART_ITConfig(USARTz, USART_IT_RXNE, ENABLE);
USART_ITConfig(USARTz, USART_IT_TXE, ENABLE);
/* Enable the USARTy */
USART_Cmd(USARTy, ENABLE);
/* Enable the USARTz */
USART_Cmd(USARTz, ENABLE);
上面的这段的程序配合注释可以很容易理解，USART_InitTypeDef定义了一个包括USART主要参数的结构体，因此首先对USART的相关参数进行配置，使用标准外设库进行配置的优势就体现出来了，通过程序可以很容易读出这个串口的配置：
l 波特率9600Kbps
l 数据长度8
l 停止位1
l 奇偶校验：无
l 硬件流控制：无
l 工作模式：收、发
然后利用USART_Init函数进行初始化，这段程序中设置了两个串口，使用同样的配置，然后配置相应的中断。最后通过USART_Cmd函数使能相应的串口，前面有过介绍，这些例程里的程序是针对官方的开发套件的，因此程序中并没有指名具体的端口，而是使用了宏定义USARTy、USARTz。通过这段程序就可以很方便的更改相关的参数得到我们需要的配置程序。
这儿只是完成了USART的配置，下面来看一下对应的I/O设置，仍然在这个文件中可以找到GPIO_Configuration(void)这个函数，程序如下：
void GPIO_Configuration(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
#ifdef USE_STM3210C_EVAL
/* Enable the USART3 Pins Software Remapping */
GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
/* Enable the USART2 Pins Software Remapping */
GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#elif defined USE_STM3210B_EVAL || defined USE_STM32100B_EVAL
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
这段函数完成了相关的I/O配置，首先通过宏定义判断是官方的哪一款开发套件，然后进行相应的端口映射（端口映射请参加官方的数据手册），然后进行相应的端口配置，同串口配置一样，这段程序中德端口用的也是宏定义USARTy_RxPin替代的，改为我们使用的实际I/O，端口时钟设置为50MHz，串口所使用到的端口设置为复用（GPIO_Mode_AF_PP）完成端口初始化。
另外仍然通过观察这个例程可以很容易发现，在使用一个外设时还需要首先打开对应的外设时钟，这部分程序如下：
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
这段程序中需要注意两点，首先，GPIO、USART等都是连在APB1、APB2两条总线上的，各外设具体的总线连接情况参见图 5‑2，因此首先应该确定外设对应的总线，例如USART1是APB2总线，而USART2是APB1总线。其次使能相应的时钟时不光要使能对应的I/O端口，还要使能总线的复用端口，这点也容易忽略。
最后根据库中的例程，借鉴库中例程的编写风格，就可以得出我们需要的程序，程序在工程的mian.c中编写，函数如下：
#include "stm32f10x.h"
void USART_Configuration(void);
void GPIO_Configuration(void);
void Delay(__IO uint32_t nCount);
void USART1_Puts(char * str);
int main(void)
{
USART_Configuration();
GPIO_Configuration();
USART1_Puts("Hello Wrold!\n");
while (1)
{
GPIOF->BSRR = 0x000000C0;
Delay(0xAFFFF);
GPIOF->BRR = 0x000000C0;
Delay(0xAFFFF);
USART1_Puts("Hello Wrold!\n");
}
}
void USART_Configuration(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
//使能串口、串口所用的I/O口以及端口复用时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);
/* A9 USART1_Tx */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //推挽输出-TX
GPIO_Init(GPIOA,&GPIO_InitStructure);
/* A10 USART1_Rx */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入-RX
GPIO_Init(GPIOA, &GPIO_InitStructure);
USART_InitStructure.USART_BaudRate = 9600;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
USART_InitStructure.USART_StopBits = USART_StopBits_1;
USART_InitStructure.USART_Parity = USART_Parity_No;
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
USART_Init(USART1, &USART_InitStructure);
/* Enable the USARTx */
USART_Cmd(USART1, ENABLE);
}
void GPIO_Configuration(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
/* 设置LED对应的引脚 */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOF, &GPIO_InitStructure);
}
void Delay(__IO uint32_t nCount)
{
for(; nCount != 0; nCount--);
}
void USART1_Puts(char * str)
{
while(*str)
{
USART_SendData(USART1, *str++);
/* Loop until the end of transmission */
while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
}
至此完成了主函数的编写。接下来还需要到stm32f10x_conf.h文件中选择相应的头文件，这儿去掉需要使用的头文件之前的注释，去掉注释的头文件如下：
l #include "stm32f10x_gpio.h"
l #include "stm32f10x_gpio.h"
l #include "stm32f10x_usart.h"
这样，我们的程序编写就完成了，下面可以进行我们的编译与调试了。
###### 2. 程序的编译
在Keil MDK的工具栏上自左至右依次有![clip_image004[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005064616.jpg)三个图标，分别为编译指定文件、生成工程、重新生成所有工程。首先在mian.c下点击编译图标，如果编译没有错误，则输出窗口如图
 5‑26所示。如果编译存在错误，点击错误的地方可以直接定位到错误行，根据错误提示进行修改。
![clip_image006[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005082499.jpg)
图 5‑26 编译输出窗口
编译不存在错误后点击![clip_image008[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005083895.jpg)图标生成工程，生成没有错误则输入如图
 5‑27所示。如果存在错哦无则可以直接点击对应的错误跳转到对应的文件对应的行，根据错误提示进行再次修改。
![clip_image010[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005088006.jpg)
图 5‑27 生成工程输出窗口
第一次生成工程成功以后，如果修改了相应的文件再次点击生成工程时只会对有改动的文件进行重新编译、生成，而重新生成全部工程会重新生成工程中的所有文件。
###### 3. 程序的调试
在实际开发时，可以首先利用软件进行仿真，利用软件进行仿真查看具体的寄存器配置，外设的工作情况等，同时通过断点的配合可以查找出程序中绝大多数的错误。软件仿真完成后可以下载到硬件平台去运行，如果程序仍然没有达到需要的效果，在硬件检查完成后可以使用在线仿真的方式，在硬件平台上直接运行程序，进一步查找问题。
首先，先看一下这个小程序的软件仿真，首先在工程设置Debug选项中选择使用软件模拟，如图 5‑28所示。
![clip_image012[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005103140.png)
图 5‑28 Debug设置选项窗口
点击![clip_image014[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005109760.jpg)图标，或者从菜单栏-Debug启动调试。启动后界面如图
 5‑29所示。调试启动后自动运行到主程序。
![clip_image016[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005149331.png)
图 5‑29 调试主界面
在工具栏可以找到与调试相关的快捷图标![clip_image018[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005153692.jpg)，同样在菜单栏Debug菜单下可以找到对应的选项，自左向右分别为复位，运行，点击后直接运行程序；停止，运行时点击停止当前运行；单步，单步执行当前程序；跳过，可以直接跳过子函数的运行；跳出，在某个子函数中运行时可以直接跳出当前函数；运行到光标位置处。
在程序行号的左侧直接双击可以非常方便的添加或者取消断点，如图 5‑30所示。
![clip_image020[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/20120309200517462.jpg)
图 5‑30 断点的添加与取消
在菜单Peripherals中可以打开需要查看的外设，这里点击Peripherals-USART1弹出USART1的相关观察窗口，点击运行后，窗口如图 5‑31所示，就会显示运行的程序对USART1所完成的相关配置。从中可以清晰的看出，程序对USART1的相关寄存器配置，从地面Settings一栏中则可以直观的看出USART1当前的工作状态，从这儿就可以检查我们的相关参数设置正确与否，对应时钟有无开启等。需要说明的是这里寄存器在进行仿真的时候是可以进行改写的，也就是说允许我们在程序运行的时候直接进行相关的操作来观察运行结果。但是此时程序本身也是可以对寄存器进行操作的，因此直接更改相应的寄存器观察时还要注意你的操作是否和程序冲突。例如在本程序的运行过程中如果将寄存器中发送使能TE的勾给去掉，则串口就不会在输出数据。
![clip_image022[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005188378.png)
图 5‑31 USART1寄存器观察窗口
接下来继续查看I/O的配置，本程序中使用的是F端口，则点击菜单Peripherals-General-Purpose I/O，选择GPIOF，出现GPIOF的配置窗口，程序运行后可以通过窗口查看GPIOF的相关配置，如图 5‑32所示。在这个窗口中可以查看引脚的配置情况，当程序运行时就能看到对应引脚的勾在闪烁。
![clip_image024[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005218015.png)
图 5‑32 GPIOF寄存器观察窗口
通过这些操作，可以非常方便、直观的查看程序运行后对应的配置是否与我们预想的一致，那么在本程序中通过串口输出了“Hello World！”又该怎样查看运行结果呢？在工具栏![clip_image026[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005251522.jpg)中可以非常方便的打开相应的查看窗口，查看相关寄存器和外设等。由于我们需要使用串口1输出字符，同时LED闪烁显示，因此首先打开串口1的输出，点击![clip_image027[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/2012030920052661.png)图标，弹出如所示下拉菜单，选择UART#1如图
 5‑33所示，或者点击View菜单点击Serial Windows选择UART#1即可打开UART的输出窗口，运行程序后会在此窗口中输出串口信息，如图 5‑34所示。
![clip_image029[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005262536.jpg)
图 5‑33 打开UART1输出窗口
![clip_image031[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005308759.jpg)
图 5‑34 UART1输出窗口
至此，已经利用Keil MDK的软件仿真功能完成了程序的调试，看到了设计的效果。通过这个例子也能够让我们深刻的体会到Keil MDK在线调试功能的强大之处。接下来我们将编写好的程序下载到开发板中运行，看看运行效果。
###### 4. 程序的下载
编写好的程序工程生成完成后下载到硬件平台的方法有很多种，一是直接利用Flash烧写工具烧写相应的hex文件，在工程配置时需要设置才能生成hex文件，具体设置参加图 5‑16，生成hex文件之后就可以利用烧写工具烧写到硬件平台，常用的仿真器都带有相应的软件工具，如笔者使用的JLink-V8，可以使用自带的J-Flash工具，使用J-Flash工具如图 5‑35所示，在工程管理中选择所使用的硬件平台，点击Target-Connect后就可以连接上硬件平台，在图中左侧显示了仿真器以及对应的硬件平台信息。然后选择所生产的hex文件，就可以讲编译的程序下载到硬件平台中。
![clip_image033[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005332333.png)
图 5‑35 J-Flash主界面
这种下载方式非常适合将程序源码不公开的情况下将程序固件提供给第三方使用，然而在使用Keil MDK时还有更为方便的方式，在工程设置中设置好了下载工具后可以点击![clip_image035[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005337283.jpg)图标，直接将程序下载到硬件平台中运行，非常方便。在使用仿真器进行在线仿真时也可以通过设置在在线仿真时将程序下载到flash中。
程序下载到硬件平台后通过实际的串口来查看程序运行情况了，将硬件平台的串口1连接至PC，笔记本等电脑上没有物理串口的可以使用USB转串口设备，注意在设备管理器中查看实际所分配的串口号，本机分配的串口号为COM4，如图 5‑36所示，串口号也可以在高级选项中进行更改。
![clip_image037[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005343314.jpg)
图 5‑36 设备管理器中查看串口号
这里使用本章所介绍的TKStudio中的串口调试助手工具来进行查看，设置串口的相应参数与程序一致，串口设置于运行结果如图 5‑37所示，从图中可以看出串口接数据正确，开发板上的两个LED也在不停闪烁。
![clip_image039[6]](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203092005366496.png)
图 5‑37 串口调试助手运行界面
至此，我们完成了一个简单的程序开发过程。本小节通过实现串口输出与LED闪烁的功能系统的介绍了怎样基于标准外设库进行程序的开发与调试，这儿只是列举了一个非常简单的小例子，更多的联系读者可以参考标准外设库中的例程自行完成。
★emouse 思·睿博客文章★ 原创文章转载请注明：http://emouse.cnblogs.com 交流请加QQ群：237389191 同时欢迎邮件联系：emouse2010@163.com
分类: [嵌入式系统原理与接口技术-连载](http://www.cnblogs.com/emouse/category/326054.html), [STM32设计相关](http://www.cnblogs.com/emouse/category/326055.html)
