# 使用Envoy 作Sidecar Proxy的微服务模式-1.熔断 - weixin_33985507的博客 - CSDN博客
2019年02月22日 07:37:53[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
本博客是深入研究Envoy Proxy和Istio.io 以及它如何实现更优雅的方式来连接和管理微服务系列文章的一部分。
这是接下来几个部分的想法（将在发布时更新链接):
- 断路器（第一部分）
- 重试/超时（第二部分）
- 分布式跟踪（第三部分）
- Prometheus的指标收集（第四部分）
- rate limiter（第五部分）
### 第一部分 - 使用envoy proxy 熔断
这篇第一篇博文向您介绍了[Envoy Proxy实现的熔断](https://www.envoyproxy.io/docs/envoy/latest/intro/arch_overview/circuit_breaking.html#arch-overview-circuit-break)功能。有意进行一些简单的演示，因此我可以单独说明模式和用法。请下载此演示的[源代码](https://github.com/christian-posta/envoy-microservices-patterns)并按照说明进行操作！
该演示由一个客户端和一个服务组成。客户端是一个Java http应用程序，模拟对“上游”服务进行http调用（注意，我们在这里使用Envoys术语，并贯穿整个repo）。客户端打包在docker.io/ceposta/http-envoy-client:latest的Docker镜像中。除了http-client Java应用程序之外，还有Envoy Proxy的一个实例。在此部署模型中，Envoy被部署为服务的sidercar（在本例中为http客户端）。当http-client进行出站调用（到“上游”服务）时，所有调用都通过Envoy Proxy sidercar。
这些示例的“上游”服务是[httpbin.org](http://httpbin.org/)。 httpbin.org允许我们轻松模拟HTTP服务行为。它很棒，所以如果你没有看到它，请查看它。
![图片描述](https://image-static.segmentfault.com/308/881/3088810163-5c6fafe848b85_articlex)
这个熔断器演示有自己的[envoy.json](https://github.com/christian-posta/envoy-microservices-patterns/blob/master/circuit-breaker/conf/envoy.json)配置文件。我绝对建议您查看配置文件每个部分的[参考文档](https://www.envoyproxy.io/docs/envoy/latest/configuration/configuration)，以帮助理解完整配置。 datawire.io的优秀人员也为Envoy及其配置提供了一个很好的[介绍](https://www.datawire.io/envoyproxy/getting-started-lyft-envoy-microservices-resilience/)，你也应该检查一下。
### 运行 circuit-breaker demo
运行熔断器演示，请熟悉演示框架，然后运行：
```bash
./docker-run.sh -d circuit-breaker
```
熔断器的Envoy配置如下所示（请参阅此处的[完整配置](https://github.com/christian-posta/envoy-microservices-patterns/blob/master/circuit-breaker/conf/envoy.json)）：
```
"circuit_breakers": {
  "default": {
    "max_connections": 1,
    "max_pending_requests": 1,
    "max_retries": 3
  }
}
```
该配置文件允许我们实现下面的功能：
- 限制我们对上游集群的HTTP / 1连接的数量，如果我们超过设定限制则将它们短路。
- 限制排队/等待连接可用的请求数量，如果我们超过设定限制则将它们短路。
- 限制在任何给定时间的总并发重试次数（假设重试策略已到位）有效地实施重试配额。
我们来看看每个配置。我们现在将忽略最大重试次数设置有两个原因：
- 我们的设置并没有多大意义;我们不能有3个并发重试，因为我们只允许1个HTTP连接和1个排队请求。
- 我们实际上没有为此演示制定任何重试政策;我们可以在重试演示中看到重试。
无论如何，此处的重试设置允许我们避免大型重试风暴 - 在大多数情况下，这可能会在处理与群集中所有实例的连接时出现问题。这是一个重要的设置，我们将回到重试演示。
#### max_connections
让我们看看当应用程序中有太多线程试图与上游集群建立太多并发连接时，envoy会做什么。
回想一下我们的上游httbin集群的熔断设置如下所示（请参阅此处的[完整配置](https://github.com/christian-posta/envoy-microservices-patterns/blob/master/circuit-breaker/conf/envoy.json)）：
```
"circuit_breakers": {
  "default": {
    "max_connections": 1,
    "max_pending_requests": 1,
    "max_retries": 3
  }
}
```
如果我们查看./circuit-breaker/http-client.env设置文件，我们将看到最初我们将开始运行单个线程，该线程创建一个连接并进行五次调用并关闭。
```
NUM_THREADS=1
DELAY_BETWEEN_CALLS=0
NUM_CALLS_PER_CLIENT=5
URL_UNDER_TEST=http://localhost:15001/get
MIX_RESPONSE_TIMES=false
```
我们来验证一下。运行演示：
```bash
./docker-run.sh -d circuit-breaker
```
这将启动了客户端应用程序，并启动了Envoy Proxy。我们将直接向Envoy Proxy发送流量，以使其帮帮助处理熔断。让我们调用我们的服务：
```bash
docker exec -it client bash -c 'java -jar http-client.jar'
```
我们将看到以下的输出：
```bash
using num threads: 1
Starting pool-1-thread-1 with numCalls=5 delayBetweenCalls=0 url=http://localhost:15001/get mixedRespTimes=false
pool-1-thread-1: successes=[5], failures=[0], duration=[545ms]
```
我们也能看到我们五次的调用成功了。
让我们看一下，Envoy为我们收集的metrics指标：
```bash
./get-envoy-stats.sh
```
Envoy为我们采集了很多的追踪指标！让我们通过以下方式查看：
```bash
/get-envoy-stats.sh | grep cluster.httpbin_service
```
这将显示我们配置的名为httpbin_service的上游群集的度量标准。快速浏览一下这些统计数据，并在[Envoy文档](https://www.envoyproxy.io/docs/envoy/latest/configuration/cluster_manager/cluster_stats.html)中查找它们的含义。需要注意的重要事项在这里提到：
```
cluster.httpbin_service.upstream_cx_http1_total: 1
cluster.httpbin_service.upstream_rq_total: 5
cluster.httpbin_service.upstream_rq_200: 5
cluster.httpbin_service.upstream_rq_2xx: 5
cluster.httpbin_service.upstream_rq_pending_overflow: 0
cluster.httpbin_service.upstream_rq_retry: 0
```
这告诉我们我们有1个http / 1连接，有5个请求（总数），其中5个以HTTP 2xx（甚至200个）结束。大！但是如果我们尝试使用两个并发连接会发生什么？
首先，让我们重置统计数据：
```bash
./reset-envoy-stats.sh
OK
```
让我们用2个线程发起这些调用：
```bash
docker exec -it client bash -c 'NUM_THREADS=2; java -jar http-client.jar'
```
我们应该可以看到如下的输出：
```bash
using num threads: 2
Starting pool-1-thread-1 with numCalls=5 delayBetweenCalls=0 url=http://localhost:15001/get mixedRespTimes=false
Starting pool-1-thread-2 with numCalls=5 delayBetweenCalls=0 url=http://localhost:15001/get mixedRespTimes=false
pool-1-thread-1: successes=[0], failures=[5], duration=[123ms]
pool-1-thread-2: successes=[5], failures=[0], duration=[513ms]
```
我们启动的一个线程中有5个成功，但其中另外一个线程一个都没有成功！该线程的所有5个请求都失败了！让我们再看看envoy的统计数据：
```bash
./get-envoy-stats.sh | grep cluster.httpbin_service
```
我们将看到如下的输出：
```bash
cluster.httpbin_service.upstream_cx_http1_total: 1
cluster.httpbin_service.upstream_rq_total: 5
cluster.httpbin_service.upstream_rq_200: 5
cluster.httpbin_service.upstream_rq_2xx: 5
cluster.httpbin_service.upstream_rq_503: 5
cluster.httpbin_service.upstream_rq_5xx: 5
cluster.httpbin_service.upstream_rq_pending_overflow: 5
cluster.httpbin_service.upstream_rq_retry: 0
```
从这个输出中我们可以看到只有一个连接成功！我们最终得到5个请求，导致HTTP 200和5个请求以HTTP 503结束。我们还看到upstream_rq_pending_overflow已经增加到5.这表明断路器在这里完成了它的工作。它会使任何与我们的配置设置不匹配的调用短路。
我们将max_connections人为设置为一个小点的数字，在这种情况下为1，为了说明Envoy的断路功能。这不是一个现实的设置，但希望有助于说明这一点。
#### max_pending_requests
让我们运行一些类似的测试来运行max_pending_requests设置。
回想一下我们的上游httbin集群的熔断设置如下所示（请参阅此处的[完整配置](https://github.com/christian-posta/envoy-microservices-patterns/blob/master/circuit-breaker/conf/envoy.json)）：
```
"circuit_breakers": {
  "default": {
    "max_connections": 1,
    "max_pending_requests": 1,
    "max_retries": 3
  }
}
```
我们想要做的是模拟在单个HTTP连接上同时发生的多个请求（因为我们只允许max_connections为1）。我们期望请求排队，但是Envoy应该拒绝排队的消息，因为我们将max_pending_requests设置为1。我们想要设置队列深度的上限，目的不允许重试风暴，恶意下游请求，DoS和我们系统中的bug。
继续上一节，让我们重置特使的统计数据：
```bash
./reset-envoy-stats.sh
OK
```
让我们启动1个线程（即1个HTTP连接）调用客户端，但是并行发送我们的请求（默认情况下是5个批次）。我们还希望随机化我们发送的延迟，以便事情可以排队：
```bash
docker exec -it client bash -c 'NUM_THREADS=1 && PARALLEL_SENDS=true && MIX_RESPONSE_TIMES=true; java -jar http-client.jar'
```
我们应该看到如下的输出：
```bash
Starting pool-1-thread-1 with numCalls=5 parallelSends=true delayBetweenCalls=0 url=http://localhost:15001/get mixedRespTimes=true
pool-2-thread-3: using delay of : 3
pool-2-thread-2: using delay of : 0
pool-2-thread-1: using delay of : 2
pool-2-thread-4: using delay of : 4
pool-2-thread-5: using delay of : 0
finished batch 0
pool-1-thread-1: successes=[1], failures=[4], duration=[4242ms]
```
我们的四个要求失败了......让我们查看envoy的统计数据:
```bash
./get-envoy-stats.sh | grep cluster.httpbin_service | grep pending
```
果然，我们看到我们的4个请求被短路了：
```bash
cluster.httpbin_service.upstream_rq_pending_active: 0
cluster.httpbin_service.upstream_rq_pending_failure_eject: 0
cluster.httpbin_service.upstream_rq_pending_overflow: 4
cluster.httpbin_service.upstream_rq_pending_total: 1
```
#### 什么时候服务完全停止？
我们已经看到了Envoy对集群的短路和批量处理线程有什么断路设施，但是如果集群中的节点完全崩溃（或者似乎下降）怎么办？
Envoy具有“离群值检测”设置，可以检测群集中的主机何时不可靠，并且可以完全从群集摘掉它们（一段时间）。需要了解的一个有趣现象是，默认情况下，Envoy会根据负载平衡算法，最多摘除某一数量的不可靠的主机。如果太多（即> 50％）的主机被认为是不健康的，那么Envoy的负载均衡器算法将检测到一个恐慌阈值，并且只会对所有主机进行负载均衡。此恐慌阈值是可配置的，并且为了获得断电功能，可以在严重中断期间为所有主机提供负载（一段时间），您可以配置异常值检测设置。在我们的示例断路器）envoy.json配置中，您可以看到以下内容：
```
outlier_detection" : {
      "consecutive_5xx": 5,
      "max_ejection_percent": 100,
      "interval_ms": 3
    }
```
让我们测试一下这个案例，看看会发生什么。首先，重置统计数据：
```bash
./reset-envoy-stats.sh
OK
```
接下来，让我们使用一个URL来调用我们的客户端，该URL将返回HTTP 500结果。我们将发起十次调用，因为我们的异常检测将检查5个连续的5xx响应，因此我们将要发起多于5次的调用。
```bash
docker exec -it client bash -c 'URL_UNDER_TEST=http://localhost:15001/status/500 && NUM_CALLS_PER_CLIENT=10; java -jar http-client.jar'
```
我们应该看到这样的响应，其中所有调用都失败了（正如我们所期望的那样：其中至少有5个会返回HTTP 500）：
```bash
using num threads: 1
Starting pool-1-thread-1 with numCalls=10 parallelSends=false delayBetweenCalls=0 url=http://localhost:15001/status/500 mixedRespTimes=false
pool-1-thread-1: successes=[0], failures=[10], duration=[929ms]
```
现在让我们检查一下Envoy的统计数据，看看究竟发生了什么：
```bash
./get-envoy-stats.sh | grep cluster.httpbin_service | grep outlier
```
```bash
cluster.httpbin_service.outlier_detection.ejections_active: 0
cluster.httpbin_service.outlier_detection.ejections_consecutive_5xx: 1
cluster.httpbin_service.outlier_detection.ejections_overflow: 0
cluster.httpbin_service.outlier_detection.ejections_success_rate: 0
cluster.httpbin_service.outlier_detection.ejections_total: 1
```
我们可以看到我们断电了连续5xx检测！我们还从负载均衡组中删除了该主机。
