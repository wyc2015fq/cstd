# ubuntu14.04+lsd_slam+bug_fixed - 站在巨人的肩膀上coding - CSDN博客





2018年04月10日 00:38:32[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：255








# **lsd_slam配置方法，吐血整理**

先贴一个LSD_SLAM官方网站：https://vision.in.tum.de/research/vslam/lsdslam


好多链接说按照github原作者的方案，但是很多错误。这也导致大家（也包括博主我走了很多弯路）

这里给出原作者github及给出的方法：https://github.com/tum-vision/lsd_slam/blob/master/lsd_slam_core/manifest.xml

网上其他博客给出的方法（基本不可用，rosmake都通不过，各种问题）：


https://blog.csdn.net/xueyinhualuo/article/details/48490939

https://blog.csdn.net/gobitan/article/details/51541977

http://blog.sina.com.cn/s/blog_7b83134b0102w4de.html

https://blog.csdn.net/zhuquan945/article/details/72980831**(有参考价值)**


https://blog.csdn.net/zhuquan945/article/details/72980831**（自己出现问题后可以参考这个教程解决部分问题）**

**网上一般的教程是这样的:**


- $ sudo apt-get install python-rosinstall  
- $ mkdir ~/rosbuild_ws  
- $ cd ~/rosbuild_ws  
- $ rosws init . /opt/ros/indigo  
- $ mkdir package_dir  
- $ rosws set ~/rosbuild_ws/package_dir -t .  
- $ echo "source ~/rosbuild_ws/setup.bash" >> ~/.bashrc  
- $ bash  
- $ cd package_dir  
- $ sudo apt-get install ros-indigo-libg2o ros-indigo-cv-bridge liblapack-dev libblas-dev freeglut3-dev libqglviewer-dev libsuitesparse-dev libx11-dev  
- $ git clone https://github.com/tum-vision/lsd_slam.git lsd_slam  
- $ rosmake lsd_slam  

但我在做到最后一步即rosmake lsd_slam的时候会出现些错误，加上对rosbuild不太熟悉，所以改用catkin来对LSD-SLAM进行编译。



## **第一部分：配置环境，并使用配置好的数据包回放数据**

先通过以下方法，仔仔catkin方式的lsd_slam


mkdir -p ~/catkin_ws/srcgit clone https://github.com/tum-vision/lsd_slam.gitcd lsd_slamgit checkout catkin
然后cd ~/catkin_ws

catkin_make

然后你看到的就是各种错误。。。。。。我的环境是ubutnu14.04+ROS indigo+opencv 2.4.9+eigen3+cmake3.2+gcc4.9.4


以下是我遇到的错误及解决办法：

（1）第一个错误


![](https://img-blog.csdn.net/20180409225025216)

在编译过程中可能会出现错误：raise Exception(r"""quotes not allowed in description string '%s'""" % description)

顾名思义，不应该在描述中出现引号（单引号或双引号），可以通过查看错误内容得知需要修改的文件名：

**1) lsd_slam_viewer/cfg/LSDSLAMViewerParams.cfg**

第20行scaledDepthVarTH：单词 point's和keyframe's 中出现了单引号，删除单引号

第21行absDepthVarTH：单词 point's

第24行cutFirstNKf：单词 keyframe's




**2) lsd_slam_core/cfg/LSDDebugParams.cfg**

第11行plotStereoImages：单词 what's

第12行plotTracking：单词 what's

第48行continuousPCOutput：单词 Keyfram's




修改上述文件并保存，重新编译：



