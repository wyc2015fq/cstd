# 三维重建：SLAM的尺度和方法论问题 - wishchinYang的专栏 - CSDN博客
置顶2019年04月14日 12:09:28[wishchin](https://me.csdn.net/wishchin)阅读数：8652
        百度百科的定义。此文引用了其他博客的一些图像，如有侵权，邮件联系删除。作为算法的SLAM，被称为同步相机位姿确定和地图构建。作为一个工程的SLAM，有众多的算法。
        在[计算机视觉](http://baike.baidu.com/view/155265.htm)中, 三维重建是指根据单视图或者多视图的[图像重建](http://baike.baidu.com/view/1288444.htm)三维信息的过程. 由于单视频的信息不完全,因此三维重建需要利用经验知识.。而多视图的三维重建(类似人的双目定位)相对比较容易, 其方法是先对摄像机进行标定, 即计算出摄像机的图象[坐标系](http://baike.baidu.com/view/84791.htm)与[世界坐标系](http://baike.baidu.com/view/829500.htm)的关系.然后利用多个[二维](http://baike.baidu.com/view/719535.htm)图象中的信息重建出三维信息。
        预读：三维重建：[SLAM的粒度和工程化问题](http://blog.csdn.net/wishchin/article/details/50505351) 。三维重建根据时间和场景的尺度不同需要引入不同的框架和优化方法：
**1. 对于小型物体建模**
        小型物体建模可使用相机固定的包围设备，直接得到整个三维CAD模型（甚至可以使用类似于硬件级别的光线分析的方法）。
        此外，使用KinectFusion的经典深度图像获取设备，也可使用此类通用双目相机完成物体重建的方法，同时此方法也适用于小型场景重建。
**框架1** = 几帧单幅深度图像 /+  简单图像拼接（**ICP算法族**或者简单三角/极线测量/视差算法）；
![](https://img-blog.csdn.net/20170225144655686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170225144414745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                  百科：[三维扫描仪](http://baike.baidu.com/link?url=iIf3QWI5ydl3vaZXFLpdyGGcOveqz1dTWoFWC2bU2kTjjLHeS9mlz3QDTaKZ2zuGLx47229gP52Lp5WLAh0V3-baq65SFxRvx2JucLQ5wojJS2FfiAeqme-bMZ5oNAo_)
**框架2** = 多帧单幅深度图像 + 相机位姿刚性变换（VO视觉里程算法族）+多个深度图像拼接（**ICP+TSDF**（Truncated Signed Distance Function）的方法族或者**Surfel**方法）；
    关键元素：VO算法族、**ICP算法族**、**TSDF方法或者Surfel方法**。
    Change：序列图像计算VO、使用ICP方法或者Surfel方法。Kinect方法使用TSDF网格方法，而ElasticFusion使用surfel表面方法。Kinect方法使用ICP获取VO，ElasticFusion可以在VO中断时使用surfel表面重新计算VO。
**2. 相机移动的小型场景**
     2.1. 像素级别：   使用KinectFusion的经典方法，也可使用通用双目相机，完成小型场景像素级别重建。使用框架2.
     2.2. 特征点级别：   特征点级别重建，三维重建问题转化为经典的SLAM问题。引入经典的**SLAM的方法**——**滤波方法** 和 对应的地图表示的另一种方式**拓扑地图**。
单目相机：
**框架3** = 多帧二维图像 + **特征点检测**算法+ 特征点深度计算/相机位姿刚性变换（ICP/**PNP算法**）+ 滤波方法（**EKF等框架**（运动模型+观测模型））+ ANN/RANSAC/ICP/......；
    关键元素：**特征点检测**、**特征点匹配（ANN近邻方法）**、**PNP算法（三维位姿计算）、滤波方法族、**RANSAC算法。
         Change：（1）基于特征点引入特征提取和特征点模式识别方法，分别为**特征点检测**算法族 和**近邻搜索方法**族。
                        （2）基于特征点的三维位姿计算，引入PNP算法族。
                        （3）基于滤波框架，线性滤波方法族，用于同时优化相机和特征点刚性变换的误差和协方差方程。
双目相机：
        使用双目相机的优势在于可获得标准的相机基线，得到空间的绝对尺度，并同时可根据固有视差算法得到特征点的深度信息。可以使用三维稀疏重建的方法。
**框架4**= 多帧成对三维图像 + 特征点检测算法/**视差计算算法**（特征点深度计算）+ 相机位姿刚性变换（ICP算法）+ RANSAC/ICP/......；
    关键元素：**视差方法**（深度计算）、**稀疏匹配**（RANSAC、ICP方法族）。
        Change：（1）深度计算：使用视差算法 Vs.  PNP算法。
                       （2）位姿刚性变换变换到ICP算法族。
                         注意：深度计算之后，整体框架类似于KinectFusion方法，区别在于点集变得稀疏，因此稠密点匹配算法替换为稀疏点匹配算法（ANN方法族）。
**3. 相机移动的中型场景**
      3.1. 像素级别：  正如上一篇所说的，是一个大型工程问题，不是一个算法和框架可以描述的，需要更多设备和人的配合。大型场景重建一般使用表面模型，再次不再对其进行多加描述。
      3.2. 特征点级别：  
            特征点级别重建，在可得到序列结构的VO之后，经典的SLAM的方法为滤波方法，为基于最大似然框架。而添加闭环检测之后，借鉴SFM思想的平差方法--捆集优化**BA**（**Bundle Adjustment**）引入到SLAM中来，因此形成了另一种框架。
**框架5**= 多帧二维/三维图像 +**特征点检测**算法+ 特征点深度计算/相机位姿刚性变换（**PNP算法**）+BA方法（关键帧平差方法）+ ANN/RANSAC/ICP/......+** 闭环检测**（场景匹配/识别方法族）；
     关键元素：**BA方法**（图优化方法等）、**闭环检测**（点集组合场景检测）。
        Change：（1）类似于框架3，位姿和场景同时优化的方法从滤波框架转换到BA框架，引入**关键帧平差方法**。
                        （2）基于BA方法，使用关键帧，形式化为无向图，一般使用**图优化方法**。常用的基础库为**G2O**。
                        （3）引入闭环检测，需要使用**场景匹配方法**（基于特征点），经典方法为**使用BOW模型**。
**4. 相机长期移动的大中型场景**
        特征点和目标级别：   大型场景需要构建大型地图，应对长期误差累计而发展的方法闭环检测重要性增加。对此，闭环检测引入目标识别，进而引入基于目标检测的场景识别，更名为语义SLAM。
        语义SLAM：大型场景重建，已走过重复地标记-闭环检测占据更多的分量，场景识别更关注语义分析。
**框架6**= 多帧二维/三维图像 + **特征点检测**算法+ 特征点深度计算/相机位姿刚性变换（**PNP算法**）+**目标检测**（目标检测方法族）+**目标识别**（目标识别方法族） + **BA方法**（**关键帧平差方法**）+ ANN/RANSAC/ICP/......+ **闭环检测**（基于特征点的闭环检测+基于目标的闭环检测+场景检测）；
    关键元素：目标检测（目标特征提取与目标识别）、场景检测标记（基于目标）。
        Change：（1）基于框架5或者进行改进，引入基于目标的特征提取方法（**目标提取**）族和模式识别方法（**目标识别**）族。由此根据不同的场景需要使用不同的目标检测的通用方法。由此一大批模式识别方法融入SLAM过程，从简单的ANN，到MSVM，到DNN方法。
                       （2）闭环检测之中增加基于目标的闭环检测，和场景识别。场景检测从BOG模型到基于图的场景识别模型，也可以使用DNN方法中CNN方法。
                       （3）......................................。
        参考文章：[场景图像的稀疏表示总结](http://blog.csdn.net/wishchin/article/details/12452331)
**总结：**
         三维重建是一个工程问题，发展到三维场景重建，SLAM问题及应对方法相应而生。经典可用的SLAM方法为把SLAM问题转化为滤波方法，是一个真正可用的解决SLAM问题的方法。而后BA方法出现，并随着计算机计算能力的提高，BA方法逐渐应用于SLAM过程。更宽范围的SLAM方法引入目标识别，模式识别的方法（从ANN到MSVM到CNN）引入SLAM，成为语义SLAM，把SLAM发展为“一切问题都可以用模式识别方法来解决”的巨坑。
**后记：红酒塔问题**
        三维重建里面有一个红酒塔问题，即是怎样使用重建算法重建出波光闪闪的透明玻璃杯红酒塔。
        其实，红酒塔问题不应该是一个重建问题，从人的感知角度分析，红酒塔也是不能只是靠双目重建的，而其本质是目标识别。识别红酒塔，然后使用模型匹配进行模型补完，重建整个红酒塔，是一个识别之后alignment的过程。
### 参考：
1. 双目相机-ElasticFusion是三维重建领域效果比较显著的开源库，其作者为Facebook Reality Labs的[Thomas Whelan](http://www.thomaswhelan.ie/)博士。[ElasticFusion](https://github.com/mp3guy/ElasticFusion)的github源码主页：[https://github.com/mp3guy/ElasticFusion.git](https://github.com/mp3guy/ElasticFusion.git)。小品文：[ElasticFusion解释](https://blog.csdn.net/fuxingyin/article/details/51433793)；
2.双目相机-KinectFusion是微软亚研院开源的一个室内深度场景重建算法，ICRA2011论文：[https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/kinectfusion-uist-comp.pdf；](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/kinectfusion-uist-comp.pdf) 小品文：[深度相机方案对比-KinectFusion的基本原理（尺度）](https://blog.csdn.net/wishchin/article/details/42193535)。
3.单目相机-EKF-SLAM方法是经典的稀疏场景重建方法，小品文：[SLAM: Inverse Depth Parametrization for Monocular SALM](https://blog.csdn.net/wishchin/article/details/49423559)。EKF在GitHub下载。
4.OrbSLAM工程化良好，成为一个流行的稀疏重建框架，并应用到不同的场景中。小品文：[ROS：ubuntuKylin17.04-Ros使用OrbSLAM2](https://blog.csdn.net/wishchin/article/details/72237032)。GitHub下载OrbSLAM2。
5.SemanticFusion，结合了CNN方法的重建方法，在数据关联时使用CNN方法。ICRA2017论文，[SemanticFusion: Dense 3D Semantic Mapping with Convolutional Neural Networks](https://arxiv.org/abs/1609.05130)。论文解析：[ElasticFusion解析](https://blog.csdn.net/Crazydogg/article/details/82669949)。SemanticFusion开源代码依赖ElasticFusion和Caffe-CNN，使用方法：[Ubuntu16使用SemanticFusion](https://blog.csdn.net/pikachu_777/article/details/84882354)
