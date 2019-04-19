# Ubuntu默认不进入图形界面 - maopig的专栏 - CSDN博客
2017年03月24日 11:10:57[maopig](https://me.csdn.net/maopig)阅读数：6061
修改 /etc/X11/default-display-manager
如果值为/usr/sbin/gdm，(ubuntu12.04 为/usr/sbin/lightdm)则进入图形界面 如果值为false，则进入控制台（命令行方式）。
如果想从控制台进入图形界面，可以在控制台上输入命令 sudo gdm或sudo startx
如果想从图形界面进入控制台，可以在终端上输入命令 sudo init 1，不过好像不好用
插入一个小知识：
XDM即“X Display Manager”，由它来启动X Window服务器，并管理图形客户端程序的
登录、会话、启动窗口管理器（KDE、Gnome）等。KDE和Gnome也提供了自己的xdm的实现，分别叫kdm和gdm。
编辑grub引导菜单，按下面的方法做
命令：
sudo gedit /etc/default/grub
找到这一行
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
改成
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash text"
在输入命令：
sudo update-grub
开机后就自动进入tty1了。
按CTRL+ALT+F1~6可以从桌面环境切换到字符界面
按CTRL+ALT+F7可以从字符界面环境切换到桌面环境。 
