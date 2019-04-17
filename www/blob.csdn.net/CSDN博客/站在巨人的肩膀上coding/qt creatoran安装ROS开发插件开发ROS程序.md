# qt creatoran安装ROS开发插件开发ROS程序 - 站在巨人的肩膀上coding - CSDN博客





2018年04月16日 21:02:25[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：420








# **配置使用qt creator开发ROS程序**

1、安装qtcreator

Ubuntu 14.04，直接在应用中心搜索qtcreator，直接install即可。安装完了查看菜单栏tool/option选项，查看build&run选项，看qmake gcc编译器设置是否正确。

2、Qtcreator配置ROS开发环境

2.1参考博客

[https://blog.csdn.net/linuxarmsummary/article/details/48476171](https://blog.csdn.net/linuxarmsummary/article/details/48476171)

[https://www.cnblogs.com/qixianyu/p/6592922.html](https://www.cnblogs.com/qixianyu/p/6592922.html)

创客智造

[https://www.ncnynl.com/archives/201701/1277.html](https://www.ncnynl.com/archives/201701/1277.html)

l 里面安装qtcreator 添加ppa源会出现

Cannot add PPA: 'ppa:levi-armstrong/qt-libraries-trusty'.

用这个命令解决：sudo add-apt-repository ppa:levi-armstrong/qt-libraries-trusty

l 问题2 qt57creator-plugin-ros : Depends: qt59creator (>= 4.4.1) but it is not going to be installed

解决方式：[https://blog.csdn.net/lingchen2348/article/details/78991468](https://blog.csdn.net/lingchen2348/article/details/78991468)

qt59creator : Depends: libstdc++6 (>= 4.9) but 4.8.4-2ubuntu1~14.04.3 is installed and it is kept back.

  升级gcc版本，安装gcc g++4.9，然后手动修改gcc版本，caffe时候修改为4.8

参考链接：[https://blog.csdn.net/u014696921/article/details/56677930](https://blog.csdn.net/u014696921/article/details/56677930)

sudo apt-get install build-essential

sudo add-apt-repository ppa:ubuntu-toolchain-r/test

sudo apt-get update && sudo apt-get install gcc-4.9 g++-4.9

但是这时候gcc版本还是默认设置4.8，需要手动修改版本，将其设置为默认

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.9

或者选择gcc版本，手动选择默认版本

sudo update-alternatives --config gcc/g++

使用的编译器版本查看

sudo update-alternatives  --query gcc

2.2结局问题，安装sudo apt-get install qt57creator-plugin-ros

网速较慢

2.3实际使用

如果运行home下的Qt creator，那么ROS插件是不会一同运行的，而这个版本的Qt creator是我最终想使用的版本。如果运行/opt/Qt那个系统目录下的捆绑自带的Qt creator，ROS插件是可以使用的，原因很显然，插件的相关文件都安装在了这个版本的Qt目录下，而且依赖关系也都由这个版本的Qt来满足。

说明介绍：[https://blog.csdn.net/lingchen2348/article/details/78991468](https://blog.csdn.net/lingchen2348/article/details/78991468)

2.4创建快捷方式方便使用

	在/usr/share/applacations/文件夹下新建快捷方式配置文件，这里给出一个eclipse的例子看看：

[Desktop Entry]

Encoding=UTF-8

Name=eclipse

Comment=EclipseIDE

Exec=/home/wang/adt-bundle-linux-x86_64-20140321/eclipse/eclipse

Icon=/home/wang/adt-bundle-linux-x86_64-20140321/eclipse/icon.xpm

Terminal=falsestarttupNotify=true

Type=Application

Categories=Application;Development;

》》》》》》》》》》》》》》》》》》》》》》》》》》

参数说明：encoding  ----制定编码方式，一般是UTF-8

name --------快捷方式的名字

exec  ---------启动文件的路径（重要）

icon ----------指定图标路径

terminal----------是否启动终端（黑框框）

其他的参数和我一样就行了

编辑完成后，保存退出。

[Desktop Entry]

Type=Application

Exec=/home/ubu/Qt5.8.0/Tools/QtCreator/bin/qtcreator

Name=Qt Creator (Community)

GenericName=The IDE of choice for Qt development.

Icon=QtProject-qtcreator

Terminal=false

Categories=Development;IDE;Qt;

MimeType=text/x-c++src;text/x-c++hdr;text/x-xsrc;application/x-designer;application/vnd.qt.qmakeprofile;application/vnd.qt.xml.resource;text/x-qml;text/x-qt.qml;text/x-qt.qbs;



然后将qtcreator59的快捷方式文件直接拖动到任务栏即可！！

********************至此，就解决了利用QT编写ROS软件包的问题*******************

********************2018.3.26.00:52**********************************************

1、运行步骤

创建工作空间、功能包、节点及节点实现代码

修改包内cmakeLists.txt文件，编译节点，连接库

执行，添run step



一定要记得，在CMakeLists.txt文件中添加生成执行文件的代码



2、是否可以发布一个可执行程序？

答案是不可以的，它只会把节点编译成可执行的文件，在rosrun的时候去调用它。







