
# Python 学习入门（14）—— logging - 阳光岛主 - CSDN博客

2013年11月30日 08:18:47[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：6895个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)




### 1. logging介绍
Python的logging模块提供了通用的日志系统，可以方便第三方模块或者是应用使用。这个模块提供不同的日志级别，并可以采用不同的方式记录日志，比如文件，HTTP GET/POST，SMTP，Socket等，甚至可以自己实现具体的日志记录方式。
logging模块与log4j的机制是一样的，只是具体的实现细节不同。
模块提供logger，handler，filter，formatter：
1）logger：提供日志接口，供应用代码使用。logger最长用的操作有两类：配置和发送日志消息。可以通过logging.getLogger(name)获取logger对象，如果不指定name则返回root对象，多次使用相同的name调用getLogger方法返回同一个logger对象。
2）handler：将日志记录（log record）发送到合适的目的地（destination），比如文件，socket等。一个logger对象可以通过addHandler方法添加0到多个handler，每个handler又可以定义不同日志级别，以实现日志分级过滤显示。
3）filter：提供一种优雅的方式决定一个日志记录是否发送到handler。
4）formatter：指定日志记录输出的具体格式。formatter的构造方法需要两个参数：消息的格式字符串和日期字符串，这两个参数都是可选的。
与log4j类似，logger，handler和日志消息的调用可以有具体的日志级别（Level），只有在日志消息的级别大于logger和handler的级别。

**RotatingFileHandler：**

```python
#!/usr/bin/python
# -*- coding:utf-8 -*-
#
# ithomer.net
import logging
import logging.handlers
LOG_FILE = 'tst.log'
handler = logging.handlers.RotatingFileHandler(LOG_FILE, maxBytes = 1024*1024, backupCount=5)
fmt = '%(asctime)s - %(filename)s:%(lineno)s - %(name)s - %(levelname)s - %(message)s'
formatter = logging.Formatter(fmt)
handler.setFormatter(formatter)
logger = logging.getLogger('tst')
logger.addHandler(handler)
logger.setLevel(logging.DEBUG)
logger.info('info msg')
logger.debug('debug msg')
```
运行结果（tst.log日志文件内容）：
2013-11-28 21:19:25,442 - logtest3.py:21 - tst - INFO - info msg
2013-11-28 21:19:25,442 - logtest3.py:22 - tst - DEBUG - debug msg

**TimedRotatingFileHandler：**

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
import logging
import logging.handlers
def main():
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    
    handler = logging.handlers.TimedRotatingFileHandler("test.log", 'D')
    # fmt = logging.Formatter("%(asctime)s - %(pathname)s - %(filename)s - %(funcName)s - %(lineno)s - %(levelname)s - %(message)s", "%Y-%m-%d %H:%M:%S")
    fmt = logging.Formatter("%(asctime)s - %(pathname)s - %(filename)s - %(funcName)s - %(lineno)s - %(levelname)s - %(message)s")
    handler.setFormatter(fmt)
    logger.addHandler(handler)
    
    logger.debug("debug msg")
    logger.info("info msg")
    logger.warn("warn msg")
    logger.warning("warning msg")
    logger.error("error msg")
    logger.fatal("fatal msg")
    logger.critical("critical msg")
if __name__ == "__main__":
    main()
