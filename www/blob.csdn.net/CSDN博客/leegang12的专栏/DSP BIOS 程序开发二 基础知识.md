# DSP/BIOS 程序开发二 基础知识 - leegang12的专栏 - CSDN博客
2013年12月22日 21:49:44[leegang12](https://me.csdn.net/leegang12)阅读数：1033
【声明：内容源于网络】
1.0 概述
       DSP/BIOS是CCS中集成的一个简易的嵌入式实时操作系统，能够大大方便用户编写多任务应用程序。DSP/BIOS拥有很多实时嵌入式操作系统的功能，如任务的调度，任务间的同步和通信，内存管理，实时时钟管理，中断服务管理等。有了它，用户可以编写复杂的多线程程序，并且会占用更少的CPU和内存资源。
DSP/BIOS是一个可用于实时调度、同步，主机和目标机通信，以及实时分析系统上的一个可裁减实时内核，它提供了抢占式的多任务调度，对硬件的及时反应，实时分析和配置工具等。同时提供标准的API接口，易于使用。它是TI的eXpressDSP实时软件技术的的一个关键部分。
## 1.1 DSP/BIOS的组件构成
1 DSP/BIOS API：编写的程序可以调用API接口函数；
2 DSP/BIOS配置：创建的配置文件定义了程序使用的静态BIOS对象；
3 DSP/BIOS分析工具：集成在CCS上的一些BIOS分析工具可以对运行与目标设备上的程序进行监测，包括CPU负载、时间、日志、线程执行状态等。
DSP/BIOS分为很多模块，提供的所有API函数都按照模块分类，以模块名加下划线开头来命名，如图 40所示为DSP/BIOS的全部模块。
![](http://hi.csdn.net/attachment/201110/27/0_131972270246Mn.gif)
## 1.2 DSP/BIOS的启动顺序
当DSP/BIOS的应用程序启动时，一般遵循下面的步骤：
1） 初始化DSP：DSP/BIOS程序从C/C++环境入口c_int00开始运行。对于C6000平台，在c_int00开始处，系统栈指针（B15）和全局页指针（B14）被分别设置在堆栈断的末尾和.bss断的开始。控制寄存器AMR、IER、CSR等被初始化；
2） 初始化.bss段：当堆栈被设置完成后，初始化任务被调用，利用.cinit的记录对.bss断的变量进行初始化；
3） 调用BIOS_init初始化用到的各个模块：BIOS_init调用MOD_init对配置用到的各个模块进行初始化，包括HWI_init、HST_init、IDL_init等；
4） 处理.pinit表：.pinit表包含一些指向初始化函数的指针，对C++程序，全局对象类的创建也在此时完成；
5） 调用用户程序的main函数：用户main函数可以是C/C++函数或者汇编语言函数，对于汇编函数，使用_main的函数名。由于此时的硬件、软件中断还没有被使能，所以在用户主函数的初始化中需要注意，可以使能单独的中断屏蔽位，但是不能调用类似HWI_enable的接口来使能全局中断；
6） 调用BIOS_start启动DSP/BIOS：BIOS_start在用户main函数退出后被调用，它负责使能使用的各个模块并调用MOD_startup启动每个模块。包括CLK_startup、PIP_startup、SWI_startup、HWI_startup等。当TSK管理模块在配置中被使用时，TSK_startup被执行，并且BIOS_start将不会结束返回；
7） 执行idle循环：有两种方式进入idle循环。当TSK管理模块使能时，任务调度器运行的TSK_idle任务调用IDL_loop在其它任务空闲时进入idle循环；当TSK模块未被使用时，BIOS_start调用将返回，并执行IDL_loop进入永久的idle循环，此时硬件和软件中断可以抢占idle循环得到执行。由于idle循环中管理和主机的通信，因此主机和目标机之间的数据交互可以进行了。
## 1.3 DSP/BIOS的配置
DSP/BIOS的静态配置是利用CCS提供的配置工具完成，包括图形化配置工具和文本配置工具。图形化工具层次清晰，比较直观，而文本工具更加灵活。通常使用图形化的配置方法，下面对主要的模块配置做一些介绍。
### 1.3.1   Global Setting Properties
右键点击“Global Settings”，点击“What’s This”，会弹出帮助窗口，该文件中有Global Setting Properties的各项设置说明。下面介绍的其他如MEM、LOG等配置都可以用同样的方法得到相应的帮助。
选择弹出菜单中的“Properties”，如图 41所示。
![](http://hi.csdn.net/attachment/201110/27/0_1319722758jhGV.gif)
弹出如图 42所示对话框:
![](http://hi.csdn.net/attachment/201110/27/0_1319722807anLR.gif)
一般选择默认设置即可，CLKOUT项需要根据DSP硬件单板提供的工作时钟设置，Himalaya通常运行在1GHz频率。
DSP Endian Mode 项根据实际情况设置。
### 1.3.2 MEM设置
MEM模块设置中可以根据具体情况设置不同的内存段，其中存在一个默认的IRAM片内内存段。需要注意的是，首先必须在IRAM段上设置一个heap段落，用于BIOS的内部使用。设置方法是在IRAM段上右键选择“Properties”，弹出如图 43对话框，必须设置红线框中的选项，heap size可根据情况具体设置。
![](http://hi.csdn.net/attachment/201110/27/0_13197228625CwL.gif)
接下来配置MEM全局属性，右键点击配置窗口中的“MEM…”，如图 44所示：
![](http://hi.csdn.net/attachment/201110/27/0_1319722919Hcm2.gif)
点击“Properties”，弹出如图 45所示窗口：
![](http://hi.csdn.net/attachment/201110/27/0_1319723017Uz7k.gif)
如图红线框中，两个heap段都要选择为IRAM，“Stack Size”项需要根据实际情况设置最大的栈大小，其他使用缺省设置即可。
在图 44的菜单中，点击“Insert MEM”可创建新的内存段。在新的内存段名上右键选择“Properties”弹出如图 43窗口，根据情况设置base（基地址）、len（段长度）、heap size（用于MEM动态内存分配的堆大小）。
### 1.3.3 LOG设置
LOG用于输出和记录一些打印信息，默认存在一个LOG_system对象，是系统内部用来处理打印信息的，不需要去设置。可以增加新的LOG对象，用来在应用程序中输出打印信息。如图 46，在LOG标签上右键选择“Insert LOG”，填写对象名LOG_test即建立了一个新的LOG对象。
![](http://hi.csdn.net/attachment/201110/27/0_1319723065d7a8.gif)
在LOG_test标签上右键选择“Properties”，弹出如图 47窗口，可对此LOG对象属性作一些设置。具体设置含义可通过Help查看。
![](http://hi.csdn.net/attachment/201110/27/0_13197231104LJf.gif)
### 1.3.4 CLK设置
在CLK属性中通常选择定时器Timer0作为DSP/BIOS的基准时钟，计时分辩率设置为每秒1000次中断，在1GHz系统时钟下，近似为每次定时中断间隔999.996微秒。Timer Mode选择为32bit unchained模式，即使用TCI6482的TMR0的TIMLO作为Timer0，而TIMHI还可以作其它用途使用。CLK属性配置界面如图 48所示：
![](http://hi.csdn.net/attachment/201110/27/0_13197231596Z3I.gif)
### 1.3.5 HWI设置
HWI（硬中断）中包含HWI_INT4到HWI_INT15可用来定义用户自己的硬件中断，HWI_RESET、HWI_NMI和HWI_RESERVED不要去改动。如图 49，每个硬HWI的优先级从上到下逐渐降低。
![](http://hi.csdn.net/attachment/201110/27/0_1319723212yuy2.gif)
选择HWI_INT10为例，来设置EMAC/MDIO的中断，需要填写中断事件号17，并且填写中断服务程序名（C函数前面需要加下划线）如图 50所示：
![](http://hi.csdn.net/attachment/201110/27/0_1319723248H82h.gif)
在Dispatcher页面，选择“Use Dispatcher”由BIOS代理控制中断的确认和清除，不需要用户中断服务程序干预，比较简便。如图 51：
![](http://hi.csdn.net/attachment/201110/27/0_1319723292NMRk.gif)
### 1.3.6 SWI设置
SWI（软中断）的优先级在HWI之后，但是比TSK高，可以通过右键菜单中的“Insert SWI”创建一个SWI对象，可以指定SWI内部优先级，从0(最低)到14(最高)。如图 52：
![](http://hi.csdn.net/attachment/201110/27/0_1319723332D1O2.gif)
### 1.3.7 TSK设置
在TSK Manager（任务管理器）中可以根据需要创建各种任务，任务间是根据优先级抢占策略来进行调度的，TSK提供有多种优先级别，包括-1(Suspend)、0(Idle)、1(最低)到15(最高)。如图 53：
![](http://hi.csdn.net/attachment/201110/27/0_1319723382FCZq.gif)
在TSK Manager上右键选择“Insert TSK”并填写任务名称后就可以创建一个任务，在相应任务上右键选择“Properties”可对任务属性进行设置，如图 54：
![](http://hi.csdn.net/attachment/201110/27/0_1319723431X3ae.gif)
Stack Size（最大堆栈大小）和Priority（优先级）需要根据任务的具体情况进行设置。在页面，填写任务实体函数名（C函数前面加一个下划线），如图 55：
![](http://hi.csdn.net/attachment/201110/27/0_13197234706cC5.gif)
### 1.3.8 Synchronization设置
DSP/BIOS中任务间的通信和同步可由SEM、MBX、QUE、LCK四个模块完成：
? SEM（信号量）：用于任务同步和互斥，有计数功能，根据需要使用；
? MBX（邮箱）：也用于任务同步，可以传递少量数据，根据需要使用；
? QUE（队列）：可实现任务同步和资源的共享，根据需要使用；
? LCK（资源锁）：实现对共享资源的互斥，根据需要使用。
四种同步模块对象都可以通过各自的右键菜单中“Insert …”来创建，并可对其属性作相应的设置。如图 56所示
![](http://hi.csdn.net/attachment/201110/27/0_1319723517GSaL.gif)
### 1.3.9 Input/Output设置
这里可以设置一些输入/输出相关的高级设置，具体可通过CCS的帮助项来了解。一般不需要进行设置。只有RTDX（实时数据交换），需要根据目标环境的情况对数据交换模式进行一下选择，用来在调试中主机和目标机进行数据交换。可以是仿真器环境的JTAG模式，或者是模拟器环境的Simulator模式。如图 57所示：
![](http://hi.csdn.net/attachment/201110/27/0_1319723560I8ku.gif)
