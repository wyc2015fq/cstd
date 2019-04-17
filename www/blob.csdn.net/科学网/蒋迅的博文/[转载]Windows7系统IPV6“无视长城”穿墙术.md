# 科学网—[转载]Windows7系统IPV6“无视长城”穿墙术 - 蒋迅的博文




# [转载]Windows7系统IPV6“无视长城”穿墙术                           

已有 6496 次阅读2010-11-4 11:39|个人分类:[科技](http://blog.sciencenet.cn/home.php?mod=space&uid=420554&do=blog&classid=136794&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=420554&catid=1)|关键词:IPV6,WIndows7|[IPv6](misc.php?mod=tag&id=975), [WIndows7](misc.php?mod=tag&id=86270)|文章来源:转载



作者：[夜の猫](http://blog.newchen.com/)

上次发了一篇“[YouTube上五个超棒的科学实验](http://www.sciencenet.cn/m/user_content.aspx?id=366722)”，结果有人告诉我在国内是完全看不到的。我因此转了一篇“[试试免费测试VPN的网站](http://www.sciencenet.cn/m/user_content.aspx?id=367689)”。今天再转一篇“[Windows7系统IPV6‘无视长城’穿墙术](http://blog.newchen.com/post/1469)”，也能帮助你穿墙而过，如果你有Windows7的话。我们的墙很强大，这个办法是不是“终极”的我不知道，但目前应该是有效的。下面是全文：


这是目前为止“翻 墙”的终极方法。。。

准确的来说，这种方法用“翻 墙”来形容并不贴切，更加形像的形容词应该叫“穿墙”。。。因为，目前来说，使用IPV6的方法，是直接无视长城存在的。。。

而且幸运的是，在Windows7系统上，默认就是支持IPV6的。。。我们要做到，仅仅就一步，替换一个HOSTS文件就OK。（Linux用户同理，Hosts文件所在目录为 /ETC。）

[**点击这里**](http://www.newchen.com/downloads/hosts.7z)下载Hosts文件压缩包，然后解压到“**C:windowssystem32driversetc**”目录覆盖原文件即可（原文件先备份下）(如下图）。

![](http://www.sciencenet.cn/photo/upload/bigimg/2010101611025449.jpg)

此Hosts文件写入了所有Google相关服务官方部署的IPV6地址（比如Picasa，Youtube，等已经被封杀的网站），另外包含了两条Flickr的IPV4地址（针对部分联通网络用户）。

完整的Hosts文件内容来自[这里](https://docs.google.com/View?docID=0ARhAbsvps1PlZGZrZG14bnRfNjFkOWNrOWZmcQ)。

替换好Hosts文件后，可以尝试PING一下Google的网址，已经变成IPV6的IP地址了（如下图）。

![](http://www.sciencenet.cn/photo/upload/bigimg/2010101611025558.jpg)

**现在。。。G F W对你的电脑访问Google相关的站点已经无法做出封杀操作了。。。**

当然，这个Hosts文档并不完整，因为还有很多站点的IPV6没有部署，或没有部署完毕，有些也不是官方的，比如Facebook和Twitter，暂时没有官方完整的IPV6可用。大家还是得借助一些翻 墙工具来访问这些站点。。。相信以后都会逐渐部署完毕的。。。不过，这个Hosts文件已经能让你完全自由的使用Google了。。。（对我来说。。。更重要的是能够不受墙的干扰，酣畅淋漓的观看Youtube了。。。呵呵。。。）

顺便说一下。。。

我知道这方法是很久前的了。。。我是一直在用著IPV6翻 墙的（这方法是最稳定的）。。。不用你说我火星什么的。。。我是希望互联网上多一份教人翻 墙的文档，所以才把这个老方法又写了一篇新文章。。。翻 墙不是什么时髦，也不是什么流行，某些脑残们你们别来追。。。翻 墙是一种悲哀。。。却被有些当成一种时尚，流行，甚至光荣。。。不会翻 墙并不可耻，以为会翻 墙很牛B，去嘲笑不会翻 墙的才是可耻的。。。是的。。。我是改变不了什么。。。可至少我还在努力尝试这做一点什么。。。虽然我做的可能屁用不顶。。。或许是我自我感觉太过良好，但我觉得这样至少比那些什么都不做，还在一旁冷嘲热讽的SB强。。。我的东西不是写给你们看的。。。拜托你们，千万别学会翻 墙。。。我朝的互联网是充分自由的。。。你们就别跟著一起反革 命了。。。赶紧洗洗睡吧。。。明天还有嫦娥3号要成功升仙呢。。。还有几千亿的外国国债等你购买呢。。。

──────── by：与口腔溃疡做了N天斗争的 夜の猫

[hosts.7z](http://blog.sciencenet.cn/home.php?mod=attachment&id=10823)



转载本文请联系原作者获取授权，同时请注明本文来自蒋迅科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-420554-380324.html](http://blog.sciencenet.cn/blog-420554-380324.html)

上一篇：[【数学都知道】2010年11月2日](blog-420554-379563.html)
下一篇：[【数学都知道】2010年11月6日](blog-420554-380942.html)



