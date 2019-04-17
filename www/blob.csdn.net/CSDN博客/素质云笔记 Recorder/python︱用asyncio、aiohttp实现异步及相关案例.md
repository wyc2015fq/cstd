# python︱用asyncio、aiohttp实现异步及相关案例 - 素质云笔记/Recorder... - CSDN博客





2018年02月03日 14:03:45[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3017








Asyncio 是并发（concurrency）的一种方式。对 Python 来说，并发还可以通过线程（threading）和多进程（multiprocessing）来实现。Asyncio 并不能带来真正的并行（parallelism）。当然，因为 GIL（全局解释器锁）的存在，Python 的多线程也不能带来真正的并行。 
![这里写图片描述](http://blog.wish7.xyz/2017/02/25/%E4%BD%BF%E7%94%A8asyncio%E5%92%8Caiohttp%E5%AE%9E%E7%8E%B0%E5%BC%82%E6%AD%A5IO/eventloop.png)

.
# 一、asyncio的异步

主要来源：[Python 的异步 IO：Asyncio 简介](https://segmentfault.com/a/1190000008814676)

## 1、定义协程

```python
import asyncio
async def hello1():
    print("1, Hello world!")
    #r = await asyncio.sleep(1)
    print("1, Hello again!")
    for i in range(5):
        print(i)

async def hello2():
    print("2, Hello world!")
    #r = await asyncio.sleep(1)
    print("2, Hello again!")
    for i in range(5,10):
        print(i)
```

协程于我的理解是跟yield 一致的，协程可以做哪些事。协程可以：
- 等待一个 future 结束
- 等待另一个协程（产生一个结果，或引发一个异常）
- 产生一个结果给正在等它的协程
- 引发一个异常给正在等它的协程 

.

## 2、运行协程

要让这个协程对象运行的话，有两种方式：
- 在另一个已经运行的协程中用 `await` 等待它
- 通过 `ensure_future` 函数计划它的执行 

简单来说，只有 loop 运行了，协程才可能运行。

```python
async def doSomething():
    ...
    pass


async def other_field():
    await doSomething()
    ...
    pass
```

async: 写在你要指定异步的方法def之前，等同于@asyncio.coroutine 

await: 写在调用此方法前，等同于yield from

```python
# 执行
loop = asyncio.get_event_loop()
loop.run_until_complete(hello1())

>>> 1, Hello world!
>>> 1, Hello again!
>>> 0
>>> 1
>>> 2
>>> 3
>>> 4
```

.

## 3、回调函数

回调函数， 执行且按照顺序， 假如协程是一个 IO 的读操作，等它读完数据后，我们希望得到通知，以便下一步数据的处理。这一需求可以通过往 future 添加回调来实现。

```python
def done_callback1(futu):  # futu是异步的函数名称
    print('Done1')

def done_callback2(futu):
    print('Done2')

futu = asyncio.ensure_future(hello1())
futu.add_done_callback(done_callback1) 
futu = asyncio.ensure_future(hello2())
futu.add_done_callback(done_callback2)

loop.run_until_complete(futu)

>>> 1, Hello world!
>>> 1, Hello again!
>>> 0
>>> 1
>>> 2
>>> 3
>>> 4
>>> 2, Hello world!
>>> 2, Hello again!
>>> 5
>>> 6
>>> 7
>>> 8
>>> 9
>>> Done1
>>> Done2
```

.

## 4、多个协程共同运行run_until_complete

有以下三种写法：

```
# 多个协程同步执行
# 第一种写法
loop.run_until_complete(asyncio.gather(hello1(), hello2()))

# 第二种写法
coros = [hello1(), hello2()]
loop.run_until_complete(asyncio.gather(*coros))

# 第三种写法
futus = [asyncio.ensure_future(hello1()),
             asyncio.ensure_future(hello2())]

loop.run_until_complete(asyncio.gather(*futus))
```

gather 起聚合的作用，把多个 futures 包装成单个 future，因为 loop.run_until_complete 只接受单个 future。 

run_until_complete 是一个阻塞（blocking）调用，直到协程运行结束，它才返回。这一点从函数名不难看出。 

run_until_complete 的参数是一个 future，但是我们这里传给它的却是协程对象，之所以能这样，是因为它在内部做了检查，通过 ensure_future 函数把协程对象包装（wrap）成了 future。 

.
## 5、一直执行的run_forever

```python
import functools
from retry import retry
from requests.exceptions import ConnectTimeout

async def do_some_work(loop, x):
    print('Waiting ' + str(x))
    await asyncio.sleep(x)
    print ('Done')

def done_callback(loop, futu):
    loop.stop()

loop = asyncio.get_event_loop()

futus = asyncio.gather(do_some_work(loop, 1), do_some_work(loop, 3))
futus.add_done_callback(functools.partial(done_callback, loop))

loop.run_forever()

>>> Waiting 3
>>> Waiting 1
>>> Done
>>> Done
```

loop.run_forever()执行之后，会按照顺序执行，完成之后，程序不会关闭，仍然处于开启状态。 

来一个例子：

```python
import asyncio
@asyncio.coroutine
def compute(x, y):
    print("Compute %s + %s ..." % (x, y))
    yield from asyncio.sleep(2.0)
    return x + y
@asyncio.coroutine
def print_sum(x, y):
    result = yield from compute(x, y)
    print("%s + %s = %s" % (x, y, result))
loop = asyncio.get_event_loop()
tasks = [print_sum(1, 2), print_sum(3, 4), print_sum(5, 6)]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()
# OUTPUT
Compute 3 + 4 ...
Compute 5 + 6 ...
Compute 1 + 2 ...
# 大约 1 秒以后
3 + 4 = 7
5 + 6 = 11
1 + 2 = 3
```

.

# 二、aiohttp

asyncio可以实现单线程并发IO操作。如果仅用在客户端，发挥的威力不大。如果把asyncio用在服务器端，例如Web服务器，由于HTTP连接就是IO操作，因此可以用单线程+coroutine实现多用户的高并发支持。（[廖雪峰](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/0014320981492785ba33cc96c524223b2ea4e444077708d000)） 

后续内容主要来源：[异步爬虫: async/await 与 aiohttp的使用，以及例子](http://blog.csdn.net/u014595019/article/details/52295642)

.
## 1、基本用法

```
with aiohttp.Timeout(0.001):
    async with aiohttp.get('https://github.com') as r:
        await r.text(encoding='windows-1251')
```

aiohttp中设置了timeout，aiohttp.get请求了github中的内容。 

.

## 2、session获取数据

aiohttp.ClientSession. 首先要建立一个session对象，然后用该session对象去打开网页。session可以进行多项操作，比如post, get, put, head。

```
async with aiohttp.ClientSession() as session:
    async with session.get('https://api.github.com/events') as resp:
        print(resp.status)
        print(await resp.text())
```

如果要使用post方法，则相应的语句要改成
`session.post('http://httpbin.org/post', data=b'data')`
官方例子：

```python
import aiohttp
import asyncio
import async_timeout
async def fetch(session, url):
    with async_timeout.timeout(10):
        async with session.get(url) as response:
            return await response.text()
async def main(loop):
    async with aiohttp.ClientSession(loop=loop) as session:
        html = await fetch(session, 'http://python.org')
        print(html)
loop = asyncio.get_event_loop()
loop.run_until_complete(main(loop))
```

.

# 三、asyncio、aiohttp结合案例

## 1、采集ReadHub案例

此案例来源：[如何实现一个自动抓取readhub的脚本](https://github.com/buzheng1949/Athena/blob/a59b177780278da390431ceae2de44857f506414/readhub/readhub.md)

Sanic是一个异步框架，为了更好的发挥它的性能，有些操作最好也要用异步的，    比如这里发起请求就必须要用异步请求框架aiohttp

```python
import aiohttp
async def get_news(size=10):
    all_news, readhub_api = [], "https://api.readhub.me/topic"
    #  # conn = aiohttp.ProxyConnector(proxy="http://127.0.0.1:8087")
    async with aiohttp.ClientSession() as client:
        headers = {'content-type': 'application/json'}
        params = {'pageSize': size}
        async with client.get(readhub_api, params=params, headers=headers) as response:  # 启动
            assert response.status == 200
            result = await response.json()  # 获取的数据

        for value in result.get('data', []):    # 稍微整理数据
            each_data = {}
            each_data['title'] = value.get('title')
            each_data['summary'] = value.get('summary')
            each_data['news_info'] = value.get('newsArray')
            each_data['updated_at'] = value.get('updatedAt')
            all_news.append(each_data)
        return all_news

async def index_json():
    nums = 2
    # 获取数据
    all_news = await get_news()
    return all_news
```

client.get中，可以设置参数params ，以及自定义header头（需以dict的形式），当然也可以设置代理conn 。 

在index_json()函数中，可以了解到，如何在另一个协程中使用前面一个协程，可以使用await ，且await 只在async （异步）中才有效。

参考：[使用asyncio和aiohttp实现异步IO](http://blog.wish7.xyz/2017/02/25/%E4%BD%BF%E7%94%A8asyncio%E5%92%8Caiohttp%E5%AE%9E%E7%8E%B0%E5%BC%82%E6%AD%A5IO/)

.

## 2、asyncio并发

来源：[Python黑魔法 — 异步IO（ asyncio） 协程](https://www.jianshu.com/p/b5e347b3a17c)

并发和并行一直是容易混淆的概念。并发通常指有多个任务需要同时进行，并行则是同一时刻有多个任务执行。

### 2.1 简单实现并发

asyncio实现并发，就需要多个协程来完成任务，每当有任务阻塞的时候就await，然后其他协程继续工作。创建多个协程的列表，然后将这些协程注册到事件循环中。

```python
import asyncio

import time

now = lambda: time.time()

async def do_some_work(x):
    print('Waiting: ', x)

    await asyncio.sleep(x)
    return 'Done after {}s'.format(x)

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
    print('Task ret: ', task.result())

print('TIME: ', now() - start)
```

结果：

```
Waiting:  1
Waiting:  2
Waiting:  4
Task ret:  Done after 1s
Task ret:  Done after 2s
Task ret:  Done after 4s
TIME:  4.003541946411133
```

总时间为4s左右。4s的阻塞时间，足够前面两个协程执行完毕。如果是同步顺序的任务，那么至少需要7s。此时我们使用了aysncio实现了并发。asyncio.wait(tasks) 也可以使用 asyncio.gather(*tasks) ,前者接受一个task列表，后者接收一堆task。

### 2.2 协程嵌套

```python
import asyncio

import time

now = lambda: time.time()

async def do_some_work(x):
    print('Waiting: ', x)

    await asyncio.sleep(x)
    return 'Done after {}s'.format(x)

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
        print('Task ret: ', task.result())

start = now()

loop = asyncio.get_event_loop()
loop.run_until_complete(main())

print('TIME: ', now() - start)
```

使用async可以定义协程，协程用于耗时的io操作，我们也可以封装更多的io操作过程，这样就实现了嵌套的协程，即一个协程中await了另外一个协程，如此连接起来。 

await的返回值就是协程运行的结果






