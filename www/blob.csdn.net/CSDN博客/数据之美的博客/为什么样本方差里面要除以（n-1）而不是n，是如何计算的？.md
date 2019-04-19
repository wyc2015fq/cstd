# 为什么样本方差里面要除以（n-1）而不是n，是如何计算的？ - 数据之美的博客 - CSDN博客
2019年03月06日 23:03:01[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：61
我们在计算方差时，通常会认为是除以(n)，但是实际上，除以(n-1)才是正确的结果，因为 除以(n-1)得到的方差估计量才是关于总体方差的无偏估计量。直接上推导过程如下： 
![](https://img-blog.csdnimg.cn/20190306225255237.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190306225645452.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==,size_16,color_FFFFFF,t_70)
如上可以明白除以(n-1)，而不是除以(n)，到底是怎么来的。
之后我会介绍为什么看上去，除以(n-1)计算得到的方差估计量更接近总体的方差的无偏估计
