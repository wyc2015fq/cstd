# 基于ROS的QT界面开发史上最强教程 - 站在巨人的肩膀上coding - CSDN博客





2018年06月25日 19:23:28[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：4487








# 使用ROS下的QT插件开发基于ROS界面的程序

## 1、安装ROS下的ros_qtc_plugin插件及git、gitk工具

### 1.1.官方指导教程

[https://ros-industrial.github.io/ros_qtc_plugin/_source/How-to-Install-Users.html](https://ros-industrial.github.io/ros_qtc_plugin/_source/How-to-Install-Users.html)

### 1.2.坊间流传教程【参考我之前的博文】

    ROS下QT环境搭建: [https://blog.csdn.net/love1055259415/article/details/80219749](https://blog.csdn.net/love1055259415/article/details/80219749)


    QT导入ROS工程：[https://blog.csdn.net/LOVE1055259415/article/details/80066519](https://blog.csdn.net/LOVE1055259415/article/details/80066519)[](https://mp.csdn.net/postedit/80066519)


### 1.3.git、gitk工具安装
[https://mp.csdn.net/postedit/8029079](https://mp.csdn.net/postedit/80290793)
   注意，此方式是在本地仓库上创建代码版本库进行管理，未上传到自己的github。安装git:sudo apt-get install git，接着配置请戳[http://www.codexiu.cn/GIT/blog/39420/](http://www.codexiu.cn/GIT/blog/39420/)

## 2、开发环境配置与开发实例说明【必备参考资料】

[https://blog.csdn.net/lightrain0/article/details/71642103](https://blog.csdn.net/lightrain0/article/details/71642103)

[http://www.cnblogs.com/casperwin/p/6206193.html](http://www.cnblogs.com/casperwin/p/6206193.html)


### 2.1.最重要的是下面两个pkg的安装配置，基于系统版本,indigo或者是kinect版本
[http://www.cnblogs.com/casperwin/p/6206193.html  中第四条](http://www.cnblogs.com/casperwin/p/6206193.html)4) Qt Dependances needed in ROS
sudo apt-get install ros-indigo-qt-create   [sudo apt-get install ros-kinetic-qt-create ] 

sudo qpt-get install ros-indigo-qt-build    [sudo apt-get install ros-kinetic-qt-build ]

![](https://img-blog.csdn.net/20180605003554680)

注意：记住该安装方法，在以后的开发过程中，如果有报错的，需要安装相应的包来解决。


### 2.2. 创建QTROS代码模板

Createa QtROS GUI template

$cd src

$catkin_create_qt_create qtros

$ source devel/setup.bash


where“qtros” is the package name you created.

Normally,it will show you as

![](https://img-blog.csdn.net/2018060500353776)


### 2.3 运行调试

    添加ROS Attach和ROS run

![](https://img-blog.csdn.net/20180605225737640)


### 2.4. “官方”Tutorial及其他参考教程

【重要】“官方”教程


[http://www.cnblogs.com/casperwin/p/6206193.htm](http://www.cnblogs.com/casperwin/p/6206193.htm)

  github中其他参考模板


[https://github.com/stonier/qt_ros](https://github.com/stonier/qt_ros)


### 2.5.工程写法与注意事项

           仿照qnode.hpp、qnode.cpp分别实现各个节点的功能，最终生成一个启动文件，在devel/lib/${project}/目录下

可能出现的问题：

（1）可能出现找不到opencv相关的函数、找不到opencvCmake.txt文件等错误(即使你已经定义好、装好opencv、也找的到相关cmake文件)


![](https://img-blog.csdn.net/20180612225152879)


此时，修改节点配置文件CMakeList.txt，添加OpenCV，同时加上：

find_package( OpenCV  REQUIRED)  【其他地方的这种问题也可以这样解决】

find_package(catkin REQUIRED COMPONENTS qt_build roscpp std_msgs cv_bridge image_transport OpenCV)

（2）BOOST库的问题

![](https://img-blog.csdn.net/20180612225342695)


        解决办法：[https://blog.csdn.net/h321654/article/details/54582341](https://blog.csdn.net/h321654/article/details/54582341)

![](https://img-blog.csdn.net/20180612225754749)


参考工程demo:[https://github.com/xpharry/ROSCppGUI/tree/master/QtROS_GUI](https://github.com/xpharry/ROSCppGUI/tree/master/QtROS_GUI)。尤其注意回调函数的写法：

声明：

![](https://img-blog.csdn.net/2018060522332820)


定义：


![](https://img-blog.csdn.net/20180605223228404)




### 2.6.节点间编译实现方式、顺序
2.6.1 star函数与ros::star()函数


2.6.2 添加好自己的节点之后，编译无错误，但是如何让自己的节点运行呢？




2.6.3 节点类是在哪里实例化？




2.6.4 mainwindow中的信号槽？窗口移动与固定大小？

2.6.5 mainwindow中实例化节点时候，报错，节点初始化函数找不到解决办法：

回到终端，找到项目工程路径，然后手动catkin_make，编译通过

2.6.6 ros中定时器回调函数的触发方式


    ros中设置定时器后，依靠ros::spin()方法和ros::spin()方法来出发定时器事件！只有在执行到这两句话之后才会触发回调函数[不仅仅是定时器回调，还包括其他回调]。spin会在回调函数队列中死循环，不执行下一句话，spinOnce会执行一次回调函数，然后接着执下面的代码。关于二者详细的比较可以参照这篇博客[https://blog.csdn.net/LOVE1055259415/article/details/79553663](https://blog.csdn.net/LOVE1055259415/article/details/79553663) 。


### 2.7 终端调用

2.7.1 终端调用的几种形式


(1)   &表示执行此终端调用后可以执行其他终端调用或者响应，这里不加&的话，执行roscore会闪退。

`  system("gnome-terminal  -x bash -c ' roscore '&");`

(2)  limited:=true'&表示的含义是？？

`  system("gnome-terminal  -x bash -c ' roslaunch usb_cam usb_cam-test.launch limited:=true'&");`

(3) sim:= true' &表示的含义是？？

`  system("gnome-terminal -x bash -c 'source /home/ubuntu/turtlebot/devel/setup.bash;roslaunch turtlebot_rviz_launchers view_navigation.launch sim:=true'&");`

    2.7.2 



## 3、Linux下串口设备别名及赋予读写权限

在机器人开发过程中，发现串口号经常变，每次插入不同的位置，串口号都可能会变化，如果每次都在程序中修改串口号就太麻烦 了， 因此重新给串口设置别名，利用第二个名字来在程序中读取，就可以避免这种问题了


[https://blog.csdn.net/zong596568821xp/article/details/78579734](https://blog.csdn.net/zong596568821xp/article/details/78579734)

sudo gedit /etc/udev/rules.d/rplidar.rules

添加以下内容：


KERNEL=="ttyUSB*", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", MODE:="0777", SYMLINK+="rplidar"



![](https://img-blog.csdn.net/20180605004443998)

## 4、QT下使用C++和Python混合编程
https://www.cnblogs.com/xia-weiwen/p/7932379.html



## 5、socket图传，C++与Python混合编程




## 6、qt编程学习

http://www.icoolxue.com/album/show/282

### 6.1 qt使用web空间显示网页

注意，ROS下qt工程不同于普通ROS工程【项目配置文件是.pro文件】，是CMake工程【项目配置文件是CMakeList.txt文件】。.pto中添加的应该是QT += webkit；此时在CMakeList中添加的应该是  


```
rosbuild_prepare_qt4(QtCore QtGui QtWebkit) # Add the appropriate components to the component list here
```

![](https://img-blog.csdn.net/20180607164228633)

如果不添加，则会报错：QWebView：：QWebView（）未定义等。


附：普通qt的web工程应该是：

![](https://img-blog.csdn.net/20180607164255977)

6.2







