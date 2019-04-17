# GPIO - DoubleLi - 博客园







一、什么是GPIO？



    首先应该理解什么是GPIO。GPIO，英文全称为General-Purpose IO ports，也就是通用IO口。在嵌入式系统中常常有数量众多，但是结构却比较简单的外部设备/电路，对这些设备/电路有的需要CPU为之提供控制手段，有的则需要被CPU用作输入信号。而且，许多这样的设备/电路只要求一位，即只要有开/关两种状态就够了，比如灯亮与灭。对这些设备/电路的控制，使用传统的串行口或并行口都不合适。所以在微控制器芯片上一般都会提供一个“通用可编程IO接口”，即GPIO。接口至少有两个寄存器，即“通用IO控制寄存器”与“通用IO数据寄存器”。数据寄存器的各位都直接引到芯片外部，而对数据寄存器中每一位的作用，即每一位的信号流通方向时输入还是输出，则可以通过控制寄存器中对应位独立的加以设置。这样，有无GPIO接口也就成为微控制器区别于微处理器的一个特征。



    在实际的MCU中，GPIO是有多种形式的。比如，有的数据寄存器可以按照位寻址，有些却不能按照位寻址，这在编程时就要区分了。比如传统的8051系列，就区分成可位寻址和不可位寻址两种寄存器。另外，为了使用的方便，很多mcu把glue logic等集成到芯片内部，增强了系统的稳定性能，比如GPIO接口除去两个标准寄存器必须具备外，还提供上拉寄存器，可以设置IO的输出模式是高阻，还是带上拉的电平输出，或者不带上拉的电平输出。这在电路设计中，外围电路就可以简化不少。



    明白了这个道理，不同的MCU，提供的GPIO口的数目不同，可选择的glue logic也不同。所以，在了解共性的基础上去了解个性。



    另外需要注意的是，**对于不同的计算机体系结构，设备可能是端口映射，也可能是内存映射的。如果系统结构支持独立的IO地址空间，并且是端口映射，就必须使用汇编语言完成实际对设备的控制，因为C语言并没有提供真正的“端口”的概念。如果是内存映射，那就方便的多了。**举个例子，比如像寄存器A（地址假定为0x48000000）写入数据0x01，那么就可以这样设置了。



|```#define A (*(volatile unsigned long *)0x48000000)...    A = 0x01;...```|
|----|




    这实际上就是内存映射机制的方便性了。其中volatile关键字是嵌入式系统开发的一个重要特点。这个就不再这里总结了。上述表达式拆开来分析，首先(volatile unsigned long *)0x48000000的意思是把0x48000000强制转换成volatile unsigned long类型的指针，暂记为p，那么就是#define A *p，即A为P指针指向位置的内容了。这里就是通过内存寻址访问到寄存器A，可以读/写操作。

我们在这里就来看看通常在嵌入式c编程中是如何来操作这些可内存寻址的寄存器：

#define CTL_REG_READ(addr)  (*(volatile unsigned long *)(addr))
#define CTL_REG_WRITE(addr, val) (*(volatile unsigned long *)(addr)=(var))



二、S3C2410的GPIO的特点



    首先看看introduction。



|`· 117-bit general purpose I/O ports / 24-ch external interrupt source`|
|----|




    可见，s3c2410的GPIO有117pin，下面应该到9 IO ports看看详细部分了。



