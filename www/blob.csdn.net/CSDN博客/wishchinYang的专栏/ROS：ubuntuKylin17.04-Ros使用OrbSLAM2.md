# ROS：ubuntuKylin17.04-Ros使用OrbSLAM2 - wishchinYang的专栏 - CSDN博客
2017年05月16日 11:37:45[wishchin](https://me.csdn.net/wishchin)阅读数：530
        忙于图像处理和DCNN,很长时间不使用ROS，重新安装系统后，再次使用ORB-SLAM2（ROS）进行三维重建和实时追踪的演示。
        参考以前的文章：[ROS：ubuntu-Ros使用OrbSLAM](http://blog.csdn.net/wishchin/article/details/50468950)
ORB-SLAM2（ROS）的GitHub链接：
        raulmur的主页：[https://github.com/raulmur/](https://github.com/raulmur/)
ORB-SLAM2使用了RGB_D相机，可以在Kinect收集得到的数据集上进行演示。
转述一下ORB-SLAM2的教程
# 一.ORB-SLAM2 安装
**Authors:**[Raul Mur-Artal](http://webdiis.unizar.es/~raulmur/), [Juan D. Tardos](http://webdiis.unizar.es/~jdtardos/), [J. M. M. Montiel](http://webdiis.unizar.es/~josemari/) and [Dorian Galvez-Lopez](http://doriangalvez.com/) ([DBoW2](https://github.com/dorian3d/DBoW2))
**13 Jan 2017**: OpenCV 3 and Eigen 3.3 are now supported.
**22 Dec 2016**: Added AR demo (see section 7).
ORB-SLAM2 is a real-time SLAM library for **Monocular**, ** Stereo** and **RGB-D** cameras that computes the camera trajectory and a sparse 3D reconstruction (in the stereo and RGB-D case with true scale). It is able to detect loops and relocalize the camera in real time. We provide examples to run the SLAM system in the [KITTI dataset](http://www.cvlibs.net/datasets/kitti/eval_odometry.php) as stereo or monocular, in the [TUM dataset](http://vision.in.tum.de/data/datasets/rgbd-dataset) as RGB-D or monocular, and in the [EuRoC dataset](http://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets) as stereo or monocular. We also provide a ROS node to process live monocular, stereo or RGB-D streams.**The library can be compiled without ROS**. ORB-SLAM2 provides a GUI to change between a*SLAM Mode* and*Localization Mode*, see section 9 of this document.
![ORB-SLAM2](https://camo.githubusercontent.com/ef6a123e7f14ec4c39b12ea18b98dc3d8dd87414/687474703a2f2f696d672e796f75747562652e636f6d2f76692f756676505335774a4178302f302e6a7067)![ORB-SLAM2](https://camo.githubusercontent.com/69427dff606900c264436130a440c5725411c4a1/687474703a2f2f696d672e796f75747562652e636f6d2f76692f542d395059434b68444c4d2f302e6a7067)![ORB-SLAM2](https://camo.githubusercontent.com/5c4b150039e838bd058749dc16065a15be827864/687474703a2f2f696d672e796f75747562652e636f6d2f76692f6b50777938794134434b4d2f302e6a7067)
###Related Publications:
[Monocular] Raúl Mur-Artal, J. M. M. Montiel and Juan D. Tardós. **ORB-SLAM: A Versatile and Accurate Monocular SLAM System**.*IEEE Transactions on Robotics,* vol. 31, no. 5, pp. 1147-1163, 2015. (**2015 IEEE Transactions on Robotics Best Paper Award**).**[PDF](http://webdiis.unizar.es/~raulmur/MurMontielTardosTRO15.pdf)**.
[Stereo and RGB-D] Raúl Mur-Artal and Juan D. Tardós. **ORB-SLAM2: an Open-Source SLAM System for Monocular, Stereo and RGB-D Cameras**.*ArXiv preprint arXiv:1610.06475***[PDF](https://128.84.21.199/pdf/1610.06475.pdf)**.
[DBoW2 Place Recognizer] Dorian Gálvez-López and Juan D. Tardós. **Bags of Binary Words for Fast Place Recognition in Image Sequences**.*IEEE Transactions on Robotics,* vol. 28, no. 5, pp. 1188-1197, 2012.**[PDF](http://doriangalvez.com/php/dl.php?dlp=GalvezTRO12.pdf)**
#1. License
ORB-SLAM2 is released under a [GPLv3 license](https://github.com/raulmur/ORB_SLAM2/blob/master/License-gpl.txt). For a list of all code/library dependencies (and associated licenses), please see[Dependencies.md](https://github.com/raulmur/ORB_SLAM2/blob/master/Dependencies.md).
For a closed-source version of ORB-SLAM2 for commercial purposes, please contact the authors: orbslam (at) unizar (dot) es.
If you use ORB-SLAM2 (Monocular) in an academic work, please cite:
```
@article{murTRO2015,
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
if you use ORB-SLAM2 (Stereo or RGB-D) in an academic work, please cite:
```
@article{murORB2,
  title={{ORB-SLAM2}: an Open-Source {SLAM} System for Monocular, Stereo and {RGB-D} Cameras},
  author={Mur-Artal, Ra\'ul and Tard\'os, Juan D.},
  journal={arXiv preprint arXiv:1610.06475},
  year={2016}
 }
```
#2. PrerequisitesWe have tested the library in **Ubuntu 12.04**, ** 14.04** and **16.04**, but it should be easy to compile in other platforms. A powerful computer (e.g. i7) will ensure real-time performance and provide more stable and accurate results.
## C++11 or C++0x Compiler
We use the new thread and chrono functionalities of C++11.
## Pangolin
We use [Pangolin](https://github.com/stevenlovegrove/Pangolin) for visualization and user interface. Dowload and install instructions can be found at:[https://github.com/stevenlovegrove/Pangolin](https://github.com/stevenlovegrove/Pangolin).
## OpenCV
We use [OpenCV](http://opencv.org) to manipulate images and features. Dowload and install instructions can be found at:[http://opencv.org](http://opencv.org).**Required at leat 2.4.3. Tested with OpenCV 2.4.11 and OpenCV 3.2**.
## Eigen3
Required by g2o (see below). Download and install instructions can be found at:[http://eigen.tuxfamily.org](http://eigen.tuxfamily.org).**Required at least 3.1.0**.
## DBoW2 and g2o (Included in Thirdparty folder)
We use modified versions of the [DBoW2](https://github.com/dorian3d/DBoW2) library to perform place recognition and [g2o](https://github.com/RainerKuemmerle/g2o) library to perform non-linear optimizations. Both modified libraries (which are BSD) are included in the*Thirdparty* folder.
## ROS (optional)
We provide some examples to process the live input of a monocular, stereo or RGB-D camera using[ROS](https://github.com/raulmur/ORB_SLAM2/blob/master/ros.org). Building these examples is optional. In case you want to use ROS, a version Hydro or newer is needed.
#3. Building ORB-SLAM2 library and TUM/KITTI examples
Clone the repository:
```cpp
git clone https://github.com/raulmur/ORB_SLAM2.git ORB_SLAM2
```
We provide a script `build.sh` to build the *Thirdparty* libraries and*ORB-SLAM2*. Please make sure you have installed all required dependencies (see section 2). Execute:
```cpp
cd ORB_SLAM2
chmod +x build.sh
./build.sh
```
**注意事项**：安装附加依赖库...
**出错及解决方法**：
在
```cpp
./build.sh
```
过程的最后
sudo make -j
出现 **usleep 未定义**问题
解决方法：
找到所有包含这个函数的源代码
在 头部添加：
#include <unistd.h>
则可以编译成功Q！
This will create **libORB_SLAM2.so** at *lib* folder and the executables**mono_tum**,**mono_kitti**,**rgbd_tum**,**stereo_kitti**,**mono_euroc** and**stereo_euroc** in*Examples* folder.
#4. Monocular Examples
# 二.例程和数据集
## TUM Dataset
- 
Download a sequence from [http://vision.in.tum.de/data/datasets/rgbd-dataset/download](http://vision.in.tum.de/data/datasets/rgbd-dataset/download) and uncompress it.
- 
Execute the following command. Change `TUMX.yaml` to TUM1.yaml,TUM2.yaml or TUM3.yaml for freiburg1, freiburg2 and freiburg3 sequences respectively. Change`PATH_TO_SEQUENCE_FOLDER`to the uncompressed sequence folder.
```cpp
./Examples/Monocular/mono_tum Vocabulary/ORBvoc.txt Examples/Monocular/TUMX.yaml PATH_TO_SEQUENCE_FOLDER
```
注释：慕尼黑工业大学 TUM数据集给出了相应的软件工具集：[http://vision.in.tum.de/data/software](http://vision.in.tum.de/data/software) 。
           数据集（3D场景）下载地址：[http://vision.in.tum.de/data/datasets/omni-lsdslam#dataset](http://vision.in.tum.de/data/datasets/omni-lsdslam#dataset)
## KITTI Dataset
- 
Download the dataset (grayscale images) from [http://www.cvlibs.net/datasets/kitti/eval_odometry.php](http://www.cvlibs.net/datasets/kitti/eval_odometry.php)
- 
Execute the following command. Change `KITTIX.yaml`by KITTI00-02.yaml, KITTI03.yaml or KITTI04-12.yaml for sequence 0 to 2, 3, and 4 to 12 respectively. Change`PATH_TO_DATASET_FOLDER` to the uncompressed dataset folder. Change`SEQUENCE_NUMBER` to 00, 01, 02,.., 11.
```
./Examples/Monocular/mono_kitti Vocabulary/ORBvoc.txt Examples/Monocular/KITTIX.yaml PATH_TO_DATASET_FOLDER/dataset/sequences/SEQUENCE_NUMBER
```
**里程数据集**：大型户外数据集合
## EuRoC Dataset
- 
Download a sequence (ASL format) from [http://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets](http://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets)
- 
Execute the following first command for V1 and V2 sequences, or the second command for MH sequences. Change PATH_TO_SEQUENCE_FOLDER and SEQUENCE according to the sequence you want to run.
```
./Examples/Monocular/mono_euroc Vocabulary/ORBvoc.txt Examples/Monocular/EuRoC.yaml PATH_TO_SEQUENCE_FOLDER/mav0/cam0/data Examples/Monocular/EuRoC_TimeStamps/SEQUENCE.txt
```
```
./Examples/Monocular/mono_euroc Vocabulary/ORBvoc.txt Examples/Monocular/EuRoC.yaml PATH_TO_SEQUENCE/cam0/data Examples/Monocular/EuRoC_TimeStamps/SEQUENCE.txt
```
#5. Stereo Examples
** Micro Aerial Vehicle** ：用于室内无人机进行场景建模的数据集合
## KITTI Dataset
- 
Download the dataset (grayscale images) from [http://www.cvlibs.net/datasets/kitti/eval_odometry.php](http://www.cvlibs.net/datasets/kitti/eval_odometry.php)
- 
Execute the following command. Change `KITTIX.yaml`to KITTI00-02.yaml, KITTI03.yaml or KITTI04-12.yaml for sequence 0 to 2, 3, and 4 to 12 respectively. Change`PATH_TO_DATASET_FOLDER` to the uncompressed dataset folder. Change`SEQUENCE_NUMBER` to 00, 01, 02,.., 11.
```
./Examples/Stereo/stereo_kitti Vocabulary/ORBvoc.txt Examples/Stereo/KITTIX.yaml PATH_TO_DATASET_FOLDER/dataset/sequences/SEQUENCE_NUMBER
```
## EuRoC Dataset
- 
Download a sequence (ASL format) from [http://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets](http://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets)
- 
Execute the following first command for V1 and V2 sequences, or the second command for MH sequences. Change PATH_TO_SEQUENCE_FOLDER and SEQUENCE according to the sequence you want to run.
```
./Examples/Stereo/stereo_euroc Vocabulary/ORBvoc.txt Examples/Stereo/EuRoC.yaml PATH_TO_SEQUENCE/mav0/cam0/data PATH_TO_SEQUENCE/mav0/cam1/data Examples/Stereo/EuRoC_TimeStamps/SEQUENCE.txt
```
```
./Examples/Stereo/stereo_euroc Vocabulary/ORBvoc.txt Examples/Stereo/EuRoC.yaml PATH_TO_SEQUENCE/cam0/data PATH_TO_SEQUENCE/cam1/data Examples/Stereo/EuRoC_TimeStamps/SEQUENCE.txt
```
#6. RGB-D Example
## TUM Dataset
- 
Download a sequence from [http://vision.in.tum.de/data/datasets/rgbd-dataset/download](http://vision.in.tum.de/data/datasets/rgbd-dataset/download) and uncompress it.
- 
Associate RGB images and depth images using the python script [associate.py](http://vision.in.tum.de/data/datasets/rgbd-dataset/tools). We already provide associations for some of the sequences in * Examples/RGB-D/associations/*. You can generate your own associations file executing:
```
python associate.py PATH_TO_SEQUENCE/rgb.txt PATH_TO_SEQUENCE/depth.txt > associations.txt
```
- Execute the following command. Change `TUMX.yaml` to TUM1.yaml,TUM2.yaml or TUM3.yaml for freiburg1, freiburg2 and freiburg3 sequences respectively. Change`PATH_TO_SEQUENCE_FOLDER`to the uncompressed sequence folder. Change`ASSOCIATIONS_FILE` to the path to the corresponding associations file.
```
./Examples/RGB-D/rgbd_tum Vocabulary/ORBvoc.txt Examples/RGB-D/TUMX.yaml PATH_TO_SEQUENCE_FOLDER ASSOCIATIONS_FILE
```
#7. ROS Examples
### Building the nodes for mono, monoAR, stereo and RGB-D
- Add the path including *Examples/ROS/ORB_SLAM2* to the ROS_PACKAGE_PATH environment variable. Open .bashrc file and add at the end the following line. Replace PATH by the folder where you cloned ORB_SLAM2:
```
export ROS_PACKAGE_PATH=${ROS_PACKAGE_PATH}:PATH/ORB_SLAM2/Examples/ROS
```
- Execute `build_ros.sh` script:
```
chmod +x build_ros.sh
./build_ros.sh
```
### Running Monocular Node
For a monocular input from topic `/camera/image_raw` run node ORB_SLAM2/Mono. You will need to provide the vocabulary file and a settings file. See the monocular examples above.
```
rosrun ORB_SLAM2 Mono PATH_TO_VOCABULARY PATH_TO_SETTINGS_FILE
```
### Running Monocular Augmented Reality Demo
This is a demo of augmented reality where you can use an interface to insert virtual cubes in planar regions of the scene.The node reads images from topic`/camera/image_raw`.
```
rosrun ORB_SLAM2 MonoAR PATH_TO_VOCABULARY PATH_TO_SETTINGS_FILE
```
### Running Stereo Node
For a stereo input from topic `/camera/left/image_raw` and `/camera/right/image_raw` run node ORB_SLAM2/Stereo. You will need to provide the vocabulary file and a settings file. If you**provide rectification matrices** (see Examples/Stereo/EuRoC.yaml example), the node will recitify the images online,**otherwise images must be pre-rectified**.
```
rosrun ORB_SLAM2 Stereo PATH_TO_VOCABULARY PATH_TO_SETTINGS_FILE ONLINE_RECTIFICATION
```
**Example**: Download a rosbag (e.g. V1_01_easy.bag) from the EuRoC dataset ([http://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets](http://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets)). Open 3 tabs on the terminal and run the following command at each tab:
```
roscore
```
```
rosrun ORB_SLAM2 Stereo Vocabulary/ORBvoc.txt Examples/Stereo/EuRoC.yaml true
```
```
rosbag play --pause V1_01_easy.bag /cam0/image_raw:=/camera/left/image_raw /cam1/image_raw:=/camera/right/image_raw
```
Once ORB-SLAM2 has loaded the vocabulary, press space in the rosbag tab. Enjoy!. Note: a powerful computer is required to run the most exigent sequences of this dataset.
### Running RGB_D Node
For an RGB-D input from topics `/camera/rgb/image_raw` and `/camera/depth_registered/image_raw`, run node ORB_SLAM2/RGBD. You will need to provide the vocabulary file and a settings file. See the RGB-D example above.
```
rosrun ORB_SLAM2 RGBD PATH_TO_VOCABULARY PATH_TO_SETTINGS_FILE
```
#8. Processing your own sequencesYou will need to create a settings file with the calibration of your camera. See the settings file provided for the TUM and KITTI datasets for monocular, stereo and RGB-D cameras. We use the calibration model of OpenCV. See the examples to learn how to create a program that makes use of the ORB-SLAM2 library and how to pass images to the SLAM system. Stereo input must be synchronized and rectified. RGB-D input must be synchronized and depth registered.
#9. SLAM and Localization ModesYou can change between the *SLAM* and * Localization mode* using the GUI of the map viewer.
### SLAM Mode
This is the default mode. The system runs in parallal three threads: Tracking, Local Mapping and Loop Closing. The system localizes the camera, builds new map and tries to close loops.
### Localization Mode
This mode can be used when you have a good map of your working area. In this mode the Local Mapping and Loop Closing are deactivated. The system localizes the camera in the map (which is no longer updated), using relocalization if needed.
