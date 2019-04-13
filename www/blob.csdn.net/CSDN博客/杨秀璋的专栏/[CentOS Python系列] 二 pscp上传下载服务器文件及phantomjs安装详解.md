
# [CentOS Python系列] 二.pscp上传下载服务器文件及phantomjs安装详解 - 杨秀璋的专栏 - CSDN博客

2018年02月17日 00:04:03[Eastmount](https://me.csdn.net/Eastmount)阅读数：947所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)[CentOS云服务端之Python爬虫及数据库知识](https://blog.csdn.net/column/details/19699.html)



从2014年开始，作者主要写了三个Python系列文章，分别是基础知识、网络爬虫和数据分析。
Python基础知识系列：Pythonj基础知识学习与提升
Python网络爬虫系列：Python爬虫之Selenium+Phantomjs+CasperJS
Python数据分析系列：知识图谱、web数据挖掘及NLP
它们都是基于Windows系统下的Python编程，每个系列都从安装过程、基础知识到实际应用三个方面进行讲解，但在写这些文章的时候，始终有两个缺陷：一是没有介绍Linux系统下的Python编程，并且所有代码使用的都是Python 2.7版本；另一方面是如何结合服务器，真实的项目中，通常会将Python代码托管到服务器中。
![这里写图片描述](https://img-blog.csdn.net/20180212160809122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161506304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161524640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
同时，随着人工智能和深度学习的风暴来临，Python变得越来越火热，作者也准备从零学习这些知识，写相关文章。本篇文章是作者学习部署阿里云服务器 CentOS环境，讲解pscp工具如何将Windows系统文件上传到云服务器中，同时将云服务器文件下载到本地；同时教大家如何安装phantomJS，它是一个无界面的浏览器，被广泛应用于Selenium自动化测试和爬虫中。
文章非常基础，希望这系列文章对您有所帮助，如果有错误或不足之处，还请海涵。



## 一. pscp上传下载文件
首先在Windows系统中下载pscp工具（pscp.exe），将其放置于putty软件目录下，然后输入Ctrl+R快捷键，打开CMD命令行模式，输入如下指令：

```python
cd C:\Software\Programe Software\Putty
pscp ?
```
运行结果如下图所示，可以查看pscp命令的用法。

![](https://img-blog.csdn.net/20180216200832937)

**1.上传文件至云服务器**
命令：pscp 本地文件 用户名@ip地址：远端路径
比如将本地中文字体文件simsun.ttf上传到服务器中，调用指令为：
```python
pscp C:\20180216\simsun.ttf root@39.107.105.166:/root/
```
![](https://img-blog.csdn.net/20180216204145567)

将C盘20180216文件夹下的字体文件上传到服务器用户名为root的根目录下，注意需要输入服务器密码。

![](https://img-blog.csdn.net/20180216204123718)

然后登陆云服务器，可以看到simsun.ttf文件已经上传。

![](https://img-blog.csdn.net/20180216204451860)

**2.下载文件至Windows系统**
命令：pscp  用户名@ip地址：远端路径  Windows文件路径
比如将阿里云服务器中的HelloWorld.py文件和test.py两个文件下载至本地C盘20180216文件夹，代码如下：
```python
pscp root@39.107.105.166:/root/test.py C:\20180216
pscp root@39.107.105.166:/root/HelloWorld.py C:\20180216
```
![](https://img-blog.csdn.net/20180216205938435)

Linux中cat命令用来查看源代码，rm命令用来删除文件（remove），如下图所示：

![](https://img-blog.csdn.net/20180216210201105)

在CMD中执行该命令，如下所示：

![](https://img-blog.csdn.net/20180216210214217)

可以看到Windows系统C:\20180216文件夹下已经成功下载了该文件，同时注意符号 \ 和 / 别写错。

![](https://img-blog.csdn.net/20180216210327248)

**3.上传文件夹至云服务器**
命令：pscp  -r  Windows文件夹  用户名@ip地址：远端路径
其中“-r”表示向下递归目录。现在需要将Windows系统的PhantomJS文件夹上传到云服务器root目录下，文件夹如下图：

![](https://img-blog.csdn.net/20180216212114862)

CMD命令行中输入命令为：
```python
pscp -r C:\phantomjs root@39.107.105.166:/root/
```
![](https://img-blog.csdn.net/20180216212943111)
运行结果如下图所示，可以在服务器root目录下看到phantomjs文件夹，并包含各文件。

![](https://img-blog.csdn.net/20180216213024633)

**4.下载文件夹至Windows系统**
命令：pscp  -r  Windows文件夹  用户名@ip地址：远端路径
将云服务器中“root/eastmount”文件夹及文件下载至本地20180216文件夹，运行如下所示：
```python
C:\Software\Program Software\Putty>pscp -r root@39.107.105.166:/root/eastmount C:\20180216\
root@39.107.105.166's password:
first.py                  | 0 kB |   0.1 kB/s | ETA: 00:00:00 | 100%
bsfirst.py                | 0 kB |   0.3 kB/s | ETA: 00:00:00 | 100%
```
本地显示结果如下图所示：

![](https://img-blog.csdn.net/2018021621330458)

讲到这里pscp基本用法已经讲述结束，它主要用来Windows与Linux系统交互，但更多的可以在线下载安装。作者这里想上传Phantomjs来执行代码，但总是报错“Messag: phantomjs.exe executable may have wrong permissions.”。
下面详细讲解如何在Linux系统中下载Phantomjs并安装。


## 二. Phantomjs安装详解
PhantomJS是一个服务器端的 JavaScript API 的Webkit（开源的浏览器引擎）。其支持各种Web标准： DOM 处理, CSS 选择器, JSON, Canvas 和 SVG。PhantomJS可以用于页面自动化，网络监测，网页截屏，以及无界面测试等。
PhantomJS is a headless WebKit scriptable with a JavaScript API. It has fast and native support for various web standards: DOM handling, CSS selector, JSON, Canvas, and SVG.Full web stack No browser required.

首先补充下yum和apt-get的区别，一般来说Linux系统基本分为两大类：
RedHat系列：RedHat、CentOS、Fedora等，常见安装包格式命令为“rpm -参数”，包管理工具是yum，如“yum install xxx”，支持tar包。
Debian系列：Debian、Ubuntu等，常见安装包格式命令为“dpkg -参数”，包管理工具是apt-get，支持tar包。
当我们执行如下代码时，提示错误“-bash: phantomjs: command not found”，表示还未安装该包。
```python
phantomjs -v
yum install phantomjs
```
同时，执行“yum install phantomjs”代码，没有可得到的扩展包，下面我们开始正式讲解吧。
![](https://img-blog.csdn.net/20180216225140272)

作者准备将Phantomjs安装包下载至/root/eastmount文件夹下。

![](https://img-blog.csdn.net/20180216225457579)

**第一步 下载安装包**
```python
wget https://bitbucket.org/ariya/phantomjs/downloads/phantomjs-1.9.7-linux-x86_64.tar.bz2
```
![](https://img-blog.csdn.net/20180216225634419)

**第二步 解压文件**
```python
tar -xjvf phantomjs-1.9.7-linux-x86_64.tar.bz2
```
如下图所示：
![](https://img-blog.csdn.net/20180216225946163)

解压后调用“ls”命令查看eastmount文件夹，包含了已经解压的phantomjs-1.9.7-linux-x86_64文件夹。
![](https://img-blog.csdn.net/20180216225806222)


**第三步 重命名文件夹并移动至/usr/local/phantomjs目录**
```python
mv phantomjs-1.9.7-linux-x86_64 /usr/local/phantomjs
```
调用“cd /usr/local/”命令去到指定目录下，在查看该目录所包含内容，可以看到已经解压的phantomjs文件夹。

![](https://img-blog.csdn.net/20180216230224274)

**第四步 建立连接，将可执行文件放入系统路径**
去到“/usr/local/bin”文件夹下，可以看到此时还没有Phantomjs连接，接下来需要建立连接。
```python
ln -s /usr/local/phantomjs/bin/phantomjs /usr/bin/
ln -s /usr/local/phantomjs/bin/phantomjs /usr/local/bin/
```
建立连接过程如下所示：

![](https://img-blog.csdn.net/20180216230719897)

建立后的连接，可以看到phantomjs已经放入了系统文件夹，我的理解该步骤相当于Windows系统下的配置环境变量PATH。

![](https://img-blog.csdn.net/20180216230843498)

![](https://img-blog.csdn.net/20180216230903429)

**第五步 安装依赖包**
```python
yum install fontconfig
yum install freetype2
```
![](https://img-blog.csdn.net/20180216231022416)

**第六步 检验Phantomjs是否安装成功**
```python
phantomjs -v
```
可以看到我们安装的版本为phantomjs 1.9.7。

![](https://img-blog.csdn.net/2018021623120429)



## 三. Phantomjs基础代码

下面简单讲解Phantomjs基础代码，参考我的文章：[在Windows下安装PhantomJS和CasperJS及入门介绍(上)](http://blog.csdn.net/eastmount/article/details/47023199)
**1.Hello World**
首先我们在root文件夹下创建一个test.js文件，命令如下：
```python
ls
touch test.js --创建文件
vim test.js   --编辑文件
```
![](https://img-blog.csdn.net/2018021623120429)

test.js代码如下：
```python
console.log("Hello CSDN!!!");
phantomjs.exit();
```
![](https://img-blog.csdn.net/20180216231745559)

输入“phantomjs test.js”执行代码输出“Hello CSDN!!!”。

![](https://img-blog.csdn.net/20180216232016105)

**2.获取网页标题**
创建baidu.js文件，并编辑代码。

![](https://img-blog.csdn.net/20180216233200773)
baidu.js代码：
```python
var page = require('webpage').create();
page.open('http://www.baidu.com', function (status) {
     var title = page.evaluate(function () {
         return document.title;
     });
     console.log('Page title is ' + title);
     phantom.exit();
});
```
![](https://img-blog.csdn.net/20180216233249393)

输出结果如下图所示“百度一下，你就知道”。

![](https://img-blog.csdn.net/20180216233324333)

**3.简单下载图片**
同样的方法创建一个baidupic.js文件，代码如下：
```python
var page = require('webpage').create();  
page.open('http://www.baidu.com', function () {  
    page.render('example.png');  
    phantom.exit();  
});
```
![](https://img-blog.csdn.net/20180216233417787)

输出结果如下，可以看到“example.png”被下载至root目录下。

![](https://img-blog.csdn.net/20180216233454235)

同时在安装过程中，您可能会遇到如下错误：
安装phantomjs后输入phantomjs --v提示：-bash:phantomjs:command not found

讲到这篇文章就介绍结束了，本文的目的是为了后面的Selenium自动化测试及爬虫操作，希望文章对您有所帮助，同时会继续深入学习，包括爬虫过程、定时爬虫、数据库操作、服务器搭建等内容。狗年第一篇文章，祝大家新年快乐，娜我们就一起进步成长吧~
(By：Eastmount CSDN 秀璋 2018-02-17 深夜1点http://blog.csdn.net/Eastmount)

|引子 · 真正的缘分都是在天上缔结的
|By: Eastmount
|也许我们不在附中见面，也不再师大的林间相遇。
|隔三年，也会在遵义支教的教室认识，
|如果不在遵义的教室认识，你的学生也并未成为我的学生。
|隔七年，也一定会在上海、南京、苏州邂逅，
|秀璋沿着娜娜的足迹，紧紧跟随，
|你在前，我在后，你在奔跑，我在漫步。
|如果七年之中，我们都只是擦肩而过，我只能沿着你的足迹不断追寻。
|隔了九年，也一定会........在贵阳认识。
|总之，你的教书梦我来完成，我的低情商你来弥补。
|可是如果那样，我们都已经老了啊！
|老了......也很美。
|丘比特的这支箭，是奇幻的一箭，
|灵魂的伴侣终究还是走在了一起。
|果真，再高的情商都抵不过淳朴，再富裕的人生都比不了温馨。
|是啊！璋与娜的缘分早已在天上缔结，这便是所谓的命中注定吧！
|


