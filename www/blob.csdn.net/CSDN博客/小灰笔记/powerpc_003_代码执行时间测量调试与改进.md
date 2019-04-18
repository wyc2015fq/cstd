# powerpc_003_代码执行时间测量调试与改进 - 小灰笔记 - CSDN博客





2018年08月29日 22:16:42[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：74








            前面两次小结都是理论性的，这一次做一个实践性的小结。使用S32DS进行软件调试，同时进一步优化之前关于软件执行时间的测试。

            调试，最简单方式就是查看我们关注的信息，比如变量和寄存器，看看他们的值有没有按照我们预期的变化。不过，这套工具似乎不支持相应信息的自动刷新，如果要查看的话需要让程序停住才能够查看。

            之前我们写了一个简单的时间测试函数，使用的汇编代码。查看的方式可以查看通用寄存器5，因为我们的计数器变化存储在这个寄存器中。在函数执行的入口处打断点，查看到的计数器变化：

![](https://img-blog.csdn.net/20180829221622670?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            为什么差值不是5，查看了一下说明是因为流水线因素导致。几次测试，感觉这个结果似乎不是我预期的。为了验证这个功能的可用性，打算重新设计软件。

            这次打算借用实现了C语言封装的几个汇编操作：

![](https://img-blog.csdn.net/20180829221622736?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            上面是对特殊寄存器的操作函数。再查看一下所需要的特殊寄存器的编号：

![](https://img-blog.csdn.net/20180829221622763?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            程序修改如下：

/*

 * main implementation: use this 'C' sample to create your own application

 *

 */



#include"derivative.h"/* include peripheral declarations */

#include"compiler_api.h"



#defineTBL_SPR_RD268

#defineTBL_SPR_WT284

#defineTBU_SPR_RD269

#defineTBU_SPR_WT285

#defineHID0_SPR1008



uint32_t time_counter1;

uint32_t time_counter2;



externvoidxcptn_xmpl(void);

voidTimeBaseMeasurement_Init(void);



voidTimeBaseMeasurement_Init(void)

{

uint32_t val_32bit_temp = 0U;



/* init TBU and TBL */

MTSPR(TBL_SPR_WT,0);

MTSPR(TBU_SPR_WT,0);

/* enable time base function */

    val_32bit_temp = MFSPR(HID0_SPR);

    val_32bit_temp |= 0x4000;

MTSPR(HID0_SPR,val_32bit_temp);

}



voidTest(void)

{

    ;

    ;

    ;

    ;

    ;

}



intmain(void)

{

xcptn_xmpl ();              /* Configure and Enable Interrupts */



TimeBaseMeasurement_Init();



/* Loop forever */

for(;;)

    {

        time_counter1 = MFSPR(TBL_SPR_RD);

Test();

        time_counter2 = MFSPR(TBL_SPR_RD);

    }

}



            经过上面的修改，我依然没有测试成功。但是，我似乎发现了一个规律，那就是两个数字其实是在有规律的增加。观察之后，我简单得出一个猜测：其实程序是OK的，这两个时间测试寄存器的特殊之处在于程序停在断点之后，计数器依然在变化。

        于是，程序设计为两个计数器求差。而监控方式上，从原来的断点查看改成实时查看。这样，主要的功能代码修改如下：



            测试查看结果：

![](https://img-blog.csdn.net/20180829221622715?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            通过上面的结果看得出，纯粹的这个读取中间其实就有18个时钟周期的时间消耗，如果力求准确，在最终的时间中应该把这个数值删除。现在只是一个粗略的测试，后面的测试时间包括这一部分执行时间，接下来测试Test函数的执行时间：

![](https://img-blog.csdn.net/20180829221622718?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180829221622955?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            数值十分稳定，就是这么长时间。进一步修改测试，执行两次Test函数。

![](https://img-blog.csdn.net/20180829221622952?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            从上面的结果看，增加的数值跟上次差不多。测试软件应该来说是准确的。



