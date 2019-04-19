# USB通信协议——错误检查 - xqhrs232的专栏 - CSDN博客
2017年09月19日 11:06:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：590
原文地址::[http://www.eeworld.com.cn/qrs/2013/1012/article_15615.html](http://www.eeworld.com.cn/qrs/2013/1012/article_15615.html)
相关文章
1、USB通信协议——循环冗佘检验位----[http://www.eeworld.com.cn/qrs/2013/1012/article_15614.html](http://www.eeworld.com.cn/qrs/2013/1012/article_15614.html)
2、USB通信数据紧密连接位----[http://www.eeworld.com.cn/qrs/2013/1012/article_15613.html](http://www.eeworld.com.cn/qrs/2013/1012/article_15613.html)
3、USB通信协议——分割数据交易----[http://www.eeworld.com.cn/qrs/2013/1012/article_15616.html](http://www.eeworld.com.cn/qrs/2013/1012/article_15616.html)
4、USB封包格式----[http://www.eeworld.com.cn/qrs/2013/1012/article_15617.html](http://www.eeworld.com.cn/qrs/2013/1012/article_15617.html)
5、USB封包中的数据域类型----[http://www.eeworld.com.cn/qrs/2013/1017/article_15691.html](http://www.eeworld.com.cn/qrs/2013/1017/article_15691.html)
6、USB通信协议——数据交易----[http://www.eeworld.com.cn/qrs/2013/1017/article_15692.html](http://www.eeworld.com.cn/qrs/2013/1017/article_15692.html)
7、USB通信协议——传输类型----[http://www.eeworld.com.cn/qrs/2013/1017/article_15693.html](http://www.eeworld.com.cn/qrs/2013/1017/article_15693.html)
8、USB的编码方式----[http://www.eeworld.com.cn/qrs/2013/1017/article_15694.html](http://www.eeworld.com.cn/qrs/2013/1017/article_15694.html)
9、USB差动信号----[http://www.eeworld.com.cn/qrs/2013/1017/article_15695.html](http://www.eeworld.com.cn/qrs/2013/1017/article_15695.html)
10、usB的通讯协议(通俗易懂)----[https://wenku.baidu.com/view/d9a5b9c06137ee06eff918b5.html](https://wenku.baidu.com/view/d9a5b9c06137ee06eff918b5.html)
通过[USB](http://www.eeworld.com.cn/tags/USB)来作数据传输的期间，可能会由硬件来检测若干错误现象。当然，通过驱动器、传送器以及缆线等的USB规范，所构建出的硬件，基本上发生错误的几率很小。因此，在稍前所介绍过，用来设计整合至USB数据交易协议的握手封包，可以确认出封包已经被成功地接收到。此外，涵盖[错误检查](http://www.eeworld.com.cn/tags/%E9%94%99%E8%AF%AF%E6%A3%80%E6%9F%A5)位的USB封包也可使得[接收器](http://www.eeworld.com.cn/tags/%E4%BF%A1%E5%8F%B7%E6%8E%A5%E6%94%B6%E5%99%A8)能确实地辨识所接收到的数据是否与传送出去的数据相符合。此外，如果需要多个数据交易，数据紧密连接（data [toggle](http://www.eeworld.com.cn/tags/Toggle)）位将会保持传送器与接收器同步，以确保没有数据交易已经完全地被遗漏掉。而由USB所支持的错误检查的机制包含了：
　　·封包错误辨识；
　　·假的EOP；
　　·总线超时（tlme-out），没有响应；
　　·数据紧密连接错误检查。
#### 关键字：[USB](http://www.eeworld.com.cn/tags/USB)[通信协议](http://www.eeworld.com.cn/tags/%E9%80%9A%E4%BF%A1%E5%8D%8F%E8%AE%AE)[错误检查](http://www.eeworld.com.cn/tags/%E9%94%99%E8%AF%AF%E6%A3%80%E6%9F%A5)

