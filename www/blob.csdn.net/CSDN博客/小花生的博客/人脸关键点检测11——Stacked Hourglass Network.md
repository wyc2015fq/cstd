
# 人脸关键点检测11——Stacked Hourglass Network - 小花生的博客 - CSDN博客


2018年07月14日 23:09:32[Peanut_范](https://me.csdn.net/u013841196)阅读数：1623



###### 《Stacked Hourglass Network for Robust Facial Landmark Localisation》
CVPR2017, Jing Yang 、Qingshan Liu
本文提出的方法主要由两部分构成，face transformation和stacked Hourglass Networks。
**第一步：supervised face transformation**
目的是减弱数据中大姿态的变化对人脸对齐的影响。通过平移、缩放、旋转变化来校正成平均人脸图像，通过5点的关键点来完成，使用相似变换进行校正图像。
**第二步：four stacked Hourglass Networks**
Houglass网络能够提取多尺度具有辨别力的特征，也可以作为一个回归器去定位最后的关键点坐标。
**整体结构：**
![这里写图片描述](https://img-blog.csdn.net/20180714230403966?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Hourglass结构：**
![这里写图片描述](https://img-blog.csdn.net/2018071423042887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/2018071423042887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
结构中参数比较清晰，仅改变深度，不改变尺寸，可以作为一种比较高级的卷积层。
[
](https://img-blog.csdn.net/2018071423042887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**损失函数**：L2 loss
![](https://img-blog.csdn.net/20180714230454692?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180714230454692?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**评价标准：**
![这里写图片描述](https://img-blog.csdn.net/20180714230522348?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**总结：**
论文结构比较清晰，方法类似于2D-FAN，但前端增加了一个校正结构，更好的处理大姿态的人脸对齐。
---

###### 注：博众家之所长，集群英之荟萃。

