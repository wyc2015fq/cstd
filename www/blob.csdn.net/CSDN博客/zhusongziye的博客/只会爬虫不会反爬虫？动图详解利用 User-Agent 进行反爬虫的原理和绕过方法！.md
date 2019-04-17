# 只会爬虫不会反爬虫？动图详解利用 User-Agent 进行反爬虫的原理和绕过方法！ - zhusongziye的博客 - CSDN博客





2018年11月19日 21:57:24[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：348








“ 阅读本文大概需要 10 分钟。”

随着 Python 和大数据的火热，大量的工程师蜂拥而上，爬虫技术由于易学、效果显著首当其冲的成为了大家追捧的对象，爬虫的发展进入了高峰期，因此给服务器带来的压力则是成倍的增加。企业或为了保证服务的正常运转或为了降低压力与成本，不得不使出各种各样的技术手段来阻止爬虫工程师们毫无节制的向服务器索取资源，我们将这种行为称为『反爬虫』。



『反爬虫技术』是互联网技术中为了限制爬虫而产生的技术总称，而反爬虫的绕过则是所有爬虫工程师要面对的问题，也是中高级爬虫工程师面试中最关注的方面。

# 问题所在

但是在平时的交流中，笔者发现大多数的初级爬虫工程师只会拿着网上别人写的技术文章唾沫横飞，除了知道在请求的时候伪造浏览器请求头信息中的 User-Agent 以外，对于：
- 
为什么要这么做？

- 
这么做有什么好处？

- 
我可以用别的方法实现么？

- 
它的原理是怎么样的？

- 
它是如何识别我的爬虫的？

- 
我应该用什么方式绕过它？


一无所知。如果你既不知道原理又不知道实现方式，那么当目标网站稍微调整一下反爬虫策略的时候，你还是一脸懵逼

对，就是一脸懵逼。

# 作者心声

我也在尝试着，能够将这样的知识分享出来，让大家在闲暇之余能够通过这篇文章学习到反爬虫知识中比较简单的反爬虫原理和实现方法，再熟悉他的绕过操作。比如 User-Agent 反爬手段，了解它的原理并且亲手实现反爬虫，再亲手绕过它。或许通过这个小小的案例，就可以打开你思维的大门、撬开你思路的下水道。



# 正文

上面是空谈，下面是实践。一位伟人曾经表达过这么一个意思：

> 
管你黑猫白猫，抓不到老鼠的猫，它就不是个好猫


### 什么是 User-Agent

User Agent中文名为用户代理，简称 UA，它是一个特殊字符串头，使得服务器能够识别客户使用的操作系统及版本、CPU 类型、浏览器及版本、浏览器渲染引擎、浏览器语言、浏览器插件等。一些网站常常通过判断 UA 来给不同的操作系统、不同的浏览器发送不同的页面，因此可能造成某些页面无法在某个浏览器中正常显示，但通过伪装 UA 可以绕过检测。浏览器向服务器发起请求的流程图，可以用下图表示：

![](https://img-blog.csdnimg.cn/20181119214755301.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



这里以火狐浏览器和谷歌浏览器为例，UA 的格式或者说表现形式是这样的：

Firefox 的 User-Agent：

```
Mozilla/5.0 (Macintosh; Intel Mac OS X 10.13; rv:63.0) Gecko/20100101 Firefox/63.0
```

Chrome 的 User-Agent：

```
Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.77 Safari/537.36
```

### User-Agent 在网络请求中充当什么角色？

> 
在网络请求当中，User-Agent 是标明身份的一种标识，服务器可以通过请求头参数中的 User-Agent 来判断请求方是否是浏览器、客户端程序或者其他的终端（当然，User-Agent 的值为空也是允许的，因为它不是必要参数）。


![](https://img-blog.csdnimg.cn/20181119214811494.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



浏览器的角色，如上图方框中所示，那么 User-Agent 的角色，就是表明身份。

### 为什么反爬虫会选择 User-Agent 这个参数呢？

从上面的介绍中，可以看出它是终端的身份标识。意味着服务器可以清楚的知道，这一次的请求是通过火狐浏览器发起的，还是通过 IE 浏览器发起的，甚至说是否是应用程序（比如 Python ）发起的。

网站的页面、动效和图片等内容的呈现是借助于浏览器的渲染功能实现的，浏览器是一个相对封闭的程序，因为它要确保数据的成功渲染，所以用户无法从浏览器中大规模的、自动化的获取内容数据。

而爬虫却不是这样的，爬虫生来就是为了获取网络上的内容并将其转化为数据。这是两种截然不同的方式，你也可以理解为通过编写代码来大规模的、自动化的获取内容数据，这是一种骚操作。



> 
回到正题，为什么会选择 User-Agent 这个参数呢？


因为编程语言都有默认的标识，在发起网络请求的时候，这个标识在你毫不知情的情况下，作为请求头参数中的 User-Agent 值一并发送到服务器。比如 Python 语言通过代码发起网络请求时， User-Agent 的值中就包含 Python 。同样的，Java 和 PHP 这些语言也都有默认的标识。

### 反爬虫的黑名单策略

既然知道编程语言的这个特点，再结合实际的需求，那么反爬虫的思路就出来了。这是一中黑名单策略，只要出现在黑名单中的请求，都视为爬虫，对于此类请求可以不予处理或者返回相应的错误提示。

![](https://img-blog.csdnimg.cn/20181119214925358.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



### 为什么用黑名单策略不用白名单策略？

现实生活中，浏览器类型繁多（火狐浏览器、谷歌浏览器、360 浏览器、傲游浏览器、欧普拉浏览器、世界之窗浏览器、QQ 浏览器等），



想要将所有的浏览器品牌、类型以及对应的标识收集并放到名单中，那是不实际的，假如漏掉了哪一种，那么对网站来说是一种损失。

再者说来，很多的服务并不仅仅开放给浏览器，有些时候这些服务以 API 的形式向应用程序提供服务，比如安卓软件的后端 API ，为安卓软件程序提供数据服务，而软件本身只承担界面和结构的任务，而数据则从后端 API 获取。这个时候，发起的请求中， User-Agent 就会变成 Android 。

以上就是不能使用白名单策略的原因。

而黑名单在于简单，当你希望屏蔽来自于 Python 代码的请求或者来自于 Java 代码的请求时，只需要将其加入黑名单中即可。

### 通过 Nginx 服务日志来查看请求头中的 User-Agent

Nginx 是一款轻量级的 Web 服务器/反向代理服务器及电子邮件（IMAP/POP3）代理服务器。其特点是占有内存少，并发能力强，事实上 Nginx 的并发能力确实在同类型的网页服务器中表现较好，使用 Nginx 企业有：百度、京东、新浪、网易、腾讯、淘宝等。



Nginx 的安装与启动

通常可以使用系统本身的安装工具（Centos 的 yum、Debian 系的 apt-get 以及 MacOS 的 brew）安装 Nginx，以 linux 系统为例，在终端中输入：

```
sudo apt-get install nginx
```



接下来根据提示选择，即可完成 Nginx 的安装。

接着在终端通过命令:

```
sudo systemctl start nginx
```

![](https://img-blog.csdnimg.cn/20181119215022125.gif)



即可启动 Nginx 服务。

> 
备注：由于各个系统差别以及版本差异，安装和启动命令略有差别，解决办法自行搜索


Nginx 的日志

Nginx 为用户提供了日志功能，其中记录了每次服务器被请求的状态和其他信息，包括 User-Agent。 Nginx 的默认日志存放路径为：

```
/var/log/nginx/
```

在终端通过命令

```
cd /var/log/nginx && ls
```

可以进入到日志存放目录并列出目录下的文件，可以看到其中有两个主要的文件，为`access.log` 和 `error.log`

![](https://img-blog.csdnimg.cn/20181119215100549.gif)



它们分别记录着成功的请求信息和错误信息。我们通过 Nginx 的访问日志来查看每次请求的信息。

发起请求的几种办法

浏览器

Nginx 启动后，默认监听 80 端口，你只需要访问 IP 地址或者域名即可。假设 IP 地址为`127.0.0.1`，那么可以在浏览器输入：

```
http://127.0.0.1
```

回车后，浏览器就会向服务器发起请求，和你平时上网是一样的。

Python 代码

这里我们利用 Requests 库来发起网络请求。在本地新建一个名为 `gets.py`的文件，其中代码为：

```
import requests
# 向目标发起请求，并打印返回的 http 状态码
resp = requests.get("http://127.0.0.1")
print(resp.status_code)
```

![](https://img-blog.csdnimg.cn/2018111921513034.gif)



Postman

Postman是一款功能强大的网页调试与发送网页HTTP请求的工具（Postman下载地址），它可以模拟浏览器，访问指定的 Url 并输出返回内容，实际使用如下图所示：

![](https://img-blog.csdnimg.cn/20181119215157386.gif)



Curl

这是一个利用URL语法在命令行下工作的传输工具，它不仅支持 url 地址访问还支持文件上传和下载，所以可以称它为综合传输工具。他也可以模拟浏览器，访问指定的 Url，实际使用如下图所示：

![](https://img-blog.csdnimg.cn/20181119215223684.gif)



Nginx 日志记录结果

上面使用了 4 种方法来向服务器发起请求，那么我们看看 Nginx 的日志中，记录了什么样的信息。在终端通过命令：

```
sudo cat access.log
```

来查看日志文件。可以看到这几次的请求记录：

```
# 请求记录
127.0.0.1 - - [04/Nov/2018:22:19:07 +0800] "GET / HTTP/1.1" 200 396 "-" "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36"
127.0.0.1 - - [04/Nov/2018:22:19:07 +0800] "GET /favicon.ico HTTP/1.1" 404 200 "http://127.0.0.1/" "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36"
127.0.0.1 - - [04/Nov/2018:22:20:36 +0800] "GET / HTTP/1.1" 304 0 "-" "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36"
127.0.0.1 - - [04/Nov/2018:22:27:14 +0800] "GET /z_stat.php?id=1256772952&web_id=1256772952 HTTP/1.1" 404 144 "http://appstore.deepin.org/" "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/538.1 (KHTML, like Gecko) deepin-appstore/4.0.9 Safari/538.1"
127.0.0.1 - - [04/Nov/2018:22:42:10 +0800] "GET / HTTP/1.1" 200 396 "-" "PostmanRuntime/7.3.0"
127.0.0.1 - - [04/Nov/2018:22:42:51 +0800] "GET / HTTP/1.1" 200 612 "-" "curl/7.60.0"
```

![](https://img-blog.csdnimg.cn/2018111921525029.gif)



无论是 Python 还是 Curl 或者浏览器以及 Postman 的请求，都被记录在日志文件中，说明 Nginx 可以识别发起请求的终端类型。

**实现反爬虫**

之前的理论和逻辑，在实验中都得到了验证，那么接下来我们就通过黑名单策略将 Python 和 Curl 发起的请求过滤掉，只允许 Firefox 和 Postman 的请求通过，并且对被过滤的请求返回 403 错误提示。

![](https://img-blog.csdnimg.cn/20181119215310697.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



反爬虫的过程如上图所示，相当于在服务器和资源之间建立了一道防火墙，在黑名单中的请求将会被当成垃圾丢弃掉。

**配置 Nginx 规则**

Nginx 提供了配置文件以及对应的规则，允许我们过滤掉不允许通过的请求，本次反爬虫我们使用的就是它。Nginx 的配置文件通常放在`/etc/nginx/`目录下,名为`nginx.conf`，我们通过查看配置文件来看一看，站点的配置文件在什么地方。再通过系统自带的编辑器（笔者所用系统自带 Nano，其他系统可能自带 Vim）来编辑配置文件。在配置文件中找到站点配置文件地址（笔者所用电脑存放路径为`/etc/nginx/sites-enable`），再到站点配置文件中找到`local`级别的配置，并在其中加上一下内容：

```
if ($http_user_agent ~* (Python|Curl)) {
     return 403;
    }
```

这段配置的释义是判断请求中请求头字符串中是否包含有 Python或者 Curl，如果包含则直接返回 403 错误，否则返回正常的资源。完成配置后保存，再通过命令：

```
sudo nginx -s reload
```

![](https://img-blog.csdnimg.cn/20181119215407806.gif)



整个操作过程如上图所示，让 Nginx 服务器重新载入配置文件，使得刚才的配置生效。

**反爬虫效果测试**

重复上面访问的步骤，通过浏览器、Python 代码、Postman 工具和 Curl发起请求。从返回的结果就可以看到，与刚才是有所区别的。
- 
浏览器返回的是正常的页面，说明没有收到影响；

- 
Python 代码的状态码变成了 403，而不是之前的 200

- 
Postman 跟之前一样，返回了正确的内容；

- 
Curl 跟 Python 一样，无法正确的访问资源，因为它们发起的请求都被过滤掉了。


![](https://img-blog.csdnimg.cn/20181119215456921.gif)

![](https://img-blog.csdnimg.cn/20181119215531888.gif)







**提示**：你可以继续修改 Nginx 的配置来进行测试，最终会发现结果会跟现在的一样：只要在黑名单中，请求就会被过滤掉并且返回 403 错误。

> 
提示：这就是你平时编写爬虫代码时，需要在请求头中伪造浏览器的原因。


绕过 User-Agent 方式的反爬虫

通过上面的学习，我们知道了 User-Agent 反爬虫这种手段的原理，并且通过 Nginx 来实现了反爬虫，接下来我们一起学习如何绕过这种反爬虫措施。

Python 绕过反爬虫

在 Requests 库中，允许用户自定义请求头信息，所以我们可以在请求头信息中将 User-Agent 的值改为浏览器的请求头标识，这样就能够欺骗 Nginx 服务器，达到绕过反爬虫的目的。将之前的 Python 代码改为：

```
import requests
# 伪造请求头信息 欺骗服务器
headers = {"User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.13; rv:9527.0) Gecko/20100101 Firefox/9527.0"}
resp = requests.get("http://127.0.0.1", headers=headers)
print(resp.status_code)
```

代码中我们用到的是 Firefox 浏览器的请求头信息，而且为了更好的观察效果，我们可以更改浏览器的版本号（改成9527）以区分真实浏览器（这不会影响请求结果）。运行这个文件，看看得到的返回结果：

```
200
```

不是 403 了，说明已经绕过了这种类型的反爬虫（你看，这就是网上那些文章所写的，需要修改请求头信息才能绕过反爬虫，现在你明白是怎么回事了吧）。

练习：使用 Postman 再测试一下

一个测试也许不准确，你还可以通过 Postman 再来测试一下，还记得怎么做吗？
- 
将需要过滤的标识（Postman）添加到 Nginx 的配置文件中

- 
重载配置文件，使其生效

- 
通过 Postman 发起请求看看是否会被过滤

- 
再次使用 Postman 工具，并且携带上浏览器的标识再发起请求，看看是否会被过滤


> 
小提示：这个练习如果你自己来做的话，会更容易理解其中的原理，并且可以加深你的映像。


# 总结

回顾一下，整篇文章的过程：

![](https://img-blog.csdnimg.cn/20181119215554139.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



我们从遇到的反爬虫现象开始入手，接着学习了 User-Agent 这种反爬虫策略的原理，并且通过 Nginx 实现了反爬虫，最后通过 Python 代码示例和 Postman 示例来验证我们的想法，最终清清楚楚、明明白白的了解到其中的缘由，待目标改变了它的策略时，我们也可以清楚的知道可以使用哪些方法来绕过。

『反爬虫报告简版PDF』下载链接: 接: https://pan.baidu.com/s/1spujrFunlpyXGYJExjeDgQ 提取码 提取码: tgn2



