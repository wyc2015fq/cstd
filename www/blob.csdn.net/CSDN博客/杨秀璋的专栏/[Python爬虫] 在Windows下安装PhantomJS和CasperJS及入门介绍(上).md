
# [Python爬虫] 在Windows下安装PhantomJS和CasperJS及入门介绍(上) - 杨秀璋的专栏 - CSDN博客

2015年08月19日 04:38:21[Eastmount](https://me.csdn.net/Eastmount)阅读数：17956所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)



最近在使用Python爬取网页内容时，总是遇到JS临时加载、动态获取网页信息的困难。例如爬取CSDN下载资源评论、搜狐图片中的“原图”等，此时尝试学习Phantomjs和CasperJS来解决这个问题。这第一篇文章当然就是安装过程及入门介绍。

## 一. 安装Phantomjs
下载地址：[http://phantomjs.org/](http://phantomjs.org/)
官网介绍：
PhantomJS is a headless WebKit scriptable with a JavaScript API. It has fast and native support for various
 web standards: DOM handling, CSS selector, JSON, Canvas, and SVG.
Full web stack No browser required.
PhantomJS是一个服务器端的 JavaScript API 的[WebKit](http://baike.baidu.com/link?url=2KTqOPM_VIV6Etls4s1esrsIGg6ntkgi5tP46b2gqxR1V319nkYrwPpUBZWMxKYQvRHYUEuBrFxXNRKk7vWGmq)（开源的浏览器引擎）。其支持各种Web标准：
 DOM 处理, CSS 选择器, JSON, Canvas 和 SVG。PhantomJS可以用于页面自动化，网络监测，网页截屏，以及无界面测试等。
下载PhantomJS解压后如下图所示：
![](https://img-blog.csdn.net/20150723171253026)
在该文件夹下创建test.js文件，代码如下：
```python
console.log('Hello world!');
phantom.exit();
```
通过Ctrl+R打开CMD调用phantomjs.exe执行该程序输出如下图所示：
![](https://img-blog.csdn.net/20150723171454294)
参考官方文档：[http://phantomjs.org/documentation/](http://phantomjs.org/documentation/)
**1、脚本参数-arguments.js**
同时其自带的examples文件夹中有很多模板代码，其中获取脚本参数代码如下：
```python
var system = require('system');
if (system.args.length === 1) {
    console.log('Try to pass some args when invoking this script!');
} else {
    system.args.forEach(function (arg, i) {
            console.log(i + ': ' + arg);
    });
}
phantom.exit();
```
运行程序及输出结果如下图所示：
**phantomjs examples/arguments.js arg0 agr1 arg2 arg3**
![](https://img-blog.csdn.net/20150723171741536)

**2、网页截图**
在根目录新建文件loadpic.js，其代码如下：
```python
var page = require('webpage').create();
page.open('http://www.baidu.com', function () {
    page.render('example.png');
    phantom.exit();
});
```
运行程序结果如下图所示：
**phantomjs loadpic.js**
![](https://img-blog.csdn.net/20150723172014075)
短短5行代码让我第一次体会到了PhantomJS和调用脚本函数的强大，它加载baidu页面并存储为一张PNG图片，这个特性可以广泛适用于网页快拍、获取网页在线知识等功能。同时也感受到了似乎能够解决我最初的加载JS问题。

**3、页面加载-Page Loading**
A web page can be loaded, analyzed, and rendered by creating a web page object.
通过创建一个网页对象，一个网页可以被加载，分析和渲染。examples文件夹中的loadspeed.js脚本加载一个特殊的URL (不要忘了http协议) 并且计量加载该页面的时间。
```python
var page = require('webpage').create(),
    system = require('system'),
    t, address;
if (system.args.length === 1) {
    console.log('Usage: loadspeed.js <some URL>');
    phantom.exit(1);
} else {
    t = Date.now();
    address = system.args[1];
    page.open(address, function (status) {
        if (status !== 'success') {
            console.log('FAIL to load the address');
        } else {
            t = Date.now() - t;
            console.log('Page title is ' + page.evaluate(function () {
                return document.title;
            }));
            console.log('Loading time ' + t + ' msec');
        }
        phantom.exit();
    });
}
```
运行程序如所示：
**phantomjs examples/loadspeed.js http://www.baidu.com**
其中包括document.title获取网页标题和t=Date.now()-t计算网页加载时间。此时输出如下图所示，但会存在中文乱码，如何解决呢？
![](https://img-blog.csdn.net/20150819030947586)
添加如下代码即可：
t = Date.now();
address = system.args[1];
**phantom.outputEncoding="gbk";**

![](https://img-blog.csdn.net/20150819031248582)


**4.代码运算-Code Evaluation**
通过在网页上下文中对JavaScript代码进行计算，使用evaluate()方法。代码是在“沙箱(sandboxed)”中运行的，它没有办法读取在其所属页面上下文之外的任何JavaScript对象和变量。evaluate()会返回一个对象，然而它仅限制于简单的对象并且不能包含方法或闭包。
下面这段代码用于显示网页标题：
```python
var page = require('webpage').create();
page.open('http://www.csdn.net', function(status) {
  var title = page.evaluate(function() {
    return document.title;
  });
  phantom.outputEncoding="gbk";
  console.log('Page title is ' + title);
  phantom.exit();
});
```
输出如下图所示：
![](https://img-blog.csdn.net/20150819033032561)
任何来自于网页并且包括来自evaluate()内部代码的控制台信息，默认不会显示的。要重写这个行为，使用onConsoleMessage回调函数，前一个示例可以被改写成：
```python
var page = require('webpage').create();
phantom.outputEncoding="gbk";
page.onConsoleMessage = function(msg) {
  console.log('Page title is ' + msg);
};
page.open('http://www.csdn.net', function(status) {
  page.evaluate(function() {
    console.log(document.title);
  });
  phantom.exit();
});
```
调用phantomjs gettile2.js即可。
**5.DOM操作-DOM Manipulation**
因为脚本好像是一个Web浏览器上运行的一样，标准的DOM脚本和CSS选择器可以很好的工作。这使得PhantomJS适合支持各种页面自动化任务。
参考[page automation tasks](http://phantomjs.org/page-automation.html)
下面的 useragent.js（examples文件样本）将读取id 为myagent的元素的 textContent 属性：
```python
var page = require('webpage').create();
console.log('The default user agent is ' + page.settings.userAgent);
page.settings.userAgent = 'SpecialAgent';
page.open('http://www.httpuseragent.org', function (status) {
    if (status !== 'success') {
        console.log('Unable to access network');
    } else {
        var ua = page.evaluate(function () {
            return document.getElementById('myagent').innerText;
        });
        console.log(ua);
    }
    phantom.exit();
});
```
输入如下指令，获取id=myagent元素的值：
**phantomjs examples/useragent.js**
![](https://img-blog.csdn.net/20150819035222385)
上面示例也提供了一种自定义user agent的方法。
使用JQuery及其他类库（Use jQuery and Other Libraries）。如果版本是1.6，你也可以把jQuery放入你的页面中，使用page.includeJs如下：

```python
var page = require('webpage').create();
page.open('http://www.sample.com', function() {
  page.includeJs("http://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js", function() {
    page.evaluate(function() {
      $("button").click();
    });
    phantom.exit()
  });
});
```
The above snippet will open up a web page, include the jQuery library into the page, and then click on all buttons using jQuery. It will then exit from the web page. Make sure
 to put the exit statement within the page.includeJs or else it may exit prematurely before the javascript code is included.
即需要确保JavaScript代码中包括引用的页面存在。The Webpage instance具体用法参考前面官方文档。
**6.网络请求及响应 – Network Requests and Responses**
当一个页面从一台远程服务器请求一个资源的时候，请求和响应均可以通过 onResourceRequested 和 onResourceReceived 回调方法追踪到。文档示例 netlog.js：
```python
var page = require('webpage').create(),
    system = require('system'),
    address;
if (system.args.length === 1) {
    console.log('Usage: netlog.js <some URL>');
    phantom.exit(1);
} else {
    address = system.args[1];
    page.onResourceRequested = function (req) {
        console.log('requested: ' + JSON.stringify(req, undefined, 4));
    };
    page.onResourceReceived = function (res) {
        console.log('received: ' + JSON.stringify(res, undefined, 4));
    };
    page.open(address, function (status) {
        if (status !== 'success') {
            console.log('FAIL to load the address');
        }
        phantom.exit();
    });
}
```
输入指令：
**phantomjs examples/netlog.js http://www.baidu.com**
输出部分内容：
```python
received: {
    "contentType": "text/javascript; charset=gbk",
    "headers": [
        {
            "name": "Server",
            "value": "bfe/1.0.8.5"
        },
        {
            "name": "Date",
            "value": "Tue, 18 Aug 2015 20:10:03 GMT"
        },
        {
            "name": "Content-Type",
            "value": "text/javascript; charset=gbk"
        },
        {
            "name": "Content-Length",
            "value": "88"
        },
        {
            "name": "Connection",
            "value": "keep-alive"
        },
        {
            "name": "Cache-Control",
            "value": "private"
        }
    ],
    "id": 13,
    "redirectURL": null,
    "stage": "end",
    "status": 200,
    "statusText": "OK",
    "time": "2015-08-18T20:09:38.085Z",
    "url": "https://sp0.baidu.com/5a1Fazu8AA54nxGko9WTAnF6hhy/su?wd=&json=1&p=3&
sid=16486_16222_1421_16896_16738_12825_12868_16800_16659_16424_16514_15936_12073
_13932_16866&csor=0&cb=jQuery110208203572703059763_1439928574608&_=1439928574609
"
}
```
获取如何把该特性用于HAR 输出以及基于YSlow的性能分析的更多信息，请参阅网络监控页面：[network monitoring](http://phantomjs.org/network-monitoring.html)
下面显示了从英国广播公司网站获得典范的瀑布图（waterfall diagram）：
![](https://img-blog.csdn.net/20150819041834700)
PS：其他本分参考[官方文档](http://phantomjs.org/quick-start.html)，目录如下，examples中包括每个js对应的用途、github中源代码、Troubleshooting等。
![](https://img-blog.csdn.net/20150819042503709)

## 二. 安装CasperJS
下载地址：[http://casperjs.org/](http://casperjs.org/)
官方文档：[http://docs.casperjs.org/en/latest/](http://docs.casperjs.org/en/latest/)
PS：准备下一篇文章介绍

**参考资料：**
[
用CasperJs自动浏览页面-by:kiwi小白 CSDN](http://blog.csdn.net/kiwi_coder/article/details/36248353)
[
PhantomJS安装及快速入门教程](http://www.phperz.com/article/14/1115/35197.html)
[
Windows中Phantomjs + Casperjs安装使用方法](http://www.111cn.net/sys/Windows/67816.htm)
[
CasperJS 的安装和快速入门-oschina](http://www.oschina.net/question/12_55791)
[使用 CasperJS 对 Web 网站进行功能测试-oschina](http://www.oschina.net/question/12_55624)
[
利用nodejs+phantomjs+casperjs采集淘宝商品的价格](http://www.cnblogs.com/xinzhyu/p/4214669.html)
[
[译]CasperJS,基于PhantomJS的工具包](http://www.cnblogs.com/ziyunfei/archive/2012/09/27/2706254.html)
**最后希望文章对你有所帮助吧！如果有不足之处，还请海涵~**
**（By:Eastmount 2015-8-19 深夜4点半****[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)****）**

