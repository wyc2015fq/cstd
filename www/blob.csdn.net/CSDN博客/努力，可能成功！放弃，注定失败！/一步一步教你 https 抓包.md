# 一步一步教你 https 抓包 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月21日 13:31:19[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：6415
在 Mac 上常用的抓包软件是 Charles，网上关于 Charles 的教程很多，这里介绍另一个抓包神器 mitmproxy。mitmproxy 是一款可交互式的命令行抓包工具，它除了可以抓包查看 http/https 请求，还有一个很强大的功能是拦截并修改 request 或者 response，后面我会用实际例子演示如何修改知乎的回答:)
#### 安装
[mitmproxy 官网](https://mitmproxy.org/) 介绍了用以下命令来安装
``pip install mitmproxy``
我在实际安装过程中遇到了一些坑，首先是 OSX El Capitan 及以上的系统版本在安装时会出现 six 模块依赖错误，具体可以看[这儿](https://github.com/pypa/pip/issues/3165)的讨论。还有一个问题是安装过程中会出现权限错误，需要以 root 权限执行。最后用以下命令成功安装,安装不了需要翻一下墙
``pip install mitmproxy --ignore-installed six``
#### 开始使用
首先需要配置一下网络环境，将手机和电脑连在同一个 wifi 环境下，然后进入手机的 设置 - 无线局域网，点击当前连接的 wifi 进入详情设置页面，在 http 代理那一栏输入当前连接的 ip 地址，端口填8080，这个 ip 地址可以在电脑上的 系统偏好 - 网络 里查看，配置完成如下图，
![](http://upload-images.jianshu.io/upload_images/128529-314da4bbd591cfc8.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
网络环境配置完成后我们可以开始实际的抓包了，只需在电脑上打开终端（命令行操作窗口），然后输入 `mitmproxy -p 8080`，如果你看到如下页面说明你已成功进入抓包模式，迈出了第一步，cheer~
![](http://upload-images.jianshu.io/upload_images/128529-8dc96d366fd77092.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
接下去你可以用手机打开一些 App，不出意外你能看到一些 http 请求的列表。这时候我们还无法抓到 https 的包，还需要做一些额外配置。
下面的操作是针对 iPhone 手机，其他设备也类似。用 iPhone 打开 Safari 浏览器并输入 mitm.it，这时你会看到如下页面，
![](http://upload-images.jianshu.io/upload_images/128529-4d9c80646296b645.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
选择对应平台并安装证书，安装完成后就可以抓 https 的包了，[官网上](http://docs.mitmproxy.org/en/latest/howmitmproxy.html)解释了这个工具是如何工作的
> 
注意：用浏览器打开时需要已经在抓包模式，否则是无法看到上述页面的
好了，到这里我们已经完成了所有的准备工作，接下去就和我一起实际体验一下 https 的抓包过程吧。
这里以知乎为例，用手机打开知乎，我们应该能看到知乎的请求出现在列表里了，类似下图：
![](http://upload-images.jianshu.io/upload_images/128529-dd0a9d81a4feef42.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
键盘上下移动，Enter 键进入查看详情，按 `Tab` 键切换顶部导航栏，如下图所示，
![](http://upload-images.jianshu.io/upload_images/128529-70280cca6e10cc73.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
Tips:输入`z`可以清空列表，输入`f`再输入`~u xxx`可以过滤出 url 为 xxx 的请求，更多匹配规则输入`?` 查看
#### 请求拦截
上面演示的是常规的操作，下面介绍一下 mitmproxy 的另一强大功能，拦截修改 request 和 response。
输入 `i`，然后输入 `~s` 再按回车键，这时候就进入了 response 拦截模式。如果输入 
```
~q
```
 则进入 request 的拦截模式，更多的命令可以输入 `？` 查看。拦截模式下的页面显示如下图所示，
![](http://upload-images.jianshu.io/upload_images/128529-7e533c061186ae4a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
其中橘红色的表示请求正被拦截，这时 Enter 进入后 再按 `e` 就可以修改 request 或者 response。修改时是用 vim 进行编辑的，修改完成后按
`a` 将请求放行，如果要放行所有请求输入 `A` 即可。
下图是我修改了某个答案的 response 请求将回答者的名字做了修改，因为只修改了 response 并不会影响其他的用户显示：）
![](http://upload-images.jianshu.io/upload_images/128529-ffaea3b21d54cea3.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
上面提到的那些指令在输入 `？`后都能查看到，如下图，另外[官网](http://docs.mitmproxy.org/en/latest/mitmproxy.html)上也有很详细的说明
![](http://upload-images.jianshu.io/upload_images/128529-5251359e15cde38c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
我用上述方式尝试了大多数的 App，包括淘宝、微博，都能抓到 https 的包，有些有加密，有些没有加密。但是简书做了防中间人攻击的设置，一启用抓包模式就显示网络错误1012，还是做的很不错的，赞一个~
按照上面的过程操作就可以 https 抓包了，如果有遇到问题可以[联系我](http://weibo.com/u/3170539585?is_all=1)，记得抓完后将手机 wifi 设置里的 http 代理关闭。开始实际体验一下吧，enjoy~
> 
如果您觉得本文对您有所帮助，请点击「喜欢」来支持我。
转载请注明出处，有任何疑问都可[联系我](http://weibo.com/u/3170539585?is_all=1)，欢迎探讨。
文／hi_xgb（简书作者）
原文链接：http://www.jianshu.com/p/1837203986f5
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
