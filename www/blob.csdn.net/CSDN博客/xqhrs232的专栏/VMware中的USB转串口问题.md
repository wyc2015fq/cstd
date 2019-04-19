# VMware中的USB转串口问题 - xqhrs232的专栏 - CSDN博客
2016年03月21日 23:19:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2169
原文地址::[http://bbs.ednchina.com/BLOG_ARTICLE_2148549.HTM](http://bbs.ednchina.com/BLOG_ARTICLE_2148549.HTM)
相关文章
1、[主机和vmware实现串口通信](http://blog.chinaunix.net/uid-10369152-id-2960273.html) ----[http://blog.chinaunix.net/uid-10369152-id-2960273.html](http://blog.chinaunix.net/uid-10369152-id-2960273.html)
2、基于VMware虚拟机Linux系统串口配置----[http://www.linuxidc.com/Linux/2013-03/81899p2.htm](http://www.linuxidc.com/Linux/2013-03/81899p2.htm)
3、[vmware串口通信配置](http://blog.chinaunix.net/uid-20761674-id-74919.html) ----[http://blog.chinaunix.net/uid-20761674-id-74919.html](http://blog.chinaunix.net/uid-20761674-id-74919.html)
前几天调通了在windows下usb转的串口，连到fs2410的开发板上，超级终端和DNW都输出正常，后来想在linux下试试效果（在windows
 xp下vmware中的redhat9），可费了两天劲也没见有输出内容。郁闷的不行。
先是从网上找资料，有的说在虚拟机下要用ttyUSB0口，我找机器上的每个usb口试了一遍，也无果而终。
后来又找pl2303（我的转串口里的芯片）的linux的驱动，折腾半天，也是无果。
今天下班后，又报着试试的心理，又从头试，把这个转接口当作一个物理的串口，（自己总觉得这个转串口相对虚拟机来说，就是一个真实的串口，后来查了一下vmware的虚拟原理，这样认为没找到明显的错误，暂且这样认为吧，），所以我又重新设了一下vmware有关串口的设置：设备状态选中“已连接”和“打开电源时连接”；使用物理串行端口选中windows下设备管理器里显示的串口号（我这里显示是com5，也不清楚这个顺序是从哪排的，试遍所有usb口，也是从com4开始，没找到com1-3），不建议用自动探测。特别提示，这几项设置要在虚拟机启动，而不启动虚拟机里的linux。然后就启动linux，进入口又从头开始设minicom的配置：用minicom
 -s打开设置项=》Serial port setup =》Serial Device设为/dev/ttyS0,其它项按实际值设即可。保存后再重新进minicom，给开发板上电，竟然又可以看见久违的串口信息了，惊奇之余，回想一下，这个过程，有种无语的感觉，从网上找资料一定根据自己的情况，灵活运用，不可照搬，把自己领到一个小胡同出不来了，
后来发现另一个关键点，就是vmware上在设置串口的界面上有一个I/O模式，当那个选项选中时，串口输出输入正常，不选中时，虽然有输出，但有乱码。
最近在学习嵌入式linux开发,本人是在虚拟机中装的linux，起初传输文件先是从linux用传到windows下，然后再传到开发板上，必须在linux和windows之间频繁切换，感觉太麻烦了！于是尝试在虚拟机中使用串口与开发板直接通信，在网上查了一些相关资料加上一番摸索，终于搞定了。
首先说明一下，我用的虚拟机是VMWare6.0，ubuntu-8.10-server linux系统。开始要在VMWare中设置串口，默认的并没有启动串口，先添加一个串口，串行端口选项选择“使用位于主机上的物理串行端口”，下一步中选择使用相应的串口（本人先是在windows下安装了usb转串口驱动（PL-2303
 Driver），这时开发板插到usb口上会显示成一个串口，我的是COM3，在这选COM3口。至于不安这个驱动，在这一步中选自动侦测可不可以，本人没尝试，就不知道了，嘿嘿）。
连接好开发板，打开linux虚拟机，进入终端输入sudo minicom –s，进入minicom的设置模式。
1. 选择串口：在选择菜单中的“Serial port setup”，按回车，再按“A”以设置 “Serial Device”/dev/ttyS0按回车返回。
注：网上朋友说要检查一下/dev下是否有ttyUSB0这个东东，不过我的系统中没有，倒是有一堆ttys0, ttyS0, ttys1, ttyS1……一堆东西。不太理解，感觉是串口用的，有哪位大侠知道，恳请指教。
此时应将“Serial Device”改为/dev/ttyUSB0
以后按下面设置就可以了
下面的设置参考的网上的资料，在此感谢一下
2. 设置波特率：按“E”键进入设置“bps/par/Bits”（波特率）界面，设置波特率为115200，按回车返回。
3. 数据流控制：按“F”键设置“Hardware Flow Control”为“NO”.
其它为缺省设置，然后按回车到串口设置主菜单，选择“Save setup as df1”，按回车键保存刚才的设置（网上说保存到“/etc/vminirc.df1”，经查看我的实际保存到的是/etc/minicom/minirc.df1，版本不同吧），最后退出设置菜单就可以使用minicom与开发板通讯了（不知为什么我的还要重启虚拟机才能进行通信）。
在使用配置中，本人有些小的体会：如果在配置中，输入的是 minicom –s ,没有用sudo的话，配置文件是保存在个人工作目录的.minirc.df1文件中。以后再使用minicom –s不管加不加sudo，都会调用~/.minirc.df1的配置，如果发现改不了配置，可能是这个原因（这点令本人当初很是困惑）。而使用命令 sudo
 minicom restart 时,它会找不到配置菜单，而使用默认配置，不知是不是bug。
注：minicom中有用的功能
命令帮助—“Ctrl A”后按“Z”
清屏 --“Ctrl A”后按“C”
设置 --“Ctrl A”后按“O”
发送文件--“Ctrl A”后按“S”
退出 --“Ctrl A”后按“Q”
本人对此只是简单实现了虚拟机下与开发板的串口通信，对有些问题没有深入研究，如有错误，欢迎指正。
虚拟机下的usb 转串口
网络技术2009-12-18 10:13阅读22评论0字号：
 大大中中小小 1、打开Vmware workstation ,单击菜单栏上的VM下的setting，进入virtual
 machine setting对话
2、单击add，添加serial port，(我这里选择了我windows下显示的usb转串口生成的COM4口，这里出了问题，其实只要点下一步就可以)，直到完成为止。
3、连接好开发板，打开linux虚拟机，进入终端输入minicom –s，进入minicom的设置模式。
a） 检查是否存在设备/dev/ttyUSB0,如果没有就建立一个：（我的一直找不到/dev/ttyUSB0,按下面的方法新建了一个，打开minicom时弹出没有此设备，我一查看原来usb转串口还运行在windows下面，没有载入linux,加载之后在/dev中发现了ttyUSB0，很顺利的连上了）
mknod /dev/ttyUSB0 c 188 0
a)选择串口：在选择菜单中的“Serial port setup”，按回车，再按“A”以设置 “Serial Device”/dev/ttyUSB0（注意其中的USB是大写），按回车返回。
b)设置波特率：按“E”键进入设置“bps/par/Bits”（波特率）界面，如果按“I”以设置波特率为115200，按回车返回。（我使用的是2440的板子）
c)设置流控制按“F”，“Hardware Flow Control”为“No”.
注：minicom中有用的功能
命令帮助 “Ctrl +A”后按“Z”
清屏“Ctrl +A”后按“C”
设置“Ctrl +A”后按“O”
发送文件“Ctrl +A”后按“S”
退出“Ctrl +A”后按“Q”
USB转串口使用设置：
在Windows下可以使用超级终端来连接交换机和路由器等工业设备，而且在Windows下使用usb转串口的线也有相应的驱动，但是如何在Linux下使用呢？
首先要有一个Linux下的终端软件叫minicom假如没有装这个的朋友可以到自己的源中去下载，然后你就要插上你的usb转串口的线，在Linux下Usb转串口的线几乎不需要驱动，你插上以后在你的dev的目录下应该会有一个ttyUSB0的文件，假如有那么恭喜你了，你的这根线现在是可用的了。进入到终端输入su以root用户进行登录，使用minicom必须要用root用户，然后输入minicom
 -s 进行设置，进入设置界面以后你会看到有几个选项，分别是：
Filename and Paths, File transfer and protocols, Serial port setup, Modem and dialing, Screen and keyboard, Save setup as df1, Save setup as…… , Exit from minicom
进入Serial port setup
输入选项字母将
第一项改成 A—Serial Device : /dev/ttyUSB0
第二项：B—lockfile Location: /var/lock
第五项：E—Bps/par/Bits :57600(这里要注重，这个值要改成你的连接的设备的波特率）
第七项：F—Hardware Flow Control: yes
然后退出进入Modem and dialing
将A—initing string B－Rest string K－Hang-up string 这三项的值去掉。
然后退出选择 Save as df1,然后选择EXIT FROM MINICOM 到这里的终端已经设置好了。
接下来你就可以将你的串口线接上设备，然后在终端下输入minicom就可以对设备进行调试了。
虚拟机Linux使用usb转com口线默认分类 2009-06-19 23:53:13 阅读295 评论0 字号：大中小
因为笔记本没有串口，所以需要用usb转接线，买的力特的usb to rs232 cable，质量不错。
为了工作方便，所以在笔记本上用VMware6.0绿色版装了Linux。
现在是要用虚拟机里的Linux访问串口，以方便嵌入式板上的串口开发。
过程：
1.windows下安装好usb to rs232的驱动程序，本机为例，在设备管理器中可以看到“Prolific USB-to-Serial Bridge(COM8)”；
2.VMware Workstation中，虚拟机->可移动设备->USB设备，里面有“Prolific USB设备”，未打勾，表明未连接；
3.选中此项后会安装vmware的驱动，安装完后可能需要重新插拔接口，操作完成后打勾表明已连接(windows下COM8此时不再可用)；
4.在虚拟机Linux下用dmesg | grep usb命令查看到类似以下的信息：
usb 2-1: new full speed USB device using address 6
usb 2-1: PL-2303 converter now attached to ttyUSB2
5.在虚拟机下运行minicom -s
修改Serial port setup: /dev/ttyUSB2
6.minicom的退出指令为ctrl a + q
//====================================================================================
备注：
1〉今天试了一下，在虚拟机下映射一下PC的USB转串口，确实可以在虚拟机种操作成功！！！

