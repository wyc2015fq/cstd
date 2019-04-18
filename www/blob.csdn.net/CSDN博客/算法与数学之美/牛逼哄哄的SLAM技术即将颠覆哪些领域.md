# 牛逼哄哄的SLAM技术即将颠覆哪些领域 - 算法与数学之美 - CSDN博客
2017年11月15日 00:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：272
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyRiaABxOUKgRc1THdVAEo7JKQSq6aBMpBeyA8ibjTib1wIGJQJ97IHRHAhF3XtJmFicFRS1IiaxpHTUpQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)SLAM的英文全程是
 Simultaneous Localization and Mapping，中文称作「同时定位与地图创建」。SLAM试图解决这样的问题：一个机器人在未知的环境中运动，如何通过对环境的观测确定自身的运动轨迹，同时构建出环境的地图。SLAM技术正是为了实现这个目标涉及到的诸多技术的总和。
![0?wx_fmt=gif&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_gif/GsVBs6tAVTopABiaWpIMSc3CPCXqYc2GqQdfgMn50o5VWUpmIYvXBbEW4aqjnZNHOlqvicicj9FYicOc41JXbowMqQ/0?wx_fmt=gif&wxfrom=5&wx_lazy=1)
SLAM技术距今已有 30 余年的发展历史，但相比于深度学习、大数据等词汇，听过的人少之又少，国内从事相关研究的机构更是屈指可数。直至最近三年，SLAM才逐渐成为国内机器人和计算机视觉领域的热门研究方向，在当前比较热门的一些创业方向中崭露头角：
> 
**VR/AR 方面**，根据
 SLAM 得到地图和当前视角对叠加虚拟物体做相应渲染，这样做可以使得叠加的虚拟物体看起来比较真实，没有违和感；
**无人机领域**，SLAM可以构建局部地图，辅助无人机进行自主避障、规划路径；
**无人驾驶领域**，
 SLAM 技术可以提供视觉里程计功能，然后跟其他的定位方式融合；
**机器人定位导航领域**，SLAM
 可以用于生成环境的地图。基于这个地图，机器人执行路径规划、自主探索、导航等任务。
提起SLAM的重要性，有人说，SLAM是无人驾驶技术的灵魂；也有人曾打比方说，手机离开了WIFI和数据网络，就像无人车和机器人离开了SLAM一样。之所以没有受到广泛关注，主要原因在于这个方向难度很大，入门的门槛太高，具体来讲有以下几个原因：
> 
1. 入门资料很少：虽然国内也有部分人在做，但这方面的中文资料几乎没有，入门教程匮乏，直至2017年4月《视觉SLAM十四讲：从理论到实践》的问世。
2. 难以实现：SLAM是一个完整的系统，由许多个分支模块组成。现在经典的方案是“图像前端，优化后端，闭环检测”的三部曲，很多文献看完了自己实现不出来。
3. 动手编程需要大量的先决知识：首先，你要会C和C++，网上很多代码还用了11标准的C++；第二，要会用Linux；第三，要会cmake，vim/emacs及一些编程工具；第四要会用openCV, PCL, Eigen等第三方库。只有学会了这些东西之后，你才能真正上手编一个SLAM系统。如果你要跑实际机器人，还要会ROS。
上述原因直接或者间接导致，国内SLAM技术人才稀缺，同时，机器人、无人机、无人驾驶、虚拟现实等企业的需求旺盛，供不应求的局面逐渐铺开。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/GsVBs6tAVTopABiaWpIMSc3CPCXqYc2GqQxvQ1onoOb71ibgFuYRkCKlNsQZDPyKTna7vMRCKdgib3RVjOdOr8G2A/640?wx_fmt=png)
困难多意味着收获也多，坎坷的道路才能锻炼人。鉴于SLAM技术入门困难，专注于前沿科技在线教育的深蓝学院，联合《视觉SLAM十四讲：从理论到实践》书籍作者高翔博士，推出《**SLAM从理论到实践**》在线系列课程，课程优秀学员可获得证书，并推荐到Momenta、地平线、图森未来、格林深瞳等知名企业实习就业。
**课程讲师**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/GsVBs6tAVTopABiaWpIMSc3CPCXqYc2GqVzFPhz19COoZPmicLaicpkBr5TXRwQDt2BRkPaC6dIf3icxxfkMMEg0Cw/640?wx_fmt=png)
**高翔**，慕尼黑工业大学博士后，清华大学自动化系博士，主编畅销书《视觉SLAM十四讲：从理论到实践》。长期从事SLAM（即时定位与地图构建）的研究，主要包括机器人的中的视觉SLAM技术、机器学习与SLAM的结合，在国际知名期刊
 IEEE Transactions on Mechatronics、Robotics and Autonomous Systems、Autonomous Robots等发表论文数篇。
