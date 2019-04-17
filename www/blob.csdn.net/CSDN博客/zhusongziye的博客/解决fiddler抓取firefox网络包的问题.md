# 解决fiddler抓取firefox网络包的问题 - zhusongziye的博客 - CSDN博客





2017年10月12日 19:51:54[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：439








1.Fiddler是位于客户端和服务器端的HTTP代理，它能够记录客户端和服务器之间的所有 HTTP请求，可以针对特定的HTTP请求，分析请求数据、设置断点、调试web应用、修改请求的数据，甚至可以修改服务器返回的数据，功能非常强大，是web调试的利器。客户端的所有请求都要先经过Fiddler，然后转发到相应的服务器，反之，服务器端的所有响应，也都会先经过Fiddler然后发送到客户端，基于这个原因，Fiddler支持所有可以设置http代理为127.0.0.1:8888的浏览器和应用程序。使用了Fiddler之后，web客户端和服务器的请求如下所示：

![](https://img-blog.csdn.net/20171012195423740?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2.Fiddler 作为系统代理，当启用 Fiddler 时，IE 的PROXY 设定会变成 127.0.0.1:8888，因此如果你的浏览器在开启fiddler之后没有设置相应的代理，则fiddler是无法捕获到HTTP请求的。如下是启动Fiddler之后，IE浏览器的代理设置：

![](https://img-blog.csdn.net/20171012200141787?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



3.以Firefox为例，默认情况下，firefox是没有启用代理的（如果你安装了proxy等代理工具或插件，是另外一种情况），在firefox中配置http代理的步骤如下：

![](https://img-blog.csdn.net/20171012200423599?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4、fiddler设置

1.打开菜单栏：Tools>Fiddler Options>HTTPS，2.勾选Decrypt HTTPS traffic，里面的两个子菜单也一起勾选了

![](https://img-blog.csdn.net/20171012200637992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

5、在firefoxs上安装fiddler证书

fiddler是一个很好的抓包工具，默认是抓http请求的，对于pc上的https请求，会提示网页不安全，这时候需要在浏览器上安装证书。



例如：提示网页不安全

1.用fiddler抓包时候，打开百度网页：https://www.baidu.com

2.提示：网页不安全

![](https://img-blog.csdn.net/20171012200840794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




导出证书：



1.点右上角Actions按钮

2.选第二个选项，导出到桌面，此时桌面上会多一个文件：FiddlerRoot.cer,如图。
![](https://img-blog.csdn.net/20171012201024323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将导入到firefox浏览器

1.打开右上角浏览器设置》选项》高级》证书》查看证书》证书机构》导入

![](https://img-blog.csdn.net/20171012201120821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

勾选文件导入

![](https://img-blog.csdn.net/20171012201206023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

打开文件后，会弹出个框，勾选三个选项就完成操作啦。

![](https://img-blog.csdn.net/20171012201238434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果还不能成功，那就重启浏览器，重启电脑了，再不行重新安装fiddler!







