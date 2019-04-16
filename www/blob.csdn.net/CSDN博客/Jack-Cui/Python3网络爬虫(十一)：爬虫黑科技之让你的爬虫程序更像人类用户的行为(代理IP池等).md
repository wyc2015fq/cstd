# Python3网络爬虫(十一)：爬虫黑科技之让你的爬虫程序更像人类用户的行为(代理IP池等) - Jack-Cui - CSDN博客





2017年05月28日 16:07:30[Jack-Cui](https://me.csdn.net/c406495762)阅读数：36484标签：[Python3爬虫																[反爬虫																[代理IP池](https://so.csdn.net/so/search/s.do?q=代理IP池&t=blog)
个人分类：[Python](https://blog.csdn.net/c406495762/article/category/6144934)

所属专栏：[Python3网络爬虫入门](https://blog.csdn.net/column/details/15321.html)](https://so.csdn.net/so/search/s.do?q=反爬虫&t=blog)




**转载请注明作者和出处：**[http://blog.csdn.net/c406495762](http://blog.csdn.net/c406495762)
**运行平台：** Windows 
**Python版本：** Python3.x 
**IDE：** Sublime text3


- [前言](#1-前言)
- [黑科技](#2-黑科技)- [1 构造合理的HTTP请求头](#21-构造合理的http请求头)
- [2 设置Cookie的学问](#22-设置cookie的学问)
- [3 正常的访问速度](#23-正常的访问速度)
- [4 注意隐含输入字段](#24-注意隐含输入字段)
- [5 爬虫如何避开蜜罐](#25-爬虫如何避开蜜罐)
- [6 创建自己的代理IP池](#26-创建自己的代理ip池)

- [总结](#3-总结)




# 1 前言

    近期，有些朋友问我一些关于如何应对反爬虫的问题。由于好多朋友都在问，因此决定写一篇此类的博客。把我知道的一些方法，分享给大家。博主属于小菜级别，玩爬虫也完全是处于兴趣爱好，如有不足之处，还望指正。

     在互联网上进行自动数据采集（抓取）这件事和互联网存在的时间差不多一样长。今天大众好像更倾向于用“网络数据采集”，有时会把网络数据采集程序称为网络机器人（bots）。最常用的方法是写一个自动化程序向网络服务器请求数据（通常是用 HTML 表单或其他网页文件），然后对数据进行解析，提取需要的信息。

    说句实在话，如果我的网站总是让人爬来爬取的，经常被虚拟访问者骚扰，我也是蛮烦的，而且如果遇到“霸道”一点的爬虫，都能直接把服务器卡死。因此，我们在爬取别人网站的时候，也多为对方考虑考虑。不过话说回来，我却没有这个烦恼，为什么呢？因为我根本就没有自己的网站。=.=



![](https://img-blog.csdn.net/20170528154236990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



# 2 黑科技

    网站防采集的前提就是要正确地区分人类访问用户和网络机器人。现在网站有很多技术来防止爬虫，比如验证码，对于一些简单的数字验证码，可以使用训练好的caffemodel诸如此类的模型去识别，准确率还是可以的。当然，也可以在Github搜一搜关于验证码识别的东西，看一看大牛们是怎么玩的。除了这些高大上的，还有一些十分简单的方法可以让你的网络机器人看起来更像人类访问用户。

## 2.1 构造合理的HTTP请求头

    除了处理网站表单，requests 模块还是一个设置请求头的利器。HTTP 的请求头是在你每次向网络服务器发送请求时，传递的一组属性和配置信息。HTTP 定义了十几种古怪的请求头类型，不过大多数都不常用。

    每个网站都有不同的请求头，如何获取这个请求头呢？可以用我从前提到过的Fiddler或者审查元素的方法，我们可以根据实际情况进行配置。例如，GET百度根目录的时候，需要添加的请求头信息如下：





![](https://img-blog.csdn.net/20170528154342226?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    部分参数说明：
- 
Upgrade-Insecure-Requests：参数为1。该指令用于让浏览器自动升级请求从http到https,用于大量包含http资源的http网页直接升级到https而不会报错。简洁的来讲,就相当于在http和https之间起的一个过渡作用。就是浏览器告诉服务器，自己支持这种操作，我能读懂你服务器发过来的上面这条信息，并且在以后发请求的时候不用http而用https；

- 
User-Agent：有一些网站不喜欢被爬虫程序访问，所以会检测连接对象，如果是爬虫程序，也就是非人点击访问，它就会不让你继续访问，所以为了要让程序可以正常运行，我们需要设置一个浏览器的User-Agent；

- 
Accept：浏览器可接受的MIME类型，可以根据实际情况进行设置；

- 
Accept-Encoding：浏览器能够进行解码的数据编码方式，比如gzip。Servlet能够向支持gzip的浏览器返回经gzip编码的HTML页面。许多情形下这可以减少5到10倍的下载时间；

- 
Accept-Language：浏览器所希望的语言种类，当服务器能够提供一种以上的语言版本时要用到；

- 
Cookie：这是最重要的请求头信息之一。中文名称为“小型文本文件”或“小甜饼“，指某些网站为了辨别用户身份而储存在用户本地终端（Client Side）上的数据（通常经过加密）。定义于RFC2109。是网景公司的前雇员卢·蒙特利在1993年3月的发明。


## 2.2 设置Cookie的学问

    虽然 cookie 是一把双刃剑，但正确地处理 cookie 可以避免许多采集问题。网站会用 cookie 跟踪你的访问过程，如果发现了爬虫异常行为就会中断你的访问，比如特别快速地填写表单，或者浏览大量页面。虽然这些行为可以通过关闭并重新连接或者改变 IP 地址来伪装，但是如果 cookie 暴露了你的身份，再多努力也是白费。

    在采集一些网站时 cookie 是不可或缺的。要在一个网站上持续保持登录状态，需要在多个页面中保存一个 cookie。有些网站不要求在每次登录时都获得一个新 cookie，只要保存一个旧的“已登录”的 cookie 就可以访问。

    如果你在采集一个或者几个目标网站，建议你检查这些网站生成的 cookie，然后想想哪一个 cookie 是爬虫需要处理的。有一些浏览器插件可以为你显示访问网站和离开网站时 cookie 是如何设置的。例如：EditThisCookie，该插件可以谷歌商店进行下载。URL：[http://www.editthiscookie.com/](http://www.editthiscookie.com/)

    Cookie信息，也可以更具实际情况填写。不过requests已经封装好了很多操作，自动管理cookie，session保持连接。我们可以先访问某个目标网站，建立一个session连接之后，获取cookie。代码如下：

```python
# -*- coding:UTF-8 -*-
import requests

if __name__ == '__main__':
    url = 'https://www.baidu.com/'
    headers = {'Upgrade-Insecure-Requests':'1',
    'User-Agent':'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36',
    'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
    'Accept-Encoding':'gzip, deflate, sdch, br',
    'Accept-Language':'zh-CN,zh;q=0.8',
    }
    s = requests.Session()
    req = s.get(url=url,headers=headers)
    print(s.cookies)
```

    运行结果如下：





![](https://img-blog.csdn.net/20170528154552901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    使用 requests.Session 会话对象让你能够跨请求保持某些参数，它也会在同一个 Session 实例发出的所有请求之间保持 cookie， 期间使用 urllib3 的 connection pooling 功能。详细内容参见requests高级用法：[http://docs.python-requests.org/zh_CN/latest/user/advanced.html](http://docs.python-requests.org/zh_CN/latest/user/advanced.html)

    因为 requests 模块不能执行 JavaScript，所以它不能处理很多新式的跟踪软件生成的 cookie，比如 Google Analytics，只有当客户端脚本执行后才设置 cookie（或者在用户浏览页面时基于网页事件产生 cookie，比如点击按钮）。要处理这些动作，需要用 Selenium 和 PhantomJS 包。

    Selenium的安装已经在之前的文章中讲到，今天就说下PhantomJS吧。URL：[http://phantomjs.org/](http://phantomjs.org/)  PhantomJS 是一个“无头”（headless）浏览器。它会把网站加载到内存并执行页面上的 JavaScript，但不会向用户展示网页的图形界面。将 Selenium 和 PhantomJS 结合在一起，就可以运行一个非常强大的网络爬虫了，可以处理 cookie、JavaScript、headers，以及任何你需要做的事情。

    PhantomJS可以依据自己的开发平台选择不同的包进行下载：[http://phantomjs.org/download.html](http://phantomjs.org/download.html) 解压即用，很方便。

    接下来呢，还是以实例出发，对 [http://pythonscraping.com](http://pythonscraping.com) 网站调用 webdriver 的 get_cookie()方法来查看 cookie(D:/phantomjs-2.1.1-windows/bin/phantomjs.exe是我的PhantomJS路径，这里需要更改成你自己的)：

```python
# -*- coding:UTF-8 -*-
from selenium import webdriver

if __name__ == '__main__':
    url = 'http://pythonscraping.com'
    driver = webdriver.PhantomJS(executable_path='D:/phantomjs-2.1.1-windows/bin/phantomjs.exe')
    driver.get(url)
    driver.implicitly_wait(1)
    print(driver.get_cookies())
```

    这样就可以获得一个非常典型的 Google Analytics 的 cookie 列表：





![](https://img-blog.csdn.net/20170528154732653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    还可以调用 delete_cookie()、add_cookie() 和 delete_all_cookies() 方法来处理 cookie。另外，还可以保存 cookie 以备其他网络爬虫使用。

    通过Selenium和PhantomJS，我们可以很好的处理一些需要事件执行后才能获得的cookie。

## 2.3 正常的访问速度

    有一些防护措施完备的网站可能会阻止你快速地提交表单，或者快速地与网站进行交互。即使没有这些安全措施，用一个比普通人快很多的速度从一个网站下载大量信息也可能让自己被网站封杀。

    因此，虽然多进程程序可能是一个快速加载页面的好办法——在一个进程中处理数据，另一个进程中加载页面——但是这对编写好的爬虫来说是恐怖的策略。还是应该尽量保证一次加载页面加载且数据请求最小化。如果条件允许，尽量为每个页面访问增加一点儿时间间隔，即使你要增加两行代码：

```
import time
time.sleep(1)
```

    合理控制速度是你不应该破坏的规则。过度消耗别人的服务器资源会让你置身于非法境地，更严重的是这么做可能会把一个小型网站拖垮甚至下线。拖垮网站是不道德的，是彻头彻尾的错误。所以请控制采集速度！

## 2.4 注意隐含输入字段

     在 HTML 表单中，“隐含”字段可以让字段的值对浏览器可见，但是对用户不可见（除非看网页源代码）。随着越来越多的网站开始用 cookie 存储状态变量来管理用户状态，在找到另一个最佳用途之前，隐含字段主要用于阻止爬虫自动提交表单。

    下图显示的例子就是 Facebook 登录页面上的隐含字段。虽然表单里只有三个可见字段（username、password 和一个确认按钮），但是在源代码里表单会向服务器传送大量的信息。





![](https://img-blog.csdn.net/20170528154851498?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    用隐含字段阻止网络数据采集的方式主要有两种。第一种是表单页面上的一个字段可以用服务器生成的随机变量表示。如果提交时这个值不在表单处理页面上，服务器就有理由认为这个提交不是从原始表单页面上提交的，而是由一个网络机器人直接提交到表单处理页面的。绕开这个问题的最佳方法就是，首先采集表单所在页面上生成的随机变量，然后再提交到表单处理页面。

    第二种方式是“蜜罐”（honey pot）。如果表单里包含一个具有普通名称的隐含字段（设置蜜罐圈套），比如“用户名”（username）或“邮箱地址”（email address），设计不太好的网络机器人往往不管这个字段是不是对用户可见，直接填写这个字段并向服务器提交，这样就会中服务器的蜜罐圈套。服务器会把所有隐含字段的真实值（或者与表单提交页面的默认值不同的值）都忽略，而且填写隐含字段的访问用户也可能被网站封杀。

    总之，有时检查表单所在的页面十分必要，看看有没有遗漏或弄错一些服务器预先设定好的隐含字段（蜜罐圈套）。如果你看到一些隐含字段，通常带有较大的随机字符串变量，那么很可能网络服务器会在表单提交的时候检查它们。另外，还有其他一些检查，用来保证这些当前生成的表单变量只被使用一次或是最近生成的（这样可以避免变量被简单地存储到一个程序中反复使用）。

## 2.5 爬虫如何避开蜜罐

    虽然在进行网络数据采集时用 CSS 属性区分有用信息和无用信息会很容易（比如，通过读取 id和 class 标签获取信息），但这么做有时也会出问题。如果网络表单的一个字段通过 CSS 设置成对用户不可见，那么可以认为普通用户访问网站的时候不能填写这个字段，因为它没有显示在浏览器上。如果这个字段被填写了，就可能是机器人干的，因此这个提交会失效。

    这种手段不仅可以应用在网站的表单上，还可以应用在链接、图片、文件，以及一些可以被机器人读取，但普通用户在浏览器上却看不到的任何内容上面。访问者如果访问了网站上的一个“隐含”内容，就会触发服务器脚本封杀这个用户的 IP 地址，把这个用户踢出网站，或者采取其他措施禁止这个用户接入网站。实际上，许多商业模式就是在干这些事情。

    下面的例子所用的网页在 [http://pythonscraping.com/pages/itsatrap.html](http://pythonscraping.com/pages/itsatrap.html)，这是一个给我们python爬虫学习的一个网站。这个页面包含了两个链接，一个通过 CSS 隐含了，另一个是可见的。另外，页面上还包括两个隐含字段：





![](https://img-blog.csdn.net/20170528154940807?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    这三个元素通过三种不同的方式对用户隐藏：
- 第一个链接是通过简单的 CSS 属性设置 display:none 进行隐藏；
- 电话号码字段 name=”phone” 是一个隐含的输入字段；
- 邮箱地址字段 name=”email” 是将元素向右移动 50 000 像素（应该会超出电脑显示器的边界）并隐藏滚动条。

    因为 Selenium 可以获取访问页面的内容，所以它可以区分页面上的可见元素与隐含元素。通过 is_displayed() 可以判断元素在页面上是否可见。

    例如，下面的代码示例就是获取前面那个页面的内容，然后查找隐含链接和隐含输入字段(同样，需要更改下PhantomJS路径)：

```python
# -*- coding:UTF-8 -*-
from selenium import webdriver

if __name__ == '__main__':
    url = 'http://pythonscraping.com/pages/itsatrap.html'
    driver = webdriver.PhantomJS(executable_path='D:/phantomjs-2.1.1-windows/bin/phantomjs.exe')
    driver.get(url)
    links = driver.find_elements_by_tag_name('a')
    for link in links:
        if not link.is_displayed():
            print('连接:' + link.get_attribute('href') + ',是一个蜜罐圈套.')

    fields = driver.find_elements_by_tag_name('input')
    for field in fields:
        if not field.is_displayed():
            print('不要改变' + field.get_attribute('name') + '的值.')
```

Selenium 抓取出了每个隐含的链接和字段，结果如下所示：





![](https://img-blog.csdn.net/20170528155151762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    虽然你不太可能会去访问你找到的那些隐含链接，但是在提交前，记得确认一下那些已经在表单中、准备提交的隐含字段的值（或者让 Selenium 为你自动提交）。

## 2.6 创建自己的代理IP池

    启用远程平台的人通常有两个目的：对更大计算能力和灵活性的需求，以及对可变 IP 地址的需求。

    有一些网站会设置访问阈值，也就是说，如果一个IP访问速度超过这个阈值，那么网站就会认为，这是一个爬虫程序，而不是用户行为。为了避免远程服务器封锁IP，或者想加快爬取速度，一个可行的方法就是使用代理IP，我们需要做的就是创建一个自己的代理IP池。

     思路：通过免费IP代理网站爬取IP，构建一个容量为100的代理IP池。从代理IP池中随机选取IP，在使用IP之前，检查IP是否可用。如果可用，使用该IP访问目标页面，如果不可用，舍弃该IP。当代理IP池中IP的数量小于20的时候，更新整个代理IP池，即重新从免费IP代理网站爬取IP，构建一个新的容量为100的代理IP池。

    还是使用在之前笔记中提到过的西刺代理，URL：[http://www.xicidaili.com/](http://www.xicidaili.com/)，如果想方便一些，可以使用他们提供的API，直接获取IP。但是这些IP的更新速度有些慢，15分钟更新一次，如果满足需求，使用这个API无妨，如果需求得不到满足呢？呃…需求…不能满足…咳咳！





![](https://img-blog.csdn.net/20170528155228878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    我们可以自己爬取IP。但是，注意一点，千万不要爬太快！很容易被服务器Block哦！

    比如，我想爬取国内高匿代理，第一页的URL为：www.xicidaili.com/nn/1，第二页的URL为：www.xicidaili.com/nn/2，其他页面一次类推，一页IP正好100个，够我们用了。





![](https://img-blog.csdn.net/20170528155259379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    通过审查元素可知，这些ip都存放在了id属性为ip_list的table中。





![](https://img-blog.csdn.net/20170528155324763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    我们可以使用lxml的xpath和Beutifulsoup结合的方法，爬取所有的IP。当然，也可以使用正则表达式，方法很多。代码如下：

```
# -*- coding:UTF-8 -*-
import requests
from bs4 import BeautifulSoup
from lxml import etree

if __name__ == '__main__':
    #requests的Session可以自动保持cookie,不需要自己维护cookie内容
    page = 1
    S = requests.Session()
    target_url = 'http://www.xicidaili.com/nn/%d' % page
    target_headers = {'Upgrade-Insecure-Requests':'1',
        'User-Agent':'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36',
        'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
        'Referer':'http://www.xicidaili.com/nn/',
        'Accept-Encoding':'gzip, deflate, sdch',
        'Accept-Language':'zh-CN,zh;q=0.8',
    }
    target_response = S.get(url = target_url, headers = target_headers)
    target_response.encoding = 'utf-8'
    target_html = target_response.text
    bf1_ip_list = BeautifulSoup(target_html, 'lxml')
    bf2_ip_list = BeautifulSoup(str(bf1_ip_list.find_all(id = 'ip_list')), 'lxml')
    ip_list_info = bf2_ip_list.table.contents

    proxys_list = []
    for index in range(len(ip_list_info)):
        if index % 2 == 1 and index != 1:
            dom = etree.HTML(str(ip_list_info[index]))
            ip = dom.xpath('//td[2]')
            port = dom.xpath('//td[3]')
            protocol = dom.xpath('//td[6]')
            proxys_list.append(protocol[0].text.lower() + '#' + ip[0].text + '#' + port[0].text)
    print(proxys_list)
```

    可以看到，通过这种方法，很容易的就获得了这100个IP，包括他们的协议、IP和端口号。这里我是用”#”符号隔开，使用之前，只需要spilt()方法，就可以提取出信息。





![](https://img-blog.csdn.net/20170528155421889?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    已经获取了IP，如何验证这个IP是否可用呢？一种方案是GET请求一个网页，设置timeout超市时间，如果超时服务器没有反应，说明IP不可用。这里的实现，可以参见Requests的高级用法：[http://docs.python-requests.org/zh_CN/latest/user/advanced.html](http://docs.python-requests.org/zh_CN/latest/user/advanced.html)

    这种设置timeout的验证方法是一种常见的方法，很多人都这样验证。所以博主就想了一个问题，有没有其他的方法呢？经过思考，想出了一个方法，测试了一个，验证一个IP大约需要3秒左右。呃..当然这种方法是我自己琢磨出来的，没有参考，所以，如果有错误之处，或者更好的方法，还望指正！

    在Windows下，可以在CMD中输入如下指令查看IP的连通性(mac和linux可以在中断查看)：





![](https://img-blog.csdn.net/20170528155518483?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    从免费代理网站获得的代理IP很不稳定，过几分钟再测试这个代理IP你可能会发现，这个IP已经不能用了。所以再使用代理IP之前，我们需要测试下代理IP是否可用。





![](https://img-blog.csdn.net/20170528155545562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    从上文可知，通过测试本机和代理IP地址的连通性，我们能够大致知道这个代理 IP的健康情况。如果，本机能够ping通这个代理 IP，那么我们也就可以使用这个代理 IP去访问其他网站。这个过程是在cmd中执行的，那么python有没有提供一个方法，通过程序来实现这样的操作呢？答案是肯定的，有！Subprocess.Popen()可以创建一个进程，当shell参数为true时，程序通过shell来执行：





![](https://img-blog.csdn.net/20170528155627462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




- 
参数args可以是字符串或者序列类型（如：list，元组），用于指定进程的可执行文件及其参数。如果是序列类型，第一个元素通常是可执行文件的路径。我们也可以显式的使用executeable参数来指定可执行文件的路径。

- 
参数stdin, stdout,stderr分别表示程序的标准输入、输出、错误句柄。他们可以是PIPE，文件描述符或文件对象，也可以设置为None，表示从父进程继承。

- 
如果参数shell设为true，程序将通过shell来执行。

- 
subprocess.PIPE：在创建Popen对象时，subprocess.PIPE可以初始化stdin,stdout或stderr参数。表示与子进程通信的标准流。

- 
subprocess.STDOUT：创建Popen对象时，用于初始化stderr参数，表示将错误通过标准输出流输出。

- 
了解到以上这些，我们就可以写我们的程序了(ping本机回环地址)：


```
# -*- coding:UTF-8 -*-
import subprocess as sp

if __name__ == '__main__':
    cmd = "ping -n 3 -w 3 127.0.0.1"
    #执行命令
    p = sp.Popen(cmd, stdin=sp.PIPE, stdout=sp.PIPE, stderr=sp.PIPE, shell=True)
    #获得返回结果并解码
    out = p.stdout.read().decode("gbk")
    print(out)
```

    运行结果如下：





![](https://img-blog.csdn.net/20170528155807703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    能都得到返回结果，跟cmd中类似，接下来，我们就可以制定相应的规则，根据返回信息来剔除不满足要求的ip。

    整体代码如下：

```python
# -*- coding:UTF-8 -*-
from bs4 import BeautifulSoup
import subprocess as sp
from lxml import etree
import requests
import random
import re

"""
函数说明:获取IP代理
Parameters:
    page - 高匿代理页数,默认获取第一页
Returns:
    proxys_list - 代理列表
Modify:
    2017-05-27
"""
def get_proxys(page = 1):
    #requests的Session可以自动保持cookie,不需要自己维护cookie内容
    S = requests.Session()
    #西祠代理高匿IP地址
    target_url = 'http://www.xicidaili.com/nn/%d' % page
    #完善的headers
    target_headers = {'Upgrade-Insecure-Requests':'1',
        'User-Agent':'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36',
        'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
        'Referer':'http://www.xicidaili.com/nn/',
        'Accept-Encoding':'gzip, deflate, sdch',
        'Accept-Language':'zh-CN,zh;q=0.8',
    }
    #get请求
    target_response = S.get(url = target_url, headers = target_headers)
    #utf-8编码
    target_response.encoding = 'utf-8'
    #获取网页信息
    target_html = target_response.text
    #获取id为ip_list的table
    bf1_ip_list = BeautifulSoup(target_html, 'lxml')
    bf2_ip_list = BeautifulSoup(str(bf1_ip_list.find_all(id = 'ip_list')), 'lxml')
    ip_list_info = bf2_ip_list.table.contents
    #存储代理的列表
    proxys_list = []
    #爬取每个代理信息
    for index in range(len(ip_list_info)):
        if index % 2 == 1 and index != 1:
            dom = etree.HTML(str(ip_list_info[index]))
            ip = dom.xpath('//td[2]')
            port = dom.xpath('//td[3]')
            protocol = dom.xpath('//td[6]')
            proxys_list.append(protocol[0].text.lower() + '#' + ip[0].text + '#' + port[0].text)
    #返回代理列表
    return proxys_list

"""
函数说明:检查代理IP的连通性
Parameters:
    ip - 代理的ip地址
    lose_time - 匹配丢包数
    waste_time - 匹配平均时间
Returns:
    average_time - 代理ip平均耗时
Modify:
    2017-05-27
"""
def check_ip(ip, lose_time, waste_time):
    #命令 -n 要发送的回显请求数 -w 等待每次回复的超时时间(毫秒)
    cmd = "ping -n 3 -w 3 %s"
    #执行命令
    p = sp.Popen(cmd % ip, stdin=sp.PIPE, stdout=sp.PIPE, stderr=sp.PIPE, shell=True)
    #获得返回结果并解码
    out = p.stdout.read().decode("gbk")
    #丢包数
    lose_time = lose_time.findall(out)
    #当匹配到丢失包信息失败,默认为三次请求全部丢包,丢包数lose赋值为3
    if len(lose_time) == 0:
        lose = 3
    else:
        lose = int(lose_time[0])
    #如果丢包数目大于2个,则认为连接超时,返回平均耗时1000ms
    if lose > 2:
        #返回False
        return 1000
    #如果丢包数目小于等于2个,获取平均耗时的时间
    else:
        #平均时间
        average = waste_time.findall(out)
        #当匹配耗时时间信息失败,默认三次请求严重超时,返回平均好使1000ms
        if len(average) == 0:
            return 1000
        else:
            #
            average_time = int(average[0])
            #返回平均耗时
            return average_time

"""
函数说明:初始化正则表达式
Parameters:
    无
Returns:
    lose_time - 匹配丢包数
    waste_time - 匹配平均时间
Modify:
    2017-05-27
"""
def initpattern():
    #匹配丢包数
    lose_time = re.compile(u"丢失 = (\d+)", re.IGNORECASE)
    #匹配平均时间
    waste_time = re.compile(u"平均 = (\d+)ms", re.IGNORECASE)
    return lose_time, waste_time

if __name__ == '__main__':
    #初始化正则表达式
    lose_time, waste_time = initpattern()
    #获取IP代理
    proxys_list = get_proxys(1)

    #如果平均时间超过200ms重新选取ip
    while True:
        #从100个IP中随机选取一个IP作为代理进行访问
        proxy = random.choice(proxys_list)
        split_proxy = proxy.split('#')
        #获取IP
        ip = split_proxy[1]
        #检查ip
        average_time = check_ip(ip, lose_time, waste_time)
        if average_time > 200:
            #去掉不能使用的IP
            proxys_list.remove(proxy)
            print("ip连接超时, 重新获取中!")
        if average_time < 200:
            break

    #去掉已经使用的IP
    proxys_list.remove(proxy)
    proxy_dict = {split_proxy[0]:split_proxy[1] + ':' + split_proxy[2]}
    print("使用代理:", proxy_dict)
```

    从上面代码可以看出，我制定的规则是，如果丢包数大于2个，则认为ip不能用。ping通的平均时间大于200ms也抛弃。当然，我这个要求有点严格，可以视情况放宽规则：





![](https://img-blog.csdn.net/20170528155913685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    从打印结果中可以看出，第一个随机选取的IP被抛弃了，第二个随机选取的IP能用。

    我只是实现了，构建代理IP池和检查IP是否可用，如果你感兴趣也可以将获取的IP放入到数据库中，不过我没这样做，因为感觉免费获取的代理IP，失效很快，随用随取就行。当然，也可以自己写代码试试reqeusts的GET请求，通过设置timeout参数来验证代理IP是否可用，因为方法简单，所以在此不再累述。

    除此之外，我们也可以个创建一个User-Agent的列表，多罗列点。也是跟代理IP一样，每次访问随机选取一个。这样在一定程度上，也能避免被服务器封杀。

# 3 总结

    如果你一直被网站封杀却找不到原因，那么这里有个检查列表，可以帮你诊断一下问题出在哪里。
- **首先，检查 JavaScript 。**如果你从网络服务器收到的页面是空白的，缺少信息，或其遇到他不符合你预期的情况（或者不是你在浏览器上看到的内容），有可能是因为网站创建页面的 JavaScript 执行有问题。
- **检查正常浏览器提交的参数。**如果你准备向网站提交表单或发出 POST 请求，记得检查一下页面的内容，看看你想提交的每个字段是不是都已经填好，而且格式也正确。用 Chrome 浏览器的网络面板（快捷键 F12 打开开发者控制台，然后点击“Network”即可看到）查看发送到网站的 POST 命令，确认你的每个参数都是正确的。
- **是否有合法的 Cookie？**如果你已经登录网站却不能保持登录状态，或者网站上出现了其他的“登录状态”异常，请检查你的 cookie。确认在加载每个页面时 cookie 都被正确调用，而且你的 cookie 在每次发起请求时都发送到了网站上。
- **IP 被封禁？**如果你在客户端遇到了 HTTP 错误，尤其是 403 禁止访问错误，这可能说明网站已经把你的 IP 当作机器人了，不再接受你的任何请求。你要么等待你的 IP 地址从网站黑名单里移除，要么就换个 IP 地址。如果你确定自己并没有被封杀，那么再检查下面的内容： 
- 确认你的爬虫在网站上的速度不是特别快。快速采集是一种恶习，会对网管的服务器造成沉重的负担，还会让你陷入违法境地，也是 IP 被网站列入黑名单的首要原因。给你的爬虫增加延迟，让它们在夜深人静的时候运行。切记：匆匆忙忙写程序或收集数据都是拙劣项目管理的表现；应该提前做好计划，避免临阵慌乱。
- 还有一件必须做的事情：修改你的请求头！有些网站会封杀任何声称自己是爬虫的访问者。如果你不确定请求头的值怎样才算合适，就用你自己浏览器的请求头吧。
- 确认你没有点击或访问任何人类用户通常不能点击或接入的信息。
- 如果你用了一大堆复杂的手段才接入网站，考虑联系一下网管吧，告诉他们你的目的。试试发邮件到 webmaster@< 域名 > 或 admin@< 域名 >，请求网管允许你使用爬虫采集数据。管理员也是人嘛！


    使用免费的代理IP也是有局限的，就是不稳定。更好的方法是，花钱买一个可以**动态切换IP的阿里云服务器**，这样IP就可以无限动态变化了！

    以上内容整理自《Python网络数据采集》，以及自己的一点小心得。重要的事情再说一遍：我们在爬取别人网站的时候，也为对方考虑考虑！

    代码获取：Python3爬虫的程序，可以在我的Github上查看。URL：[https://github.com/Jack-Cherish/python-spider](https://github.com/Jack-Cherish/python-spider)

**PS： 如果觉得本篇本章对您有所帮助，欢迎关注、评论、顶！**](https://so.csdn.net/so/search/s.do?q=Python3爬虫&t=blog)




