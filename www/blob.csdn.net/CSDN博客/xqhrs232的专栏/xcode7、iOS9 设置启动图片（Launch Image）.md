# xcode7、iOS9 设置启动图片（Launch Image） - xqhrs232的专栏 - CSDN博客
2019年03月12日 11:51:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：51
原文地址::[https://blog.csdn.net/riven_wn/article/details/49275157](https://blog.csdn.net/riven_wn/article/details/49275157)
一.添加启动图片
点击Assets.xcassets进入图片管理,右击,弹出"New Launch Image"或点下面的+号创建Launch Image；
如图,右侧的勾选可以让你选择是否要对ipad,横屏,竖屏,以及低版本的ios系统做支持.这边我选了ios8.0和ios7.0。
二.拖入相应尺寸的图片，以本例子需要为例。
我试了一下这里的图片名字可以任意命名，不过格式必须为png格式，如果只是写个demo，也可以下载我所用的这套图片http://download.csdn.net/download/riven_wn/9196015。
640*960   (4/4s)                                 2X位置
640*1136  （5/5s/5c）                      R4位置
750*1334   （6）                               R4.7位置
1242*2208  （6 plus）                      R5.5位置
三.General里面的设置。
如图，Launch Image Source 要设置为LaunchImage，Launch Screen File的内容删除为空。
四.要把LaunchScreen.storyboard（低版本Xcode为LaunchScreen.xib）中的用作LaunchScreen的复选框勾选取消掉，如图。
到这里就大功告成了，so easy，不过有时候会出现第一次运行显示不正常的情况，一般把App删除重新run一遍就好了。
如果觉得显示时间不能满足需求，可以添加此方法延长显示时间(此为swift版本，oc类似)
override func viewDidLoad() 
{
        super.viewDidLoad()
        NSThread.sleepForTimeInterval(3.0)//延长3秒
}
另外，还有常用的利用LaunchScreen.xib（LaunchScreen.storyboard）加载启动页和广告，可参考：欢迎界面Launch Screen动态加载广告
补充：之前有人出现，用LaunchScreen.xib真机运行的时候出现黑屏的情况，在网上看到这篇文章说是图片名字的问题，我亲自试了一下，发现用launchScreen.png不定性的出现黑屏，所以也贴在这里供大家参考。
--------------------- 
