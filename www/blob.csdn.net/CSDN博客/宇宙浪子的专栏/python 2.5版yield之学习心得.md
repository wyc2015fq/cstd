# python 2.5版yield之学习心得 - 宇宙浪子的专栏 - CSDN博客
2014年09月22日 14:15:24[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：542
在 shhgs 发布了关于《[Py 2.5 what’s new 之 yield](http://shhgs.blogspot.com/)》之后，原来我不是特别关注 yield 的用法，因为对于2.3中加入的yield相对来说功能简单，它是作为一个 generator 不可缺少的一条语句，只要包含它的函数即是一个 generator 。但在2.3中，generator
 不能重入，不能在运行过程中修改，不能引发异常，你要么是顺序调用，要么就创建一个新的 generator。而且 generator 中的 yield 只是一个语句。但到了 2.5 版之后，情况发生了很在的变化。
在 shhgs 的文章中对于 yield 并没有做太多的描述，也因此让我在理解上产生了许多问题，于是我仔细地研究了[What’s new](http://docs.python.org/dev/whatsnew/pep-342.html) 和 [PEP
 342](http://www.python.org/dev/peps/pep-0342/) 文档，有了一些体会，描述在下面。
这里不说为什么要对 yield 进行修改，只说功能。
1. yield 成为了表达式，它不再是语句，但可以放在单独的行上。原文：
> 
Redefine "yield" to be an expression, rather than a statement. The current yield statement would become a yield expression whose value is thrown away.
可以看到，如果你还是写成语句形式的话，其实还是一个表达式，只是它的值被扔掉了。
那么一个 yield 表达式可以这样写：
> 
x = yield i
y = x + (yield x)
那么这种机制到底是如何工作的呢？在2.3版很容易理解，你完全可以把 yield 语句理解为一个 "return" 语句，只不过 "return" 完后，函数并不结束，而是断续运行，直到再次遇到 yield 语句。那么到了 2.5 版不仅仅是一个 "return" 语句那么简单了，让我们看完下面关于 send() 的说明再描述它吧。
2. 增加了 send(msg) 方法，因此你可以使用它向 generator 发送消息。原文：
> 
Add a new send() method for generator-iterators, which resumes the generator and "sends" a value that becomes the result of the current yield-expression. The send() method returns the next value yielded by the generator, or raises StopIteration if the generator
 exits without yielding another value.
执行一个 send(msg) 会恢复 generator 的运行，然后发送的值将成为**当前** yield 表达式的**返回值**。然后 send() 会返回下一个被 generator yield 的值，如果没有下一个可以 yield 的值则引发一个异常。
那么可以看过这其实包含了一次运行，从将msg赋给当前被停住的 yield 表达式开始，到下一个 yield 语句结束，然后返回下一个yield语句的参数，然后再挂起，等待下一次的调用。理解起来的确很复杂，不知道你明白了没有。
那么让我们开始想象一下，把 yield 转变为易于理解的东西吧。
我们可以把 yield 想象成下面的伪代码：
> 
x = yield i ==> put(i); x = wait_and_get()
可以看到，可以理解为先是一个 put(i)，这个 i 就是 yield 表达式后面的参数，如果 yield 没有参数，则表示 None。它表示将 i 放到一个全局缓冲区中，相当于返回了一个值。
wait_and_get() 可以理解为一个阻塞调用，它等待着外界来唤醒它，并且可以返回一个值。
经过这种转换就容易理解多了。让我们来看一个例子：
> 
>>> def g(): 
    print ’step 1′ 
    x = yield ‘hello’ 
    print ’step 2′, ‘x=’, x 
    y = 5 + (yield x) 
    print ’step 3′, ‘y=’, y 
很简单，每执行一步都显示一个状态，并且打印出相关变量的值，让我们执行一下看一看。
> 
>>> f = g()
>>> f.next()
step 1
‘hello’
看见什么了。当我们执行 next() 时，代码执行到 x = yield ‘hello’ 就停住了，并且返回了 yield 后面的 ‘hello’。如果我们把上面的程序替换成伪代码看一看是什么样子：
> 
def g():
    print ’step 1′
    put(‘hello’)    #x = yield ‘hello’
    x = wait_and get()
    print ’stpe 2′, ’x=’, x
    put(x)
    y = 5 + wait_and_get()
    print ’step 3′, ‘y=’, y
可以从伪代码看出，第一次调用 next() 时，先返回一个 ‘hello’, 然后程序挂起在 x = wait_and_get() 上，与我们执行的结果相同。
让我们继续：
> 
>>> f.send(5)
step 2 x= 5
5
这次我们使用了 send(5) 而不是 next() 了。要注意 next() 在 2.5 中只算是 send(None) 的一种表现方式。正如伪代码演示的，send()一个值，先是激活 wait_and_get() ，并且通过它返回 send(5) 的参数5，于是 x 的值是 5，然后打印 ’step 2′，再返回 x 的值5，然后程序挂起在 y = 5 + wait_and_get() 上，与运行结果一致。
如果我们继续：
> 
>>> f.send(2)
step 3 y= 7
Traceback (most recent call last): 
  File "<pyshell#13>", line 1, in <module> 
    f.send(2) 
StopIteration
可以看到先是激活 wait_and_get()，并且通过它返回 send(2) 的参数 2，因此 y 的值是 7，然后执行下面的打印语句，但因为后面没有下一个 yield 语句了，因此程序无法挂起，于是就抛出异常来。
从上面的伪代码的示例和运行结果的分析，我想你应该对 yield 比较清楚了。还有一些要注意的：
- next()相当于send(None)
- yield后面没有参数表示返回为None
在文档中有几句话很重要：
> 
Because generator-iterators begin execution at the top of the generator’s function body, there is no yield expression to receive a value when the generator has just been created. Therefore, calling send() with a non-None argument is prohibited when the generator
 iterator has just started, and a TypeError is raised if this occurs (presumably due to a logic error of some kind). Thus, before you can communicate with a coroutine you must first call next() or send(None) to advance its execution to the first yield expression.
意思是说，第一次调用时要么使用 next() ，要么使用 send(None) ，不能使用 send() 来发送一个非 None 的值，原因就是第一次没有一个 yield 表达式来接受这个值。如果你转为伪代码就很好理解。以上例来说明，转换后第一句是一个 put() 而不是wait_and_get()，因此第一次执行只能返回，而不能接受数据。如果你真的发送了一个非 None 的值，会引发一个 TypeError 的异常，让我们试一试：
> 
>>> f = g() 
>>> f.send(5)
Traceback (most recent call last): 
  File "<pyshell#15>", line 1, in <module> 
    f.send(5) 
TypeError: can’t send non-None value to a just-started generator
看到了吧，果然出错了。
3. 增加了 throw() 方法，可以用来从 generator 内部来引发异常，从而控制 generator 的执行。试验一下：
> 
>>> f = g() 
>>> f.send(None) 
step 1 
‘hello’ 
>>> f.throw(GeneratorExit)
Traceback (most recent call last): 
  File "<pyshell#17>", line 1, in <module> 
    f.throw(GeneratorExit) 
  File "<pyshell#6>", line 3, in g 
    x = yield ‘hello’ 
GeneratorExit 
>>> f.send(5)
Traceback (most recent call last): 
  File "<pyshell#18>", line 1, in <module> 
    f.send(5) 
StopIteration
可以看出，第一次执行后，我执行了一个f.throw(GeneratorExit)，于是这个异常被引发。如果再次执行f.send(5)，可以看出 generator 已经被停止了。GeneratorExit 是新增加的一个异常类，关于它的说明：
> 
A new standard exception is defined, GeneratorExit, inheriting from Exception. A generator should handle this by re-raising it (or just not catching it) or by raising StopIteration.
可以看出，增加它的目的就是让 generator 有机会执行一些退出时的清理工作。这一点在 PEP 342 后面的 thumbnail 的例子中用到了。
4. 增加了 close 方法。它用来关闭一个 generator ，它的伪代码如下(从文档中抄来):
> 
def close(self): 
    try: 
        self.throw(GeneratorExit) 
    except (GeneratorExit, StopIteration): 
        pass 
    else: 
        raise RuntimeError("generator ignored GeneratorExit") 
# Other exceptions are not caught
因此可以看出，首先向自身引发一个 GeneratorExit 异常，如果 generator 引发了 GeneratorExit 或 StopIteration 异常，则关闭成功。如果 generator 返回了一个值，则引发 RuntimeError 异常。如果是其它的异常则不作处理，相当于向上层繁殖，由上层代码来处理。关于它的例子在 PEP 342 中的 thumbnail 的例子中也有描述。
还有其它几点变化，不再做更深入的描述。
关于 PEP 342 中的例子也很值得玩味。简单说一些，其实我也些也不是很懂也就是明白个大概其吧。
文档中一共有4个例子，其实是两个例子构成。
1,2两个例子完成了一个 thunmbnail 的处理。第一个例子 consumer 其实是一个 decorator ，它实现了对一个 generator 的封装，主要就是用来调用一次 next() 。为什么，因为这样调一次下一次就可以使用 send() 一个非 None 的值了，这样后面的代码在使用 generator 可以直接使用 send() 非 None 值来处理了。第二个例子完成对一系列的图片的缩略图的处理。这里每个图片的处理做成了一个 generator，对于图片文件的处理又是一个顶层的 generator
 ，在这个顶层的 generator 来调用每个图片处理的 generator。同时这个例子还实现了当异常退出时的一种保护工作：处理完正在处理的图片，然后退出。
3,4两个例子完成了一个 echo 服务器的演示。3完成了一个调度器，4是在3的基础上将listen处理和socket联通后的handle处理都转为可调度的 generator ，在调度器中进行调度。同时可以看到 socket 使用的是非阻塞的处理。
通过以上的学习，我深深地感受到 yield 的确很精巧，这一点还是在与 shhgs 语音交流之后才有更深的体会，许多东西可以通过 generator 表现得更优美和精巧，是一个非常值得玩味的东西。以至于 shhgs 感觉到在 2.5 中 yield 比 with 的意义要大。希望大家一同体会。
不过说实在的，yield 的东西的确有些难于理解，要仔细体会才行。
本文转自：http://qzriso.iteye.com/blog/726004
