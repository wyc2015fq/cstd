# Sina微博爬取@pyspider - =朝晖= - 博客园
# [Sina微博爬取@pyspider](https://www.cnblogs.com/dhcn/p/7120887.html)
      这是一篇不应该写的文章，都写了，针对特定“方式”的爬虫也就没法爬了。
1、模拟登录的一些文章：
解析新浪微博的登录过程(2013-12-23): http://www.cnblogs.com/houkai/p/3487816.html
[Python](http://lib.csdn.net/base/python)模拟登陆新浪微博(2013-12-24): http://www.cnblogs.com/houkai/p/3488468.html
2014_新浪微博模拟登陆_python(2014-08-08): http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/springzfx/article/details/38435069
Wap的weibo.cn模拟登录@2012年11月8日: http://qinxuye.me/article/simulate-weibo-login-in-[python](http://lib.csdn.net/base/python)/
wap端模拟登陆withPython@2015-04-15: http://www.cnblogs.com/manqing/p/4428418.html
2、Chrome下登录请求的所有信息，其中像random字段必须通过先get一次登录页来得到，通过分析chrome开发工具下的这些信息基本就Ok，再就是可以复制curl指令看看也很方便：
```
---------------  
General Info:  
Remote Address:180.149.153.4:80  
Request URL:http://login.weibo.cn/login/?rand=1623920428&backURL=http%3A%2F%2Fweibo.cn&backTitle=%E6%89%8B%E6%9C%BA%E6%96%B0%E6%B5%AA%E7%BD%91&vt=4  
Request Method:POST  
Status Code:302 Found
```
```
----------------------  
Response Headers  
view source  
Connection:close  
Content-Encoding:gzip  
Content-Length:20  
Content-Type:text/html  
Date:Thu, 05 Nov 2015 08:11:34 GMT  
Location:http://newlogin.sina.cn/crossDomain/?g=4ugB9b801ltp<span style="font-family: Arial, Helvetica, sans-serif;">*************</span>Q&t=1446711094&m=05b4&r=&u=http%3A%2F%2Fweibo.cn%3Fgsid%3D4ugB9b801ltp<span style="font-family: Arial, Helvetica, sans-serif;">*************</span>Q%26PHPSESSID%3D%****%3D4&cross=1&st=ST-NTc0MDUwMTQ4Ng==-1446711094-tc-F903CC8EC38C7F499D7D227DA862A39E,ST-NTc0MDUwMTQ4Ng==-1446711094-tc-F4B911C868B51E768875EB15BBAE6E83&vt=4  
Server:Apache  
Set-Cookie:SUB=_2A257P39mDeTxGeNJ71IU8C_IwzqIHXVYwAEurDV6PUJbrdAKLVjSkW03kbkKkoC7qLmx-EiB5I-Diw91GQ..; expires=Sat, 05-Dec-2015 08:11:34 GMT; path=/; domain=.weibo.cn; httponly  
Set-Cookie:gsid_CTandWM=4ugB9b801ltp*************Q; expires=Sat, 05-Dec-2015 08:11:34 GMT; path=/; domain=.weibo.cn; httponly  
SINA-LB:aGEuMTY1LmcxLnlmLmxiLnNpbmFub2RlLmNvbQ==  
SINA-TS:MzU2ZWMzNjggMCAwIDAgNSA5Mwo=  
Vary:Accept-Encoding  
x-debug:172.16.140.196  
X-Log-Uid:5740501486
```
```
------------------  
Request Headers view source  
Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8  
Accept-Encoding:gzip, deflate  
Accept-Language:zh-CN,zh;q=0.8  
Cache-Control:max-age=0  
Connection:keep-alive  
Content-Length:208  
  
Cookie:_T_WM=086561716ab68ff3c6e589cc5152ed66  
DNT:1  
Host:login.weibo.cn  
Origin:http://login.weibo.cn  
Referer:http://login.weibo.cn/login/  
Upgrade-Insecure-Requests:1  
User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.80 Safari/537.36
```
```
-------------------  
Query String Parameters  
view source  
view URL encoded  
rand:1623920428  
backURL:http://weibo.cn  
backTitle:手机新浪网  
vt:4  
-------------------  
Form Data  
view source  
view URL encoded  
mobile:username@sina.cn  
password_4509:user_password  
remember:on  
backURL:http%3A%2F%2Fweibo.cn  
backTitle:手机新浪网  
tryCount:  
vk:4509_290b_1897615582  
submit:登录
```
3、上一个在pyspider上能走通weibo.cn爬取示例代码，这个代码的缺憾就是如果频繁登陆可能会出验证码，目前还没加验证码识别特性。
```
#!/usr/bin/env python  
# -*- encoding: utf-8 -*-  
# Created on 2015-11-05 11:41:06  
# Project: sinaweibo  
# SinaUser:username@sina.cn  
from pyspider.libs.base_handler import *  
  
  
class Handler(BaseHandler):  
    crawl_config = {  
        'headers': {  
            'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_1) AppleWebKit/537.36 (KHTML, like Gecko)',  
        }  
    }  
     
 
    @every(minutes=24 * 60)  
    def on_start(self):  
          
        self.crawl('http://login.weibo.cn/login/', callback=self.login)  
 
    @config(age=1 * 24 * 60 * 60)  
    def login(self, response):  
        cookies = response.cookies  
        base_url = 'http://login.weibo.cn/login/'  
        url = base_url + response.doc("form").attr("action")  
        data = {}  
        for each in response.doc("form input"):  
            data[each.name]=each.value  
            if each.name == "mobile":  
                data["mobile"] = "username@sina.cn"  
            if each.type == "password":  
                data[each.name] = "password"  
        headers = {}  
        headers["Content-Type"]="application/x-www-form-urlencoded"  
        headers["Referer"]="http://login.weibo.cn/login/"  
        headers["Origin"]="http://login.weibo.cn"  
        headers["Referer"]="http://login.weibo.cn/login/"  
        self.crawl(url, callback=self.login_ok,data=data,cookies=cookies,headers=headers,method="POST")  
         
 
    @config(priority=2)  
    def login_ok(self, response):  
        ''''' 
        return { 
            "url": response.url, 
            "headers": response.headers, 
            "cookies": response.cookies, 
            "status":response.status_code, 
            "text":response.text, 
        } 
        '''  
          
        self.crawl("http://weibo.cn/yumayin",  
                   cookies=response.cookies,callback=self.index_page)  
          
    def index_page(self, response):  
          
        weibos  = []  
        for each in response.doc("div.c").items():  
            #each.find("span.kt"):  
            if each.find("span.kt").html()!= None:  
                continue  
            #if each.find("span.ctt")!= None:  
            if each.find("span.ctt").html()!= None:  
                weibos.append(each.find("span.ctt").html())      
        return weibos
```
4、phantomjs启动的一个问题，官方的文档好像有点问题，真正的启动命令是启动phantomjs proxy选项：
```
pyspider --phantomjs-proxy="localhost:25555" -c config.json
```
这个解决方法是参考：http://blog.csdn[.net](http://lib.csdn.net/base/dotnet)/jxnu_xiaobing/article/details/44983757 
还有可以把运行组件改成all，也可以全部启动:
```
pyspider  -c config.json all
```
5、启用phantomjs以后，[js](http://lib.csdn.net/base/javascript)抓取返回HTML是乱码的问题，官方给出的回答：Phantomjs doesn't support gzip, don't set Accept-Encoding header with gzip.
6、重复任务的判定问题，get某登录地址，然后post该登录地址登录，第二步操作可能失败，pyspider判定重复任务完全看URL地址，部署以后，第二个请求会因为age设定问题，直接被拒。
7、DB[数据库](http://lib.csdn.net/base/mysql)选择的问题，谈这个问题，首先说说存储方式，projectdb和taskdb，一般我们用户不用访问，就不用说了，resultdb里面，除了我们解析返回的result数据，同时存储的还有id、updatetime、url，而我们返回的result数据会被json序列化为二进制字符串，这种字符串只包含字母之类键盘有的字符串，所以你不论用[MongoDB](http://lib.csdn.net/base/mongodb)还是RDB，存储其实都是一层数据，不过在resultdb里面，如果我们做挂钩存储的话，[mongodb](http://lib.csdn.net/base/mongodb)会方便一点，起码不用改数据定义模式。做这类对pyspider的hacking开发，可以到pyspider的目录里面直接修改代码。
8、官网docs上没有[Redis](http://lib.csdn.net/base/redis)带密码连接字符串，先后在github issues和django-[redis](http://lib.csdn.net/base/redis)文档上找到这个设置格式，后者的更全面一点：
```
redis://[:password]@localhost:6379/0  
rediss://[:password]@localhost:6379/0  
unix://[:password]@/path/to/socket.sock?db=0
```
不过要使用这个带password的连接字符串，pyspider版本得升级到最新版本。
9、每个爬虫的入口函数一定要用@every来装饰，千万不要写成别的周期控制装饰器，否则你的这个爬虫项目永远不会被调度器启动。

