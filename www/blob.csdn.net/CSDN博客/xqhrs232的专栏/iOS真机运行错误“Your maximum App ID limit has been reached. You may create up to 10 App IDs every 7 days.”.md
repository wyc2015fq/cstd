# iOS真机运行错误“Your maximum App ID limit has been reached. You may create up to 10 App IDs every 7 days.” - xqhrs232的专栏 - CSDN博客
2019年02月27日 17:35:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：131
原文地址::[https://www.jianshu.com/p/db1afeeb776d](https://www.jianshu.com/p/db1afeeb776d)
今天做了一个demo，然后在真机运行的时候，报了个
Your maximum App ID limit has been reached. You may create up to 10 App IDs every 7 days.错误，
![](https://upload-images.jianshu.io/upload_images/1743453-57675d574f2fd7d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/864/format/webp)
1
在TARGETS显示为
![](https://upload-images.jianshu.io/upload_images/1743453-78aa3e34a43b4a31.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/662/format/webp)
2
字面意思就是说我的这个账号达到了最大的app ID数量，因为我的是免费账号，所以每7天只能最多创建10个app ID，所以在真机运行的时候就会报这个错误
解决方法：
1.换一个apple ID，既然每个账号只能一周最多创建10个app ID，那我们可以用另外的账号来重新登录，
2.因为我们一般都只有一个apple ID，或者来回更换apple ID比较麻烦，我们可以把以前能够真机运行的demo的bundle ID拷贝到将要运行的这个项目来，这样就能继续使用了
如下：
![](https://upload-images.jianshu.io/upload_images/1743453-04316e6b85a80f85.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/470/format/webp)
3
我这里是把以前的一个bundle ID直接拷贝过来，现在能够正常运行了。
总结：我们可以把我们能够真机运行的bundle ID给保存下来，做个列表之类的，下次运行的时候我们可以先把之前运行过的项目（但是现在已经不在手机上运行或者以后也很少运行的项目）的bundle ID拿来先使用，而不用xcode自动生成的bundle ID，因为一般情况下七天10个app ID已经足够我们使用了
