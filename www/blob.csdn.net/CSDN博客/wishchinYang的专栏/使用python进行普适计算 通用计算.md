# 使用python进行普适计算/通用计算 - wishchinYang的专栏 - CSDN博客
2017年12月28日 10:04:28[wishchin](https://me.csdn.net/wishchin)阅读数：423
# 使用[python进行普适计算](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97)---
## 目录
- [1什么是普适计算？](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E4.BB.80.E4.B9.88.E6.98.AF.E6.99.AE.E9.80.82.E8.AE.A1.E7.AE.97.EF.BC.9F)
- [2讨论组日程](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E8.AE.A8.E8.AE.BA.E7.BB.84.E6.97.A5.E7.A8.8B)
- [3文本数据处理](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E6.96.87.E6.9C.AC.E6.95.B0.E6.8D.AE.E5.A4.84.E7.90.86)- [3.1文本情感倾向判别
 2013-12-03](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E6.96.87.E6.9C.AC.E6.83.85.E6.84.9F.E5.80.BE.E5.90.91.E5.88.A4.E5.88.AB_2013-12-03)
- [3.2基于内容的关联帖子推荐
 2013-12-06](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E5.9F.BA.E4.BA.8E.E5.86.85.E5.AE.B9.E7.9A.84.E5.85.B3.E8.81.94.E5.B8.96.E5.AD.90.E6.8E.A8.E8.8D.90_2013-12-06)
- [3.3文本主题提取
 2013-12-24](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E6.96.87.E6.9C.AC.E4.B8.BB.E9.A2.98.E6.8F.90.E5.8F.96_2013-12-24)
- [4音频数据处理](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E9.9F.B3.E9.A2.91.E6.95.B0.E6.8D.AE.E5.A4.84.E7.90.86)- [4.1音乐数据预处理及风格分类
 2013-12-09](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E9.9F.B3.E4.B9.90.E6.95.B0.E6.8D.AE.E9.A2.84.E5.A4.84.E7.90.86.E5.8F.8A.E9.A3.8E.E6.A0.BC.E5.88.86.E7.B1.BB_2013-12-09)
- [4.2音乐分类及评估
 2013-12-12](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E9.9F.B3.E4.B9.90.E5.88.86.E7.B1.BB.E5.8F.8A.E8.AF.84.E4.BC.B0_2013-12-12)
- [5图像数据处理](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E5.9B.BE.E5.83.8F.E6.95.B0.E6.8D.AE.E5.A4.84.E7.90.86)- [5.1人物图像风格处理
 2013-12-13](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E4.BA.BA.E7.89.A9.E5.9B.BE.E5.83.8F.E9.A3.8E.E6.A0.BC.E5.A4.84.E7.90.86_2013-12-13)
- [5.2生物细胞图像计算处理
 2013-12-13](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E7.94.9F.E7.89.A9.E7.BB.86.E8.83.9E.E5.9B.BE.E5.83.8F.E8.AE.A1.E7.AE.97.E5.A4.84.E7.90.86_2013-12-13)
- [5.3手写数字的识别
 2013-12-16](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E6.89.8B.E5.86.99.E6.95.B0.E5.AD.97.E7.9A.84.E8.AF.86.E5.88.AB_2013-12-16)
- [6时间数据处理](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E6.97.B6.E9.97.B4.E6.95.B0.E6.8D.AE.E5.A4.84.E7.90.86)- [6.1股票时间序列建模
 2013-12-23](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E8.82.A1.E7.A5.A8.E6.97.B6.E9.97.B4.E5.BA.8F.E5.88.97.E5.BB.BA.E6.A8.A1_2013-12-23)
- [6.2股票共振网络建模
 2013-12-30](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E8.82.A1.E7.A5.A8.E5.85.B1.E6.8C.AF.E7.BD.91.E7.BB.9C.E5.BB.BA.E6.A8.A1_2013-12-30)
- [7地理数据处理](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E5.9C.B0.E7.90.86.E6.95.B0.E6.8D.AE.E5.A4.84.E7.90.86)- [7.1预测物种的分布2013-12-19](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E9.A2.84.E6.B5.8B.E7.89.A9.E7.A7.8D.E7.9A.84.E5.88.86.E5.B8.832013-12-19)
- [8网络分析、统计模型与机器学习](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E7.BD.91.E7.BB.9C.E5.88.86.E6.9E.90.E3.80.81.E7.BB.9F.E8.AE.A1.E6.A8.A1.E5.9E.8B.E4.B8.8E.E6.9C.BA.E5.99.A8.E5.AD.A6.E4.B9.A0)- [8.1PageRank算法
 2013-12-27](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#PageRank.E7.AE.97.E6.B3.95_2013-12-27)
- [8.2网络社区划分算法
 2012-12-30](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E7.BD.91.E7.BB.9C.E7.A4.BE.E5.8C.BA.E5.88.92.E5.88.86.E7.AE.97.E6.B3.95_2012-12-30)
- [8.3KNN分类器
 2013-12-05](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#KNN.E5.88.86.E7.B1.BB.E5.99.A8_2013-12-05)
- [8.4贝叶斯模型
 2013-12-03](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E8.B4.9D.E5.8F.B6.E6.96.AF.E6.A8.A1.E5.9E.8B_2013-12-03)
- [8.5logistic回归
 2013-12-12](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#logistic.E5.9B.9E.E5.BD.92_2013-12-12)
- [8.6流形学习
 2014-01-02](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E6.B5.81.E5.BD.A2.E5.AD.A6.E4.B9.A0_2014-01-02)
- [8.7人工神经网络
 2014-01-02](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E4.BA.BA.E5.B7.A5.E7.A5.9E.E7.BB.8F.E7.BD.91.E7.BB.9C_2014-01-02)
- [8.8K-means聚类
 2013-12-06](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#K-means.E8.81.9A.E7.B1.BB_2013-12-06)
- [8.9Affinity
 propagation 聚类 2014-01-02](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#Affinity_propagation_.E8.81.9A.E7.B1.BB_2014-01-02)
- [8.10最大似然参数估计
 2013-12-17](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E6.9C.80.E5.A4.A7.E4.BC.BC.E7.84.B6.E5.8F.82.E6.95.B0.E4.BC.B0.E8.AE.A1_2013-12-17)
- [8.11LDA模型
 2013-12-24](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#LDA.E6.A8.A1.E5.9E.8B_2013-12-24)
- [8.12SVM支持向量机
 2013-12-16](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#SVM.E6.94.AF.E6.8C.81.E5.90.91.E9.87.8F.E6.9C.BA_2013-12-16)
- [8.13HMM隐性马可夫模型
 2013-12-23](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#HMM.E9.9A.90.E6.80.A7.E9.A9.AC.E5.8F.AF.E5.A4.AB.E6.A8.A1.E5.9E.8B_2013-12-23)
- [9计算与模拟](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E8.AE.A1.E7.AE.97.E4.B8.8E.E6.A8.A1.E6.8B.9F)- [9.1各种分布及其产生机制
 2013-12-24](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E5.90.84.E7.A7.8D.E5.88.86.E5.B8.83.E5.8F.8A.E5.85.B6.E4.BA.A7.E7.94.9F.E6.9C.BA.E5.88.B6_2013-12-24)
- [9.2网络演化模拟](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E7.BD.91.E7.BB.9C.E6.BC.94.E5.8C.96.E6.A8.A1.E6.8B.9F)
- [9.3使用python语言表达分形与递归](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E4.BD.BF.E7.94.A8python.E8.AF.AD.E8.A8.80.E8.A1.A8.E8.BE.BE.E5.88.86.E5.BD.A2.E4.B8.8E.E9.80.92.E5.BD.92)
- [9.4使用pythony实现遗传算法](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E4.BD.BF.E7.94.A8pythony.E5.AE.9E.E7.8E.B0.E9.81.97.E4.BC.A0.E7.AE.97.E6.B3.95)
- [9.5元胞自动机
 2013-12-26](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E5.85.83.E8.83.9E.E8.87.AA.E5.8A.A8.E6.9C.BA_2013-12-26)
- [10热力学与进化论](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E7.83.AD.E5.8A.9B.E5.AD.A6.E4.B8.8E.E8.BF.9B.E5.8C.96.E8.AE.BA)- [10.1热力学第二定律与时间箭头
 2014-01-03](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E7.83.AD.E5.8A.9B.E5.AD.A6.E7.AC.AC.E4.BA.8C.E5.AE.9A.E5.BE.8B.E4.B8.8E.E6.97.B6.E9.97.B4.E7.AE.AD.E5.A4.B4_2014-01-03)
- [10.2沙堆、渗流、Ising与临界态
 2014-01-03](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E6.B2.99.E5.A0.86.E3.80.81.E6.B8.97.E6.B5.81.E3.80.81Ising.E4.B8.8E.E4.B8.B4.E7.95.8C.E6.80.81_2014-01-03)
- [11项目研发及讨论](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E9.A1.B9.E7.9B.AE.E7.A0.94.E5.8F.91.E5.8F.8A.E8.AE.A8.E8.AE.BA)
- [12课程参考资料](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E8.AF.BE.E7.A8.8B.E5.8F.82.E8.80.83.E8.B5.84.E6.96.99)- [12.1参考书目](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E5.8F.82.E8.80.83.E4.B9.A6.E7.9B.AE)
- [12.2参考论文](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E5.8F.82.E8.80.83.E8.AE.BA.E6.96.87)
- [12.3计算社会科学工具与使用：多贝公开课视频](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%99%AE%E9%80%82%E8%AE%A1%E7%AE%97#.E8.AE.A1.E7.AE.97.E7.A4.BE.E4.BC.9A.E7.A7.91.E5.AD.A6.E5.B7.A5.E5.85.B7.E4.B8.8E.E4.BD.BF.E7.94.A8.EF.BC.9A.E5.A4.9A.E8.B4.9D.E5.85.AC.E5.BC.80.E8.AF.BE.E8.A7.86.E9.A2.91)
## 什么是普适计算？
![Wolfram-alpha-logo.jpg](http://wiki.swarma.net/images/thumb/e/e6/Wolfram-alpha-logo.jpg/800px-Wolfram-alpha-logo.jpg)
普适计算的概念由[Stephen Wolfram](http://en.wikipedia.org/wiki/Stephen_Wolfram)正式提出，更早的概念在近代可以上溯到惠勒、冯诺依曼和图灵，更早的源头则与毕达哥拉斯学派和赫拉克利特。普适计算认为，计算是宇宙的本质。宇宙无时无刻不在进行计算，我们观察到的物理法则，其实是计算过程的信息论约束。所有的改变，包括自然界生物个体和种群的进化，都是在进行计算。
那么，什么是计算呢？计算，在最唯像的意义上说，就是数据的变形。有计算的地方就有等号，等号的左边是数据的输入，右边是数据的输出。要深刻理解普适计算，要从理解两点开始：（1）所有的数据本质上都是一致的，都可以被归结到同一种表达。例如现代计算机使用0和1，可以表达所有的数据：文本、图像、声音、触感。（2）所有的计算本质上都是一致的。根据图灵-丘奇定理，所有可以被执行的计算都可以被图灵机表达。与图灵机等价的形式还有元胞自动机和马科夫矩阵以及Lambda算子。这些所有的形式，虽然可以被用于制造不同的计算机，但效果是完全一样的：一台计算机的表现，一定可以被另外一台计算机模拟。这也是为什么存在那么多可以相互替代的计算机语言的原因。
Son of a gun, this is so heufpll!
## 讨论组日程
“
What I cannot create, I do not understand.
”
——Richard Feynman
por compartir tu experiencia. Demuestra que adaptar nuestros ciclos de sueÃ±o no sÃ³lo es posible, sino que no tiene por quÃ© tener efectos negativos en nuestra saenn.Biedvluido y sigue pasando por aquÃ­ aportando tus ideas :-)
## 文本数据处理
主要依赖包 [nltk](http://nltk.org/)
Vero vero vero tutto vero la sera ci si concede un pasto pif9 in sreneite0 dove e8 possibile #20;gustare&28281; invece che mangiare di corsa. Sono sicura che lo assaggerei con molto piacere questo piatto dal gusto un pf2 autunnale.Un abbraccio cara Giulia
 a presto
### 文本情感倾向判别 2013-12-03
[使用Naive
 Bayesian Model判别文本的情感倾向](http://wiki.swarma.net/index.php/%E8%B4%9D%E5%8F%B6%E6%96%AF%E5%AE%9A%E7%90%86%E4%B8%8E%E6%83%85%E6%84%9F%E5%88%86%E6%9E%90#Python_.E4.BB.A3.E7.A0.81.E5.8F.8A.E7.A4.BA.E4.BE.8B.EF.BC.9A.E4.BD.BF.E7.94.A8.E6.9C.B4.E7.B4.A0.E8.B4.9D.E5.8F.B6.E6.96.AF.E6.A8.A1.E5.9E.8B.E5.88.A4.E5.88.AB.E8.AE.BA.E5.9D.9B.E5.B8.96.E5.AD.90.E6.98.AF.E5.90.A6.E8.BE.B1.E9.AA.82.E6.80.A7)
![Parameter estimation.png](http://wiki.swarma.net/images/thumb/c/c9/Parameter_estimation.png/200px-Parameter_estimation.png)
### 基于内容的关联帖子推荐 2013-12-06
[本次主讲内容，使用K-means聚类](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8K-means%E8%81%9A%E7%B1%BB%E6%9D%A5%E8%BF%9B%E8%A1%8C%E9%97%AE%E7%AD%94%E7%B1%BB%E7%A4%BE%E5%8C%BA%E7%9A%84%E5%B8%96%E5%AD%90%E6%8E%A8%E8%8D%90)[实验数据](http://mlcomp.org/datasets/379)。
![Thread recommendation 1.png](http://wiki.swarma.net/images/thumb/5/5b/Thread_recommendation_1.png/250px-Thread_recommendation_1.png)
### [文本主题提取](http://wiki.swarma.net/index.php/%E6%96%87%E6%9C%AC%E4%B8%BB%E9%A2%98%E6%8F%90%E5%8F%96) 2013-12-24
使用了Latent Dirichlet allocation （LDA）模型[使用python画文字云](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E7%94%BB%E6%96%87%E5%AD%97%E4%BA%91)
![Cloud large.png](http://wiki.swarma.net/images/thumb/0/06/Cloud_large.png/200px-Cloud_large.png)
## 音频数据处理
### 音乐数据预处理及风格分类 2013-12-09
[使用KNN对处理基于FFT的音频数据](http://wiki.swarma.net/index.php/%E5%AF%B9%E9%9F%B3%E4%B9%90%E8%BF%9B%E8%A1%8C%E5%88%86%E7%B1%BB)
![Music classification 1.png](http://wiki.swarma.net/images/thumb/b/bc/Music_classification_1.png/200px-Music_classification_1.png)
### 音乐分类及评估 2013-12-12
[比较logistic回归与KNN的效果区别](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8logistic_regression%E5%AF%B9%E6%95%B0%E6%8D%AE%E8%BF%9B%E8%A1%8C%E5%88%86%E7%B1%BB#.E4.BD.BF.E7.94.A8logistic_regression.E5.A4.84.E7.90.86.E9.9F.B3.E4.B9.90.E6.95.B0.E6.8D.AE)
![Logistic classify5.png](http://wiki.swarma.net/images/thumb/1/11/Logistic_classify5.png/250px-Logistic_classify5.png)
## 图像数据处理
主要依赖包 [mahotas](https://mahotas.readthedocs.org/en/latest/)
### 人物图像风格处理 2013-12-13
[人物图像的各种处理：阈值处理、高斯过滤、背景虚化和波普艺术](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E5%81%9A%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86#.E5.AF.B9.E4.BA.BA.E7.89.A9.E5.9B.BE.E5.83.8F.E7.9A.84.E5.A4.84.E7.90.86)
![Lenna 3.jpeg](http://wiki.swarma.net/images/thumb/8/8f/Lenna_3.jpeg/180px-Lenna_3.jpeg)
### 生物细胞图像计算处理 2013-12-13
[计算细胞个数及活动区域划分](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E5%81%9A%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86#.E5.AF.B9.E7.94.9F.E7.89.A9.E7.BB.86.E8.83.9E.E5.9B.BE.E5.83.8F.E7.9A.84.E5.88.86.E6.9E.90)
![Dna 7.jpeg](http://wiki.swarma.net/images/thumb/f/f7/Dna_7.jpeg/200px-Dna_7.jpeg)
### 手写数字的识别 2013-12-16
[使用SVM对手写数字进行识别](http://wiki.swarma.net/index.php/%E6%89%8B%E5%86%99%E6%95%B0%E5%AD%97%E7%9A%84%E8%AF%86%E5%88%AB)
![Traindigits3.png](http://wiki.swarma.net/images/thumb/4/40/Traindigits3.png/200px-Traindigits3.png)
## 时间数据处理
### 股票时间序列建模 2013-12-23
[使用HMM建模股票价格波动](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8HMM%E5%BB%BA%E6%A8%A1%E8%82%A1%E7%A5%A8%E4%BB%B7%E6%A0%BC%E6%B3%A2%E5%8A%A8)
![Stockmarket HMM 1.png](http://wiki.swarma.net/images/thumb/b/b3/Stockmarket_HMM_1.png/200px-Stockmarket_HMM_1.png)
### 股票共振网络建模 2013-12-30
[分析股票价格共振网络的社区](http://wiki.swarma.net/index.php/%E5%88%86%E6%9E%90%E8%82%A1%E7%A5%A8%E4%BB%B7%E6%A0%BC%E5%85%B1%E6%8C%AF%E7%BD%91%E7%BB%9C)
![Stockmarket correlation network 1.png](http://wiki.swarma.net/images/thumb/0/05/Stockmarket_correlation_network_1.png/250px-Stockmarket_correlation_network_1.png)
## 地理数据处理
主要依赖包 [Basemap](http://matplotlib.org/basemap/)
### 预测物种的分布2013-12-19
[预测物种的空间分布](http://wiki.swarma.net/index.php/%E9%A2%84%E6%B5%8B%E7%89%A9%E7%A7%8D%E7%9A%84%E7%A9%BA%E9%97%B4%E5%88%86%E5%B8%83)
![Species distribution 1.png](http://wiki.swarma.net/images/thumb/b/b5/Species_distribution_1.png/250px-Species_distribution_1.png)
## 网络分析、统计模型与机器学习
主要依赖包 [sklearn](http://scikit-learn.org/stable/)
### [PageRank算法](http://wiki.swarma.net/index.php/Page_rank_%E7%AE%97%E6%B3%95) 2013-12-27
![Pagerank 1.png](http://wiki.swarma.net/images/thumb/c/ca/Pagerank_1.png/200px-Pagerank_1.png)
### [网络社区划分算法](http://wiki.swarma.net/index.php/%E7%BD%91%E7%BB%9C%E7%A4%BE%E5%8C%BA%E5%88%92%E5%88%86%E7%AE%97%E6%B3%95) 2012-12-30
![Community figure 1.png](http://wiki.swarma.net/images/thumb/f/f3/Community_figure_1.png/250px-Community_figure_1.png)
### [KNN分类器](http://wiki.swarma.net/index.php/Python%E8%81%9A%E7%B1%BB%E7%AE%97%E6%B3%95%E7%A4%BA%E4%BE%8B) 2013-12-05
![Wheatseed clustering3.png](http://wiki.swarma.net/images/thumb/1/12/Wheatseed_clustering3.png/200px-Wheatseed_clustering3.png)
### [贝叶斯模型](http://wiki.swarma.net/index.php/%E8%B4%9D%E5%8F%B6%E6%96%AF%E5%AE%9A%E7%90%86%E4%B8%8E%E6%83%85%E6%84%9F%E5%88%86%E6%9E%90) 2013-12-03
![Parameter estimation.png](http://wiki.swarma.net/images/thumb/c/c9/Parameter_estimation.png/200px-Parameter_estimation.png)
### [logistic回归](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8logistic_regression%E5%AF%B9%E6%95%B0%E6%8D%AE%E8%BF%9B%E8%A1%8C%E5%88%86%E7%B1%BB) 2013-12-12
![Logistic classify3.png](http://wiki.swarma.net/images/thumb/8/8a/Logistic_classify3.png/250px-Logistic_classify3.png)
### [流形学习](http://wiki.swarma.net/index.php/%E6%B5%81%E5%BD%A2%E5%AD%A6%E4%B9%A0) 2014-01-02
![ML SE figure.png](http://wiki.swarma.net/images/thumb/8/84/ML_SE_figure.png/200px-ML_SE_figure.png)
### [人工神经网络](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E5%BB%BA%E6%A8%A1%E4%BA%BA%E5%B7%A5%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C%E5%8F%8A%E8%BF%9B%E8%A1%8C%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0) 2014-01-02
![CNN2.png](http://wiki.swarma.net/images/thumb/3/33/CNN2.png/200px-CNN2.png)
### [K-means聚类](http://wiki.swarma.net/index.php/K-means%E8%81%9A%E7%B1%BB) 2013-12-06
![Kmeans clustering 1.png](http://wiki.swarma.net/images/thumb/4/45/Kmeans_clustering_1.png/200px-Kmeans_clustering_1.png)
### [Affinity propagation 聚类](http://wiki.swarma.net/index.php/Affinity_propagation_%E8%81%9A%E7%B1%BB) 2014-01-02
![Affinitypropagation figure 6.png](http://wiki.swarma.net/images/thumb/f/fe/Affinitypropagation_figure_6.png/200px-Affinitypropagation_figure_6.png)
### [最大似然参数估计](http://wiki.swarma.net/index.php/%E6%9C%80%E5%A4%A7%E4%BC%BC%E7%84%B6%E5%8F%82%E6%95%B0%E4%BC%B0%E8%AE%A1) 2013-12-17
![Parameter estimation3.png](http://wiki.swarma.net/images/thumb/f/f5/Parameter_estimation3.png/300px-Parameter_estimation3.png)
### [LDA模型](http://wiki.swarma.net/index.php/%E6%96%87%E6%9C%AC%E4%B8%BB%E9%A2%98%E6%8F%90%E5%8F%96) 2013-12-24
![LDA figure 1.png](http://wiki.swarma.net/images/thumb/4/4d/LDA_figure_1.png/250px-LDA_figure_1.png)
### [SVM支持向量机](http://wiki.swarma.net/index.php/SVM%E6%94%AF%E6%8C%81%E5%90%91%E9%87%8F%E6%9C%BA) 2013-12-16
![SVMClassifier3.jpg](http://wiki.swarma.net/images/thumb/7/7b/SVMClassifier3.jpg/250px-SVMClassifier3.jpg)
### [HMM隐性马可夫模型](http://wiki.swarma.net/index.php/HMM%E9%9A%90%E6%80%A7%E9%A9%AC%E5%8F%AF%E5%A4%AB%E6%A8%A1%E5%9E%8B) 2013-12-23
![HMM2.png](http://wiki.swarma.net/images/thumb/2/23/HMM2.png/200px-HMM2.png)
## 计算与模拟
### [各种分布及其产生机制](http://wiki.swarma.net/index.php/%E5%90%84%E7%A7%8D%E5%88%86%E5%B8%83%E5%8F%8A%E5%85%B6%E4%BA%A7%E7%94%9F%E6%9C%BA%E5%88%B6) 2013-12-24
![Distributions figure 4.png](http://wiki.swarma.net/images/thumb/5/50/Distributions_figure_4.png/300px-Distributions_figure_4.png)
### [网络演化模拟](http://wiki.swarma.net/index.php/%E7%BD%91%E7%BB%9C%E6%BC%94%E5%8C%96%E6%A8%A1%E6%8B%9F)
### [使用python语言表达分形与递归](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E8%AF%AD%E8%A8%80%E8%A1%A8%E8%BE%BE%E5%88%86%E5%BD%A2%E4%B8%8E%E9%80%92%E5%BD%92)
### [使用pythony实现遗传算法](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8pythony%E5%AE%9E%E7%8E%B0%E9%81%97%E4%BC%A0%E7%AE%97%E6%B3%95)
### [元胞自动机](http://wiki.swarma.net/index.php/Python%E7%9A%84%E5%85%83%E8%83%9E%E8%87%AA%E5%8A%A8%E6%9C%BA%E6%A8%A1%E6%8B%9F) 2013-12-26
![CA figure 2.png](http://wiki.swarma.net/images/thumb/9/9d/CA_figure_2.png/200px-CA_figure_2.png)
## 热力学与进化论
### 热力学第二定律与时间箭头 2014-01-03
关于热力学与时间箭头，我们可以参考两个jake写的ppt：
1. [秩序从哪里来](http://wenku.baidu.com/view/242c624ce45c3b3567ec8b4f.html)
2. [“熵”与“流”](http://www.docin.com/p-34009500.html)
### 沙堆、渗流、Ising与临界态 2014-01-03
[渗流模型](http://wiki.swarma.net/index.php/%E6%B8%97%E6%B5%81%E6%A8%A1%E5%9E%8B)
[ISING模型](http://wiki.swarma.net/index.php/ISING%E6%A8%A1%E5%9E%8B)
[使用python制造渗流模型](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8python%E5%88%B6%E9%80%A0%E6%B8%97%E6%B5%81%E6%A8%A1%E5%9E%8B)
## 项目研发及讨论
@jingjing笔记：[1126代码笔记](https://app.yinxiang.com/shard/s17/sh/b41d2970-adcd-4a98-8d92-d35f31e85272/c3f2f5c994727583071c34c23d7863d4)[1128笔记](https://app.yinxiang.com/shard/s17/sh/d9a65bde-a33f-4dc2-bbe8-414a4430cdef/de34ecf48ba9626086756f2fa160d5c0)
@LDH问题:可否使用热力图（等高线图）展示因子分析的结果？示例：
- 
![20120827103004b64cd.jpg](http://wiki.swarma.net/images/thumb/b/bc/20120827103004b64cd.jpg/120px-20120827103004b64cd.jpg)
- 
![201208271031202f169.jpg](http://wiki.swarma.net/images/thumb/4/4a/201208271031202f169.jpg/120px-201208271031202f169.jpg)
- 
![20111214080916 161505.jpg](http://wiki.swarma.net/images/thumb/3/3c/20111214080916_161505.jpg/120px-20111214080916_161505.jpg)
- 
![20120827102836755c9.jpg](http://wiki.swarma.net/images/thumb/1/12/20120827102836755c9.jpg/120px-20120827102836755c9.jpg)
@计算士回答：示例图 ![Contourf plot puzzle.png](http://wiki.swarma.net/images/thumb/3/32/Contourf_plot_puzzle.png/200px-Contourf_plot_puzzle.png)[示例图背后的代码](http://wiki.swarma.net/index.php/Python#.E7.AD.89.E9.AB.98.E7.BA.BF.E7.83.AD.E5.9B.BE)[其他的配色方案](https://gist.github.com/endolith/2719900)[测试不同函数的等高线效果](http://www.flashandmath.com/advanced/contours/combo.html)
@LDH问题:可否使用热力图（等高线图）展示双自变量回归分析的结果？
@LDH问题:K-means聚类的结果是否可以固定？
@计算士回答：可适用affinity propagation聚类。
@LDH问题:可否自动化寻找数据的多项式拟合？
@计算士回答：可以使用[SVM回归](http://scikit-learn.org/stable/auto_examples/svm/plot_svm_regression.html)或者使用[遗传算法](http://wiki.swarma.net/index.php/%E4%BD%BF%E7%94%A8pythony%E5%AE%9E%E7%8E%B0%E9%81%97%E4%BC%A0%E7%AE%97%E6%B3%95)来拟合数据。
@LDH问题:一族多项式曲线能否画出光滑的过渡效果？
## 课程参考资料
### 参考书目
机器学习与数据挖掘
- 
![Programing collective intelligence book 1.jpg](http://wiki.swarma.net/images/thumb/4/43/Programing_collective_intelligence_book_1.jpg/91px-Programing_collective_intelligence_book_1.jpg)
- 
![Python for data analysis.jpg](http://wiki.swarma.net/images/thumb/2/2b/Python_for_data_analysis.jpg/91px-Python_for_data_analysis.jpg)
- 
![Nltk python.jpg](http://wiki.swarma.net/images/thumb/4/44/Nltk_python.jpg/91px-Nltk_python.jpg)
- 
![Web puzzle.jpg](http://wiki.swarma.net/images/thumb/8/86/Web_puzzle.jpg/90px-Web_puzzle.jpg)
- 
![Cover Building.Machine.Learning.Systems.with.Python.jpg](http://wiki.swarma.net/images/thumb/8/8f/Cover_Building.Machine.Learning.Systems.with.Python.jpg/97px-Cover_Building.Machine.Learning.Systems.with.Python.jpg)
- 
![大数据时代.jpg](http://wiki.swarma.net/images/thumb/0/08/%E5%A4%A7%E6%95%B0%E6%8D%AE%E6%97%B6%E4%BB%A3.jpg/98px-%E5%A4%A7%E6%95%B0%E6%8D%AE%E6%97%B6%E4%BB%A3.jpg)
- 
![Hackers and painters.jpg](http://wiki.swarma.net/images/thumb/6/6c/Hackers_and_painters.jpg/85px-Hackers_and_painters.jpg)
- 
![Allofstatistics.jpg](http://wiki.swarma.net/images/thumb/f/f9/Allofstatistics.jpg/74px-Allofstatistics.jpg)
复杂网络、分形与复杂科学
- 
![Linkbook.jpg](http://wiki.swarma.net/images/thumb/3/3c/Linkbook.jpg/80px-Linkbook.jpg)
- 
![Complexbook.jpg](http://wiki.swarma.net/images/thumb/8/89/Complexbook.jpg/84px-Complexbook.jpg)
- 
![Hiddenorder.jpg](http://wiki.swarma.net/images/thumb/7/71/Hiddenorder.jpg/81px-Hiddenorder.jpg)
- 
![Emergence.jpg](http://wiki.swarma.net/images/thumb/3/37/Emergence.jpg/78px-Emergence.jpg)
- 
![Criticalmass.jpg](http://wiki.swarma.net/images/thumb/8/8d/Criticalmass.jpg/81px-Criticalmass.jpg)
- 
![Out of control 1.jpg](http://wiki.swarma.net/images/thumb/6/6b/Out_of_control_1.jpg/76px-Out_of_control_1.jpg)
- 
![Thinking complexity book 1.jpg](http://wiki.swarma.net/images/thumb/8/8c/Thinking_complexity_book_1.jpg/90px-Thinking_complexity_book_1.jpg)
- 
![大自然的分形几何学.jpg](http://wiki.swarma.net/images/thumb/f/fb/%E5%A4%A7%E8%87%AA%E7%84%B6%E7%9A%84%E5%88%86%E5%BD%A2%E5%87%A0%E4%BD%95%E5%AD%A6.jpg/82px-%E5%A4%A7%E8%87%AA%E7%84%B6%E7%9A%84%E5%88%86%E5%BD%A2%E5%87%A0%E4%BD%95%E5%AD%A6.jpg)
- 
![天遇.jpg](http://wiki.swarma.net/images/thumb/6/60/%E5%A4%A9%E9%81%87.jpg/82px-%E5%A4%A9%E9%81%87.jpg)
人工智能与普适计算
- 
![Newkindofsience.jpg](http://wiki.swarma.net/images/thumb/2/21/Newkindofsience.jpg/94px-Newkindofsience.jpg)
- 
![Thepatternonthestone.jpg](http://wiki.swarma.net/images/thumb/5/5d/Thepatternonthestone.jpg/78px-Thepatternonthestone.jpg)
- 
![GEB.jpg](http://wiki.swarma.net/images/thumb/9/9c/GEB.jpg/84px-GEB.jpg)
- 
![Selfreplicateautomata.jpg](http://wiki.swarma.net/images/thumb/b/b6/Selfreplicateautomata.jpg/120px-Selfreplicateautomata.jpg)
- 
![皇帝新脑.jpg](http://wiki.swarma.net/images/thumb/1/16/%E7%9A%87%E5%B8%9D%E6%96%B0%E8%84%91.jpg/81px-%E7%9A%87%E5%B8%9D%E6%96%B0%E8%84%91.jpg)
- 
![通向实在之路.jpg](http://wiki.swarma.net/images/thumb/6/6e/%E9%80%9A%E5%90%91%E5%AE%9E%E5%9C%A8%E4%B9%8B%E8%B7%AF.jpg/83px-%E9%80%9A%E5%90%91%E5%AE%9E%E5%9C%A8%E4%B9%8B%E8%B7%AF.jpg)
数据可视化
- 
![Beautiful data book 1.jpg](http://wiki.swarma.net/images/thumb/5/54/Beautiful_data_book_1.jpg/91px-Beautiful_data_book_1.jpg)
- 
![Processing book 1.jpg](http://wiki.swarma.net/images/thumb/2/27/Processing_book_1.jpg/93px-Processing_book_1.jpg)
- 
![Beautiful visualization 1.jpg](http://wiki.swarma.net/images/thumb/c/cf/Beautiful_visualization_1.jpg/91px-Beautiful_visualization_1.jpg)
### 参考论文
[Data Science and Prediction](http://m.cacm.acm.org/magazines/2013/12/169933-data-science-and-prediction/fulltext)
[D3 : Data-Driven Documents](http://vis.stanford.edu/files/2011-D3-InfoVis.pdf)
[Distilling Free-Form Natural Laws from Experimental Data](http://www.uvm.edu/~cmplxsys/newsevents/pdfs/2009/schmidt2009a.pdf)
[reCAPTCHA: Human-Based Character Recognition via Web](http://users.df.uba.ar/marcos/reCAPTCHA.pdf)
[机器学习基本模型](http://wiki.swarma.net/index.php?title=%E4%BA%BA%E5%B7%A5%E6%99%BA%E8%83%BD%E4%B8%8E%E6%9C%BA%E5%99%A8%E5%AD%A6%E4%B9%A0)
[Python matplotlib gallery](http://pandas.pydata.org/pandas-docs/stable/visualization.html)
### 计算社会科学工具与使用：多贝公开课视频
[第一讲：幂律分布的最大似然估计](http://www.duobei.com/course/4558522382)
[第二讲：网络社区的增长和标度对称原理](http://www.duobei.com/course/6434247483)
[第三讲：使用R分析网络数据及实现可视化（上）](http://www.duobei.com/course/9651542425)
[第三讲：使用R分析网络数据及实现可视化（下）](http://www.duobei.com/course/1648481161)
[第四讲：使用Mathematica分析互联网上的图像数据](http://www.duobei.com/course/2720040878)
[第五讲：使用Python抓取及分析互联网数据](http://www.duobei.com/course/3566672075)
[第六讲：使用Netlogo进行多主体交互模拟](http://www.duobei.com/course/9763334311)
[第七讲：使用Processing实现网络可视化](http://www.duobei.com/course/3074407287)
Great article! It is definitely important to not allow you social media preeplos/iagfs to be dormant or even idle for any length of time. It really is at least a daily process!
