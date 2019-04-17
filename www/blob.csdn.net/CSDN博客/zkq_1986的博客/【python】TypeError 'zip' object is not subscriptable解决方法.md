# 【python】TypeError: 'zip' object is not subscriptable解决方法 - zkq_1986的博客 - CSDN博客





2018年12月25日 16:19:06[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：252








原代码：

for dog,cat in zip(dp.dogs,dp.cats)[:1000]:

    #省略代码


报错：

TypeError: 'zip' object is not subscriptable


解决方法：使用list包装zip对象，如下，

for dog,cat in list(zip(dp.dogs,dp.cats))[:1000]:

    #省略代码


转载自：[https://blog.csdn.net/nima1994/article/details/79894854](https://blog.csdn.net/nima1994/article/details/79894854)



