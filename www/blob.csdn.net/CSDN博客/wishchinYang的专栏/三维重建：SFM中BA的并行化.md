# 三维重建：SFM中BA的并行化 - wishchinYang的专栏 - CSDN博客
2017年03月30日 10:25:31[wishchin](https://me.csdn.net/wishchin)阅读数：1174
       SFM，运动中结构重建。SFM是一种离线重建方法，SFM未得到原始数据中的帧间关系，因此在前期处理与SLAM有所不同。SLAM方法依据视觉里程算法，构建状态数据序列即图结构，仅需要解决累计误差问题。而SFM需要使用匹配和优化方法，重建数据的图结构，需要优化方法，SFM过程比SLAM过程更加复杂。
        BA：光束平差法，利用非线性最小二乘法求取相机位姿，三维点坐标。Bundle adjustment is almost always used as the last step of every feature-based [3D reconstruction](https://en.wikipedia.org/wiki/3D_reconstruction) algorithm. It amounts to an optimization problem on the 3D structure and viewing parameters (i.e., camera [pose](https://en.wikipedia.org/wiki/Pose_(computer_vision)) and possibly intrinsic calibration and radial distortion), to obtain a reconstruction which is optimal under certain assumptions regarding the noise pertaining to the observed[[1]](https://en.wikipedia.org/wiki/Bundle_adjustment#cite_note-triggs1999-1) image features：在仅给定相机内部矩阵的条件下，对四周物体进行高精度重建。Bundle Adjustment的优化目标依旧是最小化重复投影误差。作为三维重建的最后一步，可以处理数据丢失情况并提供真正的最大似然估计。
![](https://img-blog.csdnimg.cn/20190401131303718.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)重投影误差![](https://img-blog.csdnimg.cn/20190401131451983.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)平差
               图片：[https://www.cnblogs.com/Jessica-jie/p/7739775.html](https://www.cnblogs.com/Jessica-jie/p/7739775.html)
       BA的**由来**：光束平差法由Bundle Adjustment译得，即对所有光束进行平差。有**两层释义**：1.对场景中任意三维点P，由从每个视图所对应的的摄像机的光心发射出来并经过图像中P对应的像素后的光线，都将交于P这一点，对于所有三维点，则形成相当多的光束（bundle）；2. 实际过程中由于噪声等存在，每条光线几乎不可能汇聚与一点，因此在求解过程中，需要不断对待求信息进行调整（adjustment），来使得最终光线能交于点P。对n帧，含k个特征点的目标函数如下：
![](https://img-blog.csdnimg.cn/20190401133033128.png)
- k为三维空间点个数；
- n为成像平面个数；
- mij表示第i个三维点在第j$ 个成像平面对应的特征点坐标；
- vij表示点 i 在成像平面 j 上是否有投影，如果有 vij=1，否则vij=0;
- Pi表示每个成像平面对应的外参数向量；
- Mi表示每个三维点的坐标向量；
- Q(aj,bi)是重投影函数，将三维点 Mi映射到成像平面；
- d(x,y)为距离度量函数，一般为欧式距离函数。
       这是一个非凸问题。以上便是光束平差法目标函数的原理。由于场景中特征点往往较多，该问题是一个巨大的高维**非线性优化**问题。接下来，需要对上述式子进行求解，这是光束平差法的核心内容。
        BA解法：通用优化方法。一阶方法，即对问题的目标函数进行泰勒一阶展开后进行迭代求解的方法。对雅克比矩阵使用梯度下降法是一阶方法之一。二阶优化方法，即会将目标函数展开至泰勒二阶项然后进行优化求解，有牛顿法、LM方法、LBFGS方法等。LM方法易于实现，且效果较好，应用广泛。
       详细公式推理和优化推导过程（一个或者两个相机）：[机器人视觉-光束法平差。](https://www.cnblogs.com/ironstark/p/5493030.html)wiki：[https://en.wikipedia.org/wiki/Bundle_adjustment](https://en.wikipedia.org/wiki/Bundle_adjustment)。Bundle Adjust优化方法-LM方法的Code：[https://blog.csdn.net/micro_msdn/article/details/78584484。](https://blog.csdn.net/micro_msdn/article/details/78584484)
0.SLAM中的BA
        SLAM中的BA服务于重建。光束平差法定义性的阐述应用于相机和共同观测点的优化，在SLAM中，不同的位姿观察到不完全重复的点集，形成一个大的稀疏图。BA中的SLAM方法涉及了位姿的变化，对位姿和地图点进行同时优化。
       在SLAM过程中方程与三维重建中BA定义过程有所区别，对每一帧进行计算观测误差，对所有帧进行共同优化，公式为：
![](https://img-blog.csdnimg.cn/20190401134734487.png)
       对m帧，每一帧有N个观测点的共同最小二，求这个最优解。
1. BA在重建中的作用
         借鉴于运动中重建的方法，BA引入SLAM过程，而传统的滤波方法引入BA是跟随闭环检测出现，假设每一次BA过程即形成一个闭环。
1.1 BA在滤波方法中的嵌入
           PTAM分离了跟踪和建图过程，使其并行完成，只进行数据交互。引入关键帧的机制，构建稀疏性，使用BA独立线程优化局部地图和全局地图，因此可以达到实时性，突破了传统滤波算法的地图规模限制。
1.2 BA在闭环检测之后的应用
           在三维重建检测到闭环之后，则可以根据匹配结果，计算出总误差，并把误差平均到闭环之内的每一选定帧。
2. BA的并行化
          借鉴于运动中重建，把所有的误差平均到每一个选定的关键帧里，对于帧数较多时，可以使用数据并行化。
          数据并行化是方法并行化的必要条件，多帧数据进行平差的方法天然可适合并行处理。
          并行BA：[**Multicore Bundle Adjustment**](http://grail.cs.washington.edu/projects/mcba/)
###        In this project, we consider the design and implementation of new inexact Newton type Bundle Adjustment algorithms that exploit hardware parallelism for efficiently solving large scale 3D scene reconstruction problems
          即是 [BA in the Large](http://grail.cs.washington.edu/projects/bal/)
          Recent work in Structure from Motion has demonstrated the possibility ofreconstructing geometry from large-scale community photo collections. Bundle adjustment, the joint non-linear refinement of camera and point parameters, is a key component of most SfM systems, and one which can consume a significant amount of time for large problems. As the number of photos in such collections continues to grow into the hundreds of thousands or even millions, the scalability of bundle adjustment algorithms has become a critical issue.
          In this project, we consider the design and implementation ofa new Inexact Newton type bundle adjustment algorithm, which usessubstantially less time and memory than standard Schur complementbased methods, without compromising on the quality of the solution. Weexplore the use of the Conjugate Gradients algorithm for calculatingthe Newton step and its performance as a function of some simple andcomputationally efficient preconditioners. We also show that the use of the Schur complementis not limited to factorization-based methods, how it can be used as part of the Conjugate Gradients (CG) method without incurring the computational cost of actually calculating and storing it in memory, and how this use is equivalent to the choice of a particular preconditioner.
### Paper
[Bundle Adjustment in the Large](http://grail.cs.washington.edu/projects/bal/bal.pdf)
    Sameer Agarwal, Noah Snavely, Steven M. Seitz and Richard Szeliski
