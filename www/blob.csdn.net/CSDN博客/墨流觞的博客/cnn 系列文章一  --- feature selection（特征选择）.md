# cnn 系列文章一    --- feature selection（特征选择） - 墨流觞的博客 - CSDN博客





2018年09月12日 11:09:02[墨氲](https://me.csdn.net/dss_dssssd)阅读数：399标签：[python																[feature selection																[cnn																[tensorflow](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)
个人分类：[python深度学习](https://blog.csdn.net/dss_dssssd/article/category/8032217)





## CNN in tensorflow

**这是系列文章，讲述卷积神经网基础知识和著名网络结构，以及在tensorflow中实现。**

立个flag吧，每篇文章之前分享一句歌词。本人网易云id “紫迷儿”

> 
《红颜》 

  剑煮酒无味，饮一杯为谁   


## 疑难词汇

trivial 琐碎的，不值一提的

## 特征选择

### 特征选择(Feature Selection)的缺点

人工选取特征，然后计算机利用这些特征进行分类，在选择正确特征情况下，信噪比(signal-to-noise ration)将会提高，分类正确率也会提高。

![](./fs.png)![这里写图片描述](https://img-blog.csdn.net/20180912110443111?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

比如Viola和Jones，主要采取眼睛和上脸颊之间的区域、两眼之间的鼻梁区域的光强度（light density）作为特征，在使用分类器利用特征区域来进行人脸分类，达到91.4%的识别率。但是当脸部倍阴影覆盖是，算法不再适用。
简单来说，就是选取正确的特征困难，并且适用性很低，鲁棒性低。

[下一部分主要讲解卷积神经网络结构](https://blog.csdn.net/dss_dssssd/article/details/82659137)](https://so.csdn.net/so/search/s.do?q=cnn&t=blog)](https://so.csdn.net/so/search/s.do?q=feature selection&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




