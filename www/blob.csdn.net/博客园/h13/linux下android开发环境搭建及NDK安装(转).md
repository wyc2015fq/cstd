# linux下android开发环境搭建及NDK安装(转) - h13 - 博客园
对于Linux我也是一位新人，弄Linux也就半年左右吧，同时作为Android队伍中的一个新人。
由于我最近每天使用的是CentOS 5.5,所以选择CentOS5.5作为我的开发环境。
我以前一直在LinuxSir潜水，第一次发贴，我还是细致的把我作的写下来。同时作为我的学习笔记我会把我的学习成果发上来的。
作为新人，如果有什么不对的地方，还望各位不吝赐教，多多关照～
目录
1、环境搭建
　　1.1、JDK安装
　　1.2、Eclipse安装
　　1.3、ADT安装
　　1.4、Android SDK安装
1.5、Android NDK安装
内容
1.1、JDK安装
没选用CentOS自带的Java环境，下载Oracle最新版本的java,下载地址为http://www.oracle.com
/technetwork/ja...sp-136632.html 
直接Download，然后跳转页面选择Platform 
中的Linux下载，我没有注册，直接点Continue进入下一个页面，选择你要下载的包的类型，我选择
jdk-6u21-linux-i586-rpm.bin 网速快的话一会儿就下完了。
下载完成后修改可执行权限＃chmod +x jdk-6u21-linux-i586-rpm.bin 
然后执行它
#./jdk-6u1-linux-i586-rpm.bin
过程中可能出现一些选择，直接yes
安装完成后配置环境变量程序默认安装到了 /usr/java 下面
注意：在有多项时，windows下是用 分号；隔开的，但linux 下使用 冒号：
＃vim /etc/profile
在文件的最后面添加下面内容
export JAVA_HOME=/usr/java/jdk1.6.0_21 
export JAVA_BIN=/usr/java/jdk1.6.0_21/bin 
export PATH=$PATH:$JAVA_HOME/bin 
export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar 
export JAVA_HOME JAVA_BIN PATH CLASSPATH 
完成以后查看java版本
＃java -version 
Java(TM) SE Runtime Environment (build 1.6.0_21-b06) 
Java HotSpot(TM) Server VM (build 17.0-b16, mixed mode) 
Java环境配置好了下面继续安装IDE，Google推荐用eclipse.
　1.2、Eclipse安装
我也是新人当然一切最新，eclipse也是下载最新版本Eclipse (Helios版）。Eclipse下载
http://www.eclipse.org/downloads/
Eclipse IDE for Java Developers, 98 MB Linux 32 Bit
有过java使用开发经验的人基本知道，eclipse是绿色版的，解压就能使用。双击解压完成后eclipse文件夹中的eclipse文件就
运行eclipse IDE for Java Dvelopers。很熟悉吧，设置workspace，后面是欢迎界面关掉就进去开发环境了。
我把Eclipse放到/home/Android/下，以后我就把需要的东西都放到其下。
1.3、ADT安装
ADT 版本：
ADT 0.9.7 (May 2010) Android开发工具
（1）使用 Eclipse 的升级管理器来安装ADT：
1. 启动 Eclipse, 然后进入 Help > Install New Software.
2. 在 Available Software 对话框里，点击 Add....
3. 出现 Add Site 对话框，在 Name 域里面输入一个名字 (例如, "Android Plugin") ，在 "Location"
 域里面输入 URL: https://dl-ssl.google.com/android/eclipse/
4. 注意：如果有问题，可以把 https 换成 http 试一下。点击 OK.
5. 回到 Available Software 界面，你应该看到 "Developer Tools" 。选取 checkbox 。点击 
Next, 接受 license agreement， 然后点击 Finish。
6. 重起 Eclipse.
（2）手动安装ADT，下载ADT。   ADT 0.9.7  
http://dl.google.com/android/ADT-0.9.7.zip
下载完成后解压，把对应文件拷贝到eclipse相应目录即可。. 
同样也可以像上面那样把Help > Install New Software.安装的地址指向解压后ADT的文件夹。
1.4、Android SDK安装
下载Android SDK 核心包
for Linux版本 http://dl-ssl.google.com/android/rep..._r01-linux.zip
需要知道的是这个 zip 包并不是一个完整的软件包。Android 
软件包采用“组件”的形式，用户可以根据需要选取组件。上述zip文件只包含了一个组件管理工具和一个基本的工具组件。
.
下面我们就配置上面已经安装的ADT
配置 Eclipse 里的 ADT,使其指向 Android SDK 。
1. 选取 Eclipse Window > Preferences... 
2. 选择 Android
3. 点击 Browse... 定位 Android SDK 
目录。例如我的目录：/home/Android/android-sdk-linux_86
4. 点击 Apply, 然后 OK.
开发环境建基本搭建立好了，但是我们还是升级下载一些东西，把完整的SDK都下载完。里面有许多更新包，包含了Doc文档，Samples示例代码，以及
SDK文件。整个过程会自动下载安装:
1.运行 Eclipse, 选取 Window > Android SDK and AVD Manager. 
2.在左侧面板选择 Installed Packages
3.点击 Update All. 
出现 Choose Packages to Install 对话，选择Accept All，点击Install
大概1.4GB的容量。网没问题的话，一晚上绝对完事。
　　4、创建AVD
　　为使Android应用程序可以在模拟器上运行，必须创建AVD。
　　(1)、在Eclipse中。选择Windows > Android SDK and AVD Manager
　　(2)、点击左侧面板的Virtual Devices，再右侧点击New
　　(3)、填入Name，选择Target的API，SD Card大小任意，Skin随便选，Hardware目前保持默认值
(4)、点击Create AVD即可完成创建AVD
看了一些资料，现在开发都基于1.6开发。以后只需在版本编译就可，我没试过～
1.5、Android NDK安装
一些Java开发者摒弃了像C一类的过程化语言，相比起Android的开源项目而言，C世界已经积累了很多可供复用的开源项目（包括很多著名的游戏如
Quake、DOOM），在执行效率上涉及到大量数学运算的部分也可以考虑由C完成。
考虑开发C/C++程序	可能需集成Eclipse CDT
CDT，就是Eclipse的C/C++环境，在线安装地址 
http://download.eclipse.org/tools/cdt/releases/galileo 。
也可以下载完成后手动安装，就像我上面介绍的那样手动安装
1启动 Eclipse, 然后进入 Help > Install New Software.2在 Available Software 
对话框里，点击 Add....
3.添加解压后文件夹地址->local 选择好后就ok.然后选择安装的需要的内容.
我运行一个NDK的例子工程后发现，CDT并不是必须的,估计以后可能会用到，或者集成QT开发环境会用到吧.
NDK 下载地址:http://dl.google.com/android/ndk/and...-linux-x86.zip
NDK的安装很简单,就是将下载的包进行解压到指定位置即可,解压之后我们一般需要配置环境变量,这里我们将环境变量的配置放在最后一起来配置.
解压位置：/home/Android/android-NDK-r4b
#vim /etc/profile
#set NDK env
NDKROOT=/home/Android/android-ndk-r4b
export PATH=$NDKROOT:$PATH
NDK 编译时还需配置一些，以HelloJin. 很经典的Hello～创建Android 工程导入以存在的source.
打开hello-jni工程属性，选择builder，选择program，点OK
配置如下，NDK_Builder，location和working directory
/usr/bin/bash和/usr/bin
arguments要和你的工程目录一致。
我的arguments是：--login -c "cd 
/home/Android/android-ndk-r4b/samples/hello-jni && 
$NDKROOT/ndk-build"
完成后就可以运行.
  ﻿
http://blog.csdn.net/lupengcheng2025/archive/2011/02/16/6187522.aspx
