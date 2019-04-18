# 最新论文阅读（28）--Convolutional Normalizing Flows - wydbyxr的博客 - CSDN博客
2018年06月10日 20:39:22[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：131
# Convolutional Normalizing Flows
```
- 2017年11月
-   
- 卡内基梅隆大学
```
　　思想来自贝叶斯后验；我们提出了一个简单的normalizing flows的结构，即ConvFlow。 
　　该结构中的卷积会随着输入向量的维度改变而变化；其实就是不同dilate的1D-conv堆叠而成。 
　　在不同MNIST和OMNIGLOT（罗马字符数据集）上有实验。 
![这里写图片描述](https://img-blog.csdn.net/20180610203852487?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
