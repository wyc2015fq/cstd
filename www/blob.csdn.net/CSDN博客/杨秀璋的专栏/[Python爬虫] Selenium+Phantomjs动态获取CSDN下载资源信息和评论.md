
# [Python爬虫] Selenium+Phantomjs动态获取CSDN下载资源信息和评论 - 杨秀璋的专栏 - CSDN博客

2015年08月24日 02:28:26[Eastmount](https://me.csdn.net/Eastmount)阅读数：11404所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)



前面几篇文章介绍了Selenium、PhantomJS的基础知识及安装过程，这篇文章是一篇应用。通过Selenium调用Phantomjs获取CSDN下载资源的信息，最重要的是动态获取资源的评论，它是通过JavaScript动态加载的，故通过Phantomjs模拟浏览器加载获取。
希望该篇基础性文章对你有所帮助，如果有错误或不足之处，请海涵~
[[Python爬虫]
 在Windows下安装PhantomJS和CasperJS及入门介绍(上)](http://blog.csdn.net/eastmount/article/details/47023199)
[[Python爬虫]
 在Windows下安装PIP+Phantomjs+Selenium](http://blog.csdn.net/eastmount/article/details/47785123)
[[Python爬虫]
 Selenium自动访问Firefox和Chrome并实现搜索截图](http://blog.csdn.net/eastmount/article/details/47799865)
[[Python爬虫] Selenium实现自动登录163邮箱和Locating Elements介绍](http://blog.csdn.net/eastmount/article/details/47825633)

## 源代码
```python
# coding=utf-8  
  
from selenium import webdriver  
from selenium.webdriver.common.keys import Keys  
import selenium.webdriver.support.ui as ui  
from selenium.webdriver.common.action_chains import ActionChains  
import time      
import re      
import os  
  
#打开Firefox浏览器 设定等待加载时间 访问URL  
driver = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")  
driver_detail = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")  
wait = ui.WebDriverWait(driver,10)  
driver.get("http://download.csdn.net/user/eastmount/uploads")  
SUMRESOURCES = 0 #全局变量 记录资源总数(尽量避免)  
  
  
#获取列表页数 <div class="page_nav>共46个 共8页..</div>  
def getPage():  
    number = 0  
    wait.until(lambda driver: driver.find_element_by_xpath("//div[@class='page_nav']"))  
    texts = driver.find_element_by_xpath("//div[@class='page_nav']").text  
    print texts  
    m = re.findall(r'(\w*[0-9]+)\w*',texts) #正则表达式寻找数字  
    print '页数：' + str(m[1])  
    return int(m[1])  
  
  
#获取URL和文章标题   
def getURL_Title(num):  
    global SUMRESOURCES  
    url = 'http://download.csdn.net/user/eastmount/uploads/' + str(num)  
    print unicode('下载列表URL: ' + url,'utf-8')  
    ''''' 
    ' 等待最下面页面加载成功 获取URL和标题 
    ' 源码 
    ' <div class='list-container mb-bg'> 
    '     <dl> 
    '        <dt> 
    '           <div class="icon"><img src="xxx"></img></div> 
    '           <h3><a href="/detail/eastmount/8757243">MFC显示BMP图片</a></h3> 
    '        </dt> 
    '     </dl> 
    ' </div> 
    ' get_attribute('href')获取URL且自动补齐 
    ' unicode防止报错 - s.encode('utf8')unicode转换成utf8编码 decode表示utf8转换成unicode 
    '''  
    driver.get(url)  
    wait.until(lambda driver: driver.find_element_by_xpath("//div[@class='page_nav']"))  
    list_container = driver.find_elements_by_xpath("//div[@class='list-container mb-bg']/dl/dt/h3/a") 
    for title in list_container:  
        print 'Num' + str(SUMRESOURCES +1)  
        print u'标题: ' + title.text  
        print u'链接: ' + title.get_attribute('href')  
        SUMRESOURCES = SUMRESOURCES +1  
        #  
        #获取具体内容和评论  
        getDetails( str(title.get_attribute('href')) )  
    else:  
        print ' ' #换行  
          
  
#获取详细信息 因前定义的driver正在使用中 故调用driver_detail  
#否则报错 Message: Error Message => 'Element does not exist in cache'  
def getDetails(url):  
    #获取infobox  
    driver_detail.get(url)  
    details = driver_detail.find_element_by_xpath("//div[@class='info']").text  
    print details  
    #加载评论 <dl><dt></dt><dd></dd></dl>  
    comments = driver_detail.find_elements_by_xpath("//dl[@class='recom_list']/dd")  
    for com in comments:  
        print u'评论：' + com.text  
    else:  
        print ' ' #换行  
       
  
#主函数  
def main():  
    start = time.clock()  
    pageNum = getPage()  
    i=1  
    #循环获取标题和URL  
    while(i<=pageNum):  
        getURL_Title(i)   
        i = i + 1  
    else:  
        print 'SUmResouces: ' + str(SUMRESOURCES)  
        print 'Load Over'  
    end = time.clock()  
    print "Time: %f s" % (end - start)  
          
main()
```

## 代码实现步骤
1.首先获取页面总数，通过getPage()函数实现；
2.每个页面有一列资源，通过driver的find_element_by_xpath()路径获取标题和get_attribute('href')函数获取URL，它会自动补齐链接；
3.根据步骤2获取资源的URL，去到具体资源获取消息框和评论信息；
4.由于采用Phantomjs无界面浏览器加载页面，故获取class=info和recom_list的div即可。

## 运行结果
运行结果如下图所示：
![](https://img-blog.csdn.net/20150824013843994)
![](https://img-blog.csdn.net/20150824013900310)

## 程序分析
首先获取如下图所示的页面总数，此时为“**8**”页。它通过如下代码实现：
**texts = driver.find_element_by_xpath("//div[@class='page_nav']").text**
然后再while(i<=8)依次遍历每页的资源，每页资源的URL链接为：
**http://download.csdn.net/user/eastmount/uploads/8**

![](https://img-blog.csdn.net/20150824014537436)

再获取每页所有资源的标题及URL，通过代码如下：
```python
list_container = driver.find_elements_by_xpath("//div[@class='list-container mb-bg']/dl/dt/h3/a")  
for title in list_container:  
    print 'Num' + str(SUMRESOURCES +1)  
    print u'标题: ' + title.text  
    print u'链接: ' + title.get_attribute('href')
```
其中对应的源码如下所示，通过获取find_elements_by_xpath()获取多个元素，其div的class='list-container mb-bg'，同时路径为<div><dl><dt><h3><a>即可。同时自动补齐URL，如：
<a href='/detail/eastmount/6917799'会添加“http://download.csdn.net/”。
![](https://img-blog.csdn.net/20150824020420851)

最后在进入具体的资源获取相应的消息盒InfoBox和评论信息，由于通过模拟Phantomjs浏览器直接可以显示动态JS评论信息。
![](https://img-blog.csdn.net/20150824020835274)
![](https://img-blog.csdn.net/20150824020850812)

而如果通过BeautifulSoup只能获取的HTML源码如下，并没有JS信息。因为它是动态加载的，这就体现了Phantomjs的优势。而通过Chrome或FireFox浏览器审查元素能查看具体的评论div，这也是模拟浏览器的用处所在吧！
可对比前面写过的文章：[[Python学习] 简单爬取CSDN下载资源信息](http://blog.csdn.net/eastmount/article/details/46986589)

```python
<div class="section-list panel panel-default">  
   <div class="panel-heading">  
      <h3 class="panel-title">资源评论</h3>  
   </div>  
   <!-- recommand -->  
   <script language='JavaScript' defer type='text/javascript' src='/js/comment.js'></script>  
   <div class="recommand download_comment panel-body" sourceid="8772951"></div>  
</div>
```

## 总结
这篇文章主要讲述通过Selenium和Phantomjs获取CSDN下载资源信息的过程，其中由于driver调用Chrome或FireFox浏览器总会有额外空间增加，故调用Phantomjs无界面浏览器完成。同时有几个问题：
1.如何避免Phantomjs的黑框弹出；
2.程序的运行时间比较低，响应时间较慢，如何提高？
接下来如果有机会准备尝试的内容包括：
1.下载百度百科的旅游地点InfoBox（毕设知识图谱挖掘）；
2.如何爬取搜狐图片的动态加载图片，完成智能爬图工具；
3.当需要自动登录时driver访问Chrome或FireFox浏览器发送消息。
最后希望文章对你有所帮助吧！如果有错误或不足之处，还请海涵~
（By:Eastmount 2015-8-24 深夜2点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


