# linux上安装Qt4.8.6+QtCreator4.0.3 - DoubleLi - 博客园






一、Qt简介 
Qt是1991年奇趣科技开发的一个跨平台的C++图形用户界面应用程序框架。它提供给应用程序开发者建立艺术级的图形用户界面所需的所有功能。Qt很容易扩展，并且允许真正地组件编程。 
准备工作 
操作系统：centos6.5 
位数：64位

二、安装 
1、获取源码Qt4.8.6
`http://download.qt.io/official_releases/qt/4.8/4.8.6/`- 1

![](https://img-blog.csdn.net/20161021140301711)

2、获取源码QtCreator4.0.3
`http://download.qt.io/official_releases/qtcreator/4.0/4.0.3/`- 1

![这里写图片描述](https://img-blog.csdn.net/20161021140645024)

2、安装QtCreator4.0.3
`./qt-creator-opensource-linux-x86_64-4.0.3.run `- 1

进入QtCreator安装界面，指定安装位置，我的位置是/usr/local/qtcreator-4.0.3 
然后就是按照提示一直到安装结束 
安装好了QtCreator后，可以运行，但是却什么也做不了，因为我还没有安装Qt库也就是Qt4.8.6

3、安装Qt4.8.6 
（1）Qt库的话因为是源码，所以，我们需要对它进行编译，然后安装。首先解压
`#tar xzvf qt-everywhere-opensource-src-4.8.6.tar.gz`- 1

（2）进入目录并配置

```
#cd ./qt-everywhere-opensource-src-4.8.6
./configure
```
- 1
- 2

输入好配置命令后，会有一些提示。 
输入c并且回车，表示使用社区版。 
输入o并且回车，表示使用开源版。 
这里我选择o 
然后输入yes并且回车，表示同意协议。

（3）出错处理 
如果出现以下错误：
`cc1plus: 错误：无法识别的命令行选项“-std=c++11”`- 1

原因是本系统的gcc版本太低，解决办法见我的另一篇：
`http://blog.csdn.net/qq_22790049/article/details/52873915`- 1

如果出现以下错误：

```
The test for linking against libxcb and support libraries failed!
 You might need to install dependency packages, or pass -qt-xcb.
```
- 1
- 2

解决办法
`#yum install xcb*`- 1

如果出现以下错误：

```
Basic XLib functionality test failed! 
You might need to modify the include and library search paths by editing QMAKE_INCDIR_X11 and QMAKE_LIBDIR_X11 in /home/zhu/Qt/qt-x11 opensource-src-4.8.6/mkspecs/linux-g++
```
- 1
- 2

解决办法：
`#yum install libX*`- 1

最终生成了我们需要的Makefile

4、编译
`#make`- 1

然后你就可以去看电影了，至少也要1—2个小时

5、安装
`#make install`- 1

在耐心等待几分钟吧，默认安装/usr/local/Trolltech/目录下

6、设置环境变量
`# vim /etc/profile`- 1

在最后一行添加

```bash
export QTDIR=/usr/local/Trolltech/Qt-4.8.6
export PATH=$QTDIR/bin:$PATH
export MANPATH=$QTDIR/man:$MANPAT
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
```
- 1
- 2
- 3
- 4

保存，让环境变量生效
`#source  /etc/profile`- 1

7、测试

```
qmake -v
QMake version 2.01a
Using Qt version 4.8.6 in /usr/local/Trolltech/Qt-4.8.6/lib
```
- 1
- 2
- 3

安装完成

三、Qt4.8.6+QtCreator4.0.3配置 
上面只是完成了两个包的安装，但是想要让他们协作，还是需要再进行点配置的 
第一步：tools>options>Build and Run 
然后选择Qt Versions，使用 Browse选择刚才安装的Qt库 
![这里写图片描述](https://img-blog.csdn.net/20161021144923222)
然后点击应用 
第二步：选择Kits，手动添加一个，GCC，GDB会自动检测，这里需要指定Qt Version。 
默认是None，选择我们刚才添加的Qt 4.8.6 
![这里写图片描述](https://img-blog.csdn.net/20161021145042138)
然后，这样就完成了所有的安装。









