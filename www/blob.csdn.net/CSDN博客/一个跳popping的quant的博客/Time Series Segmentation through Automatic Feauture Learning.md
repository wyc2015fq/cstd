# Time Series Segmentation through Automatic Feauture Learning - 一个跳popping的quant的博客 - CSDN博客





2018年05月10日 20:02:32[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1938标签：[机器学习																[DL																[Time Series																[paper](https://so.csdn.net/so/search/s.do?q=paper&t=blog)
个人分类：[quantitative trading](https://blog.csdn.net/FrankieHello/article/category/7166871)

所属专栏：[一个用人工智能做量化的小白的进阶之路](https://blog.csdn.net/column/details/20644.html)](https://so.csdn.net/so/search/s.do?q=Time Series&t=blog)




题目：《Time Series Segmentation through Automatic Feauture Learning》

时间：2018/1

### 简介：

这是一篇由IBM Research和普林顿大学几个reasearcher写的paper。很久之前老师就推荐给我了，最近有时间看完了，所以整理下。


这篇paper主要是针对时间序列的划分进行了探讨，用到的主要技术就是Deep Learning。整个方法和模型的框架并不复杂，而且还有较高的实用性。

### 应用技术：

ChangePoint的检测无论是进行聚类还是将连续状态转换为离散状态时都是一个基础而又极为重要的过程。


文中提出了一个breakpoint的概念，意为是人类专家进行对序列划分的changpoint。传统的检验ChangePoint的方法一般都是statistics-based，通过概率的角度对边界进行划分。而文中的方法则更偏向从更细微或者其他的维度的划分。

文中主要用到了Deep Learning 中的AutoEncoder算法。

### 模型框架：

![](https://img-blog.csdn.net/20180510195016606?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


1、先是通过人为指定一个time window，图中展示了具有三个channel的数据，类似声音或者其他类型数据。


2、然后核心就是AE模型，AE常用来作为DL参数的初始化以及对数据的降维处理。这里将它作为对数据维度的一个转换（也可以称为features extraction），并通过这个新的维度来计算新的维度中的feature中t与t+1 timestap的距离，这里用到了欧式距离的计算方法。

![](https://img-blog.csdn.net/20180510195511871?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


3、通过这个local-maximum distance（对应下图中的peak）来确定breakpoint。

![](https://img-blog.csdn.net/20180510212025785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


4、最后整体进行bp更新。

之后文中还对比了其他的window size的区别以及与其他方法的对比实验。

### 总结：
这是一篇将DL应用与Time Series Segmention中较新的paper。不同于传统statistics方法，文中通过了其他维度进行了对changePoint的检测来划分序列，对一些基于discrete state-space的方法有着比较基础的意义。](https://so.csdn.net/so/search/s.do?q=DL&t=blog)](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)




