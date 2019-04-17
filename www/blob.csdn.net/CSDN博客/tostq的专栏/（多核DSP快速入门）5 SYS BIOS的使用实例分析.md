# （多核DSP快速入门）5.SYS/BIOS的使用实例分析 - tostq的专栏 - CSDN博客





2016年04月27日 22:44:43[tostq](https://me.csdn.net/tostq)阅读数：15367标签：[CCS																[DSP																[多核																[BIOS																[SYSBIOS](https://so.csdn.net/so/search/s.do?q=SYSBIOS&t=blog)
个人分类：[多核DSP](https://blog.csdn.net/tostq/article/category/6201995)

所属专栏：[多核DSP快速入门](https://blog.csdn.net/column/details/easymulticoredsp.html)](https://so.csdn.net/so/search/s.do?q=BIOS&t=blog)





原创文章

转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)


系列教程目录：[http://blog.csdn.net/tostq/article/details/51245979](http://blog.csdn.net/tostq/article/details/51245979)


       SYS/BIOS是一个可扩展的实时内核（或者说是操作系统），其提供了许多模块化的APIs（应用程序接口），支持抢占式多线程，硬件抽象，实时分析和配置工具，其设计目的是为了最大限度地减少对内存和CPU的要求。其拥有很多实时嵌入式操作系统的功能，如任务的调度，任务间的同步和通信，内存管理，实时时钟管理，中断服务管理等。有了它，用户可以编写复杂的多线程程序，并且会占用更少的CPU和内存资源。

SYS/BIOS的早期版本是DSP/BIOS，更名的原因，是因为SYS/BIOS不仅可以用于DSP，而且也可以嵌入到ARM等其他SoC中去。SYS/BIOS是一个可用于实时调度、同步，主机和目标机通信，以及实时分析系统上的一个可裁减实时内核，它提供了抢占式的多任务调度，对硬件的及时反应，实时分析和配置工具等。同时也提供标准的API接口，易于使用。它是TI的eXpressDSP实时软件技术的的一个关键部分。

CCS中集成安装了SYS/BIOS，能够大大方便用户编写多任务应用程序。另一方面，SYS/BIOS可以在XDCtools中使用配置技术，极大地方便了SYS/BIOS的开发流程。

本节主要是通过一些简单的SYS/BIOS的例子来介绍相关APIs组件的作用，这里主要包括了时钟Clock模块、任务Tsk模块、软件中断Swi模块、信号量Sem模块及日志Log模块五类简单模块的使用。




**一、新建SYS/BIOS项目**

（1）在项目模板中选择SYS/BIOS项目中的Hello Example模板，点击Next

![](https://img-blog.csdn.net/20160427222217393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）在RTSC（XDCtools的别称）配置页中选中需要的SYS/BIOS，XDCtools及其他组件的版本，Target保持默认，不需修改，如果Platform没有自动填充，选择与设备适用的平台。Build-profile决定程序链接的库，推荐使用release，即使仍然处于创建和调试阶段，点击完成创建项目。

![](https://img-blog.csdn.net/20160427222240792?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**Tips：**当我们开始使用SYS/BIOS时，是不能指定自己添加的链接命令文件.cmd。因为.cmd会在构建工程的时候由SYS/BIOS自动创建并使用。

（3）点击编译

（4）导入target文件后，点击调试，运行得到结果

![](https://img-blog.csdn.net/20160427222357435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**二、SYS/BIOS的模块与配置**

（1）SYS/BIOS可以用文本编辑器或者是图像配置编辑器XGCONF来编辑，双击打开.cfg文件如下

![](https://img-blog.csdn.net/20160427222433503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


单击System Overview，可以显示程序当前使用的主模块（带绿色小箭头的）

（2）各种APIs模块的添加这里有两种方法，一种是直接双击主模板进入，然后勾选Add

![](https://img-blog.csdn.net/20160427222501894?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


另一种方法是在Available Products中右击选中的模块，选择Use

![](https://img-blog.csdn.net/20160427222536160?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）各个API模块的作用

CLK：片内定时器模块，主要控制片内定时器并提供高精度的32位实时逻辑时钟，它能控制中断的速度，使之最快达到单指令周期时间。

HST：主机输入/输出模块，管理主机通过对象，它允许应用程序在目标系统和主机之间交流数据，主机通道通过静态配置为输入或输出。

HWI：硬件中断模块，提供对硬件中断服务例程的支持，可在配置文件中指定当硬件中断发生时需要启动的函数

IDL：休眠功能模块，管理休眠函数在目标系统程序没有更高优先权的函数运行时启动

LOG：日志模块，管理LOG对象，LOG对象在目标系统程序执行时实时捕捉事件，开发者可以使用系统日志或定义自己的日志，并在CCS中利用它实时浏览信息。

MEM：存储器模块允许指定存放目标程序的代码和数据所需的存储器段

PIP：数据通道模块管理数据通道，它用来缓存输入和输出数据流，这此数据通道提供一致性的软件数据结构，可以使用它们驱动DSP和其他实时外围设备之间的I/O通道

PRD：周期函数模块，管理周期对象，它触发应用程序的周期性执行。

RTDX：实时数据交换允许数据在主机和目标系统之间实时交换，在主机上使用自动OLE的客户都可对数据进行实时显示和分析。

STS：统计模块，管理统计累积器，在程序运行时，它存储关键统计数据并能通过CCS浏览这此统计数据

SWI：软件中断模块管理软件中断。

TRC：跟踪模块，管理一套跟踪控制比特，它们通过事件日志和统计累积器控制程序信息的实时捕捉。




**三、在项目中导入LOG模块**

LOG模块实际上是一个实现打印信息的API

（1）添加LOG模块

![](https://img-blog.csdn.net/20160427222815612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）LOG模块的使用

LOG模块定义了许多比如Log_error、Log_info、Log_warning、Log_print等之类函数，这些函数的用法同printf函数的用法很相似，这些函数都可以在<xdc/runtime/Log.h>找到，其实际上是将printf的有用法分成许多不同的类（如错误信息、提示信息、警告信息等），LOG模块打印的内容可以在Tools/RTA/PrintfLogs中看到。

**Tips：**LOG中定义了许多如下的函数，i比如Log_info1函数后面的数字代表函数接的变量数目

如Log_info1("%d",s1）、Log_info2（"%d,
 %d", s1, s2）

![](https://img-blog.csdn.net/20160427222921239?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







**四、在项目中导入TSK任务模块**

TSK任务模块是操作系统中最基本的模块，其实际上反映了多线程抢占，每个任务单独是一个线程，各个线程（任务）具有各自的优先级

（1）新建任务，首先选择Use TSK，确定TSK模拟是否导入

![](https://img-blog.csdn.net/20160427222956037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


然后创建新任务New Task

![](https://img-blog.csdn.net/20160427223019834?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里我们创建两个任务task0、task1，分别对应其函数func_tsk0、func_tsk1。其优先级都为1

![](https://img-blog.csdn.net/20160427223042443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）编写任务函数

![](https://img-blog.csdn.net/20160427223105945?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


       a). 这里我们在主函数中BIOS_start()函数，说明任务开始执行了

b). 任务执行完后调用BIOS_exit(0)退出

c). Task_yield()是个优先级调度函数，其作用就是如果有相同优先级的任务，则调度到同优先级的其它任务执行！

d). Log_info1()的作用是打印日志信息

e). while和count循环的目标是让任务执行较长的时间，而不是只执行一次就退出了。

f). 最后别忘了在增加相关头文件和函数的声明！

![](https://img-blog.csdn.net/20160427223302570?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）编译调试，运行查看结果（这里我们只需要选择单核运行就可以了）

从下面的分析，我们可以看到两个任务是相互依次运行的，每个任务运行一次后，其优先级就会降低，此时就切换到下一个任务

![](https://img-blog.csdn.net/20160427223343445?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**五、在项目中导入Swi软件中断模块**

前面我们已经知道了不同任务有不同优先级，而软件中断具有比任何任务都高的优先级，而其中硬件中断（HWI）又比软件中断（SWI）优先级更高。

（1）添加软件中断Swi，这里方法同前面添加TSK的方法不一样，似乎不能用图形界面方式去添加（我没有成功=_=||）

这里我选择通过直接在源代码中添加相关代码：

a). 首先需要添加一个全局的Swi句柄：Swi_Handle swi0;

b). 初始化Swi参数：


Swi_Params swiParams;        

Swi_Params_init(&swiParams);

swiParams.priority = 2;        // 软件中断的优先级

swiParams.trigger = 2;         // 软件中断的计数器


c). 创建软件中断：


swi0 = Swi_create(func_swi0, &swiParams, NULL);  // swi0是中断名，func_swi0是中断函数


d). 编写中断函数：


void func_swi0(void)

{

 static Int count = 0;

Log_info1("Swi0 is doing %d\n",count);

 count++;

}


e). 更改软件中断计数器trigger，要触发软件中断，首先需要让trigger的计数为0，这里我们可以在任务函数内增加一个trigger自减的函数，任务函数执行两次后，将会触发软件中断。


void func_tsk0(void)

{

 Int count = 0;

while(count<10){

 Log_info1("Task0 is doing %d\n",count);

Swi_dec(swi0);

Task_yield();

 count++;

}

 BIOS_exit(0);

}


（2）一些Swi APIs 函数


Swi_inc Swi.trigger自增函数，每次增加1


Swi_dec Swi.trigger自减函数，每次减少1



Swi_or 将trigger的值与模板求或操作


Swi_and 将trigger的值与模板求和操作

（3）编译调试，运行查看结果（这里我们只需要选择单核运行就可以了）

从下面的分析，我们可以看到两个任务的每次都会使得软件中断计数trigger减1（通过Swi_dec函数），直到trigger的值减少到0时，执行软件中断，中断后，trigger恢复到原来的值，这里的trigger初始值为2，所以执行两次任务后就会触发一次软件中断。

![](https://img-blog.csdn.net/20160427223802901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**六、在项目中导入信号量Semaphore模块**

信号量是在多线程环境下使用的一种设施，是可以用来保证两个或多个关键代码段不被并发调用，对于多个任务来说，使用信号量可以防止多个任务同时执行。

信号量可分互斥信号量和计数信号量，互斥信号量只有两种状态：1和0，为1时说明可用，否则不可用。而计数信号量通过设置一个计数值，如果计数值大于0，则任务请求该信号量时是可用。

这里例子是我们给task0任务中增加一个互斥信号量，当信号量状态为1时，task0才能继续执行。通过软件中断来达到信号量归1。

（1）添加信号量

a). 首先需要添加一个全局的信号量句柄：Semaphore_Handle sem0;

b). 创建信号量：sem0 = Semaphore_create(0, NULL, NULL);

c). 在增加互斥信号量的任务函数中增加一个等待信号量为1的函数


Semaphore_pend(sem0, BIOS_WAIT_FOREVER);  // BIOS_WAIT_FOREVER表示一直等待，直到信号量为1


d).
 在软件中断函数中增加一个解锁信号量的函数


Semaphore_post(sem0);


e).
 最后别忘了添加Semaphore模块的头文件

#include <ti/sysbios/knl/Semaphore.h>

f).
 如果.cfg文件中没有添加信号量模块，记得一定要添加，否则虽然不会报错，但信号量不会工作

（2）编译调试，运行查看结果（这里我们只需要选择单核运行就可以了）

从下面的分析，我们可以看到只有当软件中断执行后，此时信号量才解锁，task0才能执行，而任务执行两次，才能触发一次软件中断。

![](https://img-blog.csdn.net/20160427223933917?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**七、在项目中导入时钟Clock模块**

Clocks模块主要提供周期性执行函数，我们这里新建一个周期性执行函数，其每四个周期执行一次。


（1）添加信号量

a). 首先初始化时钟参数：


 Clock_Params clkParams;

Clock_Params_init(&clkParams);

clkParams.period = 5;
 // 函数执行周期


clkParams.startFlag = TRUE; // True说明时钟立即开始计时



b). 创建时钟：Clock_create(func_clk, 5, &clkParams, NULL);
 //创建时钟，func_clk是周期执行的函数，这里5是开始执行的延时。

c).  编写周期执行的时钟函数



