# Qt入门笔记----Windows版 - gauss的专栏 - CSDN博客
2013年01月03日 18:03:12[gauss](https://me.csdn.net/mathlmx)阅读数：222
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)

很早就窥视Qt了，只是在偶的小白上安装了一个Mac版，对于里面的安装配置一概不知，之前在Mac下试编写了几个例子（以后再补充Mac版的一些注意事项）。
Windows XP下的安装如下：
1、去Trolltech官方网下载Qt for Open Souce C++ development on Windows，地址如下：[ftp://ftp.trolltech.no/qt/source/qt-win-opensource-src-4.4.3.zip](ftp://ftp.trolltech.no/qt/source/qt-win-opensource-src-4.4.3.zip)；
2、解压缩到目录D:/Qt/qt-win-opensource-src-4.4.3；
3、Add环境变量QTDIR=D:/Qt/qt-win-opensource-src-4.4.3，Set环境变量PATH=%QTDIR%/bin;%PATH%；
4、拷贝VC安装目录下vsvars.bat到QTDIR目录下，运行一次；
4、运行cmd，进入QTDIR目录下，运行命令：
configure -platform win32-msvc
完毕后，运行nmake。
配置和编译的时间比较长。
在VS2008下直接编写Qt程序设置方法：
1、启动VS2008，新建Makefile Project类工程，取名为HelloQt。 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/aaronkevin021/EntryImages/20081114/1.JPG)
2、调试配置参数设置：
Build command line:        qmake -project&&qmake&&nmake debug
Clean Commands:            nmake debug-clean
Rebuild command line:     qmake -project&&qmake&&nmake debug
![](https://p-blog.csdn.net/images/p_blog_csdn_net/aaronkevin021/EntryImages/20081114/2.jpg)
3、发布配置参数设置：
Build command line:        qmake -project&&qmake&&nmake release
Clean Commands:            nmake release-clean
Rebuild command line:     qmake -project&&qmake&&nmake release
![](https://p-blog.csdn.net/images/p_blog_csdn_net/aaronkevin021/EntryImages/20081114/3.jpg)
4、完成后，选择菜单栏Project下Properties。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/aaronkevin021/EntryImages/20081114/4.jpg)
将Command下设置为$(TargetPath)修改为$(TargetDir)/Debug/$(TargetFileName)。此处设置确保在F5调试时执行正确的路径。
5、在VS2008下添加Qt库，如下设置：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/aaronkevin021/EntryImages/20081114/5.jpg)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/aaronkevin021/EntryImages/20081114/6.jpg)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/aaronkevin021/EntryImages/20081114/7.jpg)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/aaronkevin021/EntryImages/20081114/8.jpg)
6、向HelloQt工程添加新项，选取cpp类文件，取名main。
编写main.cpp代码如下：
- #include <QApplication> 
- #include <QLabel> 
- 
- int main(int argc, char *argv[])
- {
-     QApplication app(argc, argv);
-     QLabel *label = new QLabel("Hello Qt!");
-     label->show();
- return app.exec();
- }
