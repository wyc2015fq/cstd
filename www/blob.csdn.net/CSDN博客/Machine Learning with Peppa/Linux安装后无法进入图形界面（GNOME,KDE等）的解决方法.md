# Linux安装后无法进入图形界面（GNOME,KDE等）的解决方法 - Machine Learning with Peppa - CSDN博客





2018年03月27日 20:47:17[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：20498
所属专栏：[Linux私房菜](https://blog.csdn.net/column/details/20153.html)









之前一直用level3的命令行在学习Linux，今天觉得有必要把GUI的操作熟悉一下，因此试着切换到图形界面，试了很多方法都没成功。然后我突然意识到我安装的是自定义minimal版本，所以，没有给我安装什么XWINDOWS、GNOME这类东西的。所以只好自己先下载然后再调用咯。




过程如下：

1.在命令行下输入下面的命令来安装 Gnome 包


yum groupinstall "GNOME Desktop" "Graphical Administration Tools"
2. 更新系统的运行级别

如果你想在系统下次启动的时候自动进入图形界面，那么我们需要更改系统的运行级别，输入下面的命令来启用图形界面。

ln -sf /lib/systemd/system/runlevel5.target /etc/systemd/system/default.target

还有一种方法将GNOME设为默认的开机启动界面（不用ctrl+alt+F1）
    1. 进入cenos7的命令行模式
    2. 终端输入“init 3”回车进入命令行模式




![](https://img-blog.csdn.net/20170704175324448?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjMwMTQ0MzU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3. 登录成功后systemctl get-default 获取当前系统启动模式

![](https://img-blog.csdn.net/20170704180003313?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjMwMTQ0MzU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

查看配置文件
# cat /etc/inittab


![](https://img-blog.csdn.net/20170704180424547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjMwMTQ0MzU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

通过以上显示，目前系统为命令行模式
更改模式命令：systemctl set-default graphical.target由命令行模式更改为图形界面模式
                         systemctl set-default multi-user.target由图形界面模式更改为命令行模式
更改后验证是否正确：reboot；界面如下
![](https://img-blog.csdn.net/2018032720433861?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



