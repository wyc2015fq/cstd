# Kinect v1下NVIDIA  TX 2开发板安装ORBSLAM2 - 站在巨人的肩膀上coding - CSDN博客





2018年04月23日 21:17:36[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：346








#### orb-slam2（基于Kinect1）

# 1.基本环境准备

安装pangolin:

n bug1:缺少glew库，搜索安装pangolin需要的依赖，安装pangolin

         sudoapt-get install libglew-dev连接没反应，修改gedit /etc/gai.conf 中precedence :ffff:0:0/96 100前面的注释取消，有效果

         bug2:coundntfind doxygen

                   gitclone [https://github.com/doxygen/doxygen.git](https://github.com/doxygen/doxygen.git)

                   cddoxygen

                   mkdirbuild;cd build;cmake –G “Unix Makefiles” ..;make;make install

  bug3:屏蔽rrealense(pangolin/src/CMakelist.txt)

# 2.orb-slam2安装

                   obslam2不要下载错，是2：[https://github.com/raulmur/ORB_SLAM2](https://github.com/raulmur/ORB_SLAM2)

                   安装顺序：先安装.build.sh然后接着安装build_ros.sh接口版本

         参考博客：[http://www.ncnynl.com/archives/201609/870.html](http://www.ncnynl.com/archives/201609/870.html)

         2.1安装bug--fix    

1)        c++:内部编译器错误（内存不足，新建交换分区，分配给它，重新编译，编译通过后清除分配的交换分区）

     roslaunchfreenect_launch freenect.launch         

2)        realsense报错

a)        到cmake 文件中注销掉realsense的安装

3)        build_ros.sh出错（编译ROS版本得ORB-SLAM2）

在编译ubuntu16.04ros包的时候出现以下问题

`/usr/bin/ld: CMakeFiles/RGBD.dir/src/ros_rgbd.cc.o:undefined reference to symbol ‘_ZN5boost6system15system_categoryEv’

解决方法：[http://blog.csdn.net/sinat_38343378/article/details/78883919](http://blog.csdn.net/sinat_38343378/article/details/78883919)

set(LIBS 

${OpenCV_LIBS} 

${EIGEN3_LIBS} 

${Pangolin_LIBRARIES} 

${PROJECT_SOURCE_DIR}/../../../Thirdparty/DBoW2/lib/libDBoW2.so

${PROJECT_SOURCE_DIR}/../../../Thirdparty/g2o/lib/libg2o.so

${PROJECT_SOURCE_DIR}/../../../lib/libORB_SLAM2.so

之后加入${PROJECT_SOURCE_DIR}/../../../lib/libboost_filesystem.so 

${PROJECT_SOURCE_DIR}/../../../lib/libboost_system.so

4)        运行测试报错，打开三个窗口

a)        roscore

b)        roslaunch freenect_launchfreenect-registered-xyzrgb.launch

c)  rosrun ORB_SLAM2 RGBD/home/ubuntu/SD/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/ubuntu/SD/ORB_SLAM2/Examples/RGB-D

                                      i.             报错：coredump

1.        opencv版本冲突，将ros-kinect中默认的opencv3改为opencv2（Kinect版ros默认使用opencv3【/opt/ros/kinetic/include/opencv-3.1.0-dev】，而orbslam2使用的是opencv2，因此调用ros时候出现版本兼容问题）[http://ask.csdn.net/questions/349714](http://ask.csdn.net/questions/349714)

pkg-config –参数（**指令学习**）

查看opencv版本：pkg-config –modversion opencv

查看依赖库：pkg-config –cflags opencv

查看具体的库:pkg-config –libs opencv

2.        具体解决方案请看：

[http://blog.csdn.net/gauxonz/article/details/52842099](http://blog.csdn.net/gauxonz/article/details/52842099)

ros-kinetic通过cv_bridge.cmake文件关联opencv版本，所以到【/opt/ros/kinetic/share/cv_bridge/cmake/cv_bridgeConfig.cmake】文件中的有关opencv3.1的部分全部改为opencv2的路径（94-96行，及第112行的所有的lib都是opencv3的）

[注意,16.04只能是安装kinetic版本的ROS不支持indigo版本]

[但是在工作站上可以配置不同版本的ROS,自己笔记本是14.04,就配置indigo版本的就好]

a)        cmkae file,change  opencv version(keep original)

find_package(OpenCV 3.2.0 QUIET)  #版本需求,看

if(NOT OpenCV_FOUND)

   find_package(OpenCV 2.4.3 QUIET)

   if(NOT OpenCV_FOUND)

      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")

   endif()

endif()

[https://github.com/raulmur/ORB_SLAM2](https://github.com/raulmur/ORB_SLAM2)

至此,ORB-slam2测试成功.

![](https://img-blog.csdn.net/20180423211651891)




