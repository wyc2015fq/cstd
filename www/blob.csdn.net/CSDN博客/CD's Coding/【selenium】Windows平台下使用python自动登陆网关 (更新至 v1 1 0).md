# 【selenium】Windows平台下使用python自动登陆网关 (更新至 v1.1.0) - CD's Coding - CSDN博客





2017年08月29日 14:48:36[糖果天王](https://me.csdn.net/okcd00)阅读数：875








## 0x00 前言

所里开启了两步验证与二级加密（就不说是哪里了）； 

以前的auto_login用不了了，所以尝试着有没有什么新法子； 

看到一个用 Phantomjs + Selenium 的解决方案，着手试试看好了。
文中涉及的login脚本解决方案参照自如下Github： 
[https://github.com/gongshijun/ict_autoconnect](https://github.com/gongshijun/ict_autoconnect)

由于配置过于坎坷，所以个人直接做了个简化方案， 

嫌看的麻烦的或者想要直接使用的可以直接跳转[这里](#t10)

更新(Thx for lhw446)：嫌配置麻烦的可以用这个[单文件版](#t12)
## 0x01 获取登陆脚本

可以从Github仓库上直接clone到本地

```
D:\Github>git clone https://github.com/gongshijun/ict_autoconnect.git
Cloning into 'ict_autoconnect'...
remote: Counting objects: 29, done.
remote: Total 29 (delta 0), reused 0 (delta 0), pack-reused 29
Unpacking objects: 100% (29/29), done.
Checking connectivity... done.
```

也可以下载zip压缩包到本地解压 
[https://github.com/gongshijun/ict_autoconnect/archive/master.zip](https://github.com/gongshijun/ict_autoconnect/archive/master.zip)

本人放在了`D:\Github`目录下，下文中均默认设为此目录中进行介绍。

## 0x02 环境配置

### python

唔……我确实真的有仔细考虑要不要把python放进环境需求的一部分…… 

想了想……嗯，该说的还是要说的
- Windows cmd (Win+R 输入cmd然后回车) 
- `python2.7`
- 如果您想做成EXE格式的程序，请同时配有`python3.3`或者`python3.4`
- （作者大大炫技结果不小心造成了复杂度的提升）


### PhantomJS

[https://bitbucket.org/ariya/phantomjs/downloads/phantomjs-2.1.1-windows.zip](https://bitbucket.org/ariya/phantomjs/downloads/phantomjs-2.1.1-windows.zip)

windows平台的各位把这个下载下来之后解压放在刚刚的目录中，比如我解压后放在

```
D:\Github\ict_autoconnect 的目录

2017/08/29  13:38    <DIR>          .
2017/08/29  13:38    <DIR>          ..
2017/08/29  13:35             1,266 .gitignore
2017/08/29  13:35             1,005 common.py
2017/08/29  13:35    <DIR>          js
2017/08/29  13:35            11,558 LICENSE
2017/08/29  13:35             2,141 login.py
2016/01/25  06:17    <DIR>          phantomjs-2.1.1-windows  # <= 这里
2017/08/29  13:35             1,237 README.md
2017/08/29  13:35               403 setup.py
2017/08/29  13:40                40 user.conf
```

### selenium

```
# 直接安装，不过可能会连接超时或者很慢
C:\Users\Work>pip install selenium
Collecting selenium
  Retrying (Retry(total=4, connect=None, read=None, redirect=None)) after connection broken by 'ReadTimeoutError("HTTPSConnectionPool(host='pypi.python.org', port=443): Read timed out. (read timeout=15)",)'

# 如果pypi源下载不便，可以选择用国内源下载，记得加上trusted-host参数
# 这里我用豆瓣的了，当然用清华、阿里之类的都是可以的
C:\Users\Work>pip install -i http://pypi.douban.com/simple/ selenium --trusted-host pypi.douban.com
Installing collected packages: selenium
Successfully installed selenium-3.5.0
```

## 0x03 准备工作

### user.conf （设置用户名&密码）

conf文件里写好自己的用户名密码就好啦

```
username = YOUR_USERNAME
password = YOUR_PASSWORD
```

### setup.py（输出为可执行文件，python 3.3/3.4 可选）

这个`setup.py`的作用是将python脚本做成exe可执行文件， 

然而其中带有的`py2exe`模块是py3.3以上才可以使用的包，所以如果想做成exe格式的执行文件的话， 
**请自备py2与py3两个版本的python**
接下来是本人的心路历程

```
# 首先我们来看看这个setup.py
D:\Github\ict_autoconnect>python setup.py
Traceback (most recent call last):
  File "setup.py", line 10, in <module>
    import py2exe
ImportError: No module named py2exe

# 好嘛，没有我就去装呗
D:\Github\ict_autoconnect>pip install py2exe
Collecting py2exe
  Retrying (Retry(total=4, connect=None, read=None, redirect=None)) after connection broken by 'ReadTimeoutError("HTTPSConnectionPool(host='pypi.python.org', port=443): Read timed out. (read timeout=15)",)': /simple/py2exe/

# 哦忘记了……
D:\Github\ict_autoconnect>pip install -i http://pypi.douban.com/simple/ py2exe --trusted-host pypi.douban.com
Collecting py2exe
    RuntimeError: This package requires Python 3.3 or later

# WHAT???
# 转为 python3.6.0 后（请善用activate）

# 对了转成python3之后别忘了刚刚装的selenium
D:\Github\ict_autoconnect>pip install -i http://pypi.douban.com/simple/ selenium --trusted-host pypi.douban.com
Installing collected packages: selenium
Successfully installed selenium-3.5.0

# 可以，能装py2exe了
D:\Github\ict_autoconnect>pip install -i http://pypi.douban.com/simple/ py2exe --trusted-host pypi.douban.com
Installing collected packages: py2exe
Successfully installed py2exe-0.9.2.2

# ......
D:\Github\ict_autoconnect>python setup.py
  yield "store", (names[oparg],)
IndexError: tuple index out of range
```

> 
**细心的我去stackoverflow查了一下**
[传送门：python-indexerror-tuple-index-out-of-range-when-using-py2exe](https://stackoverflow.com/questions/41578808/python-indexerror-tuple-index-out-of-range-when-using-py2exe)

  The bytecode often changes in small ways from version to version that could break even Python 3.5 (given only 3.3 and 3.4 support is claimed explicitly), but 3.6 is 100% guaranteed to fail.
简单解释一下，py2exe这个东西可行的版本极为狭窄，py3.3或3.4才可以使用：（以下说明来自知乎 @刘智敏） 

把python回退到3.4之后，这个问题得到解决。根据Python3.5的API变动说明，初步判断问题原因是 generators 在3.5之后的属性有变化，而 yield 的实现用到了 generator（了解很浅，如有误请指正）。正好py2exe支持的最高版本为py3.4  。

```
# 怒开一个新的python3.3环境 
$ conda create -n python3.3 numpy python=3.3 # numpy可选，我个人装啥都喜欢带个numpy
$ activate python3.3

(python3.3) D:\Github\ict_autoconnect>pip install -i http://pypi.douban.com/simple/ py2exe --trusted-host pypi.douban.com

(python3.3) D:\Github\ict_autoconnect>pip install -i http://pypi.douban.com/simple/ selenium --trusted-host pypi.douban.com

(python3.3) D:\Github\ict_autoconnect>python setup.py
running py2exe

  5 missing Modules
  ------------------
? netbios                             imported from uuid
? readline                            imported from cmd, code, pdb
? win32api                            imported from platform
? win32con                            imported from platform
? win32wnet                           imported from uuid
Building 'dist\login.exe'.
Building shared code archive 'dist\library.zip'.
Copy c:\programdata\anaconda3\envs\python3.3\python33.dll to dist
Copy C:\ProgramData\Anaconda3\envs\python3.3\DLLs\unicodedata.pyd to dist\unicodedata.pyd
Copy C:\ProgramData\Anaconda3\envs\python3.3\DLLs\_lzma.pyd to dist\_lzma.pyd
Copy C:\ProgramData\Anaconda3\envs\python3.3\DLLs\_ctypes.pyd to dist\_ctypes.pyd
Copy C:\ProgramData\Anaconda3\envs\python3.3\DLLs\_bz2.pyd to dist\_bz2.pyd
Copy C:\ProgramData\Anaconda3\envs\python3.3\DLLs\_hashlib.pyd to dist\_hashlib.pyd
Copy C:\ProgramData\Anaconda3\envs\python3.3\DLLs\_socket.pyd to dist\_socket.pyd
Copy C:\ProgramData\Anaconda3\envs\python3.3\DLLs\_ssl.pyd to dist\_ssl.pyd
Copy C:\ProgramData\Anaconda3\envs\python3.3\DLLs\pyexpat.pyd to dist\pyexpat.pyd
Copy C:\ProgramData\Anaconda3\envs\python3.3\DLLs\select.pyd to dist\select.pyd
Copy DLL C:\ProgramData\Anaconda3\envs\python3.3\MSVCR100.dll to dist\
```

### library.zip

`$ python setup.py`搞定之后，就会发现目录下多了几个文件夹，进入目录`js/`，把里面的两个js文件放进`dist/library.zip`里的`selenium/webdriver/remote`里。

![test](https://img-blog.csdn.net/20170829143015152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后这个dist文件夹就相当于一个你本人定制的release了，那个`login.exe`就可以直接使用了（基于之前user.conf里的帐号与密码）

```
D:\Github\ict_autoconnect\dist 的目录

2017/08/29  14:35    <DIR>          .
2017/08/29  14:35    <DIR>          ..
2017/08/29  14:35         4,970,261 library.zip
2017/08/29  14:34            33,280 login.exe
2010/03/18  22:36           827,728 MSVCR100.dll
2014/03/10  00:35           168,960 pyexpat.pyd
2017/08/29  14:34         3,927,552 python33.dll
2014/03/10  00:35            10,752 select.pyd
2014/03/10  00:35           762,368 unicodedata.pyd
2014/03/10  00:35            62,464 _bz2.pyd
2014/03/10  00:35           108,032 _ctypes.pyd
2014/03/10  00:35         1,065,472 _hashlib.pyd
2014/03/10  00:35           135,168 _lzma.pyd
2014/03/10  00:35            51,712 _socket.pyd
2014/03/10  00:35         1,710,080 _ssl.pyd
```

## 0x04 人生苦短，善用python

哇这么一大堆东西怕是看着看着都要睡着了哟…… 

（有空回来做个简易版，就算不简易至少也要把那么多指令整合到一个shell脚本里……）

> 
**CDLogin for Windows/ 自制简易版解决方案使用方法**

  1) 到百度网盘下载：( 当前最新版本为 Release v1.0.1 ) 

  => [公有传送门](http://pan.baidu.com/s/1qYE2n1M) 无需密码 

  => [密码传送门](http://pan.baidu.com/s/1bXBtMY) 密码：hf3w
2) 解压到目录，然后使用cmd进入该目录，安装selenium： 
`$ pip install -i http://pypi.douban.com/simple/ selenium --trusted-host pypi.douban.com`

3) 把`user.conf`里的帐号密码改成你们自己的！ 

  请务必改成你们自己的，即便是为了稳定性

4) 双击打开`login.py`

  推荐把.py文件的默认打开方式设置为python.exe， 

  这样就可以直接运行而不需要再包一层shell来跑`$ python login.py`了
5) 如果正常运转了，发送到桌面快捷方式，然后把这个快捷方式放进你的启动文件夹里即可 

  一般来说是个叫`Startup`或者`启动`的目录

6) 尝试着为多平台努力中 

  为了程序容量不能过于庞大，就不把各平台的PhantomJS都放进去了，如果你不是windows平台， 

  请前往 [http://phantomjs.org/download.html](http://phantomjs.org/download.html) 下载你自己平台的PhantomJS， 

  解压之后在里面找到PhantomJS放在和login.py同目录下即可。
## 0x05 主文件SourceCode

```python
#coding=utf8
# ========================================================
#   Copyright (C) 2017 All rights reserved.
#   
#   filename : CDLogin.py
#   author   : chendian@baidu.com
#   date     : 2017-08-29
#   desc     : Auto_Login Test
# ======================================================== 

import os
import time
import selenium
from selenium import webdriver

import platform
# "Windows" or "Linux"
OS_TYPE = platform.system()

def auto_connect(un, pw):
    print('open a phantomJS browser...\n')
    try:
        browser = webdriver.PhantomJS()
    except selenium.common.exceptions.WebDriverException as e:
        # Message: 'phantomjs' executable needs to be in PATH.
        print("Current Version only contains Windows PhantomJS.")
        print("You need phantomJS for your own platform as <%s>" % OS_TYPE)
        print("Please visit http://phantomjs.org/download.html for downloading")
        print("Download and unzip and find PhantomJS, copy it beside this python file.")
        print("For further info, visit http://blog.csdn.net/okcd00/article/details/77677326#t10")
        os.system('pause')
        exit()

    browser.get('http://159.226.39.22')

    username, password = un, pw

    browser.find_element_by_id('username').send_keys(username)
    browser.find_element_by_id('password').send_keys(password)
    #browser.find_element_by_id('button').click()
    browser.find_element_by_id('form2').submit()

    print('waiting 10s for posting...')
    time.sleep(10)
    browser.close()

def read_conf():
    un = ''
    pw = ''
    ps = ''
    try:
        with open('user.conf', 'r') as file:
            for line in file.readlines():
                line = line.strip()
                if line == '':
                    continue
                if line[0] == '#':
                    continue
                if line.split('=')[0].strip() == 'username':
                    un = line.split('=')[1].strip()
                elif line.split('=')[0].strip() == 'password':
                    pw = line.split('=')[1].strip()
                elif line.split('=')[0].strip() == 'pingsite':
                    ps = line.split('=')[1].strip()
                else:
                    print('user.conf is a error format. \n')
    except IOError as e:
        print('cannot read user.conf file--' + str(e) + ', please create user.conf file. \n')
    return un, pw, ps

def is_connect(ps):
    if OS_TYPE == 'Windows':
        flag = os.system('ping ' + ps)
    elif OS_TYPE == 'Linux':
        flag = os.system('ping -c 3 ' + ps)
    if flag == 0:
        return True
    return False

def con_connect(un, pw, ps):
    if is_connect(ps):
        return 1
    else:
        auto_connect(un, pw)
        if is_connect(ps):
            print('\nConnect internet success.\n')
        else:
            print('\nConnect internet Failed.\n')
        return 1


if __name__ == '__main__':
    print("starting...")
    os.system('MODE con: COLS=128 LINES=32 & TITLE Welcome to CDLogin')

    from logo import show_logo
    # show_logo()
    os.system('cls')

    time.sleep(1)
    os.system("color 0A")
    os.system('MODE con: COLS=64 LINES=16 & TITLE CD_Auto_Login is working')

    un, pw, ps = read_conf()

    while 1:
        print('=== current time ===')
        os.system("DATE /T & TIME /T")

        print('If you want to stop, input Ctrl+C to break.')
        con_connect(un, pw, ps)

        print('\nNext check in 10 mins ...\n')
        time.sleep(10*60) #Test
        os.system('cls')
```

## 0xFF 单文件可行版本

```python
#coding=utf8
import os
import time
import random
import urllib
import urllib2
import platform
import cookielib

uname = "test" # Change to your username
passwd = "123456" # Change to your password
ip = "10.63.0.222" # Modify this to your own ip 
interval_time = 5*60


def login(uname, passwd):
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    opener.addheaders = [('User-agent','Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)')]
    urllib2.install_opener(opener)
    data = {
        "action": "login",
        "username": uname,
        "password": passwd,
        "ac_id": 1,
        "user_ip": ip,
        "ip": ip,
        "nas_ip": '',
        "user_mac": '',
        "save_me": 0,
        "type": 1,
        "n": 100,
        "ajax": 1
    }
    req = urllib2.Request("http://159.226.39.22/include/auth_action.php", urllib.urlencode(data))
    req.add_header("Referer","http://159.226.39.22/")
    try:
        resp = urllib2.urlopen(req)
        res = resp.read()
        print res
        if 'login_ok' in res:
            return True
        else:
            return False
    except:
        print 'log in error'
        return False


def logout(uname, passwd):
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    opener.addheaders = [('User-agent','Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)')]
    urllib2.install_opener(opener)
    data = {
        "action": "logout",
        "username": uname,
        "password": passwd,
        "ajax": 1
    }
    req = urllib2.Request("http://159.226.39.22/include/auth_action.php", urllib.urlencode(data))
    req.add_header("Referer","http://159.226.39.22/")
    try:
        resp = urllib2.urlopen(req)
        res = resp.read()
        print res
        if "网络已断开" in res or "未曾连接到网络" in res:
            return True
        else:
            return False
    except:
        print 'log out error'
        return False


def internet_on():
    os_type = platform.system()
    if os_type == "Windows":
        flag = os.system('ping www.baidu.com')
    else:
        flag = os.system('ping -c 3 -t 3 www.baidu.com')
    return flag == 0


def logout_login():
    if logout(uname, passwd):
        print 'logout at: ', time.asctime()
        time.sleep(5)
        if login(uname, passwd):
            print 'login at: ', time.asctime()
        else:
            print 'login fails at: ', time.asctime()
    else:
        print 'logout fails at: ', time.asctime()


if __name__ == '__main__':
    if uname == "lihongwei_test":
        print "please set username, password and ip!"
    else:
        while True:
            if not internet_on():
                logout_login()
            time.sleep(interval_time)
```

## UpdateLog

[2017/09/04] ver 1.1.0 (Author: @lhw446, Original: @cyx) 

+ Add single file script.

[2017/08/29] ver 1.0.1  (Feat: @lhw446)
- Add guide for downloading and setting phantomJS
- Add available platforms (MAC platform as Linux, maybe Darwin)

[2017/08/29] ver 1.0.0 (Feat: @gongshijun)
- Basic function for auto_login for ICT












