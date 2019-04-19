# Linux 下 Qt Creator 的安装使用 - xqhrs232的专栏 - CSDN博客
2016年06月12日 16:49:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：277
原文地址::[http://www.linuxidc.com/Linux/2011-04/35071p2.htm](http://www.linuxidc.com/Linux/2011-04/35071p2.htm)
相关文章
1、Linux下Qt开发环境的安装与集成----[http://blog.163.com/danshiming@126/blog/static/10941274820152124527833/](http://blog.163.com/danshiming@126/blog/static/10941274820152124527833/)
Qt 以其开源，免费，完全面向对象（很容易扩展），允许真正的组件编程以及可移植跨平台等诸多优势得到越来越多的开发人员的青睐。Qt Creator 是 Nokia 官方推出的专门针对 Qt 开发的 IDE。本文详细介绍了 Linux 下 Qt Creator 的安装，并针对 Qt Creator 的使用举了一个 Hello World 级别的例子，希望对第一次接触 Qt Creator 的朋友可以起到抛砖引玉的作用。
Qt Creator 安装
1。准备
下载并安装好 Qt（关于 Qt 的编译并安装配置，请参考前一篇《[Linux 下编译并安装配置 Qt 全过程](http://www.linuxidc.com/Linux/2011-04/35070.htm)》 ）。
2。查看
查看自己电脑系统情况，终端输入命令：
uname -a
作者的是 x86_64 GNU/Linux。
3。下载
到 Qt 官方网站下载与自己电脑相配套的 Qt Creator，地址是 [http://qt.nokia.com/downloads](http://qt.nokia.com/downloads)。 Qt Creator 的最新版本是 1.3.0，作者选择的是 Qt Creator 1.3.0 Binary for Linux/X11 64-bit
 (48 MB)。
4。安装
下载后得到大小 47.2 Mb 的安装文件 qt-creator-linux-x86_64-opensource-1.3.0.bin，在 Linux 上安装如下：
chmod u+x ./qt-creator-linux-x86_64-opensource-1.3.0.bin
./qt-creator-linux-x86_64-opensource-1.3.0.bin
这时候，启动了 Setup Qt Creator 欢迎安装对话框，点击“Next”->
协议许可界面，选择“I accept the agreement”，点击“Next” ->
选择 Qt Creator 安装目录对话框。作者选择的是 /home/defonds/TOOLS/qtcreator/qtcreator-1.3.0，点击“Next” ->
准备安装界面，点击“Next” ->
安装结束，点击“Finish”。
Qt Creator 使用
新建项目 HelloWorld
启动桌面上的 Qt Creator。新建工程 HelloWorld，菜单点击“File” -> New File or Project... -> Projects 中选择 Empty Qt4 Project -> OK -> 项目名键入 HelloWorld，工作台随意，作者选择的是 /home/defonds/cpp/qt ， Next -> Finish。
新建类 Hello
右键单击项目名 HelloWorld，Add New... -> C++ 下选择 C++ Source File，点击“OK” -> 输入类名 Hello 点击“Next” ->  Finish。
这个时候 Hello.cpp 被创建，双击它进行编辑，编辑内容仍采用上一篇《[Linux 下编译并安装配置 Qt 全过程](http://www.linuxidc.com/Linux/2011-04/35070.htm)》 中的例子，其内容如下：
- #include <QApplication> 
- #include <QLabel> 
- **int** main(**int** argc,**char** *argv[])
   
- {   
-     QApplication app(argc,argv);   
-     QLabel *label = **new** QLabel("Hello Qt!");   
-     label->show();   
- **return** app.exec();   
- }  
保存，菜单栏选择 Build -> Run，“Hello Qt!”对话框弹出，证明你写的 Qt 程序编译 OK。
这个时候，在 /home/defonds/cpp/qt/HelloWorld 目录下有个可执行文件 HelloWorld 被生成，双击它，就可以弹出“Hello Qt!”对话框。它就是你在 Linux 下使用 Qt Creator 开发出的第一个 Qt 项目。
