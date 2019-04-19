# WINCE6.0 CAB文件的制作与安装 - xqhrs232的专栏 - CSDN博客
2013年01月23日 15:40:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：783
原文地址::[http://blog.csdn.net/loongembedded/article/details/8272350](http://blog.csdn.net/loongembedded/article/details/8272350)
相关网帖
1、WinCECabManager制作修改CAB安装详细图文教程----[http://wenku.baidu.com/view/d8d17e5abe23482fb4da4c0c.html](http://wenku.baidu.com/view/d8d17e5abe23482fb4da4c0c.html)
2、CSDN有WinCE Cab Manager3.0中文版.rar可以下载----[http://download.csdn.net/download/she1354/3838791](http://download.csdn.net/download/she1354/3838791)
1.  WinCE_CAB_Manager3.0的制作向导
运行此软件，应用程序信息的对话框：
![](https://img-my.csdn.net/uploads/201212/08/1354942530_9044.gif)
图1
填入公司的名字和程序的名字之后，下一步：
![](https://img-my.csdn.net/uploads/201212/08/1354942534_8914.gif)
图2
根据需要选择支持的设备，下一步：
![](https://img-my.csdn.net/uploads/201212/08/1354942538_3789.gif)
图3
选择安装此CAB文件时需要的处理器类型，一般选择ALL/CEF就可以，下一步：
![](https://img-my.csdn.net/uploads/201212/08/1354942542_6304.gif)
图4
然后向导走完，最后一步告诉基本CAB安装项目创建完，下面就是定制自己的CAB安装包需要做的事情清单，包括设置程序安装路径，添加程序应用程序包含的文件，按需要添加快捷方式以及注册表项，另外还有其他相关的设置，点击完成进入CAB定制界面：
![](https://img-my.csdn.net/uploads/201212/08/1354942547_4625.gif)
图5
2.  CAB安装文件制作
2.1 设置程序安装默认路径
点击“安装->属性->安装目录”
![](https://img-my.csdn.net/uploads/201212/08/1354942550_5822.gif)
图6
我们的CAB程序默认安装的路径在根目录的Flash Disk2目录下，然后点击应用，再点击确定。
2.2 添加程序所需要的文件
点击左侧的“文件”，然后点击右键，选择“添加”来添加此CAB文件所需要的文件
这些文件中的exe程序(除了AppCenter.exe)都保存在安装目录的AppStore文件下面，DeviceAPI.dll和fun_config.ini文件是放在Flash Disk2目录下，UiShowConfig.ini放在Flash Disk2\AppConfig目录下，Appcenter.exe放在安装目录下。
⑴设置除了AppCenter.exe之外，其他exe文件的保存路径
按下Ctrl键，用鼠标点击需要放在安装目录的AppStore文件下面的所有exe文件，选择完成之后，点击右键，选择“属性”，如下图：
![](https://img-my.csdn.net/uploads/201212/08/1354942554_4010.gif)
图7
⑵ 设置DeviceAPI.dll和fun_config.ini文件的保存路径
这两个文件是放在Flash Disk2目录下，操作办法和上面类似，如下图：
![](https://img-my.csdn.net/uploads/201212/08/1354942557_1728.gif)
图８
⑶ 设置UiShowConfig.ini文件的存放路径
此文件放在Flash Disk2\AppConfig目录下，如下图：
![](https://img-my.csdn.net/uploads/201212/08/1354942560_9432.gif)
图9
到此对此CAB文件所需要的文件都包含进来，并且设置了它们对应的保存路径，见下图：
![](https://img-my.csdn.net/uploads/201212/08/1354942564_6667.gif)
图10
2.3 添加程序快捷方式
右键点击左侧的“快捷方式”，选择“添加”，进入创建快捷方式的向导，如下图：
![](https://img-my.csdn.net/uploads/201212/08/1354942568_8357.gif)
图11
我们指定快捷方式的名称为“测试程序”，下一步：
![](https://img-my.csdn.net/uploads/201212/08/1354942571_1621.gif)
图12
我们这里为AppCenter.exe创建快捷方式，下一步：
![](https://img-my.csdn.net/uploads/201212/08/1354942574_4977.gif)
图13
因为桌面是在根目录的Flash Disk目录下，且我们的AppCenter.exe的快捷方式是要保存在桌面上，所以就有了上面的目录，当然如果是英文版本的系统个，上面的“桌面”就应该改为“Desktop”，选择完成，到此可以看到快捷方式设置后的界面：
![](https://img-my.csdn.net/uploads/201212/08/1354942578_7729.gif)
图14
其他根据需要来设置，设置完成之后选择“保存”就可以生成CAB文件了。
3.  安装CAB文件
点击安装CAB文件，弹出下面的对话框：
![](https://img-my.csdn.net/uploads/201212/08/1354942585_9630.gif)
图15
可以看到默认的安装目录就是根目录下的Flash Disk2，安装完成之后，我们可以看到Flash Disk目录下的内容如下图所示：
![](https://img-my.csdn.net/uploads/201212/08/1354942589_2556.gif)
图16
到此制作CAB文件及安装就介绍完成了，更深入的应用还有待多学习和应用。
