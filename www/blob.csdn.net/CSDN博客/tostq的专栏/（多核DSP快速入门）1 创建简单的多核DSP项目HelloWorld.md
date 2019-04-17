# （多核DSP快速入门）1.创建简单的多核DSP项目HelloWorld - tostq的专栏 - CSDN博客





2016年04月25日 23:17:52[tostq](https://me.csdn.net/tostq)阅读数：9369标签：[多核																[CCS																[DSP																[教程](https://so.csdn.net/so/search/s.do?q=教程&t=blog)
个人分类：[多核DSP](https://blog.csdn.net/tostq/article/category/6201995)

所属专栏：[多核DSP快速入门](https://blog.csdn.net/column/details/easymulticoredsp.html)](https://so.csdn.net/so/search/s.do?q=DSP&t=blog)




原创文章

转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)



教程目录：[http://blog.csdn.net/tostq/article/details/51245979](http://blog.csdn.net/tostq/article/details/51245979)

本节我们将运行第一个多核DSP程序，熟悉CCS开发环境，学会使用CCS调试工具，主要内容如下：
（1）新建CCS项目

（2）导入Target 仿真模块

（3）使用调试工具


**一、新建CCS项目**

选择File/New/CCS Project

![](https://img-blog.csdn.net/20160425230046366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**二、新建项目对话框**

![](https://img-blog.csdn.net/20160425230114471?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（1）Project name: 指项目名称，这里要注意的项目名称不区分大小写

（2）Output type: 输出类型，Executable指可执行文件（一般为.out格式，可直接下载到DSP内运行），Static Library指静态库（输出格式一般为.lib文件），我们这里的程序需要直接在DSP内运行，所以选择Executable格式

（3）Use default location: 是指项目存放的目录

（4）Device: 确定我们程序需要在什么型号的DSP下运行，如果我们选择的型号是通用型号如Generic C66xx Device，只能进行软仿真，但是如果我们有硬件评估板之类如EVMC6747，此时下面的Connection会高亮，可以选择需要硬件仿真器，如下所示：

![](https://img-blog.csdn.net/20160425230257603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（5）Project templates and example: 可以选择项目模板，有空项目也有许多DSP自带例子，这里我们先选择Empty Project
关于项目模板的创建和使用可以参考这个文档：[http://processors.wiki.ti.com/index.php/Project_Templates_in_CCS](http://processors.wiki.ti.com/index.php/Project_Templates_in_CCS)
（6）Advance setting，保持默认就可以了

![](https://img-blog.csdn.net/20160425230414175?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（7）点击Finish，完成项目创建，之后我们可以在Project Explorer看到工程项目，当前项目中只有一个Includes文件夹，项目名称旁边的[Active - Debug]是指当前项目是工作项目，当Project Explorer有多个项目时，同一时间只能有一个项目是有效的，通过单击项目就能实现切换。 

![](https://img-blog.csdn.net/20160425230455763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**三、新建与添加文件**
（1）新建文件helloworld.c，选择当前项目，右键单击，选择/New/File，得到新建文件对话框，输入文件名后Finish

![](https://img-blog.csdn.net/20160425230535842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（2）编辑文件内容，双击文件，就可以进行编辑了，DSP的语法同C语言基本一致。这里的DNUM是在c6x.h中定义，表明当前核的ID号。

![](https://img-blog.csdn.net/20160425230554748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（3）导入Linker文件（或叫链接命令文件），其是.cmd格式的，是项目必须的，主要是给处理器安排内存、堆、栈等方面的分配，可以自己手动生成，也可以到Ti官网上根据不同的处理器下载不同Linker文件：[http://processors.wiki.ti.com/index.php/Linker_CMD_Files_for_CCS](http://processors.wiki.ti.com/index.php/Linker_CMD_Files_for_CCS)
 。这里我们假设处理器平台为C6678，因此下载对应的C6678链接命令文件。
       下载后C6678.cmd文件后，回到项目上，右击选择Add Files...，将C6678.cmd导入到项目中，此时出现如下对话框：

![](https://img-blog.csdn.net/20160425230652328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择Copy files是将导入的文件复制到项目目录下，而Link to files只是相当于快捷方式，这里我们选择Copy files，此时我们完成了项目的文件导入，如下，还有一种导入方法，是直接将文件复制到项目目录下，然后刷新Project Explorer后就能自动导入项目需要的文件。

![](https://img-blog.csdn.net/20160425230730485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**三、编译项目**
当源文件、链接命令文件、头文件就位后，就可以进行项目编译了。项目编译有如下几种方法：
（1）右击当前项目，选择Build Project
（2）选择菜单栏Project，点击Build Project
（3）选择工具栏上的小锤子，此时还可以切换Debug/Release

![](https://img-blog.csdn.net/20160425230811001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击编译后，可能会得到如下提示，这里Console是控制命令窗口，Problems是提示错误和警告，这里的编译结果是没有错误和警告。

![](https://img-blog.csdn.net/20160425230835705?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**四、项目仿真**
这里仿真是软件仿真，我们需要首先生成一个Target文件，这个文件主要目的是定义仿真器

![](https://img-blog.csdn.net/20160425230915674?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击选择Target Configuration File，键入目标配置文件名称后，Finish完成安装

![](https://img-blog.csdn.net/20160425230942940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此时Project Explorer中多了一个.ccxml文件，我们可以看到这个文件上多了一个红色小叉，这个标记说明了当前文件有错误，因为当前我们还没有配置目标仿真器，所以会出现错误。

![](https://img-blog.csdn.net/20160425231009443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们选择C6678 Device Functional Simulator, Little Endian，单击Save，完成目标配置，此时我们也可以发现文件上红色小叉也消失了。

![](https://img-blog.csdn.net/20160425231041272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
目标配置文件完成后，就可以开始仿真了，这里同项目编译一样也有三种方式，不过这里我们推荐直接点击工具栏上的小虫，点击仿真后，因为这里的C6678是多核DSP，所以会现在如下选择DSP核的对话框。这里全选完成。

![](https://img-blog.csdn.net/20160425231235882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
仿真调试窗口如下：

![](https://img-blog.csdn.net/20160425231251382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里如果有部分窗口没有出现，也没关系我们可以在窗口中选择

![](https://img-blog.csdn.net/20160425231317149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（1）单核运行，我们选择一个核，点击运行，在控制台窗口可以看到结果

![](https://img-blog.csdn.net/20160425231333554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20160425231407431?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（2）多核运行，我们通过Shift全选所有核，右击选择Groups cores，然后点击运行，在控制台窗口可以看到多个核运行的结果

![](https://img-blog.csdn.net/20160425231430760?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20160425231457492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这里如果没有出现预期的结果，可以重新导入程序，重新测试下，这里导入的执行文件是前面编译过程生成的.out文件

![](https://img-blog.csdn.net/20160425231514573?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

完成调试后，我们点击红色小方块停止调试，重新返回到我们原来的界面
这里我们就完成了第一个多核DSP项目，是不是很简单，这里关于相关调试工具的使用，我们放到后面进行讲解~_~
**五、参考文献**
Ti官网资料
关于创建项目：[http://processors.wiki.ti.com/index.php/GSG:Creating_projects_v5](http://processors.wiki.ti.com/index.php/GSG:Creating_projects_v5)

关于调试项目：[http://processors.wiki.ti.com/index.php/GSG:Debugging_projects_v5](http://processors.wiki.ti.com/index.php/GSG:Debugging_projects_v5)

例程：[https://github.com/tostq/EasyMulticoreDSP/tree/master/1.HelloWorldtest](https://github.com/tostq/EasyMulticoreDSP/tree/master/1.HelloWorldtest)](https://so.csdn.net/so/search/s.do?q=CCS&t=blog)](https://so.csdn.net/so/search/s.do?q=多核&t=blog)




