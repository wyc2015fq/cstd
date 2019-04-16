# android手机安全：被攻陷的一个场景 - 我相信...... - CSDN博客





2014年10月20日 16:53:30[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1509
个人分类：[手机安全																[Android](https://blog.csdn.net/wireless_com/article/category/789150)](https://blog.csdn.net/wireless_com/article/category/619365)








                 到处找WIFI,对于我们的手机控来说是相当普遍的了。如果你发现了有可用的wifi，并选择了浏览器连接，当浏览器出现一个web 页面的时候，你可能已经中招了。

同样，当你的手机使用一些免费应用的时候，经常会弹出一些广告页，这些广告的应用框架如果使用了web的前端技术，尤其是javascript，你也可能中招了。

这一切都祸起Javascript在android addJavascriptInterface API中的远程代码执行的Bug（CVE-2012-6636），这个Bug允许Javascript代码获得访问系统的更大权限。当app第一次运行时，它们通过HTTP下载javascript库。也就是说，App通常不安全地下载了未验证的javascript代码，而这些代码运行在可执行任意代码的环境中。

尽管权限提升技术在Android上很普遍（获得设备的“root”权限），但远程代码执行同样是一种罕见且危险得多的漏洞，它允许攻击者不经授权就在用户设备上执行特定代码。

我们应该养成“远程代码执行”与“root权限”在严重等级上等价的习惯，因为一个下定决心的黑客将可能从一个地方蹦到另一个地方，获取设备的完全控制权。

通俗地说，避免类似场景至少要采用下列两个措施：
1）将系统升级到andorid 4.2以上
2）尽量少用含有第三方广告的应用            



