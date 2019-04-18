# 最新论文阅读（26）--Self-Normalizing Neural Networks - wydbyxr的博客 - CSDN博客
2018年06月10日 20:32:35[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：49
个人分类：[深度学习论文阅读](https://blog.csdn.net/wydbyxr/article/category/7151097)
# Self-Normalizing Neural Networks
```
- 2017年9月   
- SELU激活函数，SNN（ self-normalizing neural networks）
- 约翰开普勒林茨大学【奥地利】
```
　　有望取代BN。 
　　提出缩放指数线性单元（scaled exponential linear units，selu），根据该激活函数得到的网络具有自归一化功能（SNN）。经过该激活函数后使得样本分布自动归一化到0均值和单位方差。 
　　用了97页证明在满足下面两条件时，使用该激活函数后使得样本分布满足零均值和单位方差：1）按照给定的参数对权重初始化：对于正态分布的初始化，初始化参数为0均值，方差为sqrt(1/n),n是输入的通道个数；2）使用selu。 
　　公式： 
![这里写图片描述](https://img-blog.csdn.net/20180610203221405?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
