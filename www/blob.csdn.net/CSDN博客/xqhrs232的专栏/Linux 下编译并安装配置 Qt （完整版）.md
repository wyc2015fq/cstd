# Linux 下编译并安装配置 Qt （完整版） - xqhrs232的专栏 - CSDN博客
2016年06月08日 17:54:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：473
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://mobile.51cto.com/symbian-272869.htm](http://mobile.51cto.com/symbian-272869.htm)
相关文章
1、[Linux 下编译、安装、配置
 QT](http://www.cnblogs.com/emouse/archive/2013/01/28/2880142.html)----[http://www.cnblogs.com/emouse/archive/2013/01/28/2880142.html](http://www.cnblogs.com/emouse/archive/2013/01/28/2880142.html)
2、Linux下QT安装----[http://wenku.baidu.com/link?url=QKHbACOrQ7fq_YR7hARTRw_YE3vr35BGN6U94MJFUt0LKo_dFC6_JUKHt-cEHydCpYXtBidEMs8KoyUE8LtmtJEEBzVZoOYn63jgqHB](http://wenku.baidu.com/link?url=QKHbACOrQ7fq_YR7hARTRw_YE3vr35BGN6U94MJFUt0LKo_dFC6_JUKHt-cEHydCpYXtBidEMs8KoyUE8LtmtJEEBzVZoOYn63jgqHBClDO)-[http://blog.csdn.net/neu_sunlei/article/details/5524384](http://blog.csdn.net/neu_sunlei/article/details/5524384)
3、[Qt 经典出错信息之”Basic XLib functionality test failed!”(Z..z..)](http://blog.csdn.net/neu_sunlei/article/details/5524384)----[http://blog.csdn.net/neu_sunlei/article/details/5524384](http://blog.csdn.net/neu_sunlei/article/details/5524384)

本文介绍的是Linux 下编译并安装配置 Qt，最近准备做 Nokia 的 [Symbian](http://mobile.51cto.com/symbian/),[Maemo](http://mobile.51cto.com/meego/)下触摸屏开发。考虑到程序的跨平台可移植性，最终选择使用 Qt 开发。相对来说，国内关于 Qt 相关文档并不算很多。作者将 Linux 下编译并安装配置 Qt 全过程总结了一下，只希望可以方便更多的朋友！
1、获得源代码
src 官网下载地址：ftp://ftp.qt.nokia.com/qt/source/
2009 年 10 月 1 日发布的 qt-x11-opensource-src-4.5.3.tar.gz，大小 122 Mb。
2、解压缩
- tar xvfz qt-x11-opensource-src-4.5.3.tar.gz 
3、生成 makefile 文件
- ./configure 
这个时候他会问你：“Which edition of Qt do you want to use?”
然后给你两个类型供选择：
- Type ''c'' if you want to use the Commercial Edition.  
- Type ''o'' if you want to use the Open Source Edition. 
第一个是商业版，第二个是自由版。
选择 ''o''（Qt/嵌入式自由版是Qt为了开发自由软件提供的嵌入式版本）。
这时候，出现许可界面。
选择  ''yes'' 接受许可协议。
开始生成 makefile 文件。这个大约需要 5 -10 分钟的时间。
4、Qt 编译
- gmake 
这个过程时间比较长，需要两个小时左右。
5、Qt 安装
运行 gmake install
默认安装至 /usr/local/Trolltech/Qt-4.5.3
大约 5 － 10 分钟。正常结束，安装完毕。
运行 /usr/local/Trolltech/Qt-4.5.3/bin/designer ，看见 qt 启动，也就是说 qt 安装好了。
6、设置环境变量
一、先修改用户环境变量
- vi /home/defonds/.bashrc 
在其最后添加以下信息
- export QTDIR=/usr/local/Trolltech/Qt-4.5.3  
- export PATH=$QTDIR/bin:$PATH  
- export MANPATH=$QTDIR/man:$MANPATH  
- export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH 
二、再修改所有用户环境变量
- vi /etc/profile 
在其最后添加以下信息
- export QTDIR=/usr/local/Trolltech/Qt-4.5.3  
- export PATH=$QTDIR/bin:$PATH  
- export MANPATH=$QTDIR/man:$MANPATH  
- export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH 
三、测试一下
重新开一个终端，输入命令：
- qmake -v 
输出结果：
- QMake version 2.01a  
-  Using Qt version 4.5.3 in /usr/local/Trolltech/Qt-4.5.3/lib 
输入命令：
- which qmake 
输出结果：
- /usr/local/Trolltech/Qt-4.5.3/bin/qmake 
证明 Qt 环境变量已经被配置成功。
编者按 ：
Linux 启动时，先读取用户环境变量，即 /home/defonds/.bashrc 的设置。次读取所有用户环境变量，即 /etc/profile 中的配置。
开始 Qt
写个简单的 Qt 程序使用安装的 Qt 编译来测试一下它的工作效果
新建一个文件名为 hello.cpp，其内容如下：
- view plaincopy to clipboardprint?#include <QApplication>
- #include <QLabel>
- int main(int argc,char *argv[])    
- {    
-     QApplication app(argc,argv);    
-     QLabel *label = new QLabel("Hello Qt!");    
-     label->show();    
-     return app.exec();    
- }    
- #include <QApplication>
- #include <QLabel>
- int main(int argc,char *argv[])  
- {  
-  QApplication app(argc,argv);  
-  QLabel *label = new QLabel("Hello Qt!");  
-  label->show();  
-  return app.exec();  
- }  
保存 hello.cpp，编译过程如下：
在 hello.cpp 所在目录下，运行命令
- qmake -project 
hello.cpp 同目录下有 hello.pro 文件被生成，它是与平台无关的工程文件。
在 hello.cpp 所在目录下，运行命令
- qmake hello.pro 
同目录下有 Makefile 文件被生成（Makefile是指导编译器编译源代码的配置文件，在其目录下输入make命令(nmake在win32,vc6环境)就可以完成编译）。在 hello.cpp 所在目录下，运行命令
- make 
同目录下有 hello,hello.o 两个文件被编译生成。其中 hello 就是在当前 Linux 系统下使用 Qt 编译生成的可执行文件了。双击运行 hello，“Hello Qt!“ 文本框弹出，OK！做到这一步，证明 Qt 已经完全可以在你的电脑上运作了。还等什么呢，开始你精彩而又愉快的 Qt 开发之旅吧！
小结：关于Linux 下编译并安装配置 Qt （完整版）的内容介绍完了，希望本文对你有所帮助！
