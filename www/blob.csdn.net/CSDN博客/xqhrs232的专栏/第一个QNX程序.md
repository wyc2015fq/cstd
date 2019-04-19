# 第一个QNX程序 - xqhrs232的专栏 - CSDN博客
2014年05月13日 10:44:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：899
原文地址::[http://blog.csdn.net/keyboardota/article/details/6756992](http://blog.csdn.net/keyboardota/article/details/6756992)
QNX开发环境和QNX虚拟机都搭建好了，开始写第一个QNX程序。
关于QNX程序开发的最好参考是QNX官网上的pdf书《10 Steps to Developing a QNX Program: Quickstart Guide 》，清晰地说明了如何通过十个步骤开发第一个QNX应用程序。有好心人将它翻译成中文，中文名叫《10步开发一个QNX程序》。《10步开发一个QNX程序》一贴可以轻松地从网上搜索到，而英文的《10 Steps to Developing a QNX Program: Quickstart Guide 》一书可以从QNX官网上下载，再次共享一下QNX官网下载资料的地址：
[http://www.qnx.com/download/group.html?programid=20945](http://www.qnx.com/download/group.html?programid=20945)
因为以上两份资料已经很详细的说明了QNX程序的开发过程，下面就简单说明一下步骤：
环境准备：
1. QNX Neutrino 虚拟机
2. QNX Momentics 开发环境
关于以上环境的准备可以参考之前的博文。
关键步骤：
1. 创建、编译QNX C项目
1.1 在QNX Momentics环境中选择 “File -> new -> QNX C project”。
![](http://hi.csdn.net/attachment/201109/7/0_1315381153x362.gif)
1.2 在项目创建向导的第一页填写项目名称
![](http://hi.csdn.net/attachment/201109/7/0_1315381186V1qR.gif)
1.3 在项目创建向导的第二页选择X86为目标系统，因为我们使用的QNX虚拟机是VMware上运行的系统，使用X86体系的CPU。
![](http://hi.csdn.net/attachment/201109/7/0_1315381212znfh.gif)
1.4 点击“finish”按钮创建项目，创建项目后出现程序编辑界面，如下图：
![](http://hi.csdn.net/attachment/201109/7/0_13153812485cfc.gif)
1.5 编译项目，在左边导航栏中选择项目，点击右键，选择“Build Project”
![](http://hi.csdn.net/attachment/201109/7/0_1315381271vDoC.gif)
2. 准备运行环境并调试应用
2.1 获取目标系统的IP，首先启动QNX Neutrino虚拟机，在QNX Neutrino系统中启动terminal窗口，在窗口中使用ifconfig获取目标系统的IP地址。如下图获得的IP是：192.168.201.128。
2.2 启动qconn程序，获取IP地址后同样在terminal窗口中使用qconn命令启动qconn程序。注意qconn程序运行后如果不出错的话没有任何输出，习惯DOS系统中命令运行成功会有提示的同学们需要习惯一下，运行界面如下图：
![](http://hi.csdn.net/attachment/201109/7/0_1315381319T7YH.gif)
2.3 在Momentics中配置目标系统。 在Momentics环境中选择项目，点击右键，选择“Rus As -> Run Configuration”, 会出先目标系统配置界面，如下图。在配置界面左边的导航栏中选择“c/c++ QNX QConn(ip)”,点击左上方的“新建”图标，出现目标环境配置界面，需要配置的有：
2.3.1 名称，可以随便取个名字
2.3.2 项目，点击项目栏旁边的“Browse...”按钮，选择你希望调试的应用
2.3.3 应用，选择项目后点击“Search Project...”可以选择指定项目中可以运行的程序，如果之前编译成功的话，会出现两个同名的应用供选择，一个结尾带个g字，一个不带g字。带g字的是带有调试信息的，所以我选择了带g字的。
2.3.4 目标系统，点击目标系统栏右边的“Add New Target...”按钮，会出现目标系统配置界面，填写上一步获取的IP地址即可。
![](http://hi.csdn.net/attachment/201109/7/0_1315381373IpPC.gif)
2.4 运行应用，上一步配置完成后，点击“Apply”按钮应用配置，再点击“Run”按钮运行程序，运行结果如下，注意printf输出的内容在Momentics的Console界面中显示出来：
![](http://hi.csdn.net/attachment/201109/7/0_1315381419RJ8u.gif)
