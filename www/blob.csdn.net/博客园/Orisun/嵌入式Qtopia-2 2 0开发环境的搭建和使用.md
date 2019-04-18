# 嵌入式Qtopia-2.2.0开发环境的搭建和使用 - Orisun - 博客园







# [嵌入式Qtopia-2.2.0开发环境的搭建和使用](https://www.cnblogs.com/zhangchaoyang/articles/1937551.html)





先说明一下几个QT的区别：

从Qt4.1开始，Qt/Embedded改名为Qtopia Core，又从Qt4.4.1开始，Qtopia Core又改名为Qt for Embedded Linux。

我们通常所说的 Qt 都是指的用于 Linux/Unix 的 Qt/X11，真正意义上Qt 是泛指 Qt 的所有版本的图像界面库,比如 Qt/X11,Qt Windows, Qt Mac 等。


Qt/Embeded 的概念:它是用于嵌入式 Linux 系统的 Qt 版本,Qt/Embeded 也简称 Qte 或 Qt/EQte 去掉了 X Lib 的依赖而直接工作在 Framebuffer 上,虽然它是 Qt 的嵌入式版本,但是它不是 Qt/X11 的子集,它有部分机制(比如 QCOP 等)就不能用于 Qt/X11 中。



Qtopia 是一个基于 Qte 的类似桌面系统的应用环境,包含有 PDA 版本和 Phone 版本。实质上 Qtopia Core 还是基础类库,相当于 Qte。



最近 Qtopia Core 改名了,新名字叫做:qt-embeded-linux-xxxxxx。





以下的内容结合《基于 SKY2440/TQ2440 的 Qt/Embeded 应用程序开发完全手册》

编译 PC 版本的 Qtopia-2.2.0

1、准备编译器

知道你的ubuntu10.04默认安装的是gcc4.4，现在我们要使用gcc3.4

#apt-get install gcc-3.4

安装后/usr/bin下就多出了gcc-3.4，原先这里有个gcc，它是指向gcc-4.4的软链接，现在我们要让它指向gcc-3.4

#rm gcc

#ln -s /usr/bin/gcc-3.4 /usr/bin/gcc

测试一下

$gcc -v

注意：在编译PC版本的qt4.5时我们又需要把gcc的版本从3.4升到4.4

2、解压包

#tar xvfj Qte_20100601.tar.bz2 -C /

在“Linux 资源\Linux 平台开发工具包\”里面

�arm-qtopia-2.2.0_build:将 qtopia-2.2.0.tar.bz2 源码编译成 ARM 版本,并且编译 tslib-1.4.1.tar.bz2 源码和 EmbedSky_apps.tar.bz2 源码。

� arm-qtiopia-2.2.0-konqueror_build:在 arm-qtopia-2.2.0_build 的基础上增加了 konqueror.tar.bz2 的 编 译 。

� setARM_QpeEnv:设置 ARM 版本的环境变量的脚本。

� setX86_QpeEnv:设置 X86 版本的环境变量的脚本。

� x86-qtopia-2.2.0_build:将 qtopia-2.2.20tar.bz2 源码编成 PC 版本。

� x86-qtopia-2.2.0-konqeror_build:在 x86-qtopia-2.2.0_build 的基础上增加了 konqueror.tar.bz2 的编译。

� EmbedSky_apps.tar.bz2:后面几个章节讲到的应用程序的源码合集(Makefile 文件基于 ARM 的,可以利用该源码包自行生成 x86 版本的 Makefile 文 件 )。

� konqueror.tar.bz2:Web 浏览器的源码。

� qtopia-2.2.0.tar.bz2:qtopia-2.2.0 的源码。

� test_x86:当 PC 版本的 Qtopia-2.2.0 编译成功后,仿真时运行该脚本。

� tslib-1.4.1.tar.bz2:触摸校正的源码

3、#./x86-qtopia-2.2.0-kongqueror_build

这一步会发生很多错误，下面几步来修正。

