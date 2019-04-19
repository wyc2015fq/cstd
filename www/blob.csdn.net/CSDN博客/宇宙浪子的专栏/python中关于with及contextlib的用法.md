# python中关于with及contextlib的用法 - 宇宙浪子的专栏 - CSDN博客
2016年06月15日 13:21:46[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：392
转自：[http://www.cnblogs.com/coser/archive/2013/01/28/2880328.html](http://www.cnblogs.com/coser/archive/2013/01/28/2880328.html)
 平常Coding过程中，经常使用到的with场景是（打开文件进行文件处理，然后隐式地执行了文件句柄的关闭，同样适合socket之类的，这些类都提供了对with的支持）:
```
```
with
```
`file``(``'test.py'``,``'r'`
```
)
 as f :
```
`    ``print``f.readline()`
```
    with的作用，类似try...finally...，提供一种上下文机制，要应用with语句的类，其内部必须提供两个内置函数__enter__以及__exit__。前者在主体代码执行前执行，后则在主体代码执行后执行。as后面的变量，是在__enter__函数中返回的。通过下面这个代码片段以及注释说明，可以清晰明白__enter__与__exit__的用法：
```
`#!encoding:utf-8`
`class`
```
echo
 :
```
`    ``def``output(``self`
```
)
 :
```
`        ``print`
```
'hello
 world'
```
`    ``def``__enter__(``self``):`
`        ``print``'enter'`
`        ``return``self``#返回自身实例，当然也可以返回任何希望返回的东西`
`    ``def``__exit__(``self`
```
,
 exception_type, exception_value, exception_traceback):
```
`        ``#若发生异常，会在这里捕捉到，可以进行异常处理`
`        ``print``'exit'`
`        ``#如果改__exit__可以处理改异常则通过返回True告知该异常不必传播，否则返回False`
`        ``if`
```
exception_type
```
`=``=`
```
ValueError
 :
```
`            ``return``True`
`        ``else``:`
`            ``return``False`
` `
```
with
 echo() as e:
```
`    ``e.output()`
`    ``print`
```
'do
 something inside'
```
`print``'-----------'`
```
with
 echo() as e:
```
`    ``raise``ValueError(`
```
'value
 error'
```
`)`
`print``'-----------'`
```
with
 echo() as e:
```
`    ``raise``Exception(`
```
'can
 not detect'
```
`)`
```
运行结果：
![QQ截图20130128171206](http://images.cnitblog.com/blog/146443/201301/28175417-01b09d6c49734aff90d22e6b0c603bb7.png)
contextlib是为了加强with语句，提供上下文机制的模块，它是通过Generator实现的。通过定义类以及写__enter__和__exit__来进行上下文管理虽然不难，但是很繁琐。contextlib中的contextmanager作为装饰器来提供一种针对函数级别的上下文管理机制。常用框架如下：
```
`from`
```
contextlib
```
`import``contextmanager`
` `
`@contextmanager`
`def`
```
make_context()
 :
```
`    ``print``'enter'`
`    ``try``:`
`        ``yield``{}`
`    ``except`
```
RuntimeError,
 err :
```
`        ``print``'error'`
```
,
 err
```
`    ``finally``:`
`        ``print``'exit'`
` `
```
with
 make_context() as value :
```
`    ``print``value`
```
contextlib还有连个重要的东西，一个是nested，一个是closing，前者用于创建嵌套的上下文，后则用于帮你执行定义好的close函数。但是nested已经过时了，因为with已经可以通过多个上下文的直接嵌套了。下面是一个例子：
```
`from`
```
contextlib
```
`import``contextmanager`
`from`
```
contextlib
```
`import``nested`
`from`
```
contextlib
```
`import``closing`
`@contextmanager`
`def`
```
make_context(name)
 :
```
`    ``print``'enter'`
```
,
 name
```
`    ``yield``name`
`    ``print``'exit'`
```
,
 name
```
` `
```
with
 nested(make_context(
```
`'A'`
```
),
 make_context(
```
`'B'`
```
))
 as (a, b) :
```
`    ``print``a`
`    ``print``b`
` `
```
with
 make_context(
```
`'A'`
```
)
 as a, make_context(
```
`'B'`
```
)
 as b :
```
`    ``print``a`
`    ``print``b`
` `
`class``Door(``object`
```
)
 :
```
`    ``def``open``(``self`
```
)
 :
```
`        ``print`
```
'Door
 is opened'
```
`    ``def``close(``self`
```
)
 :
```
`        ``print`
```
'Door
 is closed'
```
` `
```
with
 closing(Door()) as door :
```
`    ``door.``open``()`
```
运行结果：
![](http://images.cnitblog.com/blog/146443/201301/28174158-0862d4af882d462483c64b2121dcde33.png)
总结：python有很多强大的特性，由于我们平常总习惯于之前C++或java的一些编程习惯，时常忽略这些好的机制。因此，要学会使用这些python特性，让我们写的python程序更像是python。
