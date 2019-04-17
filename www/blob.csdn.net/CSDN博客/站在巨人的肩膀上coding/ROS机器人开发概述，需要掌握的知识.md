# ROS机器人开发概述，需要掌握的知识 - 站在巨人的肩膀上coding - CSDN博客





2018年04月17日 10:15:15[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：99








# 1.       ROS项目开发流程？

参照古月大神写的ROS探索总结系列：[http://blog.exbot.net/archives/619](http://blog.exbot.net/archives/619)

具体项目设计可看看《程序员》杂志的最新一篇文章《基于ROS的 系统》，是百度无人车的首席架构师等人写的无人车的架构的选型设计，值得参考借鉴，正在联系授权转载，无法提供链接，可自己找一下。



# 2.       ROS有哪些常用的、有用的工具？

常用工具有rqt rviz等，请经常关注官方WIKI或者论坛，一般有新的工具，在这些地方也都会有介绍。参考《ROS机器人编程》第三章讲了调试和可视化工具，此书第二版已由Exbot翻译完成，希望下半年早日出版，未来课程中可给大家发一些参考书。



# 3.       如何从零搭建机器人？

这个问题比较宽泛，建议先买个开发板一步步学，比如树莓派、odroid等，先学会一些基础的技术，明白一些基本原理了后确定你想要实现的功能，再在网上查找各种资料，去动手搭建机器人。亦可关注开源机器人项目HandsFree（wiki.exbot.net），涵盖了从嵌入式控制器的开发到SLAM ROS编程。

# 4.       硬件平台的购买或开源实现？

wiki.exbot.net有相关配件推荐的淘宝链接。

开源实现室内可参考HandsFree（wiki.exbot.net），室外可参考CarBot（wiki.exbot.net）。

也可以网上购买ROS官方的Turtlebot的原型机或者自己采购相关的配件来搭建（如底盘可以用IROBOT的扫地机，有开发的控制接口）。



硬件设计方面，首先要选用支持ubuntu系统的硬件：
- X86平台，建议使用，相对比较成熟。有问题网上也比较多解决方案。
- arm平台，可以运行功能比较强的方案有Inforce IFC6410 (Qualcomm® Snapdragon™ 600 Processor)和Nvidia Jetson TK1，简单功能的有Raspberry Pi 2，Odroid C1, X, U2, U3, XU3 and XU4等。
- 如果需要底盘运动的话，可以直接购买ROS官网的turtlebot硬件平台，不过比较贵。另外也可以自己搭建，可选用IROBO的ROOMBA系列的，如roomba 528，它有一个开发的控制接口协议，ROS上也有直接控制它的例子程序。方便开发者使用。
关于摄像头采集电路，可以采用双目摄像头，不过需要校准，运算量也大些。现在市场上有一些深度摄像头，信价比相对比较好的是kinect,华硕的Xtion PRO，intel也出了比较小型的F200和R200，这几个都有针对ROS系统的驱动和应用，可以在WIFI ROS上找到。另外也可以选也国产的，如奥比中光的，不过暂时没有直接针对ROS的应用，需要自己去完成。



# 5.       传感器的推荐型号？

kinect,华硕的Xtion PRO，intel也出了比较小型的F200和R200，这几个都有针对ROS系统的驱动和应用，可以在WIFI ROS上找到。另外也可以选也国产的，如奥比中光的，不过暂时没有直接针对ROS的应用，需要自己去完成。



# 6.       SLAM原理部分

原理可参考ros wiki的导航教程：

[http://wiki.ros.org/navigation](http://wiki.ros.org/navigation)

网上有不错的教程：

推荐清华大学高翔博士的SLAM教程，已经转过来了。

[http://blog.exbot.net/archives/1129](http://blog.exbot.net/archives/1129)

[http://blog.exbot.net/archives/2308](http://blog.exbot.net/archives/2308)



# 7.       SLAM技术的相关资料

建议把ROS wiki上的教程过一遍，ROS只是一个平台工具，学习了相关教程后，最后还是需要回归到算法的研究和研发。SLAM做为机器人中的重要技术，专业性强，一定要阅读相关的论文，可参考导航教程[1-3,11-12]; Gmapping的原理可参考资料[4-6]; ACML可参考资料[7-8]。Hector mapping的资料可参考[9-10]。

教材推荐《概率机器人学》，据悉中文译本国内也马上会出版。



[1]. [http://wiki.ros.org/navigation](http://wiki.ros.org/navigation).

[2]. [http://blog.exbot.net/archives/1129](http://blog.exbot.net/archives/1129)

[3]. [http://blog.exbot.net/archives/2308](http://blog.exbot.net/archives/2308)

[4]. [http://wiki.ros.org/gmapping](http://wiki.ros.org/gmapping)

[5]. [https://www.openslam.org/gmapping.html](https://www.openslam.org/gmapping.html)

[6]. [Improved techniques for grid mapping with rao-blackwellized particle filters](https://scholar.google.com/citations?view_op=view_citation&hl=en&user=11-D1A4AAAAJ&citation_for_view=11-D1A4AAAAJ:u5HHmVD_uO8C).G Grisetti, C Stachniss, W Burgard. Robotics, IEEE Transactions on 23 (1), 34-46

[7]. [https://www.openslam.org/gmapping.html](https://www.openslam.org/gmapping.html)

[8]. [http://wiki.ros.org/amcl](http://wiki.ros.org/amcl)

[9].[http://tedusar.eu/cms/sites/tedusar.eu.cms/files/Hector_SLAM_USAR_Kohlbrecher_RRSS_Graz_2012.pdf](http://tedusar.eu/cms/sites/tedusar.eu.cms/files/Hector_SLAM_USAR_Kohlbrecher_RRSS_Graz_2012.pdf)

[10]. [http://wiki.ros.org/hector_mapping](http://wiki.ros.org/hector_mapping)

[11]. [https://www.doc.ic.ac.uk/~ajd/Robotics/RoboticsResources/SLAMTutorial1.pdf](https://www.doc.ic.ac.uk/~ajd/Robotics/RoboticsResources/SLAMTutorial1.pdf)

[12]. [https://www.doc.ic.ac.uk/~ajd/Robotics/RoboticsResources/SLAMTutorial2.pdf](https://www.doc.ic.ac.uk/~ajd/Robotics/RoboticsResources/SLAMTutorial2.pdf)



其他问题：



# 8.       编程技术涵盖量较少，希望了解更多？

参照：[http://blog.exbot.net/archives/619](http://blog.exbot.net/archives/619)  可以有了基本了解。

# 9.       建图与路径规划内容较少，可以详解。

原理可参考ros wiki的导航教程：

[http://wiki.ros.org/navigation](http://wiki.ros.org/navigation)

网上有个不错的教程：

[http://blog.exbot.net/archives/1129](http://blog.exbot.net/archives/1129)

[http://blog.exbot.net/archives/2308](http://blog.exbot.net/archives/2308)

# 10.   基本偏向软件介绍，硬件平台涉及少，希望了解更多。

当前ROS的发行版是基于UBUNTU系统的，所以在开发使用ROS时建议也使用UBUNTU来开发，方便问题的查找得解决。

# 11.   RVIZ具体的用法
- RVIZ是ROS中一个最重要的可视化工具，功能非常强大，主要用来可视化传感器数据，机器人模型，导航等功能。
- RVIZ使用的教程：

http://wiki.ros.org/rviz/Tutorials
最好把教程的例子实践一遍

# 12.   Gazebo

参考：http://wiki.ros.org/gazebo

# 13.   urdf的使用

看看别人的例子程序。http://blog.csdn.net/sujun3304/article/details/18962719

# 14.   tf

参考：http://www.guyuehome.com/279

# 15.   在嵌入式产品中的应用

ROS基于UBUNTU操作系统,所以理论上只要能运行UBUNTU系统的嵌入式平台都可以安装ROS.但因为兼容性,依赖包缺失等问题,有些ROS功能包可能无法使用.所以在嵌入式平台的选型上尽量选择官方推荐的开发板.

# 16.   新设计一个机器人或者传感器,如何设计package

请先参考官方文档[http://wiki.ros.org/](http://wiki.ros.org/),熟悉package的设计,再有针对性地提问题

# 17.   通信框架代码讲解

简单来讲,ROS内部采用RPC来进行通信,MASTER提供各节点的注册并跟踪,实现节点间的通信.

dynamic-reconfigure

使用 parameter server

service

官方WIKI有service编写示例，请参考

http://wiki.ros.org/ROS/Tutorials/WritingServiceClient(c%2B%2B)

# 18.   AMCL不懂呀

AMCL（Adaptive Monte Carlo Localization），自适应蒙特卡罗定位。简单的说是知道了先验地图下的机器人定位。

AMCL的原理可参考《概率机器人》资料：

http://www.probabilistic-robotics.org/

如何在ros下使用AMCL，请参考ros wiki的使用教程：

http://wiki.ros.org/amcl

# 19.   RVIZ具体的用法

RVIZ是ROS中一个最重要的可视化工具，功能非常强大，主要用来可视化传感器数据，机器人模型，导航等功能。

RVIZ使用的教程：[http://wiki.ros.org/rviz/Tutorials](http://wiki.ros.org/rviz/Tutorials)

最好把教程的例子实践一遍

# 20.   对建模的思想要讲的清楚一些

建模的思想需要很多的知识，最好先学习相关的基础知识和论文。

参考资料：[http://www.probabilistic-robotics.org/](http://www.probabilistic-robotics.org/)



