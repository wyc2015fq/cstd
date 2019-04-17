# Visual Studio开发Windows Mobile程序 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月29日 17:36:24[boonya](https://me.csdn.net/boonya)阅读数：2536








转自：[http://developer.51cto.com/art/200908/144102.htm](http://developer.51cto.com/art/200908/144102.htm)




Visual Studio 2005无疑是开发Windows Mobile的最佳工具，其本身就集成了Windows Mobile开发环境，.Net Compact Framework也同时升级到了2.0，只是它自身所带的模拟器是Windows CE 2003的，有些落后时代了，呵呵。


本文就软件的安装和配置做一个简单介绍。

Visual Studio开发Windows Mobile测试环境：


WindowsXP＋SP2


Microsoft Visual Studio 2005＋SP1

安装的软件包：


Microsoft ActiveSync 4.2


Windows Mobile 5.0 Pocket PC SDK


Windows Mobile 5.0 Emulator Images for Pocket PC - CHS


所需要的软件和补丁都可以从微软网站下载。

Visual Studio开发Windows Mobile：安装顺序推荐


由于在安装时软件的先后安装顺序可能会影响到后续是否能成功安装或使用，因此在此给一个建议安装顺序：


1．Microsoft ActiveSync（一定要4.0版本之上才支持Windows Mobile 5.0）


2．Microsoft Visual Studio 2005＋SP1（VS的安装就不多说了，只是打SP1补丁实在太慢了，我大概要等了2小时）


3．Windows Mobile 5.0 Pocket PC SDK.msi（安装后出现的模拟器和镜像都是英文的，虽然一样用，但自然有点不大爽了）


4．Windows Mobile 5.0 Emulator Images for Pocket PC - CHS.msi（安装完后模拟器和镜像就都成为中文的了）


如果还要安装for Smartphone版本的，步骤也类似。


全部安装完成后（好漫长的过程，没个半天搞不定吧），启动VS2005，菜单“工具”－》“设备仿真器管理器”，启动设备仿真器管理器，此时就能看见一长串列表，其中就有Windows Mobile 5.0 Pocket PC的中英文模拟器和镜像。


选中“CHS Windows Mobile 5.0 Pocket PC Emulator”，鼠标右键“连接”，就可以启动模拟器。

Visual Studio开发Windows Mobile范例




下面用C#开发一个简单的Demo演示。


依次选择“Visual C#”－》“智能设备”－》“Windows Mobile 5.0 Pocket PC”－》“设备应用程序”新建一名为DeviceApplication1的项目。


设计一Form如下图。


编辑button1的click事件 


- privatevoid button1_Click(object sender, EventArgs e)   
- {   
- label1.Text = "Hello World";   
- } 


全部完成后，选择菜单“调试”－》“开始执行（不调试）”，此时会提示选择要部署的位置，当然选择“CHS Windows Mobile 5.0 Pocket PC Emulator”了。


点击“部署”，VS2005就开始在模拟器上开始部署了，这个过程比较慢，VS下方的状态栏会有进度条显示，此时模拟器屏幕中央也会有一个小图标显示，稍加等待。


部署完成后VS状态栏会有“部署已成功”的提示，此时模拟器上会自动打开部署上去的程序。这个程序的安装位置在Windows Mobile上的“我的设备\Program Files\DeviceApplication1”目录下。


点击按钮，显示的文字就会改变。


这样，一个小Windows Mobile程序就完成了，Windows Mobile的开发看来也不是很难啊，不过仅限于使用C#，如果用C++开发本机代码那就是完全不同的风格了，给人的感觉要复杂很多。


顺便提一下，模拟器中的截图使用Visual Studio 2005自带“远程工具”中的“远程放大”比较方便，直接用Alt＋PrintScreen是截不下模拟器的图的，呵呵。


本文来自风林火山博客：《Visual Studio 2005中配置Windows Mobile 5.0开发环境》




