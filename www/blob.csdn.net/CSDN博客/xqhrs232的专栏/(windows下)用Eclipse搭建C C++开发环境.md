# (windows下)用Eclipse搭建C/C++开发环境 - xqhrs232的专栏 - CSDN博客
2011年06月18日 22:26:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：822标签：[eclipse																[windows																[tutorials																[compiler																[makefile																[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=tutorials&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/Jerrylew/archive/2010/04/22/5515387.aspx](http://blog.csdn.net/Jerrylew/archive/2010/04/22/5515387.aspx)
--------如果你用的是或打算用Eclipse For C++，那么从这里看,
1. 首先下载Eclipse for C++, 最新版是基于Eclipse 3.5.2的,eclipse-cpp-galileo-SR2-win32.zip
2. 解压，直接运行。注意，至少JDK你已经安装了（我用的是JDK1.6）。运行后welcome页面出现，进入Tutorials。先读Tutorial是个好习惯。
3. 首先 什么是CDT... 然后说，没包含C/C++的编译器、调试器，你得自己弄。 
--------如果用的是或打算用的是Eclipse 不For C++的,比如偶用的Eclipse Classic. 那么从这里看
1.  首先下载Eclipse, 本文基于的是eclipse-SDK-3.5.2-win32.zip
2. 解压，直接运行。注意，至少JDK你已经安装了（我用的是JDK1.6）。运行后welcome页面出现，进入Tutorials。先读Tutorial是个好习惯。
3. 然后，至Help|Install new Software,输入[http://download.eclipse.org/tools/cdt/releases/galileo](http://download.eclipse.org/tools/cdt/releases/galileo)下载CDT main feature;
CDT说它没包含C/C++的编译器、调试器，你得自己弄。 
(当然也可以先把CTD下载回来，自已搞到eclipse下,但我弄了几次都没成功)
-----------------------------------------------------------------------------------------------
4. 继续。Windows下，MinGW和Cygwin 是获取GNU toolchain的2种主要方式. 这两者最大的区别是MinGW使用Windows C的运行库，叫做mscvrt，而Cygwin使用了一组基于GPL的DLLs（GPL协议具有传染性，使用GPL协议下的软件后你自己开发的东西也要遵守GPL协议），因此MinGW避开了GPL协议。
5. MinGW和CDT能很好的整合。好吧，我们装MinGW（Minimal GNU for Windows的意思，这个下载过程相当慢，我下了大半个小时）。当前版本是MinGW-5.1.6.exe，我一股脑来了个Full install。装完后才发现这么一句：Do not install the MinGW Make feature as the MSYS version of make from step 5 is a more complete implementation of make.（不要安装MinGW的Make,
 第5步的MSYS是个更好的实现方案） 
6. OK，卸了重装。这里是完整的安装步骤： 
6.1)下载MinGW
6.2)安装MinGW base tool和g++编译器（不要安装Make）,
g77 compiler不知道是什么东西，先不管，
居然还有个Ada的编译器，先不管，
java compiler貌似有JDK就差不多，也不管, 
Objective C compiler，好像和C有点关，装上,
MinGW Make, 不是说MSYS是更好的方案，不装 : 
(有人说这里只要选中G++就可以了，我没有试过)
3)没有gdb debugger? 下载(现在最新到gdb-6.6.tar.bz2)
4)解压gdb-6.6.tar.bz2 到安装MinGW的地方，gdb-6.6/下也有一系列bin,inclue文件夹，直接拷到MinGW下面覆盖
5)下载 MSYS，现在最新到MSYS-1.0.10.exe,安装最后到一个命令界面，写2个”y”，然后输入MinGW的安装路径即可。
7. 那么现在装完了，试一下.
7.1) 首先创建一个简单的HelloWorld C++工程，这个很简单，按Wizard向导建一个模板即可。
Run的时候选Run Configurations, 然后双击C/C++ application建一个新的run configuration就行。
7.2) 下面建一个Makefile类型的工程。还是HelloWord吧，选择New C++ Project -> Makefile project -> Helloword C++ Project.
点Finish, 然后选中工程，点Build(或点那个小锤子），你会发现这个错误：(Cannot run program "make": Launching failed)，啊，我们的make.exe还没设。选中工程，直接Alt-Enter到工程属性页，把msys的bin加到Path里。
(如果事先将msys的bin加到了系统的Path变量里，这个错误就不会发生.)
再Build, 完成.
-------------------
 补充下:
上面说的东西可以在sourceforge.net上下载到，比如
MinGW: [http://sourceforge.net/projects/mingw/files/](http://sourceforge.net/projects/mingw/files/)
gdb debugger: [http://downloads.sourceforge.net/mingw/gdb-6.6.tar.bz2](http://downloads.sourceforge.net/mingw/gdb-6.6.tar.bz2)
MSYS-1.0.10.exe: [http://downloads.sourceforge.net/mingw/MSYS-1.0.10.exe](http://downloads.sourceforge.net/mingw/MSYS-1.0.10.exe)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/Jerrylew/archive/2010/04/22/5515387.aspx](http://blog.csdn.net/Jerrylew/archive/2010/04/22/5515387.aspx)

