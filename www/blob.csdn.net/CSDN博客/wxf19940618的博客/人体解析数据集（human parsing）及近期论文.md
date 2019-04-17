# 人体解析数据集（human parsing）及近期论文 - wxf19940618的博客 - CSDN博客





2018年11月02日 21:35:44[rxdffz](https://me.csdn.net/wxf19940618)阅读数：553








**人体解析****-human parsing**
- 研究目标与意义

人体解析是指将在图像中捕获的人分割成多个语义上一致的区域，例如， 身体部位和衣物。作为一种细粒度的语义分割任务，它比仅是寻找人体轮廓的人物分割更具挑战性。 人体解析对于以人为中心的分析非常重要，并且具有许多工业上的应用，例如，虚拟现实，视频监控和人类行为分析等等。

![](https://img-blog.csdnimg.cn/20181102213456190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)
- 数据集
|数据集|人数/张|图像总数|训练集|验证集|测试集|分割种类|
|----|----|----|----|----|----|----|
|Fashionista|1|685|456|-|299|56|
|Person-Part|2.2|3533|1716|-|1817|7|
|ATR|1|17700|16000|700|1000|18|
|LIP|1|50462|30462|10000|10000|20|
|MHP|3(平均)|4980|3000|1000|980|19|
|CIHP|3.4|38,280|28,280|5,000|5,000|20|



Fashionista 数据集

论文：Parsing Clothing in Fashion Photographs

论文地址：[http://www.tamaraberg.com/papers/parsingclothing.pdf](http://www.tamaraberg.com/papers/parsingclothing.pdf)

![](https://img-blog.csdnimg.cn/20181102213456284.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

Person-part 数据集

论文：Detect What You Can: Detecting and Representing Objects using Holistic Models and Body Parts

论文地址：[http://www.stat.ucla.edu/~xianjie.chen/paper/Chen14cvpr.pdf](http://www.stat.ucla.edu/~xianjie.chen/paper/Chen14cvpr.pdf)

数据集地址：http://www.stat.ucla.edu/~xianjie.chen/pascal_part_dataset/pascal_

part.html

![](https://img-blog.csdnimg.cn/20181102213456243.png)

ATR 数据集

论文：Deep Human Parsing with Active Template Regression

论文地址：https://arxiv.org/pdf/1503.02391.pdf

![](https://img-blog.csdnimg.cn/20181102213456344.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

LIP数据集

论文：Look into Person: Self-supervised Structure-sensitive Learning and A New Benchmark for Human Parsing

论文地址：https://arxiv.org/pdf/1703.05446.pdf

数据集地址：[http://hcp.sysu.edu.cn/lip](http://hcp.sysu.edu.cn/lip)

代码地址：https://github.com/Engineering-Course/LIP_SSL

![](https://img-blog.csdnimg.cn/20181102213456375.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

MHP数据集

论文：Multi-Human Parsing in the Wild

论文地址：[https://arxiv.org/pdf/1705.07206.pdf](https://arxiv.org/pdf/1705.07206.pdf)

数据集地址：https://lv-mhp.github.io/dataset

![](https://img-blog.csdnimg.cn/20181102213456383.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)



CIHP 数据集

论文：Instance-level Human Parsing via Part Grouping Network

论文地址：http://openaccess.thecvf.com/content_ECCV_2018/papers/Ke_

Gong_Instance-level_Human_Parsing_ECCV_2018_paper.pdf

数据集地址：http://sysu-hcp.net/lip/overview.php

代码地址：[http://sysu-hcp.net/lip/](http://sysu-hcp.net/lip/)

![](https://img-blog.csdnimg.cn/20181102213456303.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)
- 近年论文

2018-ECCV-Mutual Learning to Adapt for Joint Human Parsing and Pose Estimation

MuLA用于联合人体解析和姿态估计，通过反复利用其并行任务中的指导信息来预测动态任务特定的模型参数

2018-ECCV-Instance-level Human Parsing via Part Grouping Network

PGN在一次传递中解析图像中的多个人，将实例级人类解析重新定义为两个可以通过统一网络共同学习和相互提炼的双关联子任务：1）用于将每个像素指定为人体部位（例如，面部，手臂）的语义分割; 2）实例感知边缘检测，以将语义部位分组为不同的人物实例

2018-ECCV-Macro-Micro Adversarial Network for Human Parsing

MMAN有两个判别器，一个Macro D用于低分辨率标签图并对语义不一致进行惩罚，例如错位的身体部位。另一个Micro D专注于高分辨率标签图的多个patch，以解决局部不一致性，例如模糊和孔洞

2018-CVPR-Weakly and Semi Supervised Human Body Part Parsing via Pose-Guided Knowledge Transfer

提出了一种获得训练数据的新方法，可以使用人体关键点的数据来生成人体部位解析数据。主要想法是利用人类之间的形态相似性，将一个人的部位解析结果传递给具有相似姿势的另一个人

2017-CVPR-Look Into Person: Self-Supervised Structure-Sensitive Learning and a New Benchmark for Human Parsing

不引入额外信息，可以用自生成的人类关节的信息反过来指导人体解析

2017-CVPR-Learning Adaptive Receptive Fields for Deep Image Parsing Network

一种自动调节深度图像解析网络中感受野的新方法，在网络主干中使用两个仿射变换层并在特征映射上运行

2017-CVPR- Multiple-Human Parsing in the Wild

MH-Parser借助新的Graph-GAN模型以自下而上的方式同时生成全局解析和人物解析

2015-ICCV-Human Parsing with Contextualized Convolutional Neural Network

Co-CNN架构，将多级图像上下文集成到一个统一的网络中



