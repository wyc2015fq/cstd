# Python如何爬取《王者荣耀盒子》APP - zhusongziye的博客 - CSDN博客





2018年03月07日 22:34:17[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：235











运行平台

Windows

Python版本

Python3.x


IDE

Sublime text3


什么是Fiddler？




Fiddler是位于客户端和服务器端的HTTP代理，也是目前最常用的http抓包工具之一 。 它能够记录客户端和服务器之间的所有 HTTP请求，可以针对特定的HTTP请求，分析请求数据、设置断点、调试web应用、修改请求的数据，甚至可以修改服务器返回的数据，功能非常强大，是web调试的利器。



![](https://img-blog.csdn.net/20180307222656104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




利用可以设置代理的这个特点，我们就可以对手机APP进行抓包了。怎么设置？不急不急，让我先把Fiddler安装上吧！

Fiddler下载地址：Fiddler下载–>点我

傻瓜式安装，一键到底。Fiddler软件界面如图所示：

![](https://img-blog.csdn.net/20180307222728259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



手机抓包设置 

### Fiddler设置

打开Fiddler软件，打开工具的设置。

(Fiddler软件菜单栏：Tools->Options)



![](https://img-blog.csdn.net/20180307222802803?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在HTTPS中设置如下

![](https://img-blog.csdn.net/20180307222814849?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在Connections中设置如下，这里使用默认8888端口，当然也可以自己更改，但是注意不要与已经使用的端口冲突

![](https://img-blog.csdn.net/20180307222838680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




  安全证书下载  

在电脑浏览器中输入地址：`http://localhost:8888/`

点击`FiddlerRoot certificate`，下载安全证书



![](https://img-blog.csdn.net/20180307222908661?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  安全证书安装  



证书是需要在手机上进行安装的，这样在电脑Fiddler软件抓包的时候

手机使用电脑的网卡上网才不会报错。

将下载好的`FiddlerRoot.cer`安装证书拷贝到手机中

然后进行证书安装(以小米5手机为例)

设置(Settings)->(系统和设备中的)更多设置->系统安全->从存储设备安装：

![](https://img-blog.csdn.net/20180307222935649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后找到拷贝的`FiddlerRoot.cer`进行安装即可。

安装好之后，可以在信任的凭证中找到我们已经安装好的安全证书

在用户中可以看到证书如下：



![](https://img-blog.csdn.net/20180307223035238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  局域网设置 

首先，查看电脑的IP地址，在`cmd`中使用命令`ipconfig`查看电脑IP地址。

找到无线局域网WLAN的IPv4地址，记下此地址。



![](https://img-blog.csdn.net/2018030722304933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在手机上，点击连接的WIFI进行网络修改，添加代理。进行手动设置，

主机名即为上图中找到的IP地址，端口号即为Fiddler设置中的端口号8888：



![](https://img-blog.csdn.net/20180307223100886?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  Fiddler手机抓包测试 

![](https://img-blog.csdn.net/20180307223114740?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



先来个简单的例子热热身，我想将《英雄联盟盒子》中的英雄图片下载下来，该如何操作？



![](https://img-blog.csdn.net/20180307223200640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

先使用Fiddler抓包看一下，在手机APP《英雄联盟盒子》中的菜单中点击`英雄`，电脑Fiddler就会抓包如下：



![](https://img-blog.csdn.net/20180307223240630?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看到，GET请求的url地址，和返回的JSON格式的信息。那么编写代码如下：




![](https://img-blog.csdn.net/20180307223255703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

从代码运行结果中可以看到，百里玄策的英雄ID为104，他的图片存放地址为`http://pic.wankacn.com/2017-08-04_59844776a6adf.png`：


![](https://img-blog.csdn.net/20180307223311814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

知道了这些信息，我们就可以将这些英雄的图片进行下载了，编写代码如下：



![](https://img-blog.csdn.net/20180307223329601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

运行上述代码，看下结果，72个英雄的图片，分分钟搞定：


![](https://img-blog.csdn.net/20180307223354905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

是不是很简单








