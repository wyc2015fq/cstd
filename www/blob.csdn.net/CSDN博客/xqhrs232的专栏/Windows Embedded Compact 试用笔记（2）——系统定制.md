# Windows Embedded Compact 试用笔记（2）——系统定制 - xqhrs232的专栏 - CSDN博客
2010年06月30日 09:54:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：840
原文地址::[http://www.cnblogs.com/yashi88/archive/2010/06/26/1765565.html](http://www.cnblogs.com/yashi88/archive/2010/06/26/1765565.html)
好了，[上篇文章](http://www.cnblogs.com/yashi88/archive/2010/06/23/1763029.html)探讨了一下WinCE 7的新特性，现在是时候动手来实际操作一下了，第一步，我们会定制一个运行在Vitual PC上的image。要完成这个任务，你要保证你的电脑上安装了：1.Windows Virtual PC 2.Visual Studio 2008+SP1 3.Windows
 Embedded Compact 7 4. Platform Builder 7  其中Virtual PC你可以去[这个地方](http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=2b6d5c18-1441-47ea-8309-2545b08e11dd)去下载。
    闲话少说，安装好上面说的软件以后，就可以正式开干了，运行Visual Studio 2008，选择新建一个项目，会发现在项目类型里多了Platform builder 2008，选择它，然后输入一个项目名称，这里我们使用MYWinCE7，接下来的过程和以前版本操作方法没什么不同，首先是BSP选择，这里我们使用“Virtual PC :x86”（如图1），然后是选择模板，这里我选择PDA Device下的Mobile Handheld，后面的组件选择里，记得在Networking and Communications里面勾选上“Wired
 Local Area Network ”。
![WinCE7_7](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_7_thumb.png)
                               图1
![WinCE7_8](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_8_thumb.png)
                               图2
在OS Design Project Wizard Complete对话框里，取消对“Build the following cnfiguration after I click finish”的选择，否则在完成这个向导后，PB会马上开始自动编译系统，而我们往往还需要做一些其它设置。
![WinCE7_9](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_9_thumb.png)
                                       图3
点击“完成”按钮，确认出现的“安全警告”对话框。后面的操作，这里就不详述，和以前的版本基本一样，你可以在catalog里面选择/删除组件，可以修改本地化等选项，完成后从菜单里选择build。
     等待一些时间，完成build以后，接下来的工作就是设置virtual pc，然后下载nk 。这部分是WinCE7新的内容，我会详细讲一讲。首先运行Windows Virtual PC，如果“New Vritual machine Wizard ”对话框没有自动弹出的话，从菜单里运行它：
![WinCE7_10](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_10_thumb.png)
                                图4
选择“Create a virtual machine”，然后按“Next”，在接下来的对话框中输入虚拟机的名字，比如“MyVPC”，点“Next”后选择“Operating System”为“Other”，下一个对话框里选择默认的RAM设置，接下来的对话框选择“An existing virtual hard desk”，点击Next
![WinCE7_11](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_11_thumb.png)
                               图5
WinCE7提供了一个虚拟机在C:/WINCE700/PLATFORM/VIRTUALPC/VM目录下，名称是hd0_sample.vhd，在接下来的对话框中，选择该文件。
![WinCE7_12](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_12_thumb.png)
                                   图6
点击Next，然后在确认对话框中选择Finish，完成新虚拟机的创建。接下来对虚机做一个设置，在Virtual PC console窗口单击Setting，选择Networking，根据你的实际，选择合适的Adapter，完成后点OK，就完成了虚机的创建。
![WinCE7_14](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_14_thumb.png)
                            图7
OK，现在运行你的虚机，在Virtual PC Console窗口下，双击我们刚创建的虚机名称“MYVCEPC”运行它，按照提示，在5秒内，按下空格键，进入bootloader的设置菜单：
![WinCE7_15](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_15_thumb.png)
                                 图8
选择2，设置boot source（从哪里去下载nk)，因为我们要从网络获取NK，所以选择2。接着选择4设置网络，按以下参数设置：
![WinCE7_17](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_17_thumb.png)
                                 图9
当然，这里的IP要根据实际设置，而且，如果网络支持，也可以使用DHCP，设置好以后，选择7，保存设置。接着在vs2008里面（在项目打开情况下），选择Target——Connectivty Options，添加一个设备“VECPC"，设置Kernal Download,Kernal Transport都为Ethernet，然后单击Kernel Download的Settings:
![WinCE7_13](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_13_thumb_1.png)
                                 图10
在虚机里选择0，这时候，虚机会发送BOOTME信息出来，上面的窗口就会显示设备名称：
![WinCE7_18](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_18_thumb.png)
                    图11
选择该设备，然后点击“Apply”按钮，在点击“Close”，关闭Target Device Connectivity Options窗口，然后选择Target——Attach Device，就会出现Device Status窗口，显示正在下载nk，等下载完后，在虚机里面，WinCE7就运行起来了：
![WinCE7](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_thumb.png)
                                 图12
上面的图示，是选择Standard Shell时的情况，在Catalog里面重新选择Silverlight for Embedded base Consumer Shell，重新build，下载，运行起来后效果如下图：
![WinCE7_2](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_2_thumb.png)
                               图13
![WinCE7_4](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_4_thumb_1.png)
                               图14
![WinCE7_5](http://images.cnblogs.com/cnblogs_com/yashi88/WindowsLiveWriter/WindowsEmbeddedCompact2_FBFF/WinCE7_5_thumb.png)
                                       图15
可以看得出，使用了Silverlight以后，用户界面比原来漂亮多了。所以，下面我准备研究一下WinCE7 上的Silverlight。
