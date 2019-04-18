# SLAM: Orb_SLAM的使用小综述 - wishchinYang的专栏 - CSDN博客
2015年12月03日 15:59:58[wishchin](https://me.csdn.net/wishchin)阅读数：7971
        Orb_SLAM被业内人士称为第一个现代SLAM方法，不仅是因为其工程化良好，简单易用。OrbSLAM更是对PTAM的改进，PTAM分离了追踪和地图构建过程。OrbSLAM走的更远，把闭环检测和世界地图的优化从局部地图优化中独立出来，另辟一个线程，使在线SLAM过程应用到了更大的场景中。
        作为传统在线SLAM算法的滤波器算法，使用EM同时完成追踪和地图构建；PTAM引入BA，把地图构建从追踪中独立出来，增大在线SLAM的使用场景；OrbSLAM工程方法，把闭环检测和世界地图优化独立出来，使在线SLAM应用到更大的场景，并可以应用到室外。
        OrbSLAM应用广泛的一个前提是其开源工程做的不错，方法论和设计模式有可圈可点之处。引发全员SLAM。
0.ORB_SLAM的官方网站：[http://webdiis.unizar.es/~raulmur/orbslam/](http://webdiis.unizar.es/~raulmur/orbslam/)
1. 参考知乎上对**orb-slam **的评价：[orb-slam在众多SLAM方法中处于怎样的地位](http://www.zhihu.com/question/35116055)
2. 评价说是 PTAM 的改进版：[PTAM主页](http://www.robots.ox.ac.uk/~gk/PTAM/)
3. Orb_SLAM的 GitHub 页面：[此页面有详细的使用方法](https://github.com/raulmur/ORB_SLAM)
4. 使用Orb_SLAM的一些实用参考工程：[视觉SLAM实战（一）：RGB-D SLAM V2、Orb_SLAM](http://www.cnblogs.com/gaoxiang12/p/4462518.html)
     "RGBDSLAMv2 is based on the ROS project, OpenCV, PCL, OctoMap, SiftGPU and more – thanks!" 你还在等什么？装装装！好在这些东西在Ubuntu下安装，就是几句话搞定的事。
     ROS hydro安装指南： [http://wiki.ros.org/cn/hydro/Installation/Ubuntu](http://wiki.ros.org/cn/hydro/Installation/Ubuntu) （加ppa源后直接安装）
     Linux OpenCV安装指南：[http://blog.sciencenet.cn/blog-571755-694742.html](http://blog.sciencenet.cn/blog-571755-694742.html) （从源代码编译）
     PCL：[http://www.pointclouds.org/downloads/linux.html](http://www.pointclouds.org/downloads/linux.html) （加ppa后安装）
5. Orb_SLAM的使用方法中文介绍：[ORB-SLAM(1) --- 让程序飞起来](http://blog.csdn.net/dourenyin/article/details/48055441)
       本文给出了一些注意事项和排错方法，值得没有时间读详细文档的使用者参考一下。
6.  SLAM:ORB_SLAM源代码中的设计模式分析
        此文分析了ORB_SLAM的设计模式的由来。原文链接：[SLAM代码（设计模式）](http://blog.csdn.net/wendox/article/details/53454768)
        若要读取代码之前先观其思路，看看这个系列的不错：[http://blog.csdn.net/wendox/article/month/2016/12](http://blog.csdn.net/wendox/article/month/2016/12)
一部分摘抄
        引入一个**Frame**结构，为了实现上述的跟踪器，第一步是读取图像，通过图像的数据估计当前帧对应的位姿状态。场景中相同的点在不同的视角中构成一个基础矩阵的关系，一个图像帧的图像平面和另一帧的图像的平面之间构成单应矩阵的关系。使用最小二乘法通过图像点可以计算得到单应矩阵和基础矩阵，，使用矩阵奇异值分解（SVD）可将单应矩阵或者基础矩阵分解恢复出位姿变化。这些内容的实现可以放到**Frame**结构，也可以像ORB那样专门放到一个**Initializer**里边。**Frame**需要和**Map**完成互动。除了需要计算当前帧的位姿，还要完成特征点的三维重建。有的方法使用了一个深度滤波器，如SVO，LSD中考虑了极线与深度的夹角，这样会得到一个半稠密的地图。因为稠密的地图点想要试试完成一般是需要使用GPU来完成的。图像的上的特征点和空间中的三维点如何对应呢？常用的方法是PnP。PnP是已知三维点计算相机位姿，三维重建是已知相机位姿计算三维点。
        PnP的计算中需要知道相机的内参数，这里引入第二个结构相机模型**Camera**，他保存并管理相机的参数，进行一些图像空间到世界空间的转换，完成图像点（关键特征点）的去畸变。
        如何解决累计误差问题，常用的方法是使用路标点和当前图像点进行匹配估计相机的运动，这时候需要存储这些路标点的描述子。另外一方面在LSD-SLAM, SVO等方法中，是怎样存储路标点的呢？这里引入一个结构叫做**Mappoint**。
        如何评价一个路标点的好坏程度？如何如何保存病管理这些路标点呢？路标点的添加删除等管理工作可以放入另外一个结构**Map**中进行。地图中以何种形式保存这些路标呢，一种方法是路标放在图像帧里作为关键帧**KeyFrame**进行保存和提取。关键帧串起来成为一个地图。这样做的好处就是我们使用了关键帧继承了帧的性质，便于以后特征点匹配。在前边累计相机运动的时候参考帧是前一帧图像，这里的参考帧是关键帧的图像。关键帧是相机运动过程中保留了重要信息的最小集合中的元素。理论上关键帧是没有重叠的。后端优化的对象主要是关键帧。
不足之处：
      ORB-SLAM工程在设计数据关联时关系链接极为复杂，应该有一种能降低工程模块耦合的方法。
