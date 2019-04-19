# STM32F030 Nucleo-开发环境的搭建和工程建立 - xqhrs232的专栏 - CSDN博客
2018年10月10日 11:28:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：101
原文地址::[https://blog.csdn.net/farsight2009/article/details/53021547](https://blog.csdn.net/farsight2009/article/details/53021547)
相关文章
1、STM32F0系列ST官方固件库----[https://download.csdn.net/download/xiaolong1126626497/10163036](https://download.csdn.net/download/xiaolong1126626497/10163036)
2、STM32F030官方固件库----[https://download.csdn.net/download/qq_39751570/10612152](https://download.csdn.net/download/qq_39751570/10612152)
作者：华清远见讲师
忍不住，还是在公司的电脑里安装了开发环境，我个人比较喜欢Keil，所以就装个MDK513!环境安装好了!包括如果在电脑中还有其他ARM的开发工具，那么Keil5就需要在环境变量中修改路径。不过这点不需要说了，度娘里面资料非常多!搜搜就好!
环境搭好后，为了验证，我就迫不及待的解压前面下载的标准外设库STM32F0xx_StdPeriph_Lib_V1.5.0，然后再路径：......\STM32F0xx_StdPeriph_Lib_V1.5.0\Projects\STM32F0xx_StdPeriph_Templates\MDK-ARM 中打开标准外设库提供的工程模板，然后就是编译了!
然后，然后，悲催的事情出现了：报错!报错!报了很多的错!!但是基本上都是一个错误如下：
..\..\..\Libraries\CMSIS\Device\ST\STM32F0xx\Include\stm32f0xx.h(479): error: #5: cannot open source input file "core_cm0.h": No such file or directory
#include "core_cm0.h"
..\system_stm32f0xx.c: 0 warnings, 1 error
从错误中来看!很明显：找不到这个core_cm0.h头文件
双击错误然后就可以看到出错的地方：
![](http://top.embedu.org/uploads/allimg/1609/11-1609230U51I23.jpg)
注意一点：#include "core_cm0.h"使用的是””号,而不是<>尖括号。所以在库里面找到添加即可!
添加之后!再编译!又来了!错误如下：
..\Libraries\CMSIS\core_cm0.h(127): error: #5: cannot open source input file "core_cmInstr.h": No such file or directory
#include /* Core Instruction Access */
进去之后如下：
![](http://top.embedu.org/uploads/allimg/1609/11-1609230U501422.jpg)
这个文件是<>尖括号包含的，那么就是说是从Keil的库中包含的，而不是我们自己添加的!(当然，要解决这个问题也可以自己添加头文件，然后将<>改成“”，但是这么多文件都包含了这个头文件，这种傻缺的事咱还是别玩了!所以另想办法!!)。于是乎我进如我的Keil5安装目录，在路径：D:\Program Files\Keil_v5\ARM\Pack\ARM\CMSIS\4.2.0\CMSIS\Include找到了core_cmInstr.h这个头文件(我在D盘安装的keil5)，那么说明在keil路径下是存在这个文件的，但是为毛就是没有被调用呢?
百度了一下，找到了很多相关的东西，但是都是解释这是啥??有的哥们是用了上面的方法编译通过了!反正就是没明说!
然后，我才想起来，这个问题早在好久以前我就在我的笔记本解决过了!那时候没记录下来，所以现在又忘了!但是我想起了，之前经过跟踪代码发现一个问题，在keil工程中，core_cm3.c/core_cm0.c这个文件里面的内嵌汇编代码根本就没有调用，调用的是core_cmInstr.c和core_cmFunc.c这两个文件的内容，是作为内核处理的依据，所以，不用说了，就是和它俩有关系!!而且经过实际验证core_cm3.c/core_cm0.c等这个文件再keil工程中可以干掉。
那么随着又想起了一个问题，keil472以前的keil在安装的时候，在安装目录下X:\Program Files\Keil\ARM\目录下存在CMSIS文件夹，而在keil5安装好之后，是不存在这个文件夹的所以才出现以上的错误!于是乎!我将
D:\Program Files\Keil_v5\ARM\Pack\ARM\CMSIS\4.2.0\目录下的CMSIS文件夹拷贝到了D:\Program Files\Keil_v5\ARM\目录下,这个文件夹整整有239M哇(我磁盘空间不多了,小小蚂蚱也是肉)!果断的将CMSIS\目录下的除了Include文件夹之外的所以文件/文件夹都干掉了!在编译!。。。。。0错误0警告通过了!
迫不及待的就从标准外设库STM32F0xx_StdPeriph_Lib_V1.5.0中将必要文件拷贝出来，自己建一个工程!!!建好之后!同样编译!哇靠!好长的警告!
..\Libraries\FWlib\src\stm32f0xx_wwdg.c(152): warning: #223-D: function "assert_param" declared implicitly
进去一看!就是找不到这个函数!啥情况呢??文件添加不完全??其实不是的，我相信只要是玩过ST的MCU的人都知道，ST的库在编译的时候，可以可控的选择编译一些必要的驱动文件，一些用不到的可以不编译!就是这个stm32f0xx_conf.h头文件了!进入这个头文件就可以看到(1)驱动文件的头文件!
![](http://top.embedu.org/uploads/allimg/1609/11-1609230U429647.jpg)
(2)上面报警告的“函数”，其实它是个宏!先不管他!编译通过再说！
![](http://top.embedu.org/uploads/allimg/1609/11-1609230U4105c.jpg)
那么也就说明stm32f0xx_conf.h这个头文件没有被包含在工程中，但是一查看
![](http://top.embedu.org/uploads/allimg/1609/11-1609230U6014L.jpg)
确实是包含了!而且头文件也在keil的C/C++->Include添加了!
那么啥情况呢??看来还没有找到最根本的问题所在!
同样的我也是凭着经验的记得!这个stm32f0xx_conf.h头文件是在stm32f0xx.h头文件下包含的!于是乎，找到文件就可以查找到了如下图：
![](http://top.embedu.org/uploads/allimg/1609/11-1609230U630459.jpg)
在stm32f0xx.h头文件的最底部找到了这句话：
#ifdef USE_STDPERIPH_DRIVER
#include "stm32f0xx_conf.h"
#endif
如果定义USE_STDPERIPH_DRIVER这个宏，那么stm32f0xx_conf.h头文件将被包含!那么问题就解决了!我们只需要定义这个宏就好!
这里提供两种解决办法!
(1)如下图
![](http://top.embedu.org/uploads/allimg/1609/11-1609230Z032c0.jpg)
直接在上面添加宏定义语句：
#define USE_STDPERIPH_DRIVER
之后编译!0错误0警告!
但是本人不推荐使用这种方法!推荐使用方法(2)
(2)如下图
![](http://top.embedu.org/uploads/allimg/1609/11-1609230Z049149.jpg)
在上图页面Define添加宏：USE_STDPERIPH_DRIVER
确定!再编译!同样的0错误0警告!
那么为什么不建议使用第一种方法呢?
因为对于库来说为了更灵活的使用(有些东西是没必要时时存在的)，当需要某部分的库时，在添加即可，在keil上添加是全局的，而且不改变库的源文件!并且容易修改!如果是第一种方法!可能那天就忘了!有时候想关个东西都得找半天!其实言下之意就是，这样的宏在库中还存在着很多!
OK了!STM32F030牛客板卡的第一个工程就建好了!我是个懒人!将其作为模板!以后直接拷贝它使用就好!
当然!对于不想使用库进行开发的童鞋，可能犹豫!因为STM32F103系列的，有很多大神写好了在直接操作寄存器时候的一些封装，所以可以直接用，但是这里，就不同了!那么咋办呢?
其实只需要添加stm32f0xx.h文件和startup_stm32f030.s启动文件，再包含必要的内核文件/头文件即可!这才是操作寄存器了!哈哈!那么有人可能不理解了，在F103的时候，我们自己写的中断函数还有一个什么函数，并且中断函数随意命名!哈哈!!建议自己去研究M0内核的中断向量表吧!嘿嘿!
文章选自华清远见嵌入式培训
--------------------- 作者：华清远见嵌入式学院 来源：CSDN 原文：https://blog.csdn.net/farsight2009/article/details/53021547?utm_source=copy 版权声明：本文为博主原创文章，转载请附上博文链接！
