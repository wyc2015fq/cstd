# SLAM: Ubuntu14.04_Kylin安装ROS-Indigo - wishchinYang的专栏 - CSDN博客
2015年12月17日 10:41:37[wishchin](https://me.csdn.net/wishchin)阅读数：1454
参考连接：[ROS－Indigo版在Ubuntu上的安装](http://my.oschina.net/u/2306127/blog/410286)
**第一步： 软件源配置**
1、 增加下载源（增加ubuntu版的ros数据仓库，即下载源）（通用指令适合任何版本的ros）
`sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'`
2、 设置key，实现安全从数据仓库下载未被修改版，故设置key增加可信度
`sudo apt-key adv --keyserver hkp://pool.sks-keyservers.net --recv-key 0xB01FA116`也有用下面这条指令的
` wget http://packages.ros.org/ros.key -O - | sudo apt-key add -`以上两步为安全配置下载源
可以增加本地本国的源，例如再sourcelist里增加163和sohu，这样没必要从外网下载，国内下载速度很快
3 、更新源
`sudo apt-get update`～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～
**第二步： 安装合适版本的ROS**
1、完全安装ros（2d、3d、rviz等）
`sudo apt-get install ros-indigo-desktop-full   # ros和desktop间的indigo（靛蓝）为 ros版本号`#如果用户计算机的空间有限可以去掉-full
2、 安装rosdep
```
sudo rosdep init
rosdep update
```
3 、user的terminal命令行环境配置，即找到可执行命令。
`source /opt/ros/indigo/setup.bash #这句是关键，但只影响当前打开的terminal`#零时性的，适合于多版本ros共存时，根据需求切换不同版本的ros的命令
下面两条将终身影响，即每次打开terminal都能找到ros的相关命令。
```
echo "source /opt/ros/indigo/setup.bash" >> ~/.bashrc #修改
source ~/.bashrc #使能
或者：
source /opt/ros/indigo/setup.bash
切换到主目录下。。。。
source  .bashrc #使能
```
～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～
4 、辅助工具rosinstall安装
`sudo apt-get install python-rosinstall`
后记：
        wiki后面的两个步骤主要是来share以及test。最后的stpe对于beginner来说还是很好的，你可以熟悉这个刚刚开始爆发的ROS[操作系统](http://www.07net01.com/tags-%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F-0.html)。
