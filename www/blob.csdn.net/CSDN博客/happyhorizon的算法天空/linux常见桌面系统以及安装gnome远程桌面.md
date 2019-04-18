# linux常见桌面系统以及安装gnome远程桌面 - happyhorizon的算法天空 - CSDN博客
2019年02月27日 16:50:51[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：79
# x window
X window 又称为X窗口系统，简称为X11或X。1984年由MIT的Bob Scheifler和Bob Scheifler开发设计。x window是Linux及其他unix系统的标准GUI，x window提供了基本图形化框架，在输入方面负责鼠标、键盘与操作系统的互动，在输出方面负责在显示器上显示可视化的图像和窗口。
x window遵循客户端/服务器架构，系统中由x服务器和多个x客户端进行通信，客户端和服务器是相对独立的。
Linux操作系统的桌面环境非常丰富，例如gnome，xfce，kde等等。
必须一提的是桌面环境和窗口管理器是不同的概念，窗口管理器是用来控制和管理窗口外观和现实方式的软件，桌面环境包括了窗口管理器，也包括一些典型的应用程序，例如gnome使用metacity窗口管理器，以及GIMP，gedit， bashee等应用程序， xfce使用xfwm作为窗口管理器。
# unity桌面
unity的特色是可用于触摸屏环境，是canonical开发，是ubuntu10.10-16.10的桌面环境。unity桌面本质上是文件系统中的一个目录，在/home/username/Desktop。
刚才提到，unity桌面的特点是支持触屏，这个功能就是自带的uTouch提供的，支持多点触摸。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190305092944765.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
菜单栏图标对应的功能如下:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227164551154.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
Unity较GNOME 3或KDE 占用系统资源较多。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190305093041501.png)
# KDE
KDE 有类windows的“开始菜单”。界面华丽。
许多发行版本如OpenSUSE、PCLinuxOS和 Mandriva等皆采用KDE作为默认桌面环境。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190305093201948.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190305093250618.png)
# XFCE
占用资源较GNOME、KDE较少。适合老机器，轻量级桌面。与windows界面环境类似。许多不习惯GNOME 3 ，Unity新桌面的同学，很多选择了XFCE 4.8，包括Linus大神同学。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190305093233314.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190305093300425.png)
# gnome 桌面
gnome桌面是主流Linux发行版本的默认桌面，主张简约易用，“够用即可”。 Unity桌面就是gnome桌面基础上修改而成的。
gnome的全称为GNU网络对象模型环境（The GNU Network Object Model Environment）, 诞生于1997年。gnome基于GTK+图形库，使用C语言开发，早先使用metacity作为窗口管理器，2011年gnome3发布后桌面管理器升级为mutter。
[gnome的官方网站是gnome.org](http://xn--gnomegnome-ko3sm68g3ibl80kmun70q.org)，ubuntu中安装可以用命令`sudo apt-get install gnome`
Nautilus是gnome的文件管理器工具，用于创建，显示，查找文件夹和文档，快速启动应用程序，定制文件图标等。
## gnome 2
gnome 2.x 作为大家熟识的传统界面，默认2个面板。在系统资源占用方面，gnome 2.x 较gnome 3 要求 RAM 较少、CPU较高。在这两个方面：
xfce ，lxde < gnome 2.x
在目前发行版本中，LInux Mint，Debian等仍使用GNOME 2.x作为默认桌面环境。
![gnome 2 ](https://img-blog.csdnimg.cn/20190305092553728.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190305092834316.png)
## gnome 3
GNOME 3作为GNOME 新版本，较GNOME 2.x变革明显.GNOME 3相对直观，只有一个顶部面板，传统菜单不复在。可在左上角点击“活动”查看应用、工作分区等。
目前，Fedora 15等发行版本采用GNOME 3 作为默认桌面环境。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190305092659536.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190305092822703.png)
# 安装远程gnome桌面
## 安装gnome桌面
```
sudo apt-get install gnome-session-flashback
sudo apt install ubuntu-desktop gnome-panel gnome-settings-daemon metacity nautilus gnome-terminal -y
```
## 安装tigervnc
Vnc服务器工具有很多，例如tightvnc，vnc4server，tigervnc，realvnc。这里选择tigervnc作为vnc服务器。
tigervnc： 包含server控制端用于实现vnc服务，其中包含一个名为 X0VNC 的特殊服务，该服务运行后可以把当前 X11 桌面会话远程传输给远端客户端让其操控，而不是传统 VNC 的虚拟会话桌面模式。 x11vnc和tigervnc很类似，但是配置使用起来不如 X0VNC 方便。在Fedora和CentOS等linux系统中，安装tigervnc非常方便，例如在Fedora中安装只需要命令：
```
sudo dnf install tigervnc-server
```
但是在ubuntu中就略微麻烦一些，需要下载源码编译安装。下载地址如下：
[https://download.csdn.net/download/happyhorizion/10979193](https://download.csdn.net/download/happyhorizion/10979193)
```
sudo dpkg -i tigervncserver_1.7.0-1ubuntu1_amd64.deb
```
也可以用源代码编译安装最新版的tigervncserver， 下载地址如下：
[https://download.csdn.net/download/happyhorizion/10979176](https://download.csdn.net/download/happyhorizion/10979176)
安装好tigervncserver之后，配置用户名和密码，参见：
[https://blog.csdn.net/happyhorizion/article/details/86762118](https://blog.csdn.net/happyhorizion/article/details/86762118)
## 配置gnome远程桌面
切换到root用户，设置/root/.vnc/xstartup，将其修改为：
```
#!/bin/sh
# unset SESSION_MANAGER
# unset DBUS_SESSION_BUS_ADDRESS
 [ -x /etc/vnc/xstartup ] && exec /etc/vnc/xstartup
 [ -r $HOME/.Xresources ] && xrdb $HOME/.Xresources
 xsetroot -solid grey
 vncconfig -iconic &
 x-terminal-emulator -geometry 1980*1800 -ls -title "$VNCDESKTOP Desktop" &
 x-window-manager &
 gnome-panel &
 gnome-settings-daemon &
 metacity &
 nautilus &
```
其中gnome-panel 面板，nautilus 文件管理器/桌面 metacity 窗口管理器。设置后，重新启动`vncserver`，启用gnome桌面。
特别棒的linux系统桌面环境及性能比较总结博客: [https://www.cnblogs.com/chenmingjun/p/8506995.html](https://www.cnblogs.com/chenmingjun/p/8506995.html)
