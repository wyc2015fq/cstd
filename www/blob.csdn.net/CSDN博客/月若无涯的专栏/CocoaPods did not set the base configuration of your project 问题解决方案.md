# CocoaPods did not set the base configuration of your project 问题解决方案 - 月若无涯的专栏 - CSDN博客
2015年07月16日 17:52:22[月若无涯](https://me.csdn.net/u010124617)阅读数：4450
今天在使用pod install的时候，出现了
[!] CocoaPods did not set the base configuration of your project because your project already has a custom config set. In order for CocoaPods integration to work at all, please either set the base configurations of the target
 `项目名` to `Pods/Target Support Files/Pods-项目名/Pods-项目名.release.xcconfig` or include the `Pods/Target Support Files/Pods-项目名/Pods-项目名.release.xcconfig` in your build configuration.
![](https://img-blog.csdn.net/20150716174040785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解决方案如下：
![](https://img-blog.csdn.net/20150716174615075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
把第3步箭头所指的地方改成如图所示的None即可。
我出现这问题是由之前的单个target改为了多个target，pod的目录结构发生了变化，此时旧的配置项仍然存在，影响了新的。
