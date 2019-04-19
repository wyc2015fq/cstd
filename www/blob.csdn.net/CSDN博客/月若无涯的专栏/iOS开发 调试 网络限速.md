# iOS开发 调试 网络限速 - 月若无涯的专栏 - CSDN博客
2014年12月26日 11:39:31[月若无涯](https://me.csdn.net/u010124617)阅读数：6429标签：[iOS																[调试																[网络																[mac																[模拟器](https://so.csdn.net/so/search/s.do?q=模拟器&t=blog)](https://so.csdn.net/so/search/s.do?q=mac&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=调试&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/u010124617/article/category/1393991)
在iOS开发中，针对不同网络状况做一下测试处理是很有必要的。但是我发现还是有一些iOS开发者不太注意到不同网络环境下的调试问题，或者说不清楚如何调试这种情况。
下面我将针对真机和模拟器分别做简单的说明。告诉大家如何模拟不同的网络状况。
**1.真机情况下**
其实Apple在iOS系统中预置了网络调试工具，但是只有添加过测试设备，并使用XCode连接下设备才能激活.
这时只要去设置中就可以看到多出来一项：开发者
![](https://img-blog.csdn.net/20141226110000477?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意中间的那行NETWORK LINK CONDITIONER
![](https://img-blog.csdn.net/20141226110223281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图中的Status是表示网络限制是否开启，点击进去就可看到详细设置
![](https://img-blog.csdn.net/20141226110341825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到系统默认配置的网络条件还是很多的，其中：
100%Loss是全丢包,
3G这个大家应该都知道
DSL是电话线上网,
Edge是2G网络，
High Latency DNS是高延迟，
Very Bad Network是网络状况不稳定。
并且底部还有一个添加额外配置的选项可以自定义网络状况。
PS：这项开启后影响的是整个系统，所以调试完毕后不要忘记关掉，免得奇怪网络状态咋不正常了。。。
**2.模拟器**
由于模拟器是在电脑上的，mac上也有和iOS设备中一样的调试工具，并且被收录在Apple官网工具中。同样，这工具也是影响整个系统的。
详情可看：[http://nshipster.com/network-link-conditioner/](http://nshipster.com/network-link-conditioner/)
该工具的Apple官方地址：[https://developer.apple.com/downloads/index.action?q=Network%20Link%20Conditioner#](https://developer.apple.com/downloads/index.action?q=Network%20Link%20Conditioner#)
具体的步骤：
下载：
![](https://img-blog.csdn.net/20141226112934090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
打开DMG文件，里面包含了许多调试工具，其他的工具有兴趣的可以自己去看一下。这里我们只打开Network Link Conditioner.prefPane这个文件![](https://img-blog.csdn.net/20141226112956789?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在系统偏好中出现了工具：
![](https://img-blog.csdn.net/20141226112950923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
打开工具，看一下，几乎和手机上是一样的。
![](https://img-blog.csdn.net/20141226112944750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**最后：**
希望各位看完这篇文章，能更好的做好应用的各种调试，开发出更加稳定可靠的应用来。