以下内容参照 [http://bbs.embedsky.net/viewthread.php?tid=2773&highlight=ubuntu9.10](http://bbs.embedsky.net/viewthread.php?tid=2773&highlight=ubuntu9.10)

和 [http://hi.baidu.com/hisumg/blog/item/d15592c675a472c639db49ad.html](http://hi.baidu.com/hisumg/blog/item/d15592c675a472c639db49ad.html)

4、修改几处代码源文件

修改x86-qtopia-2.2.0/qt2/src/tools/qmemoryfile_unix.cpp 143行
将
    if (!f)
        f = ::pen(tmpFile.latin1(), O_CREAT | O_WRONLY);   
修改为
    if (!f)
        f = ::pen(tmpFile.latin1(), O_CREAT | O_WRONLY, 0666);

修改x86-qtopia-2.2.0/qtopia/src/libraries/qtopia/qmemoryfile_unix.cpp 141行
将
    if (!f)
        f = ::pen(tmpFile.latin1(), O_CREAT | O_WRONLY);   
修改为
    if (!f)
        f = ::pen(tmpFile.latin1(), O_CREAT | O_WRONLY, 0666);  

修改/x86-qtopia-2.2.0/qtopia/src/libraries/qtopia/backend/vobject.cpp:419行
    char *dot = strrchr(g,'.');
为
    char *dot = (char*)strrchr(g,'.');

修正/x86-qtopia-2.2.0/qtopia/src/plugins/codecs/wavplugin/wavplugin.cpp:435行
    char *ext = strrchr( path.latin1(), '.' );
改为
    char *ext = (char*)strrchr( path.latin1(), '.' );

修改x86-qtopia-2.2.0-konqueror_biuld的第26行，
把
        . setQpeEnv
改为
        ./setQpeEnv

5、安装一些包

# sudo apt-get install libx11-dev x11proto-xext-dev libqt3-mt-dev uuid-dev

6、手动设置环境变量，然后执行脚本

#source ./x86-qtopia-2.2.0/setQpeEnv

#./x86-qtopia-2.2.0-kongqueror_build

7、PC 仿真 Qtopia-2.2.0

#./test_x86

编译 ARM 版本的 Qtopia-2.2.0

1、准备交叉编译器

#tar xvfj EABI_4.3.3_EmbedSky_20091210.tar.bz2 -C /

然后新的交叉编译器就在/opt/EmedSky/4.3.3/bin下面，把这个路径放到/etc/environment中，source一下

#which arm-linux-gcc　　　　;确保当前的交叉编译器就是/opt/EmedSky/4.3.3/bin下的那个，因为你之前可能装过其他版本的交叉编译器，并且把它的路径也放到了PATH中，并且还在/opt/EmedSky/4.3.3/bin在前面，那就不对了。

2、#./arm-qtopia-2.2.0-kongqueror_build

这一步过程中会有一些错误，下面几步来修正

3、 修改arm-qtopia-2.2.0/qt2/src/tools/qmemoryfile_unix.cpp 143行
将
    if (!f)
        f = ::pen(tmpFile.latin1(), O_CREAT | O_WRONLY);   
修改为
    if (!f)
        f = ::pen(tmpFile.latin1(), O_CREAT | O_WRONLY, 0666);

4、手动设置环境变量，然后执行脚本

# source ./arm-qtopia-2.2.0/setQpeEnv

#./arm-qtopia-2.2.0-kongqueror_build

最后显示done!成功。

至于2.3节所讲的制作包含 Qtopia-2.2.0 的文件系统工作就不需要做了天嵌制作好的文件系统里面已经包含了。

开发第一个Qt程序

依照天嵌资料第三章讲的去做。不过有三点需要注意

（1）在main.cpp中印少了一个右括号

（2）Makefile文件要改的地方不仅仅是红色标注的地方。红色标注的只是要添加的内容，还有一些地重复了，要删除。直接把资料上给的Makefile复制过来也有一些问题。如果是自己改注意最后一下文件项后面不能要“\"，比如

OBJECTS = first.o \

　　　　　　main.o

main.o后面就不能要“\"。

（3）当编译好arm版本的first程序后，first、first.png、first.desktop是在资料所说的三个位置，但是“把三个文件放到开发板的相应位置”怎么理解呢？

把first放到开发板的/opt/Qtopia/bin/ 下；first.png放到/opt/Qtopia/pics/下；first.desktop放到/Qtopia/apps/EmbedSky/下














