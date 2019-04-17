# 用自己的单目&双目相机在ROS 下实现双目ORB_SLAM2 - 站在巨人的肩膀上coding - CSDN博客





2018年04月16日 21:04:06[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1367








用自己的双目相机在ROS 下实现双目ORB_SLAM2

参考博客

ROS下单目SLAM

[https://blog.csdn.net/goding_learning/article/details/52950993](https://blog.csdn.net/goding_learning/article/details/52950993)



配置ROS下的ORB_SLAM2

第一步、先准备好基本依赖

   1.1安装pangolin

1. $ sudo apt-get install libglew-dev   #安装Glew  

2. $ sudo apt-get install cmake         #安装CMake  

3.     #安装Boost  

4. $ sudo apt-get install libboost-dev libboost-thread-dev libboost-filesystem-dev  

5. $ sudo apt-get install libpython2.7-dev  #安装Python2 / Python3  

6.    #下载、编译、安装Pangolin:  

7. $ git clone https://github.com/stevenlovegrove/Pangolin.git  

8. $ cd Pangolin  

9. $ mkdir build  

10. $ cd build  

11. $ cmake -DCPP11_NO_BOOST=1 ..  

12. $ make  

13. $ sudo make install  

 1.2安装opencv2.4.13

 1.3 安装eigen3.1.0

sudo apt-get install libeigen3-dev  

1.4BLAS and LAPACK

$ sudo apt-get install libblas-dev  

$ sudo apt-get install liblapack-dev 

1.4DBoW2 and g2o (Included in Thirdparty folder)

   ORB_SLAM2使用修改版的DBoW2 库进行位置识别 ，使用 g2o 库进行非线性优化. 这两个修改版的库被放在第三方文件夹内



第二步、创建ROS工作空间，将ORB_SLAM2代码下载到catkin_ws/src目录下，并编译

1. $ git clone https://github.com/raulmur/ORB_SLAM2.git ORB_SLAM2  

2. $ cd ORB_SLAM2  

3. $ chmod +x build.sh  

4. $ ./build.sh  

第三步、编译ROS下的ORB_SLAM2包

1、进入Examples/ROS/ORB_SLAM2

2、mkdir build

3、cmake .. -DROS_BUILD_TYPE=Release

4、source ~/catkin_ws/devel/setup.bash

第四步、单目实验

1、启动roscore

2、启动相机节点，根据具体启动文件而定

确保sudo /etc/init.d/ueyeusbdrc start已经运行 【如何加入开机自启】

添加开机启动，参考链接：

[http://www.jb51.net/os/Ubuntu/181138.html](http://www.jb51.net/os/Ubuntu/181138.html)

roslaunch  ueye_cam rgb8.launch（启动单目相机）

或者rosrun ueye stereo（启动双

	rosrun ueye stereo _zoom:=2

3、启动ORB_SLAM2的Mono节点

rosrun ORB_SLAM2 Mono PATH_TO_VOCABULARY(词袋位置) PATH_TO_YMAL_FILE（ymal相机标定文件位置）

 rosrun ORB_SLAM2 Mono src/ORB_SLAM2/Vocabulary/ORBvoc.txt src/ORB_SLAM2/Examples/ROS/ORB_SLAM2/Asus.yaml 

第五步、双目实验

cd catkin_ws

 rosrun ORB_SLAM2 stereo src/ORB_SLAM2/Vocabulary/ORBvoc.txt src/ORB_SLAM2/Examples/ROS/ORB_SLAM2/Asus.yaml 

l 报错：（搜搜这个，很多相关实验，可以学习学习）

rosrun ORB_SLAM2 Stereo path_to_vocabulary path_to_settings do_rectify

解决方法：

[https://blog.csdn.net/u013019296/article/details/70051908](https://blog.csdn.net/u013019296/article/details/70051908)

[https://blog.csdn.net/wangshuailpp/article/details/70231074](https://blog.csdn.net/wangshuailpp/article/details/70231074)

rosrun ORB_SLAM2 stereo src/ORB_SLAM2/Vocabulary/ORBvoc.txt src/ORB_SLAM2/Examples/ROS/ORB_SLAM2/Asus.yaml  true

l 又报错

ERROR: Calibration parameters to rectify stereo are missing!

 Emmmm....突然想起来，双目还没有标定～～～



*****好了，就到这里了，明天来标定一下相机吧，或者把师兄表定好的文件拿过来用用******

rosrun ORB_SLAM2 Stereo src/ORB_SLAM2/Vocabulary/ORBvoc.txt src/ORB_SLAM2/Examples/ROS/ORB_SLAM2/Asus.yaml  false

do_rectify（校正设置，true false）设置为false就可以了，不过这时候没有双目的图像





这时候，应该去查看ROS下的Stereo是从哪两个节点获取双目图像的，然后对应更改ueye启动双目相机后发布的图像消息话题名称。

ORBSLAM2双目图像所在的话题名是

/camera/left/image_raw

/camera/right/image_raw

双目相机在ROS下发布的额话题名称是

/left/image_raw

/right/image_raw

修改ORBSLAM2下的源码，使得话题名称一致！！！然后编译。



   catkin_ws/src/ORB_SLAM2/Examples/ROS/ORB_SLAM2/src/ros_stereo.cc文件中

    message_filters::Subscriber<sensor_msgs::Image> left_sub(nh, "/left/image_raw", 1);//default is /camera/left/image_raw

    message_filters::Subscriber<sensor_msgs::Image> right_sub(nh, "right/image_raw", 1);

重新编译Example

cd ~/catkin_ws/src/ORB_SLAM2/Example/ROS/ORB_SLAM2

mkdir buildcd build

cmake ..

make 

再来一遍命令：

rosrun ORB_SLAM2 stereo src/ORB_SLAM2/Vocabulary/ORBvoc.txt src/ORB_SLAM2/Examples/ROS/ORB_SLAM2/Asus.yaml false



*****出现的问题是，相机一直处在trying to initialize状态下************

****ccurrent frame窗口下显示的是一个相机的灰度图，是有什么问题吗****

******************以上记录于2018.3.26.00:21****************









