# fiddler更新后证书导出和报错的坑 - zhusongziye的博客 - CSDN博客





2017年09月12日 22:42:04[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：11013








          用过fiddler的，大家都知道它的强大。不过每次打开它总会弹出提示更新的对话框，很烦！在设置里可以取消掉更新提示！

在fiddler正常使用的情况下，不建议去更新新版本。



打开fiddler时update更新提示框：

![](https://img-blog.csdn.net/20170912224740436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

证书导出报错：The root certificate could not be located.

![](https://img-blog.csdn.net/20170912225653979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

打开浏览器浏览网页时证书报错：Creation of the root certificate was not succesful.

![](https://img-blog.csdn.net/20170913201749390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



       没办法，小编为了解决fiddler这两个问题（1、export failed: the root certificate could not be located; 2、certificate error: creation of the root certificate was not successful.)。小编试了自动安装证书的方法，结果成功。

1、首先确保安装的 Fiddler 是较新的版本，比较老的版本可能会出现安装不上fiddler证书生成器的问题



2、关闭Fiddler。

3、下载并安装Fiddler证书生成器。（注：Fiddler 证书生成器只能在 Vista 以上系统运行）

下载地址：[http://www.telerik.com/docs/default-source/fiddler/addons/fiddlercertmaker.exe?sfvrsn=2](http://www.telerik.com/docs/default-source/fiddler/addons/fiddlercertmaker.exe?sfvrsn=2)



还有一个方法：



尝试卸载重新安装fiddler。重新安装前，最好把以前的证书都找到并删除。

![](https://img-blog.csdn.net/20180630142123611?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装最新版本，小编是从360软件管家里，直接搜索安装的，安装成功后打开fiddler后设置好，即可正常从fiddler导出证书，并成功导入浏览器中。

一切就ok啦。。。

码代码不容易，分享一个支付宝小红包给大家，算是给小编的一点鼓励吧，谢谢支持！

![](https://img-blog.csdn.net/20181020163804827?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20181020163956700?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



