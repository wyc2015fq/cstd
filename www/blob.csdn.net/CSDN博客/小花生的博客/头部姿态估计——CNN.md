
# 头部姿态估计——CNN - 小花生的博客 - CSDN博客


2018年08月06日 19:30:08[Peanut_范](https://me.csdn.net/u013841196)阅读数：2760


《Face Alignment Assisted by Head Pose Estimation》
人脸对齐的错误案例分析：**头部姿态通常都是大角度的。**
![这里写图片描述](https://img-blog.csdn.net/2018080619250634?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/2018080619250634?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**主旨**：通过人脸头部姿态来辅助关键点的检测，保证大角度人脸对齐的正确性。
**动机**：
1）大多数现有的方法在大姿态变化的人脸图像上表现的不好。
2）最近的人脸对齐方法都是基于级联的方式和初始化于平均形状。
**策略**：
第一步，我们使用ConvNet直接从人脸图像上估计出头部姿态；
第二步，两种方案来生成初始化，
1）设计一个标准的3D人脸形状；
2）从训练集中通过头部姿态最近邻的方法来搜索形状。
**数据准备：**
![这里写图片描述](https://img-blog.csdn.net/20180806192544127?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180806192544127?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
使用3D头部姿态估计器来计算300W数据的头部姿态。
[
](https://img-blog.csdn.net/20180806192544127?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**头部姿态估计：**
![这里写图片描述](https://img-blog.csdn.net/20180806192651364?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Evaluation：**
![这里写图片描述](https://img-blog.csdn.net/20180806192719870?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---

###### 注：博众家之所长，集群英之荟萃。