**[plain]**[view plain](https://blog.csdn.net/softimite_zifeng/article/details/78632506#)[copy](https://blog.csdn.net/softimite_zifeng/article/details/78632506#)

- rosmake lsd_slam  



（2）又出现新的错误

![](https://img-blog.csdn.net/20180409225233134)

目前网上的解决办法是利用catkin_ws来构建

（参考链接：http://www.luohanjie.com/2017-03-17/ubuntu-install-lsd-slam.html；https://blog.csdn.net/zhuquan945/article/details/72980831）

具体步骤为：


mkdir -p ~/catkin_ws/srcgit clone https://github.com/tum-vision/lsd_slam.gitcd lsd_slamgit checkout catkin
安装依赖
sudo apt-get install ros-indigo-libg2o ros-indigo-cv-bridge liblapack-dev libblas-dev freeglut3-dev libqglviewer-dev libsuitesparse-dev libx11-dev
对`lsd_slam/lsd_slam_viewer`和`lsd_slam/lsd_slam_core`文件夹下的`package.xml`中添加：


<build_depend>cmake_modules</build_depend><run_depend>cmake_modules</run_depend>
对`lsd_slam/lsd_slam_viewer`和`lsd_slam/lsd_slam_core`文件夹下的`CMakeFiles.txt`中添加：


find_package(cmake_modules REQUIRED)
并且在所有的`target_link_libraries`中添加`X11`，如：
target_link_libraries(lsdslam ${FABMAP_LIB} ${G2O_LIBRARIES} ${catkin_LIBRARIES} sparse cxsparse X11)
然后开始编译


cd ~/catkin_ws/catkin_make
（3）这里再次出错

![](https://img-blog.csdn.net/20180409225700122)

是找不到eigen目录，所以修改lsd_slam_core和lsd_slam_viewer两个目录下的CMakeLists.txt文件，将原来的find_package(Eigen REQUIRED)改为find_package(Eigen3 REQUIRED)

（4）再次编译catkin_make，上一条错误解决，又报错

![](https://img-blog.csdn.net/20180409225931777)

根据提示，线程开的太多，直接用catkin_make -j1指定单线程编译

![](https://img-blog.csdn.net/20180409230149417)


上条问题解决


(5)又报错fatal error: Eigen/Core: No such file or directory，找不到eigen头文件。我们再次回到上述的两个CMakeLists.txt文件（lsd_slam_core和lsd_slam_viewer目录下）中，将对应的find_package和find_directiories修改为：

find_package(Eigen***3*** REQUIRED)

include_directories(${EIGEN***3***_INCLUDE_DIR})

**之前我们改了包含的包是eigen3，但是没有修改对应的头文件，所以找不到了，我们把include中的eigen改为eigen3**

再次编译：catkin_make  -j1,等待结果

![](https://img-blog.csdn.net/20180409230626550)

哈哈哈，终于编译成功。接下来别忘了source一下：source ~/catkin_ws/devel/setup.bash


虽然中间有几个小的警告，应该也不碍事，暂且不管吧

![](https://img-blog.csdn.net/20180409230711312)

最后的最后，我们来看看运行demo效果

（6）demo运行步骤【参考博客：http://www.luohanjie.com/2017-03-17/ubuntu-install-lsd-slam.html】

下载数据包Download the [Room Example Sequence](http://vmcremers8.informatik.tu-muenchen.de/lsd/LSD_room.bag.zip) and extract it.

数据集：[http://vision.in.tum.de/research/vslam/lsdslam?redirect=1](http://vision.in.tum.de/research/vslam/lsdslam?redirect=1)

- 开第一个终端，运行roscore
- 开第二个终端，运行rosrun lsd_slam_viewer viewer,打开点云查看器

![](https://img-blog.csdn.net/20180409231440532)
- 打开第三个终端：rosrun lsd_slam_core live_slam image:=/image_raw  camera_info:=camera_info
_calib:=/home/nvidia/catkin_ws/src/lsd_slam/lsd_slam_core/calib/OpenCV_example_calib.cfg

- 接下来，开第四个终端，rosbag play ~/Your path/LSD_room.bag （第一步下载的数据包）

![](https://img-blog.csdn.net/20180409232206193)








## **第二部分、使用自己的摄像头跑lsd_slam**

//http://www.luohanjie.com/2017-03-17/ubuntu-install-lsd-slam.html

1、安装驱动


cd ~/catkin_ws/source devel/setup.shcd ~/catkin_ws/srcgit clone https://github.com/ktossell/camera_umd.gitcd ..catkin_make
![](https://img-blog.csdn.net/20180409235134519)

xatkin_make -j1

![](https://img-blog.csdn.net/20180409235333325)

 fatal error: libv4l2.h: No such file or directory


解决方法：安装驱动libv4l-dev，sudo apt-get install libv4l-dev，完成

![](https://img-blog.csdn.net/2018041000003526)



具体步骤：



- 开第一个终端，运行roscore
- 开第二个终端，运行rosrun lsd_slam_viewer viewer,打开点云查看器
- 打开第三个终端，运行rosrun lsd_slam_core live_slam image:=/image_raw  camera_info:=camera_info
_calib:=/home/nvidia/catkin_ws/src/lsd_slam/lsd_slam_core/calib/OpenCV_example_calib.cfg**【换成你自己的路径和参数】**

- 然后开启摄像头，launch文件

          roslaunch uvc_camera camera_node.launch

校正文件calibration_file可参考`lsd_catkin_ws/src/lsd_slam/lsd_slam_core/calib`中的cfg文件。 


【第四步也可以直接使用rosrun uvc_camera uvc_camera_node device:=/dev/video0】



配置camera_node.launch文件(在/lsdslam_catkin_ws/src/camera_umd/uvc_camera/launch中)，如：



- <launch>  
-   <node pkg="uvc_camera" type="uvc_camera_node" name="uvc_camera" output="screen">  
-     <param name="width" type="int" value="640" />  
-     <param name="height" type="int" value="480" />  
-     <param name="fps" type="int" value="30" />  
-     <param name="frame" type="string" value="wide_stereo" />  
- 
-     <param name="auto_focus" type="bool" value="False" />  
-     <param name="focus_absolute" type="int" value="0" />  
-     <!-- other supported params: auto_exposure, exposure_absolute, brightness, power_line_frequency -->  
- 
-     <param name="device" type="string" value="/dev/video0" />  
-     <param name="camera_info_url" type="string" value="file://$(find uvc_camera)/example.yaml" />  
-   </node>  
- </launch>  



注意：官方程序的默认分辨率是640*480.







![](https://img-blog.csdn.net/20180410141740964)
**为什么不能持续显示呢？//TODO//自己标定相机**
## **第三部分、使用openFabMap去闭环检测**

//TODO

//https://blog.csdn.net/xueyinhualuo/article/details/48490939

//论文










