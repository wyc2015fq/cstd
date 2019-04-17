# fiddler导出jmeter脚本 - zhusongziye的博客 - CSDN博客





2018年01月20日 11:20:22[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：2286








  我们知道jmeter的脚本来源有以下几种，  badboy录制、jmeter自带的录制功能、手动书写（使用fiddler/wireshark来抓包，然后构造协议写脚本）、fiddler抓包之后直接导出jmeter脚本。今天主要是来分享下fiddler导出jmeter脚本笔记，因为将fiddler抓取的请求，导出为jmx格式，方便jmeter直接调用。




**（1）安装fiddler  jmeter（免安装）**

注意事项！fiddler版本必须在v4.6.2以上（插件支持的是4.6版本），

jmeter版本最好在v3.0以上，版本太低容易导致导出不成功

这里我自己安装的fiddler版本是v5.0 ，jmeter版本是v3.0

fiddler jmeter 插件安装包获取链接：https://pan.baidu.com/s/1dGZBvzF 密码：i0j4

![](https://img-blog.csdn.net/20180120111500138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**（2）移动dll文件到安装位置下的ImportExport文件**

将上述jmeter插件解压出来的ImportExport文件中的两个dll文件移动至放在fiddler根目录下的ImportExport文件   下，插件支持两种导出模式：一种是处理了IP和端口，使其为空；一种是保持原样输出

先找到fiddler的安装位置，我的安装位置是：C:\Users\Amay\AppData\Local\Programs\Fiddler

Amay是计算机名，安装系统时命名的一个

![](https://img-blog.csdn.net/20180120111511798?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

移动到ImportExport文件后的截图：

![](https://img-blog.csdn.net/20180120111521180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

移动完成后，重启fiddler




**（3）打开fiddler，设置过滤条件**

打开fiddler，切换到filters，以下数字标示代表操作步骤顺序

Show only the following Hosts： 只捕获你对该主机名的请求

Hide if URL contains：隐藏url中包含对某些字符的请求

![](https://img-blog.csdn.net/20180120111538543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对于fiddler过滤器学习可以自行百度学习，有些帖子介绍的很详细



**（4）测试场景：我们打开百度网页www.baidu.com  搜索：黑夜小怪**

然后我们在操作的时候会看到fiddlerz捕捉的实时情况

![](https://img-blog.csdn.net/20180120111553057?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

fiddler除了可以抓取http协议，还可以抓取https，我上面的测试场景就是抓取的https协议，因为这个问题期间我花费了很长时间

找到一篇比较靠谱的参考帖子，http://blog.csdn.net/d1240673769/article/details/74298429

 以下是我自己的操作设置过程：

**①.先在fiddler---Tools----Options---按截图勾选**

![](https://img-blog.csdn.net/20180120111608430?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**②.我使用的浏览器是firefox，**

打开火狐浏览器的配置----代理----

![](https://img-blog.csdn.net/20180120111625724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180120111635947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**③****.开始-----输入certmgr.msc并回车，打开证书管理**

![](https://img-blog.csdn.net/20180120111645376?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180120111658108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后将找出来的证书全部进行删除，记住！！是全部删除！！！

**④****打开火狐浏览器，进入选项-高级-证书-查看证书，然后找以DO_NOT开头的关于Fiddler的证书，删除！个人、服务器、证书机构、其他等标签下的以DO_NOT开头的关于Fiddler的证书也全部删除！**

![](https://img-blog.csdn.net/20180120111712511?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180120111725246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




**⑤****下载 FiddlerCertMaker.exe**

**安装证书生成器，下载地址：****http://www.enhanceie.com/dl/FiddlerCertMaker.exe**

**下载后直接打开安装-----出现下面界面---****点击确定，关掉**

![](https://img-blog.csdn.net/20180120111735854?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**⑥****重启Fiddler，重启之后，比如访问百度首页，即协议是https的网站，可能会出现以下截图提示**

![](https://img-blog.csdn.net/20180120111747626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



打开fiddler---Tools----Options--HTTPS----点击Actions，然后点击出现的第一个Trust
 Root Certificate，再点击Export Root Certificate to Desktop，桌面就有导出的证书了。

![](https://img-blog.csdn.net/20180120111758619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

桌面导出的证书

![](https://img-blog.csdn.net/20180120111806612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

⑦证书导入到桌面后，打开firefox浏览器，然后进入证书-查看证书-导入-选择刚导出的桌面的证书--点击确定。Fiddler重启，火狐浏览器也重启一下，然后开始抓HTTPS的包，此时你会发现“
 你的连接并不安全” 等类似提示已经消失，并且已经能够抓包了。

（5）开始导出jmeter脚本,导出到指定的文件夹

![](https://img-blog.csdn.net/20180120111815713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180120111825291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（6）**启动jmeter ，打开刚刚保存的文件**

打开脚本，对每个请求改下名字，然后再加响应断言 聚合报告 察看结果数

运行一下-----可以从察看结果数看到最后的结果。




![](https://img-blog.csdn.net/20180120111837327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

设置断言：

![](https://img-blog.csdn.net/20180120111849634?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




![](https://img-blog.csdn.net/20180120111857931?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

写在最后的话：

        我也是刚刚在接触fiddler jmeter wireshark 等等，第一次写分享比较笨拙，以后要学习的东西还很多，一起加油。在此谢谢黑夜小怪的资料分享，谢谢他的指点和鼓励，依然记得那句话：相信自己，成长从总结开始。也谢谢其他小伙伴的帮助，每次我问问题的时候，大家都积极回答我，给我意见，我很开心也很感谢。今天听到有位老师这样讲：找到志同道合的人一起进步，也是一件快乐的事。是的，能够进步，是件很很快乐的事情。






