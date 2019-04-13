
# [Python爬虫] Selenium爬取新浪微博移动端热点话题及评论 (下) - 杨秀璋的专栏 - CSDN博客

2016年05月06日 04:30:29[Eastmount](https://me.csdn.net/Eastmount)阅读数：15961所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)



这篇文章主要讲述了使用python+selenium爬取新浪微博的热点话题和评论信息。其中使用该爬虫的缺点是效率极低，傻瓜式的爬虫，不能并行执行等，但是它的优点是采用分析DOM树结构分析网页源码并进行信息爬取，同时它可以通过浏览器进行爬取中间过程的演示及验证码的输入。这篇文章对爬虫的详细过程就不再论述了，主要是提供可运行的代码和运行截图即可。希望文章对你有所帮助吧~
**参考文章**
[[python爬虫] Selenium爬取新浪微博内容及用户信息](http://blog.csdn.net/eastmount/article/details/50720436)
[[Python爬虫] Selenium爬取新浪微博客户端用户信息、热点话题及评论 (上)](http://blog.csdn.net/eastmount/article/details/51231852)
[[Python爬虫] 在Windows下安装PIP+Phantomjs+Selenium](http://blog.csdn.net/eastmount/article/details/47785123)
[[Python爬虫] Selenium实现自动登录163邮箱和Locating Elements介绍](http://blog.csdn.net/eastmount/article/details/47825633)
[http://selenium-python.readthedocs.org/locating-elements.html](http://selenium-python.readthedocs.org/locating-elements.html)
**实现过程**
运行截图如下所示，它通过调用Firefox浏览器，然后输入移动端网址自动登陆新浪微博，它会自动输入用户名及密码，但需要用户在我设置暂停20秒内输入验证码。登陆后它会自动跳转到微博主题搜索页面，当用户输入关键词"欢乐颂"后，就会对返回的微博信息及评论进行爬取了，爬取过程中需要注意翻页即可。
![](https://img-blog.csdn.net/20160427095811268)

**运行结果**
运行结果如下图所示：两个文件，一个是微博内容评论对应的URL，一个是爬取URL中的评论信息。
![](https://img-blog.csdn.net/20160506040922694)
![](https://img-blog.csdn.net/20160506040939663)

**源代码**
源代码如下：
```python
# coding=utf-8
"""  
Created on 2016-04-24 @author: Eastmount
功能: 爬取新浪微博用户的信息及微博评论
网址：http://weibo.cn/ 数据量更小 相对http://weibo.com/
正确的方法是把所有的URL都获取再依次访问
"""    
import time            
import re            
import os    
import sys  
import codecs  
import shutil
import urllib 
from selenium import webdriver        
from selenium.webdriver.common.keys import Keys        
import selenium.webdriver.support.ui as ui        
from selenium.webdriver.common.action_chains import ActionChains

#先调用无界面浏览器PhantomJS或Firefox    
#driver = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")    
driver = webdriver.Firefox()
wait = ui.WebDriverWait(driver,10)

#全局变量 文件操作读写信息
inforead = codecs.open("SinaWeibo_List_best_1.txt", 'r', 'utf-8')
infofile = codecs.open("SinaWeibo_Info_best_1.txt", 'w', 'utf-8')

#********************************************************************************
#                            第一步: 登陆weibo.cn 
#        该方法针对weibo.cn有效(明文形式传输数据) weibo.com见学弟设置POST和Header方法
#                LoginWeibo(username, password) 参数用户名 密码
#********************************************************************************
def LoginWeibo(username, password):
    try:
        #输入用户名/密码登录
        print u'准备登陆Weibo.cn网站...'
        driver.get("http://login.weibo.cn/login/")
        elem_user = driver.find_element_by_name("mobile") 
        elem_user.send_keys(username) #用户名
        elem_pwd = driver.find_element_by_xpath("/html/body/div[2]/form/div/input[2]")
        elem_pwd.send_keys(password)  #密码 name=password_6785
        #elem_rem = driver.find_element_by_name("remember")
        #elem_rem.click()             #记住登录状态
        #重点:暂停时间输入验证码(http://login.weibo.cn/login/ 手机端需要)
        time.sleep(20)
        #点击submit按钮登陆方式或输入回车键登陆方式
        #elem_sub = driver.find_element_by_name("submit")
        #elem_sub.click()              
        elem_pwd.send_keys(Keys.RETURN)
        time.sleep(2)
        
        #获取Coockie 
        print driver.current_url
        print driver.get_cookies()  #获得cookie信息 dict存储
        print u'输出Cookie键值对信息:'
        for cookie in driver.get_cookies(): 
            #print cookie
            for key in cookie:
                print key, cookie[key]
                    
        #driver.get_cookies()类型list 仅包含一个元素cookie类型dict
        print u'登陆成功...'
        
        
    except Exception,e:      
        print "Error: ",e
    finally:    
        print u'End LoginWeibo!\n\n'

#**********************************************************************************************
#                  第二步: 访问个人页面http://weibo.cn/5824697471并获取信息
#                                VisitPersonPage()
#        编码常见错误 UnicodeEncodeError: 'ascii' codec can't encode characters文件utf-8编码
#**********************************************************************************************
def VisitPersonPage(user_id):
    try:
        global infofile       #全局文件变量
        url = "http://weibo.com/" + user_id
        driver.get(url)
        print u'准备访问个人网站.....', url
        print u'个人详细信息'
        
        #用户id
        print u'用户id: ' + user_id
        #昵称 关注数 粉丝数 微博数 个人资料其它信息
        #URL http://weibo.cn/5824697471/follow   
        
        
    except Exception,e:      
        print "Error: ",e
    finally:    
        print u'VisitPersonPage!\n\n'
        
#********************************************************************************
#                  第三步: 访问http://weibo.cn/search/ (手机端) 页面搜索热点信息
#                         爬取微博信息及评论，注意评论翻页的效果和微博的数量
#********************************************************************************    
def GetComment(key):
    try:
        global infofile       #全局文件变量
        driver.get("http://weibo.cn/search/")
        print u'搜索热点主题关键词：', key
        #输入主题并点击搜索
        item_inp = driver.find_element_by_xpath("//div[@class='c']/form/div/input") #name=keyword
        item_inp.send_keys(key)
        item_inp.send_keys(Keys.RETURN)    #采用点击回车直接搜索

        #内容
        #content = driver.find_elements_by_xpath("//div[@class='content clearfix']/div/p")
        comment = driver.find_elements_by_xpath("//a[@class='cc']")
        content = driver.find_elements_by_xpath("//div[@class='c']")
        print content
        all_comment_url = []               #存储所有文件URL
        i = 0
        j = 0
        infofile.write(u'开始:\r\n')
        print u'长度', len(content)
        while i<len(content):
            #print content[i].text
            if (u'收藏' in content[i].text) and (u'评论' in content[i].text): #过滤其他标签
                print content[i].text
                infofile.write(u'微博信息:\r\n')
                infofile.write(content[i].text + '\r\n')
                div_id = content[i].get_attribute("id")
                print div_id
                while(1):  #存在其他包含class=cc 如“原文评论”
                    url_com = comment[j].get_attribute("href")
                    if ('comment' in url_com) and ('uid' in url_com):
                        print url_com
                        infofile.write(u'评论信息:\r\n')
                        infofile.write(url_com+'\r\n')
                        all_comment_url.append(url_com)    #保存到变量里
                        j = j + 1
                        break
                    else:
                        j = j + 1
                    
            i = i + 1
        #http://weibo.cn/search/?pos=search
        print driver.current_url
        
        #python中文转换url编码 urllib.quote(key) urllib.unquote转回来
        #转码失败
        #http://weibo.cn/search/mblog?hideSearchFrame=&keyword=欢乐颂&page=2
        #url = "http://weibo.cn/search/mblog?hideSearchFrame=&keyword="+ key_url + "&page=2"

        #获取10个下页
        N = 2
        while N<=10:
            #后面采用换页 第一次为方便给大家解决方法就采用获取搜索框id回车访问
            url_get = driver.find_element_by_xpath("//div[@id='pagelist']/form/div/a")
            url = url_get.get_attribute("href") 
            print url #获取下页
            driver.get(url)
            comment = driver.find_elements_by_xpath("//a[@class='cc']")
            content = driver.find_elements_by_xpath("//div[@class='c']")
            print content
            i = 0
            j = 0                        #第一个<a class='cc' href>是多余的
            print u'长度', len(content)
            infofile.write(u'\r\n下页:\r\n')
            while i<len(content):
                #print content[i].text
                if (u'收藏' in content[i].text) and (u'评论' in content[i].text):
                    print content[i].text
                    infofile.write(u'微博信息:\r\n')
                    infofile.write(content[i].text + '\r\n')
                    #获取该信息id值 通过id获取评论超链接
                    #先获取:<div id="M_Du3npzqSd" class="c"> 
                    #再获取:<a class="cc" href="http://weibo.cn/comment/#cmtfrm"></a>
                    div_id = content[i].get_attribute("id")
                    print div_id
                    '''
                    url = driver.find_elements_by_xpath("//div[@id=" + div_id + "]/a")
                    print url
                    for u in url:
                        print u.get_attribute("href")
                    '''
                    while(1):  #存在其他包含class=cc 如“原文评论”
                        url_com = comment[j].get_attribute("href")
                        if ('comment' in url_com) and ('uid' in url_com):
                            print url_com
                            infofile.write(u'评论信息:\r\n')
                            infofile.write(url_com + '\r\n')
                            all_comment_url.append(url_com)
                            j = j + 1
                            break
                        else:
                            j = j + 1
                i = i + 1
            N = N + 1
        else:
            print u'结束爬取评论URL 对齐while循环'
            
        #方位评论URL并进行爬取
        print u'\n\n评论'
        infocomment = codecs.open("SinaWeibo_Info_best_2.txt", 'w', 'utf-8')
        for url in all_comment_url:
            print url
            driver.get(url)
            #driver.refresh()
            time.sleep(2)
            infocomment.write(url+'\r\n')
            test = driver.find_elements_by_class_name('c')
            print len(test)
            #Error:  Message: Element not found in the cache -
            #perhaps the page has changed since it was looked up
            #http://www.51testing.com/html/21/n-862721-2.html
            #异常的说明已经很明显了：在cache中找不到元素，在元素被找到之后页面变换了。
            #这就说明，当当前页面发生跳转之后，存在cache中的关于这个页面的元素也被清空了。
            k = 0
            while k<len(test): 
                print test[k].text
                infocomment.write(test[k].text + '\r\n')
                k = k + 1
            infocomment.write('\r\n')
        infocomment.close()
                                      

    except Exception,e:      
        print "Error: ",e
    finally:    
        print u'VisitPersonPage!\n\n'
        print '**********************************************\n'
        
        
#*******************************************************************************
#                                程序入口 预先调用
#         注意: 因为sina微博增加了验证码,但是你用Firefox登陆输入验证码
#         直接跳转到明星微博那部分,即: http://weibo.cn/guangxianliuyan
#*******************************************************************************
    
if __name__ == '__main__':
    #定义变量
    username = '15201615157'             #输入你的用户名
    password = '*********'               #输入你的密码
    #操作函数
    LoginWeibo(username, password)       #登陆微博
    #在if __name__ == '__main__':引用全局变量不需要定义 global inforead 省略即可
    print 'Read file:'
    user_id = inforead.readline()
    while user_id!="":
        user_id = user_id.rstrip('\r\n')
        print user_id
        VisitPersonPage(user_id)         #访问个人页面http://weibo.cn/guangxianliuyan
        user_id = inforead.readline()
        #break
    #搜索热点微博 爬取评论
    key = u'欢乐颂' 
    GetComment(key)
    
    infofile.close()
    inforead.close()
```


**登陆部分核心代码**
其中登陆部分的核心代码及对应DOM树结构分析如下：
\#调用Firefox浏览器
driver =  webdriver.Firefox()
driver.get("http://login.weibo.cn/login/")
\#用户名
elem_user = driver.find_element_by_name("mobile")
elem_user.send_keys(username)
\#密码 name=password_6785
elem_pwd = driver.find_element_by_xpath("/html/body/div[2]/form/div/input[2]")
elem_pwd.send_keys(password)
\#记住登录状态
elem_rem = driver.find_element_by_name("remember")
elem_rem.click()
\#重点:暂停时间输入验证码(http://login.weibo.cn/login/ 手机端需要)
time.sleep(20)
\#点击submit按钮登陆方式或输入回车键登陆方式
elem_sub = driver.find_element_by_name("submit")
elem_sub.click()
elem_pwd.send_keys(Keys.RETURN)
![](https://img-blog.csdn.net/20160427100536661)

**为什么需要从移动端登陆呢？**
新浪微博有两个入口：
新浪微博登录常用接口：[http://login.sina.com.cn/](http://login.sina.com.cn/)
对应主界面：[http://weibo.com/](http://weibo.com/)
但是个人建议采用手机端微博入口：[http://login.weibo.cn/login/](http://login.weibo.cn/login/)
对应主界面：[http://weibo.cn/](http://weibo.cn/)
因为在客户端登陆爬取微博评论信息时，它总是通过JavaScript动态加载的，需要点击按钮才能加载，单纯的获取评论的节点或使用正则表达式爬取评论部分的HTML源码，都是空值，所以采用移动端进行爬取。
它们的主要区别是移动端数据相对更简练，但是内容都基本一样，只是图片较小、关注粉丝数只能显示20页、个人信息缺失些等，供手机用户使用，但是信息是和客户端对应的。
如下图所示，它表示动态加载的评论，可以看到"href=javascript:void(0)"还有一些script函数实现的动态加载。
![](https://img-blog.csdn.net/20160427101320039)

**跳转页面下一页**
常用的方法，如我前面的文章讲解爬取虎扑图片、生物预料中，都是通过分析URL的&构成进行的。新浪微博同样也是：
http://weibo.cn/search/mblog?hideSearchFrame=&keyword=欢乐颂&page=2
其中搜索关键词"欢乐颂"，只需要修改page页面即可。但是"欢乐颂"中文字符转换为URL编码时总是报错，python使用中文转换url编码方法：urllib.quote(key)。
所以采用了第二种方法，获取下一页的URL，再进行driver.get(url)访问，循环N从2到10。
\#获取下页
url_get = driver.find_element_by_xpath("//div[@id='pagelist']/form/div/a")
url = url_get.get_attribute("href")
driver.get(url)
\#评论URL
comment = driver.find_elements_by_xpath("//a[@class='cc']")
\#微博内容 需过滤其它class=c
content = driver.find_elements_by_xpath("//div[@class='c']")

**重点获取URL队列依次爬取评论**
由于新浪微博需要模拟登陆，所以获取微博的同时获取了评论的URL，但是这里使用的浏览器driver不能同时访问评论信息，因为它在循环中driver还在使用。换句话就是find_elements_by_xpath是获取多个值，当然你可以再开一个driver2，但也需要登陆，再driver2.get(url_comment)即可。
但常用的爬虫通常是有一个URL队列的概念，把爬取的评论URL都存储在一个队列或数组中。该程序循环先N从2到10，爬取10页的微博信息及评论URL，再依次爬取评论信息，如果获得了URL，也能爬取微博内容的。
**关键：使用数组存储所有评论的URL队列，再一次driver.get(url)爬取所有微博及评论。**
\#方位评论URL并进行爬取
print u'\n\n评论'
infocomment = codecs.open("SinaWeibo_Info_best_2.txt", 'w', 'utf-8')
for url in all_comment_url:
print url
driver.get(url)
\#driver.refresh()
time.sleep(2)
infocomment.write(url+'\r\n')
test = driver.find_elements_by_class_name('c')
print len(test)
k = 0
while k<len(test):
print test[k].text
infocomment.write(test[k].text + '\r\n')
k = k + 1
infocomment.write('\r\n')
infocomment.close()

**错误Element not found in the cache**
在爬取过程中可能会遇到错误：Error:  Message: Element not found in the cache - perhaps the page has changed since it was looked up
参考：[http://www.51testing.com/html/21/n-862721-2.html](http://www.51testing.com/html/21/n-862721-2.html)
异常的说明解释：在cache中找不到元素，在元素被找到之后页面变换了。这就说明，当当前页面发生跳转之后，存在cache中的关于这个页面的元素也被清空了。
通过实验发现，它是使用driver.get(url)访问评论时，太快没有加载页面获取元素，采用的方法是time.sleep(2)显示2秒。希望有更好的解决方法~当然时间设置短点也行。

**热门话题置顶**
通过比较客户端和移动端的信息，会发现搜索微博热点，它在客户端首先显示的是最近关于该话题非常热门的微博，如"欢乐颂"蒋欣的状态等。
![](https://img-blog.csdn.net/20160427101919115)

**比较移动端和客户端微博信息**
这是比较移动到和客户端的微博信息，信息基本是一致的，可能只是因为排序显示问题可能序号稍微微调，同时客户端也采用了热门话题置顶显示的一些推荐相关算法吧！如"一个五点之前就醒了的人....."
![](https://img-blog.csdn.net/20160427102719798)
![](https://img-blog.csdn.net/20160427102844717)

然后下面是具体的评论信息，可以发现也是对应的，但是通过手机端就可以显示评论信息，可以进行爬取，而客户端爬取结果为空。
![](https://img-blog.csdn.net/20160427103030708)

![](https://img-blog.csdn.net/20160427103053718)

PS：最后希望文章对你有所帮助！其实方法很简单，希望你能理解这种思想，如何分析HTML源码及DOM树结构，然后动态获取自己需要的信息。
(By:Eastmount 2016-05-06 深夜4点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



