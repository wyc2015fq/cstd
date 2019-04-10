# python中重要的模块--asyncio



一直对asyncio这个库比较感兴趣，毕竟这是官网也非常推荐的一个实现高并发的一个模块，python也是在python 3.4中引入了协程的概念。也通过这次整理更加深刻理解这个模块的使用

asyncio 是干什么的？

- 异步网络操作
- 并发
- 协程

python3.0时代，标准库里的异步网络模块：select(非常底层) python3.0时代，第三方异步网络库：Tornado python3.4时代，asyncio：支持TCP,子进程

现在的asyncio，有了很多的模块已经在支持：aiohttp,aiodns,aioredis等等 <https://github.com/aio-libs> 这里列出了已经支持的内容，并在持续更新

当然到目前为止实现协程的不仅仅只有asyncio,tornado和gevent都实现了类似功能

关于asyncio的一些关键字的说明：

- event_loop 事件循环：程序开启一个无限循环，把一些函数注册到事件循环上，当满足事件发生的时候，调用相应的协程函数
- coroutine 协程：协程对象，指一个使用async关键字定义的函数，它的调用不会立即执行函数，而是会返回一个协程对象。协程对象需要注册到事件循环，由事件循环调用。
- task 任务：一个协程对象就是一个原生可以挂起的函数，任务则是对协程进一步封装，其中包含了任务的各种状态
- future: 代表将来执行或没有执行的任务的结果。它和task上没有本质上的区别
- async/await 关键字：python3.5用于定义协程的关键字，async定义一个协程，await用于挂起阻塞的异步调用接口。

看了上面这些关键字，你可能扭头就走了，其实一开始了解和研究asyncio这个模块有种抵触，自己也不知道为啥，这也导致很长一段时间，这个模块自己也基本就没有关注和使用，但是随着工作上用python遇到各种性能问题的时候，自己告诉自己还是要好好学习学习这个模块。

## 定义一个协程

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import time
import asyncio


now = lambda : time.time()


async def do_some_work(x):
    print("waiting:", x)

start = now()
# 这里是一个协程对象，这个时候do_some_work函数并没有执行
coroutine = do_some_work(2)
print(coroutine)
#  创建一个事件loop
loop = asyncio.get_event_loop()
# 将协程加入到事件循环loop
loop.run_until_complete(coroutine)

print("Time:",now()-start)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

在上面带中我们通过async关键字定义一个协程（coroutine）,当然协程不能直接运行，需要将协程加入到事件循环loop中

asyncio.get_event_loop：创建一个事件循环，然后使用run_until_complete将协程注册到事件循环，并启动事件循环

## 创建一个task

协程对象不能直接运行，在注册事件循环的时候，其实是run_until_complete方法将协程包装成为了一个任务（task）对象. task对象是Future类的子类，保存了协程运行后的状态，用于未来获取协程的结果

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import asyncio
import time


now = lambda: time.time()


async def do_some_work(x):
    print("waiting:", x)

start = now()

coroutine = do_some_work(2)
loop = asyncio.get_event_loop()
task = loop.create_task(coroutine)
print(task)
loop.run_until_complete(task)
print(task)
print("Time:",now()-start)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

结果为：

```
<Task pending coro=<do_some_work() running at /app/py_code/study_asyncio/simple_ex2.py:13>>
waiting: 2
<Task finished coro=<do_some_work() done, defined at /app/py_code/study_asyncio/simple_ex2.py:13> result=None>
Time: 0.0003514289855957031
```

创建task后，在task加入事件循环之前为pending状态，当完成后，状态为finished

关于上面通过loop.create_task(coroutine)创建task,同样的可以通过 asyncio.ensure_future(coroutine)创建task

关于这两个命令的官网解释： <https://docs.python.org/3/library/asyncio-task.html#asyncio.ensure_future>

```
asyncio.ensure_future(coro_or_future, *, loop=None)¶
Schedule the execution of a coroutine object: wrap it in a future. Return a Task object.

If the argument is a Future, it is returned directly.
```

<https://docs.python.org/3/library/asyncio-eventloop.html#asyncio.AbstractEventLoop.create_task>

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
AbstractEventLoop.create_task(coro)
Schedule the execution of a coroutine object: wrap it in a future. Return a Task object.

Third-party event loops can use their own subclass of Task for interoperability. In this case, the result type is a subclass of Task.

This method was added in Python 3.4.2. Use the async() function to support also older Python versions.
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 绑定回调

绑定回调，在task执行完成的时候可以获取执行的结果，回调的最后一个参数是future对象，通过该对象可以获取协程返回值。

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import time
import asyncio


