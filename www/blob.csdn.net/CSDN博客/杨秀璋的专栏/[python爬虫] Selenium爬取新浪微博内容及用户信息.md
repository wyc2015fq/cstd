
# [python爬虫] Selenium爬取新浪微博内容及用户信息 - 杨秀璋的专栏 - CSDN博客

2016年02月23日 05:32:02[Eastmount](https://me.csdn.net/Eastmount)阅读数：35974所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)



在进行自然语言处理、文本分类聚类、推荐系统、舆情分析等研究中，通常需要使用新浪微博的数据作为语料，这篇文章主要介绍如果使用Python和Selenium爬取自定义新浪微博语料。因为网上完整的语料比较少，而使用Selenium方法有点简单、速度也比较慢，但方法可行，同时能够输入验证码。希望文章对你有所帮助~
源码下载地址：[http://download.csdn.net/detail/eastmount/9501273](http://download.csdn.net/detail/eastmount/9501273)
**爬取结果**
首先可以爬取用户ID、用户名、微博数、粉丝数、关注数及微博信息。其中微博信息包括转发或原创、点赞数、转发数、评论数、发布时间、微博内容等等。如下图所示：
![](https://img-blog.csdn.net/20160223031918122)
同时也可以爬取微博的众多用户的详细信息，包括基本信息、关注人ID列表和粉丝ID列表等等。如下图所示：
![](https://img-blog.csdn.net/20160223031831871)
**登录入口**
新浪微博登录常用接口：[http://login.sina.com.cn/](http://login.sina.com.cn/)
对应主界面：[http://weibo.com/](http://weibo.com/)
但是个人建议采用手机端微博入口：[http://login.weibo.cn/login/](http://login.weibo.cn/login/)
对应主界面：[http://weibo.cn/](http://weibo.cn/)
其原因是手机端数据相对更轻量型，同时基本数据都齐全，可能缺少些个人基本信息，如"个人资料完成度"、"个人等级"等，同时粉丝ID和关注ID只能显示20页，但完全可以作为语料进行大部分的验证。
通过比较下面两张图，分别是PC端和手机端，可以发现内容基本一致：
![](https://img-blog.csdn.net/20160223041531679)
手机端下图所示，其中图片相对更小，同时内容更精简。

![](https://img-blog.csdn.net/20160223042016263)

**完整源码**
下面代码主要分为三部分：
1.LoginWeibo(username, password) 登录微博
2.VisitPersonPage(user_id) 访问跟人网站，获取个人信息
3.获取微博内容，同时http://weibo.cn/guangxianliuyan?filter=0&page=1实现翻页
```python
# coding=utf-8
"""  
Created on 2016-02-22 @author: Eastmount
功能: 爬取新浪微博用户的信息
信息：用户ID 用户名 粉丝数 关注数 微博数 微博内容
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
inforead = codecs.open("SinaWeibo_List.txt", 'r', 'utf-8')
infofile = codecs.open("SinaWeibo_Info.txt", 'a', 'utf-8')

#********************************************************************************
#                  第一步: 登陆weibo.cn 获取新浪微博的cookie
#        该方法针对weibo.cn有效(明文形式传输数据) weibo.com见学弟设置POST和Header方法
#                LoginWeibo(username, password) 参数用户名 密码
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
        # No.3 获取微博内容
        # http://weibo.cn/guangxianliuyan?filter=0&page=1
        # 其中filter=0表示全部 =1表示原创
        #***************************************************************************
        print '\n'
        print u'获取微博内容信息'
        num = 1
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
    username = '1520161***'             #输入你的用户名
    password = '**********'               #输入你的密码
    user_id = 'guangxianliuyan'          #用户id url+id访问个人 

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
PS：发现CSDN编辑器的BUG，只要包含(![](https://img-blog.csdn.net/20160223053458078)) 如：r'\[(.*?)\]'就会自动换行 (⊙o⊙)

**登录页面**
首先，为什么需要登录呢？
因为新浪微博很多数据如果不登录是不能获取或访问的，如微博的粉丝列表、个人详细信息、微博下一页等等，当你点击这些超链接时就会自动跳转到登录界面，这是开发者对其进行的保护措施。同时，各个公司都会提供API接口让开发者进行操作，但此处我是使用Selenium模拟浏览器操作进行爬取的。
![](https://img-blog.csdn.net/20160223043500952)
其中登录如上图所示，函数LoginWeibo(username, password) 实现，它会自动打开浏览器并输入用户名和密码。在登录过程中由于会涉及到验证码，所以我采用暂停20秒，当用户手动输入验证码并且时间到后会自动点击按钮登录。核心代码如下：
driver.get("http://login.weibo.cn/login/")
elem_user = driver.find_element_by_name("mobile")
elem_user.send_keys(username)      \#用户名
elem_pwd = driver.find_element_by_xpath("/html/body/div[2]/form/div/input[2]")
elem_pwd.send_keys(password)       \#密码
elem_sub = driver.find_element_by_name("submit")
elem_sub.click()              \#点击登陆
如果你登陆过程中Python报错：
WebDriverException: Message: "Can't load the profile. Profile Dir:
猜测是Firefox版本问题，升级后出现的该问题，建议下载相对较老的版本，总体感觉只要Selenium、Python、Firefox版本一致就不会报错，可从下面链接中安装该版本Firefox。
下载地址：[http://download.csdn.net/detail/mengh2016/7752097](http://download.csdn.net/detail/mengh2016/7752097)
那么，登录成功后，为什么就能访问或跳转到不同的页面呢？
因为登录成功后会保存Cookies或Session信息，此时用户就可以任意跳转访问了，否则会重新跳转会登录界面。这里使用Selenium的driver.get(url)实现跳转。
获取个人信息
首先很多网站设计都是 URL+用户ID 访问个人网站，如柳岩：[http://weibo.cn/guangxianliuyan](http://weibo.cn/guangxianliuyan)
故定义一个TXT文件列表包含，所有用户ID信息，依次通过读取文件爬取其微博信息：
user_id = inforead.readline()
while user_id!="":
user_id = user_id.rstrip('\r\n')
VisitPersonPage(user_id)         \#访问个人页面
user_id = inforead.readline()
其中用户ID列表在SinaWeibo_List.txt 中，如下所示：（明星）
```python
guangxianliuyan
zhangjiani
1862829871
zmqd
houzimi
3125046087
gezhaoen
1877716733
ailleenmmm
linshenblog
superleoisme
2638613703
duiersky
ws95
wuwei1003673996
wuxin
1413971423
xiena
yangxinyu888
zhangyangguoer0418
liuyifeiofficial
```
通过分析HTML源码，获取节点位置，通过Selenium函数定义位置获取信息，然后再通过正则表达式或字符串处理获取想要的值。如获取昵称：
str_name = driver.find_element_by_xpath("//div[@class='ut']")
\#空格分隔 获取第一个值 "Eastmount 详细资料 设置 新手区"
str_t = str_name.text.split(" ")
num_name = str_t[0]
print u'昵称: ' + num_name
再如括号之间数字内容：
\#微博[294] 关注[351] 粉丝[294] 分组[1] @他的
str_gz = driver.find_element_by_xpath("//div[@class='tip2']/a[1]")
guid = re.findall(pattern, str_gz.text, re.M)
num_gz = int(guid[0])
print u'关注数: ' + str(num_gz)
![](https://img-blog.csdn.net/20160223050456086)

资料URL：http://weibo.cn/1644461042/info
关注URL：http://weibo.cn/1644461042/follow
粉丝URL：http://weibo.cn/1644461042/fans
但是手机端只能显示20页粉丝列表和关注列表。
点击"资料"可以获取个人详细信息、点击"关注[516]"可以获取关注列表，如果需要建立不同用户之间的关注网，个人建议通过关注表而不是粉丝表，因为关注表覆盖明星更大，而粉丝太多，构建的图太稀疏。个人信息如下图所示：
![](https://img-blog.csdn.net/20160223050850075)
获取微博
微博URL：http://weibo.cn/guangxianliuyan?filter=0&page=1
通过分析如下URL链接，可以发现Page=n 表示访问第n页微博，从而实现跳转。filter=1表示原创，可以分析它对应的开头几个类型。再通过函数获取内容：
info = driver.find_elements_by_xpath("//div[@class='c']")
然后如果发布的微博以"转发了..."开头表示转发的微博，否则为原创微博，代码：
info.startswith(u'转发')
同时获取微博点赞数、转发数、时间等，其中因为转发会包括点赞数，故获取最后一个"赞[xxx]"，然后通过正则表达式i获取括号之间内容。
str1 = info.split(u" 赞")[-1]
if str1:
val1 = re.match(![](https://img-blog.csdn.net/20160223053458078), str1).groups()[0]
print u'点赞数: ' + val1
infofile.write(u'点赞数: ' + str(val1) + '\r\n')
微博都是以class='c'节点，故获取是有这类值，再进行字符串处理即可。
![](https://img-blog.csdn.net/20160223051948322)
PS：最后希望文章对你有所帮助！其实方法很简单，希望你能理解这种思想，如何分析HTML源码及DOM树结构，然后动态获取自己需要的信息。
(By:Eastmount 2016-02-23 深夜5点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



