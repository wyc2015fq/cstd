# Selenium基础知识(一)环境与搜索 - _天枢 - 博客园
## [Selenium基础知识(一)环境与搜索](https://www.cnblogs.com/yhleng/p/7508481.html)
2017-09-12 09:37 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7508481)
**所需环境：**
1、python2.7 + pycharm
2、Selenium
3、浏览器驱动(**IEDriverServer.exe**)  这里使用的是ie浏览器
　 将驱动放到一个环境变量路径，这里直接放在了python27根目录
4、注意事项
**如果使用ie驱动，需要将ie启用保护模式，去掉所有勾选，4个设置要一至；否则可能会出现，执行脚本地时候可以将浏览器打开，但是不会继续往下执行**
**![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170912093502953-957143353.png)**
安装python2.7 和pycharm这里就不在说了，百度一下很多的下载地址
**一、安装selenium**
我们直接在命令行下安装，前提是安装了pip
```
pip install Selenium
```
安装完成后会有successfuly……等字样
**二、常用找元素方法 **
```
driver.find_element_by_name() #通过name属性
driver.find_element(By.ID,"kw") #通过By，可以各种找，万能找 
driver.find_element_by_class_name() #通过class_name
driver.find_element_by_id() #通过id
driver.find_element_by_css_selector() #css样式表
driver.find_elements_by_link_text() #找link文本内容
driver.find_element_by_tag_name() #找标签名<input>等
driver.find_element_by_partial_link_text()#通过link部分匹配
driver.find_element_by_xpath() #元素，没有唯一属性，可以用xpath找父找子等层层的查找定位
```
**三、第一个Selenium代码，百度搜索为例**
```
#*_*coding:utf-8*_*
from selenium import webdriver
import time
driver = webdriver.Ie()
driver.get('https://www.baidu.com/')
#输入selenium 搜索
driver.find_element_by_id('kw').send_keys('selenium')
driver.find_element_by_id('su').click()
driver.quit()
```
上图：打开百度，输入selenium点击“百度一下，你就知道”按钮
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170912092308766-552447760.png)
