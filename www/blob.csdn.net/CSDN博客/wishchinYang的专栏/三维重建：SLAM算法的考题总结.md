# 三维重建：SLAM算法的考题总结 - wishchinYang的专栏 - CSDN博客
2016年05月16日 17:26:35[wishchin](https://me.csdn.net/wishchin)阅读数：5817
       参考英文维基：[https://en.wikipedia.org/wiki/Slam](https://en.wikipedia.org/wiki/Slam) 。参考文档：[视觉slam研究分析的一点认识](http://www.opencv.org.cn/forum.php?mod=viewthread&tid=37672)
**一、四大简述**
1、 请简单描述您对机器人的SLAM的概念理解？
答： 机器人需要在自身位置不确定的条件下，在完全未知环境中创建地图, 同时利用地图进行自主定位和导航。这就是移动机器人的同时定位与地图创建(**simultaneous localization and mapping** (**SLAM**)  ) 问题。
      算法上一般分为 相机定位跟踪 和 场景地图构建 两个高度相关的部分。场景地图构建是指 构建相机所在场景的三维地图；相机定位跟踪是指 利用相机自身姿态的估计值和通过传感器得到的观测值来确定相机在环境中的位置。
       参考：[SLAM：使用G2O-ORB-SLAM](http://blog.csdn.net/wishchin/article/details/50460821)
2、 请描述常见的SLAM算法及原理？
答：常见的SLAM方法有单目SLAM方法和双目SLAM方法**.**
      单目SLAM方法用帧与帧之间的运动距离和运动角度来估算深度信息，传统的视觉SLAM方法多采用特征提取 匹配和数据关联的方式来完成。
        根据单目SLAM方法在是否引入概率框架方面 划分为 基于概率框架的方法和基于非概率框架的方法。基于概率框架的方法把SLAM问题描述为滤波问题/**最大似然方法**，代表方法有基于kalman滤波的方法等；  非概率框架的方法 主要方法为使用关键帧和稀疏绑定调整优化（BA），使用的数值优化-**最小二乘方法** 直观描述形式为图优化。此外，基于粒子滤波的方法为随机优化方法，寻找系统**状态的最小方差**估计。
        参考：[SLAM：使用G2O-ORB-SLAM](http://blog.csdn.net/wishchin/article/details/50460821)
3、 SLAM的常用概率解法有哪几类？分别有什么优缺点？
答：采用概率框架的方法的思路大致为：假设从初始时刻到当前 时刻的观测数据和控制输入已知，那么就可以联合后验概率来构建一个后验概率密度函数，这个函数用来描述相机姿态 和 地图中的特征的空间位置，进而通过递归的贝叶斯滤波算法 来对构建的概率密度函数进行估计，从而完成SLAM问题的解决。
        在基于KF的方法中，假设满足高斯分布并且系统的非线性较小时可以得到不错的效果，另外从后验概率的不同可以分为EKF-SLAM方法、扩展信息滤波（EIF）方法、UKF-SLAM方法、SEIF方法等，例如，Davison等提出的单目视觉SLAM系统（MonoSLAM）和Roussillon[提出的实时单目视觉SLAM系统（RTSLAM）就是建立在扩展卡尔曼滤波（EKF）的基础上的。
        再基于PF的方法中，主要有Doucet[7]提出的基于Rao-Blackwellized粒子滤波器（RBPF）的方法 和在此基础上Montemerlo[8]提出的 FastSLAM方法等，其中**FastSLAM**方法可以用于**非线性的系统**。
        优点： 基于概率框架的方法的计算框架类似，其中基于EKF的单目视觉，一般可以分为运动模型和观测模型并在**HMM** 的假设下可以实现系统状态的预测和观测更新。地图也可以在线更新。
        缺点：但是这种方法多采用增量式的地图创建方法，故在系统不稳定和观测的不确定性的影响下造成误差的累积，在大规模场景时会导致最终地图的不一致性，难于保持准确度和鲁棒性。
4、 SLAM中回环检测（闭环检测）的目的是什么？简述一下SLAM中可以使用的回环检测方法？
答：参考维基：Loop closure ：Loop closure is the problem of recognizing a previously-visited location and updates the beliefs accordingly. This can be a problem because model or algorithm errors can assign low priors to the location. Typical loop closure methods apply a second algorithm to measure some type of sensor similarity, and re-set the location priors when a match is detected. For example, this can be done by storing and comparing[bag of words](https://en.wikipedia.org/wiki/Bag_of_words) vectors of [SIFT](https://en.wikipedia.org/wiki/Scale-invariant_feature_transform) features from each previously visited location.
        回环检测/闭环检测 是一个 场景识别的问题，若slam完成闭环检测，则可以大大得减少检测误差。因此，SLAM中的闭环检测则是为了减少SLAM中的累积误差。
   参考文章：[****SLAM系统**的**研究点介绍****](http://blog.csdn.net/wishchin/article/details/49424467)
        闭环检测一般使用词袋模型作为场景匹配方法。而作为一个场景识别问题，闭环检测可以看做是一个视觉系统的模式识别问题。所以可以使用各种机器学习的方法来做，比如什么决策树/SVM，也可以试试CNN方法。不过实际当中要求实时检测，没有那么多时间训练分类器。所以SLAM更侧重在线的学习方法。
       在线的视觉机器学习方法有在线SVM和在线随机森林 等工具。
5、 SLAM算法中的机器人“绑架”问题是什么？可以用哪些方法解决这样的问题？
答：机器人的“绑架”问题是指在缺少它之前的位置信息情况下，去确定机器人的当前位姿，即**重定位**。例如当机器人被安置在一个已经构建好地图的环境中，但是并不知道它在地图中的相对位置，或者在移动过程中，由于传感器的暂时性功能故障或相机的快速移动，都导致机器人先前的位置信息的丢失，就像人质的眼睛被蒙上黑布条，拉上集装箱被运送到了未知的地方，此时，人质就无法给自己定位了。
  解决方案： 暂时没有详细考虑结果！！！
  方案一：使用替代传感器，比如在 视觉机器人中 使用 惯导系统。
      参考知乎文章：[http://www.zhihu.com/question/2945372](http://www.zhihu.com/question/2945372)7
**二、关于VO**
1.什么是极线约束？
   A：也叫核面约束。相机经过位姿转换，通过对同一个物体和同一组点观察得到两幅图像，同一组点在两幅图片中位置和顺序不同，但满足相机透视的 核面约束。又称基本矩阵约束。
      公式： p^T *F*p=0;
      同一个点P在两幅图像上的映射，已知映射点p1，那么另一帧映射点p2一定在相对于p1的极线上，这样可以减少待匹配的点数量。
2.相机的E矩阵和F矩阵的区别？
A：基本矩阵F和E只差了一个相机内参  
![](https://img-blog.csdnimg.cn/20190414163731889.png)  F = K^(-T) *E *K^(-1) ；
3.什么是BA？
A：Bundle Adjustment，光束法平差。
    有**两层释义**：1.对场景中**任意三维点P**，由从每个视图所对应的的摄像机的光心发射出来并经过图像中P对应的像素后的光线，都将交于P这一点，对于所有三维点，则形成相当多的光束（bundle）；2. 实际过程中由于噪声等存在，每条光线几乎不可能汇聚与一点，因此在求解过程中，需要不断对待求信息进行调整（adjustment），来使得最终光线能交于点P。调整过程即为光束法平差。
4.实现相机Agent定位的PNP方法？
A:利用二维视野内的图像，求出三维图像在场景中的位姿，这是一个三维透视投影的反向求解问题。常用方法是PNP方法，需要已知三维点集的原始模型。
    应用场景：给定物体3D点集与对应的图像2D点集，之后进行姿态计算(即求旋转与位移矩阵)。
    PNP解法：直接线性变换，求最小二乘，求解t具有12维，至少需要6个匹配点。P3P方法：使用世界坐标系中的3个点，把问题转化为了一个3D-3D的刚性变换问题。PNP的BA方法：最小化重投影误差，通用BA方法。
5.本质矩阵求解R和t？
A：R和t被称为相机外参。E= t ^ R 为3*3的矩阵，奇异值为 [ u, u, 0] ^ T 的形式。为本质矩阵的内在性质。
      性质：理论上综合旋转、平移共有6个自由度，因尺度等价，E有5个自由度。
      求解：一般使用8点法，直接通过SVD分解，恢复出R，t 。
6.LM方法对高斯牛顿法的改进？
A：LM方法又被称为阻尼牛顿法，在高斯牛顿法的二阶泰勒展开的△x处添加一个信赖区域约束，使其不能太大。
      求解线性方程(H+λI)△x=b；提供更稳定，更准确的增量。
将二次偏导省略，于是：![](https://img-blog.csdnimg.cn/20190414170414178.png)
为了避免GN方法发散，有两种方法：
![](https://img-blog.csdnimg.cn/2019041417052915.png)
λ→+∞时：Δ/λ→JTr，即方向和梯度方向一样，变成了梯度下降法。相反，如果λ为0，就变成了高斯牛顿法。
Levenberg-Marquardt方法的好处在于可以调节:
如果下降太快，使用较小的λ，使之更接近高斯牛顿法
如果下降太慢，使用较大的λ，使之更接近梯度下降法
7.单目SLAM的尺度问题？
A：单目SLAM是可以求出一个真实尺度的，但计算结果会产生漂移。三角测量计算具有不稳定性，逐帧累计不做优化会产生更大的误差。
8.三角测量不稳定性缓解方法？
A:使用可信的位移t值，使角度变换限制在一个可信范围。
    使用深度滤波器。假设特征点误差分布符合高斯分布，多次观测方差会不断缩小，即进行滤波。
