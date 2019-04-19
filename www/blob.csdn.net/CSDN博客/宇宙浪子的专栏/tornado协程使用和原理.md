# tornado协程使用和原理 - 宇宙浪子的专栏 - CSDN博客
2017年09月19日 18:01:16[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：3036
转自：[http://blog.csdn.net/happyAnger6/article/details/51277407](http://blog.csdn.net/happyAnger6/article/details/51277407)
Python3.5  async和await
async和await是python3.5引入的2个新的关键字（用这两个关键字编写的函数也称之为"原生协程"）.
从tornado4.3开始，你可以在使用yield的tornado协程中使用这两个关键字。只需将原来用@gen.coroutine装饰的函数定义成async def func(),并将原来yield语句改为await即可。
本文的后面部分为了和老版本的python兼容将会继续使用yield关键字，但是使用async和await将会更快。
举例：
from tornado.ioloop import IOLoop
from tornado.httpclient import AsyncHTTPClient
async def fetch_coroutine():
    http_cli = AsyncHTTPClient()
    reponse = await http_cli.fetch("http://www.baidu.com")
    print(reponse.body)
IOLoop.instance().run_sync(fetch_coroutine)yield关键字比await关键字更加通用。比如，在基于yield的协程中你可以yield一个Future的列表,但是在原生的协程中要想这么做，你必须用tornado.gen.multi来装饰Future的列表。另外，你可以使用tornado.gen.convert_yielded来装饰任何可以用yield关键字工作的对象，这样你就可以在await中使用它们。尽管原生的协程不与任何特定的框架绑定（比如tornado框架,原生协程也不需要额外的tornado.gen.coroutine或者asyncio.coroutine装饰器来装饰）.但是不同实现的协程之间是不兼容的。当第一个协程被调用时会选择一个协程的执行体，然后这个执行体会被通过await调用的其它协程所共享。tornado的协程执行体被设计的十分通用，它可以接受其它任何框架的await的对象。其它的协程实现可能有局限性（比如asyncio协程的执行体不能接受其它框架的协程）。因此，我们建议使用tornado的协程执行体，这样你就可以在你的应用中使用不同框架的协程。如果你已经使用了asyncio执行体来执行协程，这时你可以通过tornado.platform.asyncio.to_asyncio_future将tornado的协程适配成可以在asyncio中执行的协程。举例：下面的例子演示了如何asyncio中执行tornado的协程：注意：执行ASyncIOMainLoop().install是为了初始化ioLoop为asyncio的ioLoop.from tornado import gen
from tornado.platform.asyncio import to_asyncio_future,AsyncIOMainLoop
from tornado.httpclient import AsyncHTTPClient
import asyncio
@gen.coroutine
def tornado_coroutine():
    cli = AsyncHTTPClient()
    response = yield cli.fetch("http://www.baidu.com")
    print(response.body)
AsyncIOMainLoop().install()
asyncio.get_event_loop().run_until_complete(to_asyncio_future(tornado_coroutine()))
tornado协程的工作原理包含yield语句的函数是一个生成器。所有的生成器都是异步的。当我们调用生成器函数的时候，生成器函数返回一个生成器对象，而不是像普通函数那样直接执行完成。@gen.coroutine装饰器通过yield表达式来和生成器对象交互，调用@gen.coroutine装饰的函数会返回一个Future.下面是一个coroutine装饰器内部循环的简化实现：可以看到，coroutine装饰器内部有一个Runner对象，这个对象将我们的生成器函数包装为self.gen.这个对象就是协程的运行体，它会在内部一直循环运行生成器，直到生成器函数阻塞返回一个Future或者结束。如果遇到yield返回的Future,便通过Future对象的结束回调函数来继续运行生成器函数。具体实现是:1.当生成器函数运行到yield时，会得到一个Future.为这个Future添加结束回调通知函数callback.执行体释放执行权给ioLoop继续执行其它协程。2.当这个Future异步运行结束时，会调用callback函数。callback函数得到Future的运行结果，并将结果通过生成器的send方法发送给生成器。这样生成器函数就会在yield的地方返回Future的异步执行结果并继续运行。3.1接着生成器函数继续运行,直到生成器函数运行结束3.2或者遇到下一个yield转到步骤1# Simplified inner loop of tornado.gen.Runnerdefrun(self):# send(x) makes the current yield return x.# It returns when the next yield is reachedfuture=self.gen.send(self.next)defcallback(f):self.next=f.result()self.run()future.add_done_callback(callback)
如何调用一个协程协程并不以一般的方式产生异常。协程中产生的任何异常将会被Future包装起来直到它被yielded.下面代码展示了我们函数产生的异常是如何被包装到future中的:func为我们的函数。try:
    result = func(*args, **kwargs)
except (Return, StopIteration) as e:
    result = _value_from_stopiteration(e)
except Exception:
    future.set_exc_info(sys.exc_info())
    return future这意味着我们必须以正确的方式调用协程，否则你可能会忽略一些发生的错误。在大部分情况下，任何调用协程的函数本身也需要是一个协程，而且需要在调用另外一个协程的地方使用yield关键字。当你需要重写父类中的函数时，你需要翻阅相应的文档以确定函数是否允许被实现为一个协程，文档中需要说明函数需要是一个协程或者需要返回一个Future.下面的函数修正了上面的错误，在一个协程中通过yield关键字调用divide这个协程。@gen.coroutinedefgood_call():# yield will unwrap the Future returned by divide() and raise# the exception.yielddivide(,)有时候你仅仅是想执行一个协程并不关心其结果，这种情况建议你使用`IOLoop.``spawn_callback`(callback, *args, **kwargs)。这样如果协程执行失败，ioLoop会将调用栈记录到log中。# The IOLoop will catch the exception and print a stack trace in# the logs. Note that this doesn't look like a normal call, since# we pass the function object to be called by the IOLoop.IOLoop.current().spawn_callback(divide,,)
最后，如果在程序顶层，ioloop还没有运行，你可以通过run_sync方法开始ioloop的运行，并执行协程。run_sync常常用来执行main协程，main里面包含了一系列的协程。
# run_sync() doesn't take arguments, so we must wrap the# call in a lambda.IOLoop.current().run_sync(lambda:divide(,))注意：由于run_sync只接受一个函数参数，所以你需要通过lambda表达式传递函数参数，或者通过functools.partial来将函数变为偏函数。
IOLoop.current().run_sync(functools.partial(divide,,))
