# Tornado 文档学习：基础知识 - 宇宙浪子的专栏 - CSDN博客
2017年11月16日 18:03:58[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：238
转自：[http://blog.chriscabin.com/coding-life/web-framework/tornado/1592.html](http://blog.chriscabin.com/coding-life/web-framework/tornado/1592.html)
# 介绍
Tornado 是一个 Python web 框架和异步网络库，最开始由 FriendFeed 开发。使用非阻塞网络 I/O，Tornado 可以支撑数万个开放的连接，这让它非常适合做长耗时轮询，WebSockets 以及其它需要和用户维持长连接的应用。
Tornado 可以粗略地分成 4 个主要的组件：
1. 一个 web 框架（包含 `RequestHandler`，通过继承它来创建 web 应用，同时还有其它各种支持的类）；
2. 客户端和服务端 HTTP 实现（`HTTPServer` 和 `AsyncHTTPClient`）；
3. 一个异步的网络库，包括类 `IOLoop` 和 `IOStream`，它可以用来构建
 HTTP 组件部分，并且也能够用来实现其它的协议；
4. 一个 coroutine 库（`tornado.gen`），它允许我们以更加直接的方式来编写异步代码，而非链式回调。
Tornado web 框架和 HTTP 服务器扮演了 WSGI 的角色。我们既可以在 WSGI 容器（`WSGIAdapter`）使用 Tornado
 web 框架，也可以在其它 WSGI 框架（`WSGIContainer`）中将 Tornado HTTP 服务器当做一个容器，这两种组合都有一定的限制。为了充分利用
 Tornado 的优势，你需要将 Tornado 的 web 框架和 HTTP 服务器结合在一起使用。
# 异步与非阻塞 I/O
Web 的实时特性就需要与用户建立长存活、大部分空闲的连接。在传统的同步式 web 服务器中，这就意味着需要为每个用户开启一个线程，这是非常昂贵的。
为了减少并发连接的开销，Tornado 使用了单线程的事件循环机制。这样就意味着所有的应用程序代码都要成为异步和非阻塞的，因为某一时刻只能有一个操作可以进行。
术语异步和非阻塞意思听起来非常接近的，并且通常被交替使用，但实际上它们却并不相同。
## 阻塞
当一个函数在返回前需要等待时就会阻塞。阻塞函数原因有很多：网络 I/O、磁盘 I/O、信号量等。事实上，每个函数都会阻塞，哪怕只是一点点，因为每个函数运行时都会使用 CPU。某些情况下 CPU 阻塞需要格外关注，比如 `bcrypt` 加密函数，它就是会耗费数百毫秒的
 CPU 时间，这远比典型的网络或磁盘访问更耗死。
一个函数在某些情况下会阻塞而其它情况下并不会。例如 `tornado.httpclient` 默认配置时会在 DNS 查找时阻塞住（对于这种情况，
 我们可以使用 `ThreadedResolver` 或者一个 `tornado.curl_httpclient`，并搭配合理配置构建的 `libcurl`），但是在其它网络访问时不会。在
 Tornado 的上下文中，我们通常探讨的是网络 I/O 相关的阻塞，即便所有类型的阻塞都会减小。
## 异步
一个异步函数会在它结束前返回，应用通常会在触发一个未来动作之前需要在后台完成一些工作（和正常的同步函数相反，它们会在返回之前就做完一切）。有多种样式的异步接口：
1. 回调参数
2. 返回一个占位符 `Future`, `Promise`, `Defered`
3. 发送到一个队列
4. 回调注册（例如 POSIX 信号）
不管使用何种类型的接口，异步函数从定义上就和它们的调用者交互有所不同。并没有轻松的方法能让一个同步的函数透明的异步化（类似 gevent 的系统使用了微线程的方法提供了可与异步系统相比较的性能，但是它们并不会真的让一切异步化）。
## 示例
下面是一个简单的同步的函数：
- 
```python
from tornado.httpclient import HTTPClient
```
- 
```python
```
- 
```python
```
- 
```python
def synchronous_fetch(url):
```
- 
```python
client = HTTPClient()
```
- 
```python
response = client.fetch(url)
```
- 
```python
return response.body
```
下面则是使用异步回调参数方式重新的函数：
- 
```python
from tornado.httpclient import AsyncHTTPClient
```
- 
```python
```
- 
```python
def asynchronous_fetch(url, callback):
```
- 
```python
client = AsyncHTTPClient()
```
- 
```python
```
- 
```python
def handle_response(response):
```
- 
```python
callback(response.body)
```
- 
```python
```
- 
```python
client.fetch(url, callback=handle_response)
```
此外，我们可以使用 `Future` 代替回调：
- 
```python
from tornado.concurrent import Future
```
- 
```python
from tornado.httpclient import AsyncHTTPClient
```
- 
```python
```
- 
```python
def async_fetch_future(url):
```
- 
```python
client = AsyncHTTPClient()
```
- 
```python
future = Future()
```
- 
```python
fetch_future = client.fetch(url)
```
- 
```python
fetch_future.add_done_callback(
```
- 
```python
lambda f: future.set_result(f.result()))
```
- 
```python
return future
```
原生的 `Future` 版本相当复杂，但是 `Futures` 却是在
 Tornado 中非常推荐的一种实践，因为它们有两大优势。错误处理会更加一致，因为 `Future.result` 方法可以简单地抛出一个异常，并且 `Futures` 会很好地利用协程。协程会在下一节详细讨论。以下是协程版本，看起来和原始的同步版本很像：
- 
```python
from tornado import gen
```
- 
```python
```
- 
```python
def fetch_coroutine(url):
```
- 
```python
http_client = AsyncHTTPClient()
```
- 
```python
response = yield http_client.fetch(url)
```
- 
```python
raise gen.Return(response.body)
```
注意，上面的 `raise gen.Return(response.body)` 是在 Python 2 下使用的手段，在该版本中生成器是不能返回值的。因此，为了解决这个问题，Tornado
 的协程抛出一种特殊的异常 `Return`。协程在捕获到这个异常后，会把它当做一个返回值。在 Python 3.3+ 版本中，使用 
```
return
 response.body
```
 可以达到同样的目的。
# 协程
协程是 Tornado 中编写异步代码推荐的方式。协程使用了 Python 的 `yield` 关键字来暂停和恢复执行，而非使用链式回调的方式（像
 gevent 类似的框架提供的合作式轻量级线程有时候也叫协程，但是在 Tornado 中所有的协程都是显式进行上下文切换的，并且被叫作异步函数）。
协程和同步代码几乎一样简单，却不会使用额外的线程。通过减少上下文切换地方的数量，它们也会让并发更加轻松。
例子：
- 
```python
from tornado import gen
```
- 
```python
```
- 
```python
@gen.coroutine
```
- 
```python
def fetch_coroutine(url):
```
- 
```python
http_client = AsyncHTTPClient()
```
- 
```python
response = yield http_client.fetch(url)
```
- 
```python
# 在 Python 3.3 以前，生成器无法返回值，所以
```
- 
```python
# 需要使用 raise gen.Return(response.body)
```
- 
```python
# 来代替
```
- 
```python
return response.body
```
## Python 3.5: `async` 和 `await`
Python 3.5 开始提供了 `async` 和 `await` 关键字（使用了这些关键字的函数叫作
 “原生的协程”）。从 Tornado 4.3 开始，你就可以使用它们代替多数基于 `yield` 的协程了（详细的限制参见下文）。只需要使用
```
async
 def foo()
```
 替代使用 `@gen.coroutine` 的函数即可，然后使用 `await` 替代 `yield` 即可。本文档的后面依然会使用 `yield` 风格以兼容更旧的
 Python 版本，但是 `async` 和 `await` 在可能的情况下运行地更快：
- 
```python
async def fetch_coroutine(url):
```
- 
```python
http_client = AsyncHTTPClient()
```
- 
```python
response = await http_client.fetch(url)
```
- 
```python
return response.body
```
`await` 关键字相对 `yield` 来说有些限制。例如，在
 基于 `yield` 的协程中，你可以 `yield` 一个 `Futures` 列表，但是在
 native 协程中，你必须要将列表包裹到 `tornado.gen.multi` 中。这样会消除与 `concurrent.futures` 的集成。你可以使用 `tornado.gen.convert_yielded` 将任何可以使用 `yield` 的工作转换成 `await` 支持的形式：
- 
```python
async def f():
```
- 
```python
executor = concurrent.futures.ThreadPoolExecutor()
```
- 
```python
await tornado.gen.convert_yieled(executor.submit(g))
```
当原生协程并没有显式绑定到一个特定框架时（例如，它们不使用类似 `tornado.gen.coroutine` 或者`ayncio.coroutine`），并非所有的协程都是兼容的。
会有一个由第一个协程调用选择的协程运行器，并且所有直接使用 `await` 调用的协程都会共享该运行器。Tornado 的协程运行器天生就是灵活的，它可以支持来自任何框架的可`awaitable` 的对象；而其它的协程运行器可能会有更多限制（例如 `asyncio` 的协程运行器不会接受来自其它框架的协程）。因此，非常推荐在组合多种框架时使用
 Tornado 的协程运行器。为了能从已经使用 `asyncio` 运行器的协程中使用 Tornado 运行器调用协程，可以使用 `tornado.platform.asyncio.to_asyncio_future` 适配器。
## 工作机制
包含 `yield` 的函数就是一个生成器。所有的生成器都是异步的；当调用时，它们会返回一个生成器对象而非运行到结束。`@gen.coroutine` 装饰器通过 `yield` 表达式与生成器沟通，并通过向协程调用者返回一个 `Future` 来沟通。
以下是一个简化版本的协程装饰器的内部循环：
- 
```python
def run(self):
```
- 
```python
# send(x) 会让当前的 yield 返回 x。
```
- 
```python
# 它会在下一个 yield 到达时返回
```
- 
```python
future = self.gen.send(self.next)
```
- 
```python
def callback(f):
```
- 
```python
self.next = f.result()
```
- 
```python
self.run()
```
- 
```python
future.add_done_callback(callback)
```
装饰器会从生成器中接受一个 `Future`，等待（不阻塞）`Future` 运行完成，然后“解包” `Future`，并将结果返回给生成器，作为 `yield` 表达式的结果。多数异步代码从不会直接触及 `Future` 类，除非立即将一个异步函数返回的结果传递给 `yield` 表达式。
## 如何调用协程
协程不会像常规的方式那样抛出异常：任何异常都会被陷在 `Future` 中，直到 `yield` 出来。这就意味着以正确的方式调用协程是非常重要的，否则你将会遇到一些容易忽略的错误：
- 
```python
@gen.coroutine
```
- 
```python
def divide(x, y):
```
- 
```python
return x / y
```
- 
```python
```
- 
```python
def bad_call():
```
- 
```python
divide(1, 0)
```
几乎在所有的情况下，任何调用协程者自身都得是一个协程，并且在调用中使用 `yield` 关键字。当你正在重载一个父类中的方法时，一定要去看看文档，以确定是否可以使用 `coroutine`（文档应该会说该方法“可能是一个协程”或者“可能返回一个 `Future`”）：
- 
```python
@gen.coroutine
```
- 
```python
def good_call():
```
- 
```python
yield divide(1, 0)
```
有时候你可能想要“发射后不管”，也就是调用一个协程后而不会等待它的结果。这种情况下，我们建议你使用 `IOLoop.spawn_callback`，这样会让 `IOLoop` 负责调用。如果调用失败，`IOLoop` 会通过日志输出堆栈追踪：
- 
```python
IOLoop.current().spawn_callback(divide, 1, 0)
```
对于使用 `@gen.coroutine` 的函数，使用 `IOLoop.spawn_callback` 是推荐的方式，但对于使用了 
```
async
 def
```
 的函数则是必须的（否则协程运行器不会启动）。
最终，从应用的高层次来看，如果 `IOLoop` 还没有运行，你可以启动 `IOLoop`，运行协程，然后使用`IOLoop.run_sync` 方法停止 `IOLoop`。这种通常在脚本编程中使用：
- 
```python
IOLoop.current().run_sync(lambda: divide(1, 0))
```
## 协程实践模式
### 与回调交互
对于使用回调而非 `Future` 交互的异步代码，将调用包裹到 `Task` 中即可。这会为你添加回调参数，并且返回一个你可以 `yield` 的 `Future`：
- 
```python
@gen.coroutine
```
- 
```python
def call_task():
```
- 
```python
# 会将 Task 翻译成 `some_function(other_args, callback=callback)`
```
- 
```python
yield gen.Task(some_function, other_args)
```
### 调用阻塞的函数
从一个协程中调用一个阻塞函数最简单的方式是使用 `ThreadPoolExecutor`，这会返回与协程兼容的 `Futures`。
- 
```python
thread_pool = ThreadPoolExecutor(4)
```
- 
```python
```
- 
```python
@gen.coroutine
```
- 
```python
def call_blocking():
```
- 
```python
yield thread_pool.submit(blocking_func, args)
```
### 并行 Parallelism
协程装饰器可以识别出值为 `Futures` 的列表和字典，并且会并行等待这些 `Futures`：
- 
```python
@gen.coroutine
```
- 
```python
def parallel_fetch(url1, url2):
```
- 
```python
resp1, resp2 = yield [http_client.fetch(url1), http_client.fetch(url2)]
```
- 
```python
```
- 
```python
@gen.coroutine
```
- 
```python
def parallel_fetch_many(urls):
```
- 
```python
responses = yield [http_client.fetch(url) for url in urls]
```
- 
```python
```
- 
```python
@gen.coroutine
```
- 
```python
def parallel_fetch_dict(urls):
```
- 
```python
responses = yield {url: http_client.fetch(url) for url in urls}
```
### 交替 Interleaving
有时候在 yield 一个 `Future` 之前保存它会非常有用，这样你可以在等待前启动其它的操作：
- 
```python
@gen.coroutine
```
- 
```python
def get(self):
```
- 
```python
fetch_future = self.fetch_next_chunk()
```
- 
```python
while True:
```
- 
```python
chunk = yield fetch_future
```
- 
```python
if chunk is None:
```
- 
```python
break
```
- 
```python
```
- 
```python
self.write(chunk)
```
- 
```python
fetch_future = self.fetch_next_chunk()
```
- 
```python
yield self.flush()
```
上述模式对于 `@gen.coroutine` 很适用。如果 `fetch_next_chunk()` 使用了 
```
async
 def
```
，那么必须要调用`fetch_future = tornado.gen.convert_yielded(self.fetch_next_chunk())` 启动后台处理。
### 循环 Looping
在协程中使用循环需要一定的技巧，因为没有办法能够在 `for` 或者 `while` 循环进行每轮迭代时 `yield` 并获取到`yield` 的结果。因此，你需要将循环条件和获取结果分开，以下的例子来自 [Motor](https://motor.readthedocs.io/en/stable/)：
- 
```python
import motor
```
- 
```python
```
- 
```python
db = motor.MotorClient().test
```
- 
```python
```
- 
```python
@gen.coroutine
```
- 
```python
def loop_example(collection):
```
- 
```python
cursor = db.collection.find()
```
- 
```python
while (yield cursor.fetch_next):
```
- 
```python
doc = cursor.next_object()
```
### 后台运行
`PeriodicCallback` 实际上在协程中并不常用，相反，我们可以在 
```
while
 True:
```
 循环中使用 `tornado.gen.sleep`：
- 
```python
@gen.coroutine
```
- 
```python
def minute_loop():
```
- 
```python
while True:
```
- 
```python
yield do_something()
```
- 
```python
yield gen.sleep(60)
```
- 
```python
```
- 
```python
# 需要永久运行的 coroutine 通常使用 spawn_callback() 启动
```
- 
```python
IOLoop.current().spawn_callback(minute_loop)
```
有时候需要一个更加复杂的循环。例如，上面的循环会每 `60+N` 秒运行一次，`N` 是 `do_something()` 函数运行时间。如果想要更加精确点，需要使用下面的模式：
- 
```python
@gen.coroutine
```
- 
```python
def minute_loop_exact(seconds):
```
- 
```python
while True:
```
- 
```python
nxt = gen.sleep(seconds) # 启动时钟
```
- 
```python
yield do_something() # 在时钟计数时运行
```
- 
```python
yield nxt # 等待定时器超时
```
# 队列示例：一个并发的网络爬虫
Tornado 的 `tornado.queues` 模块为协程实现了一个异步的生产者/消费者模式，该模块功能和为线程实现的`queue` 标准库模块类似。
一个调用了 `yield Queue.get` 的协程会在队列中存在结果前暂停。如果队列设置了最大容量，则调用了 
```
yield
 Queue.put
```
 的协程会在队列没有空间时暂停。
`Queue` 维护了未完成任务的计数，其初始值为 0。`put` 会增加计数；`task_done` 则会减少计数。
在下面的爬虫示例中，队列起始只有一个 `base_url`。当 `worker` 访问了一个页面后，它会将解析出来的新链接放到队列中，然后调用 `task_done` 将计数器减
 1。最终，下载器会将所有独一无二的链接访问完，并且队列也会变空。`main` 协程会一直 `join` 等待任务完成。
- 
```python
import time
```
- 
```python
from html.parser import HTMLParser
```
- 
```python
```
- 
```python
from datetime import timedelta
```
- 
```python
from tornado import gen, ioloop, httpclient, queues
```
- 
```python
from urllib.parse import urljoin, urldefrag
```
- 
```python
```
- 
```python
base_url = 'http://www.tornadoweb.org/en/stable/'
```
- 
```python
concurrency = 15
```
- 
```python
```
- 
```python
@gen.coroutine
```
- 
```python
def main():
```
- 
```python
queue = queues.Queue()
```
- 
```python
start = time.time()
```
- 
```python
```
- 
```python
fetching, fetched = set(), set()
```
- 
```python
```
- 
```python
@gen.coroutine
```
- 
```python
def fetch():
```
- 
```python
url = yield queue.get()
```
- 
```python
```
- 
```python
try:
```
- 
```python
if url in fetched:
```
- 
```python
return
```
- 
```python
```
- 
```python
print('fetching: {}'.format(url))
```
- 
```python
fetching.add(url)
```
- 
```python
urls = yield get_links_from_url(url)
```
- 
```python
fetched.add(url)
```
- 
```python
```
- 
```python
for url_ in urls:
```
- 
```python
if url_.startswith(base_url):
```
- 
```python
yield queue.put(url_)
```
- 
```python
finally:
```
- 
```python
queue.task_done()
```
- 
```python
```
- 
```python
@gen.coroutine
```
- 
```python
def worker():
```
- 
```python
while True:
```
- 
```python
yield fetch()
```
- 
```python
```
- 
```python
queue.put(base_url)
```
- 
```python
for _ in range(concurrency):
```
- 
```python
worker()
```
- 
```python
```
- 
```python
yield queue.join(timedelta(seconds=300))
```
- 
```python
assert fetching == fetched
```
- 
```python
print('Done in {} seconds, fetched {} URLs.'.format(
```
- 
```python
time.time() - start, len(fetched)))
```
- 
```python
```
- 
```python
```
- 
```python
@gen.coroutine
```
- 
```python
def get_links_from_url(url):
```
- 
```python
"""Download page for `url` and parse links from it."""
```
- 
```python
try:
```
- 
```python
response = yield httpclient.AsyncHTTPClient().fetch(url)
```
- 
```python
print('fetched: {}'.format(url))
```
- 
```python
html = response.body if isinstance(response.body, str) \
```
- 
```python
else response.body.decode()
```
- 
```python
urls = [urljoin(url, remove_fragment(new_url))
```
- 
```python
for new_url in get_links(html)]
```
- 
```python
return urls
```
- 
```python
except Exception as err:
```
- 
```python
print('Error: {}, {}'.format(url, err))
```
- 
```python
return []
```
- 
```python
```
- 
```python
```
- 
```python
def remove_fragment(url):
```
- 
```python
pure_url, _ = urldefrag(url)
```
- 
```python
return pure_url
```
- 
```python
```
- 
```python
```
- 
```python
def get_links(html):
```
- 
```python
class URLSeeker(HTMLParser):
```
- 
```python
def __init__(self):
```
- 
```python
HTMLParser.__init__(self)
```
- 
```python
self.urls = []
```
- 
```python
```
- 
```python
def handle_starttag(self, tag, attrs):
```
- 
```python
href = dict(attrs).get('href')
```
- 
```python
if href and tag == 'a':
```
- 
```python
self.urls.append(href)
```
- 
```python
```
- 
```python
url_seeker = URLSeeker()
```
- 
```python
url_seeker.feed(html)
```
- 
```python
return url_seeker.urls
```
- 
```python
```
- 
```python
```
- 
```python
if __name__ == '__main__':
```
- 
```python
import logging
```
- 
```python
```
- 
```python
logging.basicConfig()
```
- 
```python
io_loop = ioloop.IOLoop.current()
```
- 
```python
io_loop.run_sync(main)
```
