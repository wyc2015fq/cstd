# R语言----PCA分析，热图（楔子） - weixin_33985507的博客 - CSDN博客
2018年11月26日 08:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：25
在转录组的分析当中，主成分分析（PCA）往往是成果体现的一个很重要的手段。
在《R语言做主成分分析实例》里的降解非常的好--网址 ：[https://shengxin.ren/article/84](https://shengxin.ren/article/84)
定义：主成分分析法是数据挖掘中常用的一种降维算法,是Pearson在1901年提出的,再后来由hotelling在1933年加以发展提出的一种多变量的统计方法，其最主要的用途在于“降维”，通过析取主成分显出的最大的个别差异,也可以用来削减回归分析和聚类分析中变量的数目，与因子分析类似。（说实话是在是没啥用，看不懂）
举个栗子：
比如你要做一项分析人的肥胖的因素有哪些，这时你设计了50个你觉得都很重要的指标，然而这50个指标对于你的分析确实太过繁杂，这时你就可以采用主成分分析的方法进行降维。50个指标之间会有这样那样的联系，相互之间会有影响，通过主成分分析后，得到三五个主成分指标。此时,这几个主成分指标既涵盖了你50个指标中的绝大部分信息，这让你的分析得到了简化（从50维降到3、5维）。
-------------------------分割线----------------------------------------------------------------------------------------------------
学习的过程是来自于B站孟浩巍的系列视频2017-08-04-高通量测序技术交流录像[https://www.bilibili.com/video/av12969326](https://www.bilibili.com/video/av12969326)
先附上这次学习的主成分分析的代码
```
rm(list=ls())
combine_fpkm_table <- read.csv("./diffexpr_resultsfor_TNBC18_VS_para10.csv",header = T,sep = ',')
dim(combine_fpkm_table)                                 
input_matrix <- combine_fpkm_table[,c(2:ncol(combine_fpkm_table))]
#princomp(input_matrix) not used often 
library(psych)
input_matrix <- combine_fpkm_table[,c(2:ncol(combine_fpkm_table))]
pca_result <- principal(input_matrix,nfactors = 3) #一般主成分不能超过5个，一般是2,3个
pca_result$values  #每个算法的评分？
pca_result$scores   #每个基因的权重
pca_result$weights  #具体的数值
plot(pca_result$scores[,1],pca_result$scores[,3],xlim = c(0,50),ylim = c(0,50))
```
![14720037-c6b9cf1954463928.png](https://upload-images.jianshu.io/upload_images/14720037-c6b9cf1954463928.png)
PCA分析的结果，取第一个和第三个主成分分析的因子
接下来还有一个热图的代码
```
library(pheatmap)
input_table = combine_fpkm_table[,c(2:ncol(combine_fpkm_table))]
pheatmap(log2(input_table[c(1:500),] +1))  #取了前面500行
```
![14720037-2423c3f3195cc9bf.png](https://upload-images.jianshu.io/upload_images/14720037-2423c3f3195cc9bf.png)
热图结果