now = lambda : time.time()


async def do_some_work(x):
    print("waiting:",x)
    return "Done after {}s".format(x)


def callback(future):
    print("callback:",future.result())


start = now()
coroutine = do_some_work(2)
loop = asyncio.get_event_loop()
task = asyncio.ensure_future(coroutine)
print(task)
task.add_done_callback(callback)
print(task)
loop.run_until_complete(task)

print("Time:", now()-start)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

结果为：

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
<Task pending coro=<do_some_work() running at /app/py_code/study_asyncio/simple_ex3.py:13>>
<Task pending coro=<do_some_work() running at /app/py_code/study_asyncio/simple_ex3.py:13> cb=[callback() at /app/py_code/study_asyncio/simple_ex3.py:18]>
waiting: 2
callback: Done after 2s
Time: 0.00039196014404296875
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

通过add_done_callback方法给task任务添加回调函数，当task（也可以说是coroutine）执行完成的时候,就会调用回调函数。并通过参数future获取协程执行的结果。这里我们创建 的task和回调里的future对象实际上是同一个对象

## 阻塞和await

使用async可以定义协程对象，使用await可以针对耗时的操作进行挂起，就像生成器里的yield一样，函数让出控制权。协程遇到await，事件循环将会挂起该协程，执行别的协程，直到其他的协程也挂起或者执行完毕，再进行下一个协程的执行

耗时的操作一般是一些IO操作，例如网络请求，文件读取等。我们使用asyncio.sleep函数来模拟IO操作。协程的目的也是让这些IO操作异步化。

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import asyncio
import time



now = lambda :time.time()

async def do_some_work(x):
    print("waiting:",x)
    # await 后面就是调用耗时的操作
    await asyncio.sleep(x)
    return "Done after {}s".format(x)


start = now()

coroutine = do_some_work(2)
loop = asyncio.get_event_loop()
task = asyncio.ensure_future(coroutine)
loop.run_until_complete(task)

print("Task ret:", task.result())
print("Time:", now() - start)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

在await asyncio.sleep(x)，因为这里sleep了，模拟了阻塞或者耗时操作，这个时候就会让出控制权。 即当遇到阻塞调用的函数的时候，使用await方法将协程的控制权让出,以便loop调用其他的协程。

## 并发和并行

并发指的是同时具有多个活动的系统

并行值得是用并发来使一个系统运行的更快。并行可以在操作系统的多个抽象层次进行运用

所以并发通常是指有多个任务需要同时进行，并行则是同一个时刻有多个任务执行

下面这个例子非常形象：

并发情况下是一个老师在同一时间段辅助不同的人功课。并行则是好几个老师分别同时辅助多个学生功课。简而言之就是一个人同时吃三个馒头还是三个人同时分别吃一个的情况，吃一个馒头算一个任务

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import asyncio
import time


now = lambda :time.time()


async def do_some_work(x):
    print("Waiting:",x)
    await asyncio.sleep(x)
    return "Done after {}s".format(x)

start = now()

coroutine1 = do_some_work(1)
coroutine2 = do_some_work(2)
coroutine3 = do_some_work(4)

tasks = [
    asyncio.ensure_future(coroutine1),
    asyncio.ensure_future(coroutine2),
    asyncio.ensure_future(coroutine3)
]

loop = asyncio.get_event_loop()
loop.run_until_complete(asyncio.wait(tasks))

for task in tasks:
    print("Task ret:",task.result())

print("Time:",now()-start)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

运行结果：

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
Waiting: 1
Waiting: 2
Waiting: 4
Task ret: Done after 1s
Task ret: Done after 2s
Task ret: Done after 4s
Time: 4.004154920578003
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

总时间为4s左右。4s的阻塞时间，足够前面两个协程执行完毕。如果是同步顺序的任务，那么至少需要7s。此时我们使用了aysncio实现了并发。asyncio.wait(tasks) 也可以使用 asyncio.gather(*tasks) ,前者接受一个task列表，后者接收一堆task。

关于asyncio.gather和asyncio.wait官网的说明：

<https://docs.python.org/3/library/asyncio-task.html#asyncio.gather>

```
Return a future aggregating results from the given coroutine objects or futures.

All futures must share the same event loop. If all the tasks are done successfully, the returned future’s result is the list of results (in the order of the original sequence, not necessarily the order of results arrival). If return_exceptions is true, exceptions in the tasks are treated the same as successful results, and gathered in the result list; otherwise, the first raised exception will be immediately propagated to the returned future.
```

