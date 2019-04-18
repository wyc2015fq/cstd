# 隐马尔科夫模型（HMM） - wsp_1138886114的博客 - CSDN博客





2018年07月26日 18:20:36[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：63标签：[HMM隐马尔科夫模型](https://so.csdn.net/so/search/s.do?q=HMM隐马尔科夫模型&t=blog)
个人分类：[机器学习](https://blog.csdn.net/wsp_1138886114/article/category/7692618)










- - - [一 隐马尔科夫模型（HMM）定义与概念](#一-隐马尔科夫模型hmm定义与概念)- - [1.1 前项算法实例](#11-前项算法实例)
- [1.2 后向算法实例](#12-后向算法实例)







### 一 隐马尔科夫模型（HMM）定义与概念

隐马尔科夫模型（HMM，Hidden Markov Model）可用标注问题，在语音识别，NLP、生物信息、模式识别等领域实践被证明是有效的算法。 

HMM 是关于时序的概率模型，描述由一个隐藏的马尔科夫链随机生成不可观察的状态随机序列，再有各个状态生成一个观测而产生随机序列的过程。 
![这里写图片描述](https://img-blog.csdn.net/20180720175524746?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180720180502456?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180720181707510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 1.1 前项算法实例

![这里写图片描述](https://img-blog.csdn.net/20180720182552172?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 1.2 后向算法实例

![这里写图片描述](https://img-blog.csdn.net/20180720182727741?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




