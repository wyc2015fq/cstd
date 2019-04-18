# ROS：Nvidia Jetson TK1开发平台 - wishchinYang的专栏 - CSDN博客
2016年06月23日 17:51:06[wishchin](https://me.csdn.net/wishchin)阅读数：1441
原文链接：
[http://wiki.ros.org/NvidiaJetsonTK1](http://wiki.ros.org/NvidiaJetsonTK1)
## 1. Nvidia Jetson TK1
Jetson TK1 comes pre-installed with Linux4Tegra OS (basically Ubuntu 14.04 with pre-configured drivers). There is also some official support for running other distributions using the mainline kernel.
K1 开发板暂时是最适合移动机器人使用的开发板
**Power**
12V DC barrel power jack and a 4-pin PC IDE power connector 
## 2. General Setup
- 
Optionally: install the latest [JetPack](http://wiki.ros.org/JetPack) release (which will flash the latest L4T to your Jetson): [link](https://developer.nvidia.com/jetson-tk1-development-pack)
- 
Optionally: install the [Grinch Kernel](https://devtalk.nvidia.com/default/board/139/) (pick a compatible version for your L4T release, which you can check on your Jetson using:
cat /etc/nv_tegra_release, e.g. [21.3](https://devtalk.nvidia.com/default/topic/823132/embedded-systems/-customkernel-the-grinch-21-3-4-for-jetson-tk1-developed/1/)). It provides many useful drivers that NVidia failed to include with their stock kernel.
- 
Install the nvidia cuda toolkit and opencv4tegra from [https://developer.nvidia.com/linux-tegra-rel-21](https://developer.nvidia.com/linux-tegra-rel-21) (not necessary if you installed your Jetson through
[JetPack](http://wiki.ros.org/JetPack))
- older versions of the opencv4tegra we're packaged properly, and attempting to install them alongside the main opencv libraries would result in file conflicts. use the latest version.
- 
If not using "Grinch Kernel" you can enable most Wifi mini-PCIe card and USB interfaces simply installing linux-firmware from PPA:
**sudo apt-get install linux-firmware**
预备工作：
       安装最新的 [JetPack](http://wiki.ros.org/JetPack) 包；................
## 3. Install ROS
With the Ubuntu flavor installed the standard installation instructions should work.
[indigo/Installation/UbuntuARM](http://wiki.ros.org/indigo/Installation/UbuntuARM)
## 4. Use opencv4tegra with ROS
With the latest **opencv4tegra** released by Nvidia, the compatibility problems with
**cv_bridge** and **image_geometry** packages have been solved, so installing OpenCV ROS Packages from PPA does not force
**opencv4tegra **to beuninstalled. There are yet a bit of incompatibility since cv_bridge and image_geometry search for OpenCV 2.4.8 in "/usr/lib/arm-linux-gnueabihf" and opencv4tegra is based on OpenCV 2.4.12 and is installed
 in "/usr/lib/". These diversities do not allow to compile external packages based on OpenCV. To solve the problem you can
[follow this guide](http://myzharbot.robot-home.it/blog/software/ros-nvidia-jetson-tx1-jetson-tk1-opencv-ultimate-guide/). 
Please note that **opencv4tegra **does not include "nonfree" module, so if your algorithms use
**SIFT **or **SURF **and you want full CUDA support, the only solution is to compile OpenCV by yourself following this
[guide](http://elinux.org/Jetson/Installing_OpenCV). Remember that compiling OpenCV by yourself you will lose Nvidia optimizations on the code running on the CPU that give 3-4 FPS more on heavy algorithms not running on CUDA.
## 5. Known Issues
As seen on ROS answers: 
- 
[bug in opencv cmake files](http://answers.ros.org/question/198058/cmake-error-while-building-rpg_svo/)
- 
[using opencv4tegra with ROS](http://answers.ros.org/question/196194/ros-indigo-installation-for-ubuntu-arm-jetson-tk1-ubuntu-1404/)
- 
[image view freezes](http://answers.ros.org/question/197810/image_view-frozen-image/)