<https://docs.python.org/3/library/asyncio-task.html#asyncio.wait>

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
Wait for the Futures and coroutine objects given by the sequence futures to complete. Coroutines will be wrapped in Tasks. Returns two sets of Future: (done, pending).

The sequence futures must not be empty.

timeout can be used to control the maximum number of seconds to wait before returning. timeout can be an int or float. If timeout is not specified or None, there is no limit to the wait time.

return_when indicates when this function should return.
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 协程嵌套

使用async可以定义协程，协程用于耗时的io操作，我们也可以封装更多的io操作过程，这样就实现了嵌套的协程，即一个协程中await了另外一个协程，如此连接起来。

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import asyncio
import time


now = lambda: time.time()

async def do_some_work(x):
    print("waiting:",x)
    await asyncio.sleep(x)
    return "Done after {}s".format(x)

async def main():
    coroutine1 = do_some_work(1)
    coroutine2 = do_some_work(2)
    coroutine3 = do_some_work(4)
    tasks = [
        asyncio.ensure_future(coroutine1),
        asyncio.ensure_future(coroutine2),
        asyncio.ensure_future(coroutine3)
    ]

    dones, pendings = await asyncio.wait(tasks)
    for task in dones:
        print("Task ret:", task.result())

    # results = await asyncio.gather(*tasks)
    # for result in results:
    #     print("Task ret:",result)


start = now()

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
print("Time:", now()-start)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

如果我们把上面代码中的：

```
    dones, pendings = await asyncio.wait(tasks)
    for task in dones:
        print("Task ret:", task.result())
```

替换为：

```
    results = await asyncio.gather(*tasks)
    for result in results:
        print("Task ret:",result)
```

这样得到的就是一个结果的列表

不在main协程函数里处理结果，直接返回await的内容，那么最外层的run_until_complete将会返回main协程的结果。 将上述的代码更改为：

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import asyncio
import time


now = lambda: time.time()

async def do_some_work(x):
    print("waiting:",x)
    await asyncio.sleep(x)
    return "Done after {}s".format(x)

async def main():
    coroutine1 = do_some_work(1)
    coroutine2 = do_some_work(2)
    coroutine3 = do_some_work(4)
    tasks = [
        asyncio.ensure_future(coroutine1),
        asyncio.ensure_future(coroutine2),
        asyncio.ensure_future(coroutine3)
    ]
    return await asyncio.gather(*tasks)

start = now()

loop = asyncio.get_event_loop()
results = loop.run_until_complete(main())
for result in results:
    print("Task ret:",result)

print("Time:", now()-start)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

或者返回使用asyncio.wait方式挂起协程。

将代码更改为：

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import asyncio
import time


now = lambda: time.time()

async def do_some_work(x):
    print("waiting:",x)
    await asyncio.sleep(x)
    return "Done after {}s".format(x)

async def main():
    coroutine1 = do_some_work(1)
    coroutine2 = do_some_work(2)
    coroutine3 = do_some_work(4)
    tasks = [
        asyncio.ensure_future(coroutine1),
        asyncio.ensure_future(coroutine2),
        asyncio.ensure_future(coroutine3)
    ]
    return await asyncio.wait(tasks)

start = now()

loop = asyncio.get_event_loop()
done,pending = loop.run_until_complete(main())
for task in done:
    print("Task ret:",task.result())

print("Time:", now()-start)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

也可以使用asyncio的as_completed方法

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import asyncio
import time


now = lambda: time.time()

async def do_some_work(x):
    print("waiting:",x)
    await asyncio.sleep(x)
    return "Done after {}s".format(x)

async def main():
    coroutine1 = do_some_work(1)
    coroutine2 = do_some_work(2)
    coroutine3 = do_some_work(4)
    tasks = [
        asyncio.ensure_future(coroutine1),
        asyncio.ensure_future(coroutine2),
        asyncio.ensure_future(coroutine3)
    ]
    for task in asyncio.as_completed(tasks):
        result = await task
        print("Task ret: {}".format(result))

start = now()

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
print("Time:", now()-start)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

从上面也可以看出，协程的调用和组合非常灵活，主要体现在对于结果的处理：如何返回，如何挂起

## 协程的停止

future对象有几个状态：

- Pending
- Running
- Done
- Cacelled

创建future的时候，task为pending，事件循环调用执行的时候当然就是running，调用完毕自然就是done，如果需要停止事件循环，就需要先把task取消。可以使用asyncio.Task获取事件循环的task

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import asyncio
import time


now = lambda :time.time()


async def do_some_work(x):
    print("Waiting:",x)
    await asyncio.sleep(x)
    return "Done after {}s".format(x)

