# oracle linux配置X manager和VNC-Linux - ljx0305的专栏 - CSDN博客
2009年06月19日 18:06:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1015标签：[manager																[oracle																[linux																[security																[file																[session](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=security&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=manager&t=blog)
今天在oracle linux上配置了一下X manager 和vnc，是第一次配置vnc，vnc配置要比X manager简单许多这里记录一下：
----  X manager 配置：
1.修改GDM的配置文档，/etc/X11/gdm/gdm.conf，将[xdmcp]中的enable选项配置为true 
[xdmcp]
# Distributions: Ship with this off. It is never a safe thing to leave
# out on the net. Setting up /etc/hosts.allow and /etc/hosts.deny to only
# allow local access is another alternative but not the safest.
# Firewalling port 177 is the safest if you wish to have xdmcp on.
# Read the manual for more notes on the security of XDMCP.
Enable=true
用vi编辑gdm.conf 进入后键入/xdmcp查找字符串xdmcp "n"是搜索下一句，这样就能快速的定位到xdmcp项
2.修改/etc/X11/xdm/xdm-config文档，将DisplayManager.requestPort: 0注释掉
! SECURITY: do not listen for XDMCP or Chooser requests
! Comment out this line if you want to manage X terminals with xdm
!DisplayManager.requestPort: 0
3.修改/etc/X11/xdm/Xaccess文档 #* #any host can get a login window 去掉此行首位的#
# right hand sides can match.
#
* #any host can get a login window
#
# To hardwire a specific terminal to a specific host, you can
4. 假如有防火墙，请打开177的端口
ok！配置完后用X manager进入吧
----VNC 配置
1.检查rpm包是否安装
[oracle@niuge ~]$ rpm -qa|grep vnc
vnc-server-4.0-11.el4
vnc-4.0-11.el4
只要有第一个vnc-server-4.0-11.el这个就足够，这是服务端运行程式。如未安装，请安装
2.安装好后运行 vncserver
[test@niuge ~]$ vncserver
You will require a password to access your desktops.
Password: 
Verify: 
xauth: creating new authority file /home/test/.Xauthority
New ’niuge:3 (test)’ desktop is niuge:3
Creating default startup script /home/test/.vnc/xstartup
Starting applications specified in /home/test/.vnc/xstartup
Log file is /home/test/.vnc/niuge:3.log
会提示您输入登录密码，同时会给您提示，记住这个":3" 这是您vnc客户端登录时候要用到的，我机器已开启了2个，所以这个是:3
3.在windows机器上装好vnc客户端，装好后运行客户端，输入方式如下图所示
![](http://blogimg.chinaunix.net/blog/upfile2/080115202946.jpg)
点击OK后提示输入您刚才配置的密码。点击后就会进入
![](http://blogimg.chinaunix.net/blog/upfile2/080115203248.jpg)
ok!是不是觉得命令行方式不爽，别着急，修改一下配置就能够进入windows模式
4.刚才在开启vncserver时候有如下提示：
Creating default startup script /home/test/.vnc/xstartup
Starting applications specified in /home/test/.vnc/xstartup
Log file is /home/test/.vnc/niuge:3.log
我们来看看xstartup内容：
#!/bin/sh
# Uncomment the following two lines for normal desktop:
# unset SESSION_MANAGER
# exec /etc/X11/xinit/xinitrc
[ -x /etc/vnc/xstartup ] && exec /etc/vnc/xstartup
[ -r $HOME/.Xresources ] && xrdb $HOME/.Xresources
xsetroot -solid grey
vncconfig -iconic &
xterm -geometry 80x24+10+10 -ls -title "$VNCDESKTOP Desktop" &
twm &
ok！看说明很容易就知道怎样正常进入desktop了
# Uncomment the following two lines for normal desktop:
unset SESSION_MANAGER
exec /etc/X11/xinit/xinitrc
修改后保存退出
先停止vncserver：
[test@niuge ~]$ vncserver -kill :3
Killing Xvnc process ID 30475
[test@niuge ~]$ 
重启vncserver
[test@niuge ~]$ vncserver
New ’niuge:3 (test)’ desktop is niuge:3
Starting applications specified in /home/test/.vnc/xstartup
Log file is /home/test/.vnc/niuge:3.log
OK!让我们重复上边的步骤重新登录vnc
![](http://blogimg.chinaunix.net/blog/upfile2/080115204449.jpg)
熟悉的画面出现了。
---EOF---
