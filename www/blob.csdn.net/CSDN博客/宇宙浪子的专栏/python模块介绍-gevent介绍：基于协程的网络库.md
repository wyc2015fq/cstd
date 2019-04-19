# python模块介绍-gevent介绍：基于协程的网络库 - 宇宙浪子的专栏 - CSDN博客
2015年07月21日 11:12:35[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：1344
本文转自：[http://my.oschina.net/u/1433482/blog/192562?p=1](http://my.oschina.net/u/1433482/blog/192562?p=1)
# 原 python模块介绍-gevent介绍：基于协程的网络库
*赞3*
*摘要*gevent是基于协程的Python网络库。特点：
 基于libev的快速事件循环(Linux上epoll，FreeBSD上kqueue）。 基于greenlet的轻量级执行单元。 API的概念和Python标准库一致(如事件，队列)。 可以配合socket，ssl模块使用。 能够使用标准库和第三方模块创建标准的阻塞套接字(gevent.monkey)。 默认通过线程池进行DNS查询,也可通过c-are(通过GEVENT_RESOLVER=ares环境变量开启）。 TCP/UDP/HTTP服务器 子进程支持（通过geven...
[python](http://www.oschina.net/search?scope=blog&q=python)[模块](http://www.oschina.net/search?scope=blog&q=%E6%A8%A1%E5%9D%97)[gevent](http://www.oschina.net/search?scope=blog&q=gevent)[协程](http://www.oschina.net/search?scope=blog&q=%E5%8D%8F%E7%A8%8B)[网络库](http://www.oschina.net/search?scope=blog&q=%E7%BD%91%E7%BB%9C%E5%BA%93)
*目录[-]*
- [python模块介绍-gevent介绍：基于协程的网络库](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h1_1)
- [介绍](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h2_2)
- [安装](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h3_3)
- [实例](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h3_4)
- [灵猴补丁(Monkey
 patching)](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h3_5)
- [事件循环](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h3_6)
- [多任务合作](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h2_7)
- [轻量级伪线程](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h2_8)
- [超时](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h3_9)
- [了解更多](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h3_10)
- [外部资源](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h3_11)
- [本文地址](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h2_12)
- [参考资料](http://my.oschina.net/u/1433482/blog/192562?p=1#OSC_h2_13)
# [python模块介绍-gevent介绍：基于协程的网络库](http://automationtesting.sinaapp.com/blog/m_gevent)
## 介绍
gevent是基于协程的Python网络库。特点：
- 
基于libev的快速事件循环(Linux上epoll，FreeBSD上kqueue）。
- 
基于greenlet的轻量级执行单元。
- 
API的概念和Python标准库一致(如事件，队列)。
- 
可以配合socket，ssl模块使用。
- 
能够使用标准库和第三方模块创建标准的阻塞套接字(gevent.monkey)。
- 
默认通过线程池进行DNS查询,也可通过c-are(通过GEVENT_RESOLVER=ares环境变量开启）。
- 
TCP/UDP/HTTP服务器
- 
子进程支持（通过gevent.subprocess）
- 
线程池
### 安装
gevent目前支持python2.5-2.7，python2.6以前的版本如果要使用ssl还需要安装ssl模块。
```
```python
# pip install gevent
```
```
### 实例
下面的示例展示了如何同时运行任务。
```
```python
>>>
```
```python
import
```
```python
gevent>>>
```
```python
from
```
```python
gevent
```
```python
import
```
```python
socket>>> urls
```
```python
=
```
```python
[
```
```python
'www.google.com'
```
```python
,
```
```python
'www.example.com'
```
```python
,
```
```python
'www.python.org'
```
```python
]>>> jobs
```
```python
=
```
```python
[gevent.spawn(socket.gethostbyname, url)
```
```python
for
```
```python
url
```
```python
in
```
```python
urls]>>> gevent.joinall(jobs, timeout
```
```python
=
```
```python
2
```
```python
)>>> [job.value
```
```python
for
```
```python
job
```
```python
in
```
```python
jobs][
```
```python
'74.125.128.106'
```
```python
,
```
```python
'93.184.216.119'
```
```python
,
```
```python
'82.94.164.162'
```
```python
]
```
```
job发起之后，gevent.joinall()等待完成，不超过2秒。结果收集在gevent.Greenlet.value属性。 gevent.socket.gethostbyname()和socket.gethostbyname()的接口一样，但它并不阻塞解释器，其他 greenlet继续畅通无阻的处理请求。
### 灵猴补丁(Monkey patching)
上面例子使gevent.socket进行socket操作。如果使用标准socket模块将有3倍耗时，因为DNS请求是串行的。在greenlet中使用标准socket模块毫无意义，这些模块和包是怎么建立在socket之上的？
monkey patching这时起作用了，gevent.monkey小心地使用兼容副本替换标准socket模块的函数和类。这样，即使是不知道gevent的模块也受益于greenlet环境运行。
```
```python
>>>
```
```python
from
```
```python
gevent
```
```python
import
```
```python
monkey; monkey.patch_socket()>>>
```
```python
import
```
```python
urllib2
```
```python
# it's usable from multiple greenlets now
```
```
下面是使用urllib2进行下载的实例：
```
```python
#!/usr/bin/env python# -*- coding: utf-8 -*-# Copyright (c) 2009 Denis Bilenko. See LICENSE for details."""Spawn multiple workers and wait for them to complete"""urls = ['[http://www.google.com](http://www.google.com/)', '[http://www.yandex.ru](http://www.yandex.ru/)', '[http://www.python.org](http://www.python.org/)']import geventfrom gevent import monkey# patches stdlib (including socket and ssl modules) to cooperate with other greenletsmonkey.patch_all()import urllib2def print_head(url):
```
```python
```
```python
print
```
```python
(
```
```python
'Starting %s'
```
```python
%
```
```python
url)
```
```python
```
```python
data
```
```python
=
```
```python
urllib2.urlopen(url).read()
```
```python
```
```python
print
```
```python
(
```
```python
'%s: %s bytes: %r'
```
```python
%
```
```python
(url,
```
```python
len
```
```python
(data), data[:
```
```python
50
```
```python
]))jobs
```
```python
=
```
```python
[gevent.spawn(print_head, url)
```
```python
for
```
```python
url
```
```python
in
```
```python
urls]gevent.wait(jobs)
```
```
执行结果：
```
`# ./test.py `
`Starting http:``//www``.google.com`
`Starting http:``//www``.yandex.ru`
`Starting http:``//www``.python.org`
`http:``//www``.google.com: 11246 bytes: ``'<!doctype html><html itemscope="" itemtype="http:/'``http:``//www``.python.org: 20471 bytes: ``'<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Trans'``http:``//www``.yandex.ru: 208804 bytes: ``'<!DOCTYPE html><html class="i-ua_js_no i-ua_css_st'`
```
### 事件循环
不像其他网络库，gevent和eventlet类似， 在一个greenlet中隐式开始事件循环。没有必须调用run()或dispatch()的反应器(reactor)，在twisted中是有 reactor的。当gevent的API函数想阻塞时，它获得Hub实例(执行时间循环的greenlet),并切换过去。如果没有集线器实例则会动态 创建。
libev提供的事件循环默认使用系统最快轮询机制，设置LIBEV_FLAGS环境变量可指定轮询机制。LIBEV_FLAGS=1为select， LIBEV_FLAGS = 2为poll， LIBEV_FLAGS = 4为epoll,LIBEV_FLAGS = 8为kqueue。请阅读libev文档了解更多信息[http://pod.tst.eu/http://cvs.schmorp.de/libev/ev.pod#FUNCTIONS_CONTROLLING_EVENT_LOOPS](http://pod.tst.eu/http://cvs.schmorp.de/libev/ev.pod#FUNCTIONS_CONTROLLING_EVENT_LOOPS)。
Libev的API位于gevent.core下。注意libev API的回调在Hub的greenlet运行，因此使用同步greenlet的API。可以使用spawn()和Event.set()等异步API。
## 多任务合作
所有greenlets都在同一个操作系统线程调度执行。直到特定的greenlet放弃控制，（调用阻塞函数切换到Hub），其他greenlet才有 机会运行。对于I / O密集型应用程序这通常不是问题，但做CPU密集型或者调用封锁绕过libev事件循环的I/0功能的时会有问题。
一般不需要在greenlet之间同步访问共享对象， 所以Lock和Semaphore类尽管存在，但是很少使用。从线程和多处理等其他概念仍然常用，如下：
- 
Event：唤醒在调用Event.wait()方法的greenlets。
- 
AsyncResult:和Event类似，但允许传递值或异常。队列和JoinableQueue。
- 
Queu和JoinableQueue.
## 轻量级伪线程
greenlet通过创建greenlet实例并调用其start方法发起。(spawn()函数就是做这个的快捷方式)。 start方法给greenlet安排一个开关，当前greenlet放弃控制触发。如果有多个active的事件，将不确定的顺序一一执行。
如果在执行过程中出现错误，将无法离开greenlet的边界。未处理的错误导致打印堆栈跟踪及失败函数和参数：
```
```python
>>> gevent.spawn(
```
```python
lambda
```
```python
:
```
```python
1
```
```python
/
```
```python
0
```
```python
)>>> gevent.sleep(
```
```python
1
```
```python
)Traceback (most recent call last):
```
```python
```
```python
...ZeroDivisionError: integer division
```
```python
or
```
```python
modulo by zero<Greenlet at
```
```python
0x7f2ec3a4e490
```
```python
: <function <
```
```python
lambda
```
```python
...>> failed with ZeroDivisionError
```
```
traceback在greenlet退出时同步打印至sys.stderr。
Greenlet实例有如下有用的方法：
- 
join – waits until the greenlet exits;
- 
kill – interrupts greenlet’s execution;
- 
get – returns the value returned by greenlet or re-raised the exception that killed it.
继承Greenlet类重载其str可以自定义traceback后的字符串。另外还需要重载_run()方法以及在init中调用Greenlet.init(self)。
```
```python
class
```
```python
MyNoopGreenlet(Greenlet):
```
```python
```
```python
def
```
```python
__init__(
```
```python
self
```
```python
, seconds):
```
```python
```
```python
Greenlet.__init__(
```
```python
self
```
```python
)
```
```python
```
```python
self
```
```python
.seconds
```
```python
=
```
```python
seconds
```
```python
def
```
```python
_run(
```
```python
self
```
```python
):
```
```python
```
```python
gevent.sleep(
```
```python
self
```
```python
.seconds)
```
```python
```
```python
def
```
```python
__str__(
```
```python
self
```
```python
):
```
```python
```
```python
return
```
```python
'MyNoopGreenlet(%s)'
```
```python
%
```
```python
self
```
```python
.seconds
```
```
可以异步结束Greenlet,浙江将恢复等待的greenlet，不继续执行而是引发GreenletExit。
```
```python
>>> g
```
```python
=
```
```python
MyNoopGreenlet(
```
```python
4
```
```python
)>>> g.start()>>> g.kill()>>> g.deadTrue
```
```
GreenletExit及其子类的处理方式不同于其他异常。GreenletExit不被视为异常状态，不打印traceback。get可以获得GreenletExit是GET，就好像它是由greenlet返回，不是raise。
kill方法可以自定义的异常：
```
```python
>>> g
```
```python
=
```
```python
MyNoopGreenlet.spawn(
```
```python
5
```
```python
)
```
```python
# spawn() creates a Greenlet and starts it>>> g.kill(Exception("A time to kill"))Traceback (most recent call last):
```
```python
```
```python
...Exception: A time to kill
```
```python
MyNoopGreenlet(
```
```python
5
```
```python
) failed with Exception
```
```
kill还可以接受timeout参数指定greenlet退的等待秒数。注意，kill不能保证目标greenlet不会忽视该异常，因此给kill传递timeout是个好方法。
### 超时
gevent的API中的许多函数是同步的，阻塞当前greenlet直到操作完成。例如，kill会等到greenlet结束。多数可以传递参数block=False异步执行。
此外，许多同步函数接受超时参数，指定可以阻塞多久(比如：Event.wait(), Greenlet.join(), Greenlet.kill(),[AsyncResult?](http://automationtesting.sinaapp.com/wiki/AsyncResult).get()等)。
socket和SSLObject实例也可以超时，由setTimeout方法设置。
如果这些还不够用，Timeout类可以给任意（yielding）代码块增加超时。
### 了解更多
限制并发可以使用Pool类（参见实例： dns_mass_resolve.py）
gevent自带的TCP/SSL/HTTP/WSGI服务器。参见实现服务器部分[http://www.gevent.org/servers.html](http://www.gevent.org/servers.html)。
### 外部资源
gevent For the Working Python Developer： 是一个更全面的教程。[http://sdiehl.github.io/gevent-tutorial/](http://sdiehl.github.io/gevent-tutorial/).
 中文版本参见[http://xlambda.com/gevent-tutorial/](http://xlambda.com/gevent-tutorial/)
## 本文地址
- 
[http://automationtesting.sinaapp.com/blog/m_gevent](http://automationtesting.sinaapp.com/blog/m_gevent)
- 
本站地址：python自动化测试[http://automationtesting.sinaapp.com](http://automationtesting.sinaapp.com/)python开发自动化测试群113938272和开发测试群6089740
 微博[http://weibo.com/cizhenshi](http://weibo.com/cizhenshi)
## 参考资料
- 
gevent主页：[http://www.gevent.org/](http://www.gevent.org/)
- 
gevent英文文档:[http://www.gevent.org/intro.html](http://www.gevent.org/intro.html)
- 
gevent程序员指南:[http://xlambda.com/gevent-tutorial/](http://xlambda.com/gevent-tutorial/)
- 
下载地址：[https://pypi.python.org/pypi/gevent](https://pypi.python.org/pypi/gevent)
