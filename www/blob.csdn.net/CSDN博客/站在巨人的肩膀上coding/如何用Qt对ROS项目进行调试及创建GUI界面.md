# 如何用Qt对ROS项目进行调试及创建GUI界面 - 站在巨人的肩膀上coding - CSDN博客





2018年04月17日 10:09:48[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：618








## 一、前言

本文详细介绍了利用CMakeLists.txt文件把ROS项目导入到QtCreator进行代码编写和调试的过程，文末还介绍了ROS中使用Qt界面的方法

这种导入ROS项目到Qt的方法也适用于其他CMake创建的项目，这种方案缺点是导入项目后无法直接在Qt下面新建文件和package到项目，只能用命令行在Qt外面创建之后重新导入到Qt，最近得益于Levi-Armstrong等人的工作，他们开发了一个名为`ros_qtc_plugin` 的Qt插件可以方便的导入、创建、编译、调试ROS项目，博主认为这是一个目前为止最完美的解决方案，详细请看博主的另一篇文章介绍：[如何使用Qt插件在Qt中进行ROS开发](http://blog.csdn.net/u013453604/article/details/52186375)。

本文所用操作系统为Ubuntu 14.04 64位，Qt版本为Qt5.5.1(Qt Creator3.5.1) 
本文地址：[http://blog.csdn.net/u013453604/article/details/52167213](http://blog.csdn.net/u013453604/article/details/52167213)
参考: 
[ROS wiki IDEs](http://wiki.ros.org/IDEs#QtCreator)
[Qt App Templates](http://wiki.ros.org/qt_create/Tutorials/Qt%20App%20Templates)
[Write and Debug code for ROS with QtCreator](http://myzharbot.robot-home.it/blog/software/myzharbot-ros/qtcreator-ros/)
[ROS: How to develop catkin packages from an IDE](http://www.ciencia-explicada.com/2014/12/ros-how-to-develop-catkin-packages-from-an-ide.html)

## 二、创建Qt-ROS快捷方式

因为Qt本身支持CMake项目，所以从终端启动QtCreator是可以直接加载运行ROS相关项目的，如果你不想从终端启动，那么需要做一些设置来确保启动QtCreator的同时加载ROS环境变量

这里我们来创建一个启动器（即桌面快捷方式）： 
点击左上角Ubuntu标志（或者按下Win键）搜索一个叫“启动应用程序”的软件，然后选择“添加”创建一个启动程序，如下图

![这里写图片描述](https://img-blog.csdn.net/20160809234208510)

(1) 我将这个启动器命名为“Qt-ROS”，然后点击“浏览”，选中`Qt5.5.1/Tools/QtCreator/bin/`下的qtcreator可执行文件，然后“命令”栏会出现qtcreator可执行文件的绝对路径 
(2) 我们还需要在这行绝对路径前面加一句`bash -i -c`，用空格和后面的路径隔开 
(3) 然后点击“添加”，这样“额外的启动程序”栏下就出现了名为“Qt-ROS”的启动器，用鼠标把它拖到桌面变成了一个桌面快捷方式，把它重命名为“Qt-ROS” 
(4) 右键选中它，“属性”，在“基本”页点击名称左边的图标可以设置自定义图标，我将其设为原来qtcreator的图标，图标地址在 
`/Qt5.5.1/Tools/QtCreator/share/qtcreator/welcomescreen/widgets/images/qtcreator.png`
(5) 再到“权限”页选中“允许作为程序执行文件” 
这样qtcreator快捷方式方式就建好了，如下图

![这里写图片描述](https://img-blog.csdn.net/20160809234316635)

用这个快捷方式打开ROS项目之后只需要在终端运行一个`roscore`，就能开始运行或者调试代码，并且在Debug模式下可以看到程序内部的许多变量，ROS调试再也不是问题啦

如果你不想那么麻烦可以直接在桌面新建一个`Qt-ROS.desktop`文件，然后把下面代码复制进去

```
[Desktop Entry]
Exec=bash -i -c qtcreator %F
Icon=QtProject-qtcreator
Type=Application
Terminal=false
Name=Qt-ROS
GenericName=Integrated Development Environment
MimeType=text/x-c++src;text/x-c++hdr;text/x-xsrc;application/x-designer;application/vnd.nokia.qt.qmakeprofile;application/vnd.nokia.xml.qt.resource;
Categories=Qt;Development;IDE;
InitialPreference=9
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10

其中的`Exec Icon Name`不需要再修改，这个配置可以直接用

## 三、导入ROS项目到Qt

现在我们用前面创建的`Qt-ROS.desktop`快捷方式打开Qt，开始导入ROS项目

### 1、导入单个package

如果你只想导入单个package进行调试请看这里： 
(1) 双击前面创建的`Qt-ROS`快捷方式打开Qt 
(2) “open project”或者“文件”——“打开文件或项目”，选择catkin工作空间里package包的CMakeLists.txt，如下图

![这里写图片描述](https://img-blog.csdn.net/20160810121215340)

我的catkin工作空间是catkin_qtwstest，里面有一个package叫test，test的CMakeLists.txt在`catkin_qtwstest\src\test`下面 
(3) 构建路径中的“构建目录”选择`catkin_qtwstest/build`，进入“下一步”

![这里写图片描述](https://img-blog.csdn.net/20160810161348478)

(4) “执行CMake”的步骤如下图 
如果需要Debug，填入参数`-DCMAKE_BUILD_TYPE=Debug`； 
如果不需要Debug，你想直接运行，填入参数`-DCMAKE_BUILD_TYPE=Release`

![这里写图片描述](https://img-blog.csdn.net/20160810131409366)

然后点击“执行CMake”——“完成”,package成功导入到Qt 
(5) 导入之后如下图，可以看到`src`下面有三个`.cpp`文件，这是我写的三个节点，在CMakeLists.txt里我把它们分别编译成三个可执行文件，名字不变

![这里写图片描述](https://img-blog.csdn.net/20160810161430447)

如果想运行这些节点需要在终端运行一个master，节点才能正常运行，执行命令：
`$ roscore`- 1

Qt界面左下角有个小的电脑图标“project all”，点击它可以选择运行的节点，如图我选择名为“talker”的节点 
点击左下角绿色三角形可以运行，点击带爬虫的绿色三角形可以进行调试 
(6) 如下图是运行时的情况

![这里写图片描述](https://img-blog.csdn.net/20160810161740929)

(7) 如果需要更改项目配置可以点击Qt界面左侧的“项目”，如下图 
在“构建”页的“重新装配项目”后面有一个“Run CMake…”点击它可以如(4)更改编译参数

![这里写图片描述](https://img-blog.csdn.net/20160810162002308)

(8) 在“项目”的“运行”页可以修改一些环境变量，如下图

![这里写图片描述](https://img-blog.csdn.net/20160810162145075)

(9) 如果想重新导入这个项目需要删除`catkin_qtwstest\src\test`下的CMakeLists.txt.user文件，然后按照上述操作重新导入

### 2、导入工作空间所有package

如果你想把一个catkin工作空间里的所有package导入到Qt请看这里： 
(1) 我们要导入的工作空间名字叫catkin_qtws，它里面有许多其他的package，进入工作空间根目录`catkin_qtws\src`我们可以看到工作空间的CMakeLists.txt是一个符号链接，如下图

![这里写图片描述](https://img-blog.csdn.net/20160810162722458)

我们需要把它变成一个真正的CMakeLists.txt，执行如下命令

```bash
cd ~/catkin_qtws/src
sed -i '' CMakeLists.txt
```
- 1
- 2

然后你会发现`catkin_qtws\src`下的CMakeLists.txt图标变得和package里面的一样了 
(2) 双击前面创建的`Qt-ROS`快捷方式打开Qt 
(3) “open project”或者“文件”——“打开文件或项目”，选择catkin工作空间里的CMakeLists.txt，如下图

![这里写图片描述](https://img-blog.csdn.net/20160810163131298)

(4) 构建路径中的“构建目录”选择`catkin_qtws/build`，进入“下一步”

![这里写图片描述](https://img-blog.csdn.net/20160810163230450)

(5) “执行CMake”步骤如下图 
如果需要Debug，填入参数`-DCMAKE_BUILD_TYPE=Debug`； 
如果不需要Debug，你想直接运行，填入参数`-DCMAKE_BUILD_TYPE=Release`

![这里写图片描述](https://img-blog.csdn.net/20160810163801349)

然后点击“执行CMake”——“完成”,package成功导入到Qt 
(6) 导入之后如下图，这里面有很多我写的package，这里有一个叫`odomframe_test.cpp`的文件是ROS wiki上的一个示例代码，我们以这个odomframe_test节点为例

![这里写图片描述](https://img-blog.csdn.net/20160810164336246)

如果想运行odomframe_test节点需要先在终端运行一个master，节点才能正常运行，执行命令：
`$ roscore`- 1

Qt界面左下角有个小的电脑图标“project all”，点击它可以选择运行的节点，如图我选择名为“odomframe_test”的节点 
点击左下角绿色三角形可以运行，点击带爬虫的绿色三角形可以进行调试

(7) 如下图是调试odomframe_test节点的情况

![这里写图片描述](https://img-blog.csdn.net/20160810164632852)

(8) 如果想重新导入这个项目需要删除`catkin_qtws\src`下的CMakeLists.txt.user文件，然后按照上述操作重新导入

### 3、在ROS中使用QT界面

在终端可以直接用`catkin_create_qt_pkg`命令创建带Qt界面的ROS package，再按照前面说的方法导入到Qt即可 
这里参考的是[qt_createTutorialsQt App Templates](http://wiki.ros.org/qt_create/Tutorials/Qt%20App%20Templates)

(1) 要使用`catkin_create_qt_pkg`命令需要安装一个包，执行如下命令：
`$ sudo apt-get install ros-indigo-qt-ros`- 1

这样我就可以使用`catkin_create_qt_pkg`命令创建一个名为testgui的package

```
$ cd catkin_qtwstest/src/
$ catkin_create_qt_pkg testgui
```
- 1
- 2

如下图

![这里写图片描述](https://img-blog.csdn.net/20160810164927472)

(2) 按照上述方式重新把`catkin_qtwstest`工作空间导入到Qt中，如下图

![这里写图片描述](https://img-blog.csdn.net/20160810165012099)

在左侧的树状图可以看到testgui下的目录结构，自动生成的`main.cpp`、`main_window.cpp`、`qnode.cpp`和`main_window.ui`包含Qt界面的基本元素 
(3) 要运行testgui节点先启动一个roscore,如图
`$ roscore`- 1

![这里写图片描述](https://img-blog.csdn.net/20160810165140745)

然后点击Qt左下角的绿色三角形运行testgui节点，如下图

![这里写图片描述](https://img-blog.csdn.net/20160810165412105)

根据roscore的`ROS_MASTER_URI`填写UI界面上的`ROS_MASTER_URI`，然后点击“connect”，testgui节点开始在名为`/chatter`的topic上发送字符串如图



