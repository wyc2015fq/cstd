# sjtuLib爬虫(一) - lyx的专栏 - CSDN博客





2015年11月20日 21:40:17[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：612








前段时间想看一本《天才在左，疯子在右》的书，到图书馆网站一看，发现被预约了十次之多，只能说当时我就醉了。

后来一想，其实可以利用这一预约的数量来判断一本书的好坏与质量。这个社会，我们不缺书，而是缺少好书，往往会浪费时间在读一些不是那么好的书上。利用交大童鞋的集体智慧，挖掘一些好书，这是一个好法子！

之前对爬虫也只是了解，所以先弄一个简单些的吧。

我们现在python里面加入下面几个模块：



```python
from bs4 import BeautifulSoup
import urllib
import urllib2
import pandas as pd
import numpy
```



从上往下的功能依次是:

解析标记语言

url应用模块

同上

数据帧（DataFrame）的模块，用于处理数据

同上，用于处理数据

其实后面另个模块不是必须的，只是为了练习一下python而已，而且，pandas和numpy的数据处理框架确实很好。

下面看几个函数吧~~



```python
def urlData(url):
    request = urllib2.Request(url)
    context = urllib2.urlopen(request)
    context = urllib2.urlopen(request)
    context = context.read().decode('utf-8')
    return context
```


这段代码就是普通的，输入一个url，返回数据。这就是你在浏览器中输入一个网址，浏览器返回页面给你的过程，只不过，我们返回的是html文件而已。既然是html文件，所以我们要import bs4.这个模块里面有有一个美丽的泡泡，BeautifulSoup，还是非常好用的呢~



具体如何用呢，我们要结合网页，首先打开交大的图书馆主页，然后进入思源探索（吐槽一下，交大的图书馆也就主页好看了一些，不过国内高校似乎都这样）




选着好按照书号检索之后，上方的url（网址）http://ourex.lib.sjtu.edu.cn/primo_library/libweb/action/search.do?dscnt=0&dstmp=1448027587875&searchField=callnumber&fn=BrowseRedirect&vid=chinese&searchTxt=

这就是我们爬虫的初始页面。下面代码中，有一个prefix，这是应为，在html的代码中，下一页连接并不包含全部，所以我们在用beautifuSoup获取了下一页的连接之后，还要把前缀加上。至于BeautifuSoup的使用方法，网上有很多，童鞋们先去看一下吧。告诉大家一个体会，每次获取一个与soup有关的东西之后，就是type（）一下，看看他是什么，这样你就知道这个对象有哪些方法啦。

下面这段代码中还用到了pandas来存储数据，使得数据好处理一些。pandas的DataFrame的教程百度经验都有哦~~

这个函数传入的是html代码，对其做处理之后，获取页面的书籍的书号、作者、和书籍详细情况的连接，并且获取指向下一页的url地址。最后，前面一部分一DataFrame的格式返回，后面的url连接，以字符串的形式返回。

简而言之，这个函数就是把我们人类不能理解的html（程序员不是人类）文件，提取出我们需要的信息。


```python
def page2frame(context):
    prefix = 'http://ourex.lib.sjtu.edu.cn/primo_library/libweb/action/'
    soup = BeautifulSoup(context)
    soup = soup.body
    page = soup.find_all(class_='EXLResultsListBrowseLine')#get the valide record
    page.pop(0)#pop the first line,is useless
    
    dataEmpty = {'ID': [],'BookNamwe': [],'Writer':[],'Link':[]}
    frame = pd.DataFrame(dataEmpty)
    for cnt in range(0, len(page)):
        data = {'ID': [page[cnt].contents[6].string],\
                'BookNamwe': [page[cnt].contents[8].a.string],\
                'Writer':[page[cnt].contents[10].string],\
                'Link':[page[cnt].contents[8].a['href']]}
        temp = pd.DataFrame(data)
        frame = frame.append(temp)
    
    nLink = soup.find_all(class_='EXLNext EXLBriefResultsPaginationLinkNext')
    
    if nLink == []:    
        nextLink = []
    else:
        nLink = nLink[0];
        nextLink = prefix + nLink['href']
    return frame,nextLink
```
最后就是把上面两个函数有机结合起来。由于藏书实在是太多了，根本挖不完，所以，把爬下来的数据及时的存储到硬盘文件中。我运行了几十分钟就产生了几十兆的文件。拿来玩玩就好，整个图书目录爬下来，不是这么简单的架构可以实现的咯~~

```python
def spiderForLib():
    f=file("E:/SpiderContent/content.txt","a+")
    #new_context="goodbye"
    
    context = urlData('http://ourex.lib.sjtu.edu.cn/primo_library/libweb/action/search.do?dscnt=0&dstmp=1447766424889&searchField=callnumber&fn=BrowseRedirect&vid=chinese&searchTxt=')
    frame,nextUrl = page2frame(context)
    pageNumber = 1
    print pageNumber
    while nextUrl != [] :
        context = urlData(nextUrl)
        frameTemp,nextUrl = page2frame(context)
        frame.append(frameTemp)
        new_context = str(pageNumber)+'\n'+frameTemp
        f.write(str(new_context))
        pageNumber += 1
        print pageNumber
        
    f.close()
    return frame
```
后期在这个基础上继续实现开头说的目标，最后让爬虫帮我找好书看~~技术改变生活,，但是，好书改变思维方式，改变命运。