|```The S3C2410X has 117 multi-functional input/output port pins. The ports are:— Port A (GPA): 23-output port— Port B (GPB): 11-input/output port— Port C (GPC): 16-input/output port— Port D (GPD): 16-input/output port— Port E (GPE): 16-input/output port— Port F (GPF): 8-input/output port— Port G (GPG): 16-input/output port— Port H (GPH): 11-input/output port```|
|----|




    这么多的IO口，相当于把117个io port划分为8个组，每个组也叫一个Port，每个Port控制相应数量个port，其实很多是复合功能的，既可以作为普通的IO口使用，也可以作为特殊外设接口。在程序设计时，要对整体的资源有所规划，初始化时就应该把所有资源安排合理。这样才会避免出现问题。当然，仅仅做一个最简单的led灯实验，倒是省去了很多步骤。



    现在的8个端口，针对于每个端口都存在上面提到的两个寄存器，其寄存器是相似的。除了两个通用寄存器**GPxCON、GPxDAT**外，还提供了**GPxUP**用于确定是否使用内部上拉电阻（其中x为A-H，需要注意的是没有GPAUP）。应用的主要步骤就是：



    ·设置GPIO控制寄存器GPxCON

    ·设置GPIO上拉寄存器GPxUP



    初始化完成后，就可以通过对GPxDAT的操作来实现相应的应用了。其中，PORT A与PORT B-H在功能选择方面有所不同，**GPACON的每一位对应一根引脚（共23pin有效）。当某位设为0，相应引脚为输出引脚，因为Port A控制的23个pin只能进行输出，所以也就没有输入的控制**，此时往GPADAT对应的位中写0/1，可以让引脚输出低电平/高电平；当某位设为1，则相应引脚为地址线，或者用于地址控制，此时GPADAT没有用了。一般而言，GPACON通常全设为1，以便访问外部存储器件。PORT B-H在寄存器操作方面完全相同。**GPxCON中每两位控制一根引脚**：**00表示输入，01表示输出，10表示特殊功能，11保留**。GPxDAT用于读/写引脚：当引脚设为输入时，读此寄存器可知相应引脚状态是高/低；当引脚设为输出时，写此寄存器相应位可以使相应引脚输出低电平或高电平。GPxUP：某位设为0，相应引脚无内部上拉；为1，相应引脚使用内部上拉。关于特殊功能，那就得结合特殊外设来进行设置了。



    这算是最简单的部分。完成一个led灯实验，可以用来做后续实验的调试手段。



基本实验一：LED灯循环点亮



    在EDUKIT-III实验箱上，有四个LED灯，与IO口的对应关系为GPF[7:4]----LED[4:1]。当IO引脚输出为低电平的时候，LED灯被点亮。只需要关注三个寄存器GPFCON、GPFDAT、GPFUP。由于硬件电路的关系，设置上拉电阻与否并不影响LED灯的点亮，所以GPFUP可以不必考虑。剩下的就是GPFCON和GPFDAT。



    我参考了《S3C2410完全开发》和vivi源代码，对前者的源代码进行了完善和修正，形成了两个版本。版本1是采用ARM汇编语言完成，版本2采用C语言完成。版本1练习了宏定义函数，子程序等，相对而言比较简单。版本2重点练习了软件架构，虽然短小，但是仍然模仿了vivi的软件架构。只是没有必要写复杂的Makefile，所以只写了比较简单的Makefile。在编写过程中，发现自己对ld，objcopy，和一些细节没有很好的把握，经过查看资料，已经基本掌握，后续工作需要就这些工具进行深入的学习，目标是能够熟练掌握。



ARM汇编版本：

@ register address
.equ WTCON, 0x53000000
.equ GPFCON, 0x56000050

@ offset value
.equ oGPFDAT, 0x04

