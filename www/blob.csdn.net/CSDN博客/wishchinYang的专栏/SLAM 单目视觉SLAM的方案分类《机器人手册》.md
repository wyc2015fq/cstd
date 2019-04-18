# SLAM: 单目视觉SLAM的方案分类《机器人手册》 - wishchinYang的专栏 - CSDN博客
2015年10月26日 15:20:40[wishchin](https://me.csdn.net/wishchin)阅读数：5163
摘抄知乎上一段有趣的话：
如果你出门问别人『学习SLAM需要哪些基础？』之类的问题，一定会有很热心的大哥大姐过来摸摸你的头，肩或者腰（不重要）![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)，一脸神秘地从怀里拿出一本比馒头还厚的《Multiple View Geometry》或《State Estimation For Robotics》或 《An Invitation to 3D Computer Vision》塞给你。然后告诉你，只要潜心修炼两年，一定会……
    扯淡！SLAM是一个工程，不是一门课或者一个算法.........
    单目视觉SLAM方法在是否引入概率框架方面分为基于概率框架的方法和基于非概率框架的方法。其中前者的代表方法是基于Kalman滤波（KF）的方法，基于粒子滤波（PF）的方法等；后者的代表方法是基于关键帧和稀疏捆绑调整优化的方法，基于图优化的方法等。
    在《机器人手册》 第2卷，这本书里面，第23张讲述了：三维视觉和基于视觉的实时定位于地图重建。在第37章里面，讲述了 同时定位与建图。指出了SLAM的三种基本方法。
![](https://img-blog.csdn.net/20170823154605410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       一种是EKF的方法，但由于性能的约束，逐渐变得非主流；第二种基于图表示，成功使用稀疏非线性优化方法解决SLAM问题，成为主要方法；第三种也是成为粒子滤波器的非参数统计滤波技术，是在线SLAM的一种主流方法。
## 文章链接：[http://www.opencv.org.cn/forum.php?mod=viewthread&tid=37672](http://www.opencv.org.cn/forum.php?mod=viewthread&tid=37672)
**一、概率方法**
      采用概率框架的方法的思路大致为：假设从初始时刻到当前 时刻的观测数据和控制输入已知，那么就可以联合后验概率来构建一个后验概率密度函数，这个函数用来描述相机姿态 和 地图中的特征的空间位置，进而通过递归的贝叶斯滤波算法 来对构建的概率密度函数进行估计，从而完成SLAM问题的解决。
      在基于KF的方法中，假设满足高斯分布并且系统的非线性较小时可以得到不错的效果，另外从后验概率的不同可以分为EKF-SLAM方法、扩展信息滤波（EIF）方法、UKF-SLAM方法、SEIF方法等，例如，Davison等提出的单目视觉SLAM系统（MonoSLAM）和Roussillon[提出的实时单目视觉SLAM系统（RTSLAM）就是建立在扩展卡尔曼滤波（EKF）的基础上的。
     再基于PF的方法中，主要有Doucet[7]提出的基于Rao-Blackwellized粒子滤波器（RBPF）的方法 和在此基础上Montemerlo[8]提出的 FastSLAM方法等，其中FastSLAM方法可以用于非线性的系统。
      优点： 基于概率框架的方法的计算框架类似，其中基于EKF的单目视觉，一般可以分为运动模型和观测模型并在一阶HMM 的假设下可以实现系统状态的预测和观测更新。地图也可以在线更新。
      缺点：但是这种方法多采用增量式的地图创建方法，复杂度是n^2的。另外故在系统不稳定和观测的不确定性的影响下造成误差的累积，在大规模场景时会导致最终地图的不一致性，难于保持准确度和鲁棒性。
**二、非概率方法**
   在非概率框架的方法中，Klein等在基于关键帧的基础上提出的PTAM系统（Parallel Tracking andMapping），该系统会在后文给出详细的分析和优化措施。基于图优化的SLAM方法[9]在采取了全局优化的基础上，在大规模场景下可以取得好的效果。在国外，基于图优化的方法大都被划分为前端和后端的问题，
