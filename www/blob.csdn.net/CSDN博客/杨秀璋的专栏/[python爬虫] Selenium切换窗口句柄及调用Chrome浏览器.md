# [python爬虫] Selenium切换窗口句柄及调用Chrome浏览器 - 杨秀璋的专栏 - CSDN博客





2016年11月21日 04:36:27[Eastmount](https://me.csdn.net/Eastmount)阅读数：14022
所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)








        因为我的博客是根据我每个阶段自己做的事情而写的，所以前言可能有点跑题，但它更有利于让我回忆这个阶段自己的所作所为。这篇文章主要介绍Selenium爬虫获取当前窗口句柄、切换窗口句柄以及调用Chrome浏览器几个知识点，其中获取当前句柄的方法是非常重要的一个知识点，只有真正遇到的时候才能体会到它的强大。
        最近课程比较多，写博客的内容也比较杂，包括网站搭建、HTML设计、计算机图形学、Python数据挖掘、Flash动画等。同时，这周也有老师让我帮忙写几个爬虫，自己进一步理解了爬虫的好处：一方面，人文课题如果运用计算机相关编程知识，确实能够解决很多问题并提升效率；另一方面，我爬虫主要使用Selenium、BeautifulSoup和Scrapy，其中Selenium能够结合浏览器解决登录验证码、扫二维码、效果预览、自动测试等问题；BeautifulSoup的优势是速度，两种方法都是通过分析DOM树结构实现的。
        希望这篇文章对你有所帮助，如果有错误或不足之处，还请海涵~更多爬虫主题参考我的专栏：[http://blog.csdn.net/column/details/eastmount-spider.html](http://blog.csdn.net/column/details/eastmount-spider.html)



## 一. 调用Chrome浏览器


        首先，假设通过Firefox()浏览器定向爬取CSDN首页导航栏信息，审查元素代码如下图所示，在div class="menu"路径的ul、li、a下，同时可以定位ul class="clearfix"。



![](https://img-blog.csdn.net/20161121033717456)



        代码如下所示：



```python
# coding=utf-8  
import os   
from selenium import webdriver

#PhantomJS无界面浏览器
##driver = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")  

#打开火狐浏览器
driver = webdriver.Firefox()
url = "http://www.csdn.net/"
driver.get(url)

#xpath路径定位导航栏
elem_dh = driver.find_elements_by_xpath("//div[@class='menu']/ul/li/a")
for elem in elem_dh:
    print elem.text                    #获取正文
    print elem.get_attribute('href')   #获取属性值
```
        然后转换成chrome浏览器，只需要在"C:\Program Files (x86)\Google\Chrome\Application\"路径下放置个chromedriver.exe驱动，再进行调用即可，代码如下所示：


```python
# coding=utf-8  
import os   
from selenium import webdriver

#PhantomJS无界面浏览器
##driver = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")  

#打开火狐浏览器
#driver = webdriver.Firefox()

#谷歌浏览器
chromedriver = "C:\Program Files (x86)\Google\Chrome\Application\chromedriver.exe"
os.environ["webdriver.chrome.driver"] = chromedriver
driver = webdriver.Chrome(chromedriver)

#WebDriverException: Message: unknown error: session deleted because of page crash

url = "http://www.csdn.net/"
driver.get(url)

#xpath路径定位导航栏
elem_dh = driver.find_elements_by_xpath("//div[@class='menu']/ul/li/a")
for elem in elem_dh:
    print elem.text                    #获取正文
    print elem.get_attribute('href')   #获取属性值
    print '\n'
```
        运行结果如下图所示：
```python
>>> ================================ RESTART ================================
>>> 
极客头条
http://geek.csdn.net/
知识库
http://lib.csdn.net/
学院
http://edu.csdn.net/
论坛
http://bbs.csdn.net/
博客
http://blog.csdn.net/
下载
http://download.csdn.net/
问答
http://ask.csdn.net/
商城
http://mall.csdn.net/
ITeye
http://www.iteye.com/
CODE
https://code.csdn.net/
活动
http://huiyi.csdn.net/
CTO
http://cto.csdn.net/
外包
http://www.csto.com/
程序员
http://special.csdncms.csdn.net/programmer-covers
APP
http://www.csdn.net/app
>>>
```





## 二. 获取当前句柄并切换窗口句柄

        在使用Selenium爬取知识过程，通常会遇到_blank弹出新窗体，或窗体是弹出的那种情况，而且有的需要登录，如：新浪微博、微信公众号、京东等，使用webdriver.Firefox()重新打开新窗体是无法加载已有信息的。这时候就需要通过获取当前句柄再进行窗口切换。
        这里使用该方法实现获取CSDN导航栏的每条导航的标题信息，因为它是_blank弹出新窗体。需要注意每次窗口句柄移动都需要重新定位它们的主窗体。
**核心代码：**


```python
#获取当前窗口句柄
now_handle = driver.current_window_handle 
print now_handle 

#获取所有窗口句柄
all_handles = driver.window_handles 
for handle in all_handles:
    if handle!=now_handle:
        #输出待选择的窗口句柄
        print handle
        driver.switch_to_window(handle)
        time.sleep(1)

        #具体操作
        elem_bt = driver.find_element_by_xpath("...")
        driver.close() #关闭当前窗口

#输出主窗口句柄
print now_handle
driver.switch_to_window(now_handle) #返回主窗口
```
        如下图所示：<a href="xxxx" target="_blank">极客头条</a>


![](https://img-blog.csdn.net/20161121033717456)


实例代码如下所示，它会依次打开每个导航栏，在爬取关闭。


```python
# coding=utf-8  
import os
import time
from selenium import webdriver

#By：Eastmoun 2016-11-21 凌晨4:26

#打开火狐浏览器
driver = webdriver.Firefox()
url = "http://www.csdn.net/"
driver.get(url)

#xpath路径定位导航栏
elem_dh = driver.find_elements_by_xpath("//div[@class='menu']/ul/li/a")

#获取当前窗口句柄
now_handle = driver.current_window_handle 
print now_handle


#循环获取界面
for elem in elem_dh:
    print elem.text                    #获取正文
    print elem.get_attribute('href')   #获取属性值
    #点击进入新的界面 _blank弹出
    elem.click()

    #获取所有窗口句柄
    all_handles = driver.window_handles
    
    #弹出两个界面,跳转到不是主窗体界面
    for handle in all_handles:
        if handle!=now_handle:   
            #输出待选择的窗口句柄
            print handle
            driver.switch_to_window(handle)
            time.sleep(1)

            print u'弹出界面信息'
            print driver.current_url
            print driver.title

            #获取登录连接信息
            elem_sub = driver.find_element_by_xpath("//ul[@class='btns']/li/a")
            print elem_sub.text
            print elem_sub.get_attribute('href')
            print ''

            #关闭当前窗口
            driver.close()
            

    #输出主窗口句柄
    print now_handle
    driver.switch_to_window(now_handle) #返回主窗口 开始下一个跳转
```

        输出结果如下所示：

```python
>>> ================================ RESTART ================================
>>> 
{833ee0ee-c408-432a-ab54-1222a22f580c}
极客头条
http://geek.csdn.net/
{50e7dfc6-72d8-4577-99ca-47344a068528}
弹出界面信息
http://geek.csdn.net/
最新最热 - 极客头条 - CSDN.NET
登录 
https://passport.csdn.net/account/login?ref=toolbar

{833ee0ee-c408-432a-ab54-1222a22f580c}
知识库
http://lib.csdn.net/
{4501a3d1-8ab3-4a5e-984b-ace648494f94}
弹出界面信息
http://lib.csdn.net/
知识库 - 你身边的技术百科全书 - CSDN
登录 
https://passport.csdn.net/account/login?ref=toolbar

{833ee0ee-c408-432a-ab54-1222a22f580c}
学院
http://edu.csdn.net/
{4525da42-fd77-4f04-b2f9-1687d75c1e19}
弹出界面信息
http://edu.csdn.net/
CSDN学院 - CSDN.NET
登录 
https://passport.csdn.net/account/login?ref=toolbar

{833ee0ee-c408-432a-ab54-1222a22f580c}
论坛
http://bbs.csdn.net/
{c24493c1-c937-4a90-85a1-e11f7be870c4}
弹出界面信息
http://bbs.csdn.net/home
CSDN论坛首页 - CSDN.NET-CSDN论坛-CSDN.NET-中国最大的IT技术社区
登录 
https://passport.csdn.net/account/login?ref=toolbar

{833ee0ee-c408-432a-ab54-1222a22f580c}
博客
http://blog.csdn.net/
{a4686eae-b437-4b97-97a2-160024359ade}
弹出界面信息
http://blog.csdn.net/
博客频道 - CSDN.NET
登录 
https://passport.csdn.net/account/login?ref=toolbar

{833ee0ee-c408-432a-ab54-1222a22f580c}
下载
http://download.csdn.net/
{42cf058e-e093-4619-beae-4454aaba29a3}
弹出界面信息
http://download.csdn.net/
下载频道 - CSDN.NET
登录 
https://passport.csdn.net/account/login?ref=toolbar

{833ee0ee-c408-432a-ab54-1222a22f580c}
问答
http://ask.csdn.net/
{cb0633e2-45a5-406d-9aaa-79ea994e1f6a}
弹出界面信息
http://ask.csdn.net/
编程技术问答-CSDN问答频道
登录 
https://passport.csdn.net/account/login?ref=toolbar

{833ee0ee-c408-432a-ab54-1222a22f580c}
商城
http://mall.csdn.net/
{126f85af-d5e4-4d40-a89e-6fbb575a972b}
弹出界面信息
http://mall.csdn.net/
CSDN商城
登录 
https://passport.csdn.net/account/login?ref=toolbar
```
        PS：这段代码主要是让你熟悉获取当前窗体及切换窗体，真正爬虫用到的时候是非常方便的，而且必须要使用。


![](https://img-blog.csdn.net/20161121042353293)








## 三. 一段代码 在线笔记

        下面这段代码是我的在线笔记，由于代码非常有用，所以我隐藏了些东西，但绝对好用，而且是结合上面的内容。同时，在登录过程中，我建议大家time.sleep()函数暂停，输入验证码、扫描二维码都是可行的。


```python
# -*- coding: utf-8 -*-
# By: Eastmount 2016-11-21 凌晨4点半

from selenium import webdriver  
from selenium.webdriver.common.keys import Keys  
import time
import os
  

#模拟登陆
chromedriver = "C:\Program Files (x86)\Google\Chrome\Application\chromedriver.exe"
os.environ["webdriver.chrome.driver"] = chromedriver
driver = webdriver.Chrome(chromedriver)
now_handle = driver.current_window_handle #获取当前窗口句柄

#driver = webdriver.Firefox()
#暂停10秒手动输入网址
time.sleep(3)   
  
#用户名 密码
driver.get("xxxxxxxxxxx")
elem_user = driver.find_element_by_id("account")  
elem_user.send_keys("******")  
elem_pwd = driver.find_element_by_id("pwd")  
elem_pwd.send_keys("******")  
elem_pwd.send_keys(Keys.RETURN)
#暂定60秒扫二维码 跳转到定向页面
time.sleep(60)  

#登录用户管理 获取相关标签
print driver.current_url
elem_user = driver.find_elements_by_xpath("//td[@class='table_cell user']/div/a[1]")  
now_handle = driver.current_window_handle #获取当前窗口句柄
print now_handle   #输出当前获取的窗口句柄

#单击操作
for elem in elem_user:
    print '########################################'
    print elem.text
    #点击进入查看详细用户
    elem.click()

    """
    NoSuchElementException: Message: no such element
    因为总是获取当前句柄 故找不到相关的界面，需要窗口句柄转换
    """
    all_handles = driver.window_handles #获取所有窗口句柄
    for handle in all_handles:
        if handle!=now_handle:
            #输出待选择的窗口句柄
            print handle
            driver.switch_to_window(handle)
            time.sleep(1)
            
            #点击"图文消息"
            elem_tw = driver.find_element_by_xpath("//li[@class='tab_nav tab_appmsg width5']/a")
            #elem_tw = driver.find_element_by_xpath("//div[@class='tab_navs_wrp']/ul/li/a")

            print elem_tw.text
            print elem_tw.get_attribute("href")
            elem_tw.click()
            time.sleep(1)

            #点击 '从素材库中选择'按钮
            elem_sc = driver.find_element_by_xpath("//span[@class='create_access']/a")
            print elem_sc.text
            print elem_sc.get_attribute("href")
            elem_sc.click()
            time.sleep(1)

            #点击素材 '11-14 星期二中奖名单' 注意是id
            #elem_dj = driver.find_element_by_xpath("//div[@class='edit_mask appmsg_mask']")
            #elem_dj = driver.find_element_by_xpath("//div[@id='appmsg503811334']/div/div[2]")
            elem_dj = driver.find_element_by_xpath("//div[@id='appmsg503811334']")
            print elem_dj.text
            print elem_dj.get_attribute("href")
            elem_dj.click()
            time.sleep(1)
            
            #SyntaxError: Failed to execute 'evaluate' on 'Document': The string
            #'//div[@class='appmsg503811334']/div/' is not a valid XPath expression.

            #WebDriverException: Message: unknown error: Element is not clickable
            #at point (473, 361). Other element would receive the click:
            #<div class="appmsg_content">...</div>

            #获取'确定按钮'
            elem_bt = driver.find_element_by_xpath("//div[@class='dialog_ft']/span[1]/button")
            print elem_bt.text
            print elem_bt.get_attribute("class")
            elem_bt.click()
            time.sleep(1)

            #点击 '发送'
            elem_fs = driver.find_element_by_xpath("//span[@id='js_submit']/button")
            print elem_fs.text
            print elem_fs.get_attribute("class")
            elem_fs.click()
            time.sleep(1)

            #关闭当前窗口
            driver.close() 
            

    #输出主窗口句柄
    print now_handle
    driver.switch_to_window(now_handle) #返回主窗口
    #break

    print '\n\n'
    

#暂停换页
#登录用户管理 获取相关标签
print '********************************************'
print '********************************************'
print u'换页操作1'
#elem_next = driver.find_elements_by_xpath("//a[@class='btn page_next']")
#elem_next.click()

time.sleep(10)
elem_user = driver.find_elements_by_xpath("//td[@class='table_cell user']/div/a[1]")  
now_handle = driver.current_window_handle #获取当前窗口句柄
print now_handle   #输出当前获取的窗口句柄

#单击操作
for elem in elem_user:
    print '########################################'
    print elem.text
    #点击进入查看详细用户
    elem.click()

    all_handles = driver.window_handles #获取所有窗口句柄
    for handle in all_handles:
        if handle!=now_handle:
            #输出待选择的窗口句柄
            print handle
            driver.switch_to_window(handle)
            time.sleep(1)
            
            #点击"图文消息"
            elem_tw = driver.find_element_by_xpath("//li[@class='tab_nav tab_appmsg width5']/a")
            print elem_tw.text
            print elem_tw.get_attribute("href")
            elem_tw.click()
            time.sleep(1)

            #点击 '从素材库中选择'按钮
            elem_sc = driver.find_element_by_xpath("//span[@class='create_access']/a")
            print elem_sc.text
            print elem_sc.get_attribute("href")
            elem_sc.click()
            time.sleep(1)

            #点击素材 注意是id
            elem_dj = driver.find_element_by_xpath("//div[@id='appmsg503811334']")
            print elem_dj.text
            print elem_dj.get_attribute("href")
            elem_dj.click()
            time.sleep(1)
            
            #获取'确定按钮'
            elem_bt = driver.find_element_by_xpath("//div[@class='dialog_ft']/span[1]/button")
            print elem_bt.text
            print elem_bt.get_attribute("class")
            elem_bt.click()
            time.sleep(1)

            #点击 '发送'
            elem_fs = driver.find_element_by_xpath("//span[@id='js_submit']/button")
            print elem_fs.text
            print elem_fs.get_attribute("class")
            elem_fs.click()
            time.sleep(1)

            driver.close() #关闭当前窗口
            

    #输出主窗口句柄
    print now_handle
    driver.switch_to_window(now_handle) #返回主窗口
    #break
    print '\n\n'
```

        最后希望这篇文章对你有所帮助，因为第三部分代码涉密，消除了些东西，但原理和方法更重要，希望你也能灵活应用。同时明天评估专家就来了，虽然干活到2点，但还是想写一篇文章，记录最近两天研究的东西，很重要的一个知识点。继续享受博客和教学吧，同时期待我最近欣赏的某个人吧！杨老师加油~
       (By:Eastmount 2016-11-21 凌晨4点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))









