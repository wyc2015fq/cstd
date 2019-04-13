
# 基于CNN的年龄和性别分类 - 小花生的博客 - CSDN博客


2018年08月06日 19:39:23[Peanut_范](https://me.csdn.net/u013841196)阅读数：437


《Age and Gender Classification using Convolutional Neural Networks》
2015，Gil Levi and Tal Hassner，使用卷积神经网络来对年龄和性别进行预测。
**年龄分类：**年龄问题既可以当成年龄组的分类问题，也可以当成回归问题。
传统的比较好的方法LBP+SVM。
**1.网络结构：**
![这里写图片描述](https://img-blog.csdn.net/20180806193401816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180806193401816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
网络包含：3个卷积层，2个全连接层。使用了类似Alexnet的网络结构，层数比较小，这样可以避免过拟合。对于年龄的识别，论文分成了8个年龄段，相当于8分类模型；然后对于性别识别自然而然是二分类问题。
[
](https://img-blog.csdn.net/20180806193401816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**2.网络训练：**
（1）初始化参数：权重初始化方法采用标准差为0.01，均值为0的高斯正太分布。
（2）网络训练：采用dropout，来限制过拟合。drop out比例采用0.5，还有就是数据扩充，数据扩充石通过输入256*256的图片，然后进行随机裁剪，裁剪为227*227的图片。
（3）训练方法采用，随机梯度下降法，min-batch 大小选择50，学习率大小0.001，然后当迭代到10000次以后，把学习率调为0.0001。
（4）结果预测：预测方法采用输入一张256*256的图片，一种方法是中心裁剪，另一种是裁剪5张图片，其中四张图片的裁剪方法分别采用以256*256的图片的4个角为点点，进行裁剪。然后最后一张，以人脸的中心为基点进行裁剪。然后对这5张图片进行预测，最后对预测结果进行平均。
**数据集：**
![这里写图片描述](https://img-blog.csdn.net/20180806193435424?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**数据标签：**
age_list=[‘(0, 2)’,’(4, 6)’,’(8, 12)’,’(15, 20)’,’(25, 32)’,’(38, 43)’,’(48, 53)’,’(60, 100)’]
gender_list=[‘Male’,’Female’]
---
**评价标准：**
![](https://img-blog.csdn.net/20180806193503313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[  ](https://img-blog.csdn.net/20180806193503313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注：在机器学习领域中，混淆矩阵（confusion matrix）是一种评价分类模型好坏的形象化展示工具。其中，矩阵的每一列表示的是模型预测的样本情况；矩阵的每一行表示的样本的真实情况。
[
](https://img-blog.csdn.net/20180806193503313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---

###### 注：博众家之所长，集群英之荟萃。
[
						](https://img-blog.csdn.net/20180806193503313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
	](https://img-blog.csdn.net/20180806193503313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
