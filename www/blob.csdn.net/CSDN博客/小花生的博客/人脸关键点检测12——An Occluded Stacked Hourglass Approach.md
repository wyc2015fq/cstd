
# 人脸关键点检测12——An Occluded Stacked Hourglass Approach - 小花生的博客 - CSDN博客


2018年07月14日 23:18:09[Peanut_范](https://me.csdn.net/u013841196)阅读数：674



###### 《An Occluded Stacked Hourglass Approach to Facial Landmark Localization and Occlusion Estimation》
IEEE2017，Kevan Yuen and Mohan M. Trivedi
本文主要应用于交通驾驶安全，构建了比较完善的整个系统流程，从人脸识别->关键点检测->头部姿态估计。可以通过眼部和口部关键点判断驾驶员是否有困意，疲劳驾驶等；通过头部姿态来判断驾驶员是否注意力集中等，来辅助驾驶员的操作，避免交通事故的发生。
本文提出Occluded Stacked Hourglass，是在Stacked Hourglass network for body pose joint estimation的基础上进行改进，输入上一级识别到的人脸和输出68个遮挡的热图，每张热图对应于一个人脸关键点。
**网络结构：**
![这里写图片描述](https://img-blog.csdn.net/20180714231239947?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180714231239947?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Landmark Module：**
输入固定尺寸的图像和输出68个热图
Label：振幅为1的高斯分布放置在相对于输入图像的坐标位置，其余热图区域近似为0，热图上的值大致在0~1之间。
如果关键点被遮挡，则幅值使用-1代替+1。
**训练：**
输入：人脸检测后resize到256*256（flip）
输出：68个64*64的得分图像，是输入图像的1/4。
**Ground Truth：**
![这里写图片描述](https://img-blog.csdn.net/20180714231307373?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180714231307373?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果关键点没有被遮挡：A=+1，遮挡为A=-1。
其中，$\sigma =1.5$
**损失函数：**
![这里写图片描述](https://img-blog.csdn.net/20180714231437631?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180714231437631?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
平方误差损失，输出64*64的热图对应一个关键点坐标。
![](https://img-blog.csdn.net/20180714231502144?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180714231502144?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
计算68个人脸关键点的损失。
[
](https://img-blog.csdn.net/20180714231502144?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---

###### 注：博众家之所长，集群英之荟萃。
[
						](https://img-blog.csdn.net/20180714231502144?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
	](https://img-blog.csdn.net/20180714231502144?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
