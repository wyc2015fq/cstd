# Qt .pro文件写法学习笔记 - 三少GG - CSDN博客
2013年06月04日 15:50:33[三少GG](https://me.csdn.net/scut1135)阅读数：1145
虽然以前用Qt写过小项目，但是这次要求较高，所以认真学习一下Qt先来研究一下.pro文件的写法。
查了手册，以下内容主要出自手册，顺便说一下.pro文件的作用，Qmake根据.pro文件生成相应的Makefile文件，（不知道Makefile是什么？google一下吧）
顺便说一下有些同学不知道怎么学习Qt，其实我们安装了Qt以后就已经附带了全部的文档，在安装目录的doc/html/index.html下打开这个文件就能查看文档里面有所有的东西包括API，新手教程，好了不罗嗦了。
Qmake Tutorial
介绍如何使用Qmake ，简单说下Qmake，就是用来生成Makefile文件，不需要手写Makefile。
1、首先创建.pro文件
     1.1 将.cpp文件加入.pro中，使用SOURCES += XX.cpp，如果是多个.cpp文件 可以另起一行写两个例子如下：
           SOURCES +=xx.cpp
           SOURCES +=xx.cpp
           or
           SOURCES +=xx.cpp xx.cpp ……
           or
           SOURCES +=xx.cpp /
                                xx.cpp
    1.2 将.h文件加入.pro中，使用HEADERS += xx.h具体列子就不写了 如上
    1.3 使用TARGET关键字指示生成的目标文件名称，默认如果不写使用跟项目文件相同的名称
          TARGET = xxxxx  
          这里需要说明，qmake会根据不同的平台生成不同的类型，windows会生成xxxxx.exe，linux会生成xxxxx
    1.4 the final step设置CONFIG变量指定报刊QT相关库，并且在最后生成的Makefile文件中加入moc uic的行，如下：
          CONFIG += qt
          如果要携带debug信息，如下
          CONFIG += qt debug
          最后，xx.pro应该如下面这个样子
          CONFIG += qt debug
          SOURCES += xx.cpp /
                                xx.cpp
          HEADERS += xx.h
          TARGET = xxxxx
2、在command line中输入qmake -o Makefile xx.pro便会生成用于编译的Makefile文件，如下：
     qmake -o Makefile xx.pro
**      Visual Stutio用户可以用如下命令生成.dsw或者.vcproj文件**
**     qmake -tp vc xx .pro**
3、根据平台选择要编译的文件使用如下命令
     win32{
SOURCES += xx.cpp
}
     or 
     unix{
SOURCES += xx.cpp
}
4、如果文件不存在 停止qmake，如下
     !exists(xx.cpp){
error("No xx.cpp file found")
}
分类: [Qt](http://www.cnblogs.com/zcq4856/category/356319.html)
