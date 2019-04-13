
# [python爬虫] BeautifulSoup设置Cookie解决网站拦截并爬取蚂蚁短租 - 杨秀璋的专栏 - CSDN博客

2018年03月07日 18:59:57[Eastmount](https://me.csdn.net/Eastmount)阅读数：2444所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)



我们在编写Python爬虫时，有时会遇到网站拒绝访问等反爬手段，比如这么我们想爬取蚂蚁短租数据，它则会提示“当前访问疑似黑客攻击，已被网站管理员设置为拦截”提示，如下图所示。此时我们需要采用设置Cookie来进行爬取，下面我们进行详细介绍。非常感谢我的学生承峰提供的思想，后浪推前浪啊！


## 一. 网站分析与爬虫拦截
当我们打开蚂蚁短租搜索贵阳市，反馈如下图所示结果。
网址为：http://www.mayi.com/guiyang/1/?map=n
![](https://img-blog.csdn.net/20180307180406611)

我们可以看到短租房信息呈现一定规律分布，如下图所示，这也是我们要爬取的信息。

![](https://img-blog.csdn.net/20180307181021741)

通过浏览器审查元素，我们可以看到需要爬取每条租房信息都位于<dd></dd>节点下。

![](https://img-blog.csdn.net/20180307181243630)

在定位房屋名称，如下图所示，位于<div class="room-detail clearfloat"></div>节点下。
![](https://img-blog.csdn.net/20180307181557391)
接下来我们写个简单的BeautifulSoup进行爬取。
```python
# -*- coding: utf-8 -*-
import urllib
import re 
from bs4 import BeautifulSoup
import codecs
url = 'http://www.mayi.com/guiyang/?map=no'
response=urllib.urlopen(url)
contents = response.read()
soup = BeautifulSoup(contents, "html.parser")
print soup.title
print soup
#短租房名称
for tag in soup.find_all('dd'):
    for name in tag.find_all(attrs={"class":"room-detail clearfloat"}):
        fname = name.find('p').get_text()
        print u'[短租房名称]', fname.replace('\n','').strip()
```
但很遗憾，报错了，说明蚂蚁金服防范措施还是挺到位的。

![](https://img-blog.csdn.net/20180307180353945)




## 二. 设置Cookie的BeautifulSoup爬虫

添加消息头的代码如下所示，这里先给出代码和结果，再教大家如何获取Cookie。

```python
# -*- coding: utf-8 -*-
import urllib2
import re 
from bs4 import BeautifulSoup

#爬虫函数
def gydzf(url):
    user_agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36"
    headers={"User-Agent":user_agent}
    request=urllib2.Request(url,headers=headers)
    response=urllib2.urlopen(request)
    contents = response.read() 
    soup = BeautifulSoup(contents, "html.parser")
    for tag in soup.find_all('dd'):
        #短租房名称
        for name in tag.find_all(attrs={"class":"room-detail clearfloat"}):
            fname = name.find('p').get_text()
            print u'[短租房名称]', fname.replace('\n','').strip()
        #短租房价格
        for price in tag.find_all(attrs={"class":"moy-b"}):
            string = price.find('p').get_text()
            fprice = re.sub("[￥]+".decode("utf8"), "".decode("utf8"),string)
            fprice = fprice[0:5]
            print u'[短租房价格]', fprice.replace('\n','').strip()
            #评分及评论人数
            for score in name.find('ul'):
                fscore = name.find('ul').get_text()
            print u'[短租房评分/评论/居住人数]', fscore.replace('\n','').strip()           
            #网页链接url           
            url_dzf = tag.find(attrs={"target":"_blank"})
            urls = url_dzf.attrs['href']
            print u'[网页链接]', urls.replace('\n','').strip()
            urlss = 'http://www.mayi.com' + urls + ''
            print urlss
#主函数
if __name__ == '__main__':
    i = 1
    while i<10:
        print u'页码', i
        url = 'http://www.mayi.com/guiyang/' + str(i) + '/?map=no'
        gydzf(url)
        i = i+1
    else:
        print u"结束"
```
输出结果如下图所示：
```python
页码 1
[短租房名称] 大唐东原财富广场--城市简约复式民宿
[短租房价格] 298
[短租房评分/评论/居住人数] 5.0分·5条评论·二居·可住3人
[网页链接] /room/851634765
http://www.mayi.com/room/851634765
[短租房名称] 大唐东原财富广场--清新柠檬复式民宿
[短租房价格] 568
[短租房评分/评论/居住人数] 2条评论·三居·可住6人
[网页链接] /room/851634467
http://www.mayi.com/room/851634467
...
页码 9
[短租房名称] 【高铁北站公园旁】美式风情+超大舒适安逸
[短租房价格] 366
[短租房评分/评论/居住人数] 3条评论·二居·可住5人
[网页链接] /room/851018852
http://www.mayi.com/room/851018852
[短租房名称] 大营坡（中大国际购物中心附近）北欧小清新三室
[短租房价格] 298
[短租房评分/评论/居住人数] 三居·可住6人
[网页链接] /room/851647045
http://www.mayi.com/room/851647045
```
![](https://img-blog.csdn.net/20180307183511998)

接下来我们想获取详细信息，比如：http://www.mayi.com/room/851634467
我们需要获取租房地址及详细信息等。

![](https://img-blog.csdn.net/20180307183817670)
这里作者主要是提供分析Cookie的方法，使用浏览器打开网页，右键“检查”，然后再刷新网页。在“NetWork”中找到网页并点击，在弹出来的Headers中就隐藏这这些信息。

![](https://img-blog.csdn.net/20180307184137941)

最常见的两个参数是Cookie和User-Agent，如下图所示：

![](https://img-blog.csdn.net/20180307184414108)
![](https://img-blog.csdn.net/20180307184428107)

然后在Python代码中设置这些参数，再调用Urllib2.Request()提交请求即可，核心代码如下：
**user_agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) ... Chrome/61.0.3163.100 Safari/537.36"**
**cookie="mediav=%7B%22eid%22%3A%22387123...b3574ef2-21b9-11e8-b39c-1bc4029c43b8"**
**headers={"User-Agent":user_agent,"Cookie":cookie}**
**request=urllib2.Request(url,headers=headers)**
**response=urllib2.urlopen(request)**
**contents = response.read()**
**soup = BeautifulSoup(contents, "html.parser")**
**for tag1 in soup.find_all(attrs={"class":"main"}):**
**....**
注意，每小时Cookie会更新一次，我们需要手动修改Cookie值即可，就是上面代码的cookie变量和user_agent变量。完整代码如下所示：
```python
# -*- coding: utf-8 -*-
import urllib2 
import re 
from bs4 import BeautifulSoup
import codecs
import csv

c = open("ycf.csv","wb") #write 写
c.write(codecs.BOM_UTF8)
writer = csv.writer(c)
writer.writerow(["短租房名称","地址","价格","评分","可住人数","人均价格"])
 
#爬取详细信息
def getInfo(url,fname,fprice,fscore,users):
    #通过浏览器开发者模式查看访问使用的user_agent及cookie设置访问头（headers）避免反爬虫,且每隔一段时间运行要根据开发者中的cookie更改代码中的cookie
    user_agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36"
    cookie="mediav=%7B%22eid%22%3A%22387123%22eb7; mayi_uuid=1582009990674274976491; sid=42200298656434922.85.130.130"
    headers={"User-Agent":user_agent,"Cookie":cookie}
    request=urllib2.Request(url,headers=headers)
    response=urllib2.urlopen(request)
    contents = response.read() 
    soup = BeautifulSoup(contents, "html.parser")
    #短租房地址
    for tag1 in soup.find_all(attrs={"class":"main"}):    
        print u'短租房地址:'
        for tag2 in tag1.find_all(attrs={"class":"desWord"}):
            address = tag2.find('p').get_text()
            print address
    #可住人数     
        print u'可住人数:'
        for tag4 in tag1.find_all(attrs={"class":"w258"}):
            yy = tag4.find('span').get_text()
            print yy
        fname = fname.encode("utf-8")
        address = address.encode("utf-8")
        fprice = fprice.encode("utf-8")
        fscore = fscore.encode("utf-8")
        fpeople = yy[2:3].encode("utf-8")
        ones = int(float(fprice))/int(float(fpeople))
        #存储至本地
        writer.writerow([fname,address,fprice,fscore,fpeople,ones])
    
#爬虫函数
def gydzf(url):
    user_agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36"
    headers={"User-Agent":user_agent}
    request=urllib2.Request(url,headers=headers)
    response=urllib2.urlopen(request)
    contents = response.read() 
    soup = BeautifulSoup(contents, "html.parser")
    for tag in soup.find_all('dd'):
    #短租房名称
        for name in tag.find_all(attrs={"class":"room-detail clearfloat"}):
            fname = name.find('p').get_text()
            print u'[短租房名称]', fname.replace('\n','').strip()
    #短租房价格
        for price in tag.find_all(attrs={"class":"moy-b"}):
            string = price.find('p').get_text()
            fprice = re.sub("[￥]+".decode("utf8"), "".decode("utf8"),string)
            fprice = fprice[0:5]
            print u'[短租房价格]', fprice.replace('\n','').strip()
    #评分及评论人数
            for score in name.find('ul'):
                fscore = name.find('ul').get_text()
            print u'[短租房评分/评论/居住人数]', fscore.replace('\n','').strip()           
   #网页链接url           
            url_dzf = tag.find(attrs={"target":"_blank"})
            urls = url_dzf.attrs['href']
            print u'[网页链接]', urls.replace('\n','').strip()
            urlss = 'http://www.mayi.com' + urls + ''
            print urlss
            getInfo(urlss,fname,fprice,fscore,user_agent)
      
#主函数
if __name__ == '__main__':    
    i = 0
    while i<33:
        print u'页码', (i+1)
        if(i==0):
            url = 'http://www.mayi.com/guiyang/?map=no'
        if(i>0):
            num = i+2 #除了第一页是空的，第二页开始按2顺序递增
            url = 'http://www.mayi.com/guiyang/' + str(num) + '/?map=no'
        gydzf(url)
        i=i+1
c.close()
```
输出结果如下，存储本地CSV文件：

![](https://img-blog.csdn.net/20180307185903601)

同时，大家可以尝试Selenium爬取蚂蚁短租，应该也是可行的方法。最后希望文章对您有所帮助，如果存在不足之处，请海涵~
(By:Eastmount 2017-03-07 晚上7点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



