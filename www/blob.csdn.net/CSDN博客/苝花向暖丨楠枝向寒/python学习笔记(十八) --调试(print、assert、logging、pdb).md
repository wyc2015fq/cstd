# python学习笔记(十八)  --调试(print、assert、logging、pdb) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月24日 21:43:33[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：144
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## 一：print

代码运行出错，我们可以使用print输出语句来查看变量的值，从而找到问题点，但这种方式比较麻烦，我们或许需要在多个地方加上print语句才能准确的找出问题所在。并且当我们改正错误以后还需要将print删除，否则影响代码的美观。

## 二：assert 断言

凡是调试时用到print的地方我们都可以换成 assert

如下

```python
def foo(s):
    n = int(s)
    assert n != 0, 'n is zero!' #如果n不等于0会抛出异常，并显示逗号后面的字符串
    return 10 / n

if __name__ == '__main__':
	foo('0')
```

```
C:\Users\Administrator\Desktop>python err.py
Traceback (most recent call last):
  File "err.py", line 7, in <module>
    foo('0')
  File "err.py", line 3, in foo
    assert n != 0, 'n is zero!'
AssertionError: n is zero!
```

我们可以看出，使用断言的地方，如果 assert 后面的结果为flase，则会抛出一个AssertionError异常。

当我们将bug改掉以后，可以使用 python -O 来执行脚本，从而屏蔽掉assert。

注意 是大写的 字母 O

```
C:\Users\Administrator\Desktop>python -O err.py
Traceback (most recent call last):
  File "err.py", line 7, in <module>
    foo('0')
  File "err.py", line 4, in foo
    return 10 / n
ZeroDivisionError: division by zero
```

## 三：logging

和assert相比，logging不会抛出异常，且可以输出到文件

**日志分为如下几个等级**
|日志等级（level）|描述|
|----|----|
|DEBUG|最详细的日志信息，典型应用场景是 问题诊断|
|INFO|信息详细程度仅次于DEBUG，通常只记录关键节点信息，用于确认一切都是按照我们预期的那样进行工作|
|WARNING|当某些不期望的事情发生时记录的信息（如，磁盘可用空间较低），但是此时应用程序还是正常运行的|
|ERROR|由于一个更严重的问题导致某些功能不能正常运行时记录的信息|
|CRITICAL|当发生严重错误，导致应用程序不能继续运行时记录的信息|

**日志可使用的函数 **
|函数|说明|
|----|----|
|logging.debug(msg, *args, **kwargs)|创建一条严重级别为DEBUG的日志记录|
|logging.info(msg, *args, **kwargs)|创建一条严重级别为INFO的日志记录|
|logging.warning(msg, *args, **kwargs)|创建一条严重级别为WARNING的日志记录|
|logging.error(msg, *args, **kwargs)|创建一条严重级别为ERROR的日志记录|
|logging.critical(msg, *args, **kwargs)|创建一条严重级别为CRITICAL的日志记录|
|logging.log(level, *args, **kwargs)|创建一条严重级别为level的日志记录|
|logging.basicConfig(**kwargs)|对root logger进行一次性配置|

**logging.basicConfig()函数包含参数说明**
|参数名称|描述|
|----|----|
|filename|指定日志输出目标文件的文件名（可以写文件名也可以写文件的完整的绝对路径，写文件名日志放执行文件目录下，写完整路径按照完整路径生成日志文件），指定该设置项后日志信心就不会被输出到控制台了|
|filemode|指定日志文件的打开模式，默认为'a'。需要注意的是，该选项要在filename指定时才有效|
|format|指定日志格式字符串，即指定日志输出时所包含的字段信息以及它们的顺序。logging模块定义的格式字段下面会列出。|
|datefmt|指定日期/时间格式。需要注意的是，该选项要在format中包含时间字段%(asctime)s时才有效|
|level|指定日志器的日志级别|
|stream|指定日志输出目标stream，如sys.stdout、sys.stderr以及网络stream。需要说明的是，stream和filename不能同时提供，否则会引发 `ValueError`异常|
|style|Python 3.2中新添加的配置项。指定format格式字符串的风格，可取值为'%'、'{'和'$'，默认为'%'|
|handlers|Python 3.3中新添加的配置项。该选项如果被指定，它应该是一个创建了多个Handler的可迭代对象，这些handler将会被添加到root logger。需要说明的是：filename、stream和handlers这三个配置项只能有一个存在，不能同时出现2个或3个，否则会引发ValueError异常。|

