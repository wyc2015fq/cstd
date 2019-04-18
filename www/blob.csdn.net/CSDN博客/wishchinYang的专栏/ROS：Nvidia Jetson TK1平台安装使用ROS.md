# ROS：Nvidia Jetson TK1平台安装使用ROS - wishchinYang的专栏 - CSDN博客
2016年06月23日 17:53:04[wishchin](https://me.csdn.net/wishchin)阅读数：2943
原文连接：
[http://wiki.ros.org/indigo/Installation/UbuntuARM](http://wiki.ros.org/indigo/Installation/UbuntuARM)
# Ubuntu ARM install of ROS Indigo
There are currently builds of ROS for Ubuntu Trusty armhf. These builds include most but not all packages, and save a considerable amount of time compared to doing a full source-based installation.
目录
- [Ubuntu ARM install of ROS Indigo](http://wiki.ros.org/indigo/Installation/UbuntuARM#Ubuntu_ARM_install_of_ROS_Indigo)- [Supported/Tested Platforms](http://wiki.ros.org/indigo/Installation/UbuntuARM#Supported.2BAC8-Tested_Platforms)
- [Installation](http://wiki.ros.org/indigo/Installation/UbuntuARM#Installation)- [Configure your Ubuntu repositories](http://wiki.ros.org/indigo/Installation/UbuntuARM#Configure_your_Ubuntu_repositories)
- [Set your Locale](http://wiki.ros.org/indigo/Installation/UbuntuARM#Set_your_Locale)
- [Setup your sources.list](http://wiki.ros.org/indigo/Installation/UbuntuARM#Setup_your_sources.list)
- [Set up your keys](http://wiki.ros.org/indigo/Installation/UbuntuARM#Set_up_your_keys)
- [Installation](http://wiki.ros.org/indigo/Installation/UbuntuARM#Installation-1)
- [Initialize rosdep](http://wiki.ros.org/indigo/Installation/UbuntuARM#Initialize_rosdep)
- [Environment setup](http://wiki.ros.org/indigo/Installation/UbuntuARM#indigo.2BAC8-Installation.2BAC8-DebEnvironment.Environment_setup)
- [Getting rosinstall](http://wiki.ros.org/indigo/Installation/UbuntuARM#Getting_rosinstall)
- [Verifying OS name](http://wiki.ros.org/indigo/Installation/UbuntuARM#Verifying_OS_name)
- [Build farm status](http://wiki.ros.org/indigo/Installation/UbuntuARM#Build_farm_status)
- [Tutorials](http://wiki.ros.org/indigo/Installation/UbuntuARM#indigo.2BAC8-Installation.2BAC8-PostInstall.Tutorials)- [Obtain source code of the installed packages](http://wiki.ros.org/indigo/Installation/UbuntuARM#indigo.2BAC8-Installation.2BAC8-PostInstall.Obtain_source_code_of_the_installed_packages)
- [Using RVIZ](http://wiki.ros.org/indigo/Installation/UbuntuARM#Using_RVIZ)
## Supported/Tested Platforms
If this install works for your platform, please list it here: 
- 
[Inforce IFC6410](http://tfoote.gitbooks.io/a-guide-to-using-ros-on-the-ifc6410/content/) (Qualcomm Snapdragon 600 Processor)
- 
[eInfochips Eragon600](http://einfochips.viewpage.co/pre-order-now-eragon600-qualcomm) (Qualcomm Snapdragon 600 Processor)
- 
[Nvidia Jetson TK1](http://wiki.ros.org/NvidiaJetsonTK1)
- Odroid C1, X, U2, U3, XU3 (Ubuntu 14.04) and XU4 (Ubuntu 14.04) 
- 
SolidRun CuBox-i Pro with [Ubuntu 14.04 LTS](http://www.solid-run.com/community/topic1269.html) (tested; works) 
- 
[BeagleBone](http://wiki.ros.org/BeagleBone) Black (Ubuntu 14.04)
- 
Parallella [http://www.parallella.org](http://www.parallella.org) (note default shell is not bash) 
- 
RadxaRock [http://radxa.com/](http://radxa.com/) (Ubuntu 14.04) 
- Pandaboard ES (Ubuntu 14.04) 
- 
Gumstix Overo Fire with [Ubuntu Core 14.04 rootfs](https://wiki.ubuntu.com/Core)
- 
Wandboard Quad (with [Robert Nelson's kernel](https://github.com/RobertCNelson/armv7-multiplatform) and Ubuntu 14.04) [http://wandboard.org](http://wandboard.org)
- 
Raspberry Pi 2 (with Ubuntu 14.04 2015-03-02 [https://wiki.ubuntu.com/ARM/RaspberryPi](https://wiki.ubuntu.com/ARM/RaspberryPi)) 
- 
[Google Nexus 5](http://www.google.com/nexus/5/) (with Ubuntu Touch)
- 
DJI Manifold [https://developer.dji.com/manifold/](https://developer.dji.com/manifold/) (Ubuntu 14.04) 
- 
[eInfochips Eragon 410](https://eragon.einfochips.com/products/eic-q410-200.html) (Qualcomm Snapdragon 410 Processor)
- 
[Dragonboard 410c](https://developer.qualcomm.com/hardware/dragonboard-410c) (Qualcomm Snapdragon 410 Processor)
- 
[Orange Pi 2](http://namniart.com/ros/ubuntu/2016/02/06/orangepi-ubuntu-ros.html) and probably other Orange Pi's
## Installation
### Configure your Ubuntu repositories
Configure your Ubuntu repositories to allow "restricted," "universe," and "multiverse." You can
[follow the Ubuntu guide](https://help.ubuntu.com/community/Repositories/Ubuntu) for instructions on doing this. 
### Set your Locale
Boost and some of the ROS tools require that the system locale be set. You can set it with:
- sudo update-locale LANG=C LANGUAGE=C LC_ALL=C LC_MESSAGES=POSIX
If there is a problem. Then try (other languages could be added):
- $ export LANGUAGE=en_US.UTF-8
$ export LANG=en_US.UTF-8
$ export LC_ALL=en_US.UTF-8
$ locale-gen en_US.UTF-8
$ dpkg-reconfigure locales
### Setup your sources.list
Setup your computer to accept software from the ARM mirror on packages.ros.org.
Due to limited resources, there are only active builds for Trusty armhf (14.04), since this is the stable, long-term Ubuntu release and is the most-requested distribution in conjunction with ROS Indigo.
- 
**Ubuntu 14.04 (Trusty armhf)**
- sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu trusty main" > /etc/apt/sources.list.d/ros-latest.list'
### Set up your keys
- sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net --recv-key 0xB01FA116
### Installation
First, make sure your Debian package index is up-to-date: 
- sudo apt-get update
There are many different libraries and tools in ROS - not all compile fully on ARM. You can also install ROS packages individually.
- 
**ROS-Base: (Bare Bones)** ROS package, build, and communication libraries. No GUI tools.
- sudo apt-get install ros-indigo-ros-base
#### Add Individual Packages
- You can install a specific ROS package (replace underscores with dashes of the package name):
- sudo apt-get install ros-indigo-PACKAGE- e.g. 
sudo apt-get install ros-indigo-navigation
To find available packages, use: 
apt-cache search ros-indigo
The Ubuntu ARM package status is available [here](http://repositories.ros.org/status_page/ros_indigo_arm.html)
#### Install Sizes
|**Base Package**|**robot variant**|**desktop variant**|
|----|----|----|
|407 MB|572 MB|~1GB|
### Initialize rosdep
Before you can use ROS, you will need to install and initialize
rosdep. rosdep enables you to easily install system dependencies for source you want to compile and is required to run some core components in ROS.
sudo apt-get install python-rosdep
sudo rosdep init
rosdep update
### Environment setup
It's convenient if the ROS environment variables are automatically added to your bash session every time a new shell is launched:
echo "source /opt/ros/indigo/setup.bash" >> ~/.bashrc
source ~/.bashrc
*If you have more than one ROS distribution installed, ~/.bashrc must only source the setup.bash for the version you are currently using.*
If you just want to change the environment of your current shell, you can type:
source /opt/ros/indigo/setup.bash
If you use zsh instead of bash you need to run the following commands to set up your shell:
echo "source /opt/ros/indigo/setup.zsh" >> ~/.zshrc
source ~/.zshrc
### Getting rosinstall
[rosinstall](http://wiki.ros.org/rosinstall) is a frequently used command-line tool in ROS that is distributed separately. It enables you to easily download many source trees for ROS packages with one command.
To install this tool on Ubuntu, run: 
sudo apt-get install python-rosinstall
### Verifying OS name
Make sure your OS name defined at /etc/lsb-release is as the following. Since ros does not recognize Linaro as an OS, this is necessary. The following is for Ubuntu 14.04, trusty. Modify the release number and name as per your target.
DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=14.04
DISTRIB_CODENAME=trusty
DISTRIB_DESCRIPTION="Ubuntu 14.04"
### Build farm status
The packages that you installed were built by [ROS build farm](http://jenkins.ros.org). You can check the status of individual packages [here](http://repositories.ros.org/status_page/ros_indigo_arm.html). 
## Tutorials
Now, to test your installation, please proceed to the [ROS Tutorials](http://wiki.ros.org/ROS/Tutorials). 
### Obtain source code of the installed packages
If you know the location of the repository of each package, you know you can obtain all the code there. But it's often hard even for experienced developers to reach the correct maintained repository of certain packages. Also, in some situations
 you just want to get the source of the released, installed version of a package. The methods described here the best for these cases.
- 
In earlier days of ROS (supposedly [electric](http://wiki.ros.org/electric) or earlier) you can obtain by the way noted [in this question](http://answers.ros.org/question/12478/apt-get-source-for-packagesrosorg/). 
- 
Now just apt-get source (sudo not needed) as following. You don't even need to explicitly specify
deb-src entry etc. This downloads from the server all the files in the released version of the package (i.e. things not installed in the installation rule (e.g.
CMakeLists.txt) are also included). 
$ apt-get source ros-hydro-laser-pipelineDrawback might be that you have to specifify a single, exact package name (asterisk doesn't
 work). 
### Using RVIZ
It is not recommended to run rviz on most ARM-based CPUs. They're generally too slow, and the version of OpenGL that is provided by the software (mesa) libraries it not new enough to start rviz.
**'IF**' you have a powerful board with a GPU and vendor-supplied OpenGL libraries, it might be possible to run rviz. The IFC6410 and the NVidia Jetson TK1 are two such boards where rviz will run, although neither is fast enough
 for graphics-heavy tasks such as displaying pointclouds. 
Note that rviz will segfault if you have the 
GTK_IM_MODULE environment variable set, so it's best to unset it in your 
~/.bashrc: 
unset GTK_IM_MODULE[CategoryCategory](http://wiki.ros.org/CategoryCategory)
