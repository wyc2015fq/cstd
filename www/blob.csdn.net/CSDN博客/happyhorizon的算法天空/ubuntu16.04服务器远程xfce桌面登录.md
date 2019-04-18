# ubuntu16.04服务器远程xfce桌面登录 - happyhorizon的算法天空 - CSDN博客
2019年02月04日 01:10:24[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：1895
### 文章目录
- [安装所需的工具](#_1)
- [xrdp](#xrdp_2)
- [vnc4server](#vnc4server_6)
- [xfce4](#xfce4_10)
- [配置vnc](#vnc_16)
- [启动vncserver并设置密码](#vncserver_18)
- [root用户或者sudo权限的用户](#rootsudo_27)
- [设置xstartup文件](#xstartup_28)
- [创建VNC服务](#VNC_43)
- [远程桌面效果](#_89)
- [非root用户](#root_92)
- [xsession配置](#xsession_93)
- [xfce4配置](#xfce4_96)
- [配置xstartup文件](#xstartup_102)
- [重启xrdp](#xrdp_122)
- [远程桌面效果](#_127)
- [参考：](#_131)
# 安装所需的工具
## xrdp
```
sudo apt-get install xrdp
```
## vnc4server
```
sudo apt-get install xrdp vnc4server
```
## xfce4
```
sudo apt-get update
sudo apt-get install xfce4 xfce4-goodies
```
# 配置vnc
## 启动vncserver并设置密码
首先进入root账户，启动vncserver。第一次启动一般都需要输入密码，对于root用户，密码文件将保存在/root/.vnc/passwd下，对于sudo权限的用户，密码文件保存在家目录/.vnc/passwd。如果忘记密码， 可以删除这个文件，再次运行vncserver设置密码。
设置好密码后，vncserver启动，在端口5901启动一个默认的服务器实例。5901端口被称为显示器端口，并通过VNC所提到的:1。 VNC可以在其他显示器端口启动多个实例，如:2，:3，等等。
当使用VNC服务器时，请记住:X是指显示器端口5900+X。
在.vnc文件夹中，还有一个非常重要的脚本： xstartup。这个脚本可以设置vnc远程桌面的基本选项。
## root用户或者sudo权限的用户
### 设置xstartup文件
打开该脚本，将原文件注释掉，添加下面两行：
```
#!/bin/bash 
xrdb $HOME/.Xresources 
startxfce4 &
```
- 第一个命令: `xrdb $HOME/.Xresources`，讲述了VNC的GUI框架读取服务器用户.Xresources文件。.Xresources是用户可以更改图形化桌面的某些设置，如终端的颜色，光标主题，和字体呈现。
- 第二个命令简单地告诉服务器启动Xfce，在这里你会找到所有的图形软件，你需要舒适地管理你的服务器。
修改xstartup脚本后，保存退出。关闭当前vncserver实例：
`vncserver -kill :1`
之后再重新启动，更新vncserver设置：
`vncserver :1`
### 创建VNC服务
接下来，我们将VNC服务器设置为systemd服务。 这将使其可以像任何其他systemd服务一样根据需要启动，停止和重新启动。
首先，创建`/etc/systemd/system/vncserver@.service`：
```
sudo gedit /etc/systemd/system/vncserver@.service
```
将以下内容复制并粘贴到其中。 一定要改变PIDFile的值用户的价值和用户名，以配合您的用户名。
```
/etc/systemd/system/vncserver@.service 
[Unit]
Description=Start TightVNC server at startup
After=syslog.target network.target
[Service]
Type=forking
User=dell
PAMName=login
PIDFile=/home/dell/.vnc/%H:%i.pid
ExecStartPre=-/usr/bin/vncserver -kill :%i > /dev/null 2>&1
ExecStart=/usr/bin/vncserver -depth 24 -geometry 1280x800 :%i
ExecStop=/usr/bin/vncserver -kill :%i
[Install]
WantedBy=multi-user.target
```
保存并关闭文件。
接下来，让系统知道新的单元文件。
```
sudo systemctl daemon-reload
```
启用service：`sudo systemctl enable vncserver@1.service`
查看vncserver状态：`sudo systemctl status vncserver@1`
如果它正确启动，输出应如下所示：
```
vncserver@1.service - TightVNC server on Ubuntu 16.04
   Loaded: loaded (/etc/systemd/system/vncserver@.service; enabled; vendor preset: enabled)
   Active: active (running) since Mon 2016-04-25 03:21:34 EDT; 6s ago
  Process: 2924 ExecStop=/usr/bin/vncserver -kill :%i (code=exited, status=0/SUCCESS)
...
 systemd[1]: Starting TightVNC server on Ubuntu 16.04...
 systemd[2938]: pam_unix(login:session): session opened for user finid by (uid=0)
 systemd[2949]: pam_unix(login:session): session opened for user finid by (uid=0)
 systemd[1]: Started TightVNC server on Ubuntu 16.04.
```
### 远程桌面效果
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190204010744258.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
## 非root用户
### xsession配置
对于非root用户登录服务器的情况，应该首先将xfce4-session写入该用户文件夹的xsession文件：/home/username/.xsession
`echo "xfce4-session" >~/.xsession`
### xfce4配置
之后继续配置xfce4
```
sudo vi /etc/xrdp/startwm.sh
```
在`. /etc/X11/Xsession`前一行插入`xfce4-session`
### 配置xstartup文件
设置非root用户的xstartup文件如下：
```
#!/bin/sh
# Uncomment the following two lines for normal desktop:
 unset SESSION_MANAGER
 unset DBUS_SESSION_BUS_ADDRESS
 [ -x /etc/vnc/xstartup ] && exec /etc/vnc/xstartup
 [ -r $HOME/.Xresources ] && xrdb $HOME/.Xresources
 xsetroot -solid grey
 x-terminal-emulator -geometry 1920*1600 -ls -title "$VNCDESKTOP Desktop" &
# xfce4-session  &
 xfdesktop &
 xfsettingsd &
 xfce4-menu-plugin &
 xfwm4 &
```
### 重启xrdp
```
sudo service xrdp restart
```
### 远程桌面效果
非root用户：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190214095618366.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
# 参考：
[https://www.cnblogs.com/hcbin/p/8023756.html](https://www.cnblogs.com/hcbin/p/8023756.html)
[https://blog.csdn.net/zz_1215/article/details/77921405](https://blog.csdn.net/zz_1215/article/details/77921405)
unbutu16.04配置xfce桌面：[https://www.linuxprobe.com/ubuntu-vnc-xfce4.html](https://www.linuxprobe.com/ubuntu-vnc-xfce4.html)
