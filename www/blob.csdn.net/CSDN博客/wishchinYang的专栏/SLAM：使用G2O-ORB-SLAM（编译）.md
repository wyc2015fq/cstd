# SLAM：使用G2O-ORB-SLAM（编译） - wishchinYang的专栏 - CSDN博客
2016年01月05日 11:40:21[wishchin](https://me.csdn.net/wishchin)阅读数：9214
**前言：**
         没有新雪，看看自己所做的事情，有没有前人做过。果然，EKF_SLAM的版本出现了Android版本的OpenEKFMonoSLAM, G2O-ORB SLAM也出现了VS2012版本。
**一、SLAM 问题：**
~~        机器人需要在自身位置不确定的条件下，在完全未知环境中创建地图,同时利用地图进行自主定位和导航。这就是移动机器人的同时定位与地图创建(SLAM) 问题，最先是由Smith Self 和Cheeseman 在1988年提出来的，被认为是实现真正全自主移动机器人的关键。 由 Smith, R.C. and P. Cheeseman, 提出的论文：On the Representation and Estimation of Spatial Uncertainty. 《International Journal of Robotics Research》, 1986. 5(4): p. 56 -- 68. 以滤波的形式表示SLAM问题。~~
~~       随后的时间里，Se, S., D. Lowe and J. Little,的论文： Mobile robot localization and mapping with uncertainty using scale-invariant visual landmarks. 《The international Journal of robotics Research》, 2002. 21(8): p. 735--758.中使用了卡尔曼滤波的方法。~~
~~       使用EKF的方法是一段时间内的SLAM主流方法。~~
~~**SLAM问题描述**为：机器人在未知环境中从一个未知位置开始移动，在移动过程中根据位置估计和传感器数据进行自身定位，同时建造增量式地图。在SLAM中，机器人利用自身携带的传感器识别未知环境中的特征标志，然后根据机器人与特征标志之间的相对位置和里程计的读数估计机器人和特征标志的全局坐标。这种在线的定位与地图创建需要保持机器人与特征标志之间的详细信息。近几年来，SLAM的研究取得了很大的进展,并已应用于各种不同的环境，如：室内环境、水下、室外环境。~~
**二、图计算的问题**
         另外一条路径：SLAM问题本质上是场景重建，主要方式是帧间匹配构建三维场景，即构建增量式地图。主要三维构建方法是BA ，直观上可以使用的计算机科学算法为图优化。
~~        直到21世纪初，卡尔曼滤波器仍在SLAM系统占据最主要的地位，Davison经典的单目SLAM：Divide and Conquer: EKF SLAM in O(n), Paz Lina M et al., 《IEEE Transaction on Robotics》, 2008即是用EKF做的。由于滤波器方法存储n个路标要消耗n平方的空间，在计算量上有点难以控制。尽管08年有人提出分治法的滤波器能把复杂度弄到O(n) ，但实现手段比较复杂。~~
       
 后来，出现了基于图优化的SLAM方法： Visual SLAM: Why filter? Strasdat et. al.,《 Image and Vision Computing》, 2012.，渐渐有取而代之的地位。根本原因是计算能力的提高，可以在图优化的计算上达到要求。
       图优化实际上是解一种非线性最小二乘问题，主要用于离线的slam优化，也有用在在线的方式的。 最小二乘解决的就是偏差全局最小的问题，再在原基础量上叠加最小偏差量即为最优量。 图优化将问题全部抽象成 node(点)与edge（边）的问题。
       g2o：[http://www.openslam.org/g2o.html](http://www.openslam.org/g2o.html)，就是对图优化问题的一个求解器。它原理上是一个通用的求解器，并不限定于某些SLAM问题。你可以用它来求SLAM，也可以用ICP, PnP以及其他你能想到的可以用图来表达的优化问题。g2o是一个平台，你可以加入你自己的线性方程求解器，编写自己的优化目标函数，确定更新的方式。g2o的作者说Guassian-Newton和Levenberg-Marquardt方法比较naive，但是g2o的本质就是这些算法的实现。事实上，g2o
 iSAM SPA和 sSPA等非线性优化算法只是在非线性问题线性化时处理得不一样，在线性化后要求解线性方程都是利用了已有的linear solver库来求解，如 CSparse CHOLMOD PCG等，他们都需要依靠Eigen这个线性代数库。
**三、ORB_SLAM的方法**
       ORB_SLAM：[https://github.com/raulmur/ORB_SLAM](https://github.com/raulmur/ORB_SLAM) 的创新点为使用了ORB特征，但主要构建地图框架为图优化，整个工程有用于构建地图的g2o库、用于回环检测的DBoW2、用于点云处理的PCL库及第三方库、以及用于图像处理的ORB特征提取器和预处理算法等。
**四、编译问题**
          可以直接使用工程文件附带的g2o的源代码编译，不需要其他的辅助编译工作。
        源代码出现了编译问题。
**问题一**：
               没有仔细阅读源代码，发现工程自带的G2O库找不到使用的函数： 
                    Optimizer.cpp使用的函数： g2o::EdgeSE3ProjectXYZ* e = new g2o::EdgeSE3ProjectXYZ();
                                 read()及write()函数都出现链接错误。
              使用最新的Git版本，仍然出现找不到函数这种链接错误。最后，终于发现问题，项目所带的源代码相对于工程是正确的，而Git最新版本进行了大量的修改，所以使用项目自带的源代码进行重新编译。
               可以进行编译并生成一些lib文件，有一些编译失败，但不影响程序的运行。
**问题二**：
               出现：error LNK2019: 无法解析的外部符号 __imp___  此种链接错误，修改方法为：             
               所在的项目点右键，选择“属性”->“配置属性”->“C/C++”->“预处理器”->“预处理定义”
 移除_DENUG或将_DENUG改为NDENUG
        项目可以运行了。
