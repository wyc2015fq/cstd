# log4qt 使用（二） - baidu_33570760的博客 - CSDN博客
2017年05月03日 16:09:41[carman_风](https://me.csdn.net/baidu_33570760)阅读数：298
转自： http://blog.csdn.net/dbzhang800/article/details/6917172
- 
在前面[Log4Qt
 使用笔记(一)](http://blog.csdn.net/dbzhang800/article/details/6916948)中，我们举例演示了Log4Qt的简单使用。在继续深入学习Log4Qt之前，我想似乎有必要简单整理一下qmake或者说Qt Creator下如何使用Log4Qt这些第三方库的。
本文标题取为：**qmake下使用一例(以Log4Qt为例)**可能会更好一点。想想还是就这样吧。
## 如何使用第三方库？
不少人使用第三方库时都是：
- 下载源码
- 找出必要的文件
- 然后添加进自己的工程
一般来说，我们可以将所有和这个库有关的东西放置到一个liblog4qt.pri这样的文件中，然后在需要使用的工程中直接包含这个文件即可：
比如：我们先看看，上一节中我所写例子所用的pro文件：
include(../../liblog4qt.pri)
CONFIG   += console
DESTDIR = $$PROJECT_BINDIR
SOURCES += main.cpp
有一点不同：这个文件背后，使用的不是直接包含文件，而是动态库。这也是本文的内容。
### 题外
- 
注：在[http://code.google.com/r/dbzhang800-qextserialport/](http://code.google.com/r/dbzhang800-qextserialport/)这个第三方库中，我采用的另一种思路
可以采用下面三种方式来使用QextSerialPort:
    静态库
    动态库
    直接包含源文件 
不管使用哪种方式，使用时只需要在工程文件内添加
include(pathToPri/qextserialport.pri)
或许应该考虑将Log4Qt也改造成这种使用方式
- 
注：本文或许算是[qmake使用实践：包含动态库的Qt4工程](http://blog.csdn.net/dbzhang800/article/details/6317006)一文的后续。
- 内容似乎太乱了，只好限定在：Windows下，这样可以省点代码
### 工程结构示例
一个简单的包含动态库且又使用第三方库的Qt程序，应该都可以这样来组织工程结构：
+ 
|-- myProject.pro
|-- apps\
|     |-- hello.pro
|     `-- main.cpp
|-- libA\
|    |-- libA.pro
|    |-- libA.pri
|    `-- main.cpp
|-- libB\
|    |-- libB.pro
|    |-- libB.pri
|    `-- main.cpp
...
|-- libLog4Qt
|    |-- libLog4Qt.pro
|    `-- libLog4Qt.pri
|
`-- 3rdparty\
     |-- log4qt\
     |      `....
     |-- qextserialport\
     |--     `.... 
所有的第三方代码放置到3rdParty下。程序由一个可执行程序和若干个动态链接库构成（此处忽略了文档doc和单元[测试](http://lib.csdn.net/base/softwaretest)tests相关的目录）
最终目的是：
- 执行make或者点击Qt Creator构建或运行时，所有这些东西都会自动生成。自动放置到合适的路径(windows下)或者设置合适的rpath(Linux下)，(本文只考虑windows下)
- 不去动第三方库的代码。
## 使用Log4Qt
由于程序中包含很多动态库，而几乎每个动态库都需要使用log功能，此时直接包含[Log4Qt](http://127.0.0.1/wiki/Log4Qt)的源码或者使用静态库都不是太好的注意。所以：**使用动态库**
其实只要写好工程文件，使用动态库也很简单：
|创建libLog4Qt.pro|用来构建动态库|可以单独存在，也可以被上级的pro文件所包含(通过subdir)|
|----|----|----|
|创建libLog4Qt.pri|用来构建和使用动态库|任何需要使用log工程的pro文件，只需要include该文件即可|
不过这两个文件，乍看起来，可能会让人比较头大：（但如果所有的动态库都是这么弄，倒是很简单了）
### libLog4Qt.pri
一开始定义几个变量：
- 我们下载的 log4qt 所放置的目录
- 我们的库文件准备放置到的路径(所有的动态库都需要这个，放置到单独一个common.pri中更好)
- 库文件的名字（使用了qtLibraryTarget，这样在windows下debug和release自动拥有不同的名字，而且对我们是透明的，因为我们只在这一个文件内指定库文件的名字。）
LOG4QTSRCPATH = $$PWD/../3rdparty/log4qt/src
PROJECT_LIBDIR = $$PWD/../lib
PROJECT_BINDIR = $$PWD/../bin
TEMPLATE += fakelib
LIBLOG4QT_NAME = $$qtLibraryTarget(hlog4qt)
TEMPLATE -= fakelib
INCLUDEPATH += $$LOG4QTSRCPATH
DEPENDPATH += $$LOG4QTSRCPATH
!log4qt-buildlib{
    DEFINES += LOG4QT_IMPORTS
    LIBS +=  -L$$PROJECT_LIBDIR  -l$$LIBLOG4QT_NAME
}else{
    DEFINES += LOG4QT_EXPORTS
    INCLUDEPATH += -L$$PROJECT_LIBDIR $$LOG4QTSRCPATH/helpers \
                   $$LOG4QTSRCPATH/spi \
                   $$LOG4QTSRCPATH/varia
    DEPENDPATH += $$LOG4QTSRCPATH/helpers \
                   $$LOG4QTSRCPATH/spi \
                   $$LOG4QTSRCPATH/varia
    include($$LOG4QTSRCPATH/Log4Qt.pri)
}
无论是使用，还是构建动态库，我们都使用这个文件。
- 对于前者，只需要指定头文件目录，库文件名即可
- 
对于后者，列出所有的源文件（由于它提供了[Log4Qt](http://127.0.0.1/wiki/Log4Qt).pri的文件，这儿就直接包含了）
## liblog4qt.pro
这个文件中没有任何新东西，就是构建一个动态库
TEMPLATE = lib
CONFIG += log4qt-buildlib
QT += core xml network
include(liblog4qt.pri)
TARGET = $$LIBLOG4QT_NAME
DESTDIR = $$PROJECT_LIBDIR
win32{
    DLLDESTDIR = $$PROJECT_BINDIR
    QMAKE_DISTCLEAN += $$PROJECT_BINDIR/$${LIBLOG4QT_NAME}.dll
}
CONFIG += debug_and_release build_all
LOG4QT_VERSION_MAJOR = 1
LOG4QT_VERSION_MINOR = 0
LOG4QT_VERSION_RELEASE = 0
LOG4QT_VERSION = '\\"$${LOG4QT_VERSION_MAJOR}.$${LOG4QT_VERSION_MINOR}.$${LOG4QT_VERSION_RELEASE}\\"'
DEFINES += LOG4QT_VERSION_STR=\"$${LOG4QT_VERSION}\"
DEFINES += LOG4QT_VERSION=$${LOG4QT_VERSION}
VERSION = 1.0.0
DEPENDPATH += src src/helpers src/spisrc/ varia
INCLUDEPATH += src src/helpers src/spi src/varia
