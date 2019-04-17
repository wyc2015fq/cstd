# 庖丁解牛TLD（一）——开篇 - 迭代的是人，递归的是神 - CSDN博客





2011年11月09日 16:30:16[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：24652








最近在网上多次看到有关Zdenek Kalal的TLD的文章，说他做的工作如何的帅，看了一下TLD的视频，感觉确实做的很好，有人夸张的说他这个系统可以和Kniect媲美，我倒是两者的工作可比性不大，实现的方法也不同。但这个哥们做的真的很棒，最可贵的是人家提供了源码可以下载。他相关的工作网上一搜一大片，推荐一个链接[http://www.cvchina.net/article-22-1.html](http://www.cvchina.net/article-22-1.html)，再给个作者网站的链接[http://info.ee.surrey.ac.uk/Personal/Z.Kalal/](http://info.ee.surrey.ac.uk/Personal/Z.Kalal/)。

![](http://hi.csdn.net/attachment/201111/9/0_13208326742oOr.gif)


看了他的demo视频顿时心潮澎湃，赶紧下载了他的源代码后又有些崩溃了。他的工作量还是很大的，得静下心来慢慢研究。直接看代码难免云里雾里的，先看看人家的论文吧。

这哥们这几年可真没少发paper啊，我比较关注他实现的算法。TLD即Tracking Learning Detector，我认为就是依据跟踪学习的目标检测。看了他08年发表的一篇文章，介绍了他的这个算法，那时候还把算法称为TMD（tracking、modeling、detection），他的tracking工作时基于Lucas-Kanade光流法的。modeling学习的过程有growing和pruning两方面的工作，可以正负反馈，得到较好的学习结果，对于他的学习过程，他在另一篇文章中又详细介绍了P-N
 learning这种学习算法。detection的部分用的是随机森林的机器学习办法，加上bootstarps。对于特征的选择，他提出了一种基于LBP特征的2bit BP特征。几篇文章下来，我已经有点晕乎了，这些算法都不是太熟悉，看来得结合代码一个个啃了。

代码正在一点点啃，有同样兴趣的朋友可以交流指导一下，不胜感激。