```
运行结果：

```python
2013-12-31 17:39:07,093 - /home/homer/workspace/myPython/com/mylogging.py - mylogging.py - main - 17 - DEBUG - debug msg
2013-12-31 17:39:07,093 - /home/homer/workspace/myPython/com/mylogging.py - mylogging.py - main - 18 - INFO - info msg
2013-12-31 17:39:07,093 - /home/homer/workspace/myPython/com/mylogging.py - mylogging.py - main - 19 - WARNING - warn msg
2013-12-31 17:39:07,093 - /home/homer/workspace/myPython/com/mylogging.py - mylogging.py - main - 20 - WARNING - warning msg
2013-12-31 17:39:07,093 - /home/homer/workspace/myPython/com/mylogging.py - mylogging.py - main - 21 - ERROR - error msg
2013-12-31 17:39:07,093 - /home/homer/workspace/myPython/com/mylogging.py - mylogging.py - main - 22 - CRITICAL - fatal msg
2013-12-31 17:39:07,093 - /home/homer/workspace/myPython/com/mylogging.py - mylogging.py - main - 23 - CRITICAL - critical msg
```

关于formatter的配置，采用的是%(<dict key>)s的形式，就是字典的关键字替换。提供的关键字包括：

|Format|Description|
|---|---|
|%(name)s|Name of the logger (logging channel).|
|%(levelno)s|Numeric logging level for the message (|DEBUG|,|INFO|,|WARNING|,|ERROR|,|CRITICAL|).|
|%(levelname)s|Text logging level for the message (|'DEBUG'|,|'INFO'|,|'WARNING'|,|'ERROR'|,|'CRITICAL'|).|
|%(pathname)s|Full pathname of the source file where the logging call was issued (if available).|
|%(filename)s|Filename portion of pathname.|
|%(module)s|Module (name portion of filename).|
|%(funcName)s|Name of function containing the logging call.|
|%(lineno)d|Source line number where the logging call was issued (if available).|
|%(created)f|Time when the|LogRecord|was created (as returned by|time.time()|).|
|%(relativeCreated)d|Time in milliseconds when the LogRecord was created, relative to the time the logging module was loaded.|
|%(asctime)s|Human-readable time when the|LogRecord|was created. By default this is of the form “2003-07-08 16:49:45,896” (the numbers after the comma are millisecond portion of the time).|
|%(msecs)d|Millisecond portion of the time when the|LogRecord|was created.|
|%(thread)d|Thread ID (if available).|
|%(threadName)s|Thread name (if available).|
|%(process)d|Process ID (if available).|
|%(message)s|The logged message, computed as|msg|%|args|.|



### 2. logging的配置
logging的配置可以采用python代码或是配置文件。python代码的方式就是在应用的主模块中，构建handler，handler，formatter等对象。而配置文件的方式是将这些对象的依赖关系分离出来放在文件中。比如前面的例子就类似于python代码的配置方式。这里看一下采用配置文件的方式。

```python
#!/usr/bin/python
# -*- coding:utf-8 -*-
#
# ithomer.net
import logging
import logging.config
LOG_FILE = 'tst.log'
logging.config.fileConfig('logging.conf')
# instance of logging
logger = logging.getLogger('tst')
logger.debug('debug msg')
logger.info('info msg')
logger.warn('warn msg')
logger.error('error msg')
logger.critical('critical msg')
```
loggin.conf采用了模式匹配的方式进行配置，正则表达式是r'^[(.*)]$'，从而匹配出所有的组件。对于同一个组件具有多个实例的情况使用逗号‘，’进行分隔。对于一个实例的配置采用componentName_instanceName配置块。使用这种方式还是蛮简单的。

```python
[loggers]
keys=root,tst
[handlers]
keys=consoleHandler
[formatters]
keys=tstFormatter
[logger_root]
level=DEBUG
handlers=consoleHandler
[logger_tst]
level=DEBUG
handlers=consoleHandler
qualname=tst
propagate=0
[handler_consoleHandler]
class=StreamHandler
level=DEBUG
formatter=tstFormatter
args=(sys.stdout,)
[formatter_tstFormatter]
format=%(asctime)s - %(filename)s:%(lineno)s - %(name)s - %(levelname)s - %(message)s
datefmt=
```
在指定handler的配置时，class是具体的handler类的类名，可以是相对logging模块或是全路径类名，比如需要RotatingFileHandler，则class的值可以为：RotatingFileHandler或者logging.handlers.RotatingFileHandler。args就是要传给这个类的构造方法的参数，就是一个元组，按照构造方法声明的参数的顺序。
运行结果，控制台输出：
2013-11-28 23:02:32,574 - logtest4.py:16 - tst - DEBUG - debug msg
2013-11-2823:02:32,574 - logtest4.py:17 - tst - INFO - info msg
2013-11-2823:02:32,574 - logtest4.py:18 - tst - WARNING - warn msg
2013-11-2823:02:32,574 - logtest4.py:19 - tst - ERROR - error msg
2013-11-2823:02:32,574 - logtest4.py:20 - tst - CRITICAL - critical msg
这里还要明确一点，logger对象是有继承关系的，比如名为a.b和a.c的logger都是名为a的子logger，并且所有的logger对象都继承于root。如果子对象没有添加handler等一些配置，会从父对象那继承。这样就可以通过这种继承关系来复用配置。


### 3. 多模块使用logging
logging模块保证在同一个python解释器内，多次调用logging.getLogger('log_name')都会返回同一个logger实例，即使是在多个模块的情况下。所以典型的多模块场景下使用logging的方式是在main模块中配置logging，这个配置会作用于多个的子模块，然后在其他模块中直接通过getLogger获取Logger对象即可。
这里使用上面配置文件：

```python
[loggers]
keys=root,main
[handlers]
keys=consoleHandler,fileHandler
[formatters]
keys=fmt
[logger_root]
level=DEBUG
handlers=consoleHandler
[logger_main]
level=DEBUG
handlers=fileHandler
qualname=main
#propagate=0
[handler_consoleHandler]
class=StreamHandler
level=DEBUG
formatter=fmt
args=(sys.stdout,)
[handler_fileHandler]
class=logging.handlers.RotatingFileHandler
level=DEBUG
formatter=fmt
args=('tst.log','a',20000,5,)
[formatter_fmt]
format=%(asctime)s - %(filename)s:%(lineno)s - %(name)s - %(levelname)s - %(message)s
datefmt=
```
主模块main.py：

```python
#!/usr/bin/python
# -*- coding:utf-8 -*-
#
# main.py
#
# ithomer.net
import logging
import logging.config

