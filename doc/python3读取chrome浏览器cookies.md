# [python3读取chrome浏览器cookies](https://www.cnblogs.com/camilla/p/7160858.html)



直接从浏览器中取出cookies，有以下好处和用途：
1、不需要配置用户密码，直接读出浏览器中cookies就得到一样的身份，用来完成各种自动化操作。
2、部分网站登录会更新Session,会导致之前成功登录的Session失效，与浏览器使用相同的Session，不用进行登录操作，不会互相挤下线。
3、全是废话，我不想写了，行吗？

使用到软件的sqlite3的图形管理工具有：
SQLiteDatabaseBrowserPortable <http://sqlitebrowser.org/>
sqlitespy <http://www.yunqa.de/delphi/products/sqlitespy/index>

使用到的python库有：
sqlite3 python标准库，不需要下载安装

pywin32 pywin32是python版的windows API库，让python可以调用各种各样的windows API,代码中用到的win32crypt就是属于pywin32库的一部分。
建议手动下载对应版本pywin32安装 <https://sourceforge.net/projects/pywin32/?source=directory>

requests requests是一个相对比较简单易用的http库,用来代替urllib23之类的标准库,使用命令安装pip install requests

看代码：

```python
"""
python3从chrome浏览器读取cookie
get cookie from chrome
"""
import os
import sqlite3
import requests
from win32.win32crypt import CryptUnprotectData

def getcookiefromchrome(host='.oschina.net'):
    cookiepath=os.environ['LOCALAPPDATA']+r"\Google\Chrome\User Data\Default\Cookies"
    sql="select host_key,name,encrypted_value from cookies where host_key='%s'" % host
    with sqlite3.connect(cookiepath) as conn:
        cu=conn.cursor()        
        cookies={name:CryptUnprotectData(encrypted_value)[1].decode() for host_key,name,encrypted_value in cu.execute(sql).fetchall()}
        print(cookies)
        return cookies

#运行环境windows 2012 server python3.4 x64 chrome 50
#以下是测试代码
#getcookiefromchrome()
#getcookiefromchrome('.baidu.com')

url='http://my.oschina.net/'

httphead={'User-Agent':'Safari/537.36',}

#设置allow_redirects为真，访问http://my.oschina.net/ 可以跟随跳转到个人空间
r=requests.get(url,headers=httphead,cookies=getcookiefromchrome('.oschina.net'),allow_redirects=1)
print(r.text)
```



分类: [python](https://www.cnblogs.com/camilla/category/1032138.html)