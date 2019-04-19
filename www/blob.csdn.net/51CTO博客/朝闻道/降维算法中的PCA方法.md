# 降维算法中的PCA方法-朝闻道-51CTO博客
1 主成分分析
（Principal Component Analysis，PCA）
2 线性判别分析
(Linear Discriminant Analysis, LDA)
研究背景
基本知识介绍
经典方法介绍
总结讨论
问题的提出
地理系统是多要素的复杂系统。在地理学研究中，多变量问题是经常会遇到的。变量太多，无疑会增加分析问题的难度与复杂性，而且在许多实际问题中，多个变量之间是具有一定的相关关系的。
因此，人们会很自然地想到，能否在相关分析的基础上，用较少的新变量代替原来较多的旧变量，而且使这些较少的新变量尽可能多地保留原来变量所反映的信息？
降维的动机
原始观察空间中的样本具有极大的信息冗余
样本的高维数引发分类器设计的“维数灾难”
数据可视化、特征提取、分类与聚类等任务需求
![降维算法中的PCA方法](https://s1.51cto.com/images/blog/201804/13/f26e9d1fbf612e18fde98b84605c0948.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![降维算法中的PCA方法](https://s1.51cto.com/images/blog/201804/13/e3d9a0f01f52df6e1d5a5cb9fa6bb8bd.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
在进行祝成分分析后后，竟然以97.4%的精度，用三个变量取代了原来的17个变量。
线性降维
通过特征的线性组合来降维
本质上是把数据投影到低维线性子空间
线性方法相对比较简单且容易计算
代表方法
主成分分析(PCA)
线性判别分析(LDA)
多维尺度变换(MDS)
主成分分析(PCA) [Jolliffe, 1986]
降维目的：寻找能够保持采样数据方差的最佳投影子空间
求解方法：对样本的散度矩阵进行特征值分解, 所求子空间为经过样本均值, 以最大特征值所对应的特征向量为方向的子空间![降维算法中的PCA方法](https://s1.51cto.com/images/blog/201804/13/c25aacb3a182ddc43fcff26009a7202e.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
主成分分析(PCA) [Jolliffe, 1986]
PCA对于椭球状分布的样本集有很好的效果, 学习所得的主方向就是椭球的主轴方向. 
PCA 是一种非监督的算法, 能找到很好地代表所有样本的方向, 但这个方向对于分类未必是最有利的
![降维算法中的PCA方法](https://s1.51cto.com/images/blog/201804/13/c0c5a0d027ef85242e5578845954a565.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
线性判别分析(LDA) [Fukunaga, 1991]
降维目的：寻找最能把两类样本分开的投影直线，使投影后两类样本的均值之差与投影样本的总类散度的比值最大
求解方法：经过推导把原问题转化为关于样本集总类内散度矩阵和总类间散度矩阵的广义特征值问题
![降维算法中的PCA方法](https://s1.51cto.com/images/blog/201804/13/c0826f561c6c164e72aa51d9113b8c5a.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
线性降维方法比较
主成分分析 (PCA) [Jolliffe, 1986]
线性判别分析 (LDA) [Fukunaga, 1991]
![降维算法中的PCA方法](https://s1.51cto.com/images/blog/201804/13/1969d1d6093770485b689dfa997f3969.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
线性降维方法的不足
![降维算法中的PCA方法](https://s1.51cto.com/images/blog/201804/13/747e3d958edd2ab191e123f924c1619a.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
原始数据无法表示为特征的简单线性组合
比如：PCA无法表达Helix曲线流形
一、主成分分析的基本原理
假定有n个地理样本，每个样本共有p个变量，构成一个n×p 阶的地理数据矩阵
![降维算法中的PCA方法](https://s1.51cto.com/images/blog/201804/13/806cd0853bceb61b025e5726a2878676.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
当p 较大时，在p 维空间中考察问题比较麻烦。为了克服这一困难，就需要进行降维处理，即用较少的几个综合指标代替原来较多的变量指标，而且使这些较少的综合指标既能尽量多地反映原来较多变量指标所反映的信息，同时它们之间又是彼此独立的。
定义：记x1，x2，…，xP为原变量指标，z1，z2，…，zm（m≤p）为新变量指标
![降维算法中的PCA方法](https://s1.51cto.com/images/blog/201804/13/8a265ccba82a717609aaab76c135af81.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
系数lij的确定原则： 
① zi与zj（i≠j；i，j=1，2，…，m）相互无关；
②z1是x1，x2，…，xP的一切线性组合中方差最大者，z2是与z1不相关的x1，x2，…，xP的所有线性组合中方差最大者；
……
zm是与z1，z2，……，zm－1都不相关的x1，x2，…xP，  的所有线性组合中方差最大者。
则新变量指标z1，z2，…，zm分别称为原变量指标x1，x2，…，xP的第一，第二，…，第m主成分。
从以上的分析可以看出，主成分分析的实质就是确定原来变量xj（j=1，2 ，…， p）在诸主成分zi（i=1，2，…，m）上的荷载 lij（ i=1，2，…，m； j=1，2 ，…，p）。
从数学上容易知道，从数学上可以证明，它们分别是的相关矩阵的m个较大的特征值所对应的特征向量。 
` 二、计算步骤 `
1）构建p*n阶的变量矩阵
2）将p*n阶的变量矩阵X的每一行（代表一个属性字段）进行标准化
3）求出协方差矩阵C
4）求出协方差矩阵的特征值及对应的特征向量
5）将特征向量按对应特征值大小从上到下按行排列成矩阵，取前k列组成矩阵P
6）Y=XP即为降维到k维后的数据
