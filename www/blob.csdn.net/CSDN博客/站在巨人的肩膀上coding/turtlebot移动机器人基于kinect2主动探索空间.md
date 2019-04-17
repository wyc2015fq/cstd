# turtlebot移动机器人基于kinect2主动探索空间 - 站在巨人的肩膀上coding - CSDN博客





2018年05月16日 23:42:52[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：451








转载于：[https://blog.csdn.net/xiaocainiaodeboke/article/details/53514625](https://blog.csdn.net/xiaocainiaodeboke/article/details/53514625)


作者:  [A_cainiao_A](https://blog.csdn.net/xiaocainiaodeboke/article/details/53514625)

# 写在前面的话

移动机器人是指那些通过传感器获取数据，本地处理数据或者云端处理数据，进而控制其在地面上移动的设备（狭义情况下）。在这里我们使用的移动机器人是turtlebot关于他的介绍请看[这篇博客:turtlebot机器人入门篇](http://blog.csdn.net/xiaocainiaodeboke/article/details/51476557)。使用的传感器是kinect2，关于他的介绍大家可以去网上搜搜，很多的。有了传感器和机器人后，我们就得说说它使用的软件平台，turtlebot很好支持机器人操作系统（Robot Operating System 简称ROS）。关于ROS的介绍参看他的[wiki](http://wiki.ros.org/)。我们使用ROS就可以比较方便的从kinect2中获取数据，向turtlebot的移动底座发送移动控制命令了。 
好了，万事俱备只欠东风。这个东风是指我们要采取什么样的方法来处理数据，使得机器人将要移动的下一个位置是最有效。这个最有效是指什么意思呢？在机器人主动探索领域里，空间的覆盖率是一个指标，这个指标意思是机器人要尽可能快速而有效的探索整个环境。所以根据数据预测的下一个机器人的位置，就要有效的诱导机器人探索整个空间。如下图所示的二维栅格地图，蓝色的表示障碍物，红色的表示机器人移动的历史轨迹。当然这只是我们理想的机器人运动轨迹，实际当中往往不是这样的，当然研究的目的就是希望机器人的实际运动轨迹无限的逼近这个理想轨迹。这也正是我们研究的意思和目的。 
![这里写图片描述](https://img-blog.csdn.net/20161208143513842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb2NhaW5pYW9kZWJva2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面我主要介绍其中的一种理论：基于信息论的主动探索。最后再简单介绍一下代码结构和核心代码。

# 基于信息论的主动探索

下面介绍的主要思想参照的是文献[1],主动探索理论研究的问题是：假设一个移动机器人没有环境的任何先验知识，但是其可以通过传感器收集环境数据，主动探索算法就要根据这些数据决定机器人向哪个方向移动和移动多少距离。如下图所示，蓝色的依然表示障碍物，紫红色表示机器人当前的位置，红色小圈表示传感器收集到的数据。下一步就是要决定机器人应该向那个方向移动，移动多少？ 
![图1](https://img-blog.csdn.net/20161208143311927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb2NhaW5pYW9kZWJva2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在开始介绍之前我们先了解两个概念：

> 
1、信息熵：是平均而言一件事情发生时我们得到的信息量的大小，小概率事件的信息量大。H=−∑p(x)logp(x), H：信息熵，p(x)：某个事件发生的概率。 
2、互信息是一种信息的度量，他可以看成一个随机变量中包含的关于另一个随机变量的信息量，或者说是一个随机变量由于已知另一个随机变量而减少的不确定性。上图中的绿色的星就表示互信息最大的数据。


基于信息论的主动探索理论的核心想法有二，其一是：长远来看我们探索周围环境是在降低地图的信息熵，即就是重构的环境模型更加确定。其二是：就目前来看，机器人要去的下一个位置（包括位置和姿态，位置：机器人在地图中的x，y坐标，和机器人的朝向，用一个和x坐标轴的夹角表示）的互信息要最大，也就是信息增益最大。 
首先定义二维栅格地图 m 的信息熵： 




H(m)=−∑i∑jp(mi,j)log p(mi,j)

其中mi,j表示栅格地图m中的一个栅格，如果说这个栅格没有被观测到或者说是未知的，则：0.5p(mi,j)=0.5 。如果说这个栅格已经被观测到了，不管是障碍物还是空地，都算是已知的区域，那么肯定对地图的信息熵是没有贡献的，则：1p(mi,j)=1
有了地图的信息熵，我们就可以定义互信息或者说信息增益： 



I(m,xi)=H(m)−H(m|xi)

其中H(m|xi)表示：如果机器人运动到xi位置地图的信息熵。 
那么求下一个最有效的或者最能带来信息增益的位置就很好理解了。 



x∗=argmaxxi∈Laction I(m,xi)

其中Laction表示机器人位置可行域的集合，这个式子表示的意思就是：机器人下一个最有效的位置是信息增益最大的位置。


# 代码结构和和核心代码介绍

代码在github上，[请点击这里下载](https://github.com/QinZiwen/turtlebot_k2_exploration_3d)，下图是代码的文件结构：

![这里写图片描述](https://img-blog.csdn.net/20161208154404702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb2NhaW5pYW9kZWJva2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

include：包含的头文件 
launch：ROS的launch文件（如果不知道这是什么，请先看看ROS的[wiki](http://wiki.ros.org/)） 
src：源代码 
CMakeLists.txt：是cmake的配置文件 
package.xml：ROS工程的配置文件

首先来看launch/turtlebot_gmapping.launch

```xml
<launch>  
  <arg name="scan_topic"  default="scan_kinect" />
  <arg name="base_frame"  default="base_footprint"/>
  <arg name="odom_frame"  default="odom"/>

  <include file="$(find turtlebot_bringup)/launch/minimal.launch"/>

  <include file="$(find turtlebot_exploration_3d)/launch/3dsensor.launch">
    <arg name="scan_topic" value="$(arg scan_topic)"/>
  </include>

  <!-- static_transform_publisher x y z yaw pitch roll frame_id child_frame_id period_in_ms -->  
  <!-- node pkg="tf" type="static_transform_publisher" name="odom_to_base_footprint" args="0 0 0 0 0 0 /odom /base_footprint 50" / -->
  <node pkg="tf" type="static_transform_publisher" name="base_footprint_to_laser" args="0 0 0.5 0 0 0 /base_footprint /laser 50" />
  <node pkg="tf" type="static_transform_publisher" name="base_footprint_to_kinect2laser" args="0 0 0.5 0 0 0 /base_footprint /kinect2_depth_frame 50" />
  <node pkg="tf" type="static_transform_publisher" name="base_footprint_to_kinect2_link" args="0 0 0.5 -1.57 0 -1.57 /base_footprint /kinect2_link 50" />


  <node pkg="gmapping" type="slam_gmapping" name="slam_gmapping" output="screen">
    <param name="scan_topic" value="$(arg scan_topic)"/>
    <param name="base_frame" value="$(arg base_frame)"/>
    <param name="odom_frame" value="$(arg odom_frame)"/>
    <param name="map_update_interval" value="5.0"/>
    <param name="maxUrange" value="7.9"/>
    <param name="maxRange" value="8.0"/>
    <param name="sigma" value="0.05"/>
    <param name="kernelSize" value="1"/>
    <param name="lstep" value="0.05"/>
    <param name="astep" value="0.05"/>
    <param name="iterations" value="5"/>
    <param name="lsigma" value="0.075"/>
    <param name="ogain" value="3.0"/>
    <param name="lskip" value="0"/>
    <param name="minimumScore" value="200"/>
    <param name="srr" value="0.01"/>
    <param name="srt" value="0.02"/>
    <param name="str" value="0.01"/>
    <param name="stt" value="0.02"/>
    <param name="linearUpdate" value="0.5"/>
    <param name="angularUpdate" value="0.436"/>
    <param name="temporalUpdate" value="-1.0"/>
    <param name="resampleThreshold" value="0.5"/>
    <param name="particles" value="80"/>
  <!--
    <param name="xmin" value="-50.0"/>
    <param name="ymin" value="-50.0"/>
    <param name="xmax" value="50.0"/>
    <param name="ymax" value="50.0"/>
  make the starting size small for the benefit of the Android client's memory...
  -->
    <param name="xmin" value="-1.0"/>
    <param name="ymin" value="-1.0"/>
    <param name="xmax" value="1.0"/>
    <param name="ymax" value="1.0"/>

    <param name="delta" value="0.01"/>
    <param name="llsamplerange" value="0.01"/>
    <param name="llsamplestep" value="0.01"/>
    <param name="lasamplerange" value="0.005"/>
    <param name="lasamplestep" value="0.005"/>
    <remap from="scan" to="$(arg scan_topic)"/>
  </node>

  <include file="$(find turtlebot_exploration_3d)/launch/move/move_base.launch.xml">
    <arg name="laser_topic" value="$(arg scan_topic)"/>
  </include>

  <node pkg="turtlebot_exploration_3d" type="scan_to_pcl" name="scan_to_pcl" />
</launch>
```

关于launch文件写法的详细介绍[参照这里](http://wiki.ros.org/roslaunch)，下面分别介绍launch/turtlebot_gmapping.launch中各个部分代码的含义：

```
<arg name="scan_topic"  default="scan_kinect" />
<arg name="base_frame"  default="base_footprint"/>
<arg name="odom_frame"  default="odom"/>
```

定义三个局部变量，这三个量有默认值也可以被改变，如下将“scan_topic”的值改为“scan”：
`$ roslaunch turtlebot_k2_exploration_3d turtlebot_gmapping.launch scan_topic:=/scan``<include file="$(find turtlebot_bringup)/launch/minimal.launch"/>`
启动turtlebot底盘的驱动，他订阅了发布geometry_msgs/Twist类型消息的topic，这个消息中包含了机器人运动的指令。

```
<include file="$(find turtlebot_exploration_3d)/launch/3dsensor.launch">
    <arg name="scan_topic" value="$(arg scan_topic)"/>
</include>
```

启动kinect2的驱动，关于如何安装kinect2的驱动请[参照这篇博客](http://blog.csdn.net/xiaocainiaodeboke/article/details/51476557)，另外一片博客中介绍了如何使用kinect2驱动获取数据[请点击这里查看](http://blog.csdn.net/xiaocainiaodeboke/article/details/52353191)

```
<node pkg="tf" type="static_transform_publisher" name="base_footprint_to_laser" args="0 0 0.5 0 0 0 /base_footprint /laser 50" />
<node pkg="tf" type="static_transform_publisher" name="base_footprint_to_kinect2laser" args="0 0 0.5 0 0 0 /base_footprint /kinect2_depth_frame 50" />
<node pkg="tf" type="static_transform_publisher" name="base_footprint_to_kinect2_link" args="0 0 0.5 -1.57 0 -1.57 /base_footprint /kinect2_link 50" />
```

相关坐标的转换，详细资料请参考[这里](http://wiki.ros.org/tf/Tutorials), 坐标转换的意义就是可以将所有的位置转换到同一坐标参考系，而且不同坐标系之间可以自由的转换。tf包就是帮助我们做这个事情的。

```
<node pkg="gmapping" type="slam_gmapping" name="slam_gmapping" output="screen">
    <param name="scan_topic" value="$(arg scan_topic)"/>
    <param name="base_frame" value="$(arg base_frame)"/>
    <param name="odom_frame" value="$(arg odom_frame)"/>
    <param name="map_update_interval" value="5.0"/>
    <param name="maxUrange" value="7.9"/>
    <param name="maxRange" value="8.0"/>
    <param name="sigma" value="0.05"/>
    <param name="kernelSize" value="1"/>
    <param name="lstep" value="0.05"/>
    <param name="astep" value="0.05"/>
    <param name="iterations" value="5"/>
    <param name="lsigma" value="0.075"/>
    <param name="ogain" value="3.0"/>
    <param name="lskip" value="0"/>
    <param name="minimumScore" value="200"/>
    <param name="srr" value="0.01"/>
    <param name="srt" value="0.02"/>
    <param name="str" value="0.01"/>
    <param name="stt" value="0.02"/>
    <param name="linearUpdate" value="0.5"/>
    <param name="angularUpdate" value="0.436"/>
    <param name="temporalUpdate" value="-1.0"/>
    <param name="resampleThreshold" value="0.5"/>
    <param name="particles" value="80"/>
  <!--
    <param name="xmin" value="-50.0"/>
    <param name="ymin" value="-50.0"/>
    <param name="xmax" value="50.0"/>
    <param name="ymax" value="50.0"/>
  make the starting size small for the benefit of the Android client's memory...
  -->
    <param name="xmin" value="-1.0"/>
    <param name="ymin" value="-1.0"/>
    <param name="xmax" value="1.0"/>
    <param name="ymax" value="1.0"/>

    <param name="delta" value="0.01"/>
    <param name="llsamplerange" value="0.01"/>
    <param name="llsamplestep" value="0.01"/>
    <param name="lasamplerange" value="0.005"/>
    <param name="lasamplestep" value="0.005"/>
    <remap from="scan" to="$(arg scan_topic)"/>
</node
```

这里启动了一个slam程序，[slam的介绍](http://www.slamcn.org/index.php/%E9%A6%96%E9%A1%B5)，这个程序主要是根据激光数据实时的重建二维栅格地图，为计算地图的信息熵提供最基本的依据。

```
<include file="$(find turtlebot_exploration_3d)/launch/move/move_base.launch.xml">
    <arg name="laser_topic" value="$(arg scan_topic)"/>
</include>
```

这里启动了movebase，[move_base的介绍](http://wiki.ros.org/move_base)， 这个主要是根据传感器获得的数据构建costmap，costmap指的是二维栅格地图中哪里有障碍物，障碍物周围多大范围机器人不能靠近等等信息，然后根据costmap规划全局路径和局部路径。
`<node pkg="turtlebot_exploration_3d" type="scan_to_pcl" name="scan_to_pcl" />`
这里主要是将kinect2获得的激光数据，从sensor_msgs::LaserScan类型转换为pcl::PointCloud类型。

src/turtlebot_exploration_3d.cpp是核心的程序文件，希望感兴趣的同学直接阅读源码，我在其中关键的地方已经做了注释！

# 注意问题
- tf链接问题，我使用的是turtlebot，使用tf链接的时候曾经出现了机器人抖动的问题（在rviz中看到模拟机器人一直在转来转去，其实此时真实机器人已经不动了）。最后将相机和激光的数据从链接base_link转到了链接base_footprint，问题就解决了。
- 在使用move_base时其中需要配置局部路径规划，源程序使用默认的dwa_local_planner，程序经常报错：“DWA planner failed to produce path”。我发现原因是，我使用的是kinect2，而不是360度的激光，dwa_local_planner 默认使用的是360度激光。
- 程序报错：“The origion for the sensor at(0,0,0.5) is out of map bounds so the costmap cannot raytrace for it”。出现这个问题原因可能是costmap配置文件中z_voxels参数设置可能不合适，这个参数的设置也与垂直方向的分辨率有关。

# 参考文献

[1] S. Bai, J. Wang, K. Doherty, and B. Englot, “Inference-Enabled Information-Theoretic Exploration of Continuous Action Spaces,”Proceedings of the 17th International Symposium on Robotics Research, 16 pp., September 2015.



