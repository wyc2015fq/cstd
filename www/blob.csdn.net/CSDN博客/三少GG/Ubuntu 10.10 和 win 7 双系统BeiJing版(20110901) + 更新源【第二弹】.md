# Ubuntu 10.10 和 win 7 双系统BeiJing版(20110901) + 更新源【第二弹】 - 三少GG - CSDN博客
2011年09月01日 10:15:38[三少GG](https://me.csdn.net/scut1135)阅读数：1801
【前言】着重讲遇到的几个重大问题。之前自己的blog已经提到，安装过程也需参考前文，重点内容及安装过程中遇到的问题再强调一遍。。。装了几天了..被源列表，adobe flash player ，pku网络搞崩溃了...
此次安装为光盘刻盘后，用盘安装，版本为ubuntu-10.10-desktop-i386.iso（六维空间下载）。用win7自带工具压缩卷出50G空间（笔记本小的话25G也行）
1、规划分区
1）双系统：
如果你的电脑里已经装有一个Windows系统，而且硬盘有两个以上的分区的话。你需要把其中一个10GB以上的分区删除掉（最好是删除最后一个分区），以空出一部分未分区的硬盘空间用来安装Ubuntu。这样安装Ubuntu后，系统会自动建立一个启动菜单，让你在开机时选择启动Windows还是Ubuntu。
在Windows里删除分区的方法是：
a）把要删除的分区中的所有文件移到别的分区。
b）右键单击“我的电脑”，选择“管理——磁盘管理”。或“控制面板——管理工具——双击‘计算机管理’——存储——磁盘管理”。
c）右键单击要删除的分区，选择“删除逻辑驱动器”。
**如果你的硬盘不是全新的硬盘的话，就不用新建分区表了，直接选择“空闲”的空间，再点“添加”按钮**.
先创建 / 分区：“新分区的类型”选择“主分区”；“新分区的位置”保留默认的“起始”；“用于”“Ext4 日志文件系统”；“挂载点”选择“/”。
选择“空闲”空间，点击“添加”按钮。
接着创建swap交换空间：“新分区的类型”选“逻辑分区”；“新分区的位置”保留默认的“起始”；“用于”“交换空间”；“挂载点”不用选。
还可以再分一个 /home，挂载点选“/home”就行了，用“Ext4 日志文件系统”。
优秀的参考文章：
1. [http://bbs.ylmf.net/forum.php?mod=viewthread&tid=1235981](http://bbs.ylmf.net/forum.php?mod=viewthread&tid=1235981)
2.[http://tech.163.com/digi/11/0127/18/6RE3LQL400162OUT.html](http://tech.163.com/digi/11/0127/18/6RE3LQL400162OUT.html)
......
** 强调下此次我的配置：**
**My Configuration:  50G in total (win7自带管理工具分出50G 空白空间出来，此步骤必需！)**
**200M 挂载点  /boot**
**2000M         swap space**
**15000M       /**
**33000M         /home**
安装完成后会发现，没有系统引导。。。
解决办法：
easyBCD1.7.2   EasyBCD 1.7.2 Add/Remove Entries  Add an Entr--> Linux  Type:Grub  Drive:    /boot   (看大小为200M)
++++++++++++++++++++++++++++++++++++
坑爹的更新源列表啊。两个原因：
1. 安装过程中时区随便选择了个HK, 结果默认的源列表基本都是hk.ubuntu.com.... 由于无法访问香港的ip，导致无法更新。下次要选 shanghai 啊！！！
2. pku免费地址上网问题，封掉了adobe，ubuntu，国外大部分网站。由于没有搜狗，google engine代理 无法翻墙。解决办法：在所有网站域名后添加 .sixxs.org 即通过未被屏蔽的IPv6访问。此招非常实用。** .sixxs.org**
关于更新的几个源列表（都不错，3还带了系统10.10默认的几个配置）：
sudo gedit /etc/apt/sources.list
sudo apt-get update
1. [http://apps.hi.baidu.com/share/detail/43302720](http://apps.hi.baidu.com/share/detail/43302720)
2.[http://www.ylmf.net/ubuntu/tips/2010123019307.html](http://www.ylmf.net/ubuntu/tips/2010123019307.html)
3.[http://www.cnblogs.com/xiaouisme/archive/2011/04/04/2005071.html](http://www.cnblogs.com/xiaouisme/archive/2011/04/04/2005071.html)
4.http://daydream2008.blog.163.com/blog/static/589936212010915114512389/
附上 我的 源列表（适合教育网 @PKU）
deb http://cn.archive.ubuntu.com/ubuntu/ maverick-backports main restricted universe multiverse
deb-src http://cn.archive.ubuntu.com/ubuntu/ maverick-backports main restricted universe multiverse
deb http://cn.archive.ubuntu.com/ubuntu/ maverick multiverse
deb-src http://cn.archive.ubuntu.com/ubuntu/ maverick multiverse
deb http://cn.archive.ubuntu.com/ubuntu/ maverick-updates multiverse
deb-src http://cn.archive.ubuntu.com/ubuntu/ maverick-updates multiverse
#系统自带
deb http://cn.archive.ubuntu.com/ubuntu/ maverick universe
deb-src http://cn.archive.ubuntu.com/ubuntu/ maverick universe
deb http://cn.archive.ubuntu.com/ubuntu/ maverick-updates universe
deb-src http://cn.archive.ubuntu.com/ubuntu/ maverick-updates universe
deb http://cn.archive.ubuntu.com/ubuntu/ maverick multiverse
deb-src http://cn.archive.ubuntu.com/ubuntu/ maverick multiverse
deb http://cn.archive.ubuntu.com/ubuntu/ maverick-updates multiverse
deb-src http://cn.archive.ubuntu.com/ubuntu/ maverick-updates multiverse
deb http://archive.canonical.com.sixxs.org/ubuntu maverick partner
deb-src http://archive.canonical.com.sixxs.org/ubuntu maverick partner
#官方的Ubuntu 10.10要保留几个用于紧急更新
deb http://security.Ubuntu.com/ubuntu maverick-security main restricted
deb-src http://security.Ubuntu.com/ubuntu maverick-security main restricted
deb http://security.Ubuntu.com/ubuntu maverick-security universe
deb-src http://security.Ubuntu.com/ubuntu maverick-security universe
deb http://security.Ubuntu.com/ubuntu maverick-security multiverse
deb-src http://security.Ubuntu.com/ubuntu maverick-security multiverse
# Archive.ubuntu.com更新服务器（欧洲，此为官方源，电信网通用户使用)：
deb http://archive.ubuntu.com.sixxs.org/ubuntu/ maverick main restricted universe multiverse
deb http://archive.ubuntu.com.sixxs.org/ubuntu/ maverick-security main restricted universe multiverse
deb http://archive.ubuntu.com.sixxs.org/ubuntu/ maverick-updates main restricted universe multiverse
deb http://archive.ubuntu.com.sixxs.org/ubuntu/ maverick-proposed main restricted universe multiverse
deb http://archive.ubuntu.com.sixxs.org/ubuntu/ maverick-backports main restricted universe multiverse
deb-src http://archive.ubuntu.com.sixxs.org/ubuntu/ maverick main restricted universe multiverse
deb-src http://archive.ubuntu.com.sixxs.org/ubuntu/ maverick-security main restricted universe multiverse
deb-src http://archive.ubuntu.com.sixxs.org/ubuntu/ maverick-updates main restricted universe multiverse
deb-src http://archive.ubuntu.com.sixxs.org/ubuntu/ maverick-proposed main restricted universe multiverse
deb-src http://archive.ubuntu.com.sixxs.org/ubuntu/ maverick-backports main restricted universe multiverse
#网易 Ubuntu 10.10 源（速度很快）
deb http://mirrors.163.com/ubuntu/ maverick main universe restricted multiverse
deb-src http://mirrors.163.com/ubuntu/ maverick main universe restricted multiverse
deb http://mirrors.163.com/ubuntu/ maverick-security universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ maverick-security universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ maverick-updates universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ maverick-proposed universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ maverick-proposed universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ maverick-backports universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ maverick-backports universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ maverick-updates universe main multiverse restricted
#sohu的源
deb http://mirrors.sohu.com/ubuntu/ maverick main restricted
deb-src http://mirrors.sohu.com/ubuntu/ maverick main restricted
deb http://mirrors.sohu.com/ubuntu/ maverick-updates main restricted
deb-src http://mirrors.sohu.com/ubuntu/ maverick-updates main restricted
deb http://mirrors.sohu.com/ubuntu/ maverick universe
deb-src http://mirrors.sohu.com/ubuntu/ maverick universe
deb http://mirrors.sohu.com/ubuntu/ maverick-updates universe
deb-src http://mirrors.sohu.com/ubuntu/ maverick-updates universe
deb http://mirrors.sohu.com/ubuntu/ maverick multiverse
deb-src http://mirrors.sohu.com/ubuntu/ maverick multiverse
deb http://mirrors.sohu.com/ubuntu/ maverick-updates multiverse
deb-src http://mirrors.sohu.com/ubuntu/ maverick-updates multiverse
#中国台湾的源
deb http://security.Ubuntu.com.sixxs.org/ubuntu/ maverick-security universe main multiverse restricted
deb-src http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ maverick-security universe main multiverse restricted
deb http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ maverick main universe restricted multiverse
deb-src http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ maverick main universe restricted multiverse
deb http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ maverick-updates universe main multiverse restricted
deb-src http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ maverick-updates universe main multiverse restricted
deb http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ maverick-proposed universe main multiverse restricted
deb-src http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ maverick-proposed universe main multiverse restricted
deb http://archive.canonical.com.sixxs.org/ubuntu maverick partner
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
【2012.05.25 @pku Ubuntu 12.04完整版】
# deb cdrom:[Ubuntu 12.04 LTS _Precise Pangolin_ - Release i386 (20120423)]/ precise main restricted
# See http://help.ubuntu.com/community/UpgradeNotes for how to upgrade to
# newer versions of the distribution.
deb http://archive.ubuntu.com/ubuntu precise main restricted
deb-src http://archive.ubuntu.com/ubuntu precise main restricted
## Major bug fix updates produced after the final release of the
## distribution.
deb http://archive.ubuntu.com/ubuntu precise-updates main restricted
deb-src http://archive.ubuntu.com/ubuntu precise-updates main restricted
## N.B. software from this repository is ENTIRELY UNSUPPORTED by the Ubuntu
## team. Also, please note that software in universe WILL NOT receive any
## review or updates from the Ubuntu security team.
deb http://archive.ubuntu.com/ubuntu precise universe
deb-src http://archive.ubuntu.com/ubuntu precise universe
deb http://archive.ubuntu.com/ubuntu precise-updates universe
deb-src http://archive.ubuntu.com/ubuntu precise-updates universe
## N.B. software from this repository is ENTIRELY UNSUPPORTED by the Ubuntu 
## team, and may not be under a free licence. Please satisfy yourself as to 
## your rights to use the software. Also, please note that software in 
## multiverse WILL NOT receive any review or updates from the Ubuntu
## security team.
deb http://archive.ubuntu.com/ubuntu precise multiverse
deb-src http://archive.ubuntu.com/ubuntu precise multiverse
deb http://archive.ubuntu.com/ubuntu precise-updates multiverse
deb-src http://archive.ubuntu.com/ubuntu precise-updates multiverse
## N.B. software from this repository may not have been tested as
## extensively as that contained in the main release, although it includes
## newer versions of some applications which may provide useful features.
## Also, please note that software in backports WILL NOT receive any review
## or updates from the Ubuntu security team.
deb http://archive.ubuntu.com/ubuntu precise-backports main restricted universe multiverse
deb-src http://archive.ubuntu.com/ubuntu precise-backports main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu precise-security main restricted
deb-src http://archive.ubuntu.com/ubuntu precise-security main restricted
deb http://archive.ubuntu.com/ubuntu precise-security universe
deb-src http://archive.ubuntu.com/ubuntu precise-security universe
deb http://archive.ubuntu.com/ubuntu precise-security multiverse
deb-src http://archive.ubuntu.com/ubuntu precise-security multiverse
## Uncomment the following two lines to add software from Canonical's
## 'partner' repository.
## This software is not part of Ubuntu, but is offered by Canonical and the
## respective vendors as a service to Ubuntu users.
deb http://archive.canonical.com/ubuntu precise partner
deb-src http://archive.canonical.com/ubuntu precise partner
deb http://archive.canonical.com/ubuntu lucid partner
## This software is not part of Ubuntu, but is offered by third-party
## developers who want to ship their latest software.
deb http://extras.ubuntu.com/ubuntu precise main
deb-src http://extras.ubuntu.com/ubuntu precise main
#网易 Ubuntu 10.10 源（速度很快）
deb http://archive.ubuntu.com/ubuntu precise multiverse
deb-src http://archive.ubuntu.com/ubuntu precise multiverse
deb http://archive.ubuntu.com/ubuntu precise-security restricted
deb-src http://archive.ubuntu.com/ubuntu precise-security restricted
deb http://archive.ubuntu.com/ubuntu precise-proposed restricted
deb-src http://archive.ubuntu.com/ubuntu precise-proposed restricted
deb http://archive.ubuntu.com/ubuntu precise-backports restricted
deb-src http://archive.ubuntu.com/ubuntu precise-backports restricted
#sohu的源
#中国台湾的源
deb http://security.Ubuntu.com.sixxs.org/ubuntu/ precise-security universe main multiverse restricted
deb-src http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ precise-security universe main multiverse restricted
deb http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ precise main universe restricted multiverse
deb-src http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ precise main universe restricted multiverse
deb http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ precise-updates universe main multiverse restricted
deb-src http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ precise-updates universe main multiverse restricted
deb http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ precise-proposed universe main multiverse restricted
deb-src http://tw.archive.Ubuntu.com.sixxs.org/ubuntu/ precise-proposed universe main multiverse restricted
deb http://archive.canonical.com.sixxs.org/ubuntu precise partner
# Archive.ubuntu.com更新服务器（欧洲，此为官方源，电信网通用户使用)：
deb http://archive.ubuntu.com.sixxs.org/ubuntu/ precise main restricted universe multiverse
deb http://archive.ubuntu.com.sixxs.org/ubuntu/ precise-security main restricted universe multiverse
deb http://archive.ubuntu.com.sixxs.org/ubuntu/ precise-updates main restricted universe multiverse
deb http://archive.ubuntu.com.sixxs.org/ubuntu/ precise-proposed main restricted universe multiverse
deb http://archive.ubuntu.com.sixxs.org/ubuntu/ precise-backports main restricted universe multiverse
deb-src http://archive.ubuntu.com.sixxs.org/ubuntu/ precise main restricted universe multiverse
deb-src http://archive.ubuntu.com.sixxs.org/ubuntu/ precise-security main restricted universe multiverse
deb-src http://archive.ubuntu.com.sixxs.org/ubuntu/ precise-updates main restricted universe multiverse
deb-src http://archive.ubuntu.com.sixxs.org/ubuntu/ precise-proposed main restricted universe multiverse
deb-src http://archive.ubuntu.com.sixxs.org/ubuntu/ precise-backports main restricted universe multiverse
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Language:
中文。
system-administration-language support  //这个过程是需要联网下载语言包的，也即上面的更新源问题要解决好。
添加或删除语言--应用到整个系统。键盘输入方式就可以改为ibus了
++++++++++++++++
Input: Chinese:
IBus 输入法。可参照下两文，但其实10.10已经改变很大，很简洁方便了。直接点击键盘标志符设置，跟win7非常类似了。
1. http://wiki.ubuntu.org.cn.sixxs.org/IBus
2. [http://blog.csdn.net/scut1135/article/details/5539232](http://blog.csdn.net/scut1135/article/details/5539232)
++++++++++++++++
Adobe Flash Player:
坑爹啊
各种不行。。。
http://get.adobe.com/cn/flashplayer/otherversions/ 
http://www.adobe.com/cn/downloads/ 
.sixxs.org
[http://www.williamlong.info/archives/1905.html](http://www.williamlong.info/archives/1905.html)
**Firefox的Flash问题**
　　经过我的实际测试，Ubuntu自动安装的Flash插件swfdec存在很多问题，在Firefox中，很多网页的Flash无法显示，包括Google音乐和开心网等，因此建议使用下面两条语句将其卸载。
　　sudo apt-get remove swfdec-mozilla
　　sudo apt-get remove swfdec-gnome
　　之后可安装官方的Adobe Flash Player的Linux版，下载地址是： [http://get.adobe.com/flashplayer/](http://get.adobe.com/flashplayer/)
　　安装完成后，还要解决中文乱码问题，解决方法是执行下面语句：
　　sudo cp /etc/fonts/conf.d/49-sansserif.conf /etc/fonts/conf.d/49-sansserif.conf.bak
　　sudo rm /etc/fonts/conf.d/49-sansserif.conf
　　之后，Firefox的Flash就完全正常了，在Firefox中访问开心网等Flash网站，显示都正常。
解决办法：
[http://wiki.debian.org.hk.sixxs.org/w/Install_Flash_Player_with_APT](http://wiki.debian.org.hk.sixxs.org/w/Install_Flash_Player_with_APT)
### Canonical 軟件庫中的 adobe-flashplugin 
除了 flashplugin-nonfree， Canonical 軟件庫亦為 Ubuntu 8.04 LTS (Hardy) 和 8.10 (Intrepid) 提供 adobe-flashplugin 套件安裝 Flash Player 10。要使用 adobe-flashplugin 取代 flashplugin-nonfree，請先確定您的[/etc/apt/sources.list](http://wiki.debian.org.hk.sixxs.org/w/File:/etc/apt/sources.list)
 中有以下一行：
**deb http://archive.canonical.com.sixxs.org/ubuntu/ *natty* partner**  （改为10.10相应版本。别忘了加.sixxs.org）
更新最新軟件資料：
**apt-get update**
安裝套件 adobe-flashplugin：
**apt-get install adobe-flashplugin**
然後重新啟動 [Mozilla Firefox](http://wiki.debian.org.hk.sixxs.org/w/Mozilla_Firefox) 或 [Epiphany](http://wiki.debian.org.hk.sixxs.org/w/Epiphany)。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
///////////
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1. 搭建java6 环境 ，参考我的另一篇博文照做即可
[http://blog.csdn.net/scut1135/article/details/6090002](http://blog.csdn.net/scut1135/article/details/6090002)     【不行啦！！！candeonel不再维护java6啦～～～】
用以下方法：【2012.05.25】[https://help.ubuntu.com/community/Java](https://help.ubuntu.com/community/Java)
- oracle java 6 jdk 32-bit 
 $ wget http://download.oracle.com/otn-pub/java/jdk/6u31-b04/jdk-6u31-linux-i586.bin
 $ chmod u+x jdk-6u31-linux-i586.bin
 $ ./jdk-6u31-linux-i586.bin
 $ sudo mv jdk1.6.0_31 /usr/lib/jvm/
 $ sudo update-alternatives --install "/usr/bin/java" "java" "/usr/lib/jvm/jdk1.6.0_31/bin/java" 1
 //$ sudo update-alternatives --install "/usr/lib/mozilla/plugins/libjavaplugin.so" "mozilla-javaplugin.so" "/usr/lib/jvm/jdk1.6.0_31/jre/lib/i386/libnpjp2.so" 1- 
**IMPORTANT** choose the java you installed as default   设置默认jdk
 $ sudo update-alternatives --config java
 //$ sudo update-alternatives --config mozilla-javaplugin.so
+++++++++++++++++++++
采用方法1.
设置环境变量
**run_java_env_1.6.sh#!/bin/shexport JAVA_HOME=/usr/lib/jvm/java-6-sun-1.6.0.26export PATH=$JAVA_HOME/bin:$PATHexport CLASSPATH=$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jarecho "---- JAVA_HOME is ----"echo $JAVA_HOMEecho "---- end ----"**
**设置环境变量的具体方法参见[http://www.blogjava.net/leekiang/archive/2010/12/30/341946.html](http://www.blogjava.net/leekiang/archive/2010/12/30/341946.html)**
解决方案：采用文中的方法2：修改.bashrc文件
如果你需要给某个用户权限使用这些环境变量，你只需要修改其个人用户主目录下的.bashrc就可以了,而不像第一种方法给所有用户权限。
<1>在 shell终端执行命令：gedit /home/<username>/.bashrc
<2>在.bashrc文件末尾加入：
**export JAVA_HOME=/usr/lib/jvm/java-6-sun-1.6.0.26export PATH=$JAVA_HOME/bin:$PATHexport CLASSPATH=$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar**
**<3>重新登录**
Or
sudo vi /etc/environment
 修改内容为：
- PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/home/ubuntu/jdk1.6.0_37/bin"  
- CLASSPATH=.:/home/ubuntu/jdk1.6.0_37/lib  
- JAVA_HOME=/home/ubuntu/jdk1.6.0_37     
5、source /etc/environment
6、java -version
2. 安装 myeclipse 8.6 
### [Grant ：Ubuntu下MyEclipse 8.5 8.6下载与 安装](http://blog.csdn.net/scut1135/article/details/6082104)
 注意！安装是要注意权限
更改myeclipse-installer的权限：
允许以程序执行文件 打勾
**安装后要检查更新java6的版本和位置    Windows-->Preferences-->Java--> Installed JREs --> java-6-sun-1.6.0.26/jre  (手动找到安装目录 或者 利用search)**
**MyEclipse 8.6.1 注册码： windows-->Preferences-->MyEclipse-->Subscription**
**Subscriber: oyksoftSubscription Code: bLR8ZC-855444-6867865049821387**
3. 在MyEclipse 8.6上搭建Android开发环境
Ubuntu 下 分两种情况：
如果安装的是**Eclipse**
**ADT插件下载**
使用Update Manager里的Eclipse安装功能，把ADT的最新版本安装在你的电脑上。
|Eclipse 3.5 (Galileo) and 3.6 (Helios)|Eclipse 3.4 (Ganymede)|
|----|----|
|- Start Eclipse, then select Help > Install New Software....- Click Add, in the top-right corner.- In the Add Repository dialog that appears, enter "ADT Plugin" for the Name and the following URL for theLocation:[https://dl-ssl.google.com/android/eclipse/](https://dl-ssl.google.com/android/eclipse/)Note: If you have trouble acquiring the plugin, try using "http" in the Location URL, instead of "https" (https is preferred for security reasons).Click OK.- In the Available Software dialog, select the checkbox next to Developer Tools and click Next.- In the next window, you'll see a list of the tools to be downloaded. Click Next.- Read and accept the license agreements, then clickFinish.- When the installation completes, restart Eclipse.|- Start Eclipse, then select Help > Software Updates.... In the dialog that appears, click the Available Softwaretab.- Click Add Site.- In the Add Site dialog that appears, enter this URL in the "Location" field:[https://dl-ssl.google.com/android/eclipse/](https://dl-ssl.google.com/android/eclipse/)Note: If you have trouble acquiring the plugin, you can try using "http" in the URL, instead of "https" (https is preferred for security reasons).Click OK.- Back in the Available Software view, you should see the plugin listed by the URL, with "Developer Tools" nested within it. Select the checkbox next to Developer Tools, which will automatically select the nested tools. Then click Install- On the subsequent Install window, all of the included tools should be checked. Click Next.- Read and accept the license agreements, then clickFinish.- When the installation completes, restart Eclipse.|
安装兼容版本的Eclipse IDE的，请查看这个[地址](http://developer.android.com/sdk/eclipse-adt.html#preparing)。完成这些之后请按照下列步骤下载ADT插件并安装在你的Eclipse环境中。
如果安装的是**MyEclipse**(速度奇慢无比。。。每一步都需要等很长时间，装个ADT(sdk tools为Revision 12，对应ADT也为12.0.0 参见我的系列好文 http://blog.csdn.net/scut1135/article/details/6091408)总共接近2个小时，要有耐心）
**给MyEclipse安装ADT插件**
打开eclipse的help菜单->MyEclipse Configuration Center
![](http://hi.csdn.net/attachment/201011/9/0_12892682550aQ1.gif)
**打开eclipse的help菜单**
进入后点击 Software 标签
![](http://hi.csdn.net/attachment/201011/9/0_1289268313sbAA.gif)
**Software 标签**
在Browser Software后面的 add site
![](http://hi.csdn.net/attachment/201011/9/0_1289268367Bmkc.gif)
**Browser Software**
在对话框中填写如图所示：（name：自己取     url：https://dl-ssl.google.com/android/eclipse/   ）
![](http://hi.csdn.net/attachment/201011/9/0_1289268409u1u0.gif)
** 在对话框中填写名称后点击ok**
然后在左边会出现 如下的
![](http://hi.csdn.net/attachment/201011/9/0_1289268483zv9y.gif)
**点击Add to Profile**
选中目标，右键点击Add to Profile，于是在右边的Software Updates Available 就会有所反应，
![](http://hi.csdn.net/attachment/201011/9/0_12892685123lp3.gif)
**Software Updates Available**
然后点击下面的Apply 2 changes 来开始安装
![](http://hi.csdn.net/attachment/201011/9/0_1289268588vS41.gif)
**点击下面的Apply 2 changes**
**![](http://hi.csdn.net/attachment/201011/9/0_1289268632gwgg.gif)**
** 开始安装**
点击接受下面的许可，继续。
![](http://hi.csdn.net/attachment/201011/9/0_1289268674FWfD.gif)
**点击接受 **
接着点击Update开始安装。**这个过程可能有点慢，耐心等待他的完成。**
![](http://hi.csdn.net/attachment/201011/9/0_1289268725Jjp5.gif)
**Update开始安装**
上面方法巨慢无比，基本不可取。可离线下载adt，解压后放入dropin . 详情如下：
[http://archive.cnblogs.com/a/2251000/](http://archive.cnblogs.com/a/2251000/)
最近发现许多人都在问 MyEclipse 搭建 ADT 的问题，自己也找到了解决方案，希望公布给大家尝试，但不保证完全没问题。
我曾经说到 8.5 / 8.6 搭建 ADT，一方面因为中国大陆 GFW 的关系，另一方面也因为 MyEclipse 的软件中心的确有问题。
详见：[http://www.littledai.com/20110204/232822/449](http://www.littledai.com/20110204/232822/449)
当然，自己下载的 **ADT 压缩包内我们只需要 plugins 和 features 两个文件夹**即可。
将**这两个文件夹装入 ADT 文件夹（自己建立）中。然后将 ADT 文件夹装入 MyEclipse 8.6 安装目录下的 dropins 文件夹。**
先不要急着启动 MyEclipse，接着我们要进入和 dropins 同级的 configuration 文件夹，删除 org.eclipse.update 文件夹。
这步很重要！
然后我们就可以进入 MyEclipse 选择 ADT 了。
方法其实很简单，在早期版本中我们把 plugins 和 features 覆盖 MyEclipse 自身的文件夹即可，而 8.x 版本则不接受覆盖的方式了。
但它提供了 dropins 文件夹来存放自定义插件（9.0 没有，所以具体方法待研究）。
而关键的删除 org.eclipse.update 的操作是方式 MyEclipse 连接互联网来更新我们自定义的插件，一旦检查更新后我们的自定义插件就会失效了。
**5，配置MyEclipse**
点击window –>preferences
![](http://hi.csdn.net/attachment/201011/9/0_1289268934k8PP.gif)
**点击window –>preferences**
在Android选项，填写好SDK location，点击Apply后出现以下内容。
![](http://hi.csdn.net/attachment/201011/9/0_1289268964jzzO.gif)
**点击Apply后出现的内容**
**Linux下参考：**
【重点提醒】 sdk 环境变量。
Android SDK中的tools绝对路径（windows 本机为D:\AndroidDevelop\**android-sdk-windows\tools**，linux下类似）。
**运行 android –h 如果有类似以下的输出，表明安装成功：**
ubuntu设置 path环境变量(详细)
1、Ubuntu专有方式
编辑 /etc/ld.so.conf 文件，如果以下语句不存在，则加入：
include /etc/ld.so.conf.d/*.conf
然后在/etc/ld.so.conf.d下边新建一个以 .conf 结尾的文件。
在新建的 .conf 文件中写入需要设置的 path，例如：
~/mypath/bin
2、用户目录下的 .bashrc 文件
在用户主目录下，有一个 .bashrc 文件，编辑该文件：
$gedit ~/.bashrc 
在最后边加入需要设置变量的shell语句，例如：
export PATH=~/mypath/bin:$PATH
该文件编辑保存后，可立即在新打开的终端窗口内生效。
该方式添加的变量只能当前用户使用。
3、系统目录下的 profile 文件
在系统的 etc 目录下，有一个 profile 文件，编辑该文件：
$gedit /etc/profile
在最后边加入需要设置变量的shell语句，例如：
export PATH=~/mypath/bin:$PATH
该文件编辑保存后，重启系统，变量生效。
该方式添加的变量对所有的用户都有效。
**4、系统目录下的 environment 文件【经验证】在系统的 etc 目录下，有一个 environment 文件，编辑该文件：$gedit /etc/environment找到以下的 PATH 变量：PATH="<......>"修改该 PATH 变量，在其中加入自己的path即可，例如：PATH="~/mypath/bin:<......>"各个path之间用冒号分割。该文件也是重启生效，影响所有用户。**
## [【校内网】Android Java开发环境的建立（基于最新的ADT 10.0.0）](http://xiaozu.renren.com/xiaozu/196276/332894682)
**Windows下参考：**
# [1. 图文并茂 在MyEclipse 8.6上搭建Android开发环境](http://mobile.51cto.com/android-222812.htm)
### [2.   在Windows平台的MyEclipse 8.6(Eclipse)上搭建Android开发环境](http://blog.csdn.net/lyonte/article/details/5996902)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
【Import程序时遇到诡异问题】
估计是以前安装的eclipse没有自带jre，而这次安装的myeclipse里面集成了jre1.6.0_13这个版本。而一开始安装myeclipse之前未设置自己单独安装的jdk1.6.0.26的环境变量导致，使用了myeclipse自带的版本，但这个版本的一些keytool在我这个ububtu默认下bin目录居然：
**""默认 .deb文件或者keytool这类可执行程序的  属性-->Permissions-->Allow executing file as program" 没有打上勾！！！要去提示的目录去打开这个选项！！！**
### 查看debug.keystore的保存路径
单击Eclipse的【Window】>【Preferences】菜单项，打开【Preferences】对话框，在左侧找到Android节点，单击【Build】子节点，在右侧的【Build】设置页中的【Default debug keystore】文本框的值就是debug.keystore文件的路径。
解决办法： 
linux：Window－－》Preferences－－》Android－－》Build中Default debug keystore显示了地址“/home/jinli/.android/debug.keystore”，删除此路径下的debug.keystore及 ddms.cfg
Window XP：进入：C:\Documents and Settings\Administrator\.android 
删除：debug.keystore及 ddms.cfg
