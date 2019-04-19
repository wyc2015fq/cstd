# 交叉编译ARM版Qt(qt-everywhere-opensource-src-4.8.4) - xqhrs232的专栏 - CSDN博客
2017年03月18日 10:31:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2452
原文地址::[http://www.cnblogs.com/lihaiyan/p/4274465.html](http://www.cnblogs.com/lihaiyan/p/4274465.html)
相关文章
1、[Tslib和Qt 4.8.4与在开发板上的移植](http://www.cnblogs.com/emouse/archive/2013/01/29/2881311.html)----[http://www.cnblogs.com/emouse/archive/2013/01/29/2881311.html](http://www.cnblogs.com/emouse/archive/2013/01/29/2881311.html)
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