**logging模块中定义好的可以用于format格式字符串说明**
|字段/属性名称|使用格式|描述|
|----|----|----|
|asctime|%(asctime)s|将日志的时间构造成可读的形式，默认情况下是‘2016-02-08 12:00:00,123’精确到毫秒|
|name|%(name)s|所使用的日志器名称，默认是'root'，因为默认使用的是 rootLogger|
|filename|%(filename)s|调用日志输出函数的模块的文件名； pathname的文件名部分，包含文件后缀|
|funcName|%(funcName)s|由哪个function发出的log， 调用日志输出函数的函数名|
|levelname|%(levelname)s|日志的最终等级（被filter修改后的）|
|message|%(message)s|日志信息， 日志记录的文本内容|
|lineno|%(lineno)d|当前日志的行号， 调用日志输出函数的语句所在的代码行|
|levelno|%(levelno)s|该日志记录的数字形式的日志级别（10, 20, 30, 40, 50）|
|pathname|%(pathname)s|完整路径 ，调用日志输出函数的模块的完整路径名，可能没有|
|process|%(process)s|当前进程， 进程ID。可能没有|
|processName|%(processName)s|进程名称，Python 3.1新增|
|thread|%(thread)s|当前线程， 线程ID。可能没有|
|threadName|%(thread)s|线程名称|
|module|%(module)s|调用日志输出函数的模块名， filename的名称部分，不包含后缀即不包含文件后缀的文件名|
|created|%(created)f|当前时间，用UNIX标准的表示时间的浮点数表示； 日志事件发生的时间--时间戳，就是当时调用time.time()函数返回的值|
|relativeCreated|%(relativeCreated)d|输出日志信息时的，自Logger创建以 来的毫秒数； 日志事件发生的时间相对于logging模块加载时间的相对毫秒数|
|msecs|%(msecs)d|日志事件发生事件的毫秒部分。logging.basicConfig()中用了参数datefmt，将会去掉asctime中产生的毫秒部分，可以用这个加上|

**logging模块中定义好的可以用于datefmt格式字符串说明（与time相同）**
|格式|含义|备注|
|----|----|----|
|%a|本地（locale）简化星期名称||
|%A|本地完整星期名称||
|%b|本地简化月份名称||
|%B|本地完整月份名称||
|%c|本地相应的日期和时间表示||
|%d|一个月中的第几天（01 - 31）||
|%H|一天中的第几个小时（24小时制，00 - 23）||
|%I|第几个小时（12小时制，01 - 12）||
|%j|一年中的第几天（001 - 366）||
|%m|月份（01 - 12）||
|%M|分钟数（00 - 59）||
|%p|本地am或者pm的相应符|一|
|%S|秒（01 - 61）|二|
|%U|一年中的星期数。（00 - 53星期天是一个星期的开始。）第一个星期天之前的所有天数都放在第0周。|三|
|%w|一个星期中的第几天（0 - 6，0是星期天）|三|
|%W|和%U基本相同，不同的是%W以星期一为一个星期的开始。||
|%x|本地相应日期||
|%X|本地相应时间||
|%y|去掉世纪的年份（00 - 99）||
|%Y|完整的年份||
|%Z|时区的名字（如果不存在为空字符）||
|%%|‘%’字符||

**具体使用方式：**

```python
import logging
logging.basicConfig(level=logging.DEBUG,    #设置日志级别
                    format="%(asctime)s %(name)s %(levelname)s %(message)s", #设置日志输出的内容
                    datefmt = '%Y-%m-%d  %H:%M:%S %a' # 设置日志输出的日期格式   
                    )
logging.debug("msg1")
logging.info("msg2")
logging.warning("msg3")
logging.error("msg4")
logging.critical("msg5")
```

** 升级版例子：**

