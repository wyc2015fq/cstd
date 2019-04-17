# AttributeError: 'FeedExporter' object has no attribute 'slot' - 博客堂 - CSDN博客





2017年05月20日 16:26:06[最小森林](https://me.csdn.net/u012052268)阅读数：2446








使用scrapy时候你报错：

AttributeError: ‘FeedExporter’ object has no attribute ‘slot’

因为当前需要写入的**文件**被占用，写不进去！

解决方法，关闭打开的 csv文件



