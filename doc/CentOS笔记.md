# CentOS笔记



#  时区设置

设置时区同样, 在 CentOS 7 中, 引入了一个叫 timedatectl 的设置设置程序.

用法很简单:

```shell
# timedatectl # 查看系统时间方面的各种状态
timedatectl status
Local time: 四 2014-12-25 10:52:10 CST
Universal time: 四 2014-12-25 02:52:10 UTC
RTC time: 四 2014-12-25 02:52:10
Timezone: Asia/Shanghai (CST, +0800)
NTP enabled: yes
NTP synchronized: yes
RTC in local TZ: no
DST active: n/a

timedatectl list-timezones # 列出所有时区
timedatectl set-local-rtc 1 # 将硬件时钟调整为与本地时钟一致, 0 为设置为 UTC 时间
timedatectl set-timezone Asia/Shanghai # 设置系统时区为上海
#其实不考虑各个发行版的差异化, 从更底层出发的话, 修改时间时区比想象中要简单:
# cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
```



# yum 的配置，加速插件和更改默认yum源

### yum 安装完成后生成的配置文件及目录：

1. 主配置文件：`/etc/yum.conf`
2. 资源库配置目录：`/etc/yum.repos.d`
3. 重要文件： `/etc/yum.repos.d/CentOS-Base.repo`

------

### yum 加速插件：

实现的功能：可以自动选择速度最快的镜像

1. 安装yum 加速插件： 

   ```shell
   yum install yum-plugin-fastestmirror
   ```

1. 加速插件的配置文件

   /etc/yum/pluginconf.d/fastestmirror.conf

1. yum镜像的速度测试记录文件：
   /var/cache/yum/timedhosts.txt

------

### 更换系统默认yum 源

举例：以更换yum源为阿里云yum源

1. 备份系统默认的yum源

   > mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup

2. 下载阿里云yum源

   > wget -O /etc/yum.repos.d/CentOS-Base.repo <http://mirrors.aliyun.com/repo/Centos-7.repo>