```python
import logging
LOG_FORMAT = "%(asctime)s %(name)s %(levelname)s %(pathname)s %(message)s "#配置输出日志格式
DATE_FORMAT = '%Y-%m-%d  %H:%M:%S %a ' #配置输出时间的格式，注意月份和天数不要搞乱了
logging.basicConfig(level=logging.DEBUG,
                    format=LOG_FORMAT,
                    datefmt = DATE_FORMAT ,
                    filename=r"d:\test.log" #有了filename参数就不会直接输出显示到控制台，而是直接写入文件
                    )
logging.debug("msg1")
logging.info("msg2")
logging.warning("msg3")
logging.error("msg4")
logging.critical("msg5")
```

![](https://img-blog.csdn.net/20180824211510231?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180824211610774?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##  四：pdb

pdb其实就是用于python的命令行模式，如果你使用一些IDE工具，其中的断点调试比这个用的爽快的多，但是了解一下还是有必要的。

下面是pdb的命令的相关参数。
|命令|解释|
|----|----|
|设置断点| |
|继续执行程序| |
|查看当前行的代码段| |
|进入函数| |
|执行代码直到从当前函数返回| |
|中止并退出| |
|执行下一行| |
|打印变量的值| |
|帮助| |

 使用

编写脚本

```python
# err.py
s = '0'
n = int(s)
print(10 / n)
```

 使用 python -m pdb 命令执行脚本

```
C:\Users\Administrator\Desktop>python -m pdb err.py
> c:\users\administrator\desktop\err.py(1)<module>()
-> s = '0'
(Pdb)
```

我们可以看到打印出了第一行代码，我们输入p s，来查看变量s的值

```
(Pdb) p s
*** NameError: name 's' is not defined
```

发现提示我们未定义变量s，所以终端显示的那一行代码表示的是我们要执行的代码，但目前还没有执行。

我们输入 n，执行下一行，然后再输入 p s

```
(Pdb) n
> c:\users\administrator\desktop\err.py(2)<module>()
-> n = int(s)
(Pdb) p s
'0'
```

这时成功的将变量值打印了出来。

输入 小写字母 L，查看执行的脚本中的代码， -> 表示将要执行这一行

```
(Pdb) l
  1     s = '0'
  2  -> n = int(s)
  3     print(10 / n)
[EOF]
```

再输入c，将代码执行完毕，并会自动的重新执行

```
(Pdb) c
Traceback (most recent call last):
  File "D:\工作软件\Python\lib\pdb.py", line 1697, in main
    pdb._runscript(mainpyfile)
  File "D:\工作软件\Python\lib\pdb.py", line 1566, in _runscript
    self.run(statement)
  File "D:\工作软件\Python\lib\bdb.py", line 585, in run
    exec(cmd, globals, locals)
  File "<string>", line 1, in <module>
  File "c:\users\administrator\desktop\err.py", line 2, in <module>
    n = int(s)
ZeroDivisionError: division by zero
Uncaught exception. Entering post mortem debugging
Running 'cont' or 'step' will restart the program
> c:\users\administrator\desktop\err.py(2)<module>()
-> n = int(s)
```

注意，如果代码出现问题，输入c之前代码执行到哪里，重新执行以后会自动执行到之前执行的位置。

如果代码没有报错，也会重新执行代码，不过会从头开始。

输入q退出

```
(Pdb) q

C:\Users\Administrator\Desktop>
```

这种通过pdb在命令行调试的方法太麻烦了，有一个pdb.set_trace()比它方便些。

### pdb.set_trace()

这个方法也是用pdb，但是不需要单步执行，我们只需要`import pdb`，然后，在可能出错的地方放一个`pdb.set_trace()`，就可以设置一个断点：

```
# err.py
import pdb

s = '0'
n = int(s)
pdb.set_trace() # 运行到这里会自动暂停
print(10 / n)
```

运行代码，程序会自动在`pdb.set_trace()`暂停并进入pdb调试环境，可以用命令`p`查看变量，或者用命令`c`继续运行：

```
$ python err.py 
> /Users/michael/Github/learn-python3/samples/debug/err.py(7)<module>()
-> print(10 / n)
(Pdb) p n
0
(Pdb) c
Traceback (most recent call last):
  File "err.py", line 7, in <module>
    print(10 / n)
ZeroDivisionError: division by zero
```

这个方式比直接启动pdb单步调试效率要高很多，但也高不到哪去。

