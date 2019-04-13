
# [python爬虫] BeautifulSoup和Selenium简单爬取知网信息测试 - 杨秀璋的专栏 - CSDN博客

2017年11月17日 11:16:57[Eastmount](https://me.csdn.net/Eastmount)阅读数：8301所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)



作者最近在研究复杂网络和知识图谱内容，准备爬取知网论文相关信息进行分析，包括标题、摘要、出版社、年份、下载数和被引用数、作者信息等。但是在爬取知网论文时，遇到问题如下：
1.爬取内容总为空，其原因是采用动态加载的数据，无法定位，然后作者重新选取了CNKI3.0知网进行了爬取；
2.但却不含作者信息，需要定位到详情页面，再依次获取作者信息，但是又遇到了新的问题。

## 一. 网站定位分析
知网网站如下：[http://nvsm.cnki.net/kns/brief/default_result.aspx](http://nvsm.cnki.net/kns/brief/default_result.aspx)
比如搜索Python关键字，网页反馈内容如下所示，2681篇文章。

![](https://img-blog.csdn.net/20171116141934330)

但是使用Selenium定位爬取的论文内容总为空，后来网上看到qiuqingyun大神的博客，发现另一个知网接口（CNKI3.0 知识搜索：[http://search.cnki.net/](http://search.cnki.net/)）。
强烈推荐大家阅读他的原文：[http://qiuqingyu.cn/2017/04/27/python实现CNKI知网爬虫/](http://qiuqingyu.cn/2017/04/27/python%E5%AE%9E%E7%8E%B0CNKI%E7%9F%A5%E7%BD%91%E7%88%AC%E8%99%AB/)
搜索python的的图片如下，共3428篇论文。

![](https://img-blog.csdn.net/20171116143917732)
接下来简单讲述分析的过程，方法都类似，通过DOM树节点分析定位元素。右键浏览器审查元素如下所示，每页包括15篇论文，标签位于<div class="wz_tab">下。
![](https://img-blog.csdn.net/20171116151204695)
点击具体一条内容，如下所示，定位方法如下：
1.标题定位<div class="wz_content">下的<h3>标签，并且可以获取URL；
2.摘要定位<div class="width715">内容；
3.出处定位<span class="year-count">节点下的title，年份通过正则表达式提取数据；
4.下载次数和被引用数定位<span class="count">，提取数字第一个和第二个。
![](https://img-blog.csdn.net/20171116151151549)
接下来直接讲述BeautifulSoup和Selenium两种方式的爬虫。


## 二. BeautifulSoup爬虫
BeautifulSoup完整代码如下：

```python
# -*- coding: utf-8 -*-
import time              
import re
import urllib   
from bs4 import BeautifulSoup
      
    
#主函数
if __name__ == '__main__':
    url = "http://search.cnki.net/Search.aspx?q=python&rank=relevant&cluster=all&val=&p=0"
    content = urllib.urlopen(url).read()
    soup = BeautifulSoup(content,"html.parser")
    #定位论文摘要
    wz_tab = soup.find_all("div",class_="wz_tab")
    num = 0
    for tab in wz_tab:
        #标题
        title = tab.find("h3")
        print title.get_text()
        urls = tab.find("h3").find_all("a")
        #详情超链接
        flag = 0
        for u in urls:
            if flag==0: #只获取第一个URL
                print u.get('href')
                flag += 1
        #摘要
        abstract = tab.find(attrs={"class":"width715"}).get_text()
        print abstract
        #获取其他信息
        other = tab.find(attrs={"class":"year-count"})
        content = other.get_text().split("\n")
        """
            由于无法分割两个空格，如：《怀化学院学报》  2017年 第09期
            故采用获取标题titile内容为出版杂志
            <span title="北方文学(下旬)">《北方文学(下旬)》  2017年 第06期</span>
        """
        #出版杂志+年份
        cb_from = other.find_all("span")
        flag = 0 
        for u in cb_from:
            if flag==0: #获取标题
                print u.get("title")
                flag += 1
        mode = re.compile(r'\d+\.?\d*')
        number = mode.findall(content[0])
        print number[0] #年份
        
        #下载次数 被引次数
        mode = re.compile(r'\d+\.?\d*')
        number = mode.findall(content[1])
        if len(number)==1:
            print number[0]
        elif len(number)==2:
            print number[0], number[1]
        num = num + 1
```
输出如下图所示：
![](https://img-blog.csdn.net/20171117092805637)

但是爬取的URL无法跳转，总是显示登录页面，比如“http://epub.cnki.net/kns/detail/detail.aspx?filename=DZRU2017110705G&dbname=CAPJLAST&dbcode=cjfq”，而能正确显示的是的“http://www.cnki.net/KCMS/detail/detail.aspx?filename=DZRU2017110705G&
dbname=CAPJLAST&dbcode=CJFQ&urlid=&yx=&v=MTc2ODltUm42ajU3VDN
mbHFXTTBDTEw3UjdxZVlPZHVGeTdsVXJ6QUpWZz1JVGZaZbzlDWk81NFl3OU16”。
显示如下图所示：

![](https://img-blog.csdn.net/20171117103318434)

解决方法：这里我准备采用Selenium技术定位超链接，再通过鼠标点击进行跳转，从而去到详情页面获取作者或关键词信息。


## 三. Selenium爬虫
爬取代码如下：

```python
# -*- coding: utf-8 -*-
import time              
import re              
import sys    
import codecs    
import urllib   
from selenium import webdriver            
from selenium.webdriver.common.keys import Keys            
  
      
#主函数
if __name__ == '__main__':
    url = "http://search.cnki.net/Search.aspx?q=python&rank=relevant&cluster=all&val=&p=0"
    driver = webdriver.Firefox()
    driver.get(url)
    #标题
    content = driver.find_elements_by_xpath("//div[@class='wz_content']/h3")
    #摘要
    abstracts = driver.find_elements_by_xpath("//div[@class='width715']")
    #出版杂志+年份
    other = driver.find_elements_by_xpath("//span[@class='year-count']/span[1]")
    mode = re.compile(r'\d+\.?\d*')
    #下载次数 被引次数
    num = driver.find_elements_by_xpath("//span[@class='count']")
    #获取内容
    i = 0
    for tag in content:
        print tag.text
        print abstracts[i].text
        print other[i].get_attribute("title")
        number = mode.findall(other[i].text)
        print number[0] #年份
        number = mode.findall(num[i].text)
        if len(number)==1: #由于存在数字确实 如(100) ()
            print number[0]
        elif len(number)==2:
            print number[0],number[1]
        print ''
        
        i = i + 1
        tag.click()
        time.sleep(1)
```
输出如下所示：
```python
>>> 
网络资源辅助下的Python程序设计教学 
本文对于Python学习网络资源做了归纳分类,说明了每类资源的特点,具体介绍了几个有特色的学习网站,就网络资源辅助下的Python学习进行了讨论,阐释了利用优质网络资源可以提高课堂教学效果,增加教学的生动性、直观性和交互性。同时说明了这些资源的利用能够方便学生的编程训练,使学生有更多的时间和机会动手编程,实现编程教学中...
电子技术与软件工程
2017
11 0
Python虚拟机内存管理的研究 
动态语言的简洁性,易学性缩短了软件开发人员的开发周期,所以深受研发人员的喜爱。其在机器学习、科学计算、Web开发等领域都有广泛的应用。在众多的动态语言中,Python是用户数量较大的动态语言之一。本文主要研究Python对内存资源的管理。Python开发效率高,但是运行效率常为人诟病,主要原因在于一切皆是对象的语言实现...
南京大学
2014
156 0
```
接下来是点击详情页面，窗口转化捕获信息，代码如下：
```python
# -*- coding: utf-8 -*-
import time              
import re              
import sys    
import codecs    
import urllib   
from selenium import webdriver            
from selenium.webdriver.common.keys import Keys            
  
      
#主函数
if __name__ == '__main__':
    url = "http://search.cnki.net/Search.aspx?q=python&rank=relevant&cluster=all&val=&p=0"
    driver = webdriver.Firefox()
    driver.get(url)
    #标题
    content = driver.find_elements_by_xpath("//div[@class='wz_content']/h3")
    #摘要
    abstracts = driver.find_elements_by_xpath("//div[@class='width715']")
    #出版杂志+年份
    other = driver.find_elements_by_xpath("//span[@class='year-count']/span[1]")
    mode = re.compile(r'\d+\.?\d*')
    #下载次数 被引次数
    num = driver.find_elements_by_xpath("//span[@class='count']")
    #获取当前窗口句柄  
    now_handle = driver.current_window_handle
    #获取内容
    i = 0
    for tag in content:
        print tag.text
        print abstracts[i].text
        print other[i].get_attribute("title")
        number = mode.findall(other[i].text)
        print number[0] #年份
        number = mode.findall(num[i].text)
        if len(number)==1: #由于存在数字确实 如(100) ()
            print number[0]
        elif len(number)==2:
            print number[0],number[1]
        print ''
        
        i = i + 1
        tag.click()
        time.sleep(2)
        #跳转 获取所有窗口句柄  
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
                elem_sub = driver.find_element_by_xpath("//div[@class='summary pad10']")  
                print u"作者", elem_sub.text   
                print ''  
  
                #关闭当前窗口  
                driver.close()  
              
        #输出主窗口句柄  
        print now_handle  
        driver.switch_to_window(now_handle) #返回主窗口 开始下一个跳转
```
但部分网站还是出现无法访问的问题，如下所示：
![](https://img-blog.csdn.net/20171117110956798)
最后作者拟爬取万方数据进行分析。
最后希望文章对你有所帮助，如果错误或不足之处，请海涵~
(By:Eastmount 2017-11-17 深夜12点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