3. 添加 epel 源

   > yum -y install epel-release.noarch
   > 常用的yum源
   > epel源：<https://fedoraproject.org/wiki/EPEL>
   > repoforge源：<http://repoforge.org/use/>
   > php和mysql源：[https://webtatic.com](https://webtatic.com/)

4. 清理缓存

   > yum clean all

5. 生成新的缓存

   > yum makecache

### 安装X Window

```shell
#可查询哪些组件是否已经安装(可用来对照组件名称）
yum grouplist
#安装X图形界面系统
yum list 列出所有可安装的软件包 可以通过 yum grouplist 来查看可能批量安装哪些列表 比如 #yum groupinstall "DNS Name Server" //安装 bind 及 bind-chroot 套件
yum groupinstall "X Window System" -y
#安装GNOME桌面环境
yum groupinstall  "GNOME Desktop Environment" -y
#安装KDE桌面环境
yum groupinstall "KDE (K Desktop Environment)"
```


### 卸载X Window

```shell
#卸载GNOME桌面环境
yum groupremove GNOME Desktop Environment'
#卸载KDE桌面环境
yum groupremove 'K Desktop Environment'
```

### 启动X图形界面的方法

```shell
startx
#设置开机自动启动，修改/etc/inittab
#            id:3:initdefault:      ------>      id:5:initdefault:
init 5
```

### 默认桌面环境选择 

#### 　　一、设置GNOME或者KDE为默认的启动桌面环境

  - 方法1：修改/etc/sysconfig/desktop，根据需要将“DESKTOP”后面的参数设置为KDE或GNOME。
  - 方法2：在当前用户目录下建立“.xinitrc”这个文件，文件的内容就一行startkde或gnome-session。

#### 　　二、GNOME和KDE的切换

##### 　　1、如果需要切换到GNOME：

```shell
switchdesk gnome
```

##### 　　2、如果需要切换到KDE：

```shell
switchdesk kde
```

##### 　　2、如果需要切换到KDE：

```shell
startkde
```

### 图形界面与字符界面的切换

在LINUX中是有多控制台的，其中前6个是字符界面，第七个是图形界面。   
如果你需要切换到字符界面，可以使用CTRL+ALT+F*n*来实现，其中F*n*是F1-F6中的任何一个，
当然如果你是在字符界面之间互相切换就没必要CTRL了，直接ALT F*n*

### 添加第三方yum源

CentOS由于很追求稳定性，所以官方源中自带的软件不多，因而需要一些第三方源。 
比如EPEL、ATrpms、ELRepo、Nux Dextop、RepoForge等。 
网上有很多添加第三方yum源的方法，内德整理了部分方法

#### 添加阿里yum

```shell
wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo
```

#### EPEL

EPEL即Extra Packages for Enterprise Linux，为CentOS提供了额外的10000多个软件包，而且在不替换系统组件方面下了很多功夫，因而可以放心使用。 

```shell
rpm -ivh http://dl.fedoraproject.org/pub/epel/7/x86_64/e/epel-release-7-5.noarch.rpm
```

倘若网站连接失效，可以到以下网站搜索epel-release-7-5.noarch.rpm 
http://www.baidu.com/link?url=MdhH8ypAtivcLZJAQD4JbBcQKrx9X7yNDU–6evVPcO&wd=&eqid=ca19ff090001cdc7000000025a092c44

#### Nux Dextop

Nux Dextop中包含了一些与多媒体相关的软件包，作者尽量保证不覆盖base源。官方说明中说该源与EPEL兼容，实际上个别软件包存在冲突，但基本不会造成影响: 
rpm -Uvh http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-5.el7.nux.noarch.rpm

#### ELRepo

ELRepo包含了一些硬件相关的驱动程序，比如显卡、声卡驱动:

```shell
rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org
rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
```

#### Remi

Remi源大家或许很少听说，不过Remi源GoFace强烈推荐，尤其对于不想编译最新版的linux使用者，因为Remi源中的软件几乎都是最新稳定版。或许您会怀疑稳定不？放心吧，这些都是Linux骨灰级的玩家编译好放进源里的，他们对于系统环境和软件编译参数的熟悉程度毋庸置疑。

```
rpm -Uvh http://rpms.famillecollet.com/enterprise/remi-release-7.rpm
```



#### RPMForge 

RPMForge是CentOS系统下的软件仓库，拥有4000多种的软件包，被CentOS社区认为是最安全也是最稳定的一个软件仓库。

```shell
rpm -Uvh http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.3-1.el7.rf.x86_64.rpm
```

#### RPMFusion

如果您现在正在使用Fedora 15，对RPMFusion一定不陌生吧，各种音频软件如MPlayer在标准源中是没有的，一般先安装RPMFusion源，之后就可以放便地yum install各种需要的软件啦。 
添加阿里云的RPMFusion源

```shell
rpm -Uvh http://mirrors.aliyun.com/rpmfusion/free/el/updates/6/x86_64/rpmfusion-free-release-6-1.noarch.rpm
sudo rpm -Uvh http://mirrors.aliyun.com/rpmfusion/nonfree/el/updates/6/x86_64/rpmfusion-nonfree-release-6-1.noarch.rpm
```

### 配置共享文件夹

#### 安装samba

```shell
#查询是否已经安装了Samba
rpm -qi samba
#下载Samba包并安装。输入“y”，确认安装软件和软件依赖包。
yum install samba
#查看已经安装好的Samba的信息
rpm -qa | grep samba
```

#### 配置samba

```shell
#添加共享文件夹的用户
smbpasswd -a username
#smbpasswd命令的常用方法
#smbpasswd -a 增加用户（要增加的用户必须是系统用户）
#smbpasswd -d 冻结用户，就是这个用户不能在登录了
#smbpasswd -e 恢复用户，解冻用户，让冻结的用户可以在使用
#smbpasswd -n 把用户的密码设置成空. 要在global中写入 null passwords -truesmbpasswd -x 删除用户
#在用户文件夹下新建共享文件夹。
mkdir /home/noiccy/myshare
#将整个myshare文件夹与其中的文件和子目录的权限都设置为rwxrwxrwx,即所有人都可以查看操作。
#777: 0表示没有权限，1表示可执行权限，2表示可写权限，4表示可读权限，然后将其相加。
#所以数字属性的格式应为3个从0到7的八进制数，其顺序是（u）（g）（o）。 
#例如，如果想让某个文件的属主有“读/写”二种权限，需要把4（可读）+2（可写）＝6（读/写）。
chmod -R 777 /home/willy/myshare
cd /etc/samba
#备份smb.conf文件: 
cp smb.conf smb.conf.bat
#修改Samba配置文件
vi smb.conf
```

在/etc/samba/smb.conf 中添加：

```ini
[usr]
        comment = usr guest share   #共享描述
        path = /usr                 #共享目录
        public = yes                #允许guest用户访问
        writable = yes              #允许在heijunma目录下写入
        directory mask = 0775       #默认创建目录权限 rwxrwxr_x
        create mask = 0775          #默认创建文件权限 rwxrwxr_x
        valid users = root          #允许访问该共享的用户
        write list = root           #可写入共享的用户列表
        browseable = yes            #该指定共享目录可浏览
        available = yes             #该指定共享资源可使用
        
[myroot]
        comment = usr guest share
        path = /
        public = yes
        writable = yes
        directory mask = 0775
        create mask = 0775
        valid users = root
        write list = root
        browseable = yes
        available = yes

```

按下“Esc”键，并输入“：wq！”，退出配置文件，

然后运行如下命令重新加载smb.conf，并重启samba

```shell
#关闭防火墙
systemctl stop firewalld
#禁止防火墙开机启动
systemctl disable firewalld
#启动Samba服务
systemctl start smb
#查看IP地址
ifconfig
#重新加载smb.conf
service smb reload
#并重启samba
service smb restart
service nmb restart
```

#### 开机启动smb服务

##### 方法一：修改 /etc/rc.local文件(增加以下部分)

```shell
service smb restart
```

##### 方法一：修改 /etc/rc.d/rc.local文件(增加以下部分)

```shell
/usr/local/samba/sbin/smbd -D
```

##### 方法二：chkconfig smb on

##### 方法三：ntsysv打开图形界面，找到samba选行确认即可

### 安装 VNC 服务器

```shell
# 安装 VNC 服务器
yum install tigervnc-server -y
#service就是5901  连接的时候会用到，之后会说。先cp文件到服务目录system里面
cp /lib/systemd/system/vncserver@.service /etc/systemd/system/vncserver@:1.service
#编辑刚才复制的1.service  ,之前先切到用户 su - wuyantao  && vncpasswd 设置用户连接的密码和ssh登录的密码不一样。
vim /etc/systemd/system/vncserver@:1.service
#pidfile放到用户家目录里面就行，之后执行下面的重新加载配置
systemctl daemon-reload
systemctl enable vncserver@:1.service  #开启启动
systemctl start vncserver@:1.service   #启动服务
#如果要启动那个就把1换了就行   
netstat -anlpt |grep 590
#这是查看服务的启动端口情况
cp /lib/systemd/system/vncserver@.service /etc/systemd/system/vncserver@:3.service
#一个用户用一个编号
#安装抓包工具
yum install wireshark  && yum install wireshark-gnome
#几个命令是要记住的：
vncserver -kill :2   #停止刚才启动的vnc
vncserver :3         #启动的时候选择端口为3
vncserver :1 -geometry 1600x1024  #分辨率
vncserver -list      #查看启动了那些，必须切到启动用户的里面看用户自己的。
```

##### 修改配置文件 /root/.vnc/xstartup

注:有的系统配置里有 –nolisten tcp 和 –nohttpd ，这两个是阻止Xwindows登陆和HTTP方式VNC登陆的，如果需要图形界面，那就删除这部分

```shell
#如果使用的是gnome图像界面，则需要注释掉以下两行,
#xterm -geometry 80x24+10+10 -ls -title “$VNCDESKTOP Desktop” &
#twm &
#并添加以下这行：
gnome-session &
```

例如: 我的配置文件如下
```shell
#!/bin/sh
# Uncomment the following two lines for normal desktop:
unset SESSION_MANAGER
exec /etc/X11/xinit/xinitrc
[ -x /etc/vnc/xstartup ] && exec /etc/vnc/xstartup
[ -r HOME/.Xresources ] && xrdbHOME/.Xresources ] &amp;&amp; xrdbHOME/.Xresources
xsetroot -solid grey
vncconfig -iconic &
xterm -geometry 80x24+10+10 -ls -title “$VNCDESKTOP Desktop” &
twm &
gnome-session &
```
##### 修改用户配置文件：/etc/sysconfig/vncservers

打开配置文件：vi /etc/sysconfig/vncservers，去掉下面这两行前的“#”，也就是说把这两行的注释给去掉。

```ini
VNCSERVERS="1:root"
VNCSERVERARGS[1]="-geometry 800×600"
# 1表示VNC以桌面1运行，这样可以启动root的桌面。你可以添加其他的用户，例如VNCSERVERS=”
# 1:root,2:user1,3:user2”,当然这些用户是实现系统存在的。
```

例如：我的/etc/sysconfig/vncservers配置文件如下：

```ini
VNCSERVERS=”1:root”
VNCSERVERARGS[1]=”-geometry 800×600”
```


另外：如果有图像界面的话，在系统设置>服务器设置>服务中把 vncserver 打勾，可以随系统启动。

##### 改变xstartup的权限

这个问题是很多人容易忽视的问题，也是造成连接成功后黑屏的原因之一。

在CentOS中，xstartup的文件路径为：/root/.vnc/ （可以通过locate xstartup来查看xstartup的文件路径）
所以需要执行：chmod 777 root/.vnc/xstartup

##### 关闭防火墙对VNC端口的限制

如果需要从外网控制内网的某台Linux，则需要打开防火墙相应的端口，并在路由器上进行端口映射。
VNC给浏览器的端口是5800+N，给vncviewer的端口是5900+N，N是设置的display号。
如果不想打开防火墙，但是仍需要VNC这项服务，则需要：
编辑vi /etc/sysconfig/iptables
在里边加入如下两行：
-A RH-Firewall-1-INPUT -m state –state NEW -m tcp -p tcp –dport 5901 -j ACCEPT
-A RH-Firewall-1-INPUT -m state –state NEW -m tcp -p tcp –dport 5902 -j ACCEPT
这里需要的是，display N 对应的端口就是 5900+N,所以不建议设的太多。

##### 启动 VNC Server：

打开终端执行：

```shell
vncserver :1 -geometry 1600x1024
```

第一次运行 vncserver，将会要求输入两次密码，会看到如下提示信息：
New ‘localhost.localdomain:1 (root)’ desktop is localhost.localdomain:1
Starting applications specified in /root/.vnc/xstartup
Log file is /root/.vnc/localhost.localdomain:1.log
上边信息中的“:1”表示启动的第一个vnc桌面。

##### 停止vncserver：（选读）

停止 vncserver 的方法是执行：vncserver -kill :1
注意：“:1”，表示停止第一个vnc桌面，1代表当前启用的VNCserver序号，“:1”之前是有一个空格的。如果目前启动的序号是2，若要关闭其，则应该使用“vncserver -kill :2”.

##### 修改vnc密码：（选读）

修改vnc密码的方法是执行：vncpasswd
输入两次密码即可。

##### 检查服务器端VNC Server是否正常运行中：

执行：ps -ef|grep vnc
如果vnc在执行，可以看到类似的消息：

```shell
[root@IMS ~]# ps -ef|grep vnc
root      3605    1 0 Jul13 ?        00:13:51 Xvnc :1 -desktop MyIMS.local:1 (root) -httpd /usr/share/vnc/classes -auth /root/.Xauthority -geometry 800x600 -depth 16 -rfbwait 30000 -rfbauth /root/.vnc/passwd -rfbport 5901 -pn
```
说明是序号为”:1”vnc进程在端口为5901上进行监听。

##### VNC 客户端

服务器端运行正常后，我们方可以考虑使用VNC客户端与其相连。
Windows 客户端 VNC Viewer 的配置：
1. 从 http://www.realvnc.com/download.html 下载 VNC Free Edition for Windows Version 4.1.2
2. 打开 VNCViewer,填入VNCServer 的IP:编号(1或2或…)
3. VNCViewer 切换全屏模式：F8

##### 一些配置文件：

/etc/X11/xorg.conf 设置屏幕分辨率

到这里，您的VNC应该已经能够连上，但有可能出现黑屏的问题：

1、查看/root/.vnc/下的log日志，有没有报错；有可能是xstartup权限不够。“chmod xstartup 777”更改下权限，重启后再进。
2、重新启动下电脑，试一试。我就是各种配置之后，始终黑屏，重启电脑后OK。 



### 显卡驱动和cuda安装

1. #### 检测显卡驱动

   ```shell
   #添加ELPepo源
   sudo rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org
   sudo rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
   #安装NVIDIA驱动检测
   sudo yum install nvidia-detect
   nvidia-detect -v
   ```

   　　在[NVIDIA](https://www.baidu.com/s?wd=NVIDIA&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)官网上下载对应型号的显卡驱动离线包： http://www.nvidia.cn/Download/Find.aspx?lang=cn

   　　可能需要sudo权限，建议通过su root直接到root账户下执行安装。安装过程中选择“Accept”，并且会询问是否要配置启动参数，选择“yes”。

#### 确认有可以执行cuda程序的gpu

   ```shell
   lspci | grep -i nvidia
   #确认linux版本正确
   uname -m && cat /etc/*release
   ```

#### 显卡冲突

  　　因为NVIDIA驱动会和系统自带nouveau驱动冲突，执行命令查看该驱动状态：

  ```shell
  lsmod | grep nouveau    
  ```

  　　修改/etc/modprobe.d/blacklist.conf 文件，以阻止 nouveau 模块的加载，如果系统没有该文件需要新建一个，这里使用root权限，普通用户无法再在/etc内生成.conf文件。

  ```shell
  su root
  echo -e "blacklist nouveau\noptions nouveau modeset=0" > /etc/modprobe.d/blacklist.conf
  ```

#### 安装驱动 

  ```shell
  #关闭图形界面去安装
  service lightdm stop 
  init 3 
  # 无报错即可
  chmod +x NVIDIA-Linux-x86_64-378.13.linux.run (以实际包名为准) 
  #验证：运行ls 命令，可以看到包名绿色高亮显示 
  #开始安装驱动
  ./NVIDIA-Linux-x86_64-378.13.linux.run –no-opengl-files
  #集显需加opengl相关参数
  # 安装选项
  # Install NVIDIA's 32-bit compatibility libraries ->  NO
  ```

#### 无报错情况下，退回到命令行，即完成了显卡驱动的安装！ : ) 

   ```shell
   #挂载Nvidia驱动 
   modprobe nvidia
   #查看到对应GPU卡的类似信息
   nvidia-smi
   ```

#### 登入系统

 使用组合键【Ctrl】+ 【Alt】+【F2】 ，用root用户登录系统后，使用命令行安装，确保关闭图形界面： 

  命令： service lightdm stop 
  命令： init 3 
  验证：无报错即可 

#### 配置cuda环境变量： 
    命令：vim /etc/profile.d/cuda.sh 
    之后按【i】键，直至屏幕左下角出现 【–INSERT –】字样， 使用 【←】【→】并更改其文本内容为下图所示： 
    
    内容为： 
    
    ```ini
    export PATH=/usr/local/cuda/bin:$PATH
    export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$PATH
    export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH 
    ```
    
    完成更改之后，先按【Esc】键盘，发现【–INSERT –】消失了！然后依次输入【:wq!】（出现在左下角）再回车以保存文件，如下图所示： 
    
    命令：source /etc/profile

#### 完成安装后，验证操作： 
    验证：输入 nvcc -V 可以看到cuda版本 

##### cuda路径添加到bashprofile中

```shell
vim ~/.bash_profile
```

 ```shell
PATH=$PATH:$HOME/bin:/usr/local/cuda/bin
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib64/
CUDA_HOME=/usr/local/cuda

export PATH
export LD_LIBRARY_PATH
export CUDA_HOME
 ```

 查看nvcc版本号

```shell
nvcc -V
```



### 安装cudnn

　　cudnn官网需要注册无法wget，所以在Windows下载然后通过ssh传过去，为什么不用U盘或者共享文件夹？因为我在家。。。服务器刚配置。。。不嫌麻烦用xshell插件也可以。这里安装了一个Windows下ssh窗口传输文件的工具：

```
yum install lrzsz
```

　　运行命令为rz，不用scp了，很方便。

解压 .solitairetheme8 文件到cuda的安装目录

```shell
cp cudnn-8.0-linux-x64-v5.1.solitairetheme8 cudnn-8.0-linux-x64-v5.1.tgz
tar -xvf cudnn-8.0-linux-x64-v5.1.tgz -C /usr/local
```



### 安装vscode

```javascript
sudo rpm --import https://packages.microsoft.com/keys/microsoft.asc
sudo sh -c 'echo -e "[code]\nname=Visual Studio Code\nbaseurl=https://packages.microsoft.com/yumrepos/vscode\nenabled=1\ngpgcheck=1\ngpgkey=https://packages.microsoft.com/keys/microsoft.asc" > /etc/yum.repos.d/vscode.repo'
```

然后用yum命令安装

```javascript
yum check-update
sudo yum install code
```

完成后，就可以用vscode了

## 编译protobuf

protobuf的github地址：https://github.com/google/protobuf支持多种语言，有多个语言的版本，本文采用的是在[CentOS](http://www.linuxidc.com/topicnews.aspx?tid=14) 7下编译源码进行安装。

github上有详细的安装说明：https://github.com/google/protobuf/blob/master/src/README.md

1、确定centos7上已经安装了下面的软件，或者直接用yum进行更新

```shell
autoconf
automake
libtool
curl (used to download gmock)
make
g++
unzip
```

2、下载源码包，解压，编译安装

地址：https://github.com/google/protobuf/releases

选择**Source code** (tar.gz)下载

```shell
tar -zxvf protobuf-3.1.0.tar.gz -C /usr/local/
cd protobuf-3.1.0/
# 如果使用的不是源码，而是release版本 (已经包含gmock和configure脚本)，可以略过这一步
./autogen.sh
# 指定安装路径
./configure --prefix=/usr/local/protobuf
#编译
make
# 测试，这一步很耗时间
make check
make install
# refresh shared library cache.
ldconfig
```

注意make check这一步会花费比较多的时间



3、设置一下环境变量 /etc/profile

```shell
# (动态库搜索路径) 程序加载运行期间查找动态链接库时指定除了系统默认路径之外的其他路径
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/protobuf/lib
# (静态库搜索路径) 程序编译期间查找动态链接库时指定查找共享库的路径
export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/protobuf/lib

export PATH=$PATH:/usr/local/protobuf/bin
```

4、查看版本

```
protoc --version
```

之后，对于相同的系统环境，就不需要再编译了，直接将编译好的protobuf（目录：/usr/local/protobuf）分发到其他计算机，设置环境变量即可。



## 使用openblas-openmp（多线程版本）多线程加速

- yum安装openblas

```shell
sudo yum install openblas-devel
```

- 源代码编译安装

```shell
git clone https://github.com/xianyi/OpenBLAS.git
cd OpenBLAS
sudo make USE_OPENMP=1 -j8
sudo make install
```

- 设置系统环境变量OMP_NUM_THREADS

```
export OPENBLAS_NUM_THREADS=4
or
export GOTO_NUM_THREADS=4
or
export OMP_NUM_THREADS=4

void goto_set_num_threads(int num_threads);
void openblas_set_num_threads(int num_threads);
```



