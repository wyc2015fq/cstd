# QT笔记 - L_Andy的专栏 - CSDN博客

2017年06月04日 16:25:53[卡哥](https://me.csdn.net/L_Andy)阅读数：394


Qt下载：

download.qt.io

mingw 的gcc 版本 

可在命令行中

![](https://img-blog.csdn.net/20170604161915820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

终端编译Qt工程

![](https://img-blog.csdn.net/20170604161929201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170604161936061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170604161944851?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170604161950211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170604162002593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170604162018242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

///////////////////////////////////////////////////////////////////////////////////////////////

Ctrl  +  H水平布局

Ctrl +  L 垂直布局

alt + shift + R 预览

Ctrl +  R 运行  

Ctrl +  i 代码排版

Shift +  F4 UI编辑器和代码编辑器

F3编辑部件

F4编辑信号和槽

代码编码格式设置

操作如下：编辑->Select Encoding进行编码设置

//////////////////////////////////////////////////////////////////////////////////////////////////////

qt在.pro添加这三行，然后release版本调试。

# QMAKE *.pro-file: enable debugging info in release mode

#

# gcc

#QMAKE_CXXFLAGS_RELEASE += -O0 -g

#QMAKE_CFLAGS_RELEASE += -O0 -g

#QMAKE_LFLAGS_RELEASE =

# msvc

QMAKE_CXXFLAGS_RELEASE += /Zi

QMAKE_CXXFLAGS_RELEASE += /Od

QMAKE_LFLAGS_RELEASE += /DEBUG

# DEFINES +=QT_NO_DEBUG_OUTPUT # disable debug output

DEFINES -=QT_NO_DEBUG_OUTPUT #enable debug output

//注意：

屏蔽release版本中所有QDebug()提示信息：

如果要在命令行输出时不打出所有qdebug信息，可以在pro文件中添加DEFINES+=QT_NO_DEBUG_OUTPUT

/////////////////////////////////////////////////////////////////////

更新UI界面的编辑(当界面出问题时)。

user interface complier的缩写 UIC(用户界面的编译器)

![](https://img-blog.csdn.net/20170604162049859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

//////////////////////////////////////////////////////////////////////////////////////////////////////

![](https://img-blog.csdn.net/20170604162055337?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

//////////////////////////////////////////////////////////////////////////////////////////////////////

这3个东西到底是做什么呢？

QTextCodec::setCodecForCStrings(textc);

QTextCodec::setCodecForTr(textc);

QTextCodec::setCodecForLocale(textc);

3个很简单的东西。字节流<==> 字符串

//////////////////////////////////////////////////////////////////////////////////////////////////////

一些使用Qt Creator的tip

1.    使用network或opengl或sql等模块后编译不通过

在pro文件中写如下语句：QT += network 或 QT += opengl 或 QT += sql

2.      代码自动完成功能区分大小写

在Tools->Options菜单中的Text Editor->Completion中，勾掉“Case-sensitivecompletion”，这样就不会因为你大小写错了而使整个completion都消失掉。

3.      Debug时提示没有debug helper

如下图，在Options菜单中红圈的位置点击Rebuild。

4.      Qrc中增加了图片，但是在程序中用不了

Qrc文件中可以增加Qt程序使用的资源文件。这些文件会被编译到exe文件中。增加资源时需要先增加前缀prefix。看看你的prefix写的对不对。如果前缀是“/”那么一个典型的图片文件路径可能是“:/Resources/Images/aaa.png”。另外，在qrc中，文件名和路径名是区分大小写的。我曾经在这里卡了好几个小时，最后都怀疑Qt Creator是不是“水货”了。

5.      Qt Creator在Windows系统中，怎样链接VC生成的动态链接库

这个问题曾经困扰了我一整天。我想的是按照VC中的方法，增加include文件，增加lib文件，然后编译即可。谁知链接时总是出现问题。提示是undefined reference to XXXXX。查了手册和网络，原来这是mingw用户在windows上经常遇到的问题，而且好像至今没有完美的解决方案。这个错误的起因是因为VC生成lib的_stdcall函数名与mingw生成的不一致。一种解决方案是使用libdll等工具生成新的用于mingw的lib，另一种解决方案是把代码全拿过来重新编译（汗）。

最后我也不知道为什么自己试出来一个新的方法，这个方法尚未从技术上找到原因。也未经过广泛的测试。有兴趣的TX可以试试。

如果你要链接到一个abc.lib文件，头文件是abc.h，动态链接库是abc.dll。那么在pro文件中先加入下面一行

INCLUDEPATH += D:/Qt/include

INCLUDEPATH是一个编译变量，存储了所有包含include文件的路径，这个目录下保存了abc.h文件。然后再加入下面一行：

LIBS += D:/Qt/bin/abc.dll

LIBS也是一个编译变量，存储了所有需要链接的库文件。看清楚了，不是“abc.lib”哦，是“abc.dll”。神奇吧，这样居然可以链接成功了（lib文件可以删了）。喜欢研究的TX可以研究研究mingw究竟是怎么做的。

6.      Qt项目更换文件目录后编译失败

Qt Creator编译时不支持中文路径名。对于带空格的路径，如果你想在pro文件中加入，需要使用$$quote来指定，

如：$$quote(C:/mylibs/extra libs/extra.lib)。另外，如果你习惯了Windows下‘/’的话，最好改改你的习惯，在Qt中路径习惯用‘/’。

///////////////////////////////////////////////////////////////////////////////////////////////

1　解决问题：没有用选项“/LIBPATH:”指定的参数

2　LIBS+=到底如何用！！！！！！

语法是 LIBS+= -L直接打地址 -l直接打库名
注意点：-L后面没有空格，直接跟着地址，然后空格，-l后面是去掉lib之后的文件名。(真TMDSB！）
一个例子：如我们有E:\CLibraries\NewmatL\debug\libNewmatL.a这个文件，则：
LIBS += -LE:\CLibraries\NewmatL\debug\ -lNewmatL
或者如果需要编译的pro在E:\CLibraries\TEST-build-desktop下，则可以：

NEWMAT_ROOT = ..\NewmatL

LIBS += -L$${NEWMAT_ROOT}\debug -lNewmatL


