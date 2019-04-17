# Python爬虫学习笔记总结(一) - zhusongziye的博客 - CSDN博客





2018年11月01日 21:00:07[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：125








### 〇. python 基础

先放上python 3 的官方文档:https://docs.python.org/3/ (看文档是个好习惯)

关于python 3 基础语法方面的东西,网上有很多,大家可以自行查找.

### 一. 最简单的爬取程序

爬取百度首页源代码:

![](https://img-blog.csdnimg.cn/20181101205804719.png)



来看上面的代码:
- 
对于python 3来说,urllib是一个非常重要的一个模块 ，可以非常方便的模拟浏览器访问互联网,对于python 3 爬虫来说, urllib更是一个必不可少的模块,它可以帮助我们方便地处理URL.

- 
urllib.request是urllib的一个子模块,可以打开和处理一些复杂的网址


> 
The urllib.request

module defines functions and classes which help in opening URLs (mostly HTTP) in a complex world — basic and digest authentication, redirections, cookies and more.

- 
urllib.request.urlopen()方法实现了打开url,并返回一个 http.client.HTTPResponse对象,通过http.client.HTTPResponse的read()方法,获得response body,转码最后通过print()打印出来.


> 
urllib.request.urlopen(*url*, *data=None*, [*timeout*, ]***, *cafile=None*,*capath=None*, *cadefault=False*, *context=None*)

For HTTP and HTTPS URLs, this function returns a http.client.HTTPResponse

object slightly modified.

< 出自: https://docs.python.org/3/library/urllib.request.html >

- 
decode('utf-8')用来将页面转换成utf-8的编码格式，否则会出现乱码


### 二  模拟浏览器爬取信息

在访问某些网站的时候，网站通常会用判断访问是否带有头文件来鉴别该访问是否为爬虫，用来作为反爬取的一种策略。

先来看一下Chrome的头信息（F12打开开发者模式）如下：

![](https://img-blog.csdnimg.cn/20181101205816387.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



如图，访问头信息中显示了浏览器以及系统的信息（headers所含信息众多，具体可自行查询）

Python中urllib中的request模块提供了模拟浏览器访问的功能，代码如下：



```
from urllib import request

url = 'http://www.baidu.com'
# page = request.Request(url)
# page.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36')
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36'}
page = request.Request(url, headers=headers)
page_info = request.urlopen(page).read().decode('utf-8')
print(page_info)
```

可以通过add_header(key, value) 或者直接以参数的形式和URL一起请求访问，urllib.request.Request()

> 
urllib.request.Request(url, data=None, headers={}, origin_req_host=None, unverifiable=False, method=None)




### 三 爬虫利器Beautiful Soup

> 
Beautiful Soup是一个可以从HTML或XML文件中提取数据的Python库.它能够通过你喜欢的转换器实现惯用的文档导航,查找,修改文档的方式.


文档中的例子其实说的已经比较清楚了，那下面就以爬取简书首页文章的标题一段代码来演示一下：

先来看简书首页的源代码：

![](https://img-blog.csdnimg.cn/20181101205831445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



可以发现简书首页文章的标题都是在<a/>标签中，并且class='title',所以，通过

```
find_all('a', 'title')
```

便可获得所有的文章标题，具体实现代码及结果如下：

```
# -*- coding:utf-8 -*-

from urllib import request
from bs4 import BeautifulSoup

url = r'http://www.jianshu.com'
# 模拟真实浏览器进行访问
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36'}
page = request.Request(url, headers=headers)
page_info = request.urlopen(page).read()
page_info = page_info.decode('utf-8')

# 将获取到的内容转换成BeautifulSoup格式，并将html.parser作为解析器
soup = BeautifulSoup(page_info, 'html.parser')  

# 以格式化的形式打印html
# print(soup.prettify())
titles = soup.find_all('a', 'title')  # 查找所有a标签中class='title'的语句
# 打印查找到的每一个a标签的string
for title in titles:
    print(title.string)
```

Beautiful Soup支持Python标准库中的HTML解析器,还支持一些第三方的解析器,下表列出了主要的解析器,以及它们的优缺点:

![](https://img-blog.csdnimg.cn/20181101205843587.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



### **四  将爬取的信息存储到本地**

之前我们都是将爬取的数据直接打印到了控制台上，这样显然不利于我们对数据的分析利用，也不利于保存，所以现在就来看一下如何将爬取的数据存储到本地硬盘。

1 对.txt文件的操作

读写文件是最常见的操作之一，python3 内置了读写文件的函数：open

> 
open(*file*, *mode='r'*, *buffering=-1*, *encoding=None*, *errors=None*,*newline=None*, *closefd=True*, *opener=None*))

Open *file* and return a corresponding  file object. If the file cannot be opened, an OSError

is raised.


其中比较常用的参数为*file*和*mode*，参数*file*为文件的路径，参数*mode*为操作文件的方式（读/写），函数的返回值为一个file对象，如果文件操作出现异常的话，则会抛出 一个OSError

还以简书首页文章题目为例，将爬取到的文章标题存放到一个.txt文件中，具体**代码如下：**

```
# -*- coding:utf-8 -*-

from urllib import request
from bs4 import BeautifulSoup

url = r'http://www.jianshu.com'
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36'}
page = request.Request(url, headers=headers)
page_info = request.urlopen(page).read().decode('utf-8')
soup = BeautifulSoup(page_info, 'html.parser')
titles = soup.find_all('a', 'title')

try:
    # 在E盘以只写的方式打开/创建一个名为 titles 的txt文件
    file = open(r'E:\titles.txt', 'w')
    for title in titles:
    # 将爬去到的文章题目写入txt中
        file.write(title.string + '\n')
finally:
    if file:
        # 关闭文件（很重要）
        file.close()
```

*open*中mode参数的含义见下表：

![](https://img-blog.csdnimg.cn/20181101205911708.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



其中't'为默认模式，'r'相当于'rt',符号可以叠加使用，像'r+b'

另外，对文件操作一定要注意的一点是：*打开的文件一定要关闭*，否则会占用相当大的系统资源，所以对文件的操作最好使用try:...finally:...的形式。但是try:...finally:...的形式会使代码显得比较杂乱，所幸python中的*with*语句可以帮我们自动调用close（）而不需要我们写出来，所以，上面代码中的try:...finally:...可使用下面的*with*语句来代替：



```
with open(r'E:\title.txt', 'w') as file:
    for title in titles:
        file.write(title.string + '\n')
```



效果是一样的，建议使用with语句



![](https://img-blog.csdnimg.cn/201811012059252.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**2 图片的储存**

有时候我们的爬虫不一定只是爬取文本数据，也会爬取一些图片，下面就来看怎么将爬取的图片存到本地磁盘。

我们先来选好目标，知乎话题：女生怎么健身锻造好身材？ (单纯因为图多，不要多想哦 （# _ # ) ）

看下页面的源代码，找到话题下图片链接的格式，如图：

![](https://img-blog.csdnimg.cn/20181101205933620.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



可以看到，图片在img标签中，且class=origin_image zh-lightbox-thumb，而且链接是由.jpg结尾，我们便可以用Beautiful Soup结合正则表达式的方式来提取所有链接，如下:

```
links = soup.find_all('img', "origin_image zh-lightbox-thumb",src=re.compile(r'.jpg$'))
```

提取出所有链接后，使用request.urlretrieve来将所有链接保存到本地

> 
Copy a network object denoted by a URL to a local file. If the URL points to a local file, the object will not be copied unless filename is supplied. Return a tuple (filename, headers)

where *filename* is the local file name under which the object can be found, and *headers* is whatever the info()

method of the object returned by urlopen()

returned (for a remote object). Exceptions are the same as for urlopen()

.


具体实现代码如下：

```
# -*- coding:utf-8 -*-
import time
from urllib import request
from bs4 import BeautifulSoup
import re

url = r'https://www.zhihu.com/question/22918070'
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36'}
page = request.Request(url, headers=headers)
page_info = request.urlopen(page).read().decode('utf-8')
soup = BeautifulSoup(page_info, 'html.parser')

# Beautiful Soup和正则表达式结合，提取出所有图片的链接（img标签中，class=**，以.jpg结尾的链接）
links = soup.find_all('img', "origin_image zh-lightbox-thumb",src=re.compile(r'.jpg$'))
# 设置保存的路径，否则会保存到程序当前路径
local_path = r'E:\Pic'

for link in links:
    print(link.attrs['src'])
    # 保存链接并命名，time防止命名冲突
    request.urlretrieve(link.attrs['src'], local_path+r'\%s.jpg' % time.time())
```

运行结果

![](https://img-blog.csdnimg.cn/20181101205946710.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



注意营养哦~~~

```
'''
作者：Veniendeavor
链接：https://www.jianshu.com/p/2cc8310a51c4
'''
```



