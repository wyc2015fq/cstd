# Python中使用PhantomJS抓取Javascript网页数据 - 小平子的专栏 - CSDN博客





2015年07月01日 17:08:22[阿拉丁吃米粉](https://me.csdn.net/jinping_shi)阅读数：8208标签：[Selenium																[phantomJS																[python																[javascript](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)




![版權聲明](https://img-blog.csdn.net/20160117172627181)

有些网页不是静态加载的，而是通过javascirpt函数动态加载网页，比如下面这个网页，表格中的看涨合约和看跌合约的数据都是通过javascirpt函数从后台加载。仅仅使用beautifulsoup并不能抓到这个[表格](http://stock.finance.sina.com.cn/option/quotes.html)中的数据。 
![这里写图片描述](https://img-blog.csdn.net/20150701163951611)
查找资料，发现可以使用PhantomJS来抓取这类网页的数据。但PhantomJS主要用于Java，若要在python中使用，则要通过Selenium在python中调用PhantomJS。写代码时主要参考了这个网页：[Is there a way to use PhantomJS in Python?](http://stackoverflow.com/questions/13287490/is-there-a-way-to-use-phantomjs-in-python)

Selenium是一个浏览器虚拟器，可以通过Selenium在各种浏览器上模拟各种行为。python中通过Selenium使用PhantomJS抓取动态网页数据时需要安装以下库： 

1.  Beautifulsoup，用于解析网页内容 

2. Node.js 

3. 安装好Node.js之后通过Node.js安装PhantomJS。在Mac终端中输入`npm -g install phantomjs`即可（Windows下的cmd也是一样） 

4. 安装Selenium 

完成上述四个步骤后即可在python中使用PhantomJS。
代码如下：

```python
# -*- coding: utf-8 -*-
from bs4 import BeautifulSoup
from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
import urllib2
import time

baseUrl = "http://stock.finance.sina.com.cn/option/quotes.html"
csvPath = "FinanceData.csv"
csvFile = open(csvPath, 'w')

def is_chinese(uchar):
        # 判断一个unicode是否是汉字
    if uchar >= u'\u4e00' and uchar<=u'\u9fa5':
        return True
    else:
        return False

def readPage(url):
    webURL = urllib2.urlopen(baseUrl)
    content = webURL.read()
    soup = BeautifulSoup(content)
    return soup

def getFinance(soup, tableName):
    divs = soup.findAll('div', attrs={'class': tableName}) #看涨合约在这个div中
    if len(divs) < 0 or len(divs) == 0:
        print "No div class named " + str(tableName)
        return
    tbs = divs[0].findChildren('tbody') # 獲取tbody內容，在這個標籤下只有一個tbody
    print tbs[0]
    trs = tbs[0].findChildren('tr') # tr就是table中的每一行
    for tr in trs: 
        tds = tr.findChildren('td') # td是表格中的內容
        content = list()
        string = ""
        print tr

        index = 0 # 判斷漢字出現的位置
        for td in tds:
            temp = td.text
            print temp
            if index == 7 or index == 0:
                temp2 = ""
                for d in temp:
                    if not is_chinese(d): # 去除漢字
                        temp2 += d
                temp = temp2

            string = string + temp
            string = string + ","
            index += 1
        print string
        csvFile.write(string)
        csvFile.write('\n')

tableName = "table_down fr" # 表格名稱

driver = webdriver.PhantomJS(executable_path='/Users/Pan/node_modules/phantomjs/lib/phantom/bin/phantomjs')
driver.get(baseUrl)
data = driver.page_source # 获取整个页面的内容
driver.quit

#soup = readPage(loadUrl)
soup = BeautifulSoup(data)
getFinance(soup, tableName)
print "Finished!"
csvFile.close()
```

但是以上代码有问题！以上代码有时可以抓取到数据，有时抓取不到。原因时上述代码执行时有些数据还没有被加载。因此需要判断网页何时加载了想要的数据。为了解决这个问题，Selenium提供了[Waits机制](http://docs.seleniumhq.org/docs/04_webdriver_advanced.jsp#explicit-and-implicit-waits)，可以等待一段时间再读取网页。waits机制分为Explicit Waits和Implicit Waits。Waits方法可以和ExpectedCondition结合，这样在抓取数据时可以等待一段时间，若这段时间满足ExpectedCondition中指定的条件则执行后面的代码；若超过时间后仍未满足指定的条件，则抛出异常。下面是使用Explicit Waits的一段示例代码：

```python
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait # available since 2.4.0
from selenium.webdriver.support import expected_conditions as EC # available since 2.26.0

ff = webdriver.Firefox()
ff.get("http://somedomain/url_that_delays_loading")
try:
# 等待10s，若10s内能获取到ID为myDynamicElement的内容，则执行后面的代码；否则抛出异常
    element = WebDriverWait(ff, 10).until(EC.presence_of_element_located((By.ID, "myDynamicElement")))
finally:
    ff.quit()
```

本例中，看涨合约每次都可以抓取到，看跌合约则不稳定，大概网页是先执行看涨合约的函数，再执行看跌合约的函数（猜的，没有看代码），因此看跌合约数据的加载要慢一点。

修改后的代码如下：

```python
# -*- coding: utf-8 -*-
from bs4 import BeautifulSoup
from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
import urllib2
import time

baseUrl = "http://stock.finance.sina.com.cn/option/quotes.html"
csvPath = "FinanceData.csv"
csvFile = open(csvPath, 'w')

def is_chinese(uchar):
        # 判断一个unicode是否是汉字
    if uchar >= u'\u4e00' and uchar<=u'\u9fa5':
        return True
    else:
        return False

def readPage(url):
    webURL = urllib2.urlopen(baseUrl)
    content = webURL.read()
    soup = BeautifulSoup(content)
    return soup

def getFinance(soup, tableName):
    divs = soup.findAll('div', attrs={'class': tableName}) #看涨合约在这个div中
    if len(divs) < 0 or len(divs) == 0:
        print "No div class named " + str(tableName)
        return
    tbs = divs[0].findChildren('tbody') # 獲取tbody內容，在這個標籤下只有一個tbody
    print tbs[0]
    trs = tbs[0].findChildren('tr') # tr就是table中的每一行
    for tr in trs: 
        tds = tr.findChildren('td') # td是表格中的內容
        content = list()
        string = ""
        print tr

        index = 0 # 判斷漢字出現的位置
        for td in tds:
            temp = td.text
            print temp
            if index == 7 or index == 0:
                temp2 = ""
                for d in temp:
                    if not is_chinese(d): # 去除漢字
                        temp2 += d
                temp = temp2

            string = string + temp
            string = string + ","
            index += 1

            #content.append(td.text)
        #print content
        #string = string[:-1]
        print string
        csvFile.write(string)
        csvFile.write('\n')

tableName = "table_down fr" # 表格名稱

driver = webdriver.PhantomJS(executable_path='/Users/Pan/node_modules/phantomjs/lib/phantom/bin/phantomjs')
driver.get(baseUrl)

####################################################
# wait 10s until the specified table name presents
try:
# 看涨合约和看跌合约的表格是一个class，要用CLASS_NAME指定
    element = WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.CLASS_NAME, tableName)))
except Exception, e:
    print e
finally:
    data = driver.page_source # 取到加載js後的頁面content
    driver.quit
####################################################

#soup = readPage(loadUrl)
soup = BeautifulSoup(data)
getFinance(soup, tableName)
print "Finished!"
csvFile.close()
```](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=phantomJS&t=blog)](https://so.csdn.net/so/search/s.do?q=Selenium&t=blog)