附：《视觉SLAM十四讲：从理论到实践》是国内第一本也是唯一一本专注于SLAM的书籍，现已成为国内入门SLAM的必备资料。
**课程特色**
**1. **成熟的课程体系设置，从必备数学基础知识讲起；
**2. **理论结合实践，降低入门门槛；
**3. **课上在线答疑，课下微信群答疑；
**4. **作业设置新颖，**批阅**讲解仔细；
**5. **优秀学员颁发证书，推荐名企**实习就业**；
**6. **课程**PPT和源程序**，会提前公开给学员。
**课程目录**
**1. ****概述与预备知识（2学时）**
    1.1  课程内容提要与预备知识
    1.2  SLAM是什么
    1.3  视觉SLAM数学表述与框架
    1.4  Linux下的C++编程基础
    1.5  实践：Hello SLAM
**2. ****三维空间的刚体运动（2学时）**
    2.1 点与坐标系
    2.2 旋转矩阵
    2.3 旋转向量与欧拉角
    2.4 四元数
    2.5 相似、仿射和射影变换
    2.6 实践：Eigen矩阵运算
    2.7 实践：Eigen几何模块
**3. ****李群与李代数（2学时）**
    3.1 群
    3.2 李群与李代数
    3.3 指数与对数映射
    3.4 李代数求导与扰动模型
    3.5 实践：Sophus李代数运算
**4. ****相机模型与非线性优化（2学时）**
    4.1 针孔相机模型与畸变
    4.2 图像的组成
    4.3 从状态估计到最小二乘
    4.4 非线性优化与最小二乘法
    4.5 实践：Ceres曲线拟合
    4.6 实践：g2o曲线拟合
**5. ****特征点法视觉里程计（2学时）**
    5.1 特征点的提取与匹配
    5.2 对极几何
    5.3 三角测量
    5.4 3D-2D：PnP
    5.5 3D-3D：ICP
    5.6 实践：ORB特征点
    5.7 实践：PnP
    5.8 实践：ICP
**6. ****直接法视觉里程计（2学时）**
    6.1 直接法的引出
    6.2 光流
    6.3 直接法
    6.4 实践：LK光流
    6.5 实践：RGB-D直接法
**7. ****后端优化（2学时）**
    7.1 滤波器
    7.2 Bundle Adjustment与图优化
    7.3 Pose Graph
    7.4 Factor Graph
    7.5 实践：Bundle Adjustment
    7.6 实践：Pose Graph
**8. ****回环检测（2学时）**
    8.1 概述
    8.2 词袋模型
    8.3 实践：建立字典以计算图像间相似性
    8.4 课程小结
**报名**
现在可领取** 100元优惠券 **，12月6日-12月30日每周三、周六晚
 19点到 21 点授课。
请添加工作人员「**深蓝学院**」**助教**报名
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/GsVBs6tAVTpByaibRiaYkU6bfOkiaGE0I7wW7zdVMuuThLfkhK29txiaN8EicKiaZcnTnZrpEswsy5S3O3kxxDwokLNw/640?wx_fmt=jpeg)
