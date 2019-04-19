# 浅析tornado协程运行原理 - 宇宙浪子的专栏 - CSDN博客
2017年08月15日 17:39:50[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：236
个人分类：[Python																[tornado](https://blog.csdn.net/bluehawksky/article/category/7102850)](https://blog.csdn.net/bluehawksky/article/category/2542577)

来自： [http://xidui.github.io/2016/01/26/浅析tornado协程运行原理/](http://xidui.github.io/2016/01/26/%E6%B5%85%E6%9E%90tornado%E5%8D%8F%E7%A8%8B%E8%BF%90%E8%A1%8C%E5%8E%9F%E7%90%86/)
### 前言
去年有一段时间一直在研究各种python协程框架，包括gevent, asyncio, tornado。阅读tornado的源码还是两个多月前的事了，一直想写一篇文章出来整理整理，但不知道从何处开始下笔。如果贴上一段段源码，然后通过语言来描述各种流程，这种类型的文章网上也有不少，况且这样子的讲解对于读者来说可能会比较乏味。
我希望我对于源码分析的博文能够通过贴上更容易理解的图(当然也会有一些代码来辅助讲解)，这样的分享对读者来说会更加容易读懂，也更有价值。对自己要求高了，反而更难下笔，在试图画图的过程中，发现其实有好多细节自己也没有琢磨透，导致在如何组织这幅流程图的问题上斟酌了好久，不过好在最后终于捯饬出了一张自己觉得还算及格的流程图，作完图的时候我感觉比起之前刚阅读完代码时候的理解又上了一个层次。
### 流程图
tornado执行协程的方式有很多，但协程内部的运行原理没什么区别，这篇文章以 IOLoop 中的 run_sync 函数作为入口进行介绍。在开始进行分析之前，先把流程图贴上，其中的细节后面会通过代码辅助的方式一一讲解。
![](http://static.open-open.com/lib/uploadImg/20160127/20160127204230_285.png)
在理解tornado运行原理的过程中，我是通过写一个demo，然后在源码中到处打断点，然后调试的方式，一遍遍走，到最后慢慢地理解。顺便也把我的demo代码贴上吧(看过我之前的一篇译文的读者可能会发现，这个demo是从那儿仿照过来的)。
```python
import random
import time
from tornado import gen
from tornado.ioloop import IOLoop
@gen.coroutine
def get_url(url):
    wait_time = random.randint(1, 4)
    yield gen.sleep(wait_time)
    print('URL {} took {}s to get!'.format(url, wait_time))
    raise gen.Return((url, wait_time))
@gen.coroutine
def outer_coroutine():
    before = time.time()
    coroutines = [get_url(url) for url in ['URL1', 'URL2', 'URL3']]
    result = yield coroutines
    after = time.time()
    print(result)
    print('total time: {} seconds'.format(after - before))
if __name__ == '__main__':
    IOLoop.current().run_sync(outer_coroutine)
```
有兴趣的读者可以自己去执行一下玩玩，输出类似于这样：
```python
URL URL1 took 1s to get!
URL URL2 took 2s to get!
URL URL3 took 2s to get!
[('URL1', 1), ('URL2', 2), ('URL3', 2)]
total time: 2.00353884697 seconds
```
### Coroutine
起初我以为调用协程后，返回的是一个生成器对象，毕竟 gen.coroutine 装饰在一个函数或者生成器上。看了源码发现，其实每次调用一个协程，它在获取了生成器对象之后，同时又对它执行了 next 操作来获取生成器内部yield出来的值，这个可以是一个值，当然也可以是一个由内部协程嵌套调用返回的future对象。
```python
# gen.py
def _make_coroutine_wrapper(func, replace_callback):
 @functools.wraps(func)
    def wrapper(*args, **kwargs):
        future = TracebackFuture()
        # 省略n行
        try:
            result = func(*args, **kwargs)
        # 省略n个except
        else:
            if isinstance(result, types.GeneratorType):
                try:
                    orig_stack_contexts = stack_context._state.contexts
                    yielded = next(result)    
                    # 如果func内部有yield关键字，result是一个生成器
                    # 如果func内部又调用了其它协程，yielded将会是由嵌套协程返回的future对象
                    # 省略n行
                # 省略n个except
                else:
                    Runner(result, future, yielded)
                try:
                    return future
                finally:
                    future = None
        future.set_result(result)
        return future
    return wrapper
```
### Future
我觉得 Future 在tornado中是一个很奇妙的对象，它是一个穿梭于协程和调度器之间的信使。提供了回调函数注册(当异步事件完成后，调用注册的回调)、中间结果保存、嵌套协程唤醒父协程(通过Runner实现)等功能。Coroutine和Future是一一对应的，可以从上节gen.coroutine装饰器的实现中看到。每调用一个协程，表达式所返回的就是一个Future对象，它所表达的意义为： 这个协程的内部各种异步逻辑执行完毕后，会把结果保存在这个Future中，同时调用这个Future中指定的回调函数 ，而future中的回调函数是什么时候被注册的呢？那就是当前——你通过调用协程，返回了这个future对象的时候：
我们看看demo代码中run_sync的实现:
```python
# ioloop.py IOLoop
def run_sync(self, func, timeout=None):
    future_cell = [None]
    def run():
        try:
            result = func()
        except Exception:
            future_cell[0] = TracebackFuture()
            future_cell[0].set_exc_info(sys.exc_info())
        else:
            if is_future(result):
                future_cell[0] = result
            else:
                future_cell[0] = TracebackFuture()
                future_cell[0].set_result(result)
        self.add_future(future_cell[0], lambda future: self.stop())
    self.add_callback(run)
    if timeout is not None:
        timeout_handle = self.add_timeout(self.time() + timeout, self.stop)
    self.start()
    if timeout is not None:
        self.remove_timeout(timeout_handle)
    if not future_cell[0].done():
        raise TimeoutError('Operation timed out after %s seconds' % timeout)
    return future_cell[0].result()
```
代码中先给 IOLoop 注册一个回调函数，等下个事件循环再执行内部定义的run函数。在run中通过 result = func() 执行协程 outer_coroutine ，result则是该协程对应的future对象。如果这个时候不对future作任何操作，最后这个future完成后也不会执行任何回调。所以在源码中通过 add_future 给这个future添加回调函数，也就是
 self.stop() ，表明这个协程执行完毕后触发的操作是退出事件循环。
其实IOLoop::add_future这个函数的命名会有些奇怪，刚读代码还不知道它是干嘛的(给IOLoop添加future是什么鬼？如果说是add_callback那还容易理解)，看了add_future的实现就明白了:
```python
# ioloop.py IOLoop
def add_future(self, future, callback):
    """Schedules a callback on the ``IOLoop`` when the given
 `.Future` is finished.
 The callback is invoked with one argument, the
 `.Future`.
 """
    assert is_future(future)
    callback = stack_context.wrap(callback)
    future.add_done_callback(
        lambda future: self.add_callback(callback, future))
```
它并不会给IOLoop添加future(也没有什么意义)，它只是给这个future添加回调函数而已，而这个回调函数是当这个future完成以后给IOLoop添加一个回调函数(有点绕，哈哈~给IOLoop添加的回调函数在这里就是stop)。 因此当一个future完成以后，到最后future的回调函数真正被执行将会隔着一个IOLoop的事件循环，而不是马上会被执行 。
### Runner
如果说tornado是一辆车，那么Runner对象就是它的发动机，由它来调度各种协程来完成异步事件的操作。Coroutine和Runner也是一一对应的，每个Coroutine都是由一个Runner实例去执行的。协程包装着生成器(当然也有可能是函数，本文考虑比较复杂的协程嵌套调用的情况)，在生成器内部，也有可能会调用其它的协程，从而把内部协程的future对象yield出来，这个runner就会通过调用返回的方式(
 future = next(gen) )接到内部出来的future，并把它纳入执行的loop中，先是 handle_yielded ，再是 run (中间会隔着一个或者多个IOLoop的事件循环，因此图中是用虚线表示的)。
调度器中有两个比较重要的函数: handle_yielded 和 run ，先来看 handle_yielded :
```python
# gen.py Runner
def handle_yield(self, yielded):
    # Lists containing YieldPoints require stack contexts;
    # other lists are handled via multi_future in convert_yielded.
    if (isinstance(yielded, list) and
            any(isinstance(f, YieldPoint) for f in yielded)):
        yielded = Multi(yielded)
    elif (isinstance(yielded, dict) and
          any(isinstance(f, YieldPoint) for f in yielded.values())):
        yielded = Multi(yielded)
    if isinstance(yielded, YieldPoint):
        # 省略n行
    else:
        try:
            self.future = convert_yielded(yielded)
        except BadYieldError:
            self.future = TracebackFuture()
            self.future.set_exc_info(sys.exc_info())
    if not self.future.done() or self.future is moment:
        self.io_loop.add_future(
            self.future, lambda f: self.run())
        return False
    return True
```
在runner中， handle_yielded 用于处理generator返回的内部协程future对象。因为协程处理的大部分是异步的事件，所以内部协程yield出来的future对象状态多半还是处于未完成。这个时候收到该future的Runner所能做的也仅仅只是注册一个回调函数而已(上面源码的最后几行)。
再来看看 run :
```python
# gen.py Runner
def run(self):
    """Starts or resumes the generator, running until it reaches a
 yield point that is not ready.
 """
    if self.running or self.finished:
        return
    try:
        self.running = True
        while True:
            future = self.future
            if not future.done():
                return
            self.future = None
            try:
                orig_stack_contexts = stack_context._state.contexts
                exc_info = None
                try:
                    value = future.result()
                except Exception:
                    self.had_exception = True
                    exc_info = sys.exc_info()
                if exc_info is not None:
                    yielded = self.gen.throw(*exc_info)
                    exc_info = None
                else:
                    yielded = self.gen.send(value)
                if stack_context._state.contexts is not orig_stack_contexts:
                    self.gen.throw(
                        stack_context.StackContextInconsistentError(
                            'stack_context inconsistency (probably caused '
                            'by yield within a "with StackContext" block)'))
            except (StopIteration, Return) as e:
                self.finished = True
                self.future = _null_future
                if self.pending_callbacks and not self.had_exception:
                    # If we ran cleanly without waiting on all callbacks
                    # raise an error (really more of a warning). If we
                    # had an exception then some callbacks may have been
                    # orphaned, so skip the check in that case.
                    raise LeakedCallbackError(
                        "finished without waiting for callbacks %r" %
                        self.pending_callbacks)
                self.result_future.set_result(getattr(e, 'value', None))
                self.result_future = None
                self._deactivate_stack_context()
                return
            except Exception:
                self.finished = True
                self.future = _null_future
                self.result_future.set_exc_info(sys.exc_info())
                self.result_future = None
                self._deactivate_stack_context()
                return
            if not self.handle_yield(yielded):
                return
    finally:
        self.running = False
```
run函数中的注释很好得诠释了它的作用，它就是不断地给传入Runner的generator执行next或者send操作(next或send都会让生成器继续运行，区别就是send会传一个参数进去)，直到generator返回的future对象状态还未完成，需要等待异步响应，这个时候它会调用handle_yielded。
异步响应来了以后，就会调用这个run，为什么呢？因为在 handle_yielded 中给这个future注册了回调函数，回调函数就是 run 函数。然后在run函数中执行send(value)，让这个生成器继续运行，如此往复循环，直到generator退出。
generator退出就代表着这个Runner引擎所跑的Coroutine完成了，然后再给这个Coroutine所对应的Future对象执行set_result操作，表示这个协程的Future已完成了，可以执行它的回调函数了。
这个回调函数对于outer_coroutine的future来说就是执行IOLoop的stop操作。对于inner_coroutine的future来说就是outer_coroutine对应的Runner的run操作。这句话很绕，但是要是真读懂了，相信对于它的运行原理也就了解的差不多了。
### IOLoop
IOLoop是一个很常见的模块，就是多路复用IO机制，好多项目中都有这一块的封装，原理都差不多。也可以参考 [shadowsocks](https://github.com/Long-live-shadowsocks/shadowsocks) 中的loop模块，它也是用python实现的基于多种不同操作系统io多路复用的封装。tornado的ioloop也是类似的，记录了一个个文件描述符和handler的pair，每当有io事件发生，就会调用该文件描述符对应的handler。如果这个handler是对future执行set_result操作，那连锁地就会执行Runner中的run，从而进入Runner的运行循环中，直到需要等待下一个异步事件，然后再向ioloop注册事件。。。如此循环往复。
### 总结
我讲的可能词不达意，毕竟我自己也是看了好多遍源码，才一步步理解清晰的。读者也不妨运行我的例子，逐步调试看看，说不定会有意想不到的收获。如果我哪些地方讲的欠妥当，也欢迎大家来指正。

