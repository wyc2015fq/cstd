# Raspberry Pi 基础设置与 opencv - 三少GG - CSDN博客
2013年06月06日 21:46:55[三少GG](https://me.csdn.net/scut1135)阅读数：6092
【1】我为什么爱树莓派 / Why I love Raspberry Pi
话说现在的电脑都已经很便宜了，随便买一台上网本都比树莓派的性能要强得多，而且用来开发软件也更容易（编译快，运行快，等等），为什么要“自虐”般地使用树莓派这样一个貌似“不健全”的卡片式电脑呢？
或者换另一个问题，明明都已经有了很多很流行的嵌入式平台，例如Arduino这样的东西，而且Arduino还很便宜，为什么要用树莓派这样的东西来实现一些它可以实现的功能？
我只想从我个人的角度来谈谈这些问题。
先从一个故事说起吧。很久以前，我想自己做一个桌面软件与嵌入式软件结合的系统——例如，一个搭载嵌入式系统的手持设备，需要与Windows上的软件交互——但我个人能力有限，无法自己开发嵌入式程序，因此，我一直想学一些嵌入式系统开发知识，由于工作原因，计划一直搁置。我一直觉得，如果能在嵌入式系统上直接用C++，调用STL库来写程序，那该多好啊，那样我就可以利用自己已经掌握的一部分知识来实现自己的愿望了。

终于，树莓派出现了。装上了Arch Linux ARM的树莓派就像是一个完整的桌面电脑，我可以在上面跟x86计算机一样，没有多少差别地编写程序了！而树莓派同时又具有超便宜的价格、超低的功耗、超小的体积，这些完美地结合在一起，圆了我可以一个人“做点什么”的梦。
所以我爱树莓派爱得深。尽管我不如那些大神一般牛，我不能用它设计出一个令人惊叹的系统，但是我也从上面找到了自己的快乐。
------------------------------------------------------------------华丽丽的分割线-----------------------------------------------------------------------------------------
安装准备：  
![](https://img-blog.csdn.net/20130607203946906)
你还需要以下这些东西：
▪ 一张SD记忆卡
▪ 一个SD读卡器，用于将系统映像写入到SD卡中
▪ 供电来源。我们用的是一个旧的安卓手机充电器，你需要一个5V的micro USB接口充电器为它供电
▪ 如果你是用的普通显示器而不是高清电视，你需要一条HDMI连接线与HDMI-DVI转换器。如果你的显示器支持HDMI或者你打算使用电视机进行连接，那么你就可以省去转换器了
▪ USB接口的键盘和鼠标
▪ 一条以太网线
可选设备：
• 用于保护Raspberry Pi的盒子
• 除了键盘和鼠标之外，如果你打算要连接更多的USB设备，那么你需要一个USB Hub集线器。
我们在本文中使用的东西：
▪ 一个Model B Raspberry Pi
▪ 一张16GB Class 10 SanDisk Ultra SDHC记忆卡（传输率标为300MB/s）
▪ 一个原本用于HTC Inspire的旧充电器
▪ 罗技K260无线键鼠套装（两件只用一个接收器，因此只占用一个USB口）
▪ 一条RJ-45网线
测试环境：Raspbian Wheezy release on original Model B.（部分连接Arduino内容可参考[http://letsmakerobots.com/node/36884](http://letsmakerobots.com/node/36884) ）
**login:  pi  raspberry**
SSH 登录，用secure crt  连接,登录成功~~~~
默认是pi   raspberry
1. sudo apt-get update (否则与opencv相关的libx11-dev下载不下来）
sudo apt-get install **synaptic**
**sudo synaptic**
**VNC远程**
**sudo apt-get install tightvncserver**
sudo vncserver :1 -geometry 1200x700 -depth 24
-pixelformat rgb565   //**800x600**
**key: *****1**
**开机自启动慎用，参考附录10及导致的诡异错误12.**
**tightvnc win7下远程，头一次忘写端口了，被PI积极的拒绝了。5901记住啊~**
Enter command: startx to start the GUI & click LXTerminal, enter:
**lsusb**(a list of USB devices is displayed, one of those lines listed the LifeCam)
**camorama**(command to start camorama, or command **cheese **to
 start cheese)
**sudo apt-get install camorama, **cheese
----------------------------
在synaptic搜索opencv , 下载libopencv-dev 即可。
或者 sudo apt-get install**libopencv-dev**
 python-opencv（可选）
----------------------------
参考：1. [https://github.com/andygrove/rasp_pi_camera_opencv](https://github.com/andygrove/rasp_pi_camera_opencv)
2.
[Lentin] didn’t feel like installing OpenCV from its source, a process that takes the better part of a day. Instead, he installed it using the synaptic
 package manager. After connecting a webcam, [Lentin] ssh’d into his Raspi and installed a face detection example script that comes with OpenCV.
It should be noted that [Lentin]‘s install of OpenCV isn’t exactly fast, but for a lot of projects being able to update a face tracker five times a second is more than enough. Once the Raspberry Pi camera module is released the speed of face detection on a
 Raspi should increase dramatically, though, leading to even more useful computer vision builds with the Raspberry Pi.
3. How to easily install OpenCV on Raspberry Pi
[http://www.technolabsz.com/2013/03/how-to-easily-install-opencv-on.html](http://www.technolabsz.com/2013/03/how-to-easily-install-opencv-on.html)
Hi all
Building opencv source code in Raspberry Pi will take about 6-7 hours.
Here is the link to install opencv from source
[Raspberry Pi+ OpenCV](http://mitchtech.net/raspberry-pi-opencv/) (注：参照官方网站linux环境中的安装方法）
**Easy method to install opencv**
1)Connect to Raspberry Pi using ssh. Fig shows an example of connecting Rpi using ssh
![](http://2.bp.blogspot.com/-_ywXflSM_gY/UTMNuGwYS_I/AAAAAAAABE4/d_G_fYmjy8k/s400/5.png)
![](http://3.bp.blogspot.com/-E7KGeaEmb-k/UTMNuLbf1gI/AAAAAAAABE8/PpMCnDxS1bE/s400/6.png)
2)Install synaptic package manager from command line of raspberry pi
![](http://3.bp.blogspot.com/-GA5pFnfWask/UTMN_xvzbsI/AAAAAAAABFI/Fixk8jMN7bg/s400/3.png)
3)Install tight-vnc server from command line
![](http://3.bp.blogspot.com/-4TlEZU4ctk8/UTMQDA0WE4I/AAAAAAAABFY/pIDzu6apv9Q/s400/28.png)
4)Start a vnc server in Rpi for viewing the screen and detection window from opencv
Enter the command to start a vnc-server on Rpi
**vncserver :1 -geometry 1200x700 -depth 24 -pixelformat rgb565**
**#New ‘X’ desktop is raspberrypi:1**
5)Start vnc client from PC
![](http://3.bp.blogspot.com/-_vmkupa-F2Y/UTMXAdF1ALI/AAAAAAAABF4/l9e3KvK28IY/s400/10.png)
eg: vncviewer 192.168.1.3:5901
eg:vncviewer rpi_ip:5901
6)Start synaptic from root terminal like this 
![](http://2.bp.blogspot.com/-gYVVE2syOSY/UTMWQe3d48I/AAAAAAAABFo/FspIrV4DSRM/s400/13.png)
7)Install opencv from synaptic like this
![](http://1.bp.blogspot.com/-f7VGhaHI9x8/UTMWflfRUcI/AAAAAAAABFw/gT_8cg2ItUY/s400/16.png)
8)Download the source code from below
[Download](http://www.mediafire.com/?ahd677sxyfpf1m7)
9)Copy files from PC to Rpi using **scp** command
**scp -r facedetect.py face.xml pi@192.168.1.3:"/home/pi"**
10)Run python file using following command
**python facedetect.py --cascade=face.xml 0**
Output
--------
![](http://4.bp.blogspot.com/-3StwPRzn2p8/UTMahdqv9ZI/AAAAAAAABGU/mcURwKxN1gU/s320/29.png)
Video:
4.**摄像头 Logitech C270，插上就可以用。**
如今的主流操作系统(如Windows XP SP2 and later, Linux 2.4.6 and later, MacOS 10.5 and later)都已提供UVC设备驱动，因此符合UVC规格的硬件设备在不需要安装任何的驱动程序下即可在主机中正常使用。使用UVC技术的包括摄像头、数码相机、类比影像转换器、电视棒及静态影像相机等设备。
最新的UVC版本为UVC 1.1，由USB Implementers Forum定义包括基本协议及负载格式。
网络摄像头是第一个支持UVC而且也是数量最多的UVC设备，目前，操作系统只要是 Windows XP SP2 之后的版本都可以支持 UVC，当然 Vista 就更不用说了。Linux系统自2.4以后的内核都支持了大量的设备驱动，其中支持UVC设备。
使用 UVC 的好处 USB 在 Video这块也成为一项标准了之后，硬件在各个程序之间彼此运行会更加顺利，而且也**省略了驱动程序安装这一环节。**
5. 测试opencv和摄像头
I
 recommend using C or C++ rather than python because it makes a HUGE difference in speed.
You can test your opencv installation and webcam by downloading and running
 the script below:
http://stevenhickson-code.googlecode.com/svn/trunk/AUI/Imaging/test
You may have to make this program executable by running:
chmod +x test
**The source code and makefile can be found here:http://stevenhickson-code.googlecode.com.sixxs.org/svn/trunk/AUI/Imaging/**
6. 不同xbmc的对比: 
# RaspBMC vs OpenELEC vs XBian: The Final XBMC Raspberry Pi Shootout
[http://www.gaducated.com/raspbmc-vs-openelec-vs-xbian-the-final-xbmc-raspberry-pi-shootout/](http://www.gaducated.com/raspbmc-vs-openelec-vs-xbian-the-final-xbmc-raspberry-pi-shootout/)
最终选用： RaspBMC
安装方法：
**wget http://svn.stmlabs.com/svn/raspbmc/testing/installers/python/install.pychmod +x install.pysudo python install.py但上述过程安装后启动不了了。。。慎用，再研究~~~**
You will then be greeted with a screen similar to the one below. You are now using the installer. Simply follow the instructions and you should be fine.
[http://www.raspbmc.com/wiki/user/os-x-linux-installation/](http://www.raspbmc.com/wiki/user/os-x-linux-installation/)
7.关机---------------------------------------------------------------------------------------------------------------------------------------------
raspberry pi关机很必要，在替换SD卡时常常强行断掉电源，不少人都遭遇了断电后SD卡系统无法启动的问题，这是因为tmp缓存系统也在SD卡上，可能造成文件损坏导致的，最好是使用**sudo shutdown -h now或sudo poweroff**之类的命令关机后再断电。关机后板子上还有一个红灯亮着，这是通电提示，下面三个绿色黄色LED才是工作指示灯。raspberry pi不支持WOL，没有power on 按钮，这就对开机造成了障碍，所以关机后要开机必须断电后再通电，通电自动启动，所以插线板支持开关就再好不过了
8.超频------------------------------------------------------------------------------------
另外还有超频问题，只需要配置config.txt文件即可，当然一定不要启用force_turbo=1，可能损坏保修资格，使用超频到900的Medium方案最合适，温度不高，速度提升也明显，不同的系统使用相同的超频频率配置运行一次raspi-config来超一次就行了，这样config.txt中的超频设置就自动建立好了，不同的系统不同的频率，就要如上面一样，建立多个config.txt，不同的系统启动不同的超频频率，这个不是很推荐改来改去，我认为对硬件本身伤害很深，使用一个固定的都流畅运行的频率即可。
以上的修改还可以在windows中进行，/boot分区默认是FAT格式，windows可以识别的，所以修改非常方便。高手可以自己配置更复杂的boot方式，比如不同的内核kernel.img，但bootcode.bin和loader.bin似乎不要改，否则就启动不了了。
9. 看门狗--------------------------------------
你还在担心树莓派死机吗？给他装个看门狗吧！[http://geekpi.cn/blog/2013/01/2173](http://geekpi.cn/blog/2013/01/2173)
什么是看门狗程序？
看门狗程序就是一个健康监控程序，每间隔一定时间（默认10秒)检查一次是否在设定的健康工作范围内，如果超过即启动硬件复位程序，让设备重新启动，恢复健康。
树莓派支持硬件自动复位？
是的，开源的BCM2708芯片让树莓派拥有了硬件看门狗芯片功能，加上linux内核编译好了的bcm2708_wdog模块，树莓派通过watchdog程序实现硬件看门狗功能，达到自动复位功能。
看门狗能做些什么？
比如CPU占用过高假死一定时间，系统严重报错无法响应，CPU温度过高…各种状态均可
接下来一步一步指导如何实现看门狗功能
首先让硬件的看门狗模块运行起来
```
```bash
modprobe
 bcm2708_wdog
```
```bash
vi
```
```bash
/etc/modules
```
```bash
#
 加入一行 "bcm2708_wdog"
```
```
就我所知从2012-05以后的内核都默认支持这个模块，如果报错，使用我的精简版系统，我测试过没问题
接下来安装看门狗守护进程，他的功能就是每间隔一定时间向看门狗硬件模块发送一个状态，如果失败，则触发硬件看门狗让树莓派重启
```
```bash
apt-get
```
```bash
install
```
```bash
watchdog
```
```bash
apt-get
```
```bash
install
```
```bash
chkconfig
```
```bash
#我的精简系统精简掉了chkconfig，手动安装一下就好
```
```bash
chkconfig
 watchdog on
```
```bash
#chkconfig是设定看门狗程序随系统启动自动运行
```
```
在开始运行watchdog之前，先配置一下这个程序
```
```bash
vi
```
```bash
/etc/watchdog
```
```bash
.conf
```
```bash
取消掉
 watchdog-device =
```
```bash
/dev/watchdog
```
```bash
前的注释
```
```bash
#号，让他监控的设备指向CPU的硬件看门狗
```
```bash
取消掉
 max-load-1 = 24 前的注释
```
```bash
#号，当1分钟load进程超过24个的时候（一般5个就是超高负载了，再高可以认为是死机，这在遭遇DDOS攻击的时候很容易遇到）就触发重启
```
```
还可以设置如温度到了多少度就重启，如 取消掉
temperature-device =
max-temperature = 120
前的注释#号，改为
temperature-device = /sys/class/thermal/thermal_zone0/temp
max-temperature = 75000
（温度一般不超过85度就不会损坏芯片，/sys/class/thermal/thermal_zone0/temp记录的是实时的温度，单位为千分之一摄氏度，所以75000就是75℃）
还可以设置内存耗尽就重启，如min-memory =1 前的注释#号去掉
还可以设置监控的间隔，如 interval = 1 前的注释#号去掉，该1为任意数字，单位是秒，默认是10秒一次健康检查
更多设置查阅watchdog文档
接下来我们让watchdog程序运行起来
```
```bash
/etc/init
```
```bash
.d
```
```bash
/watchdog
```
```bash
start
```
```
让我们来测试一下死机后会不会自动重启吧
![ddos](http://geekpi.cn/blog/wp-content/uploads/2013/01/ddos.jpg)
运行这一串字符会让系统内核立马崩溃，等等看，是不是10秒后他就自动重启了。
利用看门狗程序，结合raspberry pi 的CPU硬件看门狗模块，实现了raspberry pi永不死机。
---------------------------------------------------------------------------------------------------------------------------------------------------
10.   **开机自启动，慎用！！！**
安装VNC需要使用命令行。如果需要远程操作安装VNC，就必须通过SSH登录到命令行界面。请
安装
树莓派命令行：
- sudo apt-get install tightvncserver
开机自动启动
安装好之后请一定先使用此命令设置一个VNC密码：
- vncpasswd
复制代码
（先输入操作密码两次，然后会询问是否设置一个查看(view-only)密码，按自己喜欢，一般没必要。）
设置开机启动，需要在/etc/init.d/中创建一个文件。例如tightvncserver：
（注：启动脚本的名称，有和程序名一致的习惯）
- **sudo nano /etc/init.d/tightvncserver**
参考： [http://www.penguintutor.com/linux/tightvnc](http://www.penguintutor.com/linux/tightvnc)
内容如下：（putty窗口中按右键=粘贴）
> #!/bin/sh
### BEGIN INIT INFO
# Provides:          tightvncserver
# Required-Start:    $local_fs
# Required-Stop:     $local_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start/stop tightvncserver
### END INIT INFO
# More details see:
# [http://www.penguintutor.com/linux/tightvnc](http://www.penguintutor.com/linux/tightvnc)
### Customize this entry
# Set the USER variable to the name of the user to start tightvncserver under
export USER='pi'
### End customization required
eval cd ~$USER
case "$1" in
  start)
    # define resolution, console number or other parameters
    su $USER -c '/usr/bin/tightvncserver -geometry 1366x760 :1'
    echo "Starting TightVNC server for $USER "
    ;;
  stop)
    # the same console number with above
    su $USER -c '/usr/bin/tightvncserver -kill :1'
    echo "Tightvncserver stopped"
    ;;
  *)
    echo "Usage: /etc/init.d/tightvncserver {start|stop}"
    exit 1
    ;;
esac
exit 0
有可能导致： [http://www.raspberrypi.org/phpBB3/viewtopic.php?f=27&t=13568](http://www.raspberrypi.org/phpBB3/viewtopic.php?f=27&t=13568)
注：少数玩家默认用户不是pi的请自行更改USER变量
按Ctrl+X，回答Y（存盘）退出nano编辑器。
然后给tightvncserver文件加执行权限，并更新开机启动列表。
- **sudo chmod 755 /etc/init.d/tightvncserver**
- **sudo update-rc.d tightvncserver defaults**
附：手工启动与参数
以下用处不大，没兴趣请略过。
使用此命令手工启动VNC服务器程序：
- tightvncserver -geometry 800x600 :1
复制代码
如果首次启动，并且未曾使用vncpasswd命令设置密码，程序会要求设置一个。
开机启动很方便。如果没理由，真的不推荐手工启动。
命令行参数说明：
一、:1，指定控制台的号码。
启动多个控制台，可以提供互不影响的多个桌面环境。（大多数人不用多用户操作所以没意义）
可以不加此参数，tightvncserver会自动寻找从1开始的下一个空闲控制台。
加上此参数，会强制使用指定的控制台，如果此控制台已经启动则报错。加此参数可有效防止无意多次启动程序（会启动多个控制台）白白浪费系统资源。
特殊的0号控制台
0号控制台就是连接真实显示器真正输出图像的那个桌面。
对于VNC客户端，不输入端口号登录，默认就登录到0号控制台，方便。
但是因为0号是真正的桌面，所以和开机启动桌面环境，或者自己用startx命令，都存在啰嗦的冲突。
到头来是个麻烦。因此自动启动的配置教程中，一律使用1号控制台。
二、-geometry 800x600，分辨率。可以不加。
终止VNC控制台：
- tightvncserver -kill :1
复制代码
查看正在运行的控制台列表：
- ps ax | grep Xtightvnc | grep -v grep
复制代码
------------------------------------------------
11.
|你想要远程登录到你的 Raspberry Pi 然后使用图形桌面吗？这其实很简单的。假如你不想专门为 Pi 配一台显示器的话，这里有你想要的解决方案。OK，这是另外一篇 Raspberry Pi 的文章。由于回复的数量巨大，我将在研究的同事在这里发布更新的新。我希望这能吸引更多人使用这个产品。这篇文章是针对初学者的，所以请不要说这篇文章的内容太浅。||
|假如你和我一样，不想连接各种线或者将 Raspberry Pi 接到电视上，我的方法将让你在笔记本或者台式机上拥有一个 Pi 的图形桌面，如图：![](http://static.oschina.net/uploads/img/201212/30085051_MzWV.png)|[](http://my.oschina.net/zhlmmc)|
|### 配置 Pi确定你的 Pi 已经连到网络，网络中还有其他的电脑。为了设置联网，你需要在 Pi 上运行一个服务以便其他电脑连接进来。我这里使用的是 [Raspian Wheezy](http://www.raspberrypi.org/downloads) ，我的大多数教程都是用的这个。|[](http://my.oschina.net/javayou)|
|我选择用 [xrdp](http://www.xrdp.org/) 来实现，我之前已经使用 xrdp 多时。要在 Raspian 中安装 xrdp 只需要输入以下命令：> sudo apt-get install xrdp这个命令将为你安装在 Pi 上使用 xrdp 的所有软件包，而且将在你启动 Raspberry Pi 时自动启动 xrdp。现在获取你的 IP 地址：> ifconfig eth0 | grep inet | cut -c21-30记住这个 IP 地址，接下来你需要用这个 IP 来连接你的 Pi。||
|### 使用 Windows 连接到 Pi你可以在不同的机器来连接 Pi，我们先从 Windows 系统开始。只需要简单的从开始菜单进入：开始 -> 程序 -> 远程桌面连接然后输入 Pi 的 IP 地址：![](http://static.oschina.net/uploads/img/201212/30085051_HoA9.png)点击“连接”按钮，你将看到一个消息提醒说远程计算机没有被验证，如下图所示：![](http://static.oschina.net/uploads/img/201212/30085051_3jO0.png)你可以直接忽略这个提示信息，因为你的 Pi 运行了 Linux 而不是 Windows ，所以有此提示。||
|你将看到如下图所示的截图：![](http://static.oschina.net/uploads/img/201212/30085051_aGSW.png)||
|### 在 OS X 上连接到 Pi我自己并没有 Mac 来做这个测试，但你可以找到例如 [CoRD](http://cord.sourceforge.net/) 这样的工具来连接。我争取在最近找一台 Mac 机器来试试。||
|### 额外的选项也许你已经发现，Pi 可以支持很高的屏幕分辨率，很有可能比你屏幕支持的最高分辨率还要高。这有可能造成你需要滚动条来查看屏幕的部分区域。![](http://static.oschina.net/uploads/img/201212/30085051_nBvI.png)这就很不方便了。然而在 Windows 上你可以比较容易的通过远程桌面设置来调整屏幕的分辨率。点击“Options”下拉按钮：![](http://static.oschina.net/uploads/img/201212/30085051_WW9t.png)选择 “Display” 设置页:![](http://static.oschina.net/uploads/img/201212/30085052_nv1a.png)||
|你将看到一个滑动条用来设置分辨率和颜色。你可以在“General”设置页点击“Save”按钮来保存你的设置。在 Linux 中，你可以通过以下命令来设置分辨率：|`1`|```rdesktop -r 1024x768 < the IP address of your Pi >```|rdesktop 会自动调整到你设置的分辨率。|`1`|```rdesktop -r 1024x768 < the IP address of your Pi >```|[](http://my.oschina.net/zhlmmc)|
|----|----|----|----|
|`1`|```rdesktop -r 1024x768 < the IP address of your Pi >```| | |
rdesktop 会自动调整到你设置的分辨率。
[](http://my.oschina.net/zhlmmc)
|### 为什么使用RDP？有人可能会这么问，也是个好问题。如果你正使用Linux，并且想连接到另一台Linux机器，为什么要使用微软的协议呢？我建议使用xrdp是因为它很快、很稳定，并且跨操作系统。当然你也可以选择VNC，但出于简单考虑，我选择RDP，而且它在Windows系统和Linux系统上表现都很好。|[](http://my.oschina.net/monster)|
|### 总结希望这篇文章能回答你关于“将 Pi 作为虚拟桌面”的问题。如果你像我一样,你可能也会将你的 Pi 藏匿在某个地方，使用起来更方便。而我会将它藏在我的“数据中心”里，当我想玩的时候，直接通过 SSH 或者 RDP 就可以进行控制。|[](http://my.oschina.net/javayou)|
本文中的所有译文仅用于学习和交流目的，转载请务必注明文章译者、出处、和本文链接
我们的翻译工作遵照 [CC 协议](http://zh.wikipedia.org/wiki/Wikipedia:CC)，如果我们的工作有侵犯到您的权益，请及时联系我们
----------------------------------------------
12. 诡异错误： [http://www.raspberrypi.org/phpBB3/viewtopic.php?f=28&t=28521](http://www.raspberrypi.org/phpBB3/viewtopic.php?f=28&t=28521)
I have the same problem. Upon the initial boot, I selected and set several options, including
 booting up directly into the GUI. Now, when the system starts up, I get the usual Linux wall-o-text, and then it goes to a GUI login screen. There is a small graphic in the center of the screen with a picture of a terminal and the words "raspberrypi" under
 it. There is a login window, and a drop down menu with options for "Default Xsession", "LXDE", and "Openbox". I no longer have an opportunity to get to a command line when I boot up. I am also using the Hungrypi distro.
Hi all,
For all those who cannot ssh or cannot get a shell upfront**try typingctrl+alt+F1**(or F2 or F3). when doing this the shell opened for me and i could login with user "pi" and password "raspberry" if not changed. 
Got this from here: [viewtopic.php?f=65&t=30621&p=266444&hilit=default+xsession#p266444](http://www.raspberrypi.org/phpBB3/viewtopic.php?f=65&t=30621&p=266444&hilit=default+xsession#p266444) (finally
 my french is paying off)
when logged on you can probably start the GUI by typing "startx", worked for me at least.
紧接着有一个问题： 
# [timeout in locking authority file /home/pi/.Xauthority ”](http://raspberrypi.stackexchange.com/questions/1734/please-help-startx-fails-hangs-finally-returns-error-message-xauth-timeout-i)
One possible source of this problem can be file ownership. You could try running following command:
```
** sudo chown -R pi:pi /home/pi**
```
This should change ownership of all files in pi directory (and all directories inside of it) back to the user pi and group pi.
xauth application has a commandline option `-b` which
 is intended to clean stale locks if they exists so you could also try running (when logged in as user pi):
```
** xauth -b**
```
.Xauthority file is not needed when X session is not running so you could safely remove it and it will be recreated next time X is started. But the message does not seem to suggest the file is corrupted so I don't think this can solve your issue.
13.---------------------------------------------------------------------------------------
## [Ubuntu 系统 Update-rc.d 命令](http://wangyan.org/blog/ubuntu-update-rc-d.html)
Ubuntu或者Debian系统中update-rc.d命令，是用来更新系统启动项的脚本。这些脚本的链接位于/etc/rcN.d/目录，对应脚本位于/etc/init.d/目录。在了解update-rc.d命令之前，你需要知道的是有关Linux 系统主要启动步骤，以及Ubuntu中运行级别的知识。
### 一、Linux 系统主要启动步骤
- 
读取 MBR 的信息，启动 Boot Manager。
- 
加载系统内核，启动 init 进程， init 进程是 Linux 的根进程，所有的系统进程都是它的子进程。
- 
init 进程读取 /etc/inittab 文件中的信息，并进入预设的运行级别。通常情况下 /etc/rcS.d/ 目录下的启动脚本首先被执行，然后是/etc/rcN.d/ 目录。
- 
根据 /etc/rcS.d/ 文件夹中对应的脚本启动 Xwindow 服务器 xorg，Xwindow 为 Linux 下的图形用户界面系统。
- 
启动登录管理器，等待用户登录。
### 二、运行级别
#### Ubuntu中的运行级别
- 
0（关闭系统）
- 
1（单用户模式，只允许root用户对系统进行维护。）
- 
2 到 5（多用户模式，其中3为字符界面，5为图形界面。）
- 
6（重启系统）
#### 切换运行级别
|12|init **[**0123456Ss**]**例如：init 0 命令关机； init 6 命令重新启动|
#### 启动项管理工具
|12|**sudo****install** sysv-rc-conf //或者使用带gui的工具bum**sudo** sysv-rc-conf|
### 三、update-rc.d命令详解
#### 从所有的运行级别中删除指定启动项
|1|update-rc.d -f ＜**basename**＞ remove|
#### 按指定顺序、在指定运行级别中启动或关闭
|1|update-rc.d ＜**basename**＞ start|stop ＜order＞ ＜runlevels＞|
|1|实例：update-rc.d apachectl start 202345 . stop 20016 .|
解析：表示在2、3、4、5这五个运行级别中，由小到大，第20个开始运行apachectl；在 0 1 6这3个运行级别中，第20个关闭apachectl。这是合并起来的写法，注意它有2个点号，效果等于下面方法：
|1|update-rc.d apachectl defaults|
#### A启动后B才能启动，B关闭后A才关闭
|12|update-rc.d A defaults 8020update-rc.d B defaults 9010|
#### 启动和关闭顺序为90，级别默认
|1|update-rc.d ＜**basename**＞ defaults 90|
原文地址 : [http://wangyan.org/blog/ubuntu-update-rc-d.html](http://wangyan.org/blog/ubuntu-update-rc-d.html)
本站遵循 : [知识共享署名-非商业性使用-相同方式共享 3.0 版权协议](http://creativecommons.org/licenses/by-nc-sa/3.0/deed.zh)
版权声明 : 原创文章转载时，请务必以超链接形式标明 [文章原始出处](http://wangyan.org/blog/ubuntu-update-rc-d.html)
