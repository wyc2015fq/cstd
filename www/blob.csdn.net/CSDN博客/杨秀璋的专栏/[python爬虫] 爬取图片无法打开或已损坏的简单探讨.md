
# [python爬虫] 爬取图片无法打开或已损坏的简单探讨 - 杨秀璋的专栏 - CSDN博客

2015年12月07日 06:25:10[Eastmount](https://me.csdn.net/Eastmount)阅读数：12816标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[python爬虫																](https://so.csdn.net/so/search/s.do?q=python爬虫&t=blog)[selenium																](https://so.csdn.net/so/search/s.do?q=selenium&t=blog)[图片无法显示																](https://so.csdn.net/so/search/s.do?q=图片无法显示&t=blog)[百度图片																](https://so.csdn.net/so/search/s.do?q=百度图片&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=图片无法显示&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=selenium&t=blog)个人分类：[Python爬虫																](https://blog.csdn.net/Eastmount/article/category/5758691)
[
																					](https://so.csdn.net/so/search/s.do?q=selenium&t=blog)所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)[
							](https://so.csdn.net/so/search/s.do?q=selenium&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=python爬虫&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=python&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=python&t=blog)

本文主要针对python使用urlretrieve或urlopen下载百度、搜狗、googto（谷歌镜像）等图片时，出现"无法打开图片或已损坏"的问题，作者对它进行简单的探讨。同时，作者将进一步帮你巩固selenium自动化操作和urllib库等知识。
感谢朋友"露为霜"的帮助！希望以后能实现强大的图片爬虫代码~

## 一. 引入Selenium自动爬取百度图片
下面这部分Selenium代码的主要功能是：
1.先自动运行浏览器，并访问百度图片链接：[http://image.baidu.com/](http://image.baidu.com/)
2.通过driver.find_element_by_xpath()函数获取输入框的位置；
3.在输入框中自动输入搜索关键词"邓肯"，再输入回车搜索"邓肯"相关图片；
4.再通过find_element_by_xpath()获取图片的原图url，这里仅获取一张图片；
5.调用urllib的urlretrieve()函数下载图片。
最后整个动态效果如下图所示，但是图片却无法显示：
![](https://img-blog.csdn.net/20151207005223353)
代码如下：
```python
# -*- coding: utf-8 -*-
import urllib
import re
import time
import os
from selenium import webdriver          
from selenium.webdriver.common.keys import Keys 
import selenium.webdriver.support.ui as ui
from selenium.webdriver.common.action_chains import ActionChains         
#Open PhantomJS
#driver = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")  
driver = webdriver.Firefox() 
wait = ui.WebDriverWait(driver,10)
#Search Picture By Baidu
url = "http://image.baidu.com/"
name = u"邓肯"
driver.get(url)
elem_inp = driver.find_element_by_xpath("//form[@id='homeSearchForm']/span[1]/input")  
elem_inp.send_keys(name)  
elem_inp.send_keys(Keys.RETURN)
time.sleep(5)
#Get the URL of Pictures
#elem_pic = driver.find_element_by_xpath("//div[@class='imgpage']/ul/li/div/a")
elem_pic = driver.find_element_by_xpath("//div[@class='imgpage']/ul/li/div/a/img")
elem_url = elem_pic.get_attribute("src")
print elem_url
#Download Pictures
driver.get(elem_url)
urllib.urlretrieve(elem_url,"picture.jpg")
print "Download Pictures!!!"
```

## 二. 简单分析原因及知识巩固
**1.urllib.urlretrieve()**
通过urlretrieve()函数可设置下载进度发现图片是一下子就加载的。这里给大家巩固这个urlretrieve函数的方法和Python时间命名方式，代码如下：

```python
# -*- coding: utf-8 -*-
import urllib
import time
import os
#显示下载进度
def schedule(a,b,c):
    #a:已下载的数据块 b:数据块的大小 c:远程文件的大小
    per = 100.0 * a * b / c
    if per > 100 :
        per = 100
    print '%.2f%%' % per
if __name__ == '__main__':
    url = "http://img4.imgtn.bdimg.com/it/u=3459898135,859507693&fm=11&gp=0.jpg"
    #定义文件名 时间命名
    t = time.localtime(time.time())
    #反斜杠连接多行
    filename = str(t.__getattribute__("tm_year")) + "_" + \
               str(t.__getattribute__("tm_mon")) + "_" + \
               str(t.__getattribute__("tm_mday"))
    target = "%s.jpg" % filename
    print target
    urllib.urlretrieve(url,target,schedule)
    print "Download Picture!!!"
```
发现该图片的大小仅为168字节，其中输出结果如下图，获取的URL地址如下：
[http://img4.imgtn.bdimg.com/it/u=3459898135,859507693&fm=11&gp=0.jpg](http://img4.imgtn.bdimg.com/it/u=3459898135,859507693&fm=11&gp=0.jpg)
而换张图片是能显示下载进度的，如我的头像。显然我想让程序加个进度就能爬取图片的想法失败。头像地址：[http://avatar.csdn.net/F/8/5/1_eastmount.jpg](http://avatar.csdn.net/F/8/5/1_eastmount.jpg)
![](https://img-blog.csdn.net/20151207015652175)
猜测可能获取的百度URL不是原图地址，或者是个服务器设置了相应的拦截或加密。参考"[Python爬虫抓取网页图片](http://blog.csdn.net/cruise_h/article/details/25737737)"，函数相关介绍如下：

```python
>>> help(urllib.urlretrieve)
Help on function urlretrieve in module urllib:
urlretrieve(url, filename=None, reporthook=None, data=None)
参数url：
    指定的下载路径
参数 finename：
    指定了保存本地路径（如果参数未指定，urllib会生成一个临时文件保存数据。）
参数 reporthook：
    是一个回调函数，当连接上服务器、以及相应的数据块传输完毕时会触发该回调，
    我们可以利用这个回调函数来显示当前的下载进度。
参数 data：
    指 post 到服务器的数据，该方法返回一个包含两个元素的(filename, headers)元组，
    filename 表示保存到本地的路径，header 表示服务器的响应头。
```
**2****.urllib2.urlopen()**
换个方法urlopen()实现，同时设置消息头试试，并输出信息和图片大小。

```python
# -*- coding: utf-8 -*-
import os      
import sys    
import urllib
import urllib2
#设置消息头
url = "http://img4.imgtn.bdimg.com/it/u=3459898135,859507693&fm=11&gp=0.jpg"
header = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) \
        AppleWebKit/537.36 (KHTML, like Gecko) \
        Chrome/35.0.1916.114 Safari/537.36',
    'Cookie': 'AspxAutoDetectCookieSupport=1'
}
request = urllib2.Request(url, None, header)
response = urllib2.urlopen(request)
print response.headers['Content-Length']
with open("picture.jpg","wb") as f:
    f.write(response.read())
print response.geturl()
print response.info()    #返回报文头信息
print urllib2.urlopen(url).read()
```
返回内容是”HTTPError: HTTP Error 403: Forbidden“，Selenium打开如下：
![](https://img-blog.csdn.net/20151207035654008)
其中403错误介绍如下，服务器拒绝服务：
![](https://img-blog.csdn.net/20151207035756661)
换成我的博客图像那张图是能下载的，同时设置消息头和代理，推荐一篇文章：
[[Python]网络爬虫（五）：urllib2的使用细节与抓站技巧](http://blog.csdn.net/pleasecallmewhy/article/details/8925978)

## 三. 解决方法
主要参考三篇文章和自己的一些想法：
[selenium+python 爬取网络图片(2) -- 百度](http://blog.csdn.net/seanwang_25/article/details/43318907)
[Python 3 多线程下载百度图片搜索结果](http://lovenight.github.io/2015/11/15/Python-3-%E5%A4%9A%E7%BA%BF%E7%A8%8B%E4%B8%8B%E8%BD%BD%E7%99%BE%E5%BA%A6%E5%9B%BE%E7%89%87%E6%90%9C%E7%B4%A2%E7%BB%93%E6%9E%9C/)
[CSDN博客搬家到WordPress  - curl设置headers爬取](blog.csdn.net/sunboy_2050/article/details/50095929)
**第一个方法 F12审查元素和SRC的骗局**
这是感谢"露为霜"同学提供的方法，如果你通过浏览器点开百度搜索"邓肯"的第一张图片，复制网址后，会发现图片真实的地址为：
[http://gb.cri.cn/mmsource/images/2015/11/22/sb2015112200073.jpg](http://gb.cri.cn/mmsource/images/2015/11/22/sb2015112200073.jpg)
此时你再分析百度搜索页面，你会发现"F12审查元素和获取src元素的行为欺骗了你"，正是因为它俩定位到了错误的图片链接。而真实的URL是在"ul/li/"中的"data-objurl"属性中。
![](https://img-blog.csdn.net/20151207051701675)
代码如下：

```python
# -*- coding: utf-8 -*-
import urllib
import re
import time
import os
from selenium import webdriver          
from selenium.webdriver.common.keys import Keys
import selenium.webdriver.support.ui as ui        
from selenium.webdriver.common.action_chains import ActionChains
#Open PhantomJS
#driver = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")  
driver = webdriver.Firefox() 
wait = ui.WebDriverWait(driver,10)
#Search Picture By Baidu
url = "http://image.baidu.com/"
name = u"邓肯"
driver.get(url)
elem_inp = driver.find_element_by_xpath("//form[@id='homeSearchForm']/span[1]/input")  
elem_inp.send_keys(name)  
elem_inp.send_keys(Keys.RETURN)
time.sleep(5)
#Get the URL of Pictures
num = 1
elem_pic = driver.find_elements_by_xpath("//div[@class='imgpage']/ul/li")
for elem in elem_pic:
    elem_url = elem.get_attribute("data-objurl")
    print elem_url
    #Download Pictures
    name = "%03d" % num
    urllib.urlretrieve(elem_url, str(name) + ".jpg")
    num = num + 1
else:
    print "Download Pictures!!!"
```
运行代码成功爬取了9张图片，显然成功了！虽然最后报错：IOError: [Errno socket error] [Errno 10060] ，只爬取了9张图片，但是至少可以正确解决了该问题。运行截图如下所示：
![](https://img-blog.csdn.net/20151207051919086)
同样的道理，googto的elem.get_attribute("src")改成elem.get_attribute("data-imgurl")即可获取正确的图片地址并正确下载。
PS：百度图片动态加载的功能是非常强大的，当你的鼠标拖动时，它会自动增加新的页面，在<ul>中包括新的一批<li>张图片，这也是不同于其它网页在右下角点击"1、2、3..."翻页的，可能也会成为海量图片爬取的又一难点。
**第二个方法 Selenium使用右键另存为**
还是使用老的链接，虽然读取是无法显示的，但尝试通过Selenium的鼠标右键另存为功能，看能不能爬取成功。
```python
# -*- coding: utf-8 -*-
import urllib
import re
import time
import os
from selenium import webdriver          
from selenium.webdriver.common.keys import Keys
import selenium.webdriver.support.ui as ui        
from selenium.webdriver.common.action_chains import ActionChains
#Open PhantomJS 
driver = webdriver.Firefox()
wait = ui.WebDriverWait(driver,10)
#Search Picture By Baidu
url = "http://image.baidu.com/"
name = u"邓肯"
driver.get(url)
elem_inp = driver.find_element_by_xpath("//form[@id='homeSearchForm']/span[1]/input")  
elem_inp.send_keys(name)  
elem_inp.send_keys(Keys.RETURN)
time.sleep(5)
#Get the URL of Pictures
elem_pic = driver.find_element_by_xpath("//div[@class='imgpage']/ul/li/div/a/img")
elem_url = elem_pic.get_attribute("src")
print elem_url
#鼠标移动至图片上 右键保存图片
driver.get(elem_url)
print driver.page_source
elem = driver.find_element_by_xpath("//img")
action = ActionChains(driver).move_to_element(elem)
action.context_click(elem) #右键  
#当右键鼠标点击键盘光标向下则移动至右键菜单第一个选项
action.send_keys(Keys.ARROW_DOWN)
action.send_keys('v') #另存为
action.perform()
print "Download Pictures!!!"
```
运行效果如下图所示。虽然它能实现右键另存为，但是需要手动点击保存，其原因是selenium无法操作操作系统级的对话框，又说"set profile"代码段的设置能解决问题的并不靠谱。通过钩子Hook函数可以实现，以前做过C\#的钩子自动点击功能，但是想到下载图片需要弹出并点击无数次对话框就很蛋疼，所以该方法并不好！
钩子函数java版本结合robot可以阅读下面这篇文章：
[selenium webdriver 右键另存为下载文件（结合robot and autoIt）](http://www.cnblogs.com/tobecrazy/p/3969390.html)
![](https://img-blog.csdn.net/20151207060517754)![](https://img-blog.csdn.net/20151207060545516)
**第三个方法 通过Selenium自动点击百度的下载按钮**
其实现过程就是通过Selenium找到"下载"按钮，再点击或获取链接即可。
该方法参考文章：[selenium+python 爬取网络图片(2) -- 百度](http://blog.csdn.net/seanwang_25/article/details/43318907)
同时，这里需要强调百度动态加载，可以通过Selenium模拟滚动窗口实现，也参考上面文章。其中核心代码为：
driver.maximize_window()
pos += i*500\# 每次下滚500
js ="document.documentElement.scrollTop=%d"% pos
driver.execute_script(js)
**第四个方法 百度图片解码下载及线程实现**
参考文章：[Python
 3 多线程下载百度图片搜索结果](http://lovenight.github.io/2015/11/15/Python-3-%E5%A4%9A%E7%BA%BF%E7%A8%8B%E4%B8%8B%E8%BD%BD%E7%99%BE%E5%BA%A6%E5%9B%BE%E7%89%87%E6%90%9C%E7%B4%A2%E7%BB%93%E6%9E%9C/)
最近看了一些优秀的文章，真心感觉自己缕蚁一般，太过渺小，还有好多知识需要学习啊！加油~而且不知道现在自己做的这些东西是否有用？心理的几个想法一直还未实现，挺担心的。还是自己博客描述那句话：
无知的自己 · 乐观的面对 · 谦逊的学习 · 低调的前行 · 更要会生活
希望文章对你有所帮助，如果有错误或不足之处，还请海涵~
（By:Eastmount 2015-12-07 清晨6点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