void func_clk(UArg arg0)

{

    UInt32 time;

    time = Clock_getTicks();
 // 这里是定时器的节拍器

    System_printf("System time in clk0Fxn = %lu\n", (ULong)time);

    if(time>20)

       BIOS_exit(0);

}

d). 因为任务的执行时间非常快，所以需要先把任务内的退出BIOS命令先删除下，否则当任务完成后，时钟函数还没执行


void func_tsk1(void)

{

    Int count = 0;

    while(1){

   //while(count<10){

       Log_info1("Task1 is doing %d\n",count);

      Swi_dec(swi0);

       Task_yield();

        count++;

  }

  //BIOS_exit(0);

}




e). 最后别忘了添加时钟模块的头文件


#include <ti/sysbios/knl/Clock.h>


f). 如果.cfg文件中没有添加时钟Clock模块，记得一定要添加，否则虽然不会报错，但时钟模块不会工作

（2）编译调试，运行查看结果（这里我们只需要选择单核运行就可以了）
从下面的分析，我们可以看到只有当周期函数func_clk每隔5个周期开始执行，开始执行时间为5。


![](https://img-blog.csdn.net/20160427224229371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**项目代码下载：**

[https://github.com/tostq/EasyMulticoreDSP/tree/master/5.helloworldBios](https://github.com/tostq/EasyMulticoreDSP/tree/master/5.helloworldBios)






**附录：关于SYS/BIOS的官方视频学习资料**

一、概览 [http://www.eeworld.com.cn/training/TI_study/2013/0503/257.html](http://www.eeworld.com.cn/training/TI_study/2013/0503/257.html)

二、定时器和时钟模块 [http://www.eeworld.com.cn/training/TI_study/2013/0503/258.html](http://www.eeworld.com.cn/training/TI_study/2013/0503/258.html)

三、硬件中断和空闲线程_1 [http://www.eeworld.com.cn/training/TI_study/2013/0503/259.html](http://www.eeworld.com.cn/training/TI_study/2013/0503/259.html)

四、硬件中断和空闲线程_2 [http://www.eeworld.com.cn/training/TI_study/2013/0503/266.html](http://www.eeworld.com.cn/training/TI_study/2013/0503/266.html)

五、软件中断_1 [http://www.eeworld.com.cn/training/TI_study/2013/0503/260.html](http://www.eeworld.com.cn/training/TI_study/2013/0503/260.html)

六、软件中断_2 [http://www.eeworld.com.cn/training/TI_study/2013/0503/261.html](http://www.eeworld.com.cn/training/TI_study/2013/0503/261.html)

七、任务 [http://www.eeworld.com.cn/training/TI_study/2013/0503/262.html](http://www.eeworld.com.cn/training/TI_study/2013/0503/262.html)

八、信号量 [http://www.eeworld.com.cn/training/TI_study/2013/0503/263.html](http://www.eeworld.com.cn/training/TI_study/2013/0503/263.html)

九、对MSP430的支持_1 [http://www.eeworld.com.cn/training/TI_study/2013/0503/264.html](http://www.eeworld.com.cn/training/TI_study/2013/0503/264.html)

十、对MSP430的支持_2 [http://www.eeworld.com.cn/training/TI_study/2013/0503/265.html](http://www.eeworld.com.cn/training/TI_study/2013/0503/265.html)](https://so.csdn.net/so/search/s.do?q=多核&t=blog)](https://so.csdn.net/so/search/s.do?q=DSP&t=blog)](https://so.csdn.net/so/search/s.do?q=CCS&t=blog)




