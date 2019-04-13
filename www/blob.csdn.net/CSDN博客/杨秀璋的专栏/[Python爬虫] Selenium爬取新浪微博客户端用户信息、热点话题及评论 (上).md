
# [Python爬虫] Selenium爬取新浪微博客户端用户信息、热点话题及评论 (上) - 杨秀璋的专栏 - CSDN博客

2016年04月24日 07:29:34[Eastmount](https://me.csdn.net/Eastmount)阅读数：18733标签：[python爬虫																](https://so.csdn.net/so/search/s.do?q=python爬虫&t=blog)[selenium																](https://so.csdn.net/so/search/s.do?q=selenium&t=blog)[新浪微博																](https://so.csdn.net/so/search/s.do?q=新浪微博&t=blog)[评论																](https://so.csdn.net/so/search/s.do?q=评论&t=blog)[爬虫																](https://so.csdn.net/so/search/s.do?q=爬虫&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=评论&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=新浪微博&t=blog)个人分类：[Python爬虫																](https://blog.csdn.net/Eastmount/article/category/5758691)
[
																					](https://so.csdn.net/so/search/s.do?q=新浪微博&t=blog)所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)[
							](https://so.csdn.net/so/search/s.do?q=新浪微博&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=selenium&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=python爬虫&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=python爬虫&t=blog)


## 一. 文章介绍
源码下载地址：[http://download.csdn.net/detail/eastmount/9501273](http://download.csdn.net/detail/eastmount/9501273)
前一篇文章"[[python爬虫] Selenium爬取新浪微博内容及用户信息](http://blog.csdn.net/eastmount/article/details/50720436)"简单讲述了如何爬取新浪微博手机端用户信息和微博信息。
用户信息：包括用户ID、用户名、微博数、粉丝数、关注数等。
微博信息：包括转发或原创、点赞数、转发数、评论数、发布时间、微博内容等。
![](https://img-blog.csdn.net/20160223031918122)
它主要通过从文本txt中读取用户id，通过"URL+用户ID" 访问个人网站，如柳岩：
[http://weibo.cn/guangxianliuya](http://weibo.cn/guangxianliuyan)
因为手机端数据相对精简简单，所以采用输入用户的形式依次爬取各个明星的信息。
而这篇文章主要爬取客户端的微博信息，相对信息更多；同时登录微博后在输入框中搜索热点话题，然后依次爬取微博信息和对应的评论。这篇文章的输出如下图所示：
![](https://img-blog.csdn.net/20160424072418375)
PS：注意这篇文章爬取微博内容和评论的时候，由于它是动态加载的，故爬取失败，但思考可以参考。后面下篇会进行解决，如果实在不行只能爬取手机端的信息了。

## 二. 核心代码
这篇文章打算先给出完整代码，再进行讲解的方法：
1.LoginWeibo(username, password) 登录微博，自动输入用户名和密码
2.VisitPersonPage(user_id) 访问跟人网站，获取个人信息，通过如下网址访问柳岩：
http://weibo.cn/guangxianliuyan
3.GetComment(key) 获取微博信息及评论信息，获取输入框按钮进行搜索
获取微博内容评论是注意翻页功能

```python
# coding=utf-8
"""  
Created on 2016-04-24 @author: Eastmount
功能: 爬取新浪微博用户的信息及微博评论
网址：http://weibo.cn/ 数据量更小 相对http://weibo.com/
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
infofile = codecs.open("SinaWeibo_Info_best_1.txt", 'a', 'utf-8')
#********************************************************************************
#                            第一步: 登陆weibo.cn 
#        该方法针对weibo.cn有效(明文形式传输数据) weibo.com见学弟设置POST和Header方法
#                LoginWeibo(username, password) 参数用户名 密码
#********************************************************************************
def LoginWeibo(username, password):
    try:
        #输入用户名/密码登录
        print u'准备登陆Weibo.cn网站...'
        driver.get("http://login.sina.com.cn/")
        elem_user = driver.find_element_by_name("username")
        elem_user.send_keys(username) #用户名
        elem_pwd = driver.find_element_by_name("password")
        elem_pwd.send_keys(password)  #密码
        #elem_rem = driver.find_element_by_name("safe_login")
        #elem_rem.click()             #安全登录
        #重点: 暂停时间输入验证码(http://login.weibo.cn/login/ 手机端需要)
        time.sleep(20)
        
        #elem_sub = driver.find_element_by_xpath("//input[@class='smb_btn']")
        #elem_sub.click()              #点击登陆 因无name属性
        #如果登陆按钮采用动态加载 则采用输入回车键登陆微博
        elem_pwd.send_keys(Keys.RETURN)
        time.sleep(2)
        
        #获取Coockie 推荐资料：http://www.cnblogs.com/fnng/p/3269450.html
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

#********************************************************************************
#                  第二步: 访问个人页面http://weibo.cn/5824697471并获取信息
#                                VisitPersonPage()
#        编码常见错误 UnicodeEncodeError: 'ascii' codec can't encode characters 
#********************************************************************************
def VisitPersonPage(user_id):
    try:
        global infofile       #全局文件变量
        url = "http://weibo.com/" + user_id
        driver.get(url)
        print u'准备访问个人网站.....', url
        print u'个人详细信息'
        
        #用户id
        print u'用户id: ' + user_id
        #昵称
        str_name = driver.find_element_by_xpath("//div[@class='pf_username']/h1")
        name = str_name.text        #str_name.text是unicode编码类型
        print u'昵称: ', name
        
        #关注数 粉丝数 微博数 <td class='S_line1'>
        str_elem = driver.find_elements_by_xpath("//table[@class='tb_counter']/tbody/tr/td/a")
        str_gz = str_elem[0].text    #关注数
        num_gz = re.findall(r'(\w*[0-9]+)\w*', str_gz)
        str_fs = str_elem[1].text    #粉丝数
        num_fs = re.findall(r'(\w*[0-9]+)\w*', str_fs)
        str_wb = str_elem[2].text    #微博数
        num_wb = re.findall(r'(\w*[0-9]+)\w*', str_wb)
        print u'关注数: ', num_gz[0]
        print u'粉丝数: ', num_fs[0]
        print u'微博数: ', num_wb[0]
        #文件操作写入信息
        infofile.write('=====================================================================\r\n')
        infofile.write(u'用户: ' + user_id + '\r\n')
        infofile.write(u'昵称: ' + name + '\r\n')
        infofile.write(u'关注数: ' + str(num_gz[0]) + '\r\n')
        infofile.write(u'粉丝数: ' + str(num_fs[0]) + '\r\n')
        infofile.write(u'微博数: ' + str(num_wb[0]) + '\r\n')
        
        
    except Exception,e:      
        print "Error: ",e
    finally:    
        print u'VisitPersonPage!\n\n'
        print '**********************************************\n'
        infofile.write('=====================================================================\r\n\r\n')

#********************************************************************************
#                  第三步: 访问http://s.weibo.com/页面搜索热点信息
#                  爬取微博信息及评论，注意评论翻页的效果和微博的数量
#********************************************************************************    
def GetComment(key):
    try:
        global infofile       #全局文件变量
        driver.get("http://s.weibo.com/")
        print u'搜索热点主题：', key
        #输入主题并点击搜索
        item_inp = driver.find_element_by_xpath("//input[@class='searchInp_form']")
        item_inp.send_keys(key)
        item_inp.send_keys(Keys.RETURN)    #采用点击回车直接搜索
        #内容
        #content = driver.find_elements_by_xpath("//div[@class='content clearfix']/div/p")
        content = driver.find_elements_by_xpath("//p[@class='comment_txt']")
        print content
        i = 0
        print u'长度', len(content)
        while i<len(content):
            print '微博信息:'
            print content[i].text
            infofile.write(u'微博信息:\r\n')
            infofile.write(content[i].text + '\r\n')
            i = i + 1
        #评论 由于评论是动态加载，爬取失败
        #Error:  list index out of range
        comment = driver.find_elements_by_xpath("//p[@class='list_ul']/dl/dd/div[0]")
        j = 0
        while j<10:
            print comment[j].text
            j = j + 1

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
    username = '1520161****'             #输入你的用户名
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
PS：后面是具体的实现过程分析讲解，如果你只需要代码，上面就是所有完整代码，但建议也看看后面的分析过程，虽然是傻瓜式爬虫，但至少能用，而且方法类似。


## 三. 登录入口
新浪微博登录常用接口：[http://login.sina.com.cn/](http://login.sina.com.cn/)
对应主界面：[http://weibo.com/](http://weibo.com/)
但是个人建议采用手机端微博入口：[http://login.weibo.cn/login/](http://login.weibo.cn/login/)
对应主界面：[http://weibo.cn/](http://weibo.cn/)
通过比较下面两张图，分别是PC端和手机端，可以发现内容基本一致：
![](https://img-blog.csdn.net/20160223041531679)
手机端下图所示，其中图片相对更小，同时内容更精简。

![](https://img-blog.csdn.net/20160223042016263)



## 四. 分析-登录微博
## LoginWeibo

登录过程如下图所示，先通过函数获取用户名、密码、登录按钮结点，然后再自动输入信息并登录。如果需要输入验证码，也可以在手动输入。
![](https://img-blog.csdn.net/20160424045252188)
对应源码：
```python
#********************************************************************************
#                            第一步: 登陆weibo.cn 
#        该方法针对weibo.cn有效(明文形式传输数据) weibo.com见学弟设置POST和Header方法
#                LoginWeibo(username, password) 参数用户名 密码
#********************************************************************************
def LoginWeibo(username, password):
    try:
        #输入用户名/密码登录
        print u'准备登陆Weibo.cn网站...'
        driver.get("http://login.sina.com.cn/")
        elem_user = driver.find_element_by_name("username")
        elem_user.send_keys(username) #用户名
        elem_pwd = driver.find_element_by_name("password")
        elem_pwd.send_keys(password)  #密码
        #elem_rem = driver.find_element_by_name("safe_login")
        #elem_rem.click()             #安全登录
        #重点: 暂停时间输入验证码(http://login.weibo.cn/login/ 手机端需要)
        time.sleep(20)
        
        elem_sub = driver.find_element_by_xpath("//input[@class='smb_btn']")
        elem_sub.click()              #点击登陆 因无name属性
        time.sleep(2)
        
        #获取Coockie 推荐资料：http://www.cnblogs.com/fnng/p/3269450.html
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
```
分析网页结点如下图所示：
![](https://img-blog.csdn.net/20160424045653678)
核心代码：
elem_user = driver.find_element_by_name("username")
elem_user.send_keys(username)     \#用户名
elem_pwd = driver.find_element_by_name("password")
elem_pwd.send_keys(password)      \#密码
elem_sub = driver.find_element_by_xpath("//input[@class='smb_btn']")
elem_sub.click()                               \#点击登陆
登录后跳转到下面页面：
![](https://img-blog.csdn.net/20160424045928196)


## 五. 分析-爬取用户个人信息
## VisitPersonPage
通过URL+用户ID的形式访问信息，访问页面如下图所示：
![](https://img-blog.csdn.net/20160424054922826)
代码如下所示：
```python
#********************************************************************************
#                  第二步: 访问个人页面http://weibo.cn/5824697471并获取信息
#                                VisitPersonPage()
#        编码常见错误 UnicodeEncodeError: 'ascii' codec can't encode characters 
#********************************************************************************
def VisitPersonPage(user_id):
    try:
        global infofile       #全局文件变量
        url = "http://weibo.com/" + user_id
        driver.get(url)
        print u'准备访问个人网站.....', url
        print u'个人详细信息'
        
        #用户id
        print u'用户id: ' + user_id
        #昵称
        str_name = driver.find_element_by_xpath("//div[@class='pf_username']/h1")
        name = str_name.text        #str_name.text是unicode编码类型
        print u'昵称: ', name
        
        #关注数 粉丝数 微博数 <td class='S_line1'>
        str_elem = driver.find_elements_by_xpath("//table[@class='tb_counter']/tbody/tr/td/a")
        str_gz = str_elem[0].text    #关注数
        num_gz = re.findall(r'(\w*[0-9]+)\w*', str_gz)
        str_fs = str_elem[1].text    #粉丝数
        num_fs = re.findall(r'(\w*[0-9]+)\w*', str_fs)
        str_wb = str_elem[2].text    #微博数
        num_wb = re.findall(r'(\w*[0-9]+)\w*', str_wb)
        print u'关注数: ', num_gz[0]
        print u'粉丝数: ', num_fs[0]
        print u'微博数: ', num_wb[0]
        #文件操作写入信息
        infofile.write('=====================================================================\r\n')
        infofile.write(u'用户: ' + user_id + '\r\n')
        infofile.write(u'昵称: ' + name + '\r\n')
        infofile.write(u'关注数: ' + str(num_gz[0]) + '\r\n')
        infofile.write(u'粉丝数: ' + str(num_fs[0]) + '\r\n')
        infofile.write(u'微博数: ' + str(num_wb[0]) + '\r\n')
        
        
    except Exception,e:      
        print "Error: ",e
    finally:    
        print u'VisitPersonPage!\n\n'
        print '**********************************************\n'
```
其中SinaWeibo_List_best_1.txt中仅包含两个用户id的情况：
![](https://img-blog.csdn.net/20160424054500090)
该部分输出如下图所示：
![](https://img-blog.csdn.net/20160424054558262)

分析页面DOM树结构如下图所示：
![](https://img-blog.csdn.net/20160424055439897)

同时这里只获取简单的信息，详细信息还可以自动点击"查看更多"进行获取：
![](https://img-blog.csdn.net/20160424055605735)

## 六. 分析-爬取微博和评论信息GetComment
该部分代码如下：
```python
#********************************************************************************
#                  第三步: 访问http://s.weibo.com/页面搜索热点信息
#                  爬取微博信息及评论，注意评论翻页的效果和微博的数量
#********************************************************************************    
def GetComment(key):
    try:
        global infofile       #全局文件变量
        driver.get("http://s.weibo.com/")
        print u'搜索热点主题：', key
        #输入主题并点击搜索
        item_inp = driver.find_element_by_xpath("//input[@class='searchInp_form']")
        item_inp.send_keys(key)
        item_inp.send_keys(Keys.RETURN)    #采用点击回车直接搜索
        #内容
        #content = driver.find_elements_by_xpath("//div[@class='content clearfix']/div/p")
        content = driver.find_elements_by_xpath("//p[@class='comment_txt']")
        print content
        i = 0
        print u'长度', len(content)
        while i<len(content):
            print '微博信息:'
            print content[i].text
            infofile.write(u'微博信息:\r\n')
            infofile.write(content[i].text + '\r\n')
            i = i + 1
        #评论 由于评论是动态加载，爬取失败
        #Error:  list index out of range
        comment = driver.find_elements_by_xpath("//p[@class='list_ul']/dl/dd/div[0]")
        j = 0
        while j<10:
            print comment[j].text
            j = j + 1

    except Exception,e:      
        print "Error: ",e
    finally:    
        print u'VisitPersonPage!\n\n'
        print '**********************************************\n'
```
通过访问该URL进行热点搜索：[http://s.weibo.com/](http://s.weibo.com/)
![](https://img-blog.csdn.net/20160424061617899)

再通过核定代码输入主题如“欢乐颂”并点击回车键，分析节点方法与前面类似：
item_inp = driver.find_element_by_xpath("//input[@class='searchInp_form']")
item_inp.send_keys(key)
item_inp.send_keys(Keys.RETURN)    \#采用点击回车直接搜索
自动返回搜索结果如下图所示：
![](https://img-blog.csdn.net/20160424061905291)
分析DOM树结构如下，右键浏览器"审查元素"：
![](https://img-blog.csdn.net/20160424062557080)
分析具体的信息如下所示：
![](https://img-blog.csdn.net/20160424062919718)
但爬取博客过程中，总显示空值，不知道为什么，怀疑是动态加载的。
content = driver.find_elements_by_xpath("//div[@class='content clearfix']/div/p")
content = driver.find_elements_by_xpath("//p[@class='comment_txt']")
评论信息需要点击"评论1897"才能进行加载：
![](https://img-blog.csdn.net/20160424070716123)
对应源码如下所示，它是动态进行加载的：
![](https://img-blog.csdn.net/20160424071051500)
![](https://img-blog.csdn.net/20160424071139609)

如图，审查元素点击"评论"可以发现它是通过JavaScript加载，这就比较头疼了。
![](https://img-blog.csdn.net/20160424071334314)

PS：最后希望文章对你有所帮助！其实方法很简单，希望你能理解这种思想，如何分析HTML源码及DOM树结构，然后动态获取自己需要的信息。
关于如何动态爬取评论部分我还在研究当中，实在不行可能只能通过手机端进行爬取了。同时因为最近太忙，只能写写这种效率很低的傻瓜式爬虫，后面毕业了会深入研究爬虫知识。但至少代码能运行，可以爬取信息，当前阶段就非常不错了。不喜勿喷，加油~
**记录一段可运行代码，帮娜姐爬取数据使用，注意需要****手动****填写验证码：**
```python
# coding=utf-8
"""  
Created on 2016-05-23 @author: Eastmount
功能: 爬取新浪微博用户的信息
信息：用户ID 用户名 注册时间 性别 地址(城市) 是否认证 用户标签(明星、搞笑等信息)
    个人资料完成度 粉丝数 关注数 微博数 粉丝ID列表 关注人ID列表 特别关注列表
网址：http://weibo.cn/ 数据量更小 相对http://weibo.com/
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

'''
版本过低
pip install -U selenium
WebDriverException: Message: Can't load the profile.
Profile Dir: %s If you specified a log_file in the FirefoxBinary constructor,
check it for details.
'''
#先调用无界面浏览器PhantomJS或Firefox    
#driver = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")    
driver = webdriver.Firefox()
#options = webdriver.ChromeOptions()
#options.add_experimental_option("excludeSwitches", ["ignore-certificate-errors"])
#driver = webdriver.Chrome(chrome_options=options)
wait = ui.WebDriverWait(driver,10)

#全局变量 文件操作读写信息
inforead = codecs.open("SinaWeibo_List_best_1.txt", 'r', 'utf-8')
infofile = codecs.open("SinaWeibo_Info_best_1.txt", 'a', 'utf-8')

#********************************************************************************
#                  第一步: 登陆weibo.cn 获取新浪微博的cookie
#        该方法针对weibo.cn有效(明文形式传输数据) weibo.com见学弟设置POST和Header方法
#                LoginWeibo(username, password) 参数用户名 密码
#        https://www.zhihu.com/question/21451510
#        http://www.cnblogs.com/fnng/p/3606934.html
#                             验证码暂停时间手动输入
#********************************************************************************
def LoginWeibo(username, password):
    try:
        #**********************************************************************
        # 直接访问driver.get("http://weibo.cn/5824697471")会跳转到登陆页面 用户id
        #
        # 用户名<input name="mobile" size="30" value="" type="text"></input>
        # 密码 "password_4903" 中数字会变动,故采用绝对路径方法,否则不能定位到元素
        #
        # 勾选记住登录状态check默认是保留 故注释掉该代码 不保留Cookie 则'expiry'=None
        #**********************************************************************
        
        #输入用户名/密码登录
        print u'准备登陆Weibo.cn网站...'
        driver.get("http://login.weibo.cn/login/")
        elem_user = driver.find_element_by_name("mobile")
        elem_user.send_keys(username) #用户名
        elem_pwd = driver.find_element_by_xpath("/html/body/div[2]/form/div/input[2]")
        elem_pwd.send_keys(password)  #密码
        #elem_rem = driver.find_element_by_name("remember")
        #elem_rem.click()             #记住登录状态
        #重点: 暂停时间输入验证码
        #pause(millisenconds)
        time.sleep(20)
        
        elem_sub = driver.find_element_by_name("submit")
        elem_sub.click()              #点击登陆
        time.sleep(2)
        
        #获取Coockie 推荐 http://www.cnblogs.com/fnng/p/3269450.html
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

#********************************************************************************
#                  第二步: 访问个人页面http://weibo.cn/5824697471并获取信息
#                                VisitPersonPage()
#        编码常见错误 UnicodeEncodeError: 'ascii' codec can't encode characters 
#********************************************************************************
def VisitPersonPage(user_id):
    try:
        global infofile
        print u'准备访问个人网站.....'
        #原创内容 http://weibo.cn/guangxianliuyan?filter=1&page=2
        driver.get("http://weibo.cn/" + user_id)
        #**************************************************************************
        # No.1 直接获取 用户昵称 微博数 关注数 粉丝数
        #      str_name.text是unicode编码类型
        #**************************************************************************
        #用户id
        print u'个人详细信息'
        print '**********************************************'
        print u'用户id: ' + user_id
        #昵称
        str_name = driver.find_element_by_xpath("//div[@class='ut']")
        str_t = str_name.text.split(" ")
        num_name = str_t[0]      #空格分隔 获取第一个值 "Eastmount 详细资料 设置 新手区"
        print u'昵称: ' + num_name 
        #微博数 除个人主页 它默认直接显示微博数 无超链接
        #Error:  'unicode' object is not callable
        #一般是把字符串当做函数使用了 str定义成字符串 而str()函数再次使用时报错
        str_wb = driver.find_element_by_xpath("//div[@class='tip2']")  
        pattern = r"\d+\.?\d*"   #正则提取"微博[0]" 但r"(\[.*?\])"总含[] 
        guid = re.findall(pattern, str_wb.text, re.S|re.M)
        print str_wb.text        #微博[294] 关注[351] 粉丝[294] 分组[1] @他的
        for value in guid:
            num_wb = int(value)
            break
        print u'微博数: ' + str(num_wb)
        #关注数
        str_gz = driver.find_element_by_xpath("//div[@class='tip2']/a[1]")
        guid = re.findall(pattern, str_gz.text, re.M)
        num_gz = int(guid[0])
        print u'关注数: ' + str(num_gz)
        #粉丝数
        str_fs = driver.find_element_by_xpath("//div[@class='tip2']/a[2]")
        guid = re.findall(pattern, str_fs.text, re.M)
        num_fs = int(guid[0])
        print u'粉丝数: ' + str(num_fs)
        #页数
        str_page = driver.find_element_by_xpath("//div[@id='pagelist']")
        guid = re.findall(pattern, str_page.text, re.M)
        numPage = int(guid[1])
        print u'页数: ', str_page.text, numPage
        
        #***************************************************************************
        # No.2 文件操作写入信息
        #***************************************************************************
        infofile.write('=====================================================================\r\n')
        infofile.write(u'用户: ' + user_id + '\r\n')
        infofile.write(u'昵称: ' + num_name + '\r\n')
        infofile.write(u'微博数: ' + str(num_wb) + '\r\n')
        infofile.write(u'关注数: ' + str(num_gz) + '\r\n')
        infofile.write(u'粉丝数: ' + str(num_fs) + '\r\n')
        infofile.write(u'微博内容: ' + '\r\n\r\n')
        
        
        #***************************************************************************
        # No.3 获取关注人列表
        # http://weibo.cn/guangxianliuyan?filter=0&page=1
        # 其中filter=0表示全部 =1表示原创
        #***************************************************************************
        print '\n'
        print u'获取微博内容信息'
        num = 1
        #num = numPage
        while num <= 5:
            url_wb = "http://weibo.cn/" + user_id + "?filter=0&page=" + str(num)
            print url_wb
            driver.get(url_wb)
            #info = driver.find_element_by_xpath("//div[@id='M_DiKNB0gSk']/")
            info = driver.find_elements_by_xpath("//div[@class='c']")
            for value in info:
                print value.text
                info = value.text
                #跳过最后一行数据为class=c
                #Error:  'NoneType' object has no attribute 'groups'
                if u'设置:皮肤.图片' not in info:
                    if info.startswith(u'转发'):
                        print u'转发微博'
                        infofile.write(u'转发微博\r\n')
                    else:
                        print u'原创微博'
                        infofile.write(u'原创微博\r\n')
                        
                    #获取最后一个点赞数 因为转发是后有个点赞数
                    str1 = info.split(u" 赞")[-1]
                    if str1: 
                        val1 = re.match(r'\[(.*?)\]', str1).groups()[0]
                        print u'点赞数: ' + val1
                        infofile.write(u'点赞数: ' + str(val1) + '\r\n')
                    str2 = info.split(u" 转发")[-1]
                    if str2: 
                        val2 = re.match(r'\[(.*?)\]', str2).groups()[0]
                        print u'转发数: ' + val2
                        infofile.write(u'转发数: ' + str(val2) + '\r\n')
                    str3 = info.split(u" 评论")[-1]
                    if str3:
                        val3 = re.match(r'\[(.*?)\]', str3).groups()[0]
                        print u'评论数: ' + val3
                        infofile.write(u'评论数: ' + str(val3) + '\r\n')
                    str4 = info.split(u" 收藏 ")[-1]
                    flag = str4.find(u"来自")
                    print u'时间: ' + str4[:flag]
                    infofile.write(u'时间: ' + str4[:flag] + '\r\n')
                    print u'微博内容:'
                    print info[:info.rindex(u" 赞")]  #后去最后一个赞位置
                    infofile.write(info[:info.rindex(u" 赞")] + '\r\n')
                    infofile.write('\r\n')
                    print '\n'
                else:
                    print u'跳过', info, '\n'
                    break
            else:
                print u'next page...\n'
                infofile.write('\r\n\r\n')
            num += 1
            print '\n\n'
        print '**********************************************'
              
        
    except Exception,e:      
        print "Error: ",e
    finally:    
        print u'VisitPersonPage!\n\n'
        print '**********************************************\n'
        
    
#*******************************************************************************
#                                程序入口 预先调用
#*******************************************************************************
    
if __name__ == '__main__':
    #定义变量
    username = '15201615157'             #输入你的用户名
    password = '013579yxz'               #输入你的密码
    
    #user_id = '2778357077'              #用户id url+id访问个人
    #user_id = 'renzhiqiang'
    #user_id = 'guangxianliuyan'  
    #'renzhiqiang' 任志强
    #username = '1685256697@qq.com'
    #password = '1.74.109.1.440'
    
    #操作函数
    LoginWeibo(username, password)      #登陆微博
    #driver.add_cookie({'name':'name', 'value':'_T_WM'})
    #driver.add_cookie({'name':'value', 'value':'c86fbdcd26505c256a1504b9273df8ba'})
    #注意
    #因为sina微博增加了验证码,但是你用Firefox登陆一次输入验证码,再调用该程序即可,因为Cookies已经保证
    #会直接跳转到明星微博那部分,即: http://weibo.cn/guangxianliuyan
    
    #在if __name__ == '__main__':引用全局变量不需要定义 global inforead 省略即可
    print 'Read file:'
    user_id = inforead.readline()
    while user_id!="":
        user_id = user_id.rstrip('\r\n')
        VisitPersonPage(user_id)         #访问个人页面
        user_id = inforead.readline()
        #break
    
    infofile.close()
    inforead.close()
```

(By:Eastmount 2016-04-24 早上7点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


