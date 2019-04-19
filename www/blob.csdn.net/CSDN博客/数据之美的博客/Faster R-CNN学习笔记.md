# Faster R-CNN学习笔记 - 数据之美的博客 - CSDN博客
2017年06月22日 13:31:26[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：180
个人分类：[深度学习/机器学习																[通用物体检测](https://blog.csdn.net/oppo62258801/article/category/6840016)](https://blog.csdn.net/oppo62258801/article/category/6739087)
R-CNN学习笔记：http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/xzzppp/article/details/51345742
Fast R-CNN学习笔记：http://blog.csdn[.net](http://lib.csdn.net/base/dotnet)/xzzppp/article/details/51377771
1、简介
       Faster R-CNN（其中R对应于“Region(区域)” ）是基于[深度学习](http://lib.csdn.net/base/deeplearning)R-CNN系列目标检测最好的方法。使用VOC2007+2012训练集训练，VOC2007[测试](http://lib.csdn.net/base/softwaretest)集测试mAP达到73.2%，目标检测的速度可以达到每秒5帧。
       技术上将RPN网络和Fast R-CNN网络结合到了一起，将RPN获取到的proposal直接连到ROI pooling层，是一个CNN网络实现端到端目标检测的框架。
2、R-CNN系列方法对比
![](https://img-blog.csdn.net/20160604083845786)
3、Fast R-CNN目标检测系统框图
![](https://img-blog.csdn.net/20160604083926161)
4、Faster R-CNN
卷积阶段
![](https://img-blog.csdn.net/20160604084153600)
　　RPN的核心思想是使用卷积神经网络直接产生region proposal，使用的方法本质上就是滑动窗口。RPN网络结构图如上所示（ZF模型:256维），假设给定600*1000的输入图像，经过卷积操作得到最后一层的卷积feature map（大小约为40*60），最后一层卷积层共有256个feature map。
Region ProposalNetwork(RPN)
![](https://img-blog.csdn.net/20160604084245553)
　　在这个特征图上使用3*3的卷积核（滑动窗口）与特征图进行卷积，那么这个3*3的区域卷积后可以获得一个256维的特征向量。因为这个3*3的区域上，每一个特征图上得到一个1维向量，256个特性图即可得到256维特征向量。
       3*3滑窗中心点位置，对应预测输入图像3种尺度（128,256,512），3种长宽比（1:1,1:2,2:1）的regionproposal，这种映射的机制称为anchor，产生了k=9个anchor。即每个3*3区域可以产生9个region proposal。所以对于这个40*60的feature map，总共有约20000(40*60*9)个anchor，也就是预测20000个region proposal。
       后边接入到两个全连接层，即cls layer和reglayer分别用于分类和边框回归。clslayer包含2个元素，用于判别目标和非目标的估计概率。reglayer包含4个坐标元素（x,y,w,h），用于确定目标位置。cls：正样本，与真实区域重叠大于0.7，负样本，与真实区域重叠小于0.3。reg：返回区域位置。
      最后根据region proposal得分高低，选取前300个region proposal，作为Fast R-CNN的输入进行目标检测。
5、训练4步阶段：
(1)使用在ImageNet上预训练的模型初始化RPN网络参数，微调RPN网络；
(2) 使用(1)中RPN网络提取region proposal训练Fast R-CNN网络，也用ImageNet上预训练的模型初始化该网络参数；（现在看来两个网络相对独立）
(3) 使用(2)的Fast R-CNN网络重新初始化RPN, 固定卷积层进行微调，微调RPN网络；
(4) 固定(2)中Fast R-CNN的卷积层，使用(3)中RPN提取的region proposal对Fast R-CNN网络进行微调。
