# （多核DSP快速入门）4.编译测试多核DSP实例Vlfft - tostq的专栏 - CSDN博客





2016年04月27日 22:09:24[tostq](https://me.csdn.net/tostq)阅读数：7203
所属专栏：[多核DSP快速入门](https://blog.csdn.net/column/details/easymulticoredsp.html)








原创文章
转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)

系列教程目录：[http://blog.csdn.net/tostq/article/details/51245979](http://blog.csdn.net/tostq/article/details/51245979)



      Vlfft是由TI官网提供的一个大尺寸快速傅里叶变换的多核DSP实例，其下载地址如下：


[http://www.deyisupport.com/question_answer/dsp_arm/c6000_multicore/f/53/p/12423/67467.aspx](http://www.deyisupport.com/question_answer/dsp_arm/c6000_multicore/f/53/p/12423/67467.aspx)


      这个实例是许多同学多核DSP的入门实例，实例内也提供了简单的安装编译教程，不过并没有讲得很清楚，在编译的过程中还是会出现很多问题，本节主要提供一个详细的Vlfft安装编译步骤，但是由于这个实例涉及了较多关于多核通信的内容，所以这里并不详细解释代码原理，如果后面有时间的话，以后再具体分析这个实例。




**一、安装MCSDK和STMlib**

这MCSDK组件是Vlfft必须的，不过STMlib却是不必须的。

（1）MCSDK的安装

MCSDK ( Multicore Software Development Kit )是一个多核的软件开发组件，它主要是通过同BIOS协同开发，里面提供了比如PDK等工具，与CCS5.2兼容的MCSDK应该是2.0版本，而我使用的是2.1版本，会出现一个警告，但也是可以运行的。

MCSDK的下载地址：[http://www.ti.com/tool/bioslinuxmcsdk](http://www.ti.com/tool/bioslinuxmcsdk) ，这里我选择的是最后一个。

![](https://img-blog.csdn.net/20160427095447954?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


下载MCSDK后，直接点击安装，安装目录同CCS在同一文件夹下，如D:/ti，在安装组件上，可以全部选择啦。

![](https://img-blog.csdn.net/20160427095503298?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


      完成安装，重新启动CCS，查看相关组件是否安装到CCS中去（一般都自动导入了），如果不是，请手动导入。

![](https://img-blog.csdn.net/20160427095525599?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        关于SYS/BIOS的官方安装方法：[http://processors.wiki.ti.com/index.php/BIOS_MCSDK_2.0_Getting_Started_Guide](http://processors.wiki.ti.com/index.php/BIOS_MCSDK_2.0_Getting_Started_Guide)

（2）STMlib安装

        STMlib（System Trace）是一个系统追踪组件，Vlfft这个工程支持2种配置，一种是纯VLFFT运算工程（默认模块，也可以不安装STM），还有一种是集成了STM 模块的VLFFT工程，即在Build option中的build variables中定义了STM_LIBRARY_ROOT，如果未安装STM lib或者STM的路径不对，则可能编译报错。

        STMLib 下载地址：[https://gforge.ti.com/gf/project/ctoolslib/frs/](https://gforge.ti.com/gf/project/ctoolslib/frs/)

        STMLib 的学习使用：[http://processors.wiki.ti.com/index.php/Using_System_Trace_(STM)](http://processors.wiki.ti.com/index.php/Using_System_Trace_(STM))

        STMLib安装只需要将安装解压跟CCS放到同一个文件夹就可以了（如D:/ti）




**二、Vlfft的配置**

（1）将Vlfft项目的整体文件夹放到MCSDK的demo文件夹下：如\mcsdk_2_01_02_06\demos

（2）CCS中导入项目

在Project Explorer中右击选择Import...，选择导入一个已存在的CCS项目

![](https://img-blog.csdn.net/20160427095633673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


项目位置中输入Vlfft文件夹所在地址，此时在Vlfft文件夹下，发现了两个项目文件（6678版本和6670版本）这里选择6678版本就可以了。

![](https://img-blog.csdn.net/20160427095656711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果是选择6670版本，还需要打开vlfftconfig.h（vlfft\vlfftInc文件夹下），把常量EIGHT_CORE_DEVICE设为0，常量FOUR_CORE_DEVICE设为1，（默认是6678，与个这个设置刚好相反）

**TIPs:**

a).关于FFT的大小设置，将vlfftconfig.h中的如下宏中的一个设为1，其他为0

![](https://img-blog.csdn.net/20160427095748018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


b). DSP内核的数目设置，选择1，2，4，8个设置

![](https://img-blog.csdn.net/20160427095802143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）BIOS文件的配置

vlfft_evmc6678l.cfg文件是BIOS文件的配置文件，下一节会具体解释如何使用BIOS，这里直接双击点开，出现如下：

![](https://img-blog.csdn.net/20160427095821112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

BIOS的设置提供了界面和源代码编辑两种选择，这里我们点击Source，选择源代码编辑

如果是Debug模型，需要将.cfg源代码中的92~96行的代码注释，而Release模式，则不作改变。

![](https://img-blog.csdn.net/20160427095913065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


此时点击小锤子进行Debug模式编译，出现如下错误，这里的警告主要是MCSDK版本不配，关系不大。

![](https://img-blog.csdn.net/20160427095941731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


错误主要是placement fails for object

![](https://img-blog.csdn.net/20160427100008075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里提示.const所在段L2SRAM太小，内存段分配太小，需要打开链接命令文件.cmd（在Debug/configPkg/Linker.cmd)查看：

![](https://img-blog.csdn.net/20160427100033534?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里的L2SRAM只有156kb，实在太小，于是我们可以增加其大小到4M左右，但是我们不能直接到.cmd文件上修改，因为这个文件是由BIOS配置文件.cfg自动生成的，所以即使我们在.cmd文件中修改了，最后还是会出现原来的错误。

（3）在BIOS文件中修改内存设置

![](https://img-blog.csdn.net/20160427100116952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


SYS/BIOS是DSP的一个实时操作系统，其给DSP提供许多API函数和操作组件，CSS提供了界面式的配置方法（通过System Overview）可以打开，这里的Task、Swi、Hwi、Memory等都是BIOS的相关组件，这里绿色小勾表明了当前组件已经被安装到了BIOS中了，现在我们需要重新分配内存，所以我们点开Memory组件，选择Program模式

![](https://img-blog.csdn.net/20160427100141925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在Program中选择View，找到cpu中的MemoryMap中修改L2SRAM的大小和L2SRAM_1的起始地址，这里需要注意的是虽然错误只是提示L2SRAM的太小，但由于L2SRAM和L2SRAM_1相邻，所以如果修改L2SRAM的大小，就必然也要修改L2SRAM_1的起始地址。

![](https://img-blog.csdn.net/20160427100210874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20160427100306204?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


还有一种更为便捷的方法是直接在.cfg的源文件（Source）上修改，这里在文件末尾添加如下：

![](https://img-blog.csdn.net/20160427100251410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


var execontextInstance0 = xdc.lookup('xdc.platform.ExeContext.Instance#0');

execontextInstance0.memoryMap["L2SRAM"].len = 4194304;

execontextInstance0.memoryMap["L2SRAM_1"].base = 12582912;

此时再次点击编译，完成编译

![](https://img-blog.csdn.net/20160427100655052?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







**三、Vlfft的调试结果**

（1）导入目标配置文件.ccxml，选择C6678 Device Functional Simulator, Little Endian的目标配置文件

（2）将所有的（8个）核Group，点击运行，获得结果：

![](https://img-blog.csdn.net/20160427220551137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**四、集成了STM 模块的VLFFT工程**

前面我们安装了STMLib，这一节介绍如何使用STMLib

（1）在Build option中的build variables中定义了STM_LIBRARY_ROOT

![](https://img-blog.csdn.net/20160427220629850?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）在Linked Resources中修正STM_LIBRARY_ROOT的地址

![](https://img-blog.csdn.net/20160427220648465?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


做完上面两件事后，我们点击项目文件中的STMhelper.c，发现现在是可以打开的，说明STMlib已经成功导入了




Vlfft代码下载

[https://github.com/tostq/EasyMulticoreDSP/tree/master/4.vlfft](https://github.com/tostq/EasyMulticoreDSP/tree/master/4.vlfft)










