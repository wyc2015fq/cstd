# sharesdk for android集成调试的几个问题 - =朝晖= - 博客园
# [sharesdk for android集成调试的几个问题](https://www.cnblogs.com/dhcn/p/7121300.html)
1、一定要下载最新版，这个东西目前版本升级很频繁，证明产品本身还不稳定，最新版bug会少一点
2、下载最新版SDK的时候，跟随下载最新Sample，官网文档的示例代码及时性很差。
3、调试的几个Key要凑齐:sharesdk的appid，分享目标平台开放应用的appkey和securitykey.
4、[微信](http://lib.csdn.net/base/wechat)和微博分享需要的签名原理是把你的[Android](http://lib.csdn.net/base/android) signkey导出一个原文信息，然后把这个原文信息md5计算校验值，方法很多种，可见此帖：http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/chen825919148/article/details/15340267
5、sharesdk for [android](http://lib.csdn.net/base/android) 目前还有问题，它最新版本支持似乎还跟不上android官方的版本，android官方目前最新API是23，sharesdk demo的target是21，其官方提供的cn.sharesdk.onekeyshare.theme.classic.EditPage类有明显语法错误，在release编译时，编译无法通过。
6、调试时容易出的问题：1、直接run的编译版本用的是调试sign key，所以无法通过微信的签名验证。所以必须手动导出签名key，然后adb install安装这个签名apk做[测试](http://lib.csdn.net/base/softwaretest)；2、权限配置一定要到位，这个是官方文档刚开始的步骤，一般不会出错。