logging.config.fileConfig('logging.conf')
# root
logger_root = logging.getLogger('root')
logger_root.debug('debug root')
# main
logger_main = logging.getLogger('main')
logger_main.info('info main')
logger_main.info('start import module \'mod\'...')
import mod
logger_main.debug('test mod.testLogger()')
mod.testLogger()
logger_root.info('the end!')
```
子模块mod.py：

```python
#!/usr/bin/python
# -*- coding:utf-8 -*-
#
# mod.py
#
# ithomer.net
import logging
logger = logging.getLogger('main.mod')
logger.info('info of main.mod')
def testLogger():
    logger.debug('this is main.mod testLogging()')
    logger.info('start import module \'submod\'...')
    import submod
    submod.testLogger()
```
子子模块submod.py：

```python
#!/usr/bin/python
# -*- coding:utf-8 -*-
#
# submod.py
#
# ithomer.net
import logging
logger = logging.getLogger('main.mod.submod')
logger.info('info of main.mod.submod')
def testLogger():
    logger.debug('this is main.mod.submod testLogging()')
```
然后运行python main.py，控制台输出：

```python
2013-11-28 16:58:15,517 - main.py:16 - root - DEBUG - debug root
2013-11-28 16:58:15,517 - main.py:20 - main - INFO - info main
2013-11-28 16:58:15,517 - main.py:22 - main - INFO - start import module 'mod'...
2013-11-28 16:58:15,518 - mod.py:11 - main.mod - INFO - info of main.mod
2013-11-28 16:58:15,518 - main.py:24 - main - DEBUG - test mod.testLogger()
2013-11-28 16:58:15,518 - mod.py:14 - main.mod - DEBUG - this is main.mod testLogging()
2013-11-28 16:58:15,519 - mod.py:15 - main.mod - INFO - start import module 'submod'...
2013-11-28 16:58:15,519 - submod.py:11 - main.mod.submod - INFO - info of main.mod.submod
2013-11-28 16:58:15,519 - submod.py:14 - main.mod.submod - DEBUG - this is main.mod.submod testLogging()
2013-11-28 16:58:15,519 - main.py:27 - root - INFO - the end!
```
可以看出，和预想的一样，然后在看一下tst.log，logger配置中的输出的目的地：

```python
2013-11-28 16:58:15,517 - main.py:20 - main - INFO - info main
2013-11-28 16:58:15,517 - main.py:22 - main - INFO - start import module 'mod'...
2013-11-28 16:58:15,518 - mod.py:11 - main.mod - INFO - info of main.mod
2013-11-28 16:58:15,518 - main.py:24 - main - DEBUG - test mod.testLogger()
2013-11-28 16:58:15,518 - mod.py:14 - main.mod - DEBUG - this is main.mod testLogging()
2013-11-28 16:58:15,519 - mod.py:15 - main.mod - INFO - start import module 'submod'...
2013-11-28 16:58:15,519 - submod.py:11 - main.mod.submod - INFO - info of main.mod.submod
2013-11-28 16:58:15,519 - submod.py:14 - main.mod.submod - DEBUG - this is main.mod.submod testLogging()
```
tst.log中没有root logger输出的信息，因为logging.conf中配置了只有main logger及其子logger使用RotatingFileHandler，而root logger是输出到标准输出。



**1、从一个使用场景开始**
开发一个日志系统， 既要把日志输出到控制台， 还要写入日志文件

```python
#!/usr/bin/python
# ithomer in 2013
import logging
# create instance of logging
logger = logging.getLogger('mylogger')
logger.setLevel(logging.DEBUG)

# file handler
fh = logging.FileHandler('test.log')
fh.setLevel(logging.DEBUG)

# console handler
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)

