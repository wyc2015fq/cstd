# 在Ubuntu上安装Android-SDK的方法 - YZXnuaa的博客 - CSDN博客
2018年04月20日 16:00:49[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：3235
个人分类：[ubuntu操作](https://blog.csdn.net/YZXnuaa/article/category/7388698)
# 一、安装和配置Ubuntu系统
### 1. 安装Ubuntu Desktop 14.04 x86_64
### 2. 启用root账户
Ubuntu 14.04默认是不允许root账户登录的，在登录窗口只能看到普通用户和访客登录。在shell中运行以下命令即可启用root账户登录：
Step-1. 切换至root账户权限模式，需要输入当前普通用户的登录密码
```bash
sudo -s
```
Step-2. 启用root账户登录
`vi /usr/share/lightdm/lightdm.conf.d/50-ubuntu.conf`
在上述文件中添加如下配置：
```
greeter-show-manual-login=true
```
Step-3. 设置root账户的登录密码，根据提示输入密码，我此处将密码指定为`password`
`passwd root`
step-4. 设置stdin
`vi /root/.profile`
在上述文件中找到：
```
mesg n
```
将其修改为：
```bash
tty -s && mesg n
```
Step-5. 重启系统，然后以root账户登录
`reboot`
### 3. 关闭系统防火墙
```bash
ufw disable
```
### 4. 为root账户启用ssh登录
Step-1. 安装ssh服务端
```
apt-get update
apt-get install openssh-server
```
Step-2. 配置ssh服务
`vi /etc/ssh/sshd_config`
在上述文件中找到：
```
PermitRootLogin without-password
```
将其修改为：
```
# PermitRootLogin without-password
```
然后，在上述文件添加如下配置：
```
PermitRootLogin yes
```
Step-3 重启系统，然后以root账户登录
reboot
### 5. 配置静态IP地址
`vi /etc/network/interfaces`
将这个文件的内容修改为：
```
auto lo
iface lo inet loopback
# The primary network interface
auto eth0
iface eth0 inet static
address 192.168.1.119
netmask 255.255.255.0
gateway 192.168.1.1
```
### 6. 配置DNS
`vi /etc/resolvconf/resolv.conf.d/base`
将这个文件的内容修改为：
```
nameserver 114.114.114.114
nameserver 221.6.4.66
nameserver 221.6.4.67
```
### 7. 重启系统
`reboot`
# 二、安装VNC
### 1. 安装gnome相关组件
`apt-get install gnome-panel gnome-settings-daemon metacity nautilus gnome-terminal`
### 2. 安装VNC服务端
`apt-get install vnc4server`
### 3. 设置VNC密码
`vncserver`
运行上述命令之后，系统会提示输入VNC登录密码，此处我指定的密码为`password`。
### 4. 修改vncserver脚本
```
cp /usr/bin/vncserver /usr/bin/vncserver.bak
vi /usr/bin/vncserver
```
在上述文件中找到：
```bash
"# exec /etc/X11/xinit/xinitrc\n\n".
```
在这一行下面添加以下配置：
```bash
"gnome-panel &\n".
    "gnome-settings-daemon &\n".
    "metacity &\n".
    "nautilus &\n".
    "gnome-terminal &\n".
```
### 5. 修改配置文件
```
cp /root/.vnc/xstartup /root/.vnc/xstartup.bak
vi /root/.vnc/xstartup
```
将上述文件的内容修改为：
```bash
#!/bin/sh
export XKL_XMODMAP_DISABLE=1
unset SESSION_MANAGER
unset DBUS_SESSION_BUS_ADDRESS
[ -x /etc/vnc/xstartup ] && exec /etc/vnc/xstartup
[ -r $HOME/.Xresources ] && xrdb $HOME/.Xresources
xsetroot -solid grey
vncconfig -iconic &
gnome-panel &
gnome-settings-daemon &
metacity &
nautilus &
gnome-terminal &
```
### 6. 建立vncserver服务
`vi /etc/init.d/vncserver`
上述文件的内容为：
```bash
#!/bin/bash
### BEGIN INIT INFO
# Provides:          VNCSERVER
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start daemon at boot time
# Description:       Enable service provided by daemon.
### END INIT INFO
unset VNCSERVERARGS
VNCSERVERS=""
[ -f /etc/vncservers.conf ] && . /etc/vncservers.conf
prog=$"VNC server"
start() {
    . /lib/lsb/init-functions
    REQ_USER=$2
    echo -n $"Starting $prog: "
    ulimit -S -c 0 >/dev/null 2>&1
    RETVAL=0
    for display in $VNCSERVERS
    do
        export USER="${display##*:}"
        if test -z "$REQ_USER" -o "$REQ_USER" == $USER ; then
            echo -n "$display "
            unset BASH_ENV ENV
            DISP="${display%%:*}"
            export VNCUSERARGS="${VNCSERVERARGS[$DISP]}"
            su $USER -c "cd ~$USER && [ -f .vnc/passwd ] && vncserver :$DISP $VNCUSERARGS"
        fi
    done
}
stop() {
    . /lib/lsb/init-functions
    REQ_USER=$2
    echo -n $"Shutting down VNCServer: "
    for display in $VNCSERVERS
    do
        export USER="${display##*:}"
        if test -z "$REQ_USER" -o "$REQ_USER" == $USER ; then
            echo -n "$display "
            unset BASH_ENV ENV
            export USER="${display##*:}"
            su $USER -c "vncserver -kill :${display%%:*}" >/dev/null 2>&1
        fi
    done
    echo -e "\n"
    echo "VNCServer Stopped"
}
case "$1" in
start)
start $@
;;
stop)
stop $@
;;
restart|reload)
stop $@
sleep 3
start $@
;;
condrestart)
if [ -f /var/lock/subsys/vncserver ]; then
stop $@
sleep 3
start $@
fi
;;
status)
status Xvnc
;;
*)
echo $"Usage: $0 {start|stop|restart|condrestart|status}"
exit 1
esac
```
### 7. 修改服务脚本的访问权限
`chmod +x /etc/init.d/vncserver`
### 8. 创建vncservers.conf配置文件
`vi /etc/vncservers.conf`
上述文件的内容为：
```
VNCSERVERS="1:root"
VNCSERVERARGS[1]="-geometry 1024x768"
```
### 9. 将vncserver服务设置为开机启动
`update-rc.d vncserver defaults`
# 三、安装虚拟化环境
### 1. 检查服务器是否支持硬件虚拟化
`egrep -c '(vmx|svm)' /proc/cpuinfo`
若返回的结果为0，则表明服务器不支持硬件虚拟化；若返回的结果为大于等于1，则表明服务器支持硬件虚拟化。即使服务器支持硬件虚拟化，也需要进入BIOS中查看是否开启这项功能。
若服务器不支持硬件虚拟化，或者没有开启硬件虚拟化，那么运行虚拟机的速度会非常慢。
### 2. 检查宿主机系统内核
`egrep -c 'lm' /proc/cpuinfo`
若返回结果为0，则表明宿主机系统内核不是64位的，虚拟机内存最多只能使用2 GB，只能运行32位的客户机系统。
若返回结果为大于等于1，则表明宿主机系统内核是64位的，虚拟机内存可以大于2 GB，并且可以运行64位的客户机系统。
### 3. 安装KVM相关组件
`apt-get install qemu-system qemu-kvm libvirt-bin ubuntu-vm-builder bridge-utils virt-manager`
其中：
- `libvirt-bin` 可以提供libvirtd服务，用于管理qemu和kvm的虚拟机实例；
- `qemu-kvm` 是后端程序；
- `ubuntu-vm-builder` 是用于构建虚拟机的强大的命令行工具；
- `bridge-utils` 可用于向虚拟机提供网络桥接；
- `virt-manager` 是KVM的图形用户界面。
### 4. 添加用户
`adduser `id -un` libvirtd`
若上述命令的输出如下所示，则表明用户添加成功：
![](http://ghoulich.xninja.org/wp-content/uploads/sites/2/2015/12/image-1_adduser.png)
### 5. 重启系统
`reboot`
重启之后，以root账户登录。
必须重新启动或重新登录系统，否则root用户就不能成为libvirtd用户组的有效成员，只有这个用户组的成员才能够运行虚拟机。
### 6. 验证安装
`virsh -c qemu:///system list`
若上述命令的输出如下所示，则表明安装成功：
![](http://ghoulich.xninja.org/wp-content/uploads/sites/2/2015/12/image-2_virsh.png)
使用root账户登录系统时，选择使用GNOME图形界面，这时便可以通过 `Applications` → `System Tools` → `Virtual Machine Manager`来访问KVM的图形界面。
# 四、安装JDK
### 1. 下载JDK 8u45
下载页面：
```
http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html
```
下载目录：
```
/root/Downloads
```
### 2. 解压缩安装包
```bash
cd /root/Downloads
tar xvzf jdk-8u45-linux-x64.tar.gz
mv jdk1.8.0_45 /usr/local/Java
```
### 3. 设置环境变量
`vi /etc/bash.bashrc`
在上述文件末尾添加以下内容：
```bash
JAVA_HOME=/usr/local/Java
PATH=${JAVA_HOME}/bin:$PATH
CLASSPATH=.:${JAVA_HOME}/lib/dt.jar:${JAVA_HOME}/lib/tools.jar
export JAVA_HOME
export PATH
export CLASSPATH
```
### 4. 重新启动
`reboot`
# 五、安装Android SDK
### 1. 下载Android-SDK
```bash
cd /root/Downloads
wget  https://dl.google.com/android/android-sdk_r24.2-linux.tgz
```
### 2. 解压缩安装包
```
dpkg --add-architecture i386
apt-get install lib32bz2-1.0 lib32stdc++6 lib32z1
tar xvzf android-sdk_r24.2-linux.tgz
mv android-sdk-linux /usr/local/Android-SDK
```
### 3. 配置环境变量
`vi /etc/bash.bashrc`
在上述文件中找到`PATH`环境变量，将其修改为：
```bash
PATH=${JAVA_HOME}/bin:/usr/local/Android-SDK/tools:/usr/local/Android-SDK/platform-tools:$PATH
```
### 4. 重新启动
`reboot`
### 5. 配置Android-SDK
`android avd`
运行上述命令之后，便会打开AVD Manager，如下图所示：
![](http://ghoulich.xninja.org/wp-content/uploads/sites/2/2015/12/image-3_avd-manager.png)
点击菜单`Tools` → `Manage SDK`，打开Android SDK Manager，如下图所示：
![](http://ghoulich.xninja.org/wp-content/uploads/sites/2/2015/12/image-4_manage-sdk.png)
在Tools目录中选择 `Android SDK Platform-tools`、`Android SDK Build-tools`，选择`Android 5.1.1 (API 22)`目录，选择`Android 4.4.2 (API 19)`目录，选择`Extras`目录，最后点击 `Install 32 packages...` 按钮，打开 Choose Packages to Install 窗口，如下图所示：
![](http://ghoulich.xninja.org/wp-content/uploads/sites/2/2015/12/image-5_add-package.png)
在上述窗口中选择 Accept License，然后点击 Install 按钮，开始下载上面选择的组件，此时会弹出 Android SDK Manager Log 窗口，如下图所示：
![](http://ghoulich.xninja.org/wp-content/uploads/sites/2/2015/12/image-6_avd-log.png)
