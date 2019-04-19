# 嵌入式Linux学习笔记（基于S5PV210 TQ210） - xqhrs232的专栏 - CSDN博客
2017年03月18日 23:57:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：262
个人分类：[S5PV210_Linux_Wince](https://blog.csdn.net/xqhrs232/article/category/6798766)
原文地址::[http://blog.csdn.net/zjhsucceed_329/article/details/32336019](http://blog.csdn.net/zjhsucceed_329/article/details/32336019)
相关文章
1、3_S5PV210开发环境搭建_Linux安装----[https://wenku.baidu.com/view/4e582fb6680203d8ce2f24fb.html](https://wenku.baidu.com/view/4e582fb6680203d8ce2f24fb.html)
基于S5PV210、TQ210平台。
本文更多的是教会大家如何学习！
包括如下内容：
1、前言
2、开发环境搭建
3、制作交叉编译器
4、裸机编程
4.1、汇编学习
4.2、S5PV210启动流程
4.3、点亮一个LED
4.4、串口
4.5、实现printf
4.6、时钟配置
4.7、重定位
4.8、DDR
4.9、NAND读写
4.11、LCD操作
5、移植u-boot（基于u-boot-2014.4版本）
5.1、概述
5.2、u-boot配置过程分析
5.3、u-boot编译过程分析
5.4、SPL
5.5、添加自己的单板
5.6、移植u-boot-spl.bin
5.7、移植u-boot.bin
5.7.1、u-boot.bin 内存布局分析
…………
6、移植内核
7、制作文件系统
8、驱动
9、综合应用
# 1 前言
       学习[嵌入式](http://lib.csdn.net/base/embeddeddevelopment)，首先需要学好[C语言](http://lib.csdn.net/base/c)，推荐大家看郝斌的C语言视频教程，不要问我视频在哪。这里引用一段网友的话：我在网上搜了一大堆C语言教学视频，可是都是讲得乱七八糟的，有的就是照本宣科，从不敲代码，把做好的PPT读完，那一专讲就没了。然而，郝老师讲的C语言太犀利了，简单明了不说，老师讲的每一点东西都还是用键盘敲出来的，若遇到了难点老师都会仔细的讲出分析思路，让你不接受，也得接受。想想大学里，老师真悲剧，讲的吐白沫，到头来没几个人真正的明白C语言这门课。
       另外，还需要熟悉[Linux](http://lib.csdn.net/base/linux)发行版的使用，推荐使用ubuntu，因为安装软件非常方便，网上资料也多。熟悉Linux系统下的基本命令操作，熟悉Makefile基本操作，推荐大家看《鸟哥的linux私房菜_基础学习篇（第三版）》和《gnu
 make中文手册》。
       本文以S5PV210为学习平台，使用TQ210开发板。
       个人QQ：809205580
       技术交流群：153530783
       个人博客：[http://blog.csdn.net/zjhsucceed_329](http://blog.csdn.net/zjhsucceed_329)
       本文所有源码、工具下载地址：[http://pan.baidu.com/s/1fEfG6](http://pan.baidu.com/s/1fEfG6)
# 2  开发环境搭建
       采用VMware-workstation虚拟机+linux系统的方式
首先下载VMware-workstation（[http://pan.baidu.com/s/1pJyk0gr](http://pan.baidu.com/s/1pJyk0gr)）， 按默认安装即可。
       然后是安装ubuntu系统，大家可以从 [http://pan.baidu.com/s/1eQziJh4](http://pan.baidu.com/s/1eQziJh4)下载ubuntu镜像文件，参照我的博客《ubuntu-10.10[嵌入式开发](http://lib.csdn.net/base/embeddeddevelopment)环境搭建》搭建ubuntu环境。
       或者从 [http://pan.baidu.com/s/1dDIhDg1](http://pan.baidu.com/s/1dDIhDg1)下载我制作好的ubuntu镜像文件
Ubuntu-10.10-x86-bk.rar，下载后解压到合适的位置，比如我自己的环境是：
![](https://img-blog.csdn.net/20140619174741750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后打开VMware-workstation
![](https://img-blog.csdn.net/20140619174813718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
先设置一下，选择Edit->Preferences
![](https://img-blog.csdn.net/20140619174843921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 设置虚拟机位置。
选择 View->Autofit Window，这样在VMware-workstation窗口大小改变时，ubuntu的大小会自动变换到合适的大小，前提是要安装VMware tools。
选择File->Open，定位找到刚才解压的文件夹，选中Ubuntu.vmx，然后打开。
![](https://img-blog.csdn.net/20140619174932765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140619175005796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后单击![](https://img-blog.csdn.net/20140619175101203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 即可启动ubuntu系统，出现对话框
![](https://img-blog.csdn.net/20140619175127718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
      选择I moved it
![](https://img-blog.csdn.net/20140619175158906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
用户名为root，密码为zjh329
![](https://img-blog.csdn.net/20140619175237640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果没有安装VMware tools，现在开始安装，选择VM-> Install VMware Tools，然后在ubuntu中执行如下命令：
root@zjh:~# cp  /media/VMware\ Tools/VMwareTools-*  .    拷贝到当前目录
root@zjh:~# tar  -zxvf  VMwareTools-*
root@zjh:~# cd  vmware-tools-distrib/
root@zjh:~/vmware-tools-distrib#./vmware-install.pl
接下来一路yes和回车直到安装完毕。
root@zjh:~# rm  -r  VMwareTools-*  vmware-tools-distrib/
重启系统，现在随意改变VMware-workstation窗口的大小，ubuntu的窗口大小会自动适应。
建立Windows到ubuntu的文件夹共享：
VM->Settings, 选择options， 选择Shared Folders，
![](https://img-blog.csdn.net/20140619175323453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
单击右下角的Add添加要共享的文件夹，选中上面的Always enabled，确定后重启ubuntu系统。然后就可以在ubuntu下访问Windows下的文件夹了。例如我共享的E盘：
root@zjh:~# ls /mnt/hgfs/E/
# 3  制作交叉编译工具链
## 3.1       下载工具crosstool-ng  [http://www.crosstool-ng.org/](http://www.crosstool-ng.org/)
## 3.2       安装crosstool-ng的软件依赖包
apt-get installbison flex texinfo automake libtool cvs libncurses5-dev aria2 g++ subversiongawk cvsd
## 3.3       编译安装crosstool-ng
由于crosstool-ng默认只能使用普通用户制作交叉编译器，若想使用root用户操作，可以修改crosstool-ng源码scripts/crosstool-NG.sh.in这个文件，其中有一行“# Check running as root”，在它上面添加CT_ALLOW_BUILD_AS_ROOT_SURE=true，这样就可以使用root操作了。
下面开始编译安装：
root@zjh:/home/work/crosstool-ng-1.19.0#./configure
root@zjh:/home/work/crosstool-ng-1.19.0#make && make install
## 3.4       现在开始制作交叉编译器
root@zjh:/home/work#mkdir crosstool             首先创建一个工作目录：
root@zjh:/home/work#cd crosstool
root@zjh:/home/work/crosstool#ct-ng list-samples        查看有哪些默认配置
其中以一个默认配置arm-cortex_a8-linux-gnueabi，我们就以这个为模板进行配置
root@zjh:/home/work/crosstool#ct-ng arm-cortex_a8-linux-gnueabi              应用这个配置
下面开始自定义：
root@zjh:/home/work/crosstool#ct-ng menuconfig
Paths and miscoptions  --->
                     (/home/work/crosstool/src) Local tarballs directory
这里指定你之前下载好的tar包所在目录，这样在制作时就不会重新下载了。
(/opt/${CT_TARGET}-4.7.3) Prefix directory
这里指定交叉编译器的安装路径：/opt/arm-cortex_a8-linux-gnueabi-4.7.3
(2) Number of parallel jobs
指定同时执行2个工作，加快编译速度
       Targetoptions  --->
                     Floatingpoint: (softfp (FPU))  --->
                     指定使用软浮点
       Toolchainoptions  --->
                     (cortex_a8) Tuple's vendor string
                     添加自定义信息，比如：
                     cortex_a8，制作后的交叉编译器名如：arm-cortex_a8-linux-gnueabi-
                     zjh，制作后的交叉编译器名如：arm-zjh-linux-gnueabi-
                     不设置，制作后的交叉编译器名如：arm-unknown-linux-gnueabi-
                     (arm-linux)Tuple's alias
                     设置别名，这样会给每个工具创建一个软链接，比如：
                     arm-linux-gcc链接到到arm-cortex_a8-linux-gnueabi-gcc
       OperatingSystem  --->
                     Linuxkernel version (3.10.2)  --->
                     选择内核版本，这里最好选大些，Linux内核都是向后兼容的。
       Ccompiler  --->
                     gccversion (4.7.3) --->
                     [*] Compile libmudflap
                     Libmudflap用于检查内存泄露，包括堆和栈，具体怎么用上网查资料。
       C-library  --->
                     glibcversion (2.13) --->
                     [*] Force unwind support (READ HELP!)
       Companionlibraries  --->
                     PPLversion (0.11.2) --->
                     CLooG version (0.15.11) --->
注意：在制作过程中，若出现版本不匹配的错误，重新配置正确的软件版本，再接着制作。
Save anAlternate Configuration File
保存为配置文件.config，然后退出。
在上面的配置中配置了源码包的存放目录为 /home/work/crosstool/src 因此需要在
/home/work/crosstool目录下创建目录src
root@zjh:/home/work/crosstool#mkdir src
根据上面配置的软件版本或者查看.config文件，查看需要的软件版本，然后使用自己的高速下载工具（比如迅雷）下载所有需要的软件，然后拷贝到/home/work/crosstool/src 目录下：
root@zjh:/home/work/crosstool#ls src/
binutils-2.20.1a.tar.bz2expat-2.1.0.tar.gz  glibc-ports-2.13.tar.bz2 ltrace_0.5.3.orig.tar.gz  ppl-0.11.2.tar.lzma cloog-ppl-0.15.11.tar.gz  gcc-4.7.3.tar.bz2   gmp-4.3.2.tar.bz2         mpc-1.0.1.tar.gz strace-4.5.19.tar.bz2 dmalloc-5.5.2.tgz         gdb-6.8a.tar.bz2   
 libelf-0.8.13.tar.gz      mpfr-2.4.2.tar.xz duma_2_5_15.tar.gz   glibc-2.13.tar.bz2  linux-3.10.2.tar.bz2      ncurses-5.9.tar.gz
执行ct-ng build开始制作：
root@zjh:/home/work/crosstool#ct-ng build
![](https://img-blog.csdn.net/20140619175549937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvempoc3VjY2VlZF8zMjk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在制作过程中，可以另外开一个终端，查看制作和编译的详细过程：
root@zjh:/home/work/crosstool#tail -f build.log
制作完成后，将看到如下信息：
[INFO ]  Build completed at 20140612.004712
[INFO ]  (elapsed: 74:49.55)
[INFO ]  Finishing installation (may take a fewseconds)...
然后在我们指定的安装目录下已经安装好了交叉编译器：
root@zjh:/home/work/crosstool#ls /opt/arm-cortex_a8-linux-gnueabi-4.7.3/
arm-cortex_a8-linux-gnueabi  bin build.log.bz2  include  lib  libexec  share
配置环境变量：在/etc/profile文件的最后添加一行：
exportPATH=/opt/arm-cortex_a8-linux-gnueabi-4.7.3/bin:$PATH
执行 ./etc/profile 使配置生效
注意：这里执行的命令为 . ，相当于source /etc/profile ，这只是在当前终端生效，要使所有生效，还得重启系统。
root@zjh:/home/work/crosstool#arm-linux-gcc -v
Using built-inspecs.
COLLECT_GCC=arm-linux-gcc
COLLECT_LTO_WRAPPER=/opt/arm-cortex_a8-linux-gnueabi-4.7.3/libexec/gcc/arm-cortex_a8-linux-gnueabi/4.7.3/lto-wrapper
Target: arm-cortex_a8-linux-gnueabi
Configured with:/home/work/crosstool/.build/src/gcc-4.7.3/configure --build=i686-build_pc-linux-gnu--host=i686-build_pc-linux-gnu --target=arm-cortex_a8-linux-gnueabi--prefix=/opt/arm-cortex_a8-linux-gnueabi-4.7.3--with-sysroot=/opt/arm-cortex_a8-linux-gnueabi-4.7.3/arm-cortex_a8-linux-gnueabi/sysroot--enable-languages=c,c++
 --with-arch=armv7-a --with-cpu=cortex-a8--with-tune=cortex-a8 --with-float=softfp --with-pkgversion='crosstool-NG1.19.0' --enable-__cxa_atexit --enable-libmudflap --disable-libgomp--disable-libssp --disable-libquadmath --disable-libquadmath-support--with-gmp=/home/work/crosstool/.build/arm-cortex_a8-linux-gnueabi/buildtools--with-mpfr=/home/work/crosstool/.build/arm-cortex_a8-linux-gnueabi/buildtools--with-mpc=/home/work/crosstool/.build/arm-cortex_a8-linux-gnueabi/buildtools--with-ppl=/home/work/crosstool/.build/arm-cortex_a8-linux-gnueabi/buildtools--with-cloog=/home/work/crosstool/.build/arm-cortex_a8-linux-gnueabi/buildtools--with-libelf=no
 --with-host-libstdcxx='-static-libgcc-Wl,-Bstatic,-lstdc++,-Bdynamic -lm-L/home/work/crosstool/.build/arm-cortex_a8-linux-gnueabi/buildtools/lib -lpwl'--enable-threads=posix --enable-target-optspace --disable-nls--disable-multilib--with-local-prefix=/opt/arm-cortex_a8-linux-gnueabi-4.7.3/arm-cortex_a8-linux-gnueabi/sysroot--enable-c99
 --enable-long-long
Thread model: posix
gcc version 4.7.3 (crosstool-NG 1.19.0)
…………
内容较多，请下载pdf版本
[http://pan.baidu.com/s/1fEfG6#dir](http://pan.baidu.com/s/1fEfG6#dir)
