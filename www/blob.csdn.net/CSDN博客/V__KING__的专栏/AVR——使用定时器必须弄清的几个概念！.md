# AVR——使用定时器必须弄清的几个概念！ - V__KING__的专栏 - CSDN博客





2014年01月02日 15:08:24[v__king__](https://me.csdn.net/V__KING__)阅读数：1302








转自:http://blog.sina.com.cn/s/blog_7880f98301011k8b.html


在MCU中（M16)，定时器是独立的一个模块，M16有三个独立的定时器模块，即T/C0、T/C1和T/C2；其中T/C0和T/C2都是8位的定时器，而T/C1是一个16位的定时器。定时器的工作是独立于CPU之外自行运行的硬件模块。



1、定时器何时开始工作（或说计数）的？

 当TCCR0！=0x00任何模式下，只要MCU一上电，T/C就开始计时工作。其实TCCR0主要是定时器的预分频和波形模式、比较匹配模式的设置，说到预分频，不得不提一下这个模块，这个模块是T/C0、T/C1共用的一个模块，但可以有不同的分频设置。


2、定时器是如何进行工作的：说到定时器的工作，不得不说三个个重要参数：TCNT0、OCR0，TIMSK，TCNT0是设置定时器的计时初始值，定时器开始工作后立即从TCNT0一直累加到0XFF，累加过程所消耗的时间就是我们需要的定时时间；OCR0是一个比较设定值，当TCNT0的值累计到OCR0时（TNCT0==OCR0），如果有开启比较匹配中断功能，那么此时就会产生比较中断，所以，OCR0的值一般都是设置在TCNT0初始值和0XFF之间，之外的任何值都不会产生比较中断。TIMSK是一个中断使能位设置，就是我们需要计时器溢出中断或是比较匹配中断功能或两者都要时就对TIMSK的相应寄存器位进行设置。


3、定时器的中断使用，一个定时器可以有两个中断资源可利用，一个只溢出中断，另一个是比较匹配中断，如上面2所说的。想说明的溢出中断子程序内一般要有重载TCNT0的初始值，否则，TCNT0就会从0X00开始累加计数到0XFF，所耗费的时间就不我们想要的时间。比较中断就是当TCNT0==OCR0时，发生比较匹配中断；所以，中断子程序中一般只插入少量的处理代码，否则，会发生所谓的中断套嵌的现象，由于M16不支持中断套嵌，这样会使得中断子程序中的部分代码无法执行，严重时会造成系统崩溃。


4、TCNT0和OCR0的值换算：对于8bit的计时器，TCNT0一般可以由下面的公式换算：

  TCNT0=256-（TV*F）/N；

  TV： 所想要设定的定时时间，单位，us

      F：晶振频率（MHz）

    N： 分频因子


定时器是独立运行的，它不占用CPU的时间，不需要指令，只有调用对应的寄存器的时候才需要参与。


以AVRmega16为例，它有三个寄存器，timer0，timer1和timer2，T0和T2是8位定时器，T1是16位寄存器，T2为异步定时器，三个定时器都可以用于产生PWM。


以定时器T0来简单介绍定时器的操作方法，T0有三个寄存器可以被CPU访问，TCCR0，TCNT0，OCR0，下面看一段ICC生成的定时器初始化程序。


**CODE:**



//TIMER0 initialize - prescale:8

// WGM: Normal

// desired value: 1KHz

// actual value: 1.000KHz (0.0%)

void timer0_init(void)

{

TCCR0 = 0x00; //stop

TCNT0 = 0x83; //set count

OCR0 = 0x7D; //set compare

TCCR0 = 0x02; //start timer

}


