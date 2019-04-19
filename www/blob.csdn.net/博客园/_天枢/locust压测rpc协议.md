# locust压测rpc协议 - _天枢 - 博客园
## [locust压测rpc协议](https://www.cnblogs.com/yhleng/p/10031209.html)
2018-11-28 11:42 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=10031209)
这里主要是google的grpc接口进行压测的一个栗子。
Locust是以HTTP为主要目标构建的。
但是，通过编写钩子触发器[`request_success`](https://docs.locust.io/en/latest/api.html#locust.events.request_success)和 [`request_failure`](https://docs.locust.io/en/latest/api.html#locust.events.request_failure)事件的自定义客户端，可以轻松扩展到任何基于请求/响应的系统的负载测试 。
我们知道locust默认内部只封装httplocust；使用的是requests中的session进行了封装；
那么问题来了，如果我想测试其它协议怎么办，比如websocket  , grpc等等。
以grpc为例：
在开始之前，需要了解的几点：
1>self.client: locust协议入口实例，我们只要重写一个实例给client即可。
2>以下两个钩子事件，用来收集报告信息，否则写好后执行你会发现收集不到性能数据
```
events.request_failure.fire()
```
```
events.request_success.fire()
```
**具体步骤：**
**1>编写GrpcClient类（主要用来替换掉self.client的http实例）**
```
import time
import grpc
from grpctest.snowid import snowid_pb2,snowid_pb2_grpc
from locust import (TaskSet,task,events,Locust)
from gevent._semaphore import Semaphore
all_locusts_spawned = Semaphore()
all_locusts_spawned.acquire()
def on_hatch_complete(**kwargs):
    all_locusts_spawned.release()
events.hatch_complete += on_hatch_complete
class GrpcClient(object):
    """重写self.client"""
    def __init__(self):
        self.ht = '172.17.31.220'
        self.pt = '50073'
    def connect(self):
        """grpc实例"""try:
            #记录开始时间
            start_time = int(time.time())
            #创建链接
            self.conn = grpc.insecure_channel(self.ht +':'+self.pt)
            self.cl = snowid_pb2_grpc.snowidStub(channel=self.conn)
            #参数实例
            args = snowid_pb2.GenerateSnowidRequest()
            args.uniqId = 10000 #此参数现在未起作用，可以为任意数字
            #调用
            res = self.cl.generateSnowid(args)
            total_time = int((time.time() - start_time) * 1000)
            if res.errCode != 0:
                raise Exception
            events.request_success.fire(
                request_type='grpc',
                name=r'/generateSnowid',
                response_time=total_time,
                response_length=0
            )
        except Exception as e:
            total_time = int((time.time() - start_time) * 1000)
            events.request_failure.fire(
                request_type='grpc',
                name='/generateSnowid',
                response_time=total_time,
                exception=e
            )
        return res
```
**注意：**该类中定义了，grpc的常用调用操作；最主要是events.request_failure.fire和events.request_success.fire这两个用来收集性能数据，如果不写
报告收集不到性能数据。
上边代码只是以grpc举例，其它协议也一样，只要注意收集性能数据就可以。当然也可以不写在这里。这个看代码怎么写了。
**2>重写一个HttpLocust类，我们这里叫做GrpcLocust类**
```
class GrpcLocust(Locust):
    def __init__(self, *args, **kwargs):
        super(GrpcLocust, self).__init__(*args, **kwargs)
        self.client = GrpcClient()
```
**注意：**GrpcLocust**从Locust继承； 这里主要是将self.client重新实例成，我们第一部写好的GrpcClient实例。  **
**这样一来我们通过self.client.xxxx就可以使用其方法**
**3>编写TaskSet类**
```
class GrpcTask(TaskSet):
    """压测任务"""
　　def on_start():
        all_locusts_spawned.wait()
    @task
    def generateSnowid(self):
        #grpc接口响应数据
        res = self.client.connect()
        # print('errCode:{}'.format(res.errCode))
        # print('result:{}'.format(res.result))
        # print('errMsg:{}'.format(res.errMsg))
```
**注意：**
**此类就是任务类，跟http的写法一样，只是这里用的self.client.xxxx已经变成了我们自已重写的Grpc类，将原来的requests http替换了。**
**另外在TaskSet类中可以定义def on_start()：方法来定义，执行压测任务，最先执行的方法。这个是重写taskset中的on_start方法。**
**4>编写站点类**
```
class WebsiteUser(GrpcLocust):
    task_set = GrpcTask
    min_wait = 5000
    max_wait = 9000
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
Name                                                          # reqs      # fails     Avg     Min     Max  |  Median   req/s
--------------------------------------------------------------------------------------------------------------------------------------------
 grpc /generateSnowid                                           22559     0(0.00%)     484       0    1007  |     480  673.60
--------------------------------------------------------------------------------------------------------------------------------------------
 Total                                                          22559     0(0.00%)                                     673.60
Percentage of the requests completed within given times
 Name                                                           # reqs    50%    66%    75%    80%    90%    95%    98%    99%   100%
--------------------------------------------------------------------------------------------------------------------------------------------
 grpc /generateSnowid                                            22559    480    640    740    790    890    950    980    990   1000
--------------------------------------------------------------------------------------------------------------------------------------------
 Total                                                           22559    480    640    740    790    890    950    980    990   1000
```
