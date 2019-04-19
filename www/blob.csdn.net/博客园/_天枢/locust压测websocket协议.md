# locust压测websocket协议 - _天枢 - 博客园
## [locust压测websocket协议](https://www.cnblogs.com/yhleng/p/10025465.html)
2018-11-27 11:40 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=10025465)
Locust是以HTTP为主要目标构建的。
但是，通过编写触发器[`request_success`](https://docs.locust.io/en/latest/api.html#locust.events.request_success)和 [`request_failure`](https://docs.locust.io/en/latest/api.html#locust.events.request_failure)事件的自定义客户端，可以轻松扩展到任何基于请求/响应的系统的负载测试 。
我们知道locust默认内部只封装httplocust；使用的是requests中的session进行了封装；
那么问题来了，如果我想测试其它协议怎么办，比如websocket  , grpc等等。
以websoket为例：
在开始之前，需要了解的几点：
1>self.client: locust协议入口实例，我们只要重写一个实例给client即可。
2>以下两个事件，用来收集报告信息，否则写好后执行你会发现收集不到性能数据
```
events.request_failure.fire()
```
```
events.request_success.fire()
```
**具体步骤：**
**1>编写WebSocketClient类（主要用来替换掉self.client的http实例）**
```
class WebSocketClient(object):
    def __init__(self, host):
        self.host = host
        self.ws = websocket.WebSocket()
    def connect(self, burl):
        start_time = time.time()
        try:
            self.conn = self.ws.connect(url=burl)
        except websocket.WebSocketTimeoutException as e:
            total_time = int((time.time() - start_time) * 1000)
            **events.request_failure.fire**(request_type="websockt", name='urlweb', response_time=total_time, exception=e)
        else:
            total_time = int((time.time() - start_time) * 1000)
            **events.request_success.fire**(request_type="websockt", name='urlweb', response_time=total_time, response_length=0)
        return self.conn
    def recv(self):
        return self.ws.recv()
    def send(self, msg):
        self.ws.send(msg)
```
**注意：**该类中定义了，websocket的常用操作，链接、接收、发送；最主要是events.request_failure.fire和events.request_success.fire这两个用来收集性能数据，如果不写
报告收集不到性能数据。
上边代码只是以websocket举例，其它协议也一样，只要注意收集性能数据就可以。当然也可以不写在这里。这个看代码怎么写了。
**2>重写一个HttpLocust类，我们这里叫做WebsoketLoscust类**
```
class WebsocketLocust(Locust):
    def __init__(self, *args, **kwargs):
        super(WebsocketLocust, self).__init__(*args, **kwargs)
        self.client = WebSocketClient(self.host)
```
**注意：WebsocketLocust从Locust继承； 这里主要是将self.client重新实例成，我们第一部写好的websocketClient实例。  **
**这样一来我们通过self.client.xxxx就可以使用其方法**
**3>编写TaskSet类**
```
class SupperDianCan(TaskSet):
    @task
    def test_baidu(self):
        self.url = 'wss://xxxxxx.xxxx.com/cart/chat?sid=11303&table_no=103&user_id=ofZjWs40HxEzvV08l6m4PnqGbxqc_2_1_&version=2'
        self.data = {}
        self.client.connect(self.url)
        while True:
            recv = self.client.recv()
            print(recv)
            if eval(recv)['type'] == 'keepalive':
                self.client.send(recv)
            else:
                self.client.send(self.data)
```
**注意：此类就是任务类，跟http的写法一样，只是这里用的self.client.xxxx已经变成了我们自已重写的websocket类，将原来的requests http替换了。**
**4>编写站点类**
```
class WebsiteUser(WebsocketLocust):
    task_set = SupperDianCan
    min_wait=5000
    max_wait=9000
```
**注意：站点类从第二步中的locust继承**
**到这里代码编写完成，直接到cmd命令行执行**
```
locust -f supperdiancan.py --no-web -c 10 -r 3 --run-time 10s
```
参数：
-f   locust_file.py文件(locust任务文件)
-c   指定要生成的Locust用户数
-r   每秒生成的用户数
-n   用于0.8用于`-n`指定请求数
--run-time 或-t   指定测试的运行时间
注意：以上是以非web执行，当然你也可以用web执行。
以下是结果，从上可以看到，已经收集到了，请求数据。
```
Percentage of the requests completed within given times
 Name                                                           # reqs    50%    66%    75%    80%    90%    95%    98%    99%   100%
--------------------------------------------------------------------------------------------------------------------------------------------
 websockt urlweb                                                    10     48     50     59     65    540    540    540    540    540
--------------------------------------------------------------------------------------------------------------------------------------------
 Total                                                              10     48     50     59     65    540    540    540    540    540
```
以上并非是在分布式下运行，如要分布式，请参考其它。
官方参考文档，请穿越：[https://docs.locust.io/en/stable/testing-other-systems.html](https://docs.locust.io/en/stable/testing-other-systems.html)
