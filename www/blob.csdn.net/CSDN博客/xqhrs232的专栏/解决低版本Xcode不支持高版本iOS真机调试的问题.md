# 解决低版本Xcode不支持高版本iOS真机调试的问题 - xqhrs232的专栏 - CSDN博客
2018年12月10日 17:29:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：56
原文地址::[https://blog.csdn.net/duotemplar/article/details/80355402](https://blog.csdn.net/duotemplar/article/details/80355402)
相关文章
1、解决低版本Xcode不支持高版本iOS真机调试----[https://blog.csdn.net/yancechen2013/article/details/67636498](https://blog.csdn.net/yancechen2013/article/details/67636498)
2、主题 : 真机测试 xcode版本低  iPhone版本高 出现的问题----[http://www.cocoachina.com/bbs/read.php?tid=1695922](http://www.cocoachina.com/bbs/read.php?tid=1695922)
今天在做真机调试的时候，遇到下面的一个问题： 
![这里写图片描述](https://img-blog.csdn.net/20171026231911247?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhbmd6aGlob25nOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
`Could not locate device support files. `
- 
`This iPhone 6s is running iOS 11.1 (15B93), which may not be supported by this version of Xcode.`
- 1
- 2
- 3
- 1
- 2
- 3
- 1
- 2
- 3
上面的意思是，在调试前我将我手机的iOS系统 升级到了最新版11.1版本，而我的Xcode9支持的最高版本是11.0的。这也不奇怪，手机肯定都是先推送的，这是苹果一贯的做法。那么遇到上面的问题我们怎么解决呢？
# 解决办法
1、复制一份旧的SDK，并重新命名为真机测试需要的SDK版本； 
具体做法是，找到路径: /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk （提示：要在应用程序中找到Xcode，右键点击 -> 显示包内容，以前Xcode版本的iOS SDK有的保存在系统根目录下）。
复制一份iPhoneOS.sdk，并命名为iPhoneOS11.1.sdk。如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20171026232057643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhbmd6aGlob25nOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2，新增真机调试包及内容 
打开路径： 
/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/DeviceSupport 。我在CocoaChina上找到了11.1的真机包，链接地址如下： 
[http://www.cocoachina.com/bbs/read.php?tid=1726904](http://www.cocoachina.com/bbs/read.php?tid=1726904)。然后我们打开DeviceSupport并复制一份。 
![这里写图片描述](https://img-blog.csdn.net/20171026233043238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhbmd6aGlob25nOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
具体做法是，创建一个新的真机需要的版本11.1(15B93)，然后将从 cocoachina下载的文件拷贝到这个地方即可。
3、修改SDKSettings.plist文件中的版本号
按照/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk 的顺序打开SDKSettings.plist 文件，将里面所有跟版本有关的数字都修改为11.1即可。再次运行就好了。 
![这里写图片描述](https://img-blog.csdn.net/20171026233457764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhbmd6aGlob25nOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
