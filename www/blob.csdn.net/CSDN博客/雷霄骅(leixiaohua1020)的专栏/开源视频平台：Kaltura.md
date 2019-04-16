# 开源视频平台：Kaltura - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月22日 00:16:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：44
个人分类：[开源多媒体系统																[开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1652801)](https://blog.csdn.net/leixiaohua1020/article/category/1362938)









Kaltura是一个很优秀的开源视频平台。提供了视频的管理系统，视频的在线编辑系统等等一整套完整的系统，功能甚是强大。

![](https://img-blog.csdn.net/20131220154126343)

Kaltura不同于其他诸如Brightcove，Ooyala这样的网络视频平台，主要区别表现在：一方面，它的代码是开源的，可以免费部 署到企业；另一方面，它拥有一个模块化结构，可以让用户根据自己的需要购买平台的部分服务，而不是像其他公司那样采取一刀切的购买模式；最后它允许客户连 接到Kaltura自己的托管平台，在后台部署自己的防火墙，为用户提供最大的安全保障。由于上述的产品优势，Kaltura已经在一些企业客户、政府机构和学术机构中被广泛采用，这些组织可以使用Kaltura来建立内部的视频服务网站，用于员工的工作、培训和交流。同时，Kaltura也有自己的开发者社区，开发者可以开发自己的插件和模块系统，目前已经有超过4000家开发商同Kaltura签约。

本来想自己部署一个Kaltura Community 版本的系统的。但是它要求操作系统是Linux的，而它提供的虚拟机版本的软件体积又太大，所以就不自己配置了，直接自己注册了一个Kaltura的账号体验一下它的视频服务。

![](https://img-blog.csdn.net/20131220161139328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这次我拿自己学校注册了一个账号（Communication University of China）。

注册过程不细说。免费账号一共有10GB 的视频空间，包含了以下内容：

**MediaSpace服务**（类似于优酷，土豆这类的视频网站）
 属于自己的视频门户，可以创建，分享，搜索，观看视频。

**KMC - （Kaltura Management Console，Kaltura管理控制台）服务**
 上传视频，图像，音频文件并管理这些内容，可以把这些内容嵌入自己的网站中。



注册完成后会收到2封邮件：

Your Kaltura MediaSpace video portal is waiting for you：这封邮件告知有关MediaSpace服务的内容

Registration Confirmation for your Kaltura Video Service：这封邮件告知有关KMC服务的内容

通过邮件里的地址（其实通过官网首页也可以）就可以登录以上2个系统了。



**KMC - （Kaltura Management Console，Kaltura管理控制台）服务**

首先来看看KMC服务。系统的登录界面如图所示，是使用Flash做的。

![](https://img-blog.csdn.net/20131220160841390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

登录KMC之后，欢迎界面如图所示。右边是视频访问的统计信息。左边分为上传内容，嵌入内容到网站，自定义播放器。

![](https://img-blog.csdn.net/20131220160846109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击上方的“Content”，可以管理多媒体内容。系统中已经包含了一些Kaltura的宣传片，logo，特性介绍等内容。

![](https://img-blog.csdn.net/20131220160904437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在这里我已经上传几个内容，单击打开一个内容：中国传媒大学校歌 年轻的白杨

![](https://img-blog.csdn.net/20131220162343796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对话框中包含了视频的名称，描述等信息。并且提供了视频的预览功能。

左边有一个列表。包含了一些其他的功能，默认打开的是“Metadata”。这里我们打开“Thumbnails”。可以设置视频的截图。

![](https://img-blog.csdn.net/20131220162946406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其他功能很多，就不一一介绍了。

**MediaSpace服务**

再来看看MediaSpace服务。

点击邮件里的地址，就可以访问自己的视频门户了。刚才在KMC中上传的视频在这里也可以显示出来。

![](https://img-blog.csdn.net/20131220164816187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击自己上传的视频，就可以浏览视频的内容。

![](https://img-blog.csdn.net/20131220164838218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以登录管理后台，登陆界面如图所示。

![](https://img-blog.csdn.net/20131220164850609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

具体的登录配置还没有仔细研究。但是可以看出配置选项还是很多的。

![](https://img-blog.csdn.net/20131220164855828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



PS：Kaltura官方网站官方网站上大部分帮助信息，文档等等都是视频的，很生动。它在每年还有自己的年度大会，可以看出它现在已经发展的相当成熟了。

Kaltura官方网站：[http://www.kaltura.com/](http://www.kaltura.com/)

Kaltura Community Edition 官方网站（开源）：[http://www.kaltura.org/](http://www.kaltura.org/)





