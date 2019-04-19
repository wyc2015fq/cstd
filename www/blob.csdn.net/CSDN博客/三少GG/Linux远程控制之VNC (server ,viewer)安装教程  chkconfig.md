# Linux远程控制之VNC (server ,viewer)安装教程 || chkconfig - 三少GG - CSDN博客
2011年10月23日 21:13:28[三少GG](https://me.csdn.net/scut1135)阅读数：11956

VNC  可以实现对另外的计算机的操作：
           A ： 可以访问另一个计算机，采用命令终端 或者窗口界面。
           B ： 可以远程控制另一个计算机，两台同步显示操作。
首先，没有readme所说的那么简单。
++++++++++++++++++++++++++++++++++++++++++++++++
本帖网址： [http://bbs.chinaunix.net/thread-1930694-1-1.html](http://bbs.chinaunix.net/thread-1930694-1-1.html)
装了下VNC，解决了不少问题，写个教学贴，给新手们看看，如果你们碰到同样的问题，就可以迅速解决，无需头疼甚至放弃了。
系统环境
VM+RHEL5.1
[root@localhost .vnc]# uname -r
2.6.18-53.el5xen
本地XP系统安装VNCVIEW去控制VM中的RHEL5.1
下面在LINUX上安装VNCSERVER
1、去[www.realvnc.com](http://www.realvnc.com/)去下载免费版for x86linux
2、**下载**是一个tar.gz文件，解压后进入文件夹，里面有个README，请注意这块不要下载RPM包，依赖问题以及包头的安全签名会让你很郁闷的。
3、**解压缩，进入该文件夹，按照readme 要求执行4步   % ./vncinstall /usr/local/bin% ./vncinstall /usr/local/bin /usr/local/man% mkdir -p /usr/local/vnc/classes% cp java/* /usr/local/vnc/classes**
4、启动VNCSERVER，输入连接密码  vncpasswd
5、可能会提示linux1x: error while loading shared libraries: libstdc++-libc6.2-2.so.3: cannot open shared object file: No such file or directory
**去下载libstdc++-libc6.2-2.so.3【下载地址：[http://download.csdn.net/detail/many_many/2601199](http://download.csdn.net/detail/many_many/2601199)】，放到/usr/lib里面**
**（这个我碰到了，通过下载这个SO文件解决了）**
下面这个方法在这个版本中不可取，我已经试过，是无效的
"
在我的系统上ls命令的结果是 libstdc++.so.5.0.7 libstdc++.so.6.0.8
libstdc++.so.6.0.8就是我们要找的，给它做个链接 
代码: 
$ sudo ln -s libstdc++.so.6.0.8 libstdc++-libc6.2-2.so.3
会出现如下错误：
vncpasswd: symbol lookup error:vncpasswd: undefined symbol: __builtin_vec_new
看到这个错感觉就是那两个库不一样造成的字符未定义
"
6、继续启动vncserver，可能还有错误，**log日志内容如下**
vncext: VNC extension running!
vncext: Listening for VNC connections on port 5901
vncext: Listening for HTTP connections on port 5801
vncext: created VNC server for screen 0
error opening security policy file /usr/X11R6/lib/X11/xserver/SecurityPolicy
Could not init font path element /usr/X11R6/lib/X11/fonts/misc/, removing from list!
Could not init font path element /usr/X11R6/lib/X11/fonts/Speedo/, removing from list!
Could not init font path element /usr/X11R6/lib/X11/fonts/Type1/, removing from list!
Could not init font path element /usr/X11R6/lib/X11/fonts/CID/, removing from list!
Could not init font path element /usr/X11R6/lib/X11/fonts/75dpi/, removing from list!
Could not init font path element /usr/X11R6/lib/X11/fonts/100dpi/, removing from list!
Fatal server error:
could not open default font 'fixed'
xsetroot: unable to open display 'localhost.localdomain:1'
vncconfig: unable to open display "localhost.localdomain:1"
Warning: This program is an suid-root program or is being run by the root user.
The full text of the error or warning message cannot be safely formatted
in this environment. You may get a more descriptive message by running the
program as a non-root user or by removing the suid bit on the executable.
xterm Xt error: Can't open display: %s 
**原因：vncserver默认寻找的X系统的字体路径在/usr/X11R6/lib/X11/fonts/下，但**RHEL5却在/usr /share/X11/fonts/下，**Ubuntu10.10却在/usr/share/fonts/X11/下**
解决：
RHEL5：
mkdir /usr/X11R6/lib
cd /usr/X11R6/lib
ln -s /usr/share/X11 X11
**Ubuntu:**
**mkdir /usr/X11R6/lib/X11/cd /usr/X11R6/lib/X11ln -s /usr/share/fonts/X11/ fonts**
7、在XP上启用vncview，注意，这个里面填写的**IP地址加桌面号，一般是1，必须要加端口号！否则进入桌面没有反应**! 
**ifconfig 命令用于查看和更改网络接口的地址和参数！但是如果有多人登录桌面可能就不是1，需要询问管理员**，如下格式：192.168.1.102:1
[root@localhost ~]# netstat -an |grep 01
tcp        0      0 0.0.0.0:5801                0.0.0.0:*                   LISTEN      
tcp        0      0 0.0.0.0:5901                0.0.0.0:*                   LISTEN      
tcp        0      0 0.0.0.0:6001                0.0.0.0:*                   LISTEN   
8 、VNC连接成功后，**发现只有TWM界面，不是传说中的桌面，修改  /home/grant/.vnc/xstartup**
去掉中以下两行前面的#号,让vnc加载默认图形界面程序
# unset SESSION_MANAGER
# exec /etc/X11/xinit/xinitrc
这两行可能有些文件中没有，需要手动添加，没有是显示不了桌面的
修改下面这个东西
把最后一行 twm& 改成------->gonme-session &
附：我的配置文件
#!/bin/sh
[ -r $HOME/.Xresources ] && xrdb $HOME/.Xresources
**unset SESSION_MANAGERexec /etc/X11/xinit/xinitrc**
xsetroot -solid grey
vncconfig -iconic &
xterm -geometry 80x24+10+10 -ls -title "$VNCDESKTOP Desktop" &
**gnome-session &**
本人证明以上方法不行！！！ 
log有错误： exec: 5: /etc/X11/xinit/xinitrc: Permission denied
需要修改如下，本人ubuntu10.10可显示桌面的配置**/home/grant/.vnc/xstartup**如下：
**#!/bin/sh# # Uncomment the following two lines for normal desktop:  unset SESSION_MANAGER  # #exec /etc/X11/xinit/xinitrc  exec /etc/X11/Xsession  [ -r $HOME/.Xresources ] && xrdb $HOME/.Xresourcesxsetroot -solid greyvncconfig -iconic &xterm -geometry 80x24+10+10 -ls -title "$VNCDESKTOP Desktop" &gnome-session &**
9、日志文件中的错误
日志文件位置在root主目录下的隐藏文件 /root/.vnc/localhost.localdomain:1.log 或者是2.log上，这个1和2的区别就是桌面号了，不同的桌面号对应的不同的LOG文件
error opening security policy file /usr/X11R6/lib/X11/xserver/SecurityPolicy
Could not init font path element /usr/X11R6/lib/X11/fonts/Speedo/, removing from list!
Could not init font path element /usr/X11R6/lib/X11/fonts/CID/, removing from list!
这个几个错误一直存在，但是目前没有发现VNC正常运行。请大侠们指点指点。
BTW：一般桌面号启动都是1，可以通过命令来控制vncserver :10 那这样桌面号就变成10，登录的时候就是XX.XX.XX.XX:10就OK了
10、**把VNC加入服务，每次启动自动开启     (比较麻烦，算了，还是自己手动每次vncserver吧)**
这个就不用说了，chkconfig add即可。
11。
修改vnc分辨率大小
------
vncserver默认的分辨率是1024x768，而我们现在的显示器一般的支持都高于这个，为了更好的现实图像，修改分辨率，只修改当前启动的服务，运行：
[root@test]**# vncserver -geometry 1280x1024**
New 'nyback.rrl.com:1 (root)' desktop is nyback.rrl.com:1
Starting applications specified in /root/.vnc/xstartup
Log file is /root/.vnc/nyback.rrl.com:1.log
[root@test]# 
修改默认的分辨率大小需要修改/usr/bin/vncserver文件。
**一般建议启动vnc服务的时候运行调整分辨率大小，不要修改默认的分辨率大小值，防止修改的分辨率过高在其他机器上不能运行。**
参考文章：
# [【VNC】修改VNC分辨率大小](http://space.itpub.net/519536/viewspace-668621)
[上一篇](http://space.itpub.net/batch.common.php?action=viewspace&op=up&itemid=668621&uid=519536) /[下一篇](http://space.itpub.net/batch.common.php?action=viewspace&op=next&itemid=668621&uid=519536)  2010-07-20 22:08:24/
 个人分类：[UNIX及其他](http://space.itpub.net/519536/spacelist-blog-itemtypeid-76431)
[查看( 981 )](http://space.itpub.net/519536/viewspace-668621#xspace-tracks) /[评论( 2 )](http://space.itpub.net/519536/viewspace-668621#xspace-itemreply) /[评分(0
 / 0 )](http://space.itpub.net/519536/viewspace-668621#xspace-itemform)
**VNC**的**分辨率**过小有可能导致图形化界面操作过程中遇到“确认键或取消键”无法点击，分辨率过高又可能导致低分辨率客户端显示器无法显示。
本文给出两种调整VNC分辨率的方法，供参考。
**1.第一种方法：使用**geometry**参数进行调整**
使用man命令获得关于geometry参数的描述
[root@secdb ~]# man vncserver
……
       -geometry widthxheight
              Specify the size of the desktop to be created. Default is 1024x768.
……
可见，默认的分辨率是1024x768，我们可以使用这个参数对分辨率进行调整。
例如，我们需要将分辨率调整到800x600
[root@secdb ~]# vncserver -geometry 800x600
New 'secdb:5 (root)' desktop is secdb:5
Starting applications specified in /root/.vnc/xstartup
Log file is /root/.vnc/secdb:5.log
此时使用“192.168.23.102:5”登录VNC便会得到一个800x600的操作窗口。
其他分辨率调整请自行尝试。
**2.第二种方法：修改配置文件vncservers  **（一直找不到这个文件，难道是安装的免费版的问题？此方法未试过）****
[root@secdb ~]# vi /etc/sysconfig/vncservers
# The VNCSERVERS variable is a list of display:user pairs.
#
# Uncomment the line below to start a VNC **server** on display :1
# as my 'myusername' (adjust this to your own).  You will also
# need to set a VNC password; run 'man vncpasswd' to see how
# to do that.
#
# DO NOT RUN THIS SERVICE if your local area network is
# untrusted!  For a secure way of using VNC, see
# <URL:http://www.uk.research.att.com/vnc/sshvnc.html>.
# VNCSERVERS="1:myusername"
# VNCSERVERARGS[1]="-geometry 800x600"
VNCSERVERS="1:root 2:oracle"
VNCSERVERARGS[1]="-geometry 1024x768"
VNCSERVERARGS[2]="-geometry 1024x768"
例如我们可以将最后一行内容调整为如下
VNCSERVERARGS[2]="-geometry 800x600"
重启vncserver后，使用“192.168.23.102:2”登录VNC便会得到一个800x600的操作窗口，用户是**oracle**。
[root@secdb ~]# /etc/init.d/vncserver restart
Shutting down VNC server: 1:root 2:oracle              [  OK  ]
Starting VNC server: 1:root 2:oracle                   [  OK  ]
**3.小结**
两种修改方法各有利弊，请根据具体环境进行调整。使用第一种方法操作简单，影响范围小，因为该方法是通过单独启动vnc进程来提供服务的，但是容易在系统中遗留过多的vnc进程；第二种方法是通过调整VNC默认分辨率的手段实现的，实现集中**管理**，但存在因分辨率调整过高导致客户端无法使用的问题。
Good luck.
12、不用在.vnc/xstartup文件里加上“scim -d” ， 直接可以在vnc里使用中文输入法。
 但是由于ubuntu和windows里切换输入法的快捷键相同，故用control+space的话会调用windows里的中文输入法。
**解决办法： 将ubuntu里默认ibus设置-->常规-->快捷键开关  设置为shit+control+space即可解决两者的冲突。**
+++++++++++++++++++++++++++++++++++++**以上内容看完即可**+++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
补充：
1安装
vnc
vnc-server
2初始化vnc
vncserver
设置密码
        --(初始化vnc后会在宿主目录下生成一个.vnc的文件夹，下面包括vnc的配置文件)
3添加vnc用户
编辑/etc/sysconfig/vncserver
VNCSERVERS="2:root"
4启动服务
chkconfig --level 35 vncserver on
service vncserver restart
5通过netstat -nal 看5901端口是否开启如果没有开启可使用putty执行下
vncserver  就OK
6使用vncclient连接vnc
ip:192.168.1.x:1
密码：
7如果登陆后没有terminal，说明没有安装xterm包
安装xterm就可以正常使用了
PS
防火墙开启vnc端口
vi /etc/sysconfig/iptables
添加
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 5901 -j ACCEPT
OK
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [Linux下VNCSERVER使用](http://www.cnblogs.com/coderzh/archive/2008/07/16/1243990.html)
#### 启动
```
#vncserver
```
#### 图形界面
```
启动vncserver后，修改/root/.vnc/xstartup,把最后一行 twm& 改成 gnome-session& 或者 kde&(据说KDE在目前的VNC Viewer上的表现不太稳定)
```
#### 自启动
```
配置VNC自动启动。在/etc/sysconfig 下有一个vncservers 的配置文件。修改成如下： VNCSERVERS="1:root" 意思就是使用ip:1即可使用root的用户桌面。 而在屏幕上不显示。 使用chkconfig --level 3 vncserver 即可在启动时加载vncserver
“程序“chkconfig”尚未安装。  您可以使用以下命令安装：
sudo apt-get install chkconfig“
```
#### VNC的启动/停止/重启
```
#service vncserver start/stop/restart 关闭具体的vncserver命令:vncserver -kill :1 vncserver -kill :2
```
#### 设置密码
```
#vncpasswd
```
#### 客户端登陆
```
在vnc客户端中输入：服务器端IP：1 或 服务器端IP：2
```
作者：[CoderZh](http://coderzh.cnblogs.com/)（[CoderZh的技术博客 - 博客园](http://coderzh.cnblogs.com/)）
微博：[http://t.sina.com.cn/coderzh](http://t.sina.com.cn/coderzh)
出处：[http://coderzh.cnblogs.com](http://coderzh.cnblogs.com/)
文章版权归本人所有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文连接，否则保留追究法律责任的权利。 
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [Linux下chkconfig命令详解](http://www.cnblogs.com/panjun-Donet/archive/2010/08/10/1796873.html)
chkconfig命令主要用来更新（启动或停止）和查询系统服务的运行级信息。谨记chkconfig不是立即自动禁止或激活一个服务，它只是简单的改变了符号连接。
使用语法：
chkconfig [--add][--del][--list][系统服务] 或 chkconfig [--level <等级代号>][系统服务][on/off/reset]
chkconfig在没有参数运行时，显示用法。如果加上服务名，那么就检查这个服务是否在当前运行级启动。如果是，返回true，否则返回false。如果在服务名后面指定了on，off或者reset，那么chkconfi 会改变指定服务的启动信息。on和off分别指服务被启动和停止，reset指重置服务的启动信息，无论有问题的初始化脚本指定了什么。on和off开关，系统默认只对运行级3，4，5有效，但是reset可以对所有运行级有效。
参数用法：
   --add 　增加所指定的系统服务，让chkconfig指令得以管理它，并同时在系统启动的叙述文件内增加相关数据。
   --del 　删除所指定的系统服务，不再由chkconfig指令管理，并同时在系统启动的叙述文件内删除相关数据。
   --level<等级代号> 　指定读系统服务要在哪一个执行等级中开启或关毕。
      等级0表示：表示关机
      等级1表示：单用户模式
      等级2表示：无网络连接的多用户命令行模式
      等级3表示：有网络连接的多用户命令行模式
      等级4表示：不可用
      等级5表示：带图形界面的多用户模式
      等级6表示：重新启动
      需要说明的是，level选项可以指定要查看的运行级而不一定是当前运行级。对于每个运行级，只能有一个启动脚本或者停止脚本。当切换运行级时，init不会重新启动已经启动的服务，也不会再次去停止已经停止的服务。
    chkconfig --list [name]：显示所有运行级系统服务的运行状态信息（on或off）。如果指定了name，那么只显示指定的服务在不同运行级的状态。
    chkconfig --add name：增加一项新的服务。chkconfig确保每个运行级有一项启动(S)或者杀死(K)入口。如有缺少，则会从缺省的init脚本自动建立。
    chkconfig --del name：删除服务，并把相关符号连接从/etc/rc[0-6].d删除。
    chkconfig [--level levels] name：设置某一服务在指定的运行级是被启动，停止还是重置。
运行级文件：
每个被chkconfig管理的服务需要在对应的init.d下的脚本加上两行或者更多行的注释。第一行告诉chkconfig缺省启动的运行级以及启动和停止的优先级。如果某服务缺省不在任何运行级启动，那么使用 - 代替运行级。第二行对服务进行描述，可以用\ 跨行注释。
例如，random.init包含三行：
# chkconfig: 2345 20 80
# description: Saves and restores system entropy pool for \
# higher quality random number generation.
使用范例：
chkconfig --list        #列出所有的系统服务
chkconfig --add httpd        #增加httpd服务
chkconfig --del httpd        #删除httpd服务
chkconfig --level httpd 2345 on        #设置httpd在运行级别为2、3、4、5的情况下都是on（开启）的状态
chkconfig --list        #列出系统所有的服务启动情况
chkconfig --list mysqld        #列出mysqld服务设置情况
chkconfig --level 35 mysqld on        #设定mysqld在等级3和5为开机运行服务，--level 35表示操作只在等级3和5执行，on表示启动，off表示关闭
chkconfig mysqld on        #设定mysqld在各等级为on，“各等级”包括2、3、4、5等级
如何增加一个服务：
1.服务脚本必须存放在/etc/ini.d/目录下；
2.chkconfig --add servicename
    在chkconfig工具服务列表中增加此服务，此时服务会被在/etc/rc.d/rcN.d中赋予K/S入口了；
3.chkconfig --level 35 mysqld on
    修改服务的默认启动等级。
+++++++++++++++++++++++++++
- 功能说明：检查、设定系统的各种服务。  
- 
- 语法：chkconfig [--add][--del][--list][系统服务]或  
- chkconfig [--level<等级代号>][系统服务][on/off/reset]  
- 
- 补充说明:这个是redhat公司遵循gpl规则所开发的程序,它可以查询操作系统在每一个执行等级(runlevel)中,会执行哪些系统服务，其中包括各种daemon。  
- 
- linux os 将操作环境分为以下7个等级:  
- 
- 
- 0:开机(请不要切换到此等级)  
- 1:单人使用者模式的文字界面  
- 2:多人使用者模式的文字界面,不具有网络档案系统(NFS)功能  
- 3:多人使用者模式的文字界面,具有网络档案系统(NFS)功能  
- 4:某些发行版的linux使用此等级进入x windows system  
- 5:某些发行版的linux使用此等级进入x windows system  
- 6:重新启动  
- 
- 参数:    --add   新增所指定的系统服务  
-          --del 删除所指定的系统服务  
-          --level 指定该系统服务要在哪个执行等级中开启或关闭  
-          --list   列出当前可从chkconfig指令管理的所有系统服务和等级代号  
-             on/off/reset   在指定的执行登记,开启/关闭/重置该系统服务  
- 
- chkconfig命令主要用来更新（启动或停止）和查询系统服务的运行级信息。谨记chkconfig不是立即自动禁止或激活一个服务，它只是简单的改变了符号连接。  
- 
- 语法：  
- 
-     chkconfig --list [name]  
-     chkconfig --add name  
-     chkconfig --del name  
-     chkconfig [--level levels] name <on|off|reset>  
-     chkconfig [--level levels] name  
- 
-     chkconfig 没有参数运行时，显示用法。如果加上服务名，那么就检查这个服务是否在当前运行级启动。如果是，返回**true**，否则返回**false**。如果在服务名后面指定 了on，off或者reset，那么chkconfi 会改变指定服务的启动信息。on和off分别指服务被启动和停止，reset指重置服务的启动信息，无论有问题的初始化脚本指定了什么。on和off开 关，系统默认只对运行级3，4，5有效，但是reset可以对所有运行级有效。  
- 
-     --level选项可以指定要查看的运行级而不一定是当前运行级。  
- 
-     需要说明的是，对于每个运行级，只能有一个启动脚本或者停止脚本。当切换运行级时，init不会重新启动已经启动的服务，也不会再次去停止已经停止的服务。  
- 
-     chkconfig --list ：显示所有运行级系统服务的运行状态信息（on或off）。如果指定了name，那么只显示指定的服务在不同运行级的状态。  
- 
-     chkconfig --add name：增加一项新的服务。chkconfig确保每个运行级有一项启动(S)或者杀死(K)入口。如有缺少，则会从缺省的init脚本自动建立。  
- 
-     chkconfig --del name：删除服务，并把相关符号连接从/etc/rc[0-6].d删除。  
- 
-     chkconfig [--level levels] name <on|off|reset>：设置某一服务在指定的运行级是被启动，停止还是重置。例如，要在3，4，5运行级停止nfs服务，则命令如下：  
- 
-     chkconfig --level 345 nfs off  
- 
- 运行级文件：  
- 
-     每个被chkconfig管理的服务需要在对应的init.d下的脚本加上两行或者更多行的注释。第一行告诉chkconfig缺省启动的运行级以及启动 和停止的优先级。如果某服务缺省不在任何运行级启动，那么使用 - 代替运行级。第二行对服务进行描述，可以用\ 跨行注释。  
- 例如，random.init包含三行：  
- # chkconfig: 23452080
- # description: Saves and restores system entropy pool **for** \  
- # higher quality random number generation.  
- 
- 附加介绍一下Linux系统的运行级的概念：  
-     Linux中有多种运行级，常见的就是多用户的2，3，4，5 ，很多人知道5是运行X-Windows的级别，而0就是关机了。运行级的改变可以通过init命令来切换。例如，假设你要维护系统进入单用户状态，那 么，可以使用init1来切换。在Linux的运行级的切换过程中，系统会自动寻找对应运行级的目录/etc/rc[0-6].d下的K和S开头的文件， 按后面的数字顺序，执行这些脚本。对这些脚本的维护，是很繁琐的一件事情，Linux提供了chkconfig命令用来更新和查询不同运行级上的系统服 务。  
- 
- 范例:  
- 
- 1.查看在各种不同的执行等级中,各项服务的状况:  
- $chkconfig --list  
- 
- 2.列出系统服务vsftpd在各个执行等级的启动情况：  
- $chkconfig --list vsftpd  
- 
- 3.在执行等级3,5时，关闭vsftpd系统服务：  
- $chkconfig --level 35 vsftpd off  
- 
- 4.在执行等级2，3，5时，开启 vsftpd系统服务：  
- $chkconfig --level 235 vsftpd on  
- 
- 5.关闭一些自己不需要的服务 ->  
- 如果没有打印机：  
- chkconfig --level 235 cups off   
- 如果没有局域网：  
- chkconfig --level 235 smb off  
- 如果不需要远程用户登录的：  
- chkconfig --level 235 sshd off  
- 如果不需要定时任务的：  
- chkconfig --level 235 crond off  
- 如果不需要添加新硬件的：  
- chkconfig --level 235 kudzu off  
++++++++++
不像DOS 或者 Windows，Linux 可以有多种运行级。常见的就是多用户的2,3,4,5 ，很多人知道 5 是运行 X-Windows 的级别，而 0 就是关机了。运行级的改变可以通过 init 命令来切换。例如，假设你要维护系统进入单用户状态，那么，可以使用 init 1 来切换。在Linux 的运行级的切换过程中，系统会自动寻找对应运行级的目录/etc/rc[0-6].d下的K 和 S 开头的文件，按后面的数字顺序，执行这些脚本。对这些脚本的维护，是很繁琐的一件事情，Linux 提供了chkconfig
 命令用来更新和查询不同运行级上的系统服务。
语法为：
chkconfig --list [name]
chkconfig --add name
chkconfig --del name
chkconfig [--level levels] name <on|off|reset>
chkconfig [--level levels] name
chkconfig 有五项功能：添加服务，删除服务，列表服务，
改变启动信息以及检查特定服务的启动状态。
chkconfig 没有参数运行时，显示用法。如果加上服务名，那么就检查这个服务是否在当前运行级启动。如果是，返回 true，否则返回 false。 --level 选项可以指定要查看的运行级而不一定是当前运行级。
如果在服务名后面指定了on，off 或者 reset，那么 chkconfig 会改变指定服务的启动信息。on 和 off 分别指服务在改变运行级时的启动和停止。reset 指初始化
服务信息，无论有问题的初始化脚本指定了什么。
对于 on 和 off 开关，系统默认只对运行级 3，4， 5有效，但是 reset 可以对所有运行级有效。指定 --level 选项时，可以选择特定的运行级。
需要说明的是，对于每个运行级，只能有一个启动脚本或者停止脚本。当切换运行级时，init 不会重新启动已经启动的服务，也不会再次去停止已经停止的服务。
选项介绍：
--level levels
指定运行级，由数字 0 到 7 构成的字符串，如：
--level 35 表示指定运行级3 和5。
--add name
这个选项增加一项新的服务，chkconfig 确保每个运行级有一项 启动(S) 或者 杀死(K) 入口。如有缺少，则会从缺省的init 脚本自动建立。
--del name
用来删除服务，并把相关符号连接从 /etc/rc[0-6].d 删除。
--list name
列表，如果指定了name 那么只是显示指定的服务名，否则，列出全部服务在不同运行级的状态。
运行级文件
每个被chkconfig 管理的服务需要在对应的init.d 下的脚本加上两行或者更多行的注释。
第一行告诉 chkconfig 缺省启动的运行级以及启动和停止的优先级。如果某服务缺省不在任何运行级启动，那么使用 - 代替运行级。
第二行对服务进行描述，可以用\ 跨行注释。
例如，random.init 包含三行：
# chkconfig: 2345 20 80
# description: Saves and restores system entropy pool for \
# higher quality random number generation.
表明 random 脚本应该在运行级 2, 3, 4, 5 启动，启动优先权为20，停止优先权为 80。
好了，介绍就到这里了，去看看自己目录下的/etc/rc.d/init.d 下的脚本吧。
**[Linux中修改系统服务的命令:chkconfig (check config)](http://www.2cto.com/os/201108/101597.html)**
chkconfig
功能说明：检查，设置系统的各种服务。
语　　法：chkconfig [--add][--del][--list][系统服务] 或 chkconfig [--level <等级代号>][系统服务][on/off/reset]
补充说明：这是Red Hat公司遵循GPL规则所开发的程序，它可查询操作系统在每一个执行等级中会执行哪些系统服务，其中包括各类常驻服务。
参　　数：
　--add 　增加所指定的系统服务，让chkconfig指令得以管理它，并同时在系统启动的叙述文件内增加相关数据。
　--del 　删除所指定的系统服务，不再由chkconfig指令管理，并同时在系统启动的叙述文件内删除相关数据。
　--level<等级代号> 　指定读系统服务要在哪一个执行等级中开启或关毕
首先来执行 chkconfig --list命令来查看所有已添加的系统服务(下面的结果只是一部分)
[root@fd init.d]# chkconfig --list
diskdump        0:off   1:off   2:off   3:off   4:off   5:off   6:off
xfs             0:off   1:off   2:on    3:on    4:on    5:on    6:off
rhnsd           0:off   1:off   2:off   3:on    4:on    5:on    6:off
cups-config-daemon      0:off   1:off   2:off   3:on    4:on    5:on    6:off
netfs           0:off   1:off   2:off   3:on    4:on    5:on    6:off
nfs             0:off   1:off   2:off   3:off   4:off   5:off   6:off
microcode_ctl   0:off   1:off   2:on    3:on    4:on    5:on    6:off
netplugd        0:off   1:off   2:off   3:off   4:off   5:off   6:off
netdump         0:off   1:off   2:off   3:off   4:off   5:off   6:off
cpuspeed        0:off   1:on    2:on    3:on    4:on    5:on    6:off
syslog          0:off   1:off   2:on    3:on    4:on    5:on    6:off
smb             0:off   1:off   2:off   3:off   4:off   5:off   6:off
ntpd            0:off   1:off   2:off   3:off   4:off   5:off   6:off
rawdevices      0:off   1:off   2:off   3:on    4:on    5:on    6:off
psacct          0:off   1:off   2:off   3:off   4:off   5:off   6:off
vncserver       0:off   1:off   2:off   3:off   4:off   5:off   6:off
saslauthd       0:off   1:off   2:off   3:off   4:off   5:off   6:off
dc_server       0:off   1:off   2:off   3:off   4:off   5:off   6:off
mdmonitor       0:off   1:off   2:on    3:on    4:on    5:on    6:off
httpd           0:off   1:off   2:off   3:off   4:off   5:off   6:off
ibmasm          0:off   1:off   2:off   3:off   4:off   5:off   6:off
dc_client       0:off   1:off   2:off   3:off   4:off   5:off   6:off
其中第一列表示服务名，后面的0-6表示在这六种启动方式中该服务是启动（on）还是关闭（off）
比如默认情况下，自动检测新硬件的服务是启动的(服务名是kudzu)，每次启动需要检测一段时间，我们可以关闭服务，以后有了硬件改动在调用它。
chkconfig kudzu off
chkconfig --list kudzu
kudzu           0:off   1:off   2:off   3:off   4:off   5:off   6:off
可以看到检测新硬件的服务都被关闭了，再次启动时就不会检测新硬件了。如果有了硬件改动你可以选择将此服务打开(chkconfig kudzu on)，其实有更简单的方法，输入
service kudzu start
手动启动检测新硬件服务就可以了。这些服务都存放在/etc/init.d目录下，他们都是可执行的shell文件，比如刚才用的检测新硬件服务
[root@fd /]# ls -l /etc/init.d/kudzu
-rwxr-xr-x  1 root root 2095 Aug 23  2005 /etc/init.d/kudzu
其实我们编写的shell文件也可以放在这里，然后添加成系统服务，但是我们必须遵守一个简单的预定。我们先来看看kudzu的具体内容吧
根据这个模式编写一个简单的启动tomcat的服务脚本，vi /etc/init.d/tomcat，内容如下
[root@fd /]# cat /etc/init.d/kudzu
#!/bin/bash
#
# kudzu         This scripts runs the kudzu hardware probe.
#
# chkconfig: 345 05 95
# description:  This runs the hardware probe, and optionally configures \
#               changed hardware.
# This is an interactive program, we need the current locale
具体内容省略
# chkconfig: 2345 20 80
# description: simple example to start tomcat
export JAVA_HOME=/usr/java/jdk1.5.0_16
export CLASS_PATH=/usr/java/jdk1.5.0_16/lib
export PATH=$JAVA_HOME/bin:$PATH
/usr/Tomcat/bin/startup.sh
将tomcat添加到服务中
chkconfig --add tomcat
状态设为启动
chkconfig tomcat on
        在tomcat文件的头几行的注释语句中，必须包含chkconfig和description两部分内容，否则在执行“chkconfig --add tomcat”时，会出现“tomcat服务不支持chkconfig”的错误提示。chkconfig这行表示缺省启动的运行级别以及启动和停止的优先级，如该服务缺省不再任何运行级启动，则以 - 代替运行级别。在tomcat中表示脚本在运行级2、3、4、5启动，启动优先权为20（优先权数越大服务启动的越晚），停止优先权为80。如果服务已经设置好了，那么通过编辑tomcat文件来修改启动优先权就不管用了，先删除服务再添加进来就可以了。
        我模仿检测新硬件服务的内容编写一个较为复杂的tomcat服务，在系统启动时启动tomcat，在系统关闭时关闭tomcat。内容如下
# chkconfig: 2345 30 70
# description: Starts and Stops Tomcat.
export JAVA_HOME=/usr/java/jdk1.5.0_16
export CLASS_PATH=/usr/java/jdk1.5.0_16/lib
export PATH=$JAVA_HOME/bin:$PATH
case "$1" in
 start)
 /usr/Tomcat/bin/startup.sh
 touch /var/lock/subsys/tomcat
 ;;
 status)
        if [ -f /var/lock/subsys/tomcat ]; then
          echo $"tomcat is running"
          exit 0
        fi
        echo $"tomcat has stopped"
        exit 3
        ;;
 stop)
 /usr/Tomcat/bin/shutdown.sh
 rm -f /var/lock/subsys/tomcat
 ;;
 restart)
 /usr/Tomcat/bin/shutdown.sh
/usr/Tomcat/bin/startup.sh
;;
*)
echo "Usage: tomcat {start|stop|restart|status}"
exit 1
esac
exit 0
