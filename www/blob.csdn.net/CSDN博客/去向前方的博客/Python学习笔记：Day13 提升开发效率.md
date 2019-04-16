# Python学习笔记：Day13 提升开发效率 - 去向前方的博客 - CSDN博客





2018年07月08日 09:15:51[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：97








# 前言

最近在学习深度学习，已经跑出了几个模型，但Pyhton的基础不够扎实，因此，开始补习Python了，大家都推荐廖雪峰的课程，因此，开始了学习，但光学有没有用，还要和大家讨论一下，因此，写下这些帖子，廖雪峰的课程连接在这里：[廖雪峰](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000)

Python的相关介绍，以及它的历史故事和运行机制，可以参见这篇：[python介绍](https://blog.csdn.net/lvsehaiyang1993/article/details/80644237)

Python的安装可以参见这篇：[Python安装](https://blog.csdn.net/lvsehaiyang1993/article/details/80644321)

Python的运行模式以及输入输出可以参见这篇：[Python IO](https://blog.csdn.net/lvsehaiyang1993/article/details/80644827)

Python的基础概念介绍，可以参见这篇：[Python 基础](https://blog.csdn.net/lvsehaiyang1993/article/details/80647010)

Python字符串和编码的介绍，可以参见这篇：[Python字符串与编码](https://blog.csdn.net/lvsehaiyang1993/article/details/80648947)

Python基本数据结构：list和tuple介绍，可以参见这篇：[Python list和tuple](https://blog.csdn.net/lvsehaiyang1993/article/details/80649384)

Python控制语句介绍：ifelse，可以参见这篇：[Python 条件判断](https://blog.csdn.net/lvsehaiyang1993/article/details/80649552)

Python控制语句介绍：循环实现，可以参见这篇：[Python循环语句](https://blog.csdn.net/lvsehaiyang1993/article/details/80651808)

Python数据结构：dict和set介绍[Python数据结构dict和set](https://blog.csdn.net/lvsehaiyang1993/article/details/80652307)

Python函数相关：[Python函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80654173)

Python高阶特性：[Python高级特性](https://blog.csdn.net/lvsehaiyang1993/article/details/80661973)

Python高阶函数：[Python高阶函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80665892)

Python匿名函数：[Python匿名函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80667629)

Python装饰器：[Python装饰器](https://blog.csdn.net/lvsehaiyang1993/article/details/80667177)

Python偏函数：[Python偏函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80673522)

Python模块：[Python模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80673944)

Python面向对象编程（1）：[Python面向对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80674239)

Python面向对象编程（2）：[Python面向对象（2）](https://blog.csdn.net/lvsehaiyang1993/article/details/80675404)

Python面向对象编程（3）：[Python面向对象（3）](https://blog.csdn.net/lvsehaiyang1993/article/details/80675873)

Python面向对象编程（4）：[Pyhton面向对象（4）](https://blog.csdn.net/lvsehaiyang1993/article/details/80676298)

Python面向对象高级编程（上）：[Python面向对象高级编程（上）](https://blog.csdn.net/lvsehaiyang1993/article/details/80688026)

Python面向对象高级编程（中上）：[Python面向对象高级编程（中上）](https://blog.csdn.net/lvsehaiyang1993/article/details/80689908)

Python面向对象高级编程（中下）：[Python面向对象高级编程（中下）](https://blog.csdn.net/lvsehaiyang1993/article/details/80691702)

Python面向对象高级编程（完）：[Python面向对象高级编程（完）](https://blog.csdn.net/lvsehaiyang1993/article/details/80692002)

Python错误调试（起）：[Python调试：起](https://blog.csdn.net/lvsehaiyang1993/article/details/80694444)

Python错误调试（承）：[Python调试：承](https://blog.csdn.net/lvsehaiyang1993/article/details/80694819)

Python错误调试（转）：[Python调试：转](https://blog.csdn.net/lvsehaiyang1993/article/details/80695280)

Python错误调试（合）：[python调试：合](https://blog.csdn.net/lvsehaiyang1993/article/details/80697743)

Python文件IO编程：[Python文件IO](https://blog.csdn.net/lvsehaiyang1993/article/details/80700751)

Python文件IO编程2：[Python文件IO2](https://blog.csdn.net/lvsehaiyang1993/article/details/80707102)

Python文件IO编程3：[PYthon文件IO3](https://blog.csdn.net/lvsehaiyang1993/article/details/80707554)

Python进程和线程（起）：[Python进程和线程起](https://blog.csdn.net/lvsehaiyang1993/article/details/80709180)

Python进程和线程（承）：[Python进程和线程承](https://blog.csdn.net/lvsehaiyang1993/article/details/80709260)

Python进程和线程（转）：[Python进程和线程转](https://blog.csdn.net/lvsehaiyang1993/article/details/80709353)

Python进程和线程（合）：[Python进程和线程合](https://blog.csdn.net/lvsehaiyang1993/article/details/80709405)

Python正则表达式：[Python正则表达式](https://blog.csdn.net/lvsehaiyang1993/article/details/80711331)

Python学习笔记:常用内建模块1：[Python学习笔记：常用内建模块1](https://blog.csdn.net/lvsehaiyang1993/article/details/80712912)

Python学习笔记:常用内建模块2：[Python学习笔记：常用内建模块2](https://blog.csdn.net/lvsehaiyang1993/article/details/80713218)

Python学习笔记:常用内建模块3：[Python学习笔记：常用内建模块3](https://blog.csdn.net/lvsehaiyang1993/article/details/80713747)

Python学习笔记:常用内建模块4：[Python学习笔记:   常用内建模块4](https://blog.csdn.net/lvsehaiyang1993/article/details/80716665)

Python学习笔记:常用内建模块5：[Python学习笔记:   常用内建模块5](https://blog.csdn.net/lvsehaiyang1993/article/details/80716912)

Python学习笔记:常用内建模块6:[Python学习笔记：常用内建模块6](https://blog.csdn.net/lvsehaiyang1993/article/details/80717102)

Python学习笔记:第三方模块1：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726046)

Python学习笔记:第三方模块2：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726301)

Python学习笔记:第三方模块3：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726378)

Pytho学习笔记：网络编程：[Python网络编程](https://blog.csdn.net/lvsehaiyang1993/article/details/80729939)

Python学习笔记：电子邮件：[Python电子邮件1](https://blog.csdn.net/lvsehaiyang1993/article/details/80731086)

Python学习笔记：SMTP服务器：[PythonSMTP服务器](https://blog.csdn.net/lvsehaiyang1993/article/details/80731573)

Python学习笔记：POP3服务器：[PythonPOP3服务器](https://blog.csdn.net/lvsehaiyang1993/article/details/80731697)

Python学习笔记：Python数据库 [Python数据库1](https://blog.csdn.net/lvsehaiyang1993/article/details/80742388)

Python学习笔记：Python数据库2[Python数据库2](https://blog.csdn.net/lvsehaiyang1993/article/details/80747424)

Python学习笔记：web开发1[Python学习笔记：web开发1](https://blog.csdn.net/lvsehaiyang1993/article/details/80765955)

Python学习笔记：web开发2[Python学习笔记: web开发2](https://blog.csdn.net/lvsehaiyang1993/article/details/80778507)

Python学习笔记:  web开发3[Python学习笔记: web开发3](https://blog.csdn.net/lvsehaiyang1993/article/details/80779521)

Python学习笔记：异步IO（1）[Python学习笔记：异步IO（1）](https://blog.csdn.net/lvsehaiyang1993/article/details/80789370)

Python学习笔记：异步IO（2）[Python学习笔记：异步IO（2）](https://blog.csdn.net/lvsehaiyang1993/article/details/80789574)

Python学习笔记：异步IO（3）[Python学习笔记：异步IO（3）](https://blog.csdn.net/lvsehaiyang1993/article/details/80795526)

Python学习笔记：Day 1-2开发[Python学习笔记：Day1-2开发](https://blog.csdn.net/lvsehaiyang1993/article/details/80807565)

Python学习笔记：Day 3 ORM[Python学习笔记：Day3 ORM](https://blog.csdn.net/lvsehaiyang1993/article/details/80842307)

Python学习笔记：Day 4 Model[Python学习笔记：Day4Model](https://blog.csdn.net/lvsehaiyang1993/article/details/80845475)

Python学习笔记：Day 5 web框架[PYTHON学习笔记：DAy5](https://blog.csdn.net/lvsehaiyang1993/article/details/80866237)

Python学习笔记：Day 6 配置文件[Python学习笔记：Day 6 配置文件](https://blog.csdn.net/lvsehaiyang1993/article/details/80875144)

Python学习笔记：Day 7 编写MVC[Python学习笔记：Day 7编写MVC](https://blog.csdn.net/lvsehaiyang1993/article/details/80880125)

Python学习笔记：Day 8 构建前端[Python学习笔记:Day 8 构建前端](https://blog.csdn.net/lvsehaiyang1993/article/details/80899296)

Python学习笔记：Day 9 编写API[Python 9编写API](https://blog.csdn.net/lvsehaiyang1993/article/details/80914717)

Python学习笔记：Day 10 用户注册和登陆[Python 10用户注册和登陆](https://blog.csdn.net/lvsehaiyang1993/article/details/80921837)

Python学习笔记：Day11 编写日志创建页[day11 编写目录](https://blog.csdn.net/lvsehaiyang1993/article/details/80935496)

Python学习笔记：Day12 编写日志列表页[day12 编写日志](https://blog.csdn.net/lvsehaiyang1993/article/details/80949143)
# 目录


- [前言](#前言)
- [目录](#目录)
- [提升开发效率](#提升开发效率)



# 提升开发效率

现在，我们已经把一个Web App的框架完全搭建好了，从后端的API到前端的MVVM，流程已经跑通了。 

在继续工作前，注意到每次修改Python代码，都必须在命令行先Ctrl-C停止服务器，再重启，改动才能生效。 

在开发阶段，每天都要修改、保存几十次代码，每次保存都手动来这么一下非常麻烦，严重地降低了我们的开发效率。有没有办法让服务器检测到代码修改后自动重新加载呢？ 

Django的开发环境在Debug模式下就可以做到自动重新加载，如果我们编写的服务器也能实现这个功能，就能大大提升开发效率。 

可惜的是，Django没把这个功能独立出来，不用Django就享受不到，怎么办？
其实Python本身提供了重新载入模块的功能，但不是所有模块都能被重新载入。另一种思路是检测www目录下的代码改动，一旦有改动，就自动重启服务器。

按照这个思路，我们可以编写一个辅助程序pymonitor.py，让它启动wsgiapp.py，并时刻监控www目录下的代码改动，有改动时，先把当前wsgiapp.py进程杀掉，再重启，就完成了服务器进程的自动重启。

要监控目录文件的变化，我们也无需自己手动定时扫描，Python的第三方库watchdog可以利用操作系统的API来监控目录文件的变化，并发送通知。我们先用pip安装：
`$ pip3 install watchdog`
利用watchdog接收文件变化的通知，如果是.py文件，就自动重启wsgiapp.py进程。

利用Python自带的subprocess实现进程的启动和终止，并把输入输出重定向到当前进程的输入输出中：

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

__author__ = 'Mike Li'

import os, sys, time, subprocess

from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

def log(s):
    print('[Monitor] %s' % s)

class MyFileSystemEventHander(FileSystemEventHandler):

    def __init__(self, fn):
        super(MyFileSystemEventHander, self).__init__()
        self.restart = fn

    def on_any_event(self, event):
        if event.src_path.endswith('.py'):
            log('Python source file changed: %s' % event.src_path)
            self.restart()

command = ['echo', 'ok']
process = None

def kill_process():
    global process
    if process:
        log('Kill process [%s]...' % process.pid)
        process.kill()
        process.wait()
        log('Process ended with code %s.' % process.returncode)
        process = None

def start_process():
    global process, command
    log('Start process %s...' % ' '.join(command))
    process = subprocess.Popen(command, stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr)

def restart_process():
    kill_process()
    start_process()

def start_watch(path, callback):
    observer = Observer()
    observer.schedule(MyFileSystemEventHander(restart_process), path, recursive=True)
    observer.start()
    log('Watching directory %s...' % path)
    start_process()
    try:
        while True:
            time.sleep(0.5)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()

if __name__ == '__main__':
    argv = sys.argv[1:]
    if not argv:
        print('Usage: ./pymonitor your-script.py')
        exit(0)
    if argv[0] != 'python3':
        argv.insert(0, 'python3')
    command = argv
    path = os.path.abspath('.')
    start_watch(path, None)
```

一共70行左右的代码，就实现了Debug模式的自动重新加载。用下面的命令启动服务器：
`$ python3 pymonitor.py wsgiapp.py`
或者给pymonitor.py加上可执行权限，启动服务器：
`$ ./pymonitor.py app.py`
在编辑器中打开一个.py文件，修改后保存，看看命令行输出，是不是自动重启了服务器：

```
$ ./pymonitor.py app.py 
[Monitor] Watching directory /Users/michael/Github/awesome-python3-webapp/www...
[Monitor] Start process python app.py...
...
INFO:root:application (/Users/michael/Github/awesome-python3-webapp/www) will start at 0.0.0.0:9000...
[Monitor] Python source file changed: /Users/michael/Github/awesome-python-webapp/www/handlers.py
[Monitor] Kill process [2747]...
[Monitor] Process ended with code -9.
[Monitor] Start process python app.py...
...
INFO:root:application (/Users/michael/Github/awesome-python3-webapp/www) will start at 0.0.0.0:9000...
```

现在，只要一保存代码，就可以刷新浏览器看到效果，大大提升了开发效率。





