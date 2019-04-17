# VC6.0+ddk+DriverStudio3.2安装与配置 - 博客堂 - CSDN博客





2016年11月18日 17:42:28[最小森林](https://me.csdn.net/u012052268)阅读数：3926








再学习VS2013+wdk之前。首先学习VC6.0+ddk+DriverStudio3.2环境下的驱动开发，来理解驱动的世界。

完全新手，从零开始，亚历山大，且行且珍惜。。。。。




首先是：环境的搭建






**安装和配置**

一.


软件的安装顺序:Windows XP（虚拟机中） --> VC6.0 --> WinXP_DDK -> DriverStudio3.2，


如果顺序装错了，那么把DriverStudio3.2删除再重装就OK了。








下载地址：


1.vc6.0英文原版（最好不要用中文版）:  http://down.liangchan.net/microsoftvisualc-en6.0.zip




2.WinXP_DDK：安装DDK，安装时注意需要将所有的组件、工具、例子等都安装（避免出现hidport.h找不到的问题）。[http://download.microsoft.com/download/9/0/f/90f019ac-8243-48d3-91cf-81fc4093ecfd/1830_usa_ddk.iso](http://download.microsoft.com/download/9/0/f/90f019ac-8243-48d3-91cf-81fc4093ecfd/1830_usa_ddk.iso)




3.DriverStudio3.2：    
  http://ttl4.pc6.com/tx/driverstudio.zip （安装时会检测系统有无D盘，如果虚机中XP无D盘，安装不上。用“分区助手”分下区即可）











二.配置DriverStudio


1.从网上找到库文件 ntstrsafe.lib+csq.lib.rar，把解压出来的两个库文件拷贝到WinXP_DDK的安装目录下的库目录中.


（我的是 C:\WINDDK\2600\lib\wxp\i386）。启动vc6，然后进行简单的配置：菜单DriverStudio菜单下的DDK Build Settings，在弹出的对话框中选择已经安装的DDK目录（比如我的是C:\WINDDK\2600），




（ntstrsafe.lib+csq.lib.rar下载地址： [http://download.csdn.net/detail/victoryckl/4444640](http://download.csdn.net/detail/victoryckl/4444640)）








2.在Windows DDK compiler Options中选择"Enable only for Driver Studio"。

VC6.0-->Tools-->Options，点击"Directories"选项卡：
1）·“Show directories for:"下选择Include files，然后检查有没有包含ddk的头文件目录（我的是C:\WINDDK\2600\inc\wxp），如果没有则加上；

以及 C:\WINDDK\2600\INC\W2K 
             C:\WINDDK\2600\INC\DDK\W2K 
             C:\WINDDK\2600\INC\DDK\WDM\W2K 

2）·“Show directories for:"下选择Library files，然后检查有没有包含ddk的库文件目录（我的是C:\WINDDK\2600\lib\wxp\i386），如果没有则加上；

以及  C:\WINDDK\2600\LIB\W2K\I386 








三。编译库文件（一定注意32位）

·编译适合本机使用的库文件（以下内容，如果用DDK编程者可以不看）
(1).启动VC6.0。开始-->所有程序 -->Compuware DriveStudio-->Develop-->DDK Building Settings，确保“DDK Root Directory”下方的内容是ddk的安装目录（比如我的是C:\WINDDK\2600），然后点击下方的"Luanch
 Program"正式启动vc6的开发环境。
(2).进入菜单File-->Open Workspace(打开位于DriverStudio3.2安装目录的\DriverWorks\Source\vdwlibs.dsw)--> 进入菜单Build-->batch Build，点击“Select x86"按钮只选中全部的32位库（我的电脑是32位的。注意：对于32位的电脑一定不要选中64位的库，否则后面编译会出错）-->点击按
 钮"Rebuild AlL”开始编译。(注意：这种方法我曾经成功过一次，但后来再也没成功过，于是百度GOOGLE很久，发现下面这样也行：设置VdwLibs为活动项，选中Win32 WDM Checked,然后用DriverStudio里面的 Build with
 Build.exe即可以。然后再选Win32 WDM Free再Build一下，这样就出现了想要的vdw_wdm.lib）
注:如果出现无法打开文件这类的错误,一般都是DDK Build Settings指向不对，或安装顺序有误，或者你在３２位机器上选中了６４位库。










**四·编译一个DriverStudio自带的实例**（１）·启动ｖｃ６，点击菜单 File-->Open Workspace，打开项目文件C:\Program Files\Compuware\DriverStudio\DriverWorks\Examples\wdm\hellowdm \HelloWdm.dsw，然后编译，如果没有报错，那说明安装和配置成功。但请你别高兴的太早，开发环境安装配置成功只是万里长城的第一步，剩下的你 就是要理解驱动模块的[架构](http://lib.csdn.net/base/architecture)和具体的代码编写了。







**五·使用Driver wizard生成驱动程序框架**

(1).在VC6.0的界面下,点击菜单DriverStudio-->Driver wizard，此后系统会一步一步引导你完成设置，最后自动生产的驱动程序框架。

(2).设置好后将生成驱动文件,然后用VC6.0进行编译：进行Build菜单,Rebuild AlL将生成.sys文件,说明驱动模块编译成功！

注:如果出现无法打开ntstrsafe.lib的错误，说明系统缺少这个库文件，参照上面的方法补上这个库文件，或者进入菜单 Project-->Settings，鼠标点击左边方框里的最上面一行，然后右边"Project Option"下的ntstrsafe.lib并删除它。





