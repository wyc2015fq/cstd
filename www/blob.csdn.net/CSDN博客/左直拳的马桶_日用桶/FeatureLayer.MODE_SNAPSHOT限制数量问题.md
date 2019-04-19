# FeatureLayer.MODE_SNAPSHOT限制数量问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年04月27日 11:34:29[左直拳](https://me.csdn.net/leftfist)阅读数：1547
我在加载FeatureLayer的时候，采用`MODE_SNAPSHOT`模式，发现有的图层少了一些数据，改回`MODE_ONDEMAND`又正常。
究其原因，`MODE_SNAPSHOT` 是一次性将数据从服务器获取到客户端，之后的操作就不用再到服务器请求了。快照嘛，不就是这么个意思吗。但是，服务器对这种模式有一个限制，限制返回的数量，比如只能返回500条记录。这个限制数量可以在服务器进行设置。
想想也有道理，如果图层数量很多，数据很大，不加限制，首次加载的时候就会非常慢。
所以，`MODE_SNAPSHOT` 只适合数据量少的图层。正常情况下，还是采用默认的`MODE_ONDEMAND` 模式为好。这种模式大部分时候都是最佳，用到再请求，性能均衡。
参考文章 
[ArcGIS服务器的feature图层限制](http://www.cnblogs.com/liflying/p/4271651.html)
