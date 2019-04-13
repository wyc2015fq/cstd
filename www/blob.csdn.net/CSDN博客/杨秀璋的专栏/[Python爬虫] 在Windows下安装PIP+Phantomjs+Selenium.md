
# [Python爬虫] 在Windows下安装PIP+Phantomjs+Selenium - 杨秀璋的专栏 - CSDN博客

2015年08月19日 20:04:34[Eastmount](https://me.csdn.net/Eastmount)阅读数：28676所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)



最近准备深入学习Python相关的爬虫知识了，如果说在使用Python爬取相对正规的网页使用"**urllib2 + BeautifulSoup + 正则表达式**"就能搞定的话；那么动态生成的信息页面，如Ajax、JavaScript等就需要通过"**Phantomjs
 +****CasperJS + Selenium**"来实现了。所以先从安装和功能介绍入门，后面在介绍一些Python相关的爬虫应用。
## 一. 介绍
**PhantomJS**
PhantomJS是一个服务器端的 JavaScript API 的[WebKit](http://baike.baidu.com/link?url=2KTqOPM_VIV6Etls4s1esrsIGg6ntkgi5tP46b2gqxR1V319nkYrwPpUBZWMxKYQvRHYUEuBrFxXNRKk7vWGmq)（开源的浏览器引擎）。其支持各种Web标准：
 DOM 处理, CSS 选择器, JSON, Canvas 和 SVG。PhantomJS可以用于页面自动化，网络监测，网页截屏，以及无界面测试等。
**Selenium**
Selenium是一个用于Web应用程序测试的工具。Selenium测试直接运行在浏览器中，就像真正的用户在操作一样。支持的浏览器包括IE(7、8、9)、Mozilla Firefox、Mozilla Suite等。这个工具的主要功能包括：测试与浏览器的兼容性、测试系统功能，它ThoughtWorks专门为Web应用程序编写的一个验收测试工具。
**PIP**
在介绍介绍它们之前，需要安装PIP软件。正如[xifeijian大神](http://blog.csdn.net/xifeijian/article/details/12576455)所说：“作为Python爱好者，如果不知道easy_install或者pip中的任何一个的话，那么......”。
easy_insall的作用和perl中的cpan，ruby中的gem类似，都提供了在线一键安装模块的傻瓜方便方式，而pip是easy_install的改进版，提供更好的提示信息，删除package等功能。老版本的python中只有easy_install，没有pip。常见的具体用法如下：

```python
easy_install的用法：  
1） 安装一个包  
 $ easy_install <package_name>  
 $ easy_install "<package_name>==<version>"  
2) 升级一个包  
 $ easy_install -U "<package_name>>=<version>"  
  
pip的用法  
1) 安装一个包  
 $ pip install <package_name>  
 $ pip install <package_name>==<version>  
2) 升级一个包 (如果不提供version号，升级到最新版本）  
 $ pip install --upgrade <package_name>>=<version>  
3）删除一个包  
 $ pip uninstall <package_name>
```

## 二. 安装PIP
PS：你可以使用easy_install pip 直接安装pip软件，这更加方便。
**第一步：下载PIP软件**
可以在官网[http://pypi.python.org/pypi/pip\#downloads](http://pypi.python.org/pypi/pip#downloads)下载，同时cd切换到PIP目录，在通过python
 setup.py install安装。而我采用的是下载pip-Win_1.7.exe进行安装，下载地址如下：
[https://sites.google.com/site/pydatalog/python/pip-for-windows](https://sites.google.com/site/pydatalog/python/pip-for-windows)
**第二步：安装PIP软件**
![](https://img-blog.csdn.net/20150819191610482)
![](https://img-blog.csdn.net/20150819191652551)
当提示"pip and virtualenv installed"表示安装成功，那怎么测试PIP安装成功呢？
**第三步：配置环境变量**
此时在cmd中输入pip指令会提示错误“不是内部或外部命令”。
![](https://img-blog.csdn.net/20150819192258231)
所以需要添加path环境变量。PIP安装完成后，会在Python安装目录下添加python\Scripts目录，即在python安装目录的Scripts目录下，将此目录加入环境变量中即可！过程如下：
![](https://img-blog.csdn.net/20150819191909357)
![](https://img-blog.csdn.net/20150819191929044)
**第四步：使用PIP命令**
下面在CMD中使用PIP命令，“pip list outdate”列举Python安装库的版本信息。
![](https://img-blog.csdn.net/20150819192428460)
**PIP常用的命令如下所示:**(参考[pip安装使用详解](http://www.ttlsa.com/python/how-to-install-and-use-pip-ttlsa/))
```python
Usage:     
  pip <command> [options]  
   
Commands:  
  install                     安装软件.  
  uninstall                   卸载软件.  
  freeze                      按着一定格式输出已安装软件列表  
  list                        列出已安装软件.  
  show                        显示软件详细信息.  
  search                      搜索软件，类似yum里的search.  
  wheel                       Build wheels from your requirements.  
  zip                         不推荐. Zip individual packages.  
  unzip                       不推荐. Unzip individual packages.  
  bundle                      不推荐. Create pybundles.  
  help                        当前帮助.  
   
General Options:  
  -h, --help                  显示帮助.  
  -v, --verbose               更多的输出，最多可以使用3次  
  -V, --version               现实版本信息然后退出.  
  -q, --quiet                 最少的输出.  
  --log-file <path>           覆盖的方式记录verbose错误日志，默认文件：/root/.pip/pip.log  
  --log <path>                不覆盖记录verbose输出的日志.  
  --proxy <proxy>             Specify a proxy in the form [user:passwd@]proxy.server:port.  
  --timeout <sec>             连接超时时间 (默认15秒).  
  --exists-action <action>    默认活动当一个路径总是存在: (s)witch, (i)gnore, (w)ipe, (b)ackup.  
  --cert <path>               证书.
```




## 三. 安装Phantomjs+Selenium
通过pip命令安装Selenium：
![](https://img-blog.csdn.net/20150819192734965)
![](https://img-blog.csdn.net/20150819192758993)
官网[http://phantomjs.org/](http://phantomjs.org/)下载PhantomJS解压后如下图所示：
![](https://img-blog.csdn.net/20150819192938843)
调用时可能会报错“**Unable to start phantomjs with ghostdriver**”如图：![](https://img-blog.csdn.net/20150819193434045)
此时可以设置下Phantomjs的路径，同时如果你配置了Scripts目录环境变量，可以解压Phantomjs到该文件夹下。
参考：[Selenium
 with GhostDriver in Python on Windows - stackoverflow](http://stackoverflow.com/questions/21768554/selenium-with-ghostdriver-in-python-on-windows)

## 四. 测试代码

设置executable_path路径后的代码如下：
```python
from selenium import webdriver
driver = webdriver.PhantomJS(executable_path="F:\Python\phantomjs-1.9.1-windows\phantomjs.exe")
driver.get("http://www.baidu.com")
data = driver.title
print data
```
运行结果如下图所示：
![](https://img-blog.csdn.net/20150819194504828)
获取"百度一下，你就知道"，对应HTML源码：
<title>百度一下，你就知道</title>
但是总会弹出PhantomJS的黑框，怎么办呢？同时如何通过Python直接调用Phantomjs运行JS呢？
同时下面的代码可以进行截图：
```python
from selenium import webdriver  
driver=webdriver.PhantomJS(executable_path="F:\Python\phantomjs-1.9.1-windows\phantomjs.exe")  
driver.get("http://www.csdn.net")  
data = driver.title
driver.save_screenshot('csdn.png')
print data
```
输出如下图所示，图片太长仅仅部分：
```python
>>> 
CSDN.NET - 全球最大中文IT社区，为IT专业技术人员提供最全面的信息传播和服务平台
>>>
```
![](https://img-blog.csdn.net/20150819213042538)
PS：我准备使用C\#调用PhantomJS.exe完成页面截图功能，但是没有成功，而且使用WebBrowser中DrawToBitmap函数获取图片，由于ActiveX 控件不支持DrawToBitmap方法，获取总是空白的，各种问题。
![](https://img-blog.csdn.net/20150819195154949)




**参考资料：**
[数据抓取的艺术（一）：Selenium+Phantomjs数据抓取环境配置(强推)](http://blog.chinaunix.net/uid-22414998-id-3692113.html)
[数据抓取的艺术（二）：数据抓取程序优化](http://blog.chinaunix.net/uid-22414998-id-3695673.html)
[Python使用Selenium/PhantomJS(强推)](http://www.haiyun.me/archives/989.html)
[Python selenium的js扩展实现](http://www.cnblogs.com/shizioo/p/3813929.html)
[使用python+phantomjs抓取动态页面](http://my.oschina.net/ushuaia/blog/361091)
[用phantomjs 进行网页整页截屏](http://www.cnblogs.com/yesicoo/archive/2012/05/25/2518729.html)
[Selenium-webdriver系列教程(15)——万能的截图(强推)](http://www.cnblogs.com/timsheng/archive/2012/09/05/2672651.html)
[pyspider 爬虫教程（三）：使用 PhantomJS 渲染带 JS 的页面](http://segmentfault.com/a/1190000002477913)
[【PHP】【.NET】【JS】【AJAX】关于抓取网页源代码的问题](http://segmentfault.com/q/1010000000581910#a-1020000000582320)
[
使用python/casperjs编写终极爬虫-客户端App的抓取](http://blog.csdn.net/xiaoyao3857/article/details/9180989)
[Python 爬虫如何获取 JS 生成的 URL 和网页内容-知乎](http://www.zhihu.com/question/21471960)
[通过 WebBrowser 获取网页截图 - C\#](http://www.cnblogs.com/snowdream/archive/2011/05/16/get-webpage-snapshot-by-webbrowser-control.html)
[Control.DrawToBitmap 方法不支持Ajax - 官网](https://msdn.microsoft.com/zh-cn/library/system.windows.forms.control.drawtobitmap(v=VS.90).aspx)
[IE浏览器整页截屏程序（二） - C\#](http://www.cnblogs.com/lavezhang/archive/2012/05/26/2519646.html)
[C\# 网络编程之最简单浏览器实现 - 自己](http://blog.csdn.net/eastmount/article/details/9490009)
**最后希望该篇基础性文章对你有所帮助吧！如果有不足之处，还请海涵~**
**（By:Eastmount 2015-8-19 晚上8点****[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)****）**

