# ubuntu14.04下创建catkin_ws工作空间，靠谱，简单，清晰 - 站在巨人的肩膀上coding - CSDN博客





2018年04月12日 09:11:39[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1647








经常会用到这个，但是网上资料比较杂乱，转载一下，以表收藏


**1.****mkdir -p ~/catkin_ws/src**解析：
在 /home/你的用户名目录/ 下创建一个ROS工作空间，即文件夹catkin_ws,然后在工作空间 catkin_ws 下创建一个文件夹 src你也可以将工作空间 catkin_ws 起成其他名字，例如：mkdir -p ~/tutorial/src  ，那么下面的所有命令中的catkin_ws都要对应换成 tutorial（命令 ls 的功能即查看当前所在目录里有哪些文件）**2.****cd ~/catkin_ws/src**解析：
打开工作空间catkin_ws下的文件夹src
(命令 pwd 的功能即查看当前所在目录)**3.****catkin_init_workspace**解析：
执行完该命令后，src目录下会多出一个 CMakeLists.txt 文件。
**4.****cd ~/catkin_ws/**解析：
返回到上层目录
**5.****catkin_make**解析：
执行完该命令后，发现工作空间catkin_ws中有三个目录： build  devel  src
其中，src是我们创建工作空间时创建的目录，另外两个是执行完 **catkin_make **后生成的。
**6.****source devel/setup.bash**解析：
将对应的工作空间的路径加入环境变量**ROS_PACKAGE_PATH**中。
如果新开了一个终端命令行，在使用该工作空间前，必须先将该工作空间的路径加入环境变量**ROS_PACKAGE_PATH**中。**7.****echo $ROS_PACKAGE_PATH**解析：
查看当前环境变量
**如果打印出以下信息，说明你的工作环境设置好了：**/home/你的用户名/你的工作空间名/src:/opt/ros/indigo/share:/opt/ros/indigo/stacks
创建一个ROS Workspace 的方法总结：
**mkdir -p ~/catkin_ws/src****cd ~/catkin_ws/src****catkin_init_workspace****cd ~/catkin_ws/****catkin_make****source devel/setup.bash**



