# Robust Real-Time Extreme Head Pose Estimation - happyhorizon的算法天空 - CSDN博客
2017年09月28日 19:09:24[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：346标签：[算法																[人脸识别																[姿态识别](https://so.csdn.net/so/search/s.do?q=姿态识别&t=blog)](https://so.csdn.net/so/search/s.do?q=人脸识别&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[Deep Learning](https://blog.csdn.net/happyhorizion/article/category/7157842)
## 基本思路：
用RGB-D 的摄像头，利用RGB和深度信息对人脸进行三位建模和合成。之后建立了一个由33个人不同头部姿态点云合成数据组成的数据库Dali3DHP，基于级联决策树（5个）训练，采用的ground truth是用Shimmer Sensor得到的头部的姿态角（在正视状态下，roll方向接近于零）得到了在不同头部姿态下的人脸朝向判断模型。这也就决定了算法在头部姿态偏离较大的时候正确率反而更高一些。 
测试中将方位和倾斜角度按照20度/15度划分网格，下图中展示了最终模型在不同角度下的识别正确率。 
![这里写图片描述](https://img-blog.csdn.net/20170928191003551?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下图展示了一些在极端头部角度下识别面部朝向的例子： 
![这里写图片描述](https://img-blog.csdn.net/20170928191131678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
基于点云的三维人脸建模数据管道如下图。从这里可以看出三维人脸建模的基本原理，就是对人脸的不同角度用RGB-D摄像头拍摄，之后对点云数据进行融合实现三维建模。在点云融合之前，首先利用Gauss核对深度图像做卷积处理以降噪。光滑点云的Voxel-grid方法备用来以比较小的三维点数据得到人脸的建模。
![这里写图片描述](https://img-blog.csdn.net/20170928191056085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 参考文献：
Tulyakov S, Vieriu R L, Semeniuta S, et al. Robust Real-Time Extreme Head Pose Estimation[C]// International Conference on Pattern Recognition. IEEE Computer Society, 2014:2263-2268.
