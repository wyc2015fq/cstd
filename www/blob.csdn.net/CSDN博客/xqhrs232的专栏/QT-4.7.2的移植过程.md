# QT-4.7.2的移植过程 - xqhrs232的专栏 - CSDN博客
2016年05月20日 09:40:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：554
原文地址::[http://blog.sina.com.cn/s/blog_b74a41780101ihlt.html](http://blog.sina.com.cn/s/blog_b74a41780101ihlt.html)
相关文章
1、Qt-4.7.3开发环境的搭建----[http://wenku.baidu.com/link?url=I2I6GAGHaY6Z6MW86KQNaan0naNM5Gb5JEG9PWjqbtkWBhfPhjj0uUu0JfpYbjaRNtKqU-aINsyPUZSLIP1XAR9mgIGutv_xP0V6bZqDeDS](http://wenku.baidu.com/link?url=I2I6GAGHaY6Z6MW86KQNaan0naNM5Gb5JEG9PWjqbtkWBhfPhjj0uUu0JfpYbjaRNtKqU-aINsyPUZSLIP1XAR9mgIGutv_xP0V6bZqDeDS)
2、VS2008集成QT4.7.2环境搭建----[http://www.360doc.com/content/12/0529/15/168576_214511906.shtml](http://www.360doc.com/content/12/0529/15/168576_214511906.shtml)
3、QT 4.7.2 + ftplib 4.0 例程  ----[http://download.csdn.net/detail/minking2004/5725387](http://download.csdn.net/detail/minking2004/5725387)
4、QT TCP客服端-服务端模式(一)----[http://blog.sina.com.cn/s/blog_b74a41780101jfu8.html](http://blog.sina.com.cn/s/blog_b74a41780101jfu8.html)
5、QT源码下载地址----[http://download.qt.io/archive/qt/4.7/](http://download.qt.io/archive/qt/4.7/)
6、QT源码下载地址----[https://www.qt.io/download/](https://www.qt.io/download/)
7、
# [Qt Creator下载和安装(详细教程)](http://blog.csdn.net/win_turn/article/details/50465127)----[http://blog.csdn.net/win_turn/article/details/50465127](http://blog.csdn.net/win_turn/article/details/50465127)
主机：红帽5.0
arm-linux-gcc：4.5.1
1.下载Qt源码并解压
我下载的为：qt-everywhere-opensource-src-4.7.2.tar.gz
mkdir /root/build_qt
cd /root/build_qt
tar zxvf qt-everywhere-opensource-src-4.7.2.tar.gz
                mv  qt-everywhere-opensource-src-4.7.2  qte
                cd qte
2.设置安装环境变量，编写install.sh
vim install.sh
export PATH=/usr/local/arm/4.5.1/bin:$PATH
export CPLUS_INCLUDE_PATH=/usr/local/arm/4.5.1/arm-none-linux-gnueabi/include/c++/4.5.1:/usr/local/arm/4.5.1/arm-none-linux-gnueabi/include/c++/4.5.1/arm-none-linux-gnueabi/:/usr/local/arm/4.5.1/arm-none-eabi/include/c++/4.5.1:/usr/local/arm/4.5.1/arm-none-eabi/include/c++/4.5.1/arm-none-eabi:$CPLUS_INCLUDE_PATH
source install.sh
3.修改QTE源码目录中的src/gui/embedded/qmouselinuxtp_qws.cpp文件，添加触摸屏支持
vim src/gui/embedded/qmouselinuxtp_qws.cpp
    QString mousedev;
   if (device.isEmpty()) {
mousedev =QLatin1String("/dev/event0");
4.需要安装tslib的需要先将tslib库解压好，我的不需要，故不要了，跳过
5.配置编译QTE
vim arm_build.sh
echo yes | ./configure -opensource -embedded arm -xplatform qws/linux-arm-g++  -webkit -qt-libtiff -qt-libmng -qt-mouse-pc
 -no-mouse-linuxtp -no-neon  -depths 4,8,16,32 -qvfb 2>&1 | tee ./qteconfig.log
source arm_build.sh
便开始进行配置了，其中我删除了有关tslib选项，有需要的要加上
接着会出错：
/root/built_qt/qte/src/corelib/tools/qlocale.cpp: In function ‘char* qdtoa(double, int, int, int*, int*, char**, char**)’:
/root/built_qt/qte/src/corelib/tools/qlocale.cpp:6639: 错误：‘fenv_t’ 在此作用域中尚未声明
/root/built_qt/qte/src/corelib/tools/qlocale.cpp:6639: 错误：expected `;' before ‘envp’
/root/built_qt/qte/src/corelib/tools/qlocale.cpp:6640: 错误：‘envp’ 在此作用域中尚未声明
/root/built_qt/qte/src/corelib/tools/qlocale.cpp:6640: 错误：‘feholdexcept’ 在此作用域中尚未声明
/root/built_qt/qte/src/corelib/tools/qlocale.cpp:6655: 错误：‘fesetenv’ 在此作用域中尚未声明
目前解决方法：把/usr/include中的fenv.h
 复制到qlocale.cpp所在目录
并修改#include 为#include "fenv.h"
cp /usr/include/fenv.h /root/build_qt/qte/src/corelib/tools/
#vim /root/build_qt/qte/src/corelib/tools/qlocale.cpp +81
将#include 改成#include "fenv.h"
再source arm_build.sh
没有出错，继续下一步
6.make&&
 make install 2>&1 | tee ./qtemake.log
便是长时间的编译了......等待
还以为中间会有出错的，但是很给力，一直到结束都没错。好了，一切就绪，接下来就开始做界面了
