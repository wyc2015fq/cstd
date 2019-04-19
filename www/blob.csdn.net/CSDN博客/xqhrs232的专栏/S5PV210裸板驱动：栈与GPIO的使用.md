# S5PV210裸板驱动：栈与GPIO的使用 - xqhrs232的专栏 - CSDN博客
2013年12月09日 23:18:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：809
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.chinaunix.net/uid-25014876-id-3546958.html](http://blog.chinaunix.net/uid-25014876-id-3546958.html)
如果裸板代码都用汇编代码编写的话，那确实是难度大，所以还是选择使用C语言来写，这样的话就会涉及栈了。接下来慢慢介绍
一。栈的作用
本想借着这节介绍一下Crotex-A8,的工作模式和寄存器的，但写到一半还是决定放弃，先撇开硬件上的关系，简单介绍一下栈的作用和怎么使用吧，另外的等迟点再说。
栈是在内存上分配的一块内存，当需要存放数据到栈里时，称为入栈，反之称为出栈。同时栈的数据先进后出，栈是往下延伸的，越后存放的数据，存放数据的地址越小。
我们现在的裸板代码是使用IRAM的，上一章也介绍了BL0会有分配栈空间的操作，由下图看看IRAM是怎么分配的。
![](http://blog.chinaunix.net/attachment/201303/26/25014876_1364298112hruZ.png)
这是IRAM内存分布图，之前介绍过，0xD0020000往后的16KB是存放BL0代码的210有8种工作模式，不同工作模式需要分配不同的栈，0xD0037780~0xD0037D80的1.5KB是SVC模式下的栈，0xD0037D80～0xD0037F80的512B是IQR模式下的栈。目前210工作在SVC模式下。
在代码中，栈需要有一个栈指针SP，用来表示当前可以入栈的地址。刚开始时，栈里没数据，SP=0xD0037D80，当入栈一个32位数据后，SP地址加4，指向0xD0037D84，如此类推。
那栈是用来干什么的呢，一般常见有三种
1.在调用函数前，要保存当前函数的数据，如一些寄存器里面的值，还有函数的返回地址，不然等调用函数结束后，代码就没办法返回原来的函数和继续操作原来的值
2.存放调用参数，一般代码会默认使用前几个寄存器来作为函数传参，但参数太多的时候，就只能通过存放在栈里面。因为栈指针的地址是对应同一工作模式下的所有函数是可见的。
3.保存临时变量。
接下来就稍稍修改一下代码，让汇编转跳到C代码执行
1_ARM/1_start/2/start.S
1 #define WTCON 0xE2700000
2
3 .text
4 .align 2
5 .global _start
6
7 _start:
8 //close the watchdog
9 ldr r0, =WTCON
10 mov r1, #0
11 str r1, [r0]
12
13 ldr sp, =0xD0037D80
14
15 bl main
16
17 loop:
18 b loop
在原来的基础上添加了一句 ldr sp, =0xD0037D80 ，指定栈空间的地址。
其实关开门狗和设置栈空间在这里都是多此一举的，因为在BL0的固化代码里面已经有这样的操作了，这里只是我在代码上强调一下而已。
接着就要实现main函数了。
1_ARM/1_start/2/main.c
1 int main(void)
2 {
3 return 0;
4 }
什么操作都没做。
编译后这个代码是没有实质操作的，我只是想让大家看看编译后的反汇编代码。
1_ARM/1_start/2/start.dis
19 00000020 <main>:
20 20: e52db004 push {fp} ; (str fp, [sp, #-4]!)
21 24: e28db000 add fp, sp, #0
22 28: e3a03000 mov r3, #0
23 2c: e1a00003 mov r0, r3
24 30: e28bd000 add sp, fp, #0
25 34: e8bd0800 pop {fp}
26 38: e12fff1e bx lr
27
在进入main函数后，因为接下来的操作会更改fp寄存器，所以第一步的操作就是把fp原来的值入栈，函数执行完后出栈。
二.GPIO的使用
到目前为止，我们已经知道了芯片启动的过程，并且已经跳转到C代码，接下来我们就开始用C语言来编写裸板代码。我们先从GPIO入手。
最简单的寄存器操作需要两步：
1。配置GPIO的功能。
GPIO可以配置成输入，输出，中断或者其他的特定的功能，如配置成串口功能
2。设置GPIO的值
如果配置成输出，我们就要设置GPIO输出的值，是1还是0
如果配置成输入，我们就要想办法获取输入的值是多少
当然，除了这两步外，还有其他的操作，譬如配置上拉电阻，配置驱动能力等。
接下来我们尝试通过GPIO管脚点亮LED灯。
我们先看看原理图
![](http://blog.chinaunix.net/attachment/201303/26/25014876_136429819823xx.png)
我们尝试让D22这个LED闪烁，这是通过管脚GPJ0_3控制。VDD_IO是供电，想要LED点亮，GPJ0_3必须输出低电平，反之，LED灭。
所以我们要做的事情就是：
1。配置GPJ0_3为输出管脚
2。使GPJ3_0重复输出高低电平
先介绍一下GPJ0的相关寄存器
![](http://blog.chinaunix.net/attachment/201303/26/25014876_1364298241yXtl.png)
GPJ0CON：配置寄存器，配置管脚的功能，如输出，输入以及其他功能
GPJ0DAT：数据寄存器，管脚配置成输出时，可以修改这个寄存器的值来修改管脚的输出，管脚配置成输入时，可以读取寄存器的值来知道管脚的输入。
GPJ0PUD：上/下拉寄存器，配置管脚上的上下拉电阻。
GPJ0DRV：驱动能力寄存器，配置管脚的驱动能力。
GPJ0CONPDN：power
 down模式下的配置寄存器。
GPJ0PUDPDN：power
 down模式下的上下拉配置寄存器。
我们下面的代码只需要配置前两个寄存器，后面的使用默认值就可以了。
在S5PV210中，每一个寄存器都有分配一个地址，对应的地址上存放这寄存器的值。
如寄存器GPJ0CON，它的地址是0xE0200240，寄存器的默认值就是0x00000000。
如果我们需要往GPJ0CON寄存器写入0x11111111。需要怎么做呢？
1.定义地址0xE0200240位unsigned
 int类型的指针
(unsigned int *) 0xE0200240)
2.获取0xE0200240的值
*(unsigned int *) 0xE0200240)
3.赋值
*(unsigned int *) 0xE0200240) = 0x11111111
4添加volatile，防止编译优化。
(*(volatile unsigned int *) 0xE0200240)) = 0x11111111
在原理图可以看到，GPJ0有8个GPIO，分别是GPJ0_0~~GPJ0_7。
先看配置寄存器GPJ0CON。
![](http://blog.chinaunix.net/attachment/201303/26/25014876_1364298362G1Wz.png)
我们需要修改的是GPJ0_3，在寄存器GPJ0CON的12～15bit，默认值是0000
0000是配置成输入，0001是配置成输出，1111配置成中断，以及其他的功能。
现在我们要配置成输出，即0001，其他的管脚保持为0000。
所以GPJ0CON的值二进制表示为:
0b 0000 0000 0000 0000 0001 0000
 0000 0000
也可以用代码表示为：
(1<< 12)
再看看数据寄存器GPJ0DAT
![](http://blog.chinaunix.net/attachment/201303/26/25014876_1364298449GTgD.png)
GPJ0DAT有8bit，每一个bit，对应一个GPIO。
如果我们配置了GPJ0_3管脚为输出，现在需要输出高电平的话，需要给GPJ0DAT的bit3赋1。
(*(volatile unsigned int *) 0xE0200244)) = (1 << 3)
而上下拉电阻寄存器GPJ0UP
![](http://blog.chinaunix.net/attachment/201303/26/25014876_13642984658Ge3.png)
GPJ0UP是有16个bit，每两个bit对应一个GPIO
GPJ0UP的默认值是0x55555555，代表每个GPIO都配置成下拉有效，即01。
我要将GPJ0_3配置成不需要上下拉电阻，即00。
(*(volatile unsigned int *) 0xE0200248))
 = (0 << 6)
GPJ0DRV寄存器我们就可以不需要配置了，使用默认值就可以。
代码修改如下：
1_ARM/2_gpio/1/main.c
1 #define GPJ0CON (*( volatile unsigned int *) 0xE0200240)
2 #define GPJ0DAT (*( volatile unsigned int *) 0xE0200244)
3 #define GPJ0PUD (*( volatile unsigned int *) 0xE0200248)
4
5 void delay(unsigned int count)
6 {
7 for(; count > 0; count--);
8 }
9
10 int main(void)
11 {
12 GPJ0CON = (1 << 12);
13 GPJ0PUD = (0 << 6);
14
15 while(1){
16 GPJ0DAT = (1 << 3);
17 delay(100000);
18 GPJ0DAT = (0 << 3);
19 delay(10000);
20 }
21 }
编译后，将代码烧入开发板，当按下电源开关，就会看到对应的LED不停闪烁。但是，只要我们松开开关，电源就断开了，这样可不行啊，总不能每次跑程序我都要死死按住不放吧。所以待会还要改进一下开发板的启动函数。接下来先优化一下代码。
上面的操作，是直接给整个寄存器赋值，如果我只要单独赋值的话该如何做呢？
以操作GPJ0DAT为例：
置1：
GPJ0DAT |= (1 << 3);
置零：
GPJ0DAT &= ~(1 << 3);
如果每次修改寄存器都按上面的方法操作，那该多麻烦啊，所以我定义了几个宏：
1_ARM/2_gpio/2/common.h
4 #define vi *( volatile unsigned int * )
5
6 #define SET_ZERO( addr, bit ) ( (vi addr) &= ( ~ ( 1 << (bit) ) ) )
7 #define SET_ONE( addr, bit ) ( (vi addr) |= ( 1 << ( bit ) ) )
8
9 #define SET_BIT( addr, bit, val ) ( (vi addr) = (( vi addr)&=(~(1<<(bit))) ) | ( (val)<<(bit) ) )
10
11 #define SET_2BIT( addr, bit, val ) ( (vi addr) = (( vi addr)&(~(3<<(bit))) ) | ( (val)<<(bit) ) )
12
13 #define SET_NBIT( addr, bit, len, val ) \
14 ( (vi addr) = ((( vi addr)&(~(( ((1<<(len))-1) )<<(bit)))) | ( (val)<<(bit) ) ))
15
16 #define GET_BIT( addr, bit ) ( (( vi addr ) & ( 1 << (bit) )) > 0 )
17
18 #define GET_VAL( addr, val ) ( (val) = vi addr )
19 #define SET_VAL( addr, val ) ( (vi addr) = (val) )
20 #define OR_VAL( addr, val ) ( (vi addr) |= (val) )
文件实现了一下简单的给指定某个寄存器的哪几位赋值的操作，具体不一一细说。
另外我还修改了Makefile
1 CC=/usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
2 LD=/usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-ld
3 OBJCOPY=/usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-objcopy
4 OBJDUMP=/usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-objdump
5
6 START_OBJ=start.o main.o common.o
7
8 start.bin: $(START_OBJ)
9 $(LD) -Ttext 0x0000000 -g $(START_OBJ) -o start
10 $(OBJCOPY) -O binary -S start $@
11 $(OBJDUMP) -D -m arm start > start.dis
12 gcc mkv210_image.c -o mkmini210
13 ./mkmini210 $@ 210.bin
14 cp 210.bin /fastboot
15
16 %.o:%.S
17 $(CC) -g -nostdlib -c -o $@ $<
18
19 %.o:%.c
20 $(CC) -g -nostdlib -c -o $@ $<
21
22 clean:
23 rm start *.o *.bin *.dis mkmini210
1～4行定义的编译工具的简写
第6行指定最终打包成的bin文件需要的.o文件，后续需要添加C文件的话只需要在此处添加就可以
8～14行指定了最终编出来的start.bin的编译方式
16～17行指定了汇编代码的编译规则
19～20行指定了C代码的编译规则
原来的代码也修改如下：
/1_ARM/2_gpio/2/main.c
1 #include "common.h"
2
3 #define GPJ0CON 0xE0200240
4 #define GPJ0DAT 0xE0200244
5 #define GPJ0PUD 0xE0200248
6
7 int main(void)
8 {
9 SET_2BIT(GPJ0CON, 12, 1);
10 SET_VAL(GPJ0PUD , 0); //这里GPJ0PUD全置0，因为默认值配置了下拉
11 //电阻，会让其他LED也发光
12 while(1){
13 SET_ONE(GPJ0DAT, 3);
14 delay(100000);
15 SET_ZERO(GPJ0DAT, 3);
16 delay(100000);
17 }
18 }
接下来就要解决另一个问题，开机后怎么样才能不需要一直按住按键。
先来个原理图：
![](http://blog.chinaunix.net/attachment/201303/26/25014876_1364298503r09T.png)
为了让开发板工作，必须要让MOS管APM2305导通，这样VDD_IN才能输入到右侧元件的管脚7。
现在区域1，当SWI被按下时，区域1与VDD_IN形成了通路，使APM2305导通，所以开发板就上电工作了，一旦我们松开按键，开发板就会断电。
要想松开手开发板还能工作，那就需要区域2的电路了。只要区域2或者区域1有一处与VDD_IN接通就可以了。
而区域2上有一个二极管，只要往二极管的管脚1输入高电平，就能让管脚2，3导通。
所以我们接下来就是要在开机后让EINT0输出高电平即可，而EINT0对应管脚GPH0_0。
代码如下：
3 #define GPJ0CON 0xE0200240
4 #define GPJ0DAT 0xE0200244
5 #define GPJ0PUD 0xE0200248
6 #define GPH0CON 0xE0200C00
7 #define GPH0DAT 0xE0200C04
8 #define GPH0PUD 0xE0200C08
9
10 int main(void)
11 {
12 SET_2BIT(GPJ0CON, 12, 1);
13 SET_VAL(GPJ0PUD, 0);
14 SET_2BIT(GPH0CON, 0, 1);
15 SET_VAL(GPH0PUD, 0);
16
17 SET_ONE(GPH0DAT, 0);
18 while(1){
19 SET_ONE(GPJ0DAT, 3);
20 delay(100000);
21 SET_ZERO(GPJ0DAT, 3);
22 delay(100000);
23 }
24 }
最后我们要介绍一下GPIO作为输入的问题。
需要使用GPIO的管脚功能时，需要两个步骤：
1：通过CON寄存器配置管脚为输入功能
2：通过读DAT寄存器的值来判断当前管脚电平的高低
另外，如果有需要，通过PUD寄存器配置上拉电阻
![](http://blog.chinaunix.net/attachment/201303/26/25014876_13642985285oBi.png)
以按键SW5为例，当按键没有按下是，管脚EINT2检测到的是高电平，当按键按下时，
 管脚EINT2检测到的是低电平，从原理图看EINT2链接的是GPH2_0。
接下来我们就要实现：
当SW5按下，LED
 D22点亮，SW5松开，LED
 D22熄灭
代码如下：
1_ARM/2_gpio/4/main.c
13 int main(void)
14 {
15 SET_NBIT(GPJ0CON, 12, 4, 1);
16 SET_VAL(GPJ0PUD, 0);
17
18 SET_NBIT(GPH0CON, 0, 4, 1);
19 SET_VAL(GPH0PUD, 0);
20 SET_ONE(GPH0DAT, 0);
21
22 SET_NBIT(GPH2CON, 0, 4, 0); //配置GPH2_0为输入管脚
23 SET_2BIT(GPH2PUD, 0, 2); //配置GPH2_0使能上拉电阻
24
25 while(1){
26 if(0 == GET_BIT(GPH2DAT, 0)) //如果检测到低电平，表示按键按下
27 SET_ZERO(GPJ0DAT, 3);
28 else if(1 == GET_BIT(GPH2DAT, 0)) //反之按键松开
29 SET_ONE(GPJ0DAT, 3);
30
31 delay(10000);
32 }
33 }
总结：
到现在为止，所有的内容都介绍问了，主要有两部分：
1：汇编转跳C语言的方法以及堆栈的分配
2：GPIO输入输出的配置和使用
