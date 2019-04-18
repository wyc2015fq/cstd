# ROS：ubuntu-Ros使用OrbSLAM - wishchinYang的专栏 - CSDN博客
2016年01月06日 16:27:46[wishchin](https://me.csdn.net/wishchin)阅读数：8269
一般无误的官方连接：[https://github.com/raulmur/ORB_SLAM](https://github.com/raulmur/ORB_SLAM)
### ubuntu16.04没有多少改变，还是使用kinetic老代替indigo
### Related Publications:
Artical:
[1] Raúl Mur-Artal, J. M. M. Montiel and Juan D. Tardós. **ORB-SLAM: A Versatile and Accurate Monocular SLAM System**.*IEEE Transactions on Robotics,* vol. 31, no. 5, pp. 1147-1163, 2015.
DOI: 10.1109/TRO.2015.2463671.
Link to pdf: [http://webdiis.unizar.es/~raulmur/MurMontielTardosTRO15.pdf](http://webdiis.unizar.es/~raulmur/MurMontielTardosTRO15.pdf)
# 1. License
ORB-SLAM is released under a GPLv3 license. Please note that we provide along ORB-SLAM a modified version of g2o and DBoW2 which are both BSD.
For a closed-source version of ORB-SLAM for commercial purposes, please contact the authors.
If you use ORB-SLAM in an academic work, please cite:
```
@article{murAcceptedTRO2015,
  title={{ORB-SLAM}: a Versatile and Accurate Monocular {SLAM} System},
  author={Mur-Artal, Ra\'ul, Montiel, J. M. M. and Tard\'os, Juan D.},
  journal={IEEE Transactions on Robotics},
  volume={31},
  number={5},
  pages={1147--1163},
  doi = {10.1109/TRO.2015.2463671},
  year={2015}
 }
```
# 2. Prerequisites (dependencies)
## 2.1 Boost
   We use the Boost library to launch the different threads of our SLAM system.
```cpp
sudo apt-get install libboost-all-dev
```
## 2.2 ROS
      We use ROS to receive images from the camera or from a recorded sequence (rosbag), and for visualization (rviz, image_view).**We have tested ORB-SLAM in Ubuntu 12.04 with ROS Fuerte, Groovy and Hydro; and in Ubuntu 14.04 with ROS Indigo**.
 If you do not have already installed ROS in your computer, we recommend you to install the Full-Desktop version of ROS Fuerte ([http://wiki.ros.org/fuerte/Installation/Ubuntu](http://wiki.ros.org/fuerte/Installation/Ubuntu)).
**If you use ROS Indigo, remove the depency of opencv2 in the manifest.xml.**
## 2.3 g2o (included)
       We use g2o to perform several optimizations. We include a modified copy of the library including only the components we need and also some changes that are listed in`Thirdparty/g2o/Changes.txt`. In order to compile g2o you will need to
 have installed CHOLMOD, BLAS, LAPACK and Eigen3.
```cpp
sudo apt-get install libsuitesparse-dev
sudo apt-get install libblas-dev
sudo apt-get install liblapack-dev
sudo apt-get install libeigen3-dev
```
## 2.4 DBoW2 (included)
        We make use of some components of the DBoW2 library ([https://github.com/dorian3d/DBoW2](https://github.com/dorian3d/DBoW2)) for place recognition and feature matching. We include a modified copy of the
 library including only the components we need and also some modifications that are listed in`Thirdparty/DBoW2/LICENSE.txt`. It only depends on OpenCV, but it should be included in the ROS distribution.
# 3. Installation
- 
Make sure you have installed ROS and all library dependencies (boost, eigen3, cholmod, blas, lapack).
- 
Clone the repository:
- 
```cpp
git clone https://github.com/raulmur/ORB_SLAM.git ORB_SLAM
```
``
- 
Add the path where you cloned ORB-SLAM to the `ROS_PACKAGE_PATH` environment variable (better if you add the export line to your .bashrc file)
- 
Build g2o. Go into `Thirdparty/g2o/` and execute:
- 
```cpp
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```
- 
*Tip: To achieve the best performance in your computer, set your favorite compilation flags in line 97 and 98 of*`Thirdparty/g2o/CMakeLists.txt` (by default -03 -march=native)
- 
Build DBoW2. Go into Thirdparty/DBoW2/ and execute:
- 
```cpp
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make (如果未安装OpenCV，首先编译OpenCV：
ubuntu16.04下编译安装OpenCV )
```
- 
*Tip: Set your favorite compilation flags in line 4 and 5 of*`Thirdparty/DBoW2/CMakeLists.txt` (by default -03 -march=native)
- 
Build ORB_SLAM. In the ORB_SLAM root execute:
**              If you use ROS Indigo, remove the depency of opencv2 in the manifest.xml.**
- 
```cpp
mkdir build
cd build
cmake .. -DROS_BUILD_TYPE=Release
make
```
- 
*Tip: Set your favorite compilation flags in line 12 and 13 of*`./CMakeLists.txt` (by default -03 -march=native)
             出现问题：
                    cmake .. -DROS_BUILD_TYPE=Release
           1.        这一句出现：
                           CMake Error at CMakeLists.txt:2 (include):
                           include could not find load file:
                            /core/rosbuild/rosbuild.cmake
                         CMake Error at CMakeLists.txt:4 (rosbuild_init):
                          Unknown CMake command "rosbuild_init".
                         -- Configuring incomplete, errors occurred!
                        See also "/home/wishchin/WorkFrameSrc/ORB_SLAM-master/build/CMakeFiles/CMakeO
                解决方法：原因是ROS path 没有自动添加，需要进行手动添加。
         shell语句：
```cpp
source /opt/ros/indigo/setup.bash   source /opt/ros/kinetic/setup.bash
```
- 切换到主目录下。。。。  
- source  .bashrc #使能  
       2.    出现：                      
[rosbuild] Building package ORB_SLAM-master
[rosbuild] Error from directory check: /opt/ros/indigo/share/ros/core/rosbuild/bin/check_same_directories.py  /home/wishchin/WorkFrameSrc/ORB_SLAM-master
1
Traceback (most recent call last):
  File "/opt/ros/indigo/share/ros/core/rosbuild/bin/check_same_directories.py", line 46, in <module>
    raise Exception
Exception
CMake Error at /opt/ros/indigo/share/ros/core/rosbuild/private.cmake:102 (message):
  [rosbuild] rospack found package "ORB_SLAM-master" at "", but the current
  directory is "/home/wishchin/WorkFrameSrc/ORB_SLAM-master".  You should
  double-check your ROS_PACKAGE_PATH to ensure that packages are found in the
  correct precedence order.
Call Stack (most recent call first):
  /opt/ros/indigo/share/ros/core/rosbuild/public.cmake:177 (_rosbuild_check_package_location)
  CMakeLists.txt:4 (rosbuild_init)
-- Configuring incomplete, errors occurred!
See also "/home/wishchin/WorkFrameSrc/ORB_SLAM-master/build/CMakeFiles/CMakeOutput.log".
         解决方法：                       
         在.bashrc 末尾添加：export  ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:/home/wishchin/WorkFrameSrc/ORB_SLAM
         ORB路径添加到ROS
7.编译成功：
[ 11%] Building CXX object CMakeFiles/ORB_SLAM-master.dir/src/Tracking.cc.o
[ 16%] Building CXX object CMakeFiles/ORB_SLAM-master.dir/src/LocalMapping.cc.o
[ 22%] Building CXX object CMakeFiles/ORB_SLAM-master.dir/src/LoopClosing.cc.o
[ 27%] Building CXX object CMakeFiles/ORB_SLAM-master.dir/src/ORBextractor.cc.o
[ 33%] Building CXX object CMakeFiles/ORB_SLAM-master.dir/src/ORBmatcher.cc.o
[ 38%] Building CXX object CMakeFiles/ORB_SLAM-master.dir/src/FramePublisher.cc.o
[ 44%] Building CXX object CMakeFiles/ORB_SLAM-master.dir/src/Converter.cc.o
[ 50%] Building CXX object CMakeFiles/ORB_SLAM-master.dir/src/MapPoint.cc.o
[ 55%] Building CXX object CMakeFiles/ORB_SLAM-master.dir/src/KeyFra
# 4. Usage
**See section 5 to run the Example Sequence**.
- 
Launch ORB-SLAM from the terminal (`roscore` should have been already executed):
```
rosrun ORB_SLAM ORB_SLAM PATH_TO_VOCABULARY PATH_TO_SETTINGS_FILE
```
You have to provide the path to the ORB vocabulary and to the settings file. The paths must be absolute or relative to the ORB_SLAM directory.
We already provide the vocabulary file we use in `ORB_SLAM/Data/ORBvoc.txt.tar.gz`. Uncompress the file, as it will be loaded much faster.
- 
The last processed frame is published to the topic `/ORB_SLAM/Frame`. You can visualize it using`image_view`:
```
rosrun image_view image_view image:=/ORB_SLAM/Frame _autosize:=true
```
- 
The map is published to the topic `/ORB_SLAM/Map`, the current camera pose and global world coordinate origin are sent through`/tf` in frames`/ORB_SLAM/Camera` and`/ORB_SLAM/World` respectively. Run`rviz`
 to visualize the map:
*in ROS Fuerte*:
```
rosrun rviz rviz -d Data/rviz.vcg
```
*in ROS Groovy or a newer version*:
```
rosrun rviz rviz -d Data/rviz.rviz
```
- 
ORB_SLAM will receive the images from the topic `/camera/image_raw`. You can now play your rosbag or start your camera node. If you have a sequence with individual image files, you will need to generate a bag from them. We provide a tool to do
 that: [https://github.com/raulmur/BagFromImages](https://github.com/raulmur/BagFromImages).
**Tip: Use a roslaunch to launch `ORB_SLAM`, `image_view` and`rviz` from just one instruction. We provide an example**:
*in ROS Fuerte*:
```
~~roslaunch ExampleFuerte.launch~~
```
*in ROS Groovy or a newer version*:
```
roslaunch ExampleGroovyOrNewer.launch
```
# 5. Example Sequence
We provide the settings and the rosbag of an example sequence in our lab. In this sequence you will see a loop closure and two relocalisation from a big viewpoint change.
- 
Download the rosbag file:
[http://webdiis.unizar.es/~raulmur/orbslam/downloads/Example.bag.tar.gz](http://webdiis.unizar.es/~raulmur/orbslam/downloads/Example.bag.tar.gz).
Alternative link: [https://drive.google.com/file/d/0B8Qa2__-sGYgRmozQ21oRHhUZWM/view?usp=sharing](https://drive.google.com/file/d/0B8Qa2__-sGYgRmozQ21oRHhUZWM/view?usp=sharing)
Uncompress the file.
- 
Launch ORB_SLAM with the settings for the example sequence. You should have already uncompressed the vocabulary file (`/Data/ORBvoc.txt.tar.gz`)
*in ROS Fuerte*:
```
~~roslaunch ExampleFuerte.launch~~
```
*in ROS Groovy or newer versions*:
`````rosbag play --pause Example.bag  +空格键`roslaunch  ExampleGroovyOrNewer.launch``
- 
Once the ORB vocabulary has been loaded, play the rosbag (press space to start):
```
rosbag play --pause Example.bag
```
# 6. The Settings File
ORB_SLAM reads the camera calibration and setting parameters from a YAML file. We provide an example in`Data/Settings.yaml`, where you will find all parameters and their description. We use the camera calibration model of OpenCV.
Please make sure you write and call your own settings file for your camera (copy the example file and modify the calibration)
# 7. Failure Modes
You should expect to achieve good results in sequences similar to those in which we show results in our paper [1], in terms of camera movement and texture in the environment. In general our Monocular SLAM solution is expected to have a bad time in the following
 situations:
- Pure rotations in exploration
- Low texture environments
- Many (or big) moving objects, especially if they move slowly.
The system is able to initialize from planar and non-planar scenes. In the case of planar scenes, depending on the camera movement relative to the plane, it is possible that the system refuses to initialize, see the paper [1] for details.
# 8. Need Help?
If you have any trouble installing or running ORB-SLAM, contact the authors.
