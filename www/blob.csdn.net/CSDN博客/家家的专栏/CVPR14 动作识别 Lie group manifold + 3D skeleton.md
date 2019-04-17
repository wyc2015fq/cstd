# CVPR14 动作识别 Lie group manifold + 3D skeleton - 家家的专栏 - CSDN博客





2014年05月15日 17:01:20[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2465








Human_action_recognition_by_representing_3d_skeletons_as_points_in_a_lie group

Depth sensors.

Skeleton-based human action approaches 

A new skeletal representation that model the 3D geometric relationships between body ports. 

李群： SE(3) 

![](https://img-blog.csdn.net/20140515163620250)


**A curved manifold** : SE(3) * .... * SE(3) 这种流形结构能够很好的处理Depth sensor。

Human action can be modeled as curves on the lie group manifold.

**Classification **of these curves on the Lie group manifold




**Combination of three technologies**：

Dynamic time warping (处理速率变化 把原有的**training curve**变换为**Nominal curve。**然后进行等长采样，因为同一动作，可能长度不一样。在lie group curve 上进行均匀采样。) 

+

Fourier temporal pyramid representation （remove 高频系数，减少噪音、时间非对齐等问题）

+

Linear SVM （特征向量进行分类）




涉及到的一些基本知识：

**1、paper用到的李群流形： SE(3)群 也是李群流形中一种非常常见和重要的李群结构，在物理分析中有着重要应用。**


![](https://img-blog.csdn.net/20140515164959484)
**2、投影到李代数空间后的特征向量化**

**![](https://img-blog.csdn.net/20140515165223437)**

**3、李群和李代数之间的投影关系：**

**![](https://img-blog.csdn.net/20140515165857859)**