@ macro defination LED_ON
@ you should initial IO pins about leds in order to use this macro
**.macro** LED_ON led_value //注意 在这里定义了一段宏，相当于函数，在下面可以直接通过宏名+参数值来调用该宏
 ldr r1, =/led_value
 str r1, [r0, #oGPFDAT] //将r1中的值赋给0x5300000054寄存器中，也就是GPFDAT寄存器
 bl delay //调用延时子程序
**.endm**

.text
.global _start @ specified by GNU ld. Here is
  @ ultimately 0x00000000,and you
  @ can fine this in Makefile.

_start:
 @ disable watch dog timer
 @ otherwise mcu will reset at fixed interval, and
 @ you will find led on and off in abnormal way.
 mov r0, #WTCON
 mov r1, #0x00
 str r1, [r0]

 @ initial IO pins: GPF[7:4]
 @ please read datasheet //开始初始化pin对应的Port控制寄存器和上拉寄存器
 ldr r0, =GPFCON
 ldr r1, =0x5500  //为什么要将GPFCON寄存器设置为0x5500就要一位位的来分析GPFCON寄存器，因为4个LED灯与IO口的对应关系为：GPF[7:4]----LED[4:1]，我们知道在Port F中每两位来控制一个引脚，而且要将引脚设置为输出才能控制LED，所以GPF7也就是[15:14]=01,GPF6也就是[13:12]=01,同理GPF5也就是[11:10]=01,GPF4也就是[9:8]=01,所以GPFCON要被设置为0x5500
 str r1, [r0]

 @ start to light the leds
 @ led on when low voltage
1:  //循环的依次点亮4个LED灯，也就是依次将下面的值设置给GPFDAT寄存器，通过下面的0xd0,0x70,0xe0,0xb0可以知道在GPFDAT寄存器中的[7:4]来控制LED的亮灭。
 LED_ON 0xd0
 LED_ON 0x70
 LED_ON 0xe0
 LED_ON 0xb0

 b 1b

 @ meaningless but readable
stop:
 b stop

@
@ SUB Routine
@

@ not accurately, but good effect
delay:
 mov r2, #0x10000
2:
 subs r2, r2, #0x1
 bne 2b

 mov pc, lr

.end

    （1）我采用了延时子程序，但是上电复位后，WDT默认是打开的。所以在程序的开始要禁用WDT。

    （2）关于ARM的跳转指令B、BL、BX要区分开。B一般用于本段内的指令跳转，而BL用于子程序调用，BX用于ARM和THUMB状态的切换。特别地说，BL指令会将下一条指令的地址拷贝到LR中，然后跳转到指定的地址运行程序。所以，子程序调用的模型为：

    bl delay

    ...



delay:

    ...

    mov pc, lr



    明确了这两条，程序就不难理解了。源代码见上传的附件。



C语言版本：



    （1）软件架构仿照了vivi，也可以说是Linux Kernel。当然，仅仅写这么小的程序用不到这么麻烦，但是可以训练这种架构，为写中型大型程序打好基础。

    （2）注意C语言下实现寄存器读写的(*(volatile unsigned long *)(addr))。其实就是要掌握volatile和指针的用法，明白在嵌入式环境中，为什么要这样操作。

    （3）写c时，要注意头文件如何处理。写Makefile时，要注意是否采用隐含规则，如果不采用，就要自己定义明确规则，就像vivi里面的Rules.make。在这里，因为只是涉及到.s的编译不采用隐含规则，所以没有把Rules.make单独拿出，事实上可以单独写为Rules.make，然后在Makefile后加入include Rules.make就可以了。


    （4）**要调用C子程序，必须分配堆栈空间。因为子程序调用时，要进行入栈出栈处理。又因为从nand flash启动，而nand flash在S3C2410下的特点规定堆栈不能超过4K。**



**先来看看利用C语言时对寄存器的操作：**



#define GPIO_CTL_BASE 0x56000000  //定义GPIO控制寄存器的起始地址也就是GPACON的地址
#define bGPIO(p, o) CTL_REG_READ(GPIO_CTL_BASE + (p) + (o))

/* offset */
#define oGPIO_CON 0x0
#define oGPIO_DAT 0x4
#define oGPIO_UP 0x8

#define oGPIO_F  0x50  //GPFCON寄存器相对应与GPADAT的偏移

/* registers */
#define GPFCON  bGPIO(oGPIO_F, oGPIO_CON)
#define GPFDAT  bGPIO(oGPIO_F, oGPIO_DAT)
#define GPFUP  bGPIO(oGPIO_F, oGPIO_UP)



然后来看看C代码：



#define DELAYTIME 0x5000

void delay(unsigned long n);

int main(void)
{
 unsigned char i;
 unsigned long led[4] = {0xd0, 0x70, 0xe0, 0xb0};

 GPFCON = vGPFCON; //同样vGPFCON已经初始化为0x00005500，将这个值赋值给GPFCON

 while (1) {
  for (i=0; i<4; i++) {
   GPFDAT = led[i];
   delay(DELAYTIME);
  }
 }

 return 0;
}

/* delay some time */
void delay(unsigned long n)
{
 while (n--) {
  ;
 }
}















