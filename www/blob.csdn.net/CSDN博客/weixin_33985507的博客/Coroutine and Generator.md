# Coroutine and Generator - weixin_33985507的博客 - CSDN博客
2016年04月30日 00:06:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
## Coroutine
先要知道什么是 Coroutine，按照 [Wikipedia](https://link.jianshu.com?t=https://www.wikiwand.com/en/Coroutine) 上的定义
> 
**Coroutines** are computer program components that generalize subroutines for nonpreemptive multitasking, by allowing multiple entry points for suspending and resuming execution at certain locations.
When subroutines are invoked, execution begins at the start, and once a subroutine exits, it is finished; an instance of a subroutine only returns once, and does not hold state between invocations. By contrast, coroutines can exit by calling other coroutines, which may later return to the point where they were invoked in the original coroutine; from the coroutine's point of view, it is not exiting but calling another coroutine. Thus, **a coroutine instance holds state, and varies between invocations; there can be multiple instances of a given coroutine at once.**
可以这么理解，平常的子程序(函数)都是只有一个出口(通过 return), 一般都是 caller 调用 callee，callee 结束后返回到caller。
而协程可以有多个出入口(yield 指明)，而且协程可以 yield 到其他协程， 其他协程可以继续 yield 到其他协程(或者 yield 到上一个协程)
那么生成器(Generator)是什么呢？我们平常说的生成器实现了迭代器协议，因此能作为迭代器使用。但生成器本质上是一种协程。看 Wiki 上的解释
> 
Generators, also known as semicoroutines,[5] are also a generalisation of subroutines, but are more limited than coroutines. Specifically, while both of these can yield multiple times, suspending their execution and allowing re-entry at multiple entry points, they differ in that coroutines can control where execution continues after they yield, while generators cannot, instead transferring control back to the generator's caller.[6] That is, since generators are primarily used to simplify the writing of iterators, the yield statement in a generator does not specify a coroutine to jump to, but rather passes a value back to a parent routine.
我的理解是生成器是一种有限制的协程，其不能 yield 到其他协程，只能 yield 到 caller.
由于这个特性，其十分适合作为迭代器，因此就常常作为迭代器使用了。
## Yield keyword in Python
其他从 Python 对协程的支持我们也可以发现生成器就是协程，他们在 Python 中都是用关键词 yield 定义。
Python 中的 yield 有这么三种用处:
- **pull**：caller 从生成器获得数据（`next(gen) / yield item`）
- **push**：协程从 caller 收到值 (`gen.send(value) / item = yield`)
- **tasks**：协程与 caller 之间没有数据交互，只是用来进行流程控制
我觉得第三点流程控制才是协程的本质，传输数据只是一个自然而然的功能。
可以用 inspect.getgeneratorstate(...) 获得协程状态
共有四种状态
'GEN_CREATED', 'GEN_RUNNING', 'GEN_SUSPENDED, 'GEN_CLOSED'
```
from inspect import getgeneratorstate
def simple_coro(a):
    """
    >>> coro = simple_coro(14)
    >>> coro
    <generator object simple_coro at 0x038A8A50>
    >>> getgeneratorstate(coro)
    'GEN_CREATED'
    >>> next(coro)  # 启动协程
    -> Started: a =  14
    14
    >>> getgeneratorstate(coro)
    'GEN_SUSPENDED'
    >>> coro.send(28) # 传入 b
    -> Received: b =  28
    42
    >>> coro.send(99)
    -> Received: c =  99
    StopIteration ...
    >>> getgeneratorstate(coro)
    'GEN_CLOSED'
    """
    print('-> Started: a = ', a)
    b = yield a
    print('-> Received: b = ', b)
    c = yield a + b
    print('-> Received: c = ', c)
```
![1615282-d342f8179fcc6c1b.jpg](https://upload-images.jianshu.io/upload_images/1615282-d342f8179fcc6c1b.jpg)
coroutine.jpg
#### Returning a Value from a Coroutine
有时候一个协程不是为了 yield value, 而是为了最后返回值
协程返回值时会抛出一个 StopIteration (就跟迭代器完结一样)
```
from collections import  namedtuple
Result = namedtuple('Result', 'count average')
def averager():
    total = 0.0
    count = 0
    average = None
    while True:
        term = yield
        if term is None:
            break
        total += term
        count += 1
        average = total / count
    return Result(count, average)
---------------------
>>> coro_avg = averager()
>>> next(coro_avg)
>>> coro_avg.send(10)
>>> coro_avg.send(None) # 我们发现协程结束会抛出 StopIteration 异常, 就跟迭代器结束一样
StopIteration: Result(count=1, average=10.0)
---------------------
>>> try:  # 我们要想获得协程最后返回值必须这么做...
    coro_avg.send(None)
except StopIteration as exc:
    result = exc.value
>>> result
Result(count=1, average=10.0)
```
可以发现每次捕获 StopIteration 很麻烦, 所以 Python 中 yield from 可以自动帮助我们捕获 StopIteration 并把协程返回值绑定到变量上 (具体 yield from 见下面)
```
def delegate():
    while True:
        yield (yield from averager())
--------------------
>>> coro_avg = delegate()
>>> next(coro_avg)
>>> coro_avg.send(10)
>>> coro_avg.send(None)
Result(count=1, average=10.0)
```
## Yield From
yield from 在其他支持协程的语言中往往称作 await，后者名字更利于理解。
yield from 涉及到了如下三个对象:
- caller
- delegating generator: 包含 yield from <iterable> 的生成器
- subgenerator: yield from <iterable> 中的iterable(常用生成器或迭代器)
两个注意事项:
- Every arrangement of coroutines chained with  yield from must be ultimately
driven by a caller that is not a coroutine, which invokes  next(…) or  .send(…) on the outermost delegating generator, explicitly or implicitly (e.g., in a  for loop).
- The innermost subgenerator in the chain must be a simple generator that uses just yield—or an iterable object
![1615282-0d13cbb9bf37cee6.jpg](https://upload-images.jianshu.io/upload_images/1615282-0d13cbb9bf37cee6.jpg)
yield from.jpg
一个用 yield from 的[二叉树中序遍历](https://link.jianshu.com?t=http://www.cosc.canterbury.ac.nz/greg.ewing/python/yield-from/yf_current/Examples/binary_tree.py)
```
def chain(*iterables):
    """
    >>> chain('ABC', range(2))
    ['A', 'B', 'C', 0, 1]
    """
    for it in iterables:
        yield from it
```
```
from collections import  namedtuple
Result = namedtuple('Result', 'count average')
# the subgenerator
def averager():
    total = 0.0
    count = 0
    average = None
    while True:
        term = yield
        if term is None:
            break
        total += term
        count += 1
        average = total / count
    return Result(count, average)
# the delegating generator
def grouper(results, key):
    # 这里 while True 的目的是让这个协程永远不退出, 
    # 因为协程退出时会抛出 StopIteration, 这样必须在 caller 中捕获
    while True:
        # print('here') 如果打印, 会发现打印了四次
        # 启动时打印一次，当 subgenerator 返回时，delegating generator 苏醒，又会打印一次
        results[key] = yield from averager()  # yield from 会自动帮你启动 subgenerator
# the caller
def main(data, results):
    for key, values in data.items():
        # 每次使用一个新的 grouper, 目的是有一个不同的 key
        # 可以只用一个 grouper 的, 思路是将 key 也传进去，见下面
        group = grouper(results, key)
        next(group)
        for value in values:
            # 这里的 value 都经由 group, 发送到 averager 中去了
            # averager yield 的值也经由 group 返回到 caller
            # group(delegating generator) 只是作为一个管道
            group.send(value)
        # 必须发送 None, 让 averager 返回, 这里才能在 results 中设定值
        # 如果 删掉下面那句, 会发现最后 results 为空字典
        group.send(None)
>>> data = {'a': [1, 2, 3, 4, 5], 'b': [6, 7, 8, 9, 10]}
>>> results ={}
>>> main(data, results)
>>> results
{'a': Result(count=5, average=3.0), 'b': Result(count=5, average=8.0)}
```
```
# the delegating generator
def grouper(results):
    while True:
        # print('here')  会打印 3 次
        key = yield
        results[key] = yield from averager()
# the caller
def main(data, results):
    group = grouper(results)
    next(group)
    for key, values in data.items():
        group.send(key)
        for value in values:
            group.send(value)
        group.send(None)
```
下面我们来具体看看 yield 三种功能的应用:
### Generator as Pipeline (Pull)
![1615282-ed2342b512ae35ff.jpg](https://upload-images.jianshu.io/upload_images/1615282-ed2342b512ae35ff.jpg)
generator as pipeline.jpg
```
import time
def follow(thefile):
    thefile.seek(0, 2)  # Go to the end of the file
    while True:
        line = thefile.readline()
        if not line:
            time.sleep(0.1)
            continue
        yield line
def grep(pattern, lines):
    for line in lines:
        if pattern in line:
            yield line
            
logfile = open("sample-log")
loglines = follow(logfile)
pylines = grep("python", loglines)
for line in pylines:
    print(line)
```
### Coroutine as Pipeline (Push)
![1615282-1ed7feee26a4a7e0.jpg](https://upload-images.jianshu.io/upload_images/1615282-1ed7feee26a4a7e0.jpg)
coroutine as pipeline.jpg
```
import time
def coroutine(func):
    """Decorator for start coroutine"""
    def start(*args, **kwargs):
        coro = func(*args, **kwargs)
        next(coro)
        return coro
    return start
@coroutine
def follow(thefile, target):
    thefile.seek(0, 2)  # Go to the end of the file
    while True:
        line = thefile.readline()
        if not line:
            time.sleep(0.1)
            continue
        target.send(line)
@coroutine
def grep(pattern, target):
    while True:
        line = yield
        if pattern in line:
            target.send(line)
            
@coroutine
def printer():
    while True:
        line = yield
        print(line)
logfile = open("sample-log")
loglines = follow(logfile, grep("python", printer()))
```
![1615282-32093ab885544994.jpg](https://upload-images.jianshu.io/upload_images/1615282-32093ab885544994.jpg)
broadcast.jpg
```
@coroutine
def broadcast(targets):
    while True:
        item = yield
        for target in targets:
            target.send(item)
            
logfile = open("sample-log")
loglines = follow(logfile, 
                            broadcast([grep("python", printer()),
                                              grep("java", printer()),
                                              grep("go", printer())]))
```
![1615282-0f076750ec343106.jpg](https://upload-images.jianshu.io/upload_images/1615282-0f076750ec343106.jpg)
generator vs coroutine.jpg
### Coroutine as Task
可以把 yield 看做 trap, 不过有个不同，trap 是把控制权交给操作系统，而 yield 是把控制权交还给 caller。
```
from queue import Queue
class Task(object):
    """Task is a wrapper around a coroutine"""
    taskid = 0
    def __init__(self, target):
        Task.taskid += 1
        self.tid = Task.taskid
        self.target = target  # target coroutine
        self.sendval = None  # sendval 为 system call 的结果
        
    def run(self):
        return self.target.send(self.sendval)
    
class SystemCall(object):
    def handle(self):
        pass
    
class GetTid(SystemCall):
    def handle(self):
        self.task.sendval = self.task.tid
        self.sched.schedule(self.task)
        
class NewTask(SystemCall):
    def __init__(self, target):
        self.target = target
    def handle(self):
        tid = self.sched.new(self.target)
        self.task.sendval = tid
        self.sched.schedule(self.task)
        
class KillTask(SystemCall):
    def __init__(self, tid):
        self.tid = tid
    def handle(self):
        task = self.sched.taskmap.get(self.tid, None)
        if task:
            task.target.close()
            self.task.sendval = True
        else:
            self.task.sendval = False
        self.sched.schedule(self.task)
        
class WaitTask(SystemCall):
    def __init__(self, tid):
        self.tid = tid
    def handle(self):
        result = self.sched.waitforexit(self.task, self.tid)
        self.task.sendval = result
        if not result:
            self.sched.schedule(self.task)
class Scheduler(object):
    """ Each task runs until it hits the yield
          scheduler regains control and switch to the other task
    """
    def __init__(self):
        self.ready = Queue()
        self.taskmap = {}
        self.exit_waiting = {}
        
    def new(self, target):
        """Create a new task and put them in ready queue"""
        newtask = Task(target)
        self.taskmap[newtask.tid] = newtask
        self.schedule(newtask)
        return newtask.tid
    
    def exit(self, task):
        print("Taks {} terminated".format(task.tid))
        del self.taskmap[task.tid]
        # notify other tasks waiting for this task to exit
        for task in self.exit_waiting.pop(task.tid, []):
            self.schedule(task)
            
    def waitforexit(self, task, waittid):
        if waittid in self.taskmap:
            self.exit_waiting.setdefault(waittid, []).append(task)
            return True
        else:
            return False
        
    def schedule(self, task):
        """Put task in ready queue"""
        self.ready.put(task)
        
    def mainloop(self):
        while self.taskmap:
            task = self.ready.get()
            try:
                result = task.run()
                if isinstance(result, SystemCall):
                    result.task = task
                    result.sched = self
                    result.handle()
                    continue
            except StopIteration:
                self.exit(task)
                continue
            self.schedule(task)
def foo():
    print("foo start")
    mytid = yield GetTid()
    for i in range(2):
        print("foo id={}".format(mytid))
        yield
def bar():
    print("bar start")
    mytid = yield GetTid()
    for i in range(1):
        print("bar {}".format(mytid))
        yield
        
s = Scheduler()
s.new(foo())
s.new(bar())
s.mainloop()
---------------
foo start
bar start
foo id=1
bar 2
foo id=1
Taks 2 terminated
Taks 1 terminated
************
def main():
    child = yield NewTask(foo())
    yield
    yield KillTask(child)
    print("main done")
s = Scheduler()
s.new(main())
s.mainloop()
-----------------
foo start
foo id=4
Taks 4 terminated
main done
Taks 3 terminated
***********
def main():
    child = yield NewTask(foo())
    print("Waiting for child")
    yield WaitTask(child)
    print("Child done")
    
s = Scheduler()
s.new(main())
s.mainloop()
-----------------------
foo start
Waiting for child
foo id=6
foo id=6
Taks 6 terminated
Child done
Taks 5 terminated
```
#### TODO 后面 webserver，非阻塞 IO 还没看
参考资料:
Fluent Python Control Flow
[PEP 342 -- Coroutines via Enhanced Generators](https://link.jianshu.com?t=https://www.python.org/dev/peps/pep-0342/)
[PEP 380 -- Syntax for Delegating to a Subgenerator](https://link.jianshu.com?t=https://www.python.org/dev/peps/pep-0380/)
[Generator Tricks for Systems Programmers](https://link.jianshu.com?t=http://www.dabeaz.com/generators/)
[A Curious Course on Coroutines and Concurrency](https://link.jianshu.com?t=http://www.dabeaz.com/coroutines/)
[Generators: The Final Frontier](https://link.jianshu.com?t=http://www.dabeaz.com/finalgenerator/)
[A Web Crawler With asyncio Coroutines](https://link.jianshu.com?t=http://aosabook.org/en/500L/pages/a-web-crawler-with-asyncio-coroutines.html#fnref7)
[In practice, what are the main uses for the new “yield from” syntax in Python 3.3?](https://link.jianshu.com?t=http://stackoverflow.com/questions/9708902/in-practice-what-are-the-main-uses-for-the-new-yield-from-syntax-in-python-3)
