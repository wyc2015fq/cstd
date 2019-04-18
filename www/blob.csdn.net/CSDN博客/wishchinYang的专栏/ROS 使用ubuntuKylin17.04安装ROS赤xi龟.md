# ROS:使用ubuntuKylin17.04安装ROS赤xi龟 - wishchinYang的专栏 - CSDN博客
2017年05月16日 10:58:38[wishchin](https://me.csdn.net/wishchin)阅读数：2732
使用ubuntuKylin17.04安装
参考了此篇文章：[SLAM: Ubuntu16.04安装ROS-kinetic](http://blog.csdn.net/wishchin/article/details/50635699)
重复官方链接的步骤也没有成功。
此后发现4.10的内核，不能使用Kinetic。
此后安装成功，完全按照以下步骤来即可：
页面链接：[http://wiki.ros.org/lunar/Installation/Ubuntu](http://wiki.ros.org/lunar/Installation/Ubuntu)
## 1. Installation
### Configure your Ubuntu repositories
Configure your Ubuntu repositories to allow "restricted," "universe," and "multiverse." You can[follow the Ubuntu guide](https://help.ubuntu.com/community/Repositories/Ubuntu) for instructions
 on doing this. 
### Setup your sources.list
Setup your computer to accept software from packages.ros.org. 
```cpp
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
```
|[Mirrors](http://wiki.ros.org/ROS/Installation/UbuntuMirrors)|[Source Debs](http://wiki.ros.org/DebianPackageSources) are also available|
### Set up your keys
```cpp
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
```
If you experience issues connecting to the keyserver, you can try substitutinghkp://pgp.mit.edu:80 orhkp://keyserver.ubuntu.com:80 in the previous command.
### Installation
First, make sure your Debian package index is up-to-date: 
```cpp
sudo apt-get update
```
There are many different libraries and tools in ROS. We provided four default configurations to get you started. You can also install ROS packages individually.
In case of problems with the next step, you can use following repositories instead of the ones mentioned above[ros-shadow-fixed](http://wiki.ros.org/ShadowRepository)
- 
**Desktop-Full Install: (Recommended)** : ROS, [rqt](http://wiki.ros.org/rqt), [rviz](http://wiki.ros.org/rviz), robot-generic libraries, 2D/3D simulators, navigation and 2D/3D perception
- 
```cpp
sudo apt-get install ros-lunar-desktop-full
```
or [click here]()
- 
**Desktop Install: **ROS, [rqt](http://wiki.ros.org/rqt), [rviz](http://wiki.ros.org/rviz), and robot-generic libraries
- 
```cpp
sudo apt-get install ros-lunar-desktop
```
or [click here]()
- 
**ROS-Base: (Bare Bones)** ROS package, build, and communication libraries. No GUI tools.
- 
```cpp
sudo apt-get install ros-lunar-ros-base
```
or [click here]()
- 
**Individual Package:** You can also install a specific ROS package (replace underscores with dashes of the package name):
- 
```cpp
sudo apt-get install ros-lunar-PACKAGE
```
e.g.
- 
```cpp
sudo apt-get install ros-lunar-slam-gmapping
```
To find available packages, use: 
```cpp
apt-cache search ros-lunar
```
如果出现了没有安装成功，重新apt-get update ,重新输入一次安装命令，则可以顺利安装完成。
## 2. 测试
### Test your Lunar Ros......................
### Initialize rosdep
Before you can use ROS, you will need to initialize 
rosdep. rosdep enables you to easily install system dependencies for source you want to compile and is required to run some core components in ROS.
```cpp
sudo rosdep init
rosdep update
```
### 测试成功...........................
### Environment setup
It's convenient if the ROS environment variables are automatically added to your bash session every time a new shell is launched:
```cpp
echo "source /opt/ros/lunar/setup.bash" >> ~/.bashrc
source ~/.bashrc
```
*If you have more than one ROS distribution installed, ~/.bashrc must only source thesetup.bash for the version you are currently using.*
If you just want to change the environment of your current shell, instead of the above you can type:
```cpp
source /opt/ros/lunar/setup.bash
```
If you use zsh instead of bash you need to run the following commands to set up your shell:
```cpp
echo "source /opt/ros/lunar/setup.zsh" >> ~/.zshrc
source ~/.zshrc
```
### Getting rosinstall
[rosinstall](http://wiki.ros.org/rosinstall) is a frequently used command-line tool in ROS that is distributed separately. It enables you to easily download many source trees for ROS packages with
 one command.
To install this tool on Ubuntu, run: 
```cpp
sudo apt-get install python-rosinstall
```
### Build farm status
The packages that you installed were built by the[ROS build farm](http://build.ros.org). You can check the status of
 individual packages[here](http://repositories.ros.org/status_page/ros_lunar_default.html).
