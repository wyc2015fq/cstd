
# [python爬虫] Selenium高级篇之窗口移动、弹出对话框自登录 - 杨秀璋的专栏 - CSDN博客

2017年08月10日 23:14:59[Eastmount](https://me.csdn.net/Eastmount)阅读数：17598


在我们使用Selenium Python制作自动爬虫和网页自动测试的时候，通常会遇到弹出新的窗体或对话框的时候，此时你捕获的窗体已经被打断，从而干扰你的爬虫。
那怎么解决这个问题呢？
本篇文章主要记录两段代码解决这类问题：
第一个是click()函数点击超链接后，需要捕获弹出新窗体的信息，此时需要调用switch_to_window()函数切换窗体，再捕获新的数据；
第二个是click()函数点击超链接后，弹出登录的对话框，比如百度首页登录，此时需要调用switch_to_alert()函数获取该alert对象。
文章主要是结合实际问题进行叙述，内容比较基础，希望对您有所帮助，如果存在错误或不足之处，还请海涵。更多爬虫主题参考我的专栏：[http://blog.csdn.net/column/details/eastmount-spider.html](http://blog.csdn.net/column/details/eastmount-spider.html)

## 一. Switch_to_window函数实现窗体切换
在使用Selenium爬取知识过程，通常会遇到_blank弹出新窗体，或窗体是弹出的那种情况，而且有的需要登录，如：新浪微博、公众号、京东等，使用webdriver.Firefox()重新打开新窗体是无法加载已有信息的。这时候就需要通过获取当前句柄再进行窗口切换。
这里使用该方法实现获取CSDN知识库的关键词的标题及超链接信息，因为它是_blank弹出新窗体，再获取弹出窗体的技术介绍。需要注意每次窗口句柄移动都需要重新定位它们的主窗体。
核心代码：

```python
# -*- coding: utf-8 -*-
from selenium import webdriver  
from selenium.webdriver.common.keys import Keys  
import time
import os
#访问链接
driver = webdriver.Firefox()
url = "http://lib.csdn.net/"
driver.get(url) 
elem_div = driver.find_elements_by_xpath("//ul[@class='list01 clearfix']/li/div[2]/a")
#获取当前窗口句柄
now_handle = driver.current_window_handle
print now_handle
for elem in elem_div:
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
            elem_p = driver.find_element_by_xpath("//div[@class='coltop clearfix']/div[2]")  
            print elem_p.text   
  
            #关闭当前窗口  
            driver.close()  
              
    #输出主窗口句柄  
    print now_handle  
    driver.switch_to_window(now_handle) #返回主窗口 开始下一个跳转
```
步骤如下：
1.首先，current_window_handle获取当前首页窗体并保存；
2.然后调用find_elements_by_xpath()获取首页多个关键词超链接（多个<li>），for循环访问；
3.在循环中点击超链接，弹出新的窗体，循环找到不是首页窗体的，调用switch_to_window进行跳转；
4.爬取新窗体关键词的介绍信息，位于div[@class='coltop clearfix']/div[2]下；
5.关闭当前窗体，重新返回首页窗体的超链接，循环执行3-5步骤。
部分输出如下所示：
```python
主窗体: {3f9dc2dc-d468-4695-9fb8-493b9d1dd245}
Android
http://lib.csdn.net/base/android
{9e7fbd44-b487-479d-8c5a-c5dd387a1b48}
Android知识库 - 知识库 - 你身边的技术百科全书 - CSDN
标准知识图谱
我的知识图谱(27234)
Android是一个基于Linux内核的移动操作系统，由Google成立的Open Handset Alliance（OHA，开放手持设备联盟）持续领导与开发，主要设计用于触控荧幕移动设备如智能手机和平板电脑。
知识节点： 搭建App开...熟悉Android...APP基础软件架构设...App高级开... 更多
{3f9dc2dc-d468-4695-9fb8-493b9d1dd245} React
http://lib.csdn.net/base/react
{82d2e23b-dc70-424c-a2db-bf8710844ca7}
React知识库 - 知识库 - 你身边的技术百科全书 - CSDN
标准知识图谱
我的知识图谱(2012)
React是一个Facebook和Instagram用来创建用户界面的JavaScript库，用于构建“可预期的”和“声明式的”Web用户界面。 该框架的推出主要为了开发随着时间数据不断变化的大规模应用程序。
知识节点： 基础语法插件视图与数据...工程化路由细化 更多
```
CSDN知识库如下图所示，首页显示了多个技术的关注信息及资源数量。
![](https://img-blog.csdn.net/20170810224022687)
审查元素如下图所示，Android技术等均在<ul>标签下，通过多个<li></li>实现，此时获取Android超链接的url，并点击其操作。
![](https://img-blog.csdn.net/20170810224132938)
点击后，会弹出新的窗体，而Selenium却定位了当前首页的窗体，此时就需要利用switch_to_window来切换窗体。
![](https://img-blog.csdn.net/20170810224628121)
审查元素对应的HTML源码如下：
![](https://img-blog.csdn.net/20170810224707567)



## 二. Switch_to_alert函数捕获弹出对话框
**注意：**一定不要尝试该方法登陆百度、新浪、淘宝这些网站后爬取数据，账号可能被永久封号的。
比如百度首页，点击登录界面会弹出对话框如下图所示：

![](https://img-blog.csdn.net/20170810225950871)
![](https://img-blog.csdn.net/20170810230108478)

首先，需要获取“登录”的位置，超链接<a>直接xpath定位总是失败，故作者才采用的<div id='u1'>进行定位，超链接位于第7个超链接。
\#失败
elem_login = driver.find_element_by_xpath("//a[@name='tj_login']")
\#成功
elem_login = driver.find_element_by_xpath("//div[@id='u1']/a[7]")
然后，弹出的对话框采用driver.switch_to_alert()方法选择alert对象。
![](https://img-blog.csdn.net/20170810230625011)

获取用户名和密码后，需要设置sleep暂停，输入验证码，然后自动登录即可，登录后即可爬取相关信息。
核心代码如下：

```python
# -*- coding: utf-8 -*-
from selenium import webdriver  
from selenium.webdriver.common.keys import Keys  
import time
import os
#访问链接
driver = webdriver.Firefox()
url = "https://www.baidu.com/"
driver.get(url) 
#失败
elem_login = driver.find_element_by_xpath("//a[@name='tj_login']")
#成功
elem_login = driver.find_element_by_xpath("//div[@id='u1']/a[7]")
#获取当前窗口句柄
now_handle = driver.current_window_handle
print now_handle
#获取当前登录按钮信息
print elem_login.text                    
print elem_login.get_attribute('href')
elem_login.click()
time.sleep(2)
#选择弹出的对话框
driver.switch_to_alert()
#获取用户名和密码
elem_name = driver.find_element_by_xpath("//input[@name='userName']")
elem_name.send_keys(u"eastmount")
elem_pwd = driver.find_element_by_xpath("//input[@name='password']")
elem_pwd.send_keys("12345678")
time.sleep(10)
elem_pwd.send_keys(Keys.RETURN)
```
登录后如下图所示：
![](https://img-blog.csdn.net/20170810230828822)
![](https://img-blog.csdn.net/20170810230901693)

最后希望这篇文章对你有所帮助，原理和方法更重要，希望你也能灵活应用。继续享受博客和教学吧，同时期待我最近欣赏的某个人吧！杨老师绿幺加油~
不愿清醒，宁愿一直沉迷放纵。
不知归路，宁愿一世无悔追逐。
再苦再累，也笑看庭前花开花落。
(By:Eastmount 2017-08-10 晚上11点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


