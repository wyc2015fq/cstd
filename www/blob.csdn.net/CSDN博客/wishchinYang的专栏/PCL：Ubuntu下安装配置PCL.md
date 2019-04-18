# PCL：Ubuntu下安装配置PCL - wishchinYang的专栏 - CSDN博客
2014年10月03日 10:47:31[wishchin](https://me.csdn.net/wishchin)阅读数：15023
一：安装PCL
      依据官网介绍：[http://www.pointclouds.org/downloads/linux.html](http://www.pointclouds.org/downloads/linux.html)
##    Ubuntu
     We currently support all **Ubuntu** via [PPA](https://launchpad.net/~v-launchpad-jochen-sprickerhof-de/+archive/pcl). The installation instructions are:
```python
sudo add-apt-repository ppa:v-launchpad-jochen-sprickerhof-de/pcl
   sudo apt-get update
   sudo apt-get install libpcl-all
```
     Luck！ 版本是1.7.1，省得再进行编译了..........
二：配置PCL
     在Eclipse中添加包含：
     include: 
```cpp
/usr/include/pcl-1.7.1/
```
     lib:
```cpp
/usr/lib/pcl-1.7.1/
```
     dll:
```cpp
/usr/bin/pcl-1.7.1/
```
三：测试程序
      显示pclvisualization窗口，出现不能初始化的指针，这真是个令人 淡疼的问题...
