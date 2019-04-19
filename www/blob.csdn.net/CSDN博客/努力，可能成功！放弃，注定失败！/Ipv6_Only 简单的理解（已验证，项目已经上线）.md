# Ipv6_Only 简单的理解（已验证，项目已经上线） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月25日 22:08:46[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：7563
网上好多关于ipv6的资料，说半天ipv6是什么，怎么建立测试环境，，，可是没有看到具体的操作和解决的方案，这里，为大家提供一种方案，希望给大家带来帮助吧。
总的来说有三个方面需要进行检查和修改：
1、项目里面涉及和网络有关的**网络请求**是否支持ipv6。
2、项目里面涉及和网络有关的**sdk**是否支持ipv6。
3、项目的**服务器**是否支持ipv6。
**IPv6的简介**
IPv4 和 IPv6的区别就是 IP 地址前者是 .（dot）分割，后者是以 :（冒号）分割的（更多详细信息自行搜索）。
PS：在使用 IPv6 的热点时候，记得手机开飞行模式哦，保证手机只在 Wi-Fi 下上网，以免手机在连接不到网络时候，会默认跳转到使用 蜂窝移动网络（即2G、3G、4G流量） 上网。
IPV6，是对IPV4地址空间的扩充。目前当我们用iOS设备连接上Wifi、4G、3G等网络时，设备被分配的地址均是IPV4地址，但是随着运营商和企业逐渐部署IPV6 DNS64/NAT64网络之后，设备被分配的地址会变成IPV6的地址，而这些网络就是所谓的IPV6-Only网络，并且仍然可以通过此网络去获取IPV4地址提供的内容。客户端向服务器端请求域名解析，首先通过DNS64 Server查询IPv6的地址，如果查询不到，再向DNS Server查询IPv4地址，通过DNS64 Server合成一个IPV6的地址，最终将一个IPV6的地址返回给客户端。如图所示：
![](http://upload-images.jianshu.io/upload_images/1440398-ba992ddce5c37037.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
网上对于苹果官网上ipv6的文章翻译很多，附上链接，这篇介绍的不错：[iOS应用支持IPV6，就那点事儿](http://www.jianshu.com/p/a6bab07c4062)
**本地 Mac 搭建 IPv6 测试环境**
附上链接：[本地如何搭建IPv6环境测试你的APP](http://mp.weixin.qq.com/s?__biz=MjM5OTM0MzIwMQ==&mid=2652545529&idx=5&sn=0d0323cfd40441eb49a6b7c6b3792bfb&scene=23&srcid=0613ZfE6L2COVxj3Lad9fIWm#rd)
（**上边这些几乎没什么用，网上一搜有很多。。。**）
如何判断自己的项目里面是否是ipv6、ipv4呢，我们用的方法，就是自己**项目里面涉及和网络有关的进行一一排查**。
（**文章最后会提到ipv6解决的方案除了上述两点还有一个关于服务器的原因。**）
我们的app本身支持ipv6是因为我们使用的网络请求是asihttpRequest请求，而asihttpRequest是基于CFNetwork的，苹果有说明CFNetwork库是支持IPV6的。而除了自身的网络请求外，我们项目中涉及网络的就是三方库了，所以归根到底我们做的只是第三方库的替换，即：哪些三方库和网络有关，那么挨个查看最新的sdk文档解释是否描述支持ipv6，如果支持，那就替换。如果没说支持，那就不换。
（当然，还可以用cocoadpods替换三方库，关于cocoadpods的学习可以参考本人这篇文章[Cocoapods安装](http://www.jianshu.com/writer#/notebooks/2941782/notes/3667509)）
这种方案需要验证，估计需要几天的时间才能知道上线是否被拒。如果侥幸上线了，我会第一时间在本文里面续写一下。如果被拒，我也会第一时间排查原因，进行本文的更新。谢谢大家的支持。
**已经上线了，该方案可行。**
那么我们替换了哪些三方库呢，下边一一列举。（文章下部会有各个三方库的链接）
1、Reachability
按照苹果开发者中心提示，这个必须换。
![](http://upload-images.jianshu.io/upload_images/1440398-f40e33fa0cd54173.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
新的sdk包将这个方法干掉了：reachabilityForLocalWiFi，只要自己的代码中干掉就行了，没什么影响。
原因如下介绍：
#pragma mark reachabilityForLocalWiFi
//reachabilityForLocalWiFi has been removed from the sample.  See ReadMe.md for more information.
//+ (instancetype)reachabilityForLocalWiFi;
2、新浪微博
根据官网提示，这个新的sdk支持了ipv6，所以进行替换。
![](http://upload-images.jianshu.io/upload_images/1440398-9c7921d03fa22594.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
替换后：
根据比较，新旧sdk只有上述变化，替换后command＋B编译无错误提示。应该没事。
3、连连支付
按照连连支付官方文档
最新的sdk包是支持ipv6的，但是旧包也是支持的。项目中用的是2.4.0，官网上最新包是2.4.7，最后我们做了替换。
![](http://upload-images.jianshu.io/upload_images/1440398-651570763106ae47.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
替换后报错了：
![](http://upload-images.jianshu.io/upload_images/1440398-016c44aa71cfa3d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
原因是在新的sdk包里将报错的这两个方法合成了一个。
![](http://upload-images.jianshu.io/upload_images/1440398-80394cca4a1b2727.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
在新的方法里面添加了一个判断支付类型的参数。
根据项目中报错的两个地方，第一个是快捷支付，第二个是认证支付。按照之前的进行了修改。command＋B编译无错误提示。应该没事。
但是实际上还是遇到了崩溃的bug。然后我们换回了2.4.0版本，应为连连支付官网上说之前的版本也支持ipv6，我们之前的版本没有问题，所以换了回来，看看上线能成功不。
4、友盟
按照友盟官方sdk文档描述，需要更换新的sdk包。
![](http://upload-images.jianshu.io/upload_images/1440398-66f6420d395afb4f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
按照需要，勾选了如下：
md，搞错了，项目里面的友盟是友盟分析，上边那个是友盟分享。。。
![](http://upload-images.jianshu.io/upload_images/1440398-9cd8274ac915acb0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
但是根据官方文档，好像我们的不用替换，因为涉及到什么IDFA，我们项目好像不涉及这个。
5、微信
按照微信最新sdk包1.7版本里面的README.txt，最新的sdk包支持ipv6
![](http://upload-images.jianshu.io/upload_images/1440398-d53c91fcf9661aaf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
而我们项目中的微信是1.5版本的。应该进行替换。command＋B编译无错误提示。应该没事。
比较可笑的是微信的sdk包是支持ipv6的，但是微信本身并不支持ipv6，所以说，即便你替换了最新的sdk包，在ipv6网络下还是不能用微信分享，因为你的app应用在ipv6网络环境下调不起微信，也就分享不了了，这个问题微信应该意识到了，估计后边的版本应该也是支持ipv6的吧。
6、QQ
官方文档好像也没有说ipv6的事啊
![](http://upload-images.jianshu.io/upload_images/1440398-84bfb358050435e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
7、次奥！支付宝sdk支持了ipv6，上午下载的时候没看见！！！
![](http://upload-images.jianshu.io/upload_images/1440398-f3d39613b882b38d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
可是下载的时候总是打不开.zip的压缩包。。。叫别人帮忙下载了一份
command＋B编译无错误提示。应该没事。
8、百度地图
![](http://upload-images.jianshu.io/upload_images/1440398-bfe5dbcc76db521a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
根据比较，新的sdk包没有了bundle文件。需要将旧包的bundle文件拷贝进来。
各个sdk下载地址：
[reachability](https://developer.apple.com/library/ios/samplecode/Reachability/Introduction/Intro.html#//apple_ref/doc/uid/DTS40007324-Intro-DontLinkElementID_2)
[新浪微博](https://github.com/sinaweibosdk/weibo_ios_sdk)
[连连支付](http://open.lianlianpay.com/#cat=33)
[友盟统计分析](http://dev.umeng.com/analytics/ios-doc/sdk-download)
[微信](https://open.weixin.qq.com/cgi-bin/showdocument?action=dir_list&t=resource/res_list&verify=1&id=open1419319164&token=&lang=zh_CN)
[QQ](http://wiki.open.qq.com/wiki/IOS_API%E8%B0%83%E7%94%A8%E8%AF%B4%E6%98%8E)
[支付宝](https://doc.open.alipay.com/doc2/detail.htm?treeId=54&articleId=104509&docType=1)
[百度地图](http://lbsyun.baidu.com/index.php?title=iossdk/theupdatelog)
总之，对于ipv6_Only的处理就是这样办的，从两大方面进行自己审核：**自身网络请求和三方涉及网络请求。**
关于AFNetworking是否支持ipv－6，有网友如是说：
![](http://upload-images.jianshu.io/upload_images/1440398-4adedc07c5428579.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
可见，AFNetworking是支持ipv－6的。但是，关于支持ipv6的afn版本有如下说明：
![](http://upload-images.jianshu.io/upload_images/1440398-a734a71dacdec621.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
另外有网友遇到这样一个问题：这里还遇到一个坑，内网的访问下是不可能连接到自己的服务器，后面测试一下公网阿里云的服务器，能够正常连接，这个可能是DNS64在搜索ipv6的过程中，并没有搜索内网的网络，导致内网连接失败（这里卡了半天， 切记）（这个问题笔者并没有遇到，也没有亲测，大家注意一下这个问题，如果遇到了，知道是怎么回事。）
最后，有的网友上线被拒了，**可能是因为app的服务器端没有支持ipv6**，就此问题，我咨询了我们的技术经理，技术经理说“后台（即服务器）是要做ipv6的支持工作的”。但是具体是谁管，具体怎么操作的我就不得而知了，这里大家注意一下吧：不仅仅是客户端这边支持ipv6，上线之前最好问一下后台端是不是做了相应的ipv6的支持。
这里关于自己的服务器是否支持ipv6，有网友提供了一个方法，大家可以试一下。（谢谢杭州－托儿索[被拒6次的ipv6](http://www.jianshu.com/p/ba627e73e5b4)，争得托儿索的同意，进行转发）
![](http://upload-images.jianshu.io/upload_images/1440398-c4ec45923936e0f6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
前两条说的就是我们的方案：**项目里面涉及和网络有关的进行一一排查**。
第三条是在自己电脑上判断是否服务器支持ipv6的。亲测，在终端得到下图：
![](http://upload-images.jianshu.io/upload_images/1440398-38dcf0f9c8aa7a64.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
按照托儿索的说法，上图的位置那里显示NOERROR说明服务器没有问题，也是支持ipv6的。如果有错的话，可以按照下边提供的表格进行对照。
![](http://upload-images.jianshu.io/upload_images/1440398-0d72a3e49fc7469d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
再看托儿所的结论，我们会得到下边的结论，涉及支持ipv6的其实是包括三个方面的：
1、项目里面涉及和网络有关的**网络请求**是否支持ipv6。
2、项目里面涉及和网络有关的**sdk**是否支持ipv6。
3、项目的**服务器**是否支持ipv6。
最后，说一下关于托儿索的命令行：终端  dig +nocmd + nostats 你的域名 AAAA，我在网上查了一下发现有这么个命令行：
![](http://upload-images.jianshu.io/upload_images/1440398-248d13f66980bb56.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
为了方便复制：dig[www.isc.org](http://blog.itechol.com/link.php?url=http://www.isc.org%2F)AAAA +short 
![](http://upload-images.jianshu.io/upload_images/1440398-3f5faa15156cf954.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
得到的结果直接就是一个ipv6格式的，不知道不支持ipv6服务器的是不是打印出来的是不是ip地址（有待验证）。如果谁验证了，希望告知我一声。在此谢谢了。所以也可以用这个命令行侧面验证服务器是否支持ipv6。
6月14号23：30提的包，6月16号白天查到上线了。**说明一点：这样处理ipv6的方法是可行的。**
最后，哪里不对的地方可以给我留言，我会及时改进的，谢谢大家。
文／和珏猫（简书作者）
原文链接：http://www.jianshu.com/p/97b205933c15
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
