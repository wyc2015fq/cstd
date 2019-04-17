# 深入分析java web技术内幕----读书笔记（一） - 程序员江湖 - CSDN博客





2017年03月13日 11:42:34[黄小斜](https://me.csdn.net/a724888)阅读数：1280








一、深入web请求过程

1.B/S网络架构，服务端基于同一的http。

在浏览器输入taobao.com会发生：dns解析ip——>根据ip寻找服务器（负载均衡选择服务器）--->查找数据（分布式缓存、静态文件、数据库）-->请求静态资源（图片，css等）可能会请求cdn服务器。














发起http请求的过程（dns解析过程）：




1检查浏览器缓存和系统缓存

2根据dns地址接入本地域名服务器（80%在这里完成）

3到根服务器请求解析，返回一个国际顶级域名服务器（com，cn，org等），再向国际级服务器请求解析。

4返回name server域名服务器地址，解析出ip，返回ip和ttl值给dns服务器。

5ttl值控制缓存时间，按照该值缓存到本地系统缓存中，

注:name server可能有多级，负载均衡等可能影响解析过程。




3所有网络资源基于url统一资源定位符




http报文结构请参考链接：http://blog.csdn.net/a724888/article/details/61914872






以baidu.com为例





















客户端请求头

![](https://img-blog.csdn.net/20170313131844514?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

服务端响应头

![](https://img-blog.csdn.net/20170313131552339?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




请求页面资源的状态码
![](https://img-blog.csdn.net/20170313193801975?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


关于304状态码

当浏览器第一次加载资源的时候，返回一般为200，意思是成功获取资源，并会在浏览器的缓存中记录下max-age，第二次访问的时候：

如果只是用浏览器打开，那么浏览器会去判断这个资源在缓存里有没有，如果有的话，会去判断max-age，看看过期没有，如果没有过期，则直接读缓存，根本不会和服务器进行交互，换句话说，断网都能打开，就和本地跑一样！如果已经过期了，那就去服务器请求，等待服务器响应，这是很费时间的，服务器如果发现资源没有改变过，那么就会返回304，告诉浏览器，我没变过，你去读缓存吧，于是浏览器也不用从服务器拉数据了，然而，等待服务器响应也是一个很要命的问题，在网速发达的今天，等一个响应，有时比下载还慢。

如果是用浏览器刷新的，那么浏览器不会去判断max-age了，直接去服务器拿，如果服务器判断资源没变过，则还是会返回304，和上面是一样的，所以刷新一下，其实很可怕，等于把所有的资源都要去服务器请求一边，问问服务器我过期了没有。

综上，尽量减少网页的资源数量！尽量合并JS CSS 图片！响应速度将会猛增！

当今，响应速度比网速重要！！




4、ctrl+f5刷新会重新向服务器请求页面而不会使用缓存，是因为在http请求头上加了一些请求项。









5、cache control会优先覆盖expires等字段。







5、CDN工作机制





![](https://img-blog.csdn.net/20170313131729470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





负载均衡：

1链路负载均衡

2硬件负载均衡

3软件负载均衡

有关负载均衡的文章请参考链接：http://blog.csdn.net/a724888/article/details/61915986




也可以参考：

http://blog.csdn.net/a724888/article/details/61915442


http://blog.csdn.net/a724888/article/details/61915564


http://blog.csdn.net/a724888/article/details/61915866


http://blog.csdn.net/a724888/article/details/61915876


参考《深入分析java web技术内幕》这本书，强烈推荐。

文章为原创，请尊重作者劳动成果。

转载请注明文章地址：http://blog.csdn.net/a724888/article/details/61915330












