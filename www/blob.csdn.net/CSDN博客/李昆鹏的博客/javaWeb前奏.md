
# javaWeb前奏 - 李昆鹏的博客 - CSDN博客


2018年07月12日 10:32:26[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：28标签：[tomcat																](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)个人分类：[Tomcat																](https://blog.csdn.net/weixin_41547486/article/category/7796475)


----------------------------------javaWeb前奏--------------------------------
# 1. Javaweb前奏
## 1　C/S结构
C/S结构即客户端/服务器（Client/Server），例如QQ就是C/S结构的。我们每个人电脑上都需要安装QQ的客户端，客户端软件向服务器端软件发送请求，服务器端给客户端返回响应。
但是，因为客户端需要不断的更新，用户使用起来就比较麻烦。现在的C/S结构的客户端已经好多了，可以提示用户更新，用户只需要点击更新就可以了。最早时需要用户自己去官网上下载最新客户端，然后把老版本卸载，再去安装新版本。
基于客户端的安装
![](https://img-blog.csdn.net/20180712103036804?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2　B/S结构
B/S结构即浏览器/服务器（Browser/Server），例如网站都是B/S结构的。当然，网站也只是B/S结构体系软件中的一种而已。网上银行也都是B/S结构的！
B/S结构的好处在于不需要用户更新客户端，客户端只需要有一个浏览器就OK。当软件需要更新时，开发人员只需要关心服务器端就行。就像你在浏览器中访问百度时，发现百度的主页发生了变化，但你也不需要更新什么。
基于浏览器
![](https://img-blog.csdn.net/20180712103057720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 3　静态网页和动态网页
早期在Internet上都是静态网页，即html页面。静态网页的数据都是不能自动变化的，例如网页上的当前用户信息：“欢迎XXX登录本系统”，这说明当前用户名为XXX。如果有其他用户登录系统就应该显示其他用户的名字，而且无需修改页面！说白一点，用户名应该是变量，而不是常量！这是静态网页做不到的！
动态网页可以中包含变量，数据的变化就不是问题了。但动态网页只能在服务器端使用，*客户端浏览器只能识别静态网页。所以，如果用户请求的是动态网页，那么就需要服务器先把动态网页转换成静态网页发送给客户端浏览器！*
动态网页必须有服务器来处理。

## 4　常见的网页
**动态网页：**
php、asp、aspx
jsp、.do、.action
我们要学习的是JSP（Java server pages），与其相关的是do（struts1）、action（struts2）
**静态网页：**
htm、html

## 5　web服务器
Web服务器的作用是接收客户端的请求，给客户端作出响应。
知名Java Web服务器
l  Tomcat（Apache）：用来开发学习使用；免费，开源
l  JBoss（Redhat红帽）：；
l  Weblogic（Orcale）：要钱的！使用Tomcat开发，项目写完了，放到Weblogic上运行；
l  Websphere（IBM）：要钱的！与Weblogic相似。