# formatter
fmt = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(fmt)
ch.setFormatter(fmt)

# add formatter to handler
logger.addHandler(fh)
logger.addHandler(ch)

# print a log
logger.info('ithomer')
```
运行后， 在控制台和日志文件（test.log）都有一条日志：
2013-11-27 23:45:20,329 - mylogger - INFO - ithomer

**2、logging模块的API**
结合上面的例子，我们说下几个最常使用的API
**logging.****getLogger****(****[****name****]****)**返回一个logger实例，如果没有指定name，返回root logger。只要name相同，返回的logger实例都是同一个而且只有一个，即name和logger实例是一一对应的。这意味着，无需把logger实例在各个模块中传递。只要知道name，就能得到同一个logger实例
**Logger.****setLevel****(lvl)**设置logger的level， level有以下几个级别：>>> import logging
>>> print logging.NOTSET
0
>>> print logging.DEBUG
10
>>> print logging.INFO
20
>>> print logging.WARNING
30
>>> print logging.ERROR
40
>>> print logging.CRITICAL
50NOTSET < DEBUG < INFO < WARNING < ERROR < CRITICAL
如果把looger的级别设置为INFO， 那么小于INFO级别的日志都不输出， 大于等于INFO级别的日志都输出
logger.debug("foobar")        \# 不输出
logger.info("foobar")            \# 输出
logger.warning("foobar")    \# 输出
logger.error("foobar")          \# 输出
logger.critical("foobar")       \# 输出
**Logger.****addHandler****(hdlr)**logger可以雇佣handler来帮它处理日志， handler主要有以下几种：StreamHandler\# 输出到控制台FileHandler\# 输出到文件handler还可以设置自己的level以及输出格式。
**logging.****basicConfig****(****[******kwargs****]****)**这个函数用来配置root logger， 为root logger创建一个StreamHandler，设置默认的格式。这些函数： logging.debug()、logging.info()、logging.warning()、logging.error()、logging.critical() 如果调用的时候发现root logger没有任何 handler， 会自动调 basicConfig添加一个handler， 如果root logger已有handler， 这个函数不做任何事情，使用basicConfig来配置root logger的输出格式和level：import logging
logging.basicConfig(format='%(levelname)s:%(message)s', level=logging.DEBUG)
logging.debug('This message should appear on the console')

**3、root logger与logger 父子关系**
前面多次提到root logger， 实际上logger实例之间还有父子关系， root logger就是处于最顶层的logger， 它是所有logger的祖先。如下图:![](https://img-blog.csdn.net/20131127203103937)
root logger是默认的logger，如果不创建logger实例， 直接调用logging.debug()、logging.info()、logging.warning()、logging.error()、logging.critical()这些函数，那么使用的logger就是 root logger， 它可以自动创建，也是单实例的。
如何得到root logger通过logging.getLogger()或者logging.getLogger("")得到root logger实例。
默认的levelroot logger默认的level是logging.WARNING
如何表示父子关系logger的name的命名方式可以表示logger之间的父子关系. 比如：parent_logger = logging.getLogger('foo')child_logger = logging.getLogger('foo.bar')
什么是effective levellogger有一个概念，effective level。 如果一个logger没有显示地设置level，那么它就用父亲的level。如果父亲也没有显示地设置level， 就用父亲的父亲的level，以此推....最后到达root logger，一定设置过level。默认为logging.WARNING，child loggers得到消息后，既把消息分发给它的handler处理，也会传递给所有祖先logger处理，来看一个例子：

```python
#/usr/bin/python
# ithomer in 2013
import logging
# root logger
root = logging.getLogger()      # root
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
ch.setFormatter(formatter)
root.addHandler(ch)

# logging as parent
p = logging.getLogger('foo')
p.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(message)s')
ch.setFormatter(formatter)
p.addHandler(ch)

# logging as child
c = logging.getLogger('foo.bar')
c.debug('foo')
```
运行结果：
2013-11-27 22:40:56,699 - foo
2013-11-27 22:40:56,699 - DEBUG - foo

可见， 孩子logger没有任何handler，所以对消息不做处理，但是它把消息转发给了它的父亲以及root logger，最后输出两条日志，分别是p和root 输出的logging。

**参考推荐：**
[python的logging模块](http://kenby.iteye.com/blog/1162698)
[Python日志系统Logging](http://www.52ij.com/jishu/666.html)
[python 日志模块 logging 详解](http://my.oschina.net/leejun2005/blog/126713)
[Python日志输出——logging模块](http://blog.csdn.net/chosen0ne/article/details/7319306)


