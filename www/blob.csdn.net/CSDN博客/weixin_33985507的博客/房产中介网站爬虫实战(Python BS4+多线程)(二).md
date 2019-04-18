# 房产中介网站爬虫实战(Python BS4+多线程)(二) - weixin_33985507的博客 - CSDN博客
2016年12月31日 11:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
> 
#### 本文的两大贡献：
- 给出了爬取经纬度数据的方法。
- 给出了搜房网（房天下）爬取的可实现解决方案。爬该网站的困难有二：其网页是压缩过的以及网站只给出100页的内容。
> 
本系列文章介绍了爬取链家和搜房网（房天下）数据的方法，此篇介绍房天下的部分。
[房产中介网站爬虫实战(Python BS4+多线程)(一)](https://www.jianshu.com/p/4fc40b59317f/)
[房产中介网站爬虫实战(Python BS4+多线程)(二)](https://www.jianshu.com/p/639a67bc75b9)
# 2 房天下
### 2.1 第一步：爬取思路
房天下的网页一样是有规律的：[http://zu.sh.fang.com/house/g22-h316-i31-n31](https://link.jianshu.com?t=http://zu.sh.fang.com/house/g22-h316-i31-n31)，
其中i31中的1就是指页码。
房天下的页码示意
不同于链家的是，在房天下即使把页码改到10000，例如[http://zu.sh.fang.com/house/g22-h316-i310000-n31](https://link.jianshu.com?t=http://zu.sh.fang.com/house/g22-h316-i310000-n31)，
页面上依然有房源，而且显示的内容虽然100页（末页）一致，但本人做过一个实验。先爬取1100页的链接，再爬取101200页的链接，再爬取201300页的链接，发现100页之后的链接重复十分严重，且它们均在1100页中。
但是在该网站的HTML源码中，号称自己有10w+的房源：
`<meta name="description" content="找到符合条件的上海租房信息：【112860套、真实房源】。上海租房子、找合租、个人房屋出租，就到搜房网租房。" />`
因此留下了一个问题便是：**如何爬取房天下所有的房源链接**。
### 2.2 第二步：获取房源链接
由上述所言，采取枚举页数的做法行不通。但是我发现在查看房源的时候总是有各种各样的过滤条件，因此我萌生了通过遍历过滤条件来枚举所有房源的方法。
遍历过滤条件
由于我大前提的设定是整租两房，所以首页直接过滤掉该条件，而其他的过滤条件的遍历如下表所示。
```
链接的递进：  
http://zu.sh.fang.com/house/g22-n31/                                  #首页
http://zu.sh.fang.com/house-a018/g22-n31/                             #选了行政区
http://zu.sh.fang.com/house-a018-b01601/g22-n31/                      #选了行政区下的二级区划
http://zu.sh.fang.com/house-a018-b01601/c21000-d21500-g22-n31/        #选了价格区间
http://zu.sh.fang.com/house-a018-b01603/c24000-d25000-g22-i32-n31/    #选了页码
```
注意：有些时候，某些条件下会出现无房源的情况，此时要把这种条件所代表的链接排除。
去掉无房源的链接
**我们最终的目的把每个单个房源的链接放入一个列表中。**
具体爬取链接的代码如下：
```python
#!/usr/bin/env python3
from bs4 import BeautifulSoup
import urllib
import gzip
import inspect
import re
import pandas as pd
from multiprocessing.dummy import Pool as ThreadPool
import datetime
import sqlite3
from time import sleep
from random import choice
#用代理IP方式访问，见2.21部分
conn=sqlite3.connect('/IPProxyPool/IPProxyPool_py2/data/proxy.db')
IP = pd.read_sql("select* from proxys", conn)
ip = [str(i) for i in IP['ip']]
port = [str(i) for i in IP['port']]
proxy = [ip[i] + ":" +port[i] for i in range(len(ip))]
proxy_support = urllib.request.ProxyHandler({'http':choice(proxy)})
opener = urllib.request.build_opener(proxy_support)
urllib.request.install_opener(opener)
#网页解压缩，见2.22部分
def read_zip_url(url):
    fails = 0
    while fails < 5:
        try:
            content = urllib.request.urlopen(url).read()
            content = gzip.decompress(content).decode("gb18030") #网页gb2312的编码要用这个
            break
        except:
            fails += 1
        print(inspect.stack()[1][3] + ' occused error')
    soup = BeautifulSoup(content, "lxml")
    return soup
starturl = "http://zu.sh.fang.com/house/g22-n31/"
soup = read_zip_url(starturl)
area_first_soup = soup.find_all('dl',id = 'rentid_D04_01')[0].find_all('a')
del area_first_soup[-2]
del area_first_soup[0]
area_first = [] #注1
for i in area_first_soup:
    area_first.append("http://zu.sh.fang.com" + i.get('href'))
area_second = [] #注2
for i in area_first:
    soup = read_zip_url(i)
    area_second_soup = soup.find_all('div',id = 'rentid_D04_08')[0].find_all('a')
    del area_second_soup[0]
    for i in area_second_soup:
        area_second.append("http://zu.sh.fang.com" + i.get('href'))
        
area_third = [] #注3
def area_third_func(li):
    soup = read_zip_url(li)
    area_third_soup = soup.find_all('dl',id = 'rentid_D04_02')[0].find_all('a')
    del area_third_soup[0]
    for i in area_third_soup:
        area_third.append("http://zu.sh.fang.com" + i.get('href')) 
pool = ThreadPool(4)
pool.map(area_third_func, area_second)
pool.close()
pool.join()
area_fourth = [] #注4
def area_fourth_func(li):
    soup = read_zip_url(li)
    if soup.find(text=re.compile("很抱歉")) == None:
        pagenum1 = soup.find_all('span', class_ = 'txt')[0].get_text()
        pagenum = int(re.findall(r'\d+',pagenum1)[0])
        splitted = li.split('-')
        for j in range(1, int(pagenum)+1):
            new_url = (splitted[0]+'{0}' + splitted[1] + '{0}' + splitted[2] + '{0}' + splitted[3] + \
            '{0}' + splitted[4]+'{0}' + 'i3{1}'+'{0}' + splitted[5]).format('-',j)
            area_fourth.append(new_url)
            
pool = ThreadPool(4)
pool.map(area_fourth_func, area_third)
pool.close()
pool.join()
finalinks = [] #注5
def get_links(li):
    soup = read_zip_url(li)
    urlist = soup.select('a[href^="/chuzu/"]')
    for i in urlist:
        href = 'http://zu.sh.fang.com' + i.get('href')
        if href not in finalinks:
            finalinks.append(href)
    sleep(0.1)
            
pool = ThreadPool(4)
pool.map(get_links, area_fourth)
pool.close()
pool.join()
today = datetime.date.today().strftime("%Y%m%d")
finalinks = pd.DataFrame(finalinks)
finalinks = finalinks.drop_duplicates()
finalinks.to_csv("%s" %'sf_links'+today + '.csv')
```
*注1：爬取各个一级行政区划条件过滤后的链接。*
*注2：爬取各个二级行政区划条件过滤后的链接，即包含A行政区下有a、b、c、d，B行政区下有e、f、g，则该列表是a、b、c、d、e、f、g。*
*注3：爬取加入价格条件过滤后的全部链接，思路同上。*
*注4：`if soup.find(text=re.compile("很抱歉")) == None`一句正是起过滤出有房源的网页的作用。当一级、二级行政区划，价格条件都过滤后，仍然会有好几页的情况，此时就要通过获取页面中显示的“共n页”来生成第二页、第n页的链接了。*
*注5：获取页面上的所有房源链接。*
房天下上给上海的行政区划是18个，因而对于`area_first`，有18条信息；对于二级区划`area_second`,一共有180条；价格的一共有8个选择，所以`area_third`一共有1440条；`area_fourth`一共4932条，`finalinks`一共10w+条。
#### 2.21 通过代理IP请求网页
房天下可能存在封IP反爬虫机制，因为我有几次自己就上不去网站了，换网络环境却可以。因而有了以下的额外功能，即通过代理请求它的网页。
```python
from random import choice
conn=sqlite3.connect('/IPProxyPool/IPProxyPool_py2/data/proxy.db') #注1
IP = pd.read_sql("select* from proxys", conn)
ip = [str(i) for i in IP['ip']]
port = [str(i) for i in IP['port']]
proxy = [ip[i] + ":" +port[i] for i in range(len(ip))] #注2
proxy_support = urllib.request.ProxyHandler({'http':choice(proxy)}) #随机从proxy的列表中选一个
opener = urllib.request.build_opener(proxy_support) #通过这个代理IP生成对网页的请求
urllib.request.install_opener(opener)
```
*注1：代理IP数据库的来源见文章《》*
*注2：数据库中是把IP地址和端口分开存的，所以要把它们合并起来，生成新字段proxy*
#### 2.22 爬取房天下网页的困难及解决办法
网上查到不少人问房天下的网页怎么爬取，他们以及我实际操作中遇到的困难有三：
（1）网页源码上`charset=gb2312`，但是用`gb2312`解码的时候却出错。经过我多方查找，对于标称`gb2312`的网页均使用`gb18030`来解码，具体原因就不表了。
（2）在调用*BeautifulSoup*进行解析网页时都会有这么一句`soup = BeautifulSoup(content, "lxml")`,但是对于房天下，输出的`soup`只是网页的一部分，经过多方查找，问题出在所使用的解析器`lxml`上，改用`html_parser`就可以输出全部网页。
（3）然而这样做，所有的中文却是乱码，无论用哪种编码均无法转换。最后发现是因为网页是**被压缩过的结果**，后来我发现从网页源码中也可看出网页被压缩，因为在其中有一句``。下面将讲在*Python 3*环境下如何解压缩网页。
#### 2.23 网页解压缩的办法
方法一：
```python
import gzip
content = urllib.request.urlopen(url).read()
content = gzip.decompress(content).decode("gb18030") #网页gb2312的编码要用这个
soup = BeautifulSoup(content, "lxml")
```
这种方法十分简单。。。对房天下也适用，如果对其他网站不适用，还有第二种方法。
```python
from io import BytesIO
def ungzip(content):  
    content = BytesIO(content)
    gzipper = gzip.GzipFile(fileobj=content)
    html = gzipper.read()
    return html
    
content = urllib.request.urlopen(url).read()
content = ungzip(content)
soup = BeautifulSoup(content, "lxml", from_encoding="gb18030")
```
这个在房天下身上也通过。
### 2.3 第三步：获取信息
```python
#!/usr/bin/env python3
from bs4 import BeautifulSoup
import urllib
import gzip
import inspect
import re
import pandas as pd
from multiprocessing.dummy import Pool as ThreadPool
import sqlite3
from sqlalchemy import create_engine
import glob
import os
from time import sleep
csvlist = glob.glob(os.path.join("", 'sf_links*.csv'))
temp = {}
for i in csvlist:
    temp[i] = os.path.getmtime(i)
filename = sorted(temp.items(),key=lambda item:item[1],reverse = True)[0][0]
engine = create_engine('sqlite:///%s' %'SHRENT.db', echo = False)
fullset = list(pd.read_csv(filename)['0'])
conn=sqlite3.connect('/IPProxyPool/IPProxyPool_py2/data/proxy.db')
IP = pd.read_sql("select* from proxys", conn)
ip = [str(i) for i in IP['ip']]
port = [str(i) for i in IP['port']]
proxy = [ip[i] + ":" +port[i] for i in range(len(ip))]
proxy_support = urllib.request.ProxyHandler({'http':choice(proxy)})
opener = urllib.request.build_opener(proxy_support)
urllib.request.install_opener(opener)
def read_zip_url(url):
    fails = 0
    while fails < 5:
        try:
            content = urllib.request.urlopen(url).read()
            content = gzip.decompress(content).decode("gb18030")
            break
        except:
            fails += 1
        print(inspect.stack()[1][3] + ' occused error')
    soup = BeautifulSoup(content, "lxml")
    return soup
errorlist = []
def save(urlset):
    title = []
    price = []
    room = []
    area = []
    floor1 = []
    floor2 = []
    direct = []
    district1 = []
    district2 = []
    onsaledate = []
    xiaoqu = []
    address = []
    number = []
    seven_daikan = []
    total_daikan = []
    longitude = []
    latitude = []
    URL = []
    
    try:
        soup = read_zip_url(urlset)
        info = soup.find_all('div', class_ = 'floatr house-info-wrap')
        title.append(soup.h1.get_text())
        price.append(int(info[0].find_all('li')[0].strong.get_text()))
        temp1 = info[0].find_all('li')[1].get_text()
        room.append(re.findall(r'\d\D\d\D', temp1)[0])
        area1 = info[0].find_all('li')[1].find_all('span')[3].get_text()
        area.append(re.findall(r'\d+', area1)[0])
        floor1.append(re.findall(r'\D层', temp1)[0])
        floor2.append("")
        direct.append(re.findall(r'南北|南|北', temp1)[0])
        district1.append(info[0].find_all('li')[2].find_all('a')[1].get_text())
        district2.append("")
        xiaoqu.append(info[0].find_all('li')[2].find_all('a')[0].get_text())
        address.append(info[0].find_all('li')[3].get_text().split("：")[1])
        num_temp = soup.find_all('span', class_ = 'mr10')[0].get_text()
        number.append(re.findall(r'\d+', num_temp)[0])
        geoinfo = soup.find_all("iframe")[0].get('src')
        long_temp = re.findall(r'Baidu_coord_x=\d+.\d+', geoinfo)[0]
        longitude.append(re.findall(r'\d+.\d{6}', long_temp)[0])
        lati_temp = re.findall(r'Baidu_coord_y=\d+.\d+', geoinfo)[0]
        latitude.append(re.findall(r'\d+.\d{6}', lati_temp)[0])
        URL.append(urlset)
        onsaledate.append("")
    except:
        errorlist.append(urlset)
    
    df_dic = {'title':title, 'price':price, 'room':room, 'area':area, 'floor1':floor1, 'floor2':floor2, \
               'direct':direct, 'district1':district1, 'district2':district2, 'onsaledate':onsaledate, \
               'xiaoqu': xiaoqu, 'address': address, 'number':number, 'longitude':longitude, 'latitude':latitude, 'URL':URL, \
               'source':"房天下"}
    try:
        dataset = pd.DataFrame(df_dic, index = number)
        dataset = dataset.drop(['number'], axis = 1)
    except:
        dataset = pd.DataFrame()
    dataset.to_sql('basic_information', engine, if_exists = 'append')
    sleep(0.5)
    
pool = ThreadPool(4)
pool.map(save, fullset)
pool.close()
pool.join()
```
