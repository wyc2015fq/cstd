# 三维重建：深度相机方案对比-KinectFusion的基本原理（尺度） - wishchinYang的专栏 - CSDN博客
2014年12月27日 16:04:19[wishchin](https://me.csdn.net/wishchin)阅读数：5361
个人分类：[场景处理/RgbD累积																[AR/VR_3D																[三维重建/SLAM](https://blog.csdn.net/wishchin/article/category/5723249)](https://blog.csdn.net/wishchin/article/category/5585823)](https://blog.csdn.net/wishchin/article/category/1524721)
         KinectFusion算法原理依据2011年发表的Fusion重建的论文，主要重建方法为TSDF算法，并在GitHub上开源了相关代码，可以直接编译使用。
        论文：[https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/kinectfusion-uist-comp.pdf；](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/kinectfusion-uist-comp.pdf)
        算法原理请参考此译文：  [kinect fusion 3D重建基本算法](http://blog.csdn.net/xiaohu50/article/details/51592503)[http://log.csdn.net/xiaohu50/article/details/51592503](http://log.csdn.net/xiaohu50/article/details/51592503)
         三维重建为三维空间实体建立适合计算机表示和处理的三维数学模型，建立一个相对于真实三维世界坐标系的计算机世界坐标系中的三维映射模型。
         三维重建的关键因素为尺度（点的邻域覆盖单位）、相对位置（刚体物体的体元相对位置及位置映射）、原点位置。
         对于尺度问题，通常有两种解决方法。第一种，直接得到实体到计算机世界坐标系的绝对映射关系，这就需要一个固定的位置映射系统，比如一个多个单目相机固定的摄影台，或者一个可以运动的双目摄像机。第二种，直接使用单目相机，使用特征配准或者位置点优化的方法，构建稀疏点集，进而构建模型，此种方法称为SLAM。
         使用多目或者双目摄像机系统的系统，关键点在于表面重建，局部的表面重建递进完成可以构建最终的完整表面；使用单目相机关键在于稀疏点的构建，寻找表面的显著点，构建一个物体的完整网格，再次通过补全网格的方法构建模型。
**一、绝对尺度**
         获得绝对尺度对于模型三维重建是非必须的，但对于一个扩展的三维重建应用在某些方面是必须的，比如希望与真实环境交互的移动机器人（扫地机器人、无人机、无人潜艇）。
         多目系统可以根据固定的相机位置获取计算机世界中的度量单位代表的真实世界尺度，双目相机也可以根据双目的基线获得此意义。单目相机是怎么样得到这个真实尺度的呢？通过相机的内参——焦距的定义。
        尺度的获取方式：多目和深度相机可以直接根据得到的深度值，进而由透视关系得到绝对尺度。对于单目相机，必须完成整个稀疏构建过程才能得到相对精确的深度，只有完成整个SLAM过程，才能得到真实尺度。
**二、相对位置**
        相对位置的获取为表面重建问题，可分为稀疏表面重建和稠密重建。
        SFM使用稀疏重建，需要检测特征：No explicit feature detectionUnlike structure from mo-tion (SfM) systems (e.g. [15]) or RGB plus depth (RGBD)techniques (e.g. [12,13]), which need to robustly and con-tinuously detect sparse scene features, our approach to cam-era tracking avoids an explicit detection step, and directlyworks on the full depth maps acquired from the Kinect sen-sor. Our system also avoids the reliance on RGB (used inrecent Kinect RGBD systems e.g. [12]) allowing use in in-door spaces with variable lighting conditions.
        深度相机根据双目视差原理等直接获得三维表面，这个表面是透视畸变的，可以通过透视校正的方法进行标准化补偿，得到 确定 面元，这就做到了表面重建的第一步。再经过一系列处理，最后通过ICP方法等逐帧稠密匹配的方法，可以迭代构建整个三维模型。
Kinect 关键点：High-quality reconstruction of geometryA core goal ofour work is to capture detailed (ordense) 3D models ofthe real scene. Many SLAM systems (e.g. [15]) focuson real-time tracking, usingsparsemaps for localizationrather than reconstruction. Others have used simple point-based representations (such as surfels [12] or aligned point-clouds [13]) for reconstruction. KinectFusion goes beyondthese point-based representations by reconstructingsurfaces,which more accurately approximate real-world geometry。
**单目相机对于稠密匹配在算法的设计代价上是得不偿失的**，而对于稀疏构建是合适的。而尺度的未知和深度的未知即是 地图构建和相机同步定位 对于稀疏构建则是必须同时进行的，这就是常见的SLAM问题。
          KinectFusion流程：点云获取（2D转3D，由硬件计算得到）、预处理（降噪、滤波-保边？去除或者保留超斜面？密度滤波）、相机追踪（GPU分布式的ICP算法）、密集重建（TSDF算法，即截断SDF算法，把相机坐标系的点云统一到世界坐标系）、显示结果（RGB绑定和表面渲染、构建出表面）。
          参考：[各种SDF算法](https://blog.csdn.net/tracelessle/article/details/59693743)projective TSDF/TSDF/flipped TSDF-[比较](https://blog.csdn.net/tracelessle/article/details/59693743)；Kinect重建的精度严重依赖于TSDF的一些参数，比如截断距离的阈值。
**三、原点位置**
        坐标系原点位置，是在三维表面重建时附带的一个小的问题，计算机数据世界坐标系的原点位置确定与否代表了不同的构建方案。
VS2013编译PCL1.8.0 还是失败了，没有编译成功，修改为KinectFusion的文章吧
         VS2013社区版对个人开发者免费，终于可以不用该死的盗版了。Win10科技预览版X64系统开发版本下载，与Vs2013可以完美融合。
        PCL的源代码版本已经进化到1.8.0，不知道增加了多少新特性，还是拿来编译一下试一试。对于VS2012无法编译PCL1.7.2的问题，原因是OpenNI不支持1600以上数字的编译器，即最高支持到VC10。
       以下是注意事项：
       1. OpenNI：
           关于OpenNI的支持问题，把OpenNI 的包含文件 xxxxxGrab.h里面的 if ...>1600去掉，编译会完美通过；
       2.Boost：
          关于Boost的使用问题，Boost不支持CMAKE编译，支持VC12的最低版本为1.5.6.0，GIT有完整的Win二进制安装包；
       3.VTK:
           关于VTK的问题，使用罪行版本的VTK6.1，注意 源码里面的一个错误 ：在58行处找到 宏定义 ，转到声明处， 补全定义数字 ，我补全的为 8；
```cpp
<del>    118>  Build all projects
    ========== Build: 118 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========</del>
```
       4.CMAKE:
           关于CMAKE，使用CMAKE3.0以上，添加了对VC12的支持。
**四、深度相机方案对比**：
       文章链接：[深度相机-三种方案对比](http://blog.csdn.net/app_12062011/article/details/52511701)（结构光、双目、TOF）;文章链接：[深度相机-TOF总结](http://blog.csdn.net/app_12062011/article/details/52486193) ;
RGB-D方案对比：
关键技术规格：
     1.检测范围；2.检测精度；3.检测角度；4.帧率。
     5.模块大小；  6.功耗
目前主流的深度摄像头方案在检测距离上、精度、检测速度上相差不大，区别在于：
      1、结构光方案优势在于技术成熟，深度图像分辨率可以做得比较高，但容易受光照影响，室外环境基本不能使用；
      2、TOF方案抗干扰性能好，视角更宽，不足是深度图像分辨率较低，做一些简单避障和视觉导航可以用，不适合高精度场合。受环境影响小，传感器芯片并不成熟，成本很高，实现量产困难。
       3、双目方案，成本相对前面两种方案最低，但是深度信息依赖纯软件[算法](http://lib.csdn.net/base/datastructure)得出，此算法复杂度高，难度很大，处理芯片需要很高的计算性能，同时它也继承了普通RGB摄像头的缺点：在昏暗环境下以及特征不明显的情况下并不适用。
双目RGB、结构光、TOF三种主流技术的详细的比较：
|方案|双目|结构光|TOF|
|----|----|----|----|
|基础原理|双目匹配，三角测量|激光散斑编码|反射时差|
|分辨率|中高|中|低|
|精度|中|中高|中|
|帧率|低|中|高|
|抗光照（原理角度）|高|低|中|
|硬件成本|低|中|高|
|算法开发难度|高|中|低|
|内外参标定|需要|需要||
|||||
**总结**：
       1**.双目方案**，最大的问题在于实现算法需要很高的计算资源，导致实时性很差，而且基本跟分辨率，检测精度挂钩。也就是说，分辨率越高，要求精度越高，则计算越复杂，同时，纯双目方案受光照，物体纹理性质影响。
       2.**结构光方案**，目的就是为了解决双目中匹配算法的复杂度和鲁棒性问题而提出，该方案解决了大多数环境下双目的上述问题。但是，在强光下，结构光核心技术激光散斑会被淹没。因此，不合适室外。同时，在长时间监控方面，激光发射设备容易坏，重新换设备后，需要重新标定。
       3.**TOF方案**，传感器技术不是很成熟，因此，分辨率较低，成本高，但由于其原理与另外两种完全不同，实时性高，不需要额外增加计算资源，几乎无算法开发工作量，是未来。
