# （多核DSP快速入门）6.IPC的使用+实例分析 - tostq的专栏 - CSDN博客





2016年04月27日 23:18:25[tostq](https://me.csdn.net/tostq)阅读数：13125
所属专栏：[多核DSP快速入门](https://blog.csdn.net/column/details/easymulticoredsp.html)









       IPC是SYS/BIOS处理核间通信的组件

       IPC的几种应用方式（下面中文名字是自行翻译，旁边有英文=_=||，另外下面的配图中的蓝色表示需要调用模块的APIs，而红色模块表示仅仅需要配置（如在.cfg中配置），而灰色模块表示是非必须的）：

（1）最小使用（Minimal use）：这种情况是通过核间的通知机制（notification）来实施的，而一个通知所携带的信息是非常小的（一般是32bits），所以称为最小使用。这种方式一般是用于处理核间的简单同步，却无法处理复杂的消息传递。

![](https://img-blog.csdn.net/20160427225537675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




这种情况下，需要利用到Notify模块的APIs函数，比如通过Notify_sendEvent()函数给某个特定核传递一个事件，我们可以给特定事件动态注册反馈函数。由于一个通知（notification）所携带的信息是极少的，所以只能给处理器发送一个事件号，而这个事件号所代表的反馈函数决定之后的动作。另外一些数据以函数参数方式，也可以被送出。

（2）增加数据通路（Add data passing）：这种情况是在前面的最小使用机制下，在核间增加了一个传递链表元素的数据通路，这个链表的实施一般是使用共享内容并通过门（gates）来管理同步。

![](https://img-blog.csdn.net/20160427225559128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这种情况是在最小使用上，增加了一个ListMP模块用于共享链表元素。

ListMP模块是一个双向链表，另外ListMP需要用到共离内存，所以SharedRegion模块也需要被使用，另外ListMP通过NameServer模块中来管理名称/值，同时使用GateMP模块来防止链表元素被多个处理器同时读取。

（3）增加动态分配（Add dynamic allocation）：这种情况下，增加了从堆中动态分配链表元素的功能。

![](https://img-blog.csdn.net/20160427225630523?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这种情况在上种情况下，增加了一个Heap*MP模块，这个模块主要用于从堆中给链表动态分配内存

（4）强大但易用的消息机制（Powerful but easy-to-use messaging）：这种情况下利用MessageQ模块来传递消息。

![](https://img-blog.csdn.net/20160427225702941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


除了Notify通知机制，还可以利用MessageQ来实现更为复杂的核间通信，在这种情况下，只需要配置MultiProc和SharedRegion模块就可以了，而Ipc_start()函数将自动为我们实现上面灰色模块的配置。

本节，我们暂时只以一个简单的最小使用（Minimal use）情况为例，分析CCS自带一个多核通信例子，这个例子在八核之间相互传递信息，之后我们总结核间传递方法实现步骤，实现一个主从核之间传递信息的功能。




**一、打开CCS自带例程**

打开CCS自带例程的方法和新建CCS项目是一致，只是需要在Project templates and examples中选择我们需要的例子，这里的例子是在IPC and I/O Examples中选择C6678
 Examples中的Notify，然后输入项目名称后，CCS会自行将例子复制到项目下。

![](https://img-blog.csdn.net/20160427225915161?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**二、生成项目后，编译调试查看运行结果**

（1）点击编译后，查看有无出错

（2）导入目标配置文件.ccxml，这里选择的仍然是C6678 Device Functional Simulator, Little Endian.

（3）点击调试，选择所有的核

（4）全选所有的核（通过Shift），然后Groups

![](https://img-blog.csdn.net/20160427230051200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（5）选中Group，点击运行

**三、分析运行结果**

这个例程除了调用Notify模块，在核间传递消息，通过反馈函数触发核的动作，同时通过Semaphore模块来保证核间依次执行，防止抢占，这里每个核都有一个信号量，来表明其当前是否执行或等待其它核执行完成。

（1）各核打印

![](https://img-blog.csdn.net/20160427230117044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这段是在main()中出现的结果，每个核都会执行各自的main()。

![](https://img-blog.csdn.net/20160427230139146?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


各核注册事件，并表明其反馈函数

![](https://img-blog.csdn.net/20160427230155396?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）核0执行同时释放信号量，在核0释放信号量semHandle之前，其他核都处理等待信号量释放中

![](https://img-blog.csdn.net/20160427230225046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


核0通过给核1发送事件，触发反馈函数，在反馈函数中semHandle归一，注意这个激活的信号量是在核1中的

![](https://img-blog.csdn.net/20160427230405616?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


激活核1的信号量后，核0打印结果，并等待其信号量的结果，所有核的信号量都初始为0

![](https://img-blog.csdn.net/20160427230431397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）以下是总共八个核，分别执行了NUMLOOPS次（这里设置的是10次）

![](https://img-blog.csdn.net/20160427230453647?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


下一个核信号被激活，开始执行

![](https://img-blog.csdn.net/20160427230529804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


同时通过反馈函数将当前核的下一个核激活

![](https://img-blog.csdn.net/20160427230546257?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


完成发送事件

![](https://img-blog.csdn.net/20160427230634302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（4）退出任务循环，同时退出当前核的BIOS

![](https://img-blog.csdn.net/20160427230703006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**四、多核IPC的配置**

（1）IPC的启动非常简单，在导入Ipc头文件后，在main()函数中调用Ipc_start()就能根据.cfg文件中配置启动IPC所需要的模块，比如默认情况下Ipc_start()会调用Notify_start()之类，不过要启动这些模块，需要保证提前在.cfg中添加了这些模块（如右击模块，选择Use）

（2）IPC的配置是在.cfg中完成的，配置IPC首先需要申明，当前Notify等相关模块也需要提前申明，这里如果不清楚IPC所需要的相关模块，最好使用自带IPC例程作为模块。


var Ipc = xdc.useModule('ti.sdo.ipc.Ipc');


（3）设置同步的核数


Ipc.procSync = Ipc.ProcSync_ALL;


这里Ipc.ProcSync_ALL 表示Ipc_start会自动将所有的核都启动了


Ipc.ProcSync_PAIR 表示只启动部分核，需要启动的核要通过Ipc_attach()来启动，这个默认选项


Ipc.ProcSync_NONE 表示Ipc_start()不会同步任何核

（4）核间的连接方法Ipc_attach()及Ipc_detach()

这两个函数的使用，需要.cfg文件中配置了Ipc.ProcSync_PAIR

Ipc_attach的使用方法非常简单，在Ipc_start()之后直接输入：

Ipc_attach(#coreID)，#coreID表示需要连接的核ID号，如Ipc_attach(0)表示连接核0。

**不过需要注意的是：**

a) 核的连接一定要按照ID号从小到大的顺序进行，比如当前核必须先连接了核0，才能连接核1，之后才能连接核

b) 另外两核之间的相互连接必须先满足ID号小的先连接ID号大的，比如只有当核0连接核1后，核1才能连接到核0

c) 由于核的连接并不是一次就能成功的，所以一般需要加一个循环等待的过程，一般使用方法如下：


    while(Ipc_attach(#coreID)<0){

Task_sleep(1);

}


Ipc_detach()的使用方法同Ipc_attach()是类似的，不过它的功能是解除连接。




**五、主从核之间的通信**

前面介绍IPC核间通信例子，是每个核同所有核之间都有连接，而各核之间连接都是相同且双向，而在很多情况下，我们并不需要如此多的核，或者许多核间连接也是不必要的，这些情况下使用Ipc.ProcSync_ALL未免太不高效。

下面我们介绍的例子是核间的主从通信，选择三个核，选择一个主核，另外两个是副核，主核core0同副核之间有相互连接，而副核core1与副核core2之间没有连接，这个主从通信主要完成以下事件

a) 主核向两从核发送事件，激活从核，使其执行任务

b) 两核完成任务后，向主核发送事件，主核继续执行其任务。

这个例程主要有如下内容：

（1）在.cfg文件中设置procSync

改为Ipc.procSync = Ipc.ProcSync_PAIR;


（2）定义主从核ID

![](https://img-blog.csdn.net/20160427231004259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）在main()函数Ipc_start()后增加如下代码：

![](https://img-blog.csdn.net/20160427231322544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


a) 这里主要根据主核和从核的角色分别添加连接任务：主核同两个从核都有连接，而从核只与主核有链接

b) 在添加核间连接后，分别给核间连接注册事件

（3）在任务函数tsk0_func内分别根据主核及从核的角色添加发送与接收函数，下面是以主核为例

![](https://img-blog.csdn.net/20160427231605797?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


主核在给从核分别发送事件后，通过Semaphore_pend等从核将主核的信号量激活，之后继续主核任务。

同时只有当主核通过给从核发送事件，从核的注册函数激活信号量，从核才能开始任务。

（4）仿真调试的结果

![](https://img-blog.csdn.net/20160427231644001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


从结果上看，当从核分别收到了来自主核的事件时，同时开始任务，当从核任务全部完成后，主核才开始其任务




**Tips: **

a) 这里需要注意的是在注册函数内不要加System_printf()函数，这个函数会导致如下错误产生：


ti.sysbios.gates.GateMutex: line 97: assertion failure: A_badContext: bad calling context. See GateMutex API doc for details.



xdc.runtime.Error.raise: terminating execution


b) 上面的代码并不是全部

例程代码下载地址：[https://github.com/tostq/EasyMulticoreDSP/tree/master/6.IPC_notify](https://github.com/tostq/EasyMulticoreDSP/tree/master/6.IPC_notify)



