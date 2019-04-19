# QT中知识点 - LC900730的博客 - CSDN博客
2017年08月17日 23:38:39[lc900730](https://me.csdn.net/LC900730)阅读数：197
## qml应用程序
QML 是一种声明式语言，它提供了一组接口来描述可视化组件和他们之间的互动。它是一个高度可读的语言，并且被设计成使组件以一个动态的方式相互连接。同时它使得组件很容易被复用以及创建定制的用户界面。使用QtQuick模块，设计者和开发者可以很容易地使用QML建立带有流畅动画的用户界面，并将这些界面连接到后端C++库上面。
## QML简介
QML是一个用户接口规范和编程语言，它允许开发者和设计者创建高性能的、流畅的动画和视觉效果的应用。QML提供了一个高度可读、声明式的类似json语法，并支持和js表达式相结合来达到动态属性的绑定。
## qt quick
Qt quick是qml类型与功能标准库，它包含可视化类型、交互类型、动画和模型和视图，粒子效果和着色效果。一个QML应用程序开发者可以通过一个import语句来使用这些功能。
## Qt工程的目录结构
### 使用子目录路径
```
在向工程添加或者新建文件时候，创建子文件，并将文件创建子目录下。将创建的类放在子目录下。
```
在pro文件中加入src/xxx.h和src/xxxx.h。在使用的时候，头文件也要加上相对路径，否则会出现找不到。或者向pro文件中添加环境变量：
```
INCLUDEPATH+=src
```
实际目录是没有这2个文件夹的。
### pri文件
pri文件的作用是：将pro文件的一部分放入每个路径的pri文件中，分别管理，最后在将pri文件包含到pro中即可。
#### 1.手动创建pri文件
手动创建子目录，并创建空文件或者文本文件，将后缀改为pri 
注意pri文件的文件名就是Qt工程显示的子目录名，而不是实际手动创建的子目录。建议将pri文件名和子目录名保存一致。
#### 2.包含pri文件
在pro文件中包含pri文件，使用include命令
```
include(src/src.pri)
```
在pro文件中是使用哪个模块的话，就把那一套头文件和源文件包含了进来。 
如
```
contains(CONFIG,QGC_DISABLE_BUILD_SETUP){
    message("Disable standard build setup")
}else{
    include(QGCSetup.pri);
}
```
如新建testpri文件夹，然后新建文件testpri.pri,然后新建一个类放入testpri文件中，那么实际上在使用的时候 
在pro中include(“testpri/testpri.pri”) 
在0817test/testpri/testpri.pri中需要 
HEADERS+=\ 
$PWD/testpri.hSOURCES+=\	$PWD/testpri.cpp 
在Qt项目中会显示Headers/testpri/testpri.h 
Sources/testpri/testpri.cpp
以上是Qt默认情况，没有修改显示设置等。
一般情况下需要pri文件的名称与文件夹名称相同。 
如果pri文件名称与文件夹名称不同，那么在pro中引入的时候需要引入按照文件夹名称引入，
`$$PWD/test/testpri.pri(假如文件夹名称是test,里面pri文件名称是testpri.pri)`
### qt文件层次
Qt项目中的目录结构与实际目录结构区分： 
默认情况下：一个项目中的所有文件都是在项目的这一个文件夹里，尽管Qt工程中有Headers、Sources、Resources等。如果想分层次的话，那么需要在项目所在文件夹里面，在新建目录。 
如：项目名称是0817test，放入0817test文件夹中 
目录名称是0817test,Qt打开项目，则有Headers、Resources等，在文件系统中进入项目目录，新建文件夹src，在Qt中新建文件test.h/test.cpp，选择放入src中，那么实际上在文件系统中， 
0817test/src/test.h 
0817test/src/test.cpp 
而在Qt项目中，显示是Headers/src/test.h, 
Sources/src/test.cpp。
在pro中生成的HEADERS是 
HEADERS+=\ 
src/test.h
SOURCES+=\ 
src/test.cpp
