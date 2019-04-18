# 论文阅读：《Text Matching as Image Recognition》 - Soul Joy Hub - CSDN博客

2018年11月07日 12:37:32[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：212


原文：[https://blog.csdn.net/sinat_33741547/article/details/80649542](https://blog.csdn.net/sinat_33741547/article/details/80649542)

## 一、概述

MatchPyramid来自Liang Pang等在2016发表的一篇文章[Text Matching as Image Recognition](https://arxiv.org/abs/1602.06359)，大意为利用图像识别的方式进行文本匹配。

## 二、思路

对于文本匹配，基本思路如下述公式： 
![这里写图片描述](https://img-blog.csdn.net/20180611112328708?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中T为文本，函数，而MatchPyramid应属于后一种。

### 1、基本方法

构建文本与文本的相似度矩阵，采用CNN对矩阵进行特征抽取，最后用softmax获得分类概率，评价方法为交叉熵，如下： 
![这里写图片描述](https://img-blog.csdn.net/20180611113108398?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 2、流程

#### （1）相似度矩阵

由于CNN针对的是网格型数据，而文本显然属于序列数据，那么就有必要对数据进行转换，论文中提出了三种构建相似度矩阵的方法： 
![这里写图片描述](https://img-blog.csdn.net/20180611113423489?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

0-1类型，每个序列对应的词相同为1，不同为0 
![这里写图片描述](https://img-blog.csdn.net/20180611113605203?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

cosine距离，使用预训练的Glove将词转为向量，之后计算序列对应的词的cosine距离 
![这里写图片描述](https://img-blog.csdn.net/20180611113740832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点积，同上，但是将cosine距离改为点积 

作者在文中提到，距离矩阵使用点积的效果相对较好。

#### （2）两层CNN

后续利用两层的CNN对相似度矩阵进行特征抽取，这里要注意的是由于上一层的相似度矩阵shape不一致，在第一层CNN后面进行maxpool的时候，要使用动态pool，有没有其他的小trick就不可得知了。

#### （3）两层MLP

最后用两层的全连接对CNN的结果进行转换，使用softmax函数得到最终分类概率。

## 三、其他

作者使用论文中的模型，在kaggle的quora数据集中得到一个相当不错的分数，最终小组成绩达到了第四名。 

附实现地址：[https://github.com/faneshion/MatchZoo](https://github.com/faneshion/MatchZoo)

