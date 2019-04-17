# 编译安装高翔的ORBSLAM2_with_pointcloud_map，获取点云地图 - 站在巨人的肩膀上coding - CSDN博客





2018年04月11日 22:26:51[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：2349








前言：众所周知，开源版本的orbslam2没有地图的生成和保存模块。

这里高翔博士提供了一个自己加的一个点云模块在上面，供大家学习。




# **一、下载高翔的修改后的代码**

git clone https://github.com/gaoxiang12/ORBSLAM2_with_pointcloud_map.git

数据集下载位置


https://vision.in.tum.de/data/datasets/rgbd-dataset/download#freiburg1_desk

补充：


orbslam2泡泡注释版：http://git.oschina.net/paopaoslam/ORB-SLAM2

有价值的参考资料：https://www.cnblogs.com/polobymulberry/p/6257616.html

http://www.liuxiao.org/2016/07/ubuntu-orb-slam2-%E5%9C%A8-ros-%E4%B8%8A%E7%BC%96%E8%AF%91%E8%B0%83%E8%AF%95/








# **二、分别编译安装**

将其中的压缩包解压，用压缩包中的ORBSLAM2_with_pointcloud_map代替原来的根目录下的ORBSLAM2_with_pointcloud_map。

## 2.1 编译g2o_with_orbslam2

cd g2o_with_orbslam2

mkdir build 


cd build 


cmake ..

make 


sudo make install

![](https://img-blog.csdn.net/20180411214108582)

## **2.2 编译ORB_SLAM2_modified**

cd  ORB_SLAM2_modified

mkdir build

cd build

make 


这里我出错了，没有用到刚编译的Thirdparty中的g2o，反而用的是ROS下的g2o，先卸载掉ROS下的g2o，在需要的时候装回去。

**卸载ROS下的g2o:**

**sudo apt-get remove ros-indigo-libg2o   **

**注意：这里先卸载掉，然后再在lsd_slam报错的时候再装上，命令为： sudo apt-get install ros-indigo-libg2o**

[ros下的g2o是libg2o:http://wiki.ros.org/libg2o]  


然后继续编译没有出现该问题

![](https://img-blog.csdn.net/20180411214501629)

但是又出现问题了：

 No rule to make target `/opt/ros/indigo/lib/librealsense.so', needed by `../lib/libORB_SLAM2.so'.  Stop.


表明需要[librealsense](http://wiki.ros.org/action/fullsearch/librealsense?action=fullsearch&context=180&value=linkto%3A%22librealsense%22)下载安装这个包。查阅发现ROS下有这个包http://wiki.ros.org/librealsense，安装之：

**sudo apt-get install ros-indigo-librealsense**

![](https://img-blog.csdn.net/20180411215001907)

安装完成后，重新make

make[2]: *** No rule to make target `/usr/lib/libOpenNI2.so', needed by `../lib/libORB_SLAM2.so'.  Stop.



**安装OpenNI**

~~**方法一：**~~



~~OpenNI2安装一些依赖软件：~~

~~$ sudo apt-get install git g++ make python libusb-1.0-0-dev libudev-dev pkg-config~~

~~Clone OpenNI2：~~

~~下载openni：~~

~~https://github.com/occipital/OpenNI2/tree/2.2-beta2~~

~~或用git：~~

~~$ git clone https://github.com/occipital/OpenNI2~~

~~$ git checkout -b 2.2-beta2~~

~~$ cd OpenNI2~~

~~//未完待续~~

~~//参考博客https://blog.csdn.net/billbliss/article/details/56489674~~


方法二、

[openni2_camera](http://wiki.ros.org/action/fullsearch/openni2_camera?action=fullsearch&context=180&value=linkto%3A%22openni2_camera%22)

http://wiki.ros.org/openni2_camera

sudo apt-get inatsll ros-indigo-openni2-*


【一条ROS指令能解决的事情，为什么要那么复杂，哈哈】

再次make（不需要先make clean）

搞定

![](https://img-blog.csdn.net/20180411222340155)

至此，高翔修改过的orbslam2-带点云地图版本就配置完成了。

******************************我是萌萌哒的分割线*********************


//TODO

//用单目或者双目或者rgbd来做做实验，看下效果怎么样，生成的点云地图再转换成为八叉树地图

//运行方法


``
```./Example/RGB-D/rgbd_tum`
```
Vocabulary/ORBvoc.bin      path_to_settings     path_to_sequence    path_to_association
```
``````````


such as

```
./Example/RGB-D/rgbd_tum ../../Vocabulary/ORBvoc.txt TUM2.yaml ../../rgbd-data/ ../../rgbd-data/associations.txt
```


我的运行指令如下：


#        ./Examples/RGB-D/rgbd_tum Vocabulary/ORBvoc.bin Examples/RGB-D/TUM1.yaml ~/all_SLAM_ws/data/rgbd_dataset_freiburg1_xyz Examples/RGB-D/associations/fr1_xyz.txt       运行RGB-D实例时需要RGBD(depth)图像和RGB图像，所以需要把每一张RGB图像与之对应的RGBD图像建立关联（在Examples/RGB-D/associations/目录下有一部分关联文件，可以直接使用），关联python文件[associate.py](https://svncvpr.in.tum.de/cvpr-ros-pkg/trunk/rgbd_benchmark/rgbd_benchmark_tools/src/rgbd_benchmark_tools/associate.py)(根据timestamp进行关联)。

![](https://img-blog.csdn.net/20180412101740937)

但是报了一个错，如上面所示。无法打开ORBvoc.bin文件【这个ORBvoc.bin文件是我从原始的ORB_SLAM2中下载得到的】

是要我生成自己的ORBvoc.bin吗？不应该呀，应该都是一样的。

发现，这里其实加载的不是bin文件，而是txt文件。

![](https://img-blog.csdn.net/20180412103141847)


我去。。。说好的加入对ORBvoc.bin文件的支持呢？

那么命令改为：

#   ./Examples/RGB-D/rgbd_tum Vocabulary/ORBvoc.txt  Examples/RGB-D/TUM1.yaml ~/all_SLAM_ws/data/rgbd_dataset_freiburg1_xyz Examples/RGB-D/associations/fr1_xyz.txt



这里我们可以根据实际相机的标定结果，修改参数

![](https://img-blog.csdn.net/20180412103952188)


正在加载词袋和相机参数哦！等会我们就看到结果了


![](https://img-blog.csdn.net/20180412103657111)


注意：EXAMPLE下的RGB目录中有将RGB图像与深度depth关联的associate.py文件


这里的修改版https://www.cnblogs.com/polobymulberry/p/6257616.html中的泡泡注释是https://gitee.com/paopaoslam/ORB-SLAM2/blob/master是加载ORBvoc.bin文件


# bug:运行一会卡机？？变成灰色了WTF。。。

![](https://img-blog.csdn.net/20180412103827347)





卡住了就ctrl+c吧。。。多运行几次发现也还ok，在回放数据包的时候注意不要操作电脑看就好了，想想有道理。。毕竟我电脑配置渣渣。。




# 三、PCL地图保存

高博士通过加了一个pcl_viewer和点云拼接的线程，来实现实时显示拼接的点云图像。

那我们的点云地图保存在哪里了呢？后面我还要用点云地图来生成octomap三维栅格地图呢！！

//看看源代码中保存路径。

![](https://img-blog.csdn.net/20180412104955623)


在include文件夹中的system.h里，我们来一睹真容。。。。

![](https://img-blog.csdn.net/20180412105902465)


//具体代码怎么改呢？


//orbslam2生成的地图又是什么样子的呢？


看了下吴博注释版本，也没写啊，这是要我自己写的节奏？

![](https://img-blog.csdn.net/20180412110132383)


# //TODO

//编写SaveMap和LoadMap代码（有哪位大佬写好的吗![再见](http://static-blog.csdn.net/xheditor/xheditor_emot/default/bye.gif)）

//那就只能把octomap生成这部分代码写一个线程加进来吗？这个好难

//还是先做做离线的点云地图转换成为octomap三维地图吧，心好累。






******我是萌萌哒的分割线***********2018.4.12***********




# 四、编译ROS下的接口

有问题，不要往下做！！！




找不到pcl相关头文件，搜索发现我是安装了PCL的，其实与ORB代码中的引用路径有关系，查看


![](https://img-blog.csdn.net/20180412021219973)

/home/nvidia/catkin_ws/src/ORBSLAM2_with_pointcloud_map/ORB_SLAM2_modified/Examples/ROS/ORB_SLAM2/../../../include/pointcloudmapping.h:25:35: fatal error: pcl/common/transforms.h: No such file or directory

 #include <pcl/common/transforms.h>


说在pointcloudmapping.h中引用的PCL相关头文件找不到，他这个是在当前目录下找相应头文件，可以把整个PCL相关头文件复制过来


sudo cp -r /usr/include/pcl-1.7/pcl/* /home/nvidia/catkin_ws/src/ORBSLAM2_with_pointcloud_map/ORB_SLAM2_modified/include/pcl

然后make

这个问题解决，但又遇到一个问题

![](https://img-blog.csdn.net/20180412021553129)

No rule to make target `../../../../Thirdparty/g2o/lib/libg2o.so'

缺少libg2o.so库文件，说明g2o未正确编译安装

发现Thiedparty中确实没有相关g2o文件。从高翔的github上复制过来用

![](https://img-blog.csdn.net/20180412022816929)

将ORB_SLAM2_modified中的编译好的libg2o.so拷贝出来。


![](https://img-blog.csdn.net/20180412023215192)

//TOSO

//给我的感觉好像是ROS下的接口本身就没处理好？

//有待解决，还是这个错误




