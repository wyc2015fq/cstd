
# 关键点检测——标签（Ground Truth）构建 - 小花生的博客 - CSDN博客


2019年01月02日 21:55:24[Peanut_范](https://me.csdn.net/u013841196)阅读数：504标签：[关键点																](https://so.csdn.net/so/search/s.do?q=关键点&t=blog)[Coordinate																](https://so.csdn.net/so/search/s.do?q=Coordinate&t=blog)[Heatmap																](https://so.csdn.net/so/search/s.do?q=Heatmap&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Coordinate&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=关键点&t=blog)个人分类：[人脸关键点检测																](https://blog.csdn.net/u013841196/article/category/7697032)
[
																								](https://so.csdn.net/so/search/s.do?q=关键点&t=blog)



#### 关键点检测——标签（Ground Truth）构建
Coordinate、Heatmap和Heatmap + Offsets
首先介绍一下关键点回归的Ground Truth的构建问题，主要有两种思路，**Coordinate**和**Heatmap**，Coordinate即直接将关键点坐标作为最后网络需要回归的目标，这种情况下可以直接得到每个坐标点的直接位置信息；Heatmap即将每一类坐标用一个概率图来表示，对图片中的每个像素位置都给一个概率，表示该点属于对应类别关键点的概率，比较自然的是，距离关键点位置越近的像素点的概率越接近1，距离关键点越远的像素点的概率越接近0，具体可以通过相应函数进行模拟，如Gaussian等，如果同一个像素位置距离不同关键点的距离大小不同，即相对于不同关键点该位置的概率不一样，这时可以取Max或Average，如下图（摘自论文[1]）所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190102213057493.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
对于两种Ground Truth的差别，Coordinate网络在本质上来说，需要回归的是每个关键点的一个相对于图片的offset，而长距离offset在实际学习过程中是很难回归的，误差较大，同时在训练中的过程，提供的监督信息较少，整个网络的收敛速度较慢；Heatmap网络直接回归出每一类关键点的概率，在一定程度上每一个点都提供了监督信息，网络能够较快的收敛，同时对每一个像素位置进行预测能够提高关键点的定位精度，在可视化方面，Heatmap也要优于Coordinate，除此之外，**实践证明，Heatmap确实要远优于Coordinate**，具体结构如下图所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190102213311579.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
最后，对于**Heatmap + Offsets**的Ground Truth构建思路主要是Google在CVPR 2017上提出的，与单纯的Heatmap不同的是，Google的Heatmap指的是在距离目标关键点一定范围内的所有点的概率值都为1，在Heatmap之外，使用Offsets，即偏移量来表示距离目标关键点一定范围内的像素位置与目标关键点之间的关系。目前还没有在公开的论文看到有人比较过这两种Ground Truth构建思路的效果差异，但是个人认为Heatmap + Offsets不仅构建了与目标关键点之间的位置关系，同时Offsets也表示了对应像素位置与目标关键点之间的方向信息，应该要优于单纯的Heatmap构建思路。如下图所示（摘自论文[2]）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190102213513891.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
### Heatmap训练流程：
输入：人脸检测后resize到256x256（flip）,通常为了减少计算会缩放到64x64。
输出：68个64*64的得分图像，是输入图像的1/4。
### Ground Truth：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190102214306434.png)
如果关键点没有被遮挡：A=+1，遮挡为A=-1。
其中，$\delta =1.5$
### 损失函数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190102214630818.png)
**平方误差损失**，输出64*64的热图对应一个关键点坐标。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190102214649307.png)
计算68个人脸关键点的损失。
[
](https://img-blog.csdnimg.cn/20190102214649307.png)
### 评价标准：
[
](https://img-blog.csdnimg.cn/20190102214649307.png)目前主要的衡量标准是算法所获取的关键点位置与真实关键点位置之间的偏差。在评价偏差时，由于不同人脸图像的实际大小难免会有所差异，为便于在同样的尺度下比较算法性能，需要采用一定的数据归一化策略． 目前主流的方法是基于两眼间的距离进行人脸大小的标准化，即：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190102214803903.png)
其中分子 表示估计值与真实值的欧式距离，分母 表示双眼距离，即两眼中心的欧式距离。
**注：**一般使用的度量方法是 the metric used for face alignment is the point-to-point Euclidean distance normalized by the inter ocular distance （对大角度侧脸数据不友好）；
这里改进了一下度量方式:normalize by the bounding box size. In particular, we used the Normalized Mean Error（用框来归一化）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190102214847685.png)
部分摘录：[https://zhuanlan.zhihu.com/p/37933909](https://zhuanlan.zhihu.com/p/37933909)
参考：
[1] Cao Z, Simon T, Wei S E, et al. Realtime Multi-person 2D Pose Estimation Using Part Affinity Fields[J]. 2016:1302-1310.
[2]Papandreou G, Zhu T, Kanazawa N, et al. Towards Accurate Multi-person Pose Estimation in the Wild[J]. 2017:3711-3719.
[3]Kevan Yuen and Mohan M. Trivedi. An Occluded Stacked Hourglass Approach to Facial Landmark Localization and Occlusion Estimation. IEEE2017，
[4]Adrian Bulat & Georgios Tzimiropoulos. How far are we from solving the 2D & 3D Face Alignment problem? (and a dataset of 230,000 3D facial landmarks，ICCV2017