coroutine1 =do_some_work(1)
coroutine2 =do_some_work(2)
coroutine3 =do_some_work(2)

tasks = [
    asyncio.ensure_future(coroutine1),
    asyncio.ensure_future(coroutine2),
    asyncio.ensure_future(coroutine3),
]

start = now()

loop = asyncio.get_event_loop()
try:
    loop.run_until_complete(asyncio.wait(tasks))
except KeyboardInterrupt as e:
    print(asyncio.Task.all_tasks())
    for task in asyncio.Task.all_tasks():
        print(task.cancel())
    loop.stop()
    loop.run_forever()
finally:
    loop.close()

print("Time:",now()-start)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

启动事件循环之后，马上ctrl+c，会触发run_until_complete的执行异常 KeyBorardInterrupt。然后通过循环asyncio.Task取消future。可以看到输出如下：

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
Waiting: 1
Waiting: 2
Waiting: 2
^C{<Task finished coro=<do_some_work() done, defined at /app/py_code/study_asyncio/simple_ex10.py:13> result='Done after 1s'>, <Task pending coro=<do_some_work() running at /app/py_code/study_asyncio/simple_ex10.py:15> wait_for=<Future pending cb=[Task._wakeup()]> cb=[_wait.<locals>._on_completion() at /usr/local/lib/python3.5/asyncio/tasks.py:428]>, <Task pending coro=<do_some_work() running at /app/py_code/study_asyncio/simple_ex10.py:15> wait_for=<Future pending cb=[Task._wakeup()]> cb=[_wait.<locals>._on_completion() at /usr/local/lib/python3.5/asyncio/tasks.py:428]>, <Task pending coro=<wait() running at /usr/local/lib/python3.5/asyncio/tasks.py:361> wait_for=<Future pending cb=[Task._wakeup()]>>}
False
True
True
True
Time: 1.0707225799560547
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

True表示cannel成功，loop stop之后还需要再次开启事件循环，最后在close，不然还会抛出异常

循环task，逐个cancel是一种方案，可是正如上面我们把task的列表封装在main函数中，main函数外进行事件循环的调用。这个时候，main相当于最外出的一个task，那么处理包装的main函数即可。

## 不同线程的事件循环

很多时候，我们的事件循环用于注册协程，而有的协程需要动态的添加到事件循环中。一个简单的方式就是使用多线程。当前线程创建一个事件循环，然后在新建一个线程，在新线程中启动事件循环。当前线程不会被block。

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import asyncio
from threading import Thread
import time

now = lambda :time.time()

def start_loop(loop):
    asyncio.set_event_loop(loop)
    loop.run_forever()

def more_work(x):
    print('More work {}'.format(x))
    time.sleep(x)
    print('Finished more work {}'.format(x))

start = now()
new_loop = asyncio.new_event_loop()
t = Thread(target=start_loop, args=(new_loop,))
t.start()
print('TIME: {}'.format(time.time() - start))

new_loop.call_soon_threadsafe(more_work, 6)
new_loop.call_soon_threadsafe(more_work, 3)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

启动上述代码之后，当前线程不会被block，新线程中会按照顺序执行call_soon_threadsafe方法注册的more_work方法， 后者因为time.sleep操作是同步阻塞的，因此运行完毕more_work需要大致6 + 3

## 新线程协程

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import asyncio
import time
from threading import Thread

now = lambda :time.time()


def start_loop(loop):
    asyncio.set_event_loop(loop)
    loop.run_forever()

async def do_some_work(x):
    print('Waiting {}'.format(x))
    await asyncio.sleep(x)
    print('Done after {}s'.format(x))

def more_work(x):
    print('More work {}'.format(x))
    time.sleep(x)
    print('Finished more work {}'.format(x))

start = now()
new_loop = asyncio.new_event_loop()
t = Thread(target=start_loop, args=(new_loop,))
t.start()
print('TIME: {}'.format(time.time() - start))

asyncio.run_coroutine_threadsafe(do_some_work(6), new_loop)
asyncio.run_coroutine_threadsafe(do_some_work(4), new_loop)
```

[![复制代码](http://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

上述的例子，主线程中创建一个new_loop，然后在另外的子线程中开启一个无限事件循环。 主线程通过run_coroutine_threadsafe新注册协程对象。这样就能在子线程中进行事件循环的并发操作，同时主线程又不会被block。一共执行的时间大概在6s左右。

所有的努力都值得期许，每一份梦想都应该灌溉！



分类: [python成长之路](https://www.cnblogs.com/zhaof/category/858301.html)