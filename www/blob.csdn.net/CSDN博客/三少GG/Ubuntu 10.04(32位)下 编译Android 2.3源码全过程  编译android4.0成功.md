# Ubuntu 10.04(32位)下 编译Android 2.3源码全过程 || 编译android4.0成功 - 三少GG - CSDN博客
2011年04月26日 12:34:00[三少GG](https://me.csdn.net/scut1135)阅读数：4034
Tips:
1. 怎么把ubuntu中隐藏文件显示出来呢**    ctrl+H or**ls -la
**2.**
# [ubuntu编译android4.0成功](http://bbs.imp3.net/thread-10502182-1-1.html)
ndroid 4.0发布之后的几天就想编译出来体验一下ICS，事情多得一直拖到今天，另外一些误导人的传闻越来越凶，说什么编译环境最低要求memory为16G。
今天就拿实验室的机器跑了一通，编译无不良反映，一切正常。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
使用git和repo下载Android的源代码
http://www.cnblogs.com/wanlipeng/archive/2010/08/26/1809215.html
以下操作都是在Ubuntu10.04LTS下完成：
   1. 安装git
      sudo apt - get install git - core
   2. 安装curl    sudo apt-get install curl
   3. 安装Repo，为了方便直接下载repo到用户根目录中。通过curl下载repo
      curl http: // android.git.kernel.org / repo > ~ / rep o
   4. 给repo可执行的权限
      chmod a + x ~/repo
   5. 新建一个目录，然后进入该目录。通过repo将当前Android上所有源代码下载。
    * 首先初始化本地，
      ~ / repo init - u git: // android.git.kernel.org / platform / manifest.git  看到repo initialized in /android 的提示后，则证明初始化完毕。
    * 执行   repo sync 则自动开始下载源代码。
通过以上方法可以下载到源代码，只是源代码比较大，如果网速不快的话，不建议这么做。而且下载过程中并不能看到下载的文件，
需要所有的都下载完毕之后才能看到。这点有点不尽人意，而且git支持断点续传，如果中断了也不用担心。
更多信息请参考：
[http://source.android.com/source/git-repo.html](http://source.android.com/source/git-repo.html)
[http://android.git.kernel.org/](http://android.git.kernel.org/)
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++[](http://www.eoeandroid.com/thread-853-1-1.html)
[Git和Repo扫盲——如何取得Android源代码(转)](http://www.eoeandroid.com/thread-853-1-1.html)
Git 是 Linux Torvalds 为了帮助管理 Linux内核开发而开发的一个开放源码的分布式版本控制软件，它不同于Subversion、CVS这样的集中式版本控制系统。在集中式版本控制系统中只有一个仓库（repository），许多个工作目录（working copy），而像Git这样的分布式版本控制系统中（其他主要的分布式版本控制系统还有[BitKeeper](http://www.bitkeeper.com/) 、[Mercurial](http://www.selenic.com/mercurial)
 、[GNU Arch](http://www.gnu.org/software/gnu-arch/) 、[Bazaar](http://bazaar-vcs.org/) 、[Darcs](http://darcs.net/) 、[SVK](http://svk.bestpractical.com/view/HomePage)
 、[Monotone](http://monotone.ca/) 等），每一个工作目录都包含一个完整仓库，**它们可以支持离线工作，本地提交可以稍后提交到服务器上。分布式系统理论上也比集中式的单服务器系统更健壮，单服务器系统一旦服务器出现问题整个系统就不能运行了，分布式系统通常不会因为一两个节点而受到影响** 。因为Android是由kernel、Dalvik、Bionic、prebuilt、build等多个Git项目组成，所以Android项目编写了一个名为Repo的Python的脚本来统一管理这些项目的仓库，使得Git的使用更加简单。
这几天William为了拿Android最新的sourcecode，学习了一下git和repo的一些基本操作，整理了一个如何取得Android代码的How-To，今天把他贴上来。
**1、Git的安装**
在Ubuntu 8.04上安装git只要设定了正确的更新源，然后使用apt-get就可以了，有什么依赖问题，就让它自己解决吧。其中cURL是一个利用URL语法在命令行下工作的文件传输工具，会在后面安装Repo的时候用到。
**sudo apt-get install git-core curl 2、安装Repo**
首先确保在当前用户的主目录下创建一个/bin目录（如果没有的话），然后把它(~/bin)加到PATH环境变量中
接下来通过cURL来下载Repo脚本，保存到~/bin/repo文件中
**curl [http://android.git.kernel.org/repo](http://android.git.kernel.org/repo) >~/bin/repo**
别忘了给repo可执行权限
**chmod a+x ~/bin/repo 3、初始化版本库**
如果是想把Android当前主线上最新版本的所有的sourcecode拿下来，我们需要repo的帮助。
先建立一个目录，比如~/android，进去以后用repo init命令即可。
**repo init -u git://android.git.kernel.org/platform/manifest.git**
这个过程会持续很长的时间（至少可以好好睡一觉），具体要多少时间就取决于网络条件了
最后会看到 repo initialized in /android这样的提示，就说明本地的版本库已经初始化完毕，并且包含了当前最新的sourcecode。
如果想拿某个branch而不是主线上的代码，我们需要用-b参数制定branch名字，比如：
**repo init -u git://android.git.kernel.org/platform/manifest.git -b cupcake**
另一种情况是，我们只需要某一个project的代码，比如kernel/common，就不需要repo了，直接用Git即可。
**git clone git://android.git.kernel.org/kernel/common.git**
这也需要不少的时间，因为它会把整个Linux Kernel的代码复制下来。
如果需要某个branch的代码，用git checkout即可。比如我们刚刚拿了kernel/common.get的代码，那就先进入到common目录，然后用下面的命令：
**git checkout origin/android-goldfish-2.6.27 -b goldfish**
这样我们就在本地建立了一个名为goldfish的android-goldfish-2.6.27分支，代码则已经与android-goldgish-2.6.27同步。我们可以通过git branch来列出本地的所有分支。
**4、同步版本库**
使用**repo sync** 命令，我们把整个Android代码树做同步到本地，同样，我们可以用类似
**repo sync project1 project2 … **
这样的命令来同步某几个项目
如果是同步Android中的单个项目，只要在项目目录下执行简单的
**git pull**
即可。
**5、通过GitWeb下载代码**
另外，如果只是需要主线上某个项目的代码，也可以通过[GitWeb](http://android.git.kernel.org/) 下载，在shortlog利用关键字来搜索特定的版本，或者找几个比较新的tag来下载还是很容易的。
Git最初是为Linux内核开发而设计，所以对其他平台的支持并不好，尤其是Windows平台，必须要有Cygwin才可以。现在，得益于[msysgit](http://code.google.com/p/msysgit/) 项目，我们已经可以不需要Cygwin而使用Git了。另外，[Git Extensions](http://sourceforge.net/projects/gitextensions/)
 是一个非常好用的Windows Shell扩展，它能与资源管理器紧密集成，甚至提供了Visual Studio插件。它的官方网站上有一分不错的[说明文档](http://sourceforge.net/project/showfiles.php?group_id=246547&package_id=300816&release_id=674434) ，感兴趣的朋友可以看一看。
至于Git的参考文档，我推荐[Git Magic](http://www-cs-students.stanford.edu/~blynn/gitmagic/) ，这里还有一个[Git Magic的中文版](http://docs.google.com/View?id=dfwthj68_675gz3bw8kj#__07735763982479649) 。
转自：[http://www.williamhua.com/2009/04/29/git-and-repo-for-dummies/](http://www.williamhua.com/2009/04/29/git-and-repo-for-dummies/)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Ubuntu 8.04下编译Android源码全过程
[http://blog.csdn.net/dongfengsun/archive/2009/10/16/4679902.aspx](http://blog.csdn.net/dongfengsun/archive/2009/10/16/4679902.aspx)
一、获取
Android源代码
Git是Linux Torvalds（Linux之父）为了帮助管理
Linux内核开发而开发的一个开放源码的分布式版本控制软件，它不同于
Subversion、
CVS这样的集中式版本控制系统。在集中式版本控制系统中只有一个仓库（
Repository），许多个工作目录（
Working Copy），而在
Git这样的分布式版本控制系统中（其他主要的分布式版本控制系统还有
BitKeeper、
Mercurial、
GNU Arch、
Bazaar、
Darcs、
SVK、
Monotone等），每一个工作目录都包含一个完整仓库，它们支持离线工作，本地提交可以稍后提交到服务器上。
因为Android
是由Kernel
、Dalvik
、Bionic
、Prebuilt
、build
等多个项目组成，如果我们分别使用Git
来逐个获取显得很麻烦，所以Android
项目编写了一个名为Repo
的Python
的脚本来统一管理这些项目的仓库，使得项目的获取更加简单。
在Ubuntu 8.04
上安装Git
只需要设定正确的更新源，然后使用apt-get
就可以了，apt-get
是一条Linux
命令，主要用于自动从互联网的软件仓库中搜索、安装、升级、卸载软件或操作系统。
apt-get命令一般需要
root权限执行，所以一般跟着
sudo命令。
sudo apt-get install git-core curl
这条命令会从互联网的软件仓库中安装git-core
和curl
。
其中curl
是一个利用URL
语法在命令行方式下工作的文件传输工具，它支持很多协议，包括FTP、FTPS、HTTP、HTTPS、TELENT等，我们需要安装它从网络上获取
Repo脚本文件。
curl [http://android.git.kernel.org/repo](http://android.git.kernel.org/repo)>~/bin/repo
这句命令会下载repo
脚本文件到当前主目录的/bin
目录下，并保存在文件repo
中。
最后我们需要给repo
文件可执行权限
chmod a+x ~/bin/repo
接下来我们就可以利用repo
脚本和Git
、curl
软件获取Android
的源代码了：）
首先建一个目录，比如~/android
。
然后使用下面命令获取源码：
repo init –u git://android.git.kernel.org/platform/manifest.git
这个过程会持续很长的时间（笔者下载了一天），下载完毕后会看到repo initialized in /android
这样的提示，说明本地的版本库已经初始化完毕，并且包含了当前最新的
sourcecode。
如果我们想拿某个分支版本的代码，而不是主线代码，需要使用-b
参数指定branch
的名字，比如：
repo init –u git://android.git.kernel.org/platform/manifest.git –b cupcake
如果我们只是想获取某一个project
的代码，比如kernel/common
，就不需要repo
脚本了，直接使用Git
工具即可，如果仔细研究repo
脚本会发现，repo
脚本其实就是组织Git
工具去获取各个Project
并把它们组织到同一个项目Android
内。
git clone git://android.git.kernel.org/kernel/common.git
我们上面使用repo
脚本获取了各个项目，那么接下来就需要把整个Android代码树同步到本地，如下：
repo sync project1 project2 …
笔者使用repo sync
命令直接同步全部项目到本地。
二、源码编译
**1.**全部同步完毕后，进入到Android目录下，使用
make命令编译，你会发现出现如下错误提示：
host C: libneo_cgi <= external/clearsilver/cgi/cgi.c
external/clearsilver/cgi/cgi.c:22:18: error: zlib.h: No such file or directory
这个错误是因为我们缺少zlib1 g-dev，需要使用
apt-get命令从软件仓库中安装，如下：
sudo apt-get install zlib1 g-dev
同理，我们还需要依次安装如下软件
sudo apt-get install flex
sudo apt-get install bison
sudo apt-get install gperf
sudo apt-get install libsdl-dev
sudo apt-get install libesd0 -dev
sudo apt-get install libncurses5-dev
sudo apt-get install libx11 -dev
以上软件全部安装完毕后**(转3)**，运行make
命令再次编译Android
源码.(这是个漫长的过程，耗时1小时20分钟 看机器）
这个时候你会发现出现很多java
文件无法编译的错误，打开Android
的源码我们可以看到在android/dalvik/libcore/dom/src/test/java/org/w3c/domts下有很多
java源文件，这意味着编译
Android之前需要先安装
JDK。
首先从sun
官方网站下载jdk-6u16-linux-i586.bin文件并安装它。
在Ubuntu 8.04
中，/etc/profile
文件是全局的环境变量配置文件，它适用于所有的shell。在我们登陆
Linux系统时，首先启动
/etc/profile文件，然后再启动用户目录下的
~/.bash_profile、
~/.bash_login或
~/.profile文件中的其中一个，执行的顺序和上面的排序一样。如果
~/.bash_profile文件存在的话，一般还会执行
~/.bashrc文件。
所以我们只需要把
JDK的目录放到
/etc/profile里即可，如下：
JAVA_HOME=/usr/local/src/jdk1.6.0_16
PATH=$PATH:$JAVA_HOME/bin:/usr/local/src/android-sdk-linux_x86-1.1_r1/tools:~/bin
然后重新启动一下机器，输入java –version
命令，提示如下信息代表配置成功：
java version "1.6.0_16"
Java(TM) SE Runtime Environment (build 1.6.0_16-b01)
Java HotSpot(TM) Client VM (build 14.2-b01, mixed mode, sharing)
       在**编译完整个项目后，终端会出现如下提示：**
Target system fs image: out/target/product/generic/obj/PACKAGING/systemimage_unopt_intermediates/system.img
Install system fs image: out/target/product/generic/system.img
Target ram disk: out/target/product/generic/ramdisk.img
Target userdata fs image: out/target/product/generic/userdata.img
Installed file list: out/target/product/generic/installed-files.txt
[root@dfsun2009-desktop:/bin/android](mailto:root@dfsun2009-desktop:/bin/android)# 
三、源码运行**4.**
在编译完整个项目后，如果我们需要观看编译后的运行效果，那么就需要在系统中安装模拟器android-sdk-linux_x86-1.1_r1
，这个SDK
的下载地址为：(注：之前做开发时已安装过,  ./emulator)  **转5**
linux:
[http://dl.google.com/android/android-sdk-linux_x86-1.1_r1.zip](http://dl.google.com/android/android-sdk-linux_x86-1.1_r1.zip)
mac: [http://dl.google.com/android/android-sdk-mac_x86-1.1_r1.zip](http://dl.google.com/android/android-sdk-mac_x86-1.1_r1.zip)
windows: [http://dl.google.com/android/android-sdk-windows-1.1_r1.zip](http://dl.google.com/android/android-sdk-windows-1.1_r1.zip)
解压后需要把/usr/local/src/android-sdk-linux_x86-1.1_r1/tools目录加入到系统环境变量
/etc/profile中。
然后找到编译后android
的目录文件out
，我们发现在android/out/host/linux-x86/bin下多了很多应用程序，这些应用程序就是
android得以运行的基础，所以我们需要把这个目录也添加到系统
PATH下，在
$HOME/.profile文件中加入如下内容：
PATH=”$PATH:$HOME/android/out/host/linux-x86/bin”
接下来我们需要把android
的镜像文件加载到emulator
中，使得emulator
可以看到android
运行的实际效果，在$HOME/.profile
文件中加入如下内容：
ANDROID_PRODUCT_OUT=$HOME/android/out/target/product/generic
export ANDROID_PRODUCT_OUT
然后重新启动机器。
下面就可以进入到模拟器目录中并启动模拟器
cd $HOME/android/out/target/product/generic
emulator -image system.img -data userdata.img -ramdisk ramdisk.img
总结一下安装过程中的关键点：
1：JDK版本必须安装
2：利用下面的命令确保所需软件都被正确安装
sudo apt-get install flex bison gperf libsdl-dev libesd0-dev libwxgtk2.6-dev build-essential python valgrind curl git
3:内存及虚拟内存保证在2GB以上，可以采用命令行free -m查看内存是否足够，如果内存不够终端停滞
附注：+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**2.**
在使用：
$ repo init -u git://[Android](http://www.linuxidc.com/topicnews.aspx?tid=11) .git.kernel.org/platform/manifest.git
$ repo sync
下载完代码后，进行make，
$cd ~/mydroid
$make
却出现了如下错误：
build/core/main.mk:73: You are attempting to build on a 32-bit system.
build/core/main.mk:74: Only 64-bit build environments are supported beyond froyo/2.2.
这是因为froyo/2.2默认只支持64-bit， 看到有些网友还要去下载64-比他的操作系统，很是麻烦，于是通过不断搜索资料终于解决，
解决办法：
需要进行如下修改即可，
将
./external/clearsilver/cgi/Android.mk 
./external/clearsilver/java-jni/Android.mk 
./external/clearsilver/util/Android.mk 
./external/clearsilver/cs/Android.mk
四个文件中的
LOCAL_CFLAGS += -m64 
LOCAL_LDFLAGS += -m64 
注释掉，或者将“64”换成“32”
LOCAL_CFLAGS += -m32 
LOCAL_LDFLAGS += -m32 
采用注释的方法即可(grant):
# This forces a 64-bit build for Java6
#LOCAL_CFLAGS += -m64
#LOCAL_LDFLAGS += -m64
然后，将
./build/core/main.mk 中的
ifneq (64,$(findstring 64,$(build_arch))) 
改为：
ifneq (i686,$(findstring i686,$(build_arch))) 
OK！问题解决
//转贴牛人的。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**3.**
sudo apt-get install gnupg flex bison gperf libsdl-dev libesd0-dev libwxgtk2.6-dev build-essential zip  libncurses5-dev zlib1g-dev lib64z1-dev 
libc6-dev-amd64 g++-multilib lib64stdc++6
注意：1. zlib1
   g-dev lib64z1
-dev
   红色字体为数字1，不是字母l     2.获取上面的编译过程所需软件需要花费大概40分钟的时间（看网速）
获取Android 2.3.1 代码：如果我们想拿某个分支版本的代码，而不是主线代码，需要使用-b参数指定branch的名字
eg:mkdir mydroid
cd      mydroid
repo init -u git://android.git.kernel.org/platform/manifest.git **-b android-2.3.1_r1**
repo sync
**5.**
将源码编译完后下一步的动作就是要看看效果了，怎样启动模拟器是一个问题。网上有很多种做法，下面介绍一种简单的方式进行处理。
在下载源码的目录下，make sdk命令，编译sdk。而后在/home/stone/working-directory-name/out/host/linux-x86多一个 sdk目录。在sdk的 tools目录下使用命令创建虚拟设备，此时创建方式和1.5有所不同其命令是：
./android create avd -n sphone-t android-7
2.1版本的要求其 targetid 必须是 android-7这种模式。
而后启动模拟器：
命令是：
./emulator -avd sphone
至此关于android源码下载、编译、模拟器运行的基本知识介绍完毕。下步就是深入分析其源码。
在这个过程中下载源码大约下了8个小时，编译源码用了3个小时，编译sdk用时20分钟。就是这样等待中见证欣喜。
并且**参考：http://www.61ic.com/Mobile/Android/201104/32906.html**
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
在工作目录下运行make sdk即可
运行模拟器
在~/bin/mydroid/out/target/product/generic/ 目录下运行命令： 
emulator -image system.img -data userdata.img -ramdisk ramdisk.img 
再有错误就做如下操作 
export   ANDROID_PRODUCT_OUT=/home/fgxue/bin/mydroid/out/target/product/generic 
~/bin/mydroid/out/host/linux-x86/bin/emulator -image system.img -data userdata.img  -ramdisk ramdisk.img  
[(http://www.cnmsdn.com/html/201007/1279224676ID6856_2.html)](http://www.cnmsdn.com/html/201007/1279224676ID6856_2.html)
