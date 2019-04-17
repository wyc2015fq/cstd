# Python多线程Selenium跨浏览器测试 - zhusongziye的博客 - CSDN博客





2017年12月03日 20:23:00[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：217








## 前言


在web测试中，不可避免的一个测试就是浏览器兼容性测试，在没有自动化测试前，我们总是苦逼的在一台或多台机器上安装N种浏览器，然后手工在不同的浏览器上验证主业务流程和关键功能模块功能，以检测不同浏览器或不同版本浏览器上，我们的web应用是否可以正常工作。

![](https://img-blog.csdn.net/20171203201157691?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



下面我们看看怎么利用python selenium进行自动化的跨浏览器测试。

## 什么是跨浏览器测试


跨浏览器测试是功能测试的一个分支，用以验证web应用能在不同的浏览器上正常工作。

## 为什么需要跨浏览器测试


通常情况下，我们都期望web类应用能够被我们的用户在任何浏览器上使用。例如，有的人喜欢用IE来打开开源优测web站点http://www.testingunion.com，但有的人喜欢firefox或chrome。


我们期望我们的web系统能在任何浏览器上正常的工作，这样能吸引更多的用户来使用。


需要跨浏览器测试的根源是：
- 

在不同浏览器字体大小不匹配

- 

javascrpit的实现不一样

- 

css、html的验证有所区别

- 

有的浏览器或低版本不支持HTML5

- 

页面对齐和div大小问题

- 

图片位置或大小问题

- 

浏览器和操作系统间的兼容问题



以上几个方面不仅仅对布局有影响，甚至会导致功能不可用，所以我们需要进行跨浏览器测试。

## 如何执行跨浏览器测试


如果我们使用selenium webdriver，那我们就能够自动的在IE、firefox、chrome、等不同浏览器上运行测试用例。


为了能在同一台机器上不同浏览器上同时执行测试用例，我们需要多线程技术。


下面我们基于python的多线程技术来尝试同时启动多个浏览器进行selenium自动化测试。

```
#-*- coding:utf-8 -*-

from selenium import webdriver
import sys
from time import sleep
from threading import Thread

reload(sys)
sys.setdefaultencoding("utf-8")

def test_baidu_search(browser, url):
    driver = None
    # 你可以自定义这里，添加更多浏览器支持进来
    if browser == "ie":
        driver = webdriver.Ie() 
    elif browser == "firefox":
        driver = webdriver.Firefox() 
    elif browser == "chrome":
        driver = webdriver.Chrome() 
    
    if driver == None:
        exit()    
    print u"开始[case_0001]百度搜索"
    driver.get(url)    

    print u"清除搜索中数据，输入搜索关键词"
    driver.find_element_by_id("kw").clear()
    driver.find_element_by_id("kw").send_keys(u"开源优测")    
    
    print u"单击 百度一下 按钮 开始搜索"
    driver.find_element_by_id("su").click()
    sleep(3)    

    print u"关闭浏览器，退出webdriver"
    driver.quit()    if __name__ == "__main__":    
    # 浏览器和首页url
    data = {        
        "ie":"http://www.baidu.com", 
        "firefox":"http://www.baidu.com", 
        "chrome":"http://www.baidu.com"
        }   

         
    # 构建线程 
    threads = []  
    for b, url in data.items():  
       t = Thread(target=test_baidu_search,args=(b,url))
       threads.append(t)  
    
    # 启动所有线程
    for thr in threads:
        thr.start()
```


运行上述代码，你会发现IE、firefox、chrome都会启动开始进行百度搜索，是不是很有意思？当然上面只是简单的演示，更多更实用的能力有待挖掘。

## 总结


本文初始演示了利用python多线程技术来启动多个浏览器同时进行selenium自动化测试，通过这个示例你应该要去学习更深入的知识，和深入结合实际业务测试梳理出更合适的自动化测试业务场景。


至于如何更深入的利用selenium把兼容性测试做好，还有待深入研究挖掘，真正的把selenium的特性用好。



