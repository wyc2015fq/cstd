# powerpc_002_基于时基测试一段代码的执行时间 - 小灰笔记 - CSDN博客





2018年08月29日 00:43:24[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：75








            整体的软件来源于上次的软件，我修改了文件名称，同时增加了汇编代码。而这次的汇编代码功能是基于时基进行软件执行时间的测试。

            这是PowerPC内置的一个内核级别的功能，功能依赖于几个寄存器。

            TBU à 时间计数器高位

            TBL à  时间计数器低位

            HID0 à 使能时基计数功能。

            如果相应的功能被使能，两个时间计数器寄存器会级联成一个64bit的“寄存器”进行计数。计数的单位是按照系统时钟来增加。以下是关于时基寄存器的拓扑介绍：

![](https://img-blog.csdn.net/20180829004252714?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            以下是功能使能条件：

![](https://img-blog.csdn.net/20180829004252658?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            接下来看看HID0寄存器：

![](https://img-blog.csdn.net/20180829004252742?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            之前接触MMU的时候我就曾经对上面的描述有点疑惑，其实，这个寄存器是一个32bit的寄存器。只不过在描述的时候使用的是32-63而不是0-31。为什么这么描述，我还一时间没有弄明白，但是大致的寄存器操作应该还是好理解了。

            这样，一个简单的测试代码就可以如下实现：

        .text



.global TimeBaseMeasurement



TimeBaseMeasurement:

    # set time base register to 0

    e_lis r4,0

    mttbu r4

    mttbl r4



    # enable time base function by setting according bit in HID0

    mfhid0 r5

    e_lis r4, 0x4000

or r5, r4,r5

    mthid0 r5



    # some delay

nop

nop

nop

nop

nop



    # read back value of TBL and store it into r5

    mftbl r5

    se_blrl





            软件设计比较简单，之所以只读取了低位主要还是考虑到软件执行时间短。如果超过一定时间，足够让低位寄存器溢出（达到最大）的话，应该读取两个寄存器的数值。





