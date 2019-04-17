# TODO：完成ORBSLAM2的坑--添加地图保存和加载模块SaveMap/LoadMap或者是实时点云图到八叉树地图转换 - 站在巨人的肩膀上coding - CSDN博客





2018年04月12日 11:53:10[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：2716








//TODO

//先开个博客，提醒自己。有时间来弄的时候整理，来写代码

**贴几篇有用的参考博客：**

[1]泡泡机器人注释过的代码https://gitee.com/paopaoslam/ORB-SLAM2/blob/master/include/System.h


[2]针对TUM数据集增加地图保存功能：https://www.cnblogs.com/mafuqiang/p/6972342.html


[3]针对TUM数据集增加地图加载功能：https://www.cnblogs.com/mafuqiang/p/7002568.html

[4]地图相关知识学习：https://blog.csdn.net/u010128736/article/details/53218140

[4]现有的地图保存部分代码及使用说明书（不保证有效）：

   (1) 代码：http://recherche.enac.fr/~drouin/slam/orbslam2/poine_orbslam2_04_07_16.tgz


    (2)说明：https://blog.csdn.net/yaoyao123321000/article/details/79014040

[5]学SLAM必看的网站：www.slamcn.org/index.php/%E9%A6%96%E9%A1%B5，深度了解SLAM

[6]一锅粥SLAM：https://github.com/gaoxiang12/ORB-YGZ-SLAM

[7]ORBSLAM2：https://github.com/raulmur/ORB_SLAM2


[8]ORBSLAM在SLAM中的地位：https://www.zhihu.com/question/35116055

     (1)数据集下载：https://vision.in.tum.de/data/datasets/rgbd-dataset/download

     (2)EUroc数据集下载：https://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets

[9]SLAM-AR:https://www.cnblogs.com/polobymulberry/p/6257616.html

[10]高翔的带有PCL点云显示的ORBSLAM2：https://www.cnblogs.com/gaoxiang12/p/5161223.html




********************我是萌萌哒的分割线************************************************

转念一想，其实离线转换没用呀，动态障碍物需要动态避障。必须实时进行三维PCL点云到octoMap的转换。那么，原始ORBSLAM2的map保存下来是什么点云？和PCL点云一样吗？

- 如果一样，那么就直接基于原始的ORBSLAM2代码修改，加入octoMap转换功能代码（按照高翔的octomap_tutor中的方法，实时读取点云实时转换），并且加入octovis线程显示octoMap

- 如果不一样，那么就要先实时生成PCL点云（基于高翔的带有PCL生成的ORBSLAM2代码修改），然后，实时将PCL点云转换成octoMap（按照高翔的octomap_tutor中的方法，实时读取点云实时转换），并且加入octovis线程显示octoMap，然后就可以用这个三维地图模型来导航。

有两个问题需要搞清楚下：


（1）mappoint是提取出的ORB特征点吗？

（2）PCL点云里面的点是特征点吗？如果不是，和MapPoint有什么区别？




[4]现有的地图保存部分代码及使用说明书（不保证有效）：

   (1) 代码：**http://recherche.enac.fr/~drouin/slam/orbslam2/poine_orbslam2_04_07_16.tgz**


    (2)说明：https://blog.csdn.net/yaoyao123321000/article/details/79014040

这边还提供了很多小工具，可以看看相关代码可以怎么用起来。

![](https://img-blog.csdn.net/20180412211618632)


但运行到最后卡住了，没有保存到地图，但是地图保存的代码是有的。保存在哪里了呢？？


//TODO

//电脑内存貌似不够，有空来添置一个内存条重新实验试试看。

![](https://img-blog.csdn.net/20180412212050862)







*********************************************我是萌萌哒的分割线***********************************




**//编程实现**

我的工作




