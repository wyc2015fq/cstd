# 论文阅读: Anomaly Detection with Partially Observed Anomalies - Hxyue2018's Blog - CSDN博客





2018年10月15日 09:23:32[Hxyue2018](https://me.csdn.net/Super_Json)阅读数：212








对于异常检测而言，通常是根据标签是否可用而采取监督或者无监督的方式。论文提出一种新的方法，部分观测到异常，针对大量未标记的数据和少量已经标记为异常的数据，提出了一种ADOA的两阶段检测方法。首先聚类未标签数据确信正常和可能异常。然后再用加群多分类方法来给出对应类别的置信度。

对于无标签的数据而言，常用的无监督行为Distance based approaches [26], density based approaches [3] and isolation based methods [23] are typical representatives along this way

文章以malicious URL detection为例 PU (Positive and Unlabeled) learn- ing [17, 19] 但是PUlearning的正样本通常是同一类的异常，而另一个则是单一的异常

semi-supervised clustering



ADOA follows a two-stage manner In the rst stage, we address that the observed anomalies should not be simply regarded into one concept center, and by assuming that the anomalies belong to k di erent concept centers, the anomalies are rstly clustered into k clusters. After that, both potential anomalies and reliable nor- mal samples are selected from the unlabeled samples according to the isolation degree and the similarity to the nearest anomaly clus- ter center. In stage two, a weight is set to each sample according to the con dence of its attached label, and a weighted multi-class classi cation model is built to distinguish di erent anomalies from the normal samples, using original anomalies and the selected sam- ples. Experiments on di erent datasets and a real application task demonstrate the e ectiveness of our approach.

2.相关的工作

![](https://img-blog.csdn.net/20181021151443499?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1N1cGVyX0pzb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过两阶段，第一阶段通过聚类和异常值的方法对他进行汇聚，然后为每个模型添加，再利用手动标签模型进行

3.问题简述和算法描述



