# word2vec 中的数学原理详解（五）基于 Negative Sampling 的模型 - Joe的博客 - CSDN博客





2016年09月24日 12:48:41[Joe-Han](https://me.csdn.net/u010089444)阅读数：2813








作者: peghoty  

转自：[http://blog.csdn.net/itplus/article/details/37998797](http://blog.csdn.net/itplus/article/details/37998797)

> 
word2vec 是 Google 于 2013 年开源推出的一个用于获取 word vector 的工具包，它简单、高效，因此引起了很多人的关注。由于 word2vec 的作者 Tomas Mikolov 在两篇相关的论文 [3,4] 中并没有谈及太多算法细节，因而在一定程度上增加了这个工具包的神秘感。一些按捺不住的人于是选择了通过解剖源代码的方式来一窥究竟，出于好奇，我也成为了他们中的一员。读完代码后，觉得收获颇多，整理成文，给有需要的朋友参考。


![这里写图片描述](https://img-blog.csdn.net/20160924124715945)
![这里写图片描述](https://img-blog.csdn.net/20160924124724520)
![这里写图片描述](https://img-blog.csdn.net/20160924124733664)
![这里写图片描述](https://img-blog.csdn.net/20160924124742915)
![这里写图片描述](https://img-blog.csdn.net/20160924124750207)
![这里写图片描述](https://img-blog.csdn.net/20160924124757977)
![这里写图片描述](https://img-blog.csdn.net/20160924124810259)
![这里写图片描述](https://img-blog.csdn.net/20160924124817348)
![这里写图片描述](https://img-blog.csdn.net/20160924124824322)
![这里写图片描述](https://img-blog.csdn.net/20160924120923362)
![这里写图片描述](https://img-blog.csdn.net/20160924121312114)



