# ROS: Ubuntu16.04安装ROS-kinetic - wishchinYang的专栏 - CSDN博客
2016年02月04日 18:05:53[wishchin](https://me.csdn.net/wishchin)阅读数：3764
参考连接：[](http://my.oschina.net/u/2306127/blog/410286)[SLAM: Ubuntu14.04_Kylin安装ROS-Indigo](http://blog.csdn.net/wishchin/article/details/50338251)
**第一步： 软件源配置**
1、 增加下载源（增加ubuntu版的ros数据仓库，即下载源）（通用指令适合任何版本的ros）
```cpp
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
```
2、 设置key，实现安全从数据仓库下载未被修改版，故设置key增加可信度
```cpp
sudo apt-key adv --keyserver hkp://pool.sks-keyservers.net --recv-key 0xB01FA116
```
也有用下面这条指令的
```python
wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
```
以上两步为安全配置下载源
可以增加本地本国的源，例如再sourcelist里增加163和sohu，这样没必要从外网下载，国内下载速度很快
3 、更新源
```python
sudo apt-get update
```
～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～
**第二步： 安装合适版本的ROS**
1、完全安装ros（2d、3d、rviz等）
```python
sudo apt-get install ros-kinetic-desktop-full   # ros和desktop间的indigo（靛蓝）为 ros版本号
```
#如果用户计算机的空间有限可以去掉-full
2、 安装rosdep
```cpp
sudo rosdep init 
rosdep update
```
3 、user的terminal命令行环境配置，即找到可执行命令。
source /opt/ros/indigo/setup.bash #这句是关键，但只影响当前打开的terminal  
#零时性的，适合于多版本ros共存时，根据需求切换不同版本的ros的命令
下面两条将终身影响，即每次打开terminal都能找到ros的相关命令。
```python
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc #修改  
source ~/.bashrc #使能  
    或者：  
source /opt/ros/kinetic/setup.bash  
切换到主目录下。。。。  
source  .bashrc #使能
```
注意事项：
        提前 写入了indigo的path，必须删除掉！
sudo gedit ~/.bashrc
找到条目，删除保存
～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～
4 、辅助工具rosinstall安装
```python
sudo apt-get install python-rosinstall
```
安装完成
