# [Python]网络爬虫( 连载：大牛汪海 ) - 在思索中前行！ - CSDN博客





2015年04月15日 15:03:09[_Tham](https://me.csdn.net/txl16211)阅读数：1028标签：[Python 爬虫入门																[爬虫入门																[抓取网页内容																[爬虫抓取网页内容																[unity3D 游戏引擎](https://so.csdn.net/so/search/s.do?q=unity3D 游戏引擎&t=blog)
个人分类：[开发篇																[计算机网络](https://blog.csdn.net/txl16211/article/category/2492201)](https://blog.csdn.net/txl16211/article/category/2492171)





汪海个人博客：http://blog.callmewhy.com/

Python爬虫专栏，[汪海专栏](http://blog.csdn.net/column/details/why-bug.html)


- [Python爬虫入门教程](http://blog.csdn.net/column/details/why-bug.html)
- 简单的介绍如何使用Python的相关模块如urllib2来实现网络爬虫的基础教程。
- - [汪海带你做游戏--Unity3D的开发与应用](http://blog.csdn.net/column/details/i-am-why.html)
- 简单的介绍Unity3D开发过程中会遇到的常见问题。主要涉及内容有：场景建模，人物设置，脚本讲解，关联演示，重力感应，等等内容。





## [（一）：抓取网页的含义和URL基本构成](http://blog.csdn.net/pleasecallmewhy/article/details/8922826)

一、网络爬虫的定义

网络爬虫，即Web Spider，是一个很形象的名字。

把互联网比喻成一个蜘蛛网，那么Spider就是在网上爬来爬去的蜘蛛。

网络蜘蛛是通过网页的链接地址来寻找网页的。

从网站某一个页面（通常是首页）开始，读取网页的内容，找到在网页中的其它链接地址，

然后通过这些链接地址寻找下一个网页，这样一直循环下去，直到把这个网站所有的网页都抓取完为止。

如果把整个互联网当成一个网站，那么网络蜘蛛就可以用这个原理把互联网上所有的网页都抓取下来。

这样看来，网络爬虫就是一个爬行程序，一个抓取网页的程序。

网络爬虫的基本操作是抓取网页。

那么如何才能随心所欲地获得自己想要的页面？

我们先从URL开始。




二、浏览网页的过程

抓取网页的过程其实和读者平时使用IE浏览器浏览网页的道理是一样的。

比如说你在浏览器的地址栏中输入    www.baidu.com    这个地址。


打开网页的过程其实就是浏览器作为一个浏览的“客户端”，向服务器端发送了 一次请求，把服务器端的文件“抓”到本地，再进行解释、展现。

HTML是一种标记语言，用标签标记内容并加以解析和区分。

浏览器的功能是将获取到的HTML代码进行解析，然后将原始的代码转变成我们直接看到的网站页面。




三、URI和URL的概念和举例



简单的来讲，URL就是在浏览器端输入的    http://www.baidu.com    这个字符串。

在理解URL之前，首先要理解URI的概念。

什么是URI？

Web上每种可用的资源，如 HTML文档、图像、视频片段、程序等都由一个通用资源标志符(Universal Resource Identifier， URI)进行定位。 

URI通常由三部分组成：

①访问资源的命名机制；

②存放资源的主机名；

③资源自身 的名称，由路径表示。

如下面的URI：

http://www.why.com.cn/myhtml/html1223/


我们可以这样解释它：

①这是一个可以通过HTTP协议访问的资源，

②位于主机 www.webmonkey.com.cn上，

③通过路径“/html/html40”访问。 




四、URL的理解和举例

URL是URI的一个子集。它是Uniform Resource Locator的缩写，译为“统一资源定位 符”。

通俗地说，URL是Internet上描述信息资源的字符串，主要用在各种WWW客户程序和服务器程序上。

采用URL可以用一种统一的格式来描述各种信息资源，包括文件、服务器的地址和目录等。



URL的一般格式为(带方括号[]的为可选项)：

*protocol :// hostname[:port] / path / [;parameters][?query]#fragment*



URL的格式由三部分组成： 

①第一部分是协议(或称为服务方式)。

②第二部分是存有该资源的主机IP地址(有时也包括端口号)。

③第三部分是主机资源的具体地址，如目录和文件名等。

第一部分和第二部分用“://”符号隔开，

第二部分和第三部分用“/”符号隔开。

第一部分和第二部分是不可缺少的，第三部分有时可以省略。 




五、URL和URI简单比较



URI属于URL更低层次的抽象，一种字符串文本标准。

换句话说，URI属于父类，而URL属于URI的子类。URL是URI的一个子集。

URI的定义是：统一资源标识符；

URL的定义是：统一资源定位符。


二者的区别在于，URI表示请求服务器的路径，定义这么一个资源。

而URL同时说明要如何访问这个资源（http://）。





下面来看看两个URL的小例子。




1.HTTP协议的URL示例：

使用超级文本传输协议HTTP，提供超级文本信息服务的资源。 

例：http://www.peopledaily.com.cn/channel/welcome.htm 

其计算机域名为www.peopledaily.com.cn。

超级文本文件(文件类型为.html)是在目录 /channel下的welcome.htm。

这是中国人民日报的一台计算机。 

例：http://www.rol.cn.net/talk/talk1.htm 

其计算机域名为www.rol.cn.net。

超级文本文件(文件类型为.html)是在目录/talk下的talk1.htm。

这是瑞得聊天室的地址，可由此进入瑞得聊天室的第1室。



2．文件的URL

用URL表示文件时，服务器方式用file表示，后面要有主机IP地址、文件的存取路 径(即目录)和文件名等信息。

有时可以省略目录和文件名，但“/”符号不能省略。 

例：file://ftp.yoyodyne.com/pub/files/foobar.txt 

上面这个URL代表存放在主机ftp.yoyodyne.com上的pub/files/目录下的一个文件，文件名是foobar.txt。

例：file://ftp.yoyodyne.com/pub 

代表主机ftp.yoyodyne.com上的目录/pub。 

例：file://ftp.yoyodyne.com/ 

代表主机ftp.yoyodyne.com的根目录。 




爬虫最主要的处理对象就是URL，它根据URL地址取得所需要的文件内容，然后对它 进行进一步的处理。

因此，准确地理解URL对理解网络爬虫至关重要。



[（二）：利用urllib2通过指定的URL抓取网页内容](http://blog.csdn.net/pleasecallmewhy/article/details/8923067)


版本号：Python2.7.5，Python3改动较大，各位另寻教程。


所谓网页抓取，就是把URL地址中指定的网络资源从网络流中读取出来，保存到本地。 

类似于使用程序模拟IE浏览器的功能，把URL作为HTTP请求的内容发送到服务器端， 然后读取服务器端的响应资源。



在Python中，我们使用urllib2这个组件来抓取网页。

urllib2是Python的一个获取URLs(Uniform Resource Locators)的组件。

它以urlopen函数的形式提供了一个非常简单的接口。


最简单的urllib2的应用代码只需要四行。

我们新建一个文件urllib2_test01.py来感受一下urllib2的作用：





**[python]**[view
 plain](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)[copy](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- import urllib2  
- response = urllib2.urlopen('http://www.baidu.com/')  
- html = response.read()  
- print html  


按下F5可以看到运行的结果：


![](https://img-blog.csdn.net/20130513230356450)


我们可以打开百度主页，右击，选择查看源代码（火狐OR谷歌浏览器均可），会发现也是完全一样的内容。

也就是说，上面这四行代码将我们访问百度时浏览器收到的代码们全部打印了出来。

这就是一个最简单的urllib2的例子。




除了"http:"，URL同样可以使用"ftp:"，"file:"等等来替代。

HTTP是基于请求和应答机制的：

客户端提出请求，服务端提供应答。




urllib2用一个Request对象来映射你提出的HTTP请求。

在它最简单的使用形式中你将用你要请求的地址创建一个Request对象，

通过调用urlopen并传入Request对象，将返回一个相关请求response对象，

这个应答对象如同一个文件对象，所以你可以在Response中调用.read()。


我们新建一个文件urllib2_test02.py来感受一下：






**[python]**[view
 plain](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)[copy](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- import urllib2    
- req = urllib2.Request('http://www.baidu.com')    
- response = urllib2.urlopen(req)    
- the_page = response.read()    
- print the_page
  

可以看到输出的内容和test01是一样的。


urllib2使用相同的接口处理所有的URL头。例如你可以像下面那样创建一个ftp请求。




**[python]**[view
 plain](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)[copy](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- req = urllib2.Request('ftp://example.com/')  

在HTTP请求时，允许你做额外的两件事。


**1.发送data表单数据**

这个内容相信做过Web端的都不会陌生，

有时候你希望发送一些数据到URL(通常URL与CGI[通用网关接口]脚本，或其他WEB应用程序挂接)。

在HTTP中,这个经常使用熟知的POST请求发送。

这个通常在你提交一个HTML表单时由你的浏览器来做。

并不是所有的POSTs都来源于表单，你能够使用POST提交任意的数据到你自己的程序。

一般的HTML表单，data需要编码成标准形式。然后做为data参数传到Request对象。

编码工作使用urllib的函数而非urllib2。


我们新建一个文件urllib2_test03.py来感受一下：





**[python]**[view
 plain](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)[copy](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- import urllib    
- import urllib2    
- 
- url = 'http://www.someserver.com/register.cgi'
- 
- values = {'name' : 'WHY',    
- 'location' : 'SDU',    
- 'language' : 'Python' }    
- 
- data = urllib.urlencode(values) # 编码工作
- req = urllib2.Request(url, data)  # 发送请求同时传data表单
- response = urllib2.urlopen(req)  #接受反馈的信息
- the_page = response.read()  #读取反馈的内容

如果没有传送data参数，urllib2使用GET方式的请求。
GET和POST请求的不同之处是POST请求通常有"副作用"，

它们会由于某种途径改变系统状态(例如提交成堆垃圾到你的门口)。

Data同样可以通过在Get请求的URL本身上面编码来传送。





**[python]**[view
 plain](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)[copy](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- import urllib2    
- import urllib  
- 
- data = {}  
- 
- data['name'] = 'WHY'
- data['location'] = 'SDU'
- data['language'] = 'Python'
- 
- url_values = urllib.urlencode(data)    
- print url_values  
- 
- name=Somebody+Here&language=Python&location=Northampton    
- url = 'http://www.example.com/example.cgi'
- full_url = url + '?' + url_values  
- 
- data = urllib2.open(full_url)    

这样就实现了Data数据的Get传送。





**2.设置Headers到http请求**

有一些站点不喜欢被程序（非人为访问）访问，或者发送不同版本的内容到不同的浏览器。

默认的urllib2把自己作为“Python-urllib/x.y”(x和y是Python主版本和次版本号,例如Python-urllib/2.7)，


这个身份可能会让站点迷惑，或者干脆不工作。

浏览器确认自己身份是通过User-Agent头，当你创建了一个请求对象，你可以给他一个包含头数据的字典。

下面的例子发送跟上面一样的内容，但把自身模拟成Internet Explorer。

（多谢大家的提醒，现在这个Demo已经不可用了，不过原理还是那样的）。






**[python]**[view
 plain](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)[copy](http://blog.csdn.net/pleasecallmewhy/article/details/8923067#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- import urllib    
- import urllib2    
- 
- url = 'http://www.someserver.com/cgi-bin/register.cgi'
- 
- user_agent = 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'
- values = {'name' : 'WHY',    
- 'location' : 'SDU',    
- 'language' : 'Python' }    
- 
- headers = { 'User-Agent' : user_agent }    
- data = urllib.urlencode(values)    
- req = urllib2.Request(url, data, headers)    
- response = urllib2.urlopen(req)    
- the_page = response.read()](https://so.csdn.net/so/search/s.do?q=爬虫抓取网页内容&t=blog)](https://so.csdn.net/so/search/s.do?q=抓取网页内容&t=blog)](https://so.csdn.net/so/search/s.do?q=爬虫入门&t=blog)](https://so.csdn.net/so/search/s.do?q=Python 爬虫入门&t=blog)




