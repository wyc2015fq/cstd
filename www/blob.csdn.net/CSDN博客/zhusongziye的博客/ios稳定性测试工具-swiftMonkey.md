# ios稳定性测试工具-swiftMonkey - zhusongziye的博客 - CSDN博客





2017年09月26日 21:31:37[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：695









    17年初，开始着手ios专项测试。关于流畅度、内存，都在xcode中集成了现成工具，但是，8.3砍掉了UIAutomation 框架，导致几个常用的自动化工具，都失效了。

    7月份看到恒温的帖子，才知道有swiftMonkey这个工具，可以在最新的Xcode下跑。虽然不太懂swift代码，但是我们有iOS开发，拉着以为93年的开发妹子，个把小时，monkey就跑起来了![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/smiley/smiley_13.png?wx_lazy=1)。

    上操作。


1、swiftMonkey。（需要源码）

在github下载源码https://github.com/zalando/SwiftMonkey解压后如下：

![](https://img-blog.csdn.net/20170926213200395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（1）将SwiftMonkey文件夹，拷贝到ios工程目录下。


（2）将SwiftMonkey.xcodeproj文件拷贝到xcode的ios工程下。

![](https://img-blog.csdn.net/20170926213229668?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






（3）Xcode->editor->Add Target,选择 iOS
 UI Testing Bundle，点击next，Language选择swift。

![](https://img-blog.csdn.net/20170926213308894?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






（4）选中UITests，添加依赖。


（5）继续添加Copy Files。如图：

![](https://img-blog.csdn.net/20170926213352280?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



（6）打开UITests.swift文件，在testMonkey方法中编写脚本。如图：

![](https://img-blog.csdn.net/20170926213428221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



（7）此处放脚本

核心方法1

![](https://img-blog.csdn.net/20170926214029885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





核心方法2

![](https://img-blog.csdn.net/20170926213620185?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

核心方法3

![](https://img-blog.csdn.net/20170926213941905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




核心方法4

![](https://img-blog.csdn.net/20170926213704101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)












