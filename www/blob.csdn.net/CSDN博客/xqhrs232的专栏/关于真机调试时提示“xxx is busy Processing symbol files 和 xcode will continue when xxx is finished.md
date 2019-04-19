# 关于真机调试时提示“xxx is busy:Processing symbol files"和"xcode will continue when xxx is finished" - xqhrs232的专栏 - CSDN博客
2018年12月07日 11:19:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：133
原文地址::[https://www.jianshu.com/p/fdbe7ec1ec44](https://www.jianshu.com/p/fdbe7ec1ec44)
相关文章
1、Xcode真机调试设备连接一直繁忙状态----[https://blog.csdn.net/sily_13/article/details/52698907](https://blog.csdn.net/sily_13/article/details/52698907)
2、XXX is busy: Processing symbol files----[https://blog.csdn.net/lymm000/article/details/69484083](https://blog.csdn.net/lymm000/article/details/69484083)
3、xcode will continue when iPhone is finished iPhone is busy:Processing symbol files----[https://blog.csdn.net/u014599371/article/details/79970486?utm_source=blogxgwz1](https://blog.csdn.net/u014599371/article/details/79970486?utm_source=blogxgwz1)
4、Xcode真机调试出现“ is busy: Processing symbol files”   ----[http://www.cocoachina.com/bbs/read.php?tid-1678685.html](http://www.cocoachina.com/bbs/read.php?tid-1678685.html)
5、Xcode真机调试出现“ is busy: Processing symbol files”----[https://blog.csdn.net/joker_wlk/article/details/52299007](https://blog.csdn.net/joker_wlk/article/details/52299007)
6、iOS Xcode真机调试设备连接时，Divice里面一直提示“iphone名称” is busy: Processing symbol files.----[https://blog.csdn.net/csdn_hhg/article/details/79770068](https://blog.csdn.net/csdn_hhg/article/details/79770068)
之前在苹果手机真机测试时遇到过如下提示
![](https://upload-images.jianshu.io/upload_images/2570919-18cc0f79a509d0b4.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/840/format/webp)
后来重启手机，重启xcode，甚至重启mac，过几分钟就可有调试真机了。当时也没多想，以为真的是手机的cpu在忙别的东西。最近在清理mac电脑时，才发现这个提示的真正原因：在一台手机上装东西，比如这台手机系统是10.3.3，如果之前电脑没有在10.3.3系统的手机上运行过调试的app，那么就会通过USB将手机里的一些东西拷贝到电脑的/Users/zhangxicheng/Library/Developer/Xcode/iOS DeviceSupport目录下。基本上一个系统要占2到3个G，在拷贝文件期间，咱们做什么都是没用的。拷贝完成后就可以调试app了。所以只需要静静等待几分钟的时间就可以了。
另外：我在清理mac时发现，这个iOS DeviceSupport目录下的东西比较多，我大概有十几个系统，也就是占了40多个G，想删掉，但是感觉下次连接时还要接着拷贝，比较纠结。
作者：小西ios
链接：https://www.jianshu.com/p/fdbe7ec1ec44
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
