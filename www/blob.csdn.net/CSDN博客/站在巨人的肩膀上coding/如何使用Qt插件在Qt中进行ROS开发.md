# 如何使用Qt插件在Qt中进行ROS开发 - 站在巨人的肩膀上coding - CSDN博客





2018年05月06日 22:53:27[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1619








## 如果想快速安装，就直接到第5部分，apt-get方式安装

## 一、前言

本文介绍一种Qt下进行ROS开发的完美方案，使用的是ros-industrial的Levi-Armstrong在2015年12月开发的一个Qt插件ros_qtc_plugin，这个插件使得Qt“新建项目”和“新建文件”选项中出现ROS的相关选项，让我们可以直接在Qt下创建、编译、调试ROS项目，也可以直接在Qt项目中添加ROS的package、urdf、launch，感谢Levi-Armstrong。目前这个插件还在不断完善，有问题或者其他功能建议可以在ros_qtc_plugin的[项目主页的讨论区](https://github.com/ros-industrial/ros_qtc_plugin/issues)提出。

本文是用的操作系统是ubuntu kylin 14.04中文版，ROS版本是indigo，Qt版本是Qt5.5.1(Qt Creator 4.0.3)  

本文地址：[http://blog.csdn.net/u013453604/article/details/52186375](http://blog.csdn.net/u013453604/article/details/52186375)
视频教程：[ros_qtc_plugin插件作者Levi-Armstrong录制的插件使用教程](http://download.csdn.net/detail/u013453604/9601165)
参考： 
[ROS wiki IDEs](http://wiki.ros.org/IDEs#Qt_Creator_Plugin_for_ROS)
[1. Setup ROS Qt Creator Plug in](https://github.com/ros-industrial/ros_qtc_plugin/wiki/1.-Setup-ROS-Qt-Creator-Plug-in)
[2. Setup Qt Creator for ROS](https://github.com/ros-industrial/ros_qtc_plugin/wiki/2.-Setup-Qt-Creator-for-ROS)
[3. Debugging Catkin Workspace](https://github.com/ros-industrial/ros_qtc_plugin/wiki/3.-Debugging-Catkin-Workspace)
[4. Where to find Qt Creator Plug in Support](https://github.com/ros-industrial/ros_qtc_plugin/wiki/4.-Where-to-find-Qt-Creator-Plug-in-Support)
[github ros-industrial/ros_qtc_plugin项目主页](https://github.com/ros-industrial/ros_qtc_plugin)
[插件使用问题](https://github.com/ros-industrial/ros_qtc_plugin/issues?q=is%3Aissue+is%3Aclosed)

2016-8-18更新：
- [apt方式安装](http://blog.csdn.net/u013453604/article/details/52186375#t12)
Levi-Armstrong更新了他的插件安装方式，现在你可以添加他的源直接用apt-get方式安装，请见第五部分的补充 
如果apt-get方式方式下载过程太慢的话还是按照下文的方式安装吧 
**博主建议采用apt-get方式安装，不容易出错**
- [头文件包含的问题](http://blog.csdn.net/u013453604/article/details/52186375#t13)
补充了ros相关头文件的包含配置

2017-10-28更新： 
1.[项目wiki地址更新（需要翻墙）](http://ros-industrial.github.io/ros_qtc_plugin/)
2. [apt方式安装](http://blog.csdn.net/u013453604/article/details/52186375#t12)

## 二、安装插件ros_qtc_plugin

插件要求Qt 5.5.0以上的版本，下载Qt 5.7.0请到 
[Qt Offline Installers](https://www.qt.io/download-open-source/#section-3)
[Qt 5.7.0 for Linux 64-bit (715 MB)    (info)](http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run.mirrorlist)
国内可以从以下源高速下载： 
[中国科学技术大学](http://mirrors.ustc.edu.cn/qtproject/archive/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run)
[清华大学](http://mirrors.tuna.tsinghua.edu.cn/qt/archive/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run)
[北京理工大学](http://mirror.bit.edu.cn/qtproject/archive/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run)
下载下来的`qt-opensource-linux-x64-5.7.0.run` 记得右键“属性”——“权限”勾选“允许作为程序执行文件”，再双击就能安装了

### 1、修改系统配置文件让Qt启动器选择新版的Qt Creator

首先需要修改下面的文件目的是让Qt的启动器选择新版的Qt Creator，我原本安装的Qt是Qt5.5.1(Qt Creator 3.0.1)，然后后面安装插件的脚本帮我下载了最新版的Qt Creator 4.0.3，花了好长时间下载
`$ sudo gedit /usr/lib/x86_64-linux-gnu/qt-default/qtchooser/default.conf`
打开文件看到的是下面两行

```
/usr/lib/x86_64-linux-gnu/qt4/bin
/usr/lib/x86_64-linux-gnu
```

我们需要把它们替换成如下两个路径

```
/home/zjl/Qt5.5.1/5.5/gcc_64/bin
/home/zjl/Qt5.5.1/5.5/gcc_64/lib
```

注意`/home/zjl`是我的主文件夹绝对路径 
后面的路径是从Qt安装目录下面进入`5.5/gcc_64/`找到`bin`和`lib`文件夹，用这两个文件夹路径替换掉`default.conf`文件原来的两行就是了，我安装Qt的时候选择安装在主文件夹的Qt5.5.1下，并且我的Qt版本是5.5，所以路径如上所示，安装到其他地方或者其他版本的类似

**遇到报错请看下面：**
这个插件对Qt的版本要求是最低Qt 5.5.0，如果你的Qt版本过低请安装新版本。 
如果多个Qt版本共存，而你修改上述文件仍无法进行编译，报错有类似下面的

```
make: *** 没有规则可以创建目标“clean”。 停止。
Project MESSAGE: Cannot build Qt Creator with Qt version 5.2.1.
Project ERROR: Use at least Qt 5.5.0.
make: *** 没有指明目标并且找不到 makefile。 停止。
```

那么你就要到`/usr/lib/x86_64-linux-gnu/` 下看看到底有几个`qtchooser` 了，博主有台电脑曾经是Qt5.2.0，然后卸载，重装了Qt5.7.0，结果上述脚本里面的`cd $QTC_BUILD && qmake $QTC_SOURCE/qtcreator.pro -r` 这行一直过不了，原因是`qtcreator.pro` 里面设置了版本检查，而Qt选择器选择的一直是Qt5.2.0，上述修改没有起到作用，我到`/usr/lib/x86_64-linux-gnu/` 下面查看发现两个地方有`qtchooser`

```
/usr/lib/x86_64-linux-gnu/qtchooser/default.conf
和
/usr/lib/x86_64-linux-gnu/qt-default/qtchooser/default.conf
```

于是我把另一个`qtchooser` 下面的`default.conf` 也做了上述修改，即
`$ sudo gedit /usr/lib/x86_64-linux-gnu/qtchooser/default.conf`
把里面的内容修改为

```
/home/zjl/Qt5.7.0/5.7/gcc_64/bin
/home/zjl/Qt5.7.0/5.7/gcc_64/lib
```

### 2、运行Qt Creator配置脚本

下载这个**[脚本setup.sh](https://raw.githubusercontent.com/ros-industrial/ros_qtc_plugin/master/setup.sh)** （你可以右键“另存为”来下载它），我把它放在了主目录`/home/zjl`下，那么`cd`到该位置执行以下命令来运行它

```
$ cd ~
$ bash setup.sh -u
```

这里`$ bash setup.sh -u` 命令前面不需要加`sudo` 否则编译过程中新建的文件夹和桌面的Qt快捷方式都属于root用户，不用命令行你是无法修改和删除的。。。。 
期间脚本从github下载一个新版的Qt Creator源码并重新编译花了好长时间，下载速度只有10kb/s，只好让它下载一晚上。。。期间如果网络断开导致下载中断你还得再运行一次，再等好久，可以从官网下载最新的Qt Creator 4.0.3源码，具体请看下面的说明

值得注意的是这个脚本中有个路径需要根据实际情况修改，如果你的ubuntu是英文版可以不用管，如果你是中文版的系统，你的桌面文件夹名字不是`Desktop`而是`桌面`的话你就要修改`setup.sh`文件中的第84行，原文是

```bash
rm -f $HOME/Desktop/QtCreator.desktop
ln -s $DESKTOP_FILE $HOME/Desktop/QtCreator.desktop
```

改成

```bash
rm -f $HOME/桌面/QtCreator.desktop
ln -s $DESKTOP_FILE $HOME/桌面/QtCreator.desktop
```

我修改过的脚本可以在[这里](http://download.csdn.net/detail/u013453604/9607284)下载

这样它就会在桌面生成一个名为`QtCreator.desktop`的快捷方式，你用这个快捷方式打开Qt就可以正常使用ros_qtc_plugin插件了

**下载QtCreator太慢请看下面：**
脚本中是从github下载90MB的QtCreator最新版源码，国内访问github网速太慢，可以从 
Qt官网下载Qt Creator 4.0.3的源码压缩包[qt-creator-opensource-src-4.0.3.tar.gz (21 MB)](http://download.qt.io/official_releases/qtcreator/4.0/4.0.3/qt-creator-opensource-src-4.0.3.tar.gz)（还有其他[源](http://download.qt.io/official_releases/qtcreator/4.0/4.0.3/qt-creator-opensource-src-4.0.3.tar.gz.mirrorlist)） 
下载下来之后解压得到101MB的`qt-creator-opensource-src-4.0.3` 文件夹，将其改名为`qt-creator`，放在qtc_plugins文件夹下（qtc_plugins与脚本setup.sh处于同一级目录），重新执行脚本即可

“文件”——“新建文件或项目”你会看到如下界面

![这里写图片描述](https://img-blog.csdn.net/20160812002606673)

## 三、使用ros_qtc_plugin插件新建项目

使用插件的“新建项目”中的“Import ROS Workspace”选项不仅可以新建工作空间还可以导入现有的工作空间，而“新建文件”中的“ROS”下面的“Package”、“Basic Node”等选项可以创建package和和节点、launch文件、urdf文件等 
现在我们来创建一个新的catkin工作空间,并且在里面创建一个package 

### 1、新建项目

“文件”——“新建文件或项目”，选择”Import ROS Workspace”——“choose”如上图

然后填写catkin工作空间的名字和位置，如下图    

![这里写图片描述](https://img-blog.csdn.net/20160812002720065)

我这里的Name和catkin工作空间文件夹名字相同（也可以不同）名为`catkin_new`，然后选择“浏览”——“创建文件夹”，创建一个名为`catkin_new`的文件夹，如下图，按回车，再选择“打开”

![这里写图片描述](https://img-blog.csdn.net/20160812002917049)

出现一个对话框，询问新建的工作空间未被初始化，是否执行初始化，我们选择“yes” 
这里相当于执行一次`catkin_init_workspace`命令，使工作空间初始化

![这里写图片描述](https://img-blog.csdn.net/20160812003114925)

然后点击下方的“Generate Project File”，再点击“下一步” 
在“项目管理”步骤可以配置版本控制系统，我们选择“完成”，如下图

![这里写图片描述](https://img-blog.csdn.net/20160812003213800)

建好的工作空间`catkin_new`是空的，如下图

![这里写图片描述](https://img-blog.csdn.net/20160812003258041)

### 2、创建package

接下来我们在这个工作空间创建一个新的package 
在`catkin_new`下面的`src`上右键单击，选择“添加新文件”，如下图  

![这里写图片描述](https://img-blog.csdn.net/20160812003342191)

然后选择“ROS”——“package”——“choose”

![这里写图片描述](https://img-blog.csdn.net/20160812003411926)

填写package的名字，如`test1`
填写作者(Authors)和维护者(Maintainers)，如果你的Qt环境部没有配置中文支持请不要用中文，否则无法新建package，或者新建的package里面的`package.xml`会是空文件 
在“Dependencies”下的“Catkin”一栏填写依赖，通常用C++写节点需要添加对`roscpp`的依赖 
如下图

![这里写图片描述](https://img-blog.csdn.net/20160812003456200)

点击“完成”  

### 3、添加新的节点

目前这个package还是空的，没有任何节点 
我们在test1的src文件夹上右键单击，选择“添加新文件”  

![这里写图片描述](https://img-blog.csdn.net/20160812003748210)

选择“ROS”——“Basic Node”——“choose”，创建一个节点，也就是新建一个`.cpp`文件

![这里写图片描述](https://img-blog.csdn.net/20160812004034696)

节点名称写为“node1”，则生成一个名为`node1.cpp`的源文件

![这里写图片描述](https://img-blog.csdn.net/20160812004104303)

用ROS模板新建的`node1.cpp`的源文件如下图，是自动生成的一个`Hello World`程序

![这里写图片描述](https://img-blog.csdn.net/20160812004445479)

### 4、编译节点

为了让它编译成可执行文件我们还需要编辑test1的makefile，如下图 
为test1的`CMakeLists.txt`加上下面几行，意思是把`node1.cpp`编译成名为`node1`的节点

```
add_executable(node1 src/node1.cpp)
target_link_libraries(node1
  ${catkin_LIBRARIES}
)
```

**提醒**
值得注意的是上面几行需要放在`include_directories`后面，在构建项目的时候才会在`catkin_new/devel/lib/test1`里面生成可执行文件`node1.exe`，这样后面进行“运行配置”的时候`test1`包下面才会出现`node1`节点； 
如果把上面几行放在`include_directories`前面会使得构建过程直接在`catkin_new/build/test1`下面生成可执行文件`node1.exe`，而`devel`文件夹下找不到可执行文件，导致后面选择运行节点的时候不会出现这个节点

![这里写图片描述](https://img-blog.csdn.net/20160812004530980)

修改完`CMakeLists.txt`之后点击Qt左下角的锤子图标“构建项目catkin_new”,点击底边栏的“4 编译输出”可以看到编译结果，如下图  

![这里写图片描述](https://img-blog.csdn.net/20160812004615981)

### 5、运行节点

如果你想运行某个节点还需要修改运行配置，点击左侧兰的“项目”，在“构建设置”页的“编辑构建配置”一栏选择“Default”，然后在下图所示光标处点击切换到“运行设置”页，可以看到一个“Add Run Step”的选项，点击出现一个“ROS Step”配置项，可以通过下拉菜单选择package和target（就是该package下的节点），下拉菜单可以通过键入首字母初步定位到你要找的package位置 
“Add Run Step”的选项其实也是一个下拉菜单，可以选择“Add Launch Step”的选项运行launch文件 
配置好如图，点击Qt左下角的小电脑图标，选择“Default”的构建配置，再点击Qt左下角的绿色三角形即可运行“运行设置”页下面配置好的所有Step

![这里写图片描述](https://img-blog.csdn.net/20160812004715558)

运行结果可以点开底边栏的“ROS Terminals”查看，如下图， 
打印了一句 
`[ INFO] [1470932511.713873863]: Hello world!`
如果没有“ROS Terminals”，可以在下图鼠标所在位置底边栏有两个上下三角形的地方打开“ROS Terminals”窗口

![这里写图片描述](https://img-blog.csdn.net/20160812004759949)

## 四、ROS界面开发

结合我另一篇博客的内容[在ROS中使用QT界面](http://blog.csdn.net/u013453604/article/details/52167213#t5)
我们可以使用`catkin_create_qt_pkg` 命令在工作空间创建好一个带GUI的ROS package，然后用ros_qtc_plugin插件的“Import ROS Workspace”功能导入这个这个package所在的工作空间，然后可以看到如下图所示

![这里写图片描述](https://img-blog.csdn.net/20160812101515121)

我用 `catkin_create_qt_pkg` 命令创建的package名为`testgui`，它包含一个自动生成的界面文件和相应的节点，你可以在左侧栏双击`main_window.ui` 文件然后在“设计”页面编辑UI界面，我在自动生成的页面上添加了一个空的按钮名为“PushButton”，然后配置“运行设置”运行`testgui` 的`testgui` 节点，可以看到如下图界面，点击界面上的“connect”按钮，节点就会在`/chatter` 话题上不断发送字符串，然后在界面上显示出来

![这里写图片描述](https://img-blog.csdn.net/20160812102310694)

## 五、补充

**2016-8-18更新：**

### 1、apt-get方式安装

Levi-Armstrong更新了他的插件安装方式，现在你可以添加他的源直接用apt-get方式安装

Ubuntu 14.04:

```
sudo add-apt-repository ppa:levi-armstrong/qt-libraries-trusty
sudo add-apt-repository ppa:levi-armstrong/ppa  
sudo apt-get update && sudo apt-get install qt57creator-plugin-ros
```

如果你以前使用过旧的软件源以下命令可以删除它们

```
sudo add-apt-repository --remove ppa:beineri/opt-qt57-trusty
sudo add-apt-repository --remove ppa:beineri/opt-qt571-trusty
```

或者尝试以下命令删除

```
sudo rm /etc/apt/sources.list.d/beineri-opt-qt57-trusty-trusty.list
sudo rm /etc/apt/sources.list.d/beineri-opt-qt571-trusty-trusty.list
```

Ubuntu 16.04

```
sudo add-apt-repository ppa:levi-armstrong/qt-libraries-xenial  
sudo add-apt-repository ppa:levi-armstrong/ppa  
sudo apt-get update && sudo apt-get install qt57creator-plugin-ros
```

删除旧的软件源请运行

```
sudo add-apt-repository --remove ppa:beineri/opt-qt57-xenial
sudo add-apt-repository --remove ppa:beineri/opt-qt571-xenial
```

安装完之后“文件”——“新建文件或项目”看看有没有ROS相关的选项吧

**2017-10-28更新：**
Ubuntu 16.04

```
sudo add-apt-repository ppa:levi-armstrong/qt-libraries-xenial 
sudo add-apt-repository ppa:levi-armstrong/ppa 
sudo apt update && sudo apt install qt57creator 
sudo apt install qt57creator-plugin-ros
```

### 2、头文件包含的问题

菜单栏“文件”——“Reload Project Build Info ···”可以更新文件状态，如果某些头文件添加之后IDE提示找不到，而你能确定头文件确实存在，那么你可以放心编译，之后IDE可以找到头文件并提供代码补全和提示功能。

用插件创建或导入ROS package之后还需要修改`.workspace`文件，如下图

![这里写图片描述](https://img-blog.csdn.net/20160819104609928)

在`<IncludePaths>`标签下加入下面这行
`<Directory>/opt/ros/indigo/include</Directory>`- 1

这样就可以使得Qt找到ros头文件，比如`<ros/ros.h>`
如下图，头文件包含语句不再提示找不到文件，输入ros相关函数可以自动补全

![这里写图片描述](https://img-blog.csdn.net/20160819104712163)

如果添加了上述路径仍提示找不到ros头文件，你可以保存之后重新打开该项目，博主某台电脑的Qt就是不能实时更新工作空间的文件变化真是忧伤，不过其他电脑都能实时更新文件状态，不用重新打开项目

### 3、模块找不到的问题

使用上面的命令行安装qt57及ROS插件之后发现Qt某些模块的名字变了，需要加qt57前缀，比如Qt的模块： 
按照官网[Qt Serial Port](http://doc.qt.io/qt-5/qtserialport-index.html)的说法只要在`.pro`文件加上
`QT += serialport`
然后头文件包含
`#include <QtSerialPort/QtSerialPort>`
就可以用串口模块了 
如果遇到如下报错
`Project ERROR: Unknown module(s) in QT: serialport`
需要安装`libqt5serialport5-dev`：  

```bash
sudo apt install libqt5serialport5-dev
```

如果上述`libqt5serialport5-dev`无法安装，提示依赖错误，我们可以安装`qt57serialport`

```bash
sudo apt install qt57serialport
```

之后再像之前那样调用`serialport`模块即可：  

```
QT += serialport

#include <QtSerialPort/QtSerialPort>
```

另外可以用`apt-cache search`命令查看软件源中可供安装的相关包的名字
`apt-cache search serialport`


