# video registration (利用动态纹理) - 家家的专栏 - CSDN博客





2013年05月17日 15:14:41[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：987
个人分类：[乐在科研中](https://blog.csdn.net/yihaizhiyan/article/category/742190)









分析paper：PAMI 2011 Video registration using dynamic textures

Register 多个动态场景的视频序列，包含但不局限于，非刚性对象如火焰、烟雾等~

本文目的：提出一个简单的方法来match如此复杂的场景。

把每个视频建模为一个线性动态系统的输出。 （基本思想参见：IJCV2003 dynamic textures）

这样视频的register，转变为，动态模型参数的register~

因为这些动态模型的参数并不是唯一定义的，不能直接的比较并register~

所以，本文中为了解决这种歧义，把确认的参数转换为规范的形式~

这样可以把video registration简化为多个图像registration的问题~



因为原来没做过registration，所以也写不出个一二三来~

主要算法流程有两步吧~

第一步，主要是求取视频序列的动态纹理参数：

![](https://img-blog.csdn.net/20130517150500107)





第二步才是rigistration的过程~

![](https://img-blog.csdn.net/20130517150454998)



