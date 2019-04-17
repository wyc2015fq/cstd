# ubuntu14.04 配置VNC服务，亲测可用 - 战斗蜗牛的专栏 - CSDN博客





2016年08月05日 15:41:17[vbskj](https://me.csdn.net/vbskj)阅读数：10004








1.更新系统 apt-get update -y apt-get upgrade -y 






2. 安装gnome环境相关，sudo apt-get install ubuntu-desktop gnome-panel gnome-settings-daemon metacity nautilus gnome-terminal





3. 安装vncserver, sudo apt-get install vnc4server


4.  给当前用户设置vnc登录密码：
 vncpasswd   





5. 启动vncserver，生成.vnc文件和xstartup文件


6.  修改.vnc/xstartup文件


#!/bin/sh



# Uncomment the following two lines for normal desktop:

# unset SESSION_MANAGER

# exec /etc/X11/xinit/xinitrc



#[ -x /etc/vnc/xstartup ] && exec /etc/vnc/xstartup

#[ -r $HOME/.Xresources ] && xrdb $HOME/.Xresources

#xsetroot -solid grey

#vncconfig -iconic &

#x-terminal-emulator -geometry 80x24+10+10 -ls -title "$VNCDESKTOP Desktop" &

#x-window-manager &

#gnome-session-fallback &

export XKL_XMODMAP_DISABLE=1

unset SESSION_MANAGER

unset DBUS_SESSION_BUS_ADDRESS



xsetroot -solid grey

vncconfig -iconic &

gnome-session &


gnome-panel &

gnome-settings-gaemon &

metacity &

nautilus &

gnome-terminal &



7. 配置完毕后，启动vncserver


    vncserver -geometry 1280x1024


启动后，记住vncserver的端口号





=========windows下进行vnc访问


1. 下载vncviewer客户端，输入Ip: 192.168.1.100:1(冒号后面的是7中生成的端口号)


2. 输入密码即可





=========Linux下进行vnc访问



1、安装VNC客户程序：
sudo apt-get install vnc4-common xvnc4viewer 



2、 然后你就通过Terminal Server Client登录了：Applications -> Internet -> Terminal Server Client；Computer为：服务器IP:display#，例如：192.168.10.36:1；protocol选择：VNC。 

或者命令行：
vncviewer 192.168.2.103:1











