# Ubuntu14.04安装ROS-indigo - sinat_31425585的博客 - CSDN博客
2016年10月17日 14:40:11[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：401

记忆力差的孩纸得勤做笔记！
下面是我参考[ROS官网](http://wiki.ros.org/indigo/Installation/Ubuntu)，将自己在ubuntu14.04上安装Indigo版本ROS过程，做的笔记！
1. 设置你的电脑，接收来自packages.ros.org的软件包：
```cpp
$ sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
```
2. 设置你的keys：
```cpp
$ sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net --recv-key 0xB01FA116
```
如果出现**gpg: keyserver timed out**的错误，可以尝试如下命令（增加：80）：
```cpp
$ sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 0xB01FA116
```
3. 更新一下package源：
```cpp
$ sudo apt-get update
```
4. 安装ROS，这里我安装的是ROS-Indigo完整版，有特殊需求的可以去参考官网：
```cpp
$ sudo apt-get install ros-indigo-desktop-full
```
可以用以下命令来查找ROS包：
```cpp
$ apt-cache search ros-indigo
```
5. 初始化ROS的依赖关系：
```cpp
$ sudo rosdep init
```
6. 更新依赖关系：
```cpp
$ rosdep update
```
7. 环境设置：
```cpp
$ echo "source /opt/ros/indigo/setup.bash" >> ~/.bashrc
```
同时，让当前环境设置生效：
```cpp
$ source ~/.bashrc
```
8. 安装rosinstall工具，方便以后安装很多ROS包资源：
```cpp
$ sudo apt-get install python-rosinstall
```
