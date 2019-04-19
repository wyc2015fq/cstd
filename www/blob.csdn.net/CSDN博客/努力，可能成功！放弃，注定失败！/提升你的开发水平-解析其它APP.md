# 提升你的开发水平-解析其它APP - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年02月21日 14:28:33[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：36标签：[提升你的开发水平-解析其它APP](https://so.csdn.net/so/search/s.do?q=提升你的开发水平-解析其它APP&t=blog)
要想提升iOS开发水平，最重要的还是多做项目，可是现实生活我们在负责一个app的时候可能半年甚至一年都是负责一个模块，可想而知，一年的时间不止是浪费这么简单的事了，时间就是金钱，怎么办呢？那就是分析其它市场上的app，复用app。下面我拿汽车之家这款app去做讲解。
一：解析汽车之家资源图片
1.app选择：首先金融类的app就算了，都是加密过的，你也看不到任何数据，首选新闻类，例如网易新闻之类，我这里选择汽车之家app，个人爱好，随便还可以看看汽车信息。
2.首先itunes store下载汽车之家app,下载好之后找到这个.ipa包如图：
![](https://upload-images.jianshu.io/upload_images/451169-0b0c62310e190882.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/838/format/webp)
3.将.ipa后缀改为.zip，如图
![](https://upload-images.jianshu.io/upload_images/451169-c46afcf772d172e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/914/format/webp)
4.解压zip，你会看到，如图
![](https://upload-images.jianshu.io/upload_images/451169-a8c9808c871ded3e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
5.打开文件夹，如图
![](https://upload-images.jianshu.io/upload_images/451169-a08d93ecee38bb0a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
6.你会看见Payload文件夹，打开文件夹，看到了这个文件，右键显示包内容
![](https://upload-images.jianshu.io/upload_images/451169-840aeae46a981b62.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
7.恭喜你，你看见了汽车之家所有的资源图片和.plist文件
![](https://upload-images.jianshu.io/upload_images/451169-314cf65d5c595b9f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
是不是很好玩，嘿嘿，接下来就是解析汽车之家的数据了。
二：使用Charles解析汽车之家app数据
如果你没有Charles工具的话可以去下载，下载地址：[Download a Free Trial of Charles • Charles Web Debugging Proxy](https://link.jianshu.com?t=http://www.charlesproxy.com/download/)
1.准备活动：设置代理，将电脑和手机设备置于同于无线网路环境下设置代理，将手机的代理地址设为和电脑无线ip地址一样，如图
![](https://upload-images.jianshu.io/upload_images/451169-dab449bc4aa7e04b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/668/format/webp)
电脑WiFi地址
![](https://upload-images.jianshu.io/upload_images/451169-c8845f13d558fa25.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
手机上同一wifi
  点击后面提示号，进入，选择手动
![](https://upload-images.jianshu.io/upload_images/451169-24928b9153777a18.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/599/format/webp)
填写地址和电脑WiFi地址一样，端口填写8888
2.打开Charles，点击界面齿轮
![](https://upload-images.jianshu.io/upload_images/451169-b46bb389c50c834d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
打开，选择proxy settings
![](https://upload-images.jianshu.io/upload_images/451169-d1e14205c5fc9f22.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
![](https://upload-images.jianshu.io/upload_images/451169-d16bb7c99e3a2a90.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/747/format/webp)
port填写8888
3.手机上运行汽车之家app，然后会看见Charles上显示
请求api
![](https://upload-images.jianshu.io/upload_images/451169-b50e96407e18fc4e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
呵呵，请求参数
![](https://upload-images.jianshu.io/upload_images/451169-d2ed5d6a69c88b76.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
这个当然是解析下来的数据了
![](https://upload-images.jianshu.io/upload_images/451169-69e4a91f750fca48.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
看到了把，数据是对的
总结：现在图片资源有，数据也有，还能做不出来汽车之家的原型吗，我相信这样学习可以很快的进步，如果你去汽车之家面试，你就可以直接带着重写的汽车之家的app去面试了，我想带着他们的app去面试成功机会会很大吧，同样去其他公司也一样，面试前花一天的时间去搞个面试公司的demo我想机会会更大吧。
备注：只有没有被加密的数据才可以被解析下来，大家可以找一些没加密的app去学习，愿大家越来越好。
如果您喜欢这篇文章，请关注iOS精华专题或Lizhe_iOS_Dev微信公众账号，会每天为大家收录好的文章，知识点和博客。
作者：家有海盗
链接：https://www.jianshu.com/p/35052a6acc7e
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
