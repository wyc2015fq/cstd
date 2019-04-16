# Python笔记_第一篇_童子功_6.其他控制语句(with...as等) - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Python笔记_第一篇_童子功_6.其他控制语句(with...as等)](https://www.cnblogs.com/noah0532/p/8429902.html)





　　其他控制语句，也就是说一个with... as...语句。

　　这是python非常精妙的一个语句，非常的简单但是作用非常大，在打开文件获得句柄的时候可以用它，省去f.close()忘记添加的麻烦（这个文件的IO操作会用到）。

**1.   with语句时什么？**

　　有一些任务，可能实现需要设置，时候做清理工作。对于这种场景，python的with语句提供了一种非常方便的处理方式。一个很好的例子就是文件处理，你需要获得文件句柄，从文件中读取数据，然后关闭文件。

　　如果不用with语句，代码如下：

```
1 file = open("/tmp/foo.txt")
2 data = file.read()
3 file.close()
```

　　这里有两个问题。一是可能忘记关闭文件句柄；而是文件读取数据发生异常，没有进行任何处理。

　　如果采用with的话，除了有更优雅的语法，with还可以很好的处理上下文环境产生的异常。

```
1 with open("/tmp/foo.txt") as file:
2     data = file.read()
```



**2.   with是怎么工作的？**

　　我们有了上的这个例子可以得出with...as..的方法

　　这看起来充满魔法，但不仅仅是魔法，Python对with的处理还很聪明。基本思想是with所求值的对象必须有一个__enter__()方法，一个__exit__()方法。紧跟with后面的语句被求值后，返回对象的__enter__()方法被调用，这个方法的返回值将被赋值给as后面的变量。当with后面的代码块全部被执行完之后，将调用前面返回对象的__exit__()方法。

　　下面例子可以具体说明with如何工作：

```
1 class Sample:
 2     def __enter__(self):
 3         print "In __enter__()"
 4         return "Foo"
 5  
 6     def __exit__(self, type, value, trace):
 7         print "In __exit__()"
 8  
 9 def get_sample():
10     return Sample()
11  
12 with get_sample() as sample:
13     print "sample:", sample
```

　　运行代码，输出如下：

　　In __enter__()
　　sample: Foo
　　In __exit__()

**正如你看到的**：
　　1. __enter__()方法被执行
　　2. __enter__()方法返回的值 - 这个例子中是"Foo"，赋值给变量'sample'
　　3. 执行代码块，打印变量"sample"的值为 "Foo"
　　4. __exit__()方法被调用
　　with真正强大之处是它可以处理异常。可能你已经注意到Sample类的__exit__方法有三个参数- val, type 和 trace。 这些参数在异常处理中相当有用。我们来改一下代码，看看具体如何工作的。

　　举例如下：

```
class Sample:
    def __enter__(self):
        return self
 
    def __exit__(self, type, value, trace):
        print "type:", type
        print "value:", value
        print "trace:", trace
 
    def do_something(self):
        bar = 1/0
        return bar + 10
 
with Sample() as sample:
    sample.do_something()
```

　　这个例子中，with后面的get_sample()变成了Sample()。这没有任何关系，只要紧跟with后面的语句所返回的对象有__enter__()和__exit__()方法即可。此例中，Sample()的__enter__()方法返回新创建的Sample对象，并赋值给变量sample。
代码执行后：

```
1 bash-3.2$ ./with_example02.py
 2 type: <type 'exceptions.ZeroDivisionError'>
 3 value: integer division or modulo by zero
 4 trace: <traceback object at 0x1004a8128>
 5 Traceback (most recent call last):
 6   File "./with_example02.py", line 19, in <module>
 7     sample.do_something()
 8   File "./with_example02.py", line 15, in do_something
 9     bar = 1/0
10 ZeroDivisionError: integer division or modulo by zero
```

　　实际上，在with后面的代码块抛出任何异常时，__exit__()方法被执行。正如例子所示，异常抛出时，与之关联的type，value和stack trace传给__exit__()方法，因此抛出的ZeroDivisionError异常被打印出来了。开发库时，清理资源，关闭文件等等操作，都可以放在__exit__方法当中。
　　因此，Python的with语句是提供一个有效的机制，让代码更简练，同时在异常产生时，清理工作更简单。



另外，在异常处理单元，我们说过一个except...as...,请参照条件控制语句的异常处理部分














