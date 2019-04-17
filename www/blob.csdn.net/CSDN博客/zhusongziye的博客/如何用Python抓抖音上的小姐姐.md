# 如何用Python抓抖音上的小姐姐 - zhusongziye的博客 - CSDN博客





2018年10月18日 21:16:20[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：280








爬虫的案例我们已讲得太多。不过几乎都是**网页爬虫**。即使有些手机才能访问的网站，我们也可以通过 **Chrome 开发者工具**的**手机模拟**功能来访问，以便于分析请求并抓取。

但有些 **App** 根本就没有提供网页端，比如今年火得不行的**抖音**。（网上有些教程也是用网页手机模拟的方法，但此法现已失效。）

对于这种情况，我们能不能抓取？要怎么抓取？今天就来分享一下。

**手机抓包**

本文的重点就在于**如何****获取手机 App 发出的请求**。

手机 App 不像电脑上的网页能直接通过浏览器查看相关信息，在手机设备上也不方便使用工具一边流量一边调试。所以常用的方式就是通过在电脑上装一些**“抓包”软件**，将手机上的网络请求全部显示出来。

那为什么电脑能看到手机上的网络请求？这里就要提下“**代理**”这个概念。我们之前的文章中也讲过代理。形象的解释就是字面的理解：所有你发出的请求不再是直接发到目的地，而是先发给这个代理，再由代理帮你发出。所以通过代理，可以实现**隐藏 IP、进入专用网络、翻…咳咳那啥**等功能，也包括我们今天说的：**手机抓包**。

顺带说句，在公共场所别随便连不确定的免费 wifi，理论上来说，人家也可以抓你的包。

这里，我们要用的工具是 **Fiddler**。它是一个较成熟的免费抓包工具。可以抓取网页、桌面软件、手机 App 的网络请求，并可以运行在 Windows、Mac、Linux 平台上，支持 iOS 和 Android。（虽说都支持，但强烈建议** Windows + Android**，后面我会有吐槽）

上周我们的送书活动收到不少同学的项目和代码，其中 **@离岛** 同学提交了一个 Fiddler 手机抓包的教程。

https://segmentfault.com/a/1190000015571256

本文中部分内容和图片就转自她这篇文章。她的博客上还有不少文章和学习笔记，可以关注交流。也欢迎其他同学给我们投稿。

**下载安装**

搜索一下 fiddler 很容易找到它们的官网 https://www.telerik.com/fiddler，点击 download 下载即可（有个表格随便填下）。

Windows 下载后正常安装。如果是 Mac，还会有安装步骤提示，告诉你需要先安装一个叫做 **Mono** 的框架，以便可以执行 Fiddler.exe。另外 Mac 版还有几个小坑：

1. 运行 mono 命令用 **sudo**

2. 如果报一堆错闪退，请用 `mono --arch=32 Fiddler.exe`（这个参数还必须放在文件名前面）

3. 第一次正确运行时，程序**会卡住很长时间**，以至于我以为还是挂了，这时请耐心等待。（我要不是正好有事走开，回来发现成功了，可能就放弃尝试了）

4. 即使正常运行了，Mac 上界面也会有各种显示的 bug，切记不要打开的弹窗的情况下切换程序，不然回来就找不到弹窗了……

5. 软件中无法复制……

6. 在 iOS 上无法抓取 HTTPS 请求（这基本就是废了），需要额外创建一个证书，但这个证书工具只能在 Windows 下运行……

所以可以的话，还是用 **Windows** 来做。Mac 上还有个比较知名的工具 **Charles**，有用过的可以留言评价下。

**配置**

安装好工具后，需要做一些必要配置才能抓包。

**1. Fiddler 配置**

设置允许抓取 HTTPS 信息包。打开下载好的 fiddler，找到** Tools -> Options**，然后在 **HTTPS** 的工具栏下勾选 **Decrpt HTTPS traffic**，在新弹出的选项栏下勾选**Ignore server certificate errors**。这样，fiddler 就会抓取到 HTTPS 的信息包。

![](https://img-blog.csdn.net/20181018210949308?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

设置允许外部设备发送 HTTP/HTTPS 到 fiddler。设置**端口号**，并在 **Connections** 选项栏下勾选 **Allow remote computers to connect**。

![](https://img-blog.csdn.net/20181018211010578?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

配置好后需重启软件。

**2. 设置手机代理**

在抓包前，确保你的电脑和手机是在一个**可以互访的局域网中**。最简单的情况就是都连在同一个 wifi 上，特殊情况这里不展开讨论（有些商用 wifi 并不能互访）。 

打开软件，鼠标放在右上角的 **Online** 上可以看到**本机的 IP**。或者也可以通过命令行中的 **ipconfig** 命令（Mac/Linux 是 **ifconfig**）查看。（截图仅为演示，以你自己的 IP 为准）

![](https://img-blog.csdn.net/20181018211046594?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20181018211056889?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

手机设置代理 IP。打开手机**无线网络连接**，选择已经连接的网络连接，点击一个小圆圈叹号进入可以看到下图（安卓也类似），选择**配置代理**，进入后把刚刚的 **IP 地址**输入进去，**端口**就是 fiddler 中设置的 8888。

![](https://img-blog.csdn.net/20181018211122394?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**3. 安装证书**

获取** HTTPS **请求必须要**验证证书**。电脑端访问：http://localhost:8888/ 进行安装。

![](https://img-blog.csdn.net/20181018211143959?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

手机访问前面设置的电脑的 IP 地址加端口 8888 访问，比如图中例子是：http://192.168.23.1:8888

![](https://img-blog.csdn.net/20181018211159244?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

有些安卓需要手动从设置里进入并导入证书，否则无法生效。

**4. 测试**

开启 fiddler 的状态下，打开手机随便一个 APP，应对可以正常访问，并且在 fiddler 中看到所发出的网络请求。

![](https://img-blog.csdn.net/20181018211220887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果能访问但看不到请求，确认下有没有代理有没有生效。如果不能访问，检查下证书是否都下载并验证。还是不行则按照上述步骤再仔细配置一遍。

**分析请求**

完成这一步之后，接下来的事情就和网页爬虫没太大区别了。无非就是从这些请求中，找到我们需要的那几个。

fiddler 里记录的是所有请求，比较多。在操作 App 前，记得清空已有请求，方便观察。然后再配合上 **filter 筛选器**，定义筛选规则，会较容易找你需要的内容。找到请求后，在软件里查看你要的信息，或者右键点击选择将请求导出。

![](https://img-blog.csdn.net/20181018211254590?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

经过操作+观察，可以定位到获取用户上传视频列表的请求是

https://api.amemv.com/aweme/v1/aweme/post/?…

从 **WebForms** 栏里可以查看请求的详细参数信息。返回值是一个组 **JSON** 数据，里面包含了视频的下载地址。

![](https://img-blog.csdn.net/20181018211323625?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这是一个需要经验积累的活儿，不同的网站/App，规则都不一样，但套路是相似的。对网页爬虫还不熟悉的话，搜索文章 [爬虫必备工具，掌握它就解决了一半的问题](http://mp.weixin.qq.com/s?__biz=MjM5MDEyMDk4Mw==&mid=2650167181&idx=1&sn=afc1b8c3046dc78cb179ac75d246232f&chksm=be4b5af5893cd3e3008062d0a362b3069480685364d3c767e9264314011812a84ef796d48812&scene=21#wechat_redirect)。

**代码抓取**

得到地址之后，经过在浏览器和代码里的一番尝试，找到了此请求的正确解锁方式：

1. 需要提供以下参数：`max_cursor=0&user_id=94763945245&count=20&aid=1128`，其中**user_id **是你要抓取的用户 ID，其他参数都可以固定不用改。

2. 需要使用手机的 **User-Agent**，最简单的就是 `{'user-agent': 'mobile'}`

请求代码：

```
import requests as rs
uid = 94763945245
url = 'https://api.amemv.com/aweme/v1/aweme/post/?max_cursor=0&user_id=%d&count=20&aid=1128' % uid
h = {'user-agent': 'mobile'}
req = rs.get(url, headers=h, verify=False)
data = req.json()
print(data)
```

uid 替换成你想抓的用户 ID。获取用户 ID 有个简单方法：在用户页面选择分享，链接发到微信上，从网页打开就可以看到 user_id。

提取视频列表并下载：

```
import urllib.request
for video in data['aweme_list']:
    name = video['desc'] or video['aweme_id']    
    url_v = video['video']['download_addr']['url_list'][0]
    print(name, url_v, '\n')
    urllib.request.urlretrieve(url_v, name + '.mp4')
```

![](https://img-blog.csdn.net/20181018211421130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此方法截止中秋假期还是有效的，可以通过 Chrome 开发者工具进行模拟。之后能使用多久这就没法保证了，爬虫代码都不会是一劳永逸的。



总结下，重点是** fiddler 的抓取**，关键是** 配置、代理、证书**，难点是 **对请求的分析**。最终代码只有简单两步，**获取视频列表、下载视频**。

所有代码其实就上面两段，也上传了，获取地址请在公众号（Crossin的编程教室）回复关键字 **抖音**

想看其他十多个项目代码实例（电影票、招聘、贪吃蛇、代理池等），回复关键字 **项目**



下课！



回复关键字：

**python** ：零基础入门课程目录

**新手** ：初学者指南及常见问题

**资源** ：超过500M学习资料网盘地址

**项目** ：十多个进阶项目代码实例



如需了解视频课程及答疑群等更多服务，请号内回复 **码上行动**

代码相关问题可以在论坛上发帖提问 bbs.crossincode.com

![](https://img-blog.csdn.net/20181018211535894?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





