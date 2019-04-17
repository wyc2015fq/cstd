# Turtlebot2安装Google的cartographer做激光雷达SLAM - 站在巨人的肩膀上coding - CSDN博客





2018年04月20日 19:09:55[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：992








# 前言

        由于想在室外环境下做slam，开始选择ORBSLAM2，使用单目、双目、RGB-D在室外做了实验，发现效果非常差【单目、双目特征点很稀疏，难以用于导航；而kinect V1相机在室外有效距离下降到1m，且建图效果也很差】。现在是准备在turtlebot2上安装cartographer研究Google的激光SLAM，前期使用Kinect相机模拟激光数据，后期购买激光雷达。

# 一、环境准备

        turtlebot2小车底盘+NVIDIA TX2开发板


        Ubuntu 16.04+ROS kinetic版本


# 二、参考链接及说明

## 2.1参考链接

turtlebot2:

        https://www.ncnynl.com/archives/201702/1369.html


~~ https://www.ncnynl.com/archives/201702/1370.html（该安装方式已经被遗弃，亲测按照此方法会安装失败）~~

turtlebot3:

        https://www.ncnynl.com/archives/201801/2230.html


## 2.2安装前说明

      创客制造上有三种方式，turtlebot2的两种方式和turtlebot3的一种方式（参考上述链接），因为踩了turtlebot2安装方式里面的2个坑，用了turtlebot3安装cartographer的方式，这里主要注意一个坑就够了。这个坑就在于手动修改./rosinstall这个文件，贴上github上的ceres下载路径`https://github.com/ceres-solver/ceres-solver.git`的时候``，**该链接对应着最新版的ceres，虽然最后在安装这个package不会报错，但编译cartographer包的时候依赖ceres-solver编译编译出来的链接库等文件，很难让人发现是ceres-slover包的错误。**因此，我们要找到对应版本的ceres包的源码，手动下载到你创建的cartographer根目录。

# 三、安装

    依照这个连接https://www.ncnynl.com/archives/201801/2230.html安装。

在执行到这一步的时候：
`catkin_make_isolated --install --use-ninja -j1`会遇到这个错误：
![](https://img-blog.csdn.net/20180420184741348)


问题所在：直接把这个连接`https://github.com/ceres-solver/ceres-solver.git`贴到src/.rosinstall文件中

        会出现一个隐藏错误：自动从github想下载最新版本的ceres（目前为1.14）,我们需要的是1.12 0rc4版本。这个错误很难发现，因为ceres库本身会编译通过，而起来ceres的其他库会编译失败，而且一般也看不懂错误代码，难以下手解决。


解决方法：手动去github上找到对应ceres-slover版本。

          修改ceres的github地址的时候注意了，一定要找到对应版本，或者自己手动下载放到对应目录，修改文件名，去掉版本信息，改为`ceres-solver`。 查找对应版本（1.12  0rc4）的方法：

打开连接：`https://github.com/ceres-solver/ceres-solver.git`，点击branch:master，然后点击下方的tag，会出现一些版本标签，点击对应的标签就进入到对应版本的下载界面了。


![](https://img-blog.csdn.net/20180420185757857)


然后，重新执行：


`catkin_make_isolated --install --use-ninja -j1`过程比较慢，耐心等待。最后这个包安装成功：
![](https://img-blog.csdn.net/20180420185841497)


接下来就是按照创客制造上面的教程来安装了，没有问题，最后成功的截图：

![](https://img-blog.csdn.net/20180420185953251)


# 四、安装过程中的其他问题

        因为我只遇到了这种情况，其他问题可参考上面的连接内容，后面有对应的问题及解决方案。

#### 参考链接：

创客智造官网



        https://www.ncnynl.com/archives/201702/1369.html


        https://www.ncnynl.com/archives/201702/1370.html
        https://www.ncnynl.com/archives/201801/2230.html



