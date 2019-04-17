# 看我是怎么学SLAM（二）——3D自主探索建图源码解读 - 站在巨人的肩膀上coding - CSDN博客





2018年05月12日 20:43:13[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：500








原出处：[https://blog.csdn.net/x1825048925/article/details/77187168](https://blog.csdn.net/x1825048925/article/details/77187168)

作者：今夕何夕

作者博客：[https://blog.csdn.net/x1825048925](https://blog.csdn.net/x1825048925)

### 说明

　　这份源码能实现Kobuki自主探索房间并建立房间的三维地图，地图表现形式是八叉树。

### 源码下载与配置

步骤：
- 安装octomap_ros和rviz插件
`sudo apt-get install ros-indigo-octomap*`- 1
- 源码安装

```php
cd turtlebot_ws/src
git clone https://github.com/RobustFieldAutonomyLab/turtlebot_exploration_3d.git
catkin_make
```
- 1
- 2
- 3
- deb包安装：

```
sudo apt-get update
sudo apt-get install ros-indigo-turtlebot-exploration-3d
```
- 1
- 2

运行：

```
roslaunch turtlebot_exploration_3d minimal_explo.launch
roslaunch turtlebot_exploration_3d turtlebot_gmapping.launch
rosrun turtlebot_exploration_3d turtlebot_exploration_3d
roslaunch turtlebot_exploration_3d exploration_rviz.launch
```
- 1
- 2
- 3
- 4

　　最后一条指令是打开RVIZ查看地图，不打开机器人已经能够开始探索啦。

### 代码解读

　　主程序是在src目录下的turtlebot_exploration_3d.cpp，主要讲解这个文件。 
1. 全局变量声明和配置
　　位置：exploration.h的21到40行 
　　说明：
- 第25行的octo_reso变量定义了octomap中的最小分辨率，如果要提高octomap的分辨率可以适当减小该变量的值；
- 第28行的num_of_bay定义为高斯过程的迭代次数;
- 第31至33行是对octomap和octomap消息的变量声明，特别是cur_tree，作为后面程序中octomap的指针变量；
- 第37行声明的三维点变量kinect_orig在程序中表示机器人的实时位置；
- 
第40行octomap_name_3d变量在turlebot_exploration.cpp的第38行中被初始化为Octomap3D_%m%d_%R_%S.ot的形式，并在exploration.h的第291行中，作为保存octomap的名字； 
2. 全局变量初始化
位置： turtlebot_exploration_3d.cpp的25到72行 
说明：

- 
第42至47行定义了程序中消息的订阅和发布, kinect_sub为订阅者接受kinect图像信息, GoalMarker_pub发布目标点信息, Candidates_pub发布候选点信息, 
Frontier_points_pub发布前沿点信息, pub_twist发布对机器人的控制信息,Octomap_pub发布octomap的信息;

- 第64至68行初始化octomap;
- 
第69行定义下一个探测点next_vp; 
3. 更新机器人的初始位置
位置: turtlebot_exploration_3d.cpp的74到108行 
说明:

- 
第77至88行从kinect发布的消息中获取图像信息, 通过lookupTransform函数获取位置信息，并且更新机器人的位置kinect_orig;

- 第91行ros::spinOnce()是ros的回调函数, 回调在exploration.h中265行定义的kinectCallbacks函数，通过kinect传来的图像消息,更新并发送和保存octomap信息;
- 
第97至103行为旋转60度，原因是twist_cmd.angular.z定义旋转角速度为0.6，旋转时间由ros::Duration(2.6)定义为2.6-1=1.6的旋转角度为(0.6×1.6) × 180/π = 55 ≈ 60； 
4. 获取并发布前沿点信息
位置: turtlebot_exploration_3d.cpp的115到157行 
说明:

- 
第115行中，通过extractFrontierPoints函数从octomap中获取前沿点，该获取前沿点的函数定义在exploration.h的111至184行;

- 
第118至157行根据获取到的前沿点数据frontier_groups，创建前沿点消息Frontier_points_cubelist并将前沿点消息发布出去; 
5. 提取候选点
位置: turtlebot_exploration_3d.cpp的159到169行 
说明:

- 
在160行中，使用extractCandidateViewPoints函数从前沿点中提取候选点，该函数声明在exploration.h的188至252行;

- 
在164至169行中，如果候选点的数量不够，程序终止; 
6. 为每个候选点初始化交互信息(mutual information)
位置: turtlebot_exploration_3d.cpp的175到203行 
说明:

- 
第177行使用countFreeVolume函数计算octomap中的空闲空间体积大小保存在before中，该函数定义在exploration.h的80至87行;

- 
第185至196行中，使用calc_MI函数初始化交互信息数组MIs, 为每个元素赋值为该候选点的空闲空间与当前空闲空间的体积之差，保存在MIs数组中； 
7. 高斯过程回归
位置: turtlebot_exploration_3d.cpp的207到251行 
说明:

- 
第212至223行初始化高斯回归的参数;

- 第225至250行执行高斯回归过程，得到acquisition function,并计算交互信息;
- 
详细可见论文:Information-Theoretic Exploration with Bayesian Optimization 
8. 发布候选点信息
位置: turtlebot_exploration_3d.cpp的266到297行 
说明:

- 
第272至294行，把每个候选点的信息加入要发布的消息CandidatesMarker_array中；

- 
第295行发布候选点消息； 
9. 选取并到达目标点
位置: turtlebot_exploration_3d.cpp的304到384行 
说明:

- 
第306行，通过候选点和交互信息选取下一个目标点，该交互信息数组已经使用exploration.h中第42至50行的sort_MIs函数进行了排序;

- 第307至333行，向rviz中发布目标点信息;
- 第336行，使用定义在navigation_utils.cpp中的goToDest函数向MoveBaseClient发送目标点信息，使机器人移动到目标点;
- 第338至373行，如果到达了目标点，则更新octomap并且发送msg_octomap消息;
- 第374至382行，如果没有到达目标点，且没有更多的目标点可以选择，程序结束； 
10. 点云地图的保存
在程序中使用kinect采集的点云信息的位置在exploration.h中，第265至296行的kinectCallbacks函数，该函数通过ros::spinOnce被调用. 该函数传入的参数cloud2_msg为ros的消息，程序266至277行经过如下转化

```
pcl::PCLPointCloud2 cloud2;
pcl_conversions::toPCL(*cloud2_msg, cloud2);
PointCloud* cloud (new PointCloud);
PointCloud* cloud_local (new PointCloud);
pcl::fromPCLPointCloud2(cloud2,*cloud_local);
octomap::Pointcloud hits;
ros::Duration(0.07).sleep();
while(!pcl_ros::transformPointCloud("/map", *cloud_local, *cloud, *tf_listener))
{
ros::Duration(0.01).sleep();
}
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
- 11

便把点云消息转化为点云数据保存在cloud指针指向的点云中；之后就可以用相应的保存点云数据函数保存点云图像，比如
`pcl::io::savePCDFileASCII (cloud_name, cloud); // 保存点云`- 1

### Octomap的保存和读取

环境配置
- 下载octomap源码

```php
git clone https://github.com/OctoMap/octomap
```
- 1
- 编译

```bash
mkdir build
cd build
cmake ..
make
```
- 1
- 2
- 3
- 4
- 使用

```
cd octomap/bin
./octovis the .ot file to view
```
- 1
- 2

　　这就是这份开源代码的个人解读与扩展使用



