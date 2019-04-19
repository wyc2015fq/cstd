# Linux下QT安装教程 - xqhrs232的专栏 - CSDN博客
2017年03月18日 10:37:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：7434
原文地址::[http://www.cnblogs.com/lihaiyan/p/4274465.html](http://www.cnblogs.com/lihaiyan/p/4274465.html)
相关文章
1、[Linux下编译、安装、配置 QT](http://www.cnblogs.com/emouse/archive/2013/01/28/2880142.html)----[http://www.cnblogs.com/emouse/archive/2013/01/28/2880142.html](http://www.cnblogs.com/emouse/archive/2013/01/28/2880142.html)
2、在Linux下安装Qt4.4.3 详细步骤----[http://www.linuxidc.com/Linux/2009-06/20499.htm](http://www.linuxidc.com/Linux/2009-06/20499.htm)
3、[Linux下安装Qt-如何在Linux下安装Qt](http://vvsongsunny.iteye.com/blog/756879)----[http://vvsongsunny.iteye.com/blog/756879](http://vvsongsunny.iteye.com/blog/756879)
2015-01-28
准备两个文件：
qt-x11-opensource-src-4.5.3.tar.gz
qt-creator-opensource-linux-x86_64-3.3.0.run
1、获得源代码
src 官网下载地址：[ftp://ftp.qt.nokia.com/qt/source/](ftp://ftp.qt.nokia.com/qt/source/)
2009 年 10 月 1 日发布的 qt-x11-opensource-src-4.5.3.tar.gz，大小 122 Mb。
2、解压缩
tar xvfz qt-x11-opensource-src-4.5.3.tar.gz
3、生成 makefile 文件
./configure
这个时候他会问你：“Which edition of Qt do you want to use?”
然后给你两个类型供选择：
Type ''c'' if you want to use the Commercial Edition. 
Type ''o'' if you want to use the Open Source Edition.
第一个是商业版，第二个是自由版。
选择 ''o''（Qt/嵌入式自由版是Qt为了开发自由软件提供的嵌入式版本）。
这时候，出现许可界面。
选择  ''yes'' 接受许可协议。
开始生成 makefile 文件。这个大约需要 5 -10 分钟的时间。
4、Qt 编译
gmake            //若是此处出错，解决办法见下面
这个过程时间比较长，需要两个小时左右。
5、Qt 安装
运行 gmake install
默认安装至 /usr/local/Trolltech/Qt-4.5.3
大约 5 － 10 分钟。正常结束，安装完毕。
运行 /usr/local/Trolltech/Qt-4.5.3/bin/designer ，看见 qt 启动，也就是说 qt 安装好了。
6、设置环境变量
一、先修改用户环境变量
vi /home/defonds/.bashrc
在其最后添加以下信息
export QTDIR=/usr/local/Trolltech/Qt-4.5.3 
export PATH=$QTDIR/bin:$PATH 
export MANPATH=$QTDIR/man:$MANPATH 
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
二、再修改所有用户环境变量
vi /etc/profile
在其最后添加以下信息
export QTDIR=/usr/local/Trolltech/Qt-4.5.3 
export PATH=$QTDIR/bin:$PATH 
export MANPATH=$QTDIR/man:$MANPATH 
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
三、测试一下
重新开一个终端，输入命令：
qmake -v   
输出结果：
QMake version 2.01a 
 Using Qt version 4.5.3 in /usr/local/Trolltech/Qt-4.5.3/lib
输入命令：
which qmake
输出结果：
/usr/local/Trolltech/Qt-4.5.3/bin/qmake
证明 Qt 环境变量已经被配置成功。
qt-creator的安装方法很简单，即：
1、下载qt-creator-opensource-linux-x86_64-3.3.0.run
2、chmod +x qt-creator-opensource-linux-x86_64-3.3.0.run
./ qt-creator-opensource-linux-x86_64-3.3.0.run
接下来就是非常熟悉的安装界面了
重启一下就好了！
刚开始编译时，gmake 没用的方法：
通过以上的分析可以知道，解决这个问题有两种方法，一个就是更改qmake的链接，使得qmake这个命令链接到安装目录下我们安装版本的qmake，还有一种方法就是改环境变量。
先说更改链接的方法，通过上面的介绍，这个其实操作就比较直接和简单了，命令和实验结果如下：
root@emouse:/# rm /etc/alternatives/qmake
root@emouse:/#
ln -s /usr/local/Trolltech/Qt-4.8.4/bin/qmake /etc/alternatives/qmake
root@emouse:/# qmake -v
QMake version 2.01a
Using Qt version 4.8.4 in /usr/local/Trolltech/Qt-4.8.4/lib
可以看到，qmake已经成功更改为4.8.4版本。
第二种办法就是更改环境变量，这个我没有尝试，有兴趣的自己验证。
vi /home/defonds/.bashrc 用来修改用户环境变量，在文件后面添加以下信息
export QTDIR=/usr/local/Trolltech/Qt-4.8.4
export PATH=$QTDIR/bin:$PATH 
export MANPATH=$QTDIR/man:$MANPATH 
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
再修改所有用户环境变量vi /etc/profile 同样在文件后添加上面的环境变量信息。这种方法应该同样可行。
 交叉编译ARM版 Qt (qt-everywhere-opensource-src-4.8.4)
因实际项目需求，需将Qt编译成LINUX-ARM版，以下为整个操作过程，使用Qt编写的程序已在实际开发板上运行，在此贴出，但愿能为需要的朋友提供上帮助。
准备工作：
Linux内核版本：  
        开发电脑：Ubuntu12.04，开发板：linux-2.6.37
交叉编译工具：
        arm-none-linux-gnueabi 4.4.1
Qt安装包：
        qt-everywhere-opensource-src-4.8.4.tar.gz
安装
一.安装交叉编译工具
 1.本机使用的交叉编译工具是：arm-none-linux-gnueabi
 2.添加交叉编译工具的路径至相应环境变量中：
   export PATH=/usr/local/arm-2009q1-203/bin/:$PATH
 3.通过以下命令查看arm-none-linux-gnueabi编译工具是否安装成功
   a. 执行：arm-none-linux-gnueabi-g++ -v
   b. 执行：arm-none-linux-gnueabi-gcc -v
   如果以上两步都能正确打印对应的版本信息，则表明arm-none-linux-gnueabi编译工具
   已安装成功！
二.交叉编译ARM版Qt
 1.下载Qt源码包 qt-everywhere-opensource-src-4.8.4.tar.gz；
 2.解压Qt源码包
   tar -xvfz qt-everywhere-opensource-src-4.8.4.tar.gz
 3.修改文件：
/usr/local/Qt-4.8.4-linux-arm/mkspecs/qws/linux-arm-g++/qmake.conf
   # 设置交叉编译工具链：
   # 本项目使用的交叉编译工具是 arm-none-linux-gnueabi-g++
   #
   QMAKE_CC                         = arm-none-linux-gnueabi-gcc
   QMAKE_CXX                      = arm-none-linux-gnueabi-g++
   QMAKE_LINK                      = arm-none-linux-gnueabi-g++
   QMAKE_LINK_SHLIB        = arm-none-linux-gnueabi-g++
   QMAKE_AR                   = arm-none-linux-gnueabi-ar cqs
   QMAKE_OBJCOPY      = arm-none-linux-gnueabi-objcopy
   QMAKE_STRIP             = arm-none-linux-gnueabi-strip
 4.(如有需要，需修改g++.conf文件)
   修改文件： /usr/local/Qt-4.8.4-linux-arm/mkspecs/common/g++.conf
   # 设置使用ARM高级特性的编译标志：
   #
   QMAKE_CFLAGS_RELEASE  += -O2 -march=armv7-a -mtune=cortex-a8 -mfloat-abi=softfp
 5.配置Qt编译选项
   进入Qt解压目录，执行如下命令：
 ./configure -prefix /usr/local/Qt-4.8.4 -embedded arm -xplatform /qws/linux-arm-g++ -depths 16,24,32 -little-endian -no-mmx -no-3dnow -no-sse -no-sse2 -no-glib -no-cups -no-largefile -no-accessibility -no-openssl -no-gtkstyle -qt-freetype
 -qt-mouse-pc -qt-mouse-linuxtp -qt-mouse-linuxinput -plugin-mouse-pc -fast -qt-gfx-transformed -opensource -confirm-license
   或者写一个脚本config.sh,内容如上，之后运行这个脚本：./config.sh
 6.执行 make 命令；
   该步骤大致需要2~3个小时
错误：qtconcurrentiteratekernel.cpp:(.text+0x3c): undefined reference to `clock_gettime'
修改：vim src/corelib/Makefile 加上-lrt
LIBS          = $(SUBLIBS)  -L/opt/qt-everywhere-opensource-src-4.8.4/lib -lpthread -lm -ldl -lrt
 7.执行 make install 命令；
 8.修改用户环境变量 
   export ARMQTDIR=/usr/local/Qt-4.8.4
   export PATH=$ARMQTDIR/bin:$PATH
   export MANPATH=$ARMQTDIR/man:$MANPATH
   export LD_LIBRARY_PATH=$ARMQTDIR/lib:$LD_LIBRARY_PATH
 9.测试 ARM 版 QT 是否安装并配置成功：
    a. 执行：qmake -v
        打印以下对应信息：
                QMake version 2.01a
                Using Qt version 4.8.4 in /usr/local/Qt-4.8.4/lib
    b. 执行：which qmake
                打印以下对应信息：
                 /usr/local/Qt-4.8.4/bin/qmake
    c. 编写一个简单的测试程序,文件名为hello.cpp，进入该文件目录，进行工程编译，看是否能正确通编译
         hello.cpp源码：
         #include <QApplication>
         #include <QDebug>
         int main(int argc,char *argv[])
        {
            qDebug("Hello, welcome to Qt world!");
            return 0;
         }
  顺序执行以下操作：
  @1. 执行：qmake -project hello1.cpp
        生成 hello1.pro 工程文件
  @2. 执行：qmake hello1.pro
        生成 Makefile 文件
  @3. 执行：make
        生成 hello.o, hello 文件
       如果执行@2步骤时出现以下错误：
           QMAKESPEC has not been set, so configuration cannot be deduced.
           Error processing project file: hello1.pro
       则是 QMAKESPEC 变量没有设置好，执行：
      export QMAKESPEC=/usr/local/qt-4.8.4/mkspecs/qws/linux-arm-g++
cd /usr/local/Qt-4.8.4/lib/
file libQT*   看输出信息是ELF ARM
之后将/usr/local/Qt-4.8.4/lib/libQt* 根文件系统/usr/lib/下
********************************************
* 博客园： [http://www.cnblogs.com/lihaiyan/](http://www.cnblogs.com/lihaiyan/)
* 邮箱：1063385677@qq.com 
* QQ: 1063385677 
* Copyright ©2014 Lover雪儿 
********************************************
本文版权归作者和博客园共有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文链接，否则保留追究法律责任的权利。
