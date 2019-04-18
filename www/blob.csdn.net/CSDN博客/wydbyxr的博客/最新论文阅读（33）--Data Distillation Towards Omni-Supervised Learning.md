# 最新论文阅读（33）--Data Distillation: Towards Omni-Supervised Learning - wydbyxr的博客 - CSDN博客
2018年06月10日 21:03:54[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：600
所属专栏：[深度学习--最新论文](https://blog.csdn.net/column/details/23683.html)
# Data Distillation: Towards Omni-Supervised Learning
```
- 2017年12月
- 全方位监督（Omni-Supervised）；数据精馏（data distillation）
- Ilija Radosavovic, Piotr Dollar, Ross Girshick, Georgia Gkioxari, Kaiming He,Facebook AI Research (FAIR)
```
　　全监督学习是一种半监督学习的特殊制度，学习者利用所有可用的标记数据加上互联网规模的未标记数据源。 
　　全方位监督学习在现有标记数据集上的表现更低，提供了超越最先进的完全监督方法的潜力。 
　　为了利用全方位监督的设置，我们提出了数据精馏，一种使用单一模型从多个未标记数据变换中合并预测的方法，以自动生成新的训练注释。 
　　在COCO dataset上，实验了人体关键点检测和常规的目标检测，结论是使用数据蒸馏的模型好于直接使用标签训练的模型（监督模型）。 
![这里写图片描述](https://img-blog.csdn.net/20180610210343911?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