[[Copy toclipboard]](http://bbs.avrvi.com/htm_data/33/0705/2564.html)



TCCR0为控制寄存器，用于控制定时器的工作模式细节；

TCNT0为T/C 寄存器，它的值在定时器的每个工作周期里加一或减一，实现定时操作，CPU可以随时读写TCNT0；

OCR0：输出比较寄存器，它包含一个8 位的数据，不间断地与计数器数值TCNT0进行比较。匹配事件可以用来产生输出比较中断，或者用来在OC0 引脚上产生波形。


这里说最简单的模式，TCNT一直加一，到达最大值0xFF然后清零，进入下一次计数，在上面的程序中。

TCCR0＝0x00;关闭T0的时钟源，定时器停止工作。

TCNT0＝0x83;设置T/C寄存器的初始值，及让定时器从TCNT0从0x83开始定时或计数。

OCR0 = 0x7D;设定比较匹配寄存器的值，这个程序里没有使用。

TCCR0 = 0x02;选择时钟源，来自时钟8分频，设置后定时器就开始工作。


初始化后定时器开始工作，TCNT0在每一个定时器时钟加一，当TCNT0等于OCR0的值时，T/C 中断标志寄存器－TIFR中的OCF0置位，如果这时候TIMSK中OCIE0为1（即允许T0比较匹配中断），并且全局中断允许，比较匹配中断即运行。中断程序中可以对TCNT0和0CR0进行操作，对定时器进行调整。


TCNT0继续加一，当达到0xFF时，T/C 中断标志寄存器－TIFR中的TOV0置位，如果这时候TIMSK中TOIE0为1（即允许T0溢出中断），并且全局中断允许，溢出中断即运行。中断程序中可以对TCNT0和0CR0进行操作，对定时器进行调整。


和定时器相关的寄存器还有SREG和TIMSK，前者位1控制全局中段允许，后者位1（OCIE0）和位0（TOIE0）分别控制比较匹配中断和溢出比较匹配中断允许。


实际的过程中，定时器相关寄存器的操作非常灵活，可以在溢出中断中修改TCNT0的值，也可以在中断中修改OCR0的值，后面的实验中会讲到用定时器1修改OCR1A的方法实现1S精确定时。

师傅领进门，修行靠个人，定时器的基本原理说到这里，要更深入的了解定时器，请看数据手册。


定时公式：Time=PRE*(MAX-TCNT0+1) /F_cpu单位S，其中，PRE为与分频数，本例中为8，MAX即为最大值255，TCNT0为初始化时的值，本例中为0x83（十进制的131），T_cpu，系统时钟频率，本例中为1000000。


本例程序中定时时间为：Time=8*(255-131+1)/1000000=0.001 S，即为1ms，1Khz。可以看出，如果晶振选为8M，则定时时间变为0.000125S，也就是说晶振越大，定时时间越短，预分频越大，定时越长。


在设置时如果你选择1ms，会得到如下结果，和上面的1Khz相同。

**CODE:**



//TIMER0 initialize - prescale:8

// WGM: Normal

// desired value: 1mSec

// actual value: 1.000mSec (0.0%)

void timer0_init(void)

{

TCCR0 = 0x00; //stop

TCNT0 = 0x83; //set count

OCR0 = 0x7D; //set compare

TCCR0 = 0x02; //start timer

}


[[Copy toclipboard]](http://bbs.avrvi.com/htm_data/33/0705/2564.html)


**CODE:**


//ICC-AVR application builder : 2007-6-9 0:33:58

// Target : M16

// Crystal: 1.0000Mhz

// 用途：演示定时器的工作原理

// 作者：古欣

// AVR与虚拟仪器 [url]http://www.avrvi.com[/url]

#include <iom16v.h>

#include <macros.h>


void port_init(void)

{

PORTA = 0x00;

DDRA = 0x03; //PA0 PA1 输出

PORTB = 0x00;

DDRB = 0xFF; //PB 输出

PORTC = 0x00; //m103 output only

DDRC = 0x00;

PORTD = 0x00;

DDRD = 0x00;

}


//TIMER0 initialize - prescale:8

// WGM: Normal

// desired value: 1KHz

// actual value: 1.000KHz (0.0%)

void timer0_init(void)

{

TCCR0 = 0x00; //stop

TCNT0 = 0x83; //set count

OCR0 = 0x7D; //set compare

TCCR0 = 0x02; //start timer

}


//比较匹配中断

#pragma interrupt_handler timer0_comp_isr:20

void timer0_comp_isr(void)

{

//compare occured TCNT0=OCR0

if(OCR0==0x7D) //调整0x7D

{

OCR0=0x7F;

}

else

{

OCR0=0x7D;

}

PORTA ^= 0x01; //PA0取反

}


//溢出中断中断

#pragma interrupt_handler timer0_ovf_isr:10

void timer0_ovf_isr(void)

{

TCNT0 = 0x83; //reload counter value

PORTA ^= 0x01; //PA0取反

}


//call this routine to initialize all peripherals

void init_devices(void)

{

//stop errant interrupts until set up

CLI(); //disable all interrupts

port_init();

timer0_init();


MCUCR = 0x00;

GICR = 0x00;

TIMSK = 0x03; //timer interrupt sources 允许定时器零匹配和溢出中断

SEI(); //re-enable interrupts

//all peripherals are now initialized

}


void main(void)

{

init_devices();

PORTA=0x00;

while(1)

{

PORTB = TCNT0; //任何时候都可以读TCNT0

}

}
            


