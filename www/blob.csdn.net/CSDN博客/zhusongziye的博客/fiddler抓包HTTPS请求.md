# fiddler抓包HTTPS请求 - zhusongziye的博客 - CSDN博客





2017年11月25日 20:37:19[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：537









 前段时间后台消息有人问我fiddler怎么抓https包的，今天给大家找了教程。还有公众号的留言功能已经开通了，大家之后有啥需要的可以在文章留言告诉小编，不然消息的话后台保存时间不长，然后今天也有人要那个fiddler的视频教程，小编发了点时间找了一下居然找到了，分享给大家。看文末哦！






    来看看fiddler怎么抓HTTPS请求吧




一、安装fiddler




首先准备一台可以上网的windos电脑，准备一部智能手机。


打开fiddler随便打开下浏览器。发现已经可以抓包，但想要抓手机https还需要做一些设置。


fiddler打开既视感

![](https://img-blog.csdn.net/20171125203301006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

二、配置fiddler




1、打开fiddler配置Tools –> Fiddler Options.

![](https://img-blog.csdn.net/20171125203312701?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



2、打开fiddler配置


打开HTTPS配置项，勾选“CaptureHTTPS CONNECTs”，同时勾选“Decrypt HTTPS traffic”，弹出的对话框选择是（这里是按照fiddler自己的证书）如果跟我一样手机跟电脑是用wifi进行链接的话还需要选择“…fromremote clients only”。如果需要监听不可信的证书的HTTPS请求的话，需要勾选“Ignore servercertificate errors”。

![](https://img-blog.csdn.net/20171125203323179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



打开Conections配置项， 这里可以修改Fiddler代理端口号。勾选“Allow remote computersto connect。提示需要重启fiddler。

![](https://img-blog.csdn.net/20171125203333205?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



需要写点代码。这里是为了可以抓客户端使用httpURLConnection的包。


Ruler –>CustomizeRules


在函数OnBeforeResponse里面添加下面代码：

![](https://img-blog.csdn.net/20171125203543661?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



添加后代码为：



![](https://img-blog.csdn.net/20171125203611944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






不要忘记重启fiddler！！！



三、配置手机




保证手机跟pc是在同一个网段下。


配置手机连接的wifi，可能每个手机打开wifi配置的方式都不太一样，仔细研究下，选择已经连接的网络，打开修改网络窗口。显示高级属性，配置网络代理–>手动 代理服务器主机名：填写pc机的IP地址即可例如：192.168.0.4，代理服务器端口号：fiddler的代理端口号，如果没有修改就是8888。


关键步骤哈：


下载安装fiddler的证书

![](https://img-blog.csdn.net/20171125203630137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



在手机浏览器打开 代理服务器：端口号 例如上面填写的就是192.168.0.4:8888，点击页面中的“FiddlerRootcertificate”链接，在弹出的对话框中随便设置一个名称确定即可。




抓包截图




测试机上没装多少软件，来抓包试试

![](https://img-blog.csdn.net/20171125203641099?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





好了。大功告成！


送上福利：免费哦！


fiddler工具链接：http://pan.baidu.com/s/1mil7QFA 密码：8eb1



fiddler视频教程：https://pan.baidu.com/s/1eSfF4no 密码: hvv7 


大家帮忙推广一下博客，介绍给你身边的好友即可，谢谢大家！






