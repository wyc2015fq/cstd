# 记一次线上压测Dubbo线程池队列满的问题 - 江南烟雨 - CSDN博客
2018年09月09日 14:23:41[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3521
本文记录一次线上全链路压测出现的Dubbo线程池队列满的问题。
# 1 问题描述
线上做全链路压测，其中涉及三个系统，调用关系A->B->C，均是dubbo调用。压测的时候C出现CPU满导致服务响应超时的情况，进而导致B以及A接口均超时。停止压测后，B->C的流量依然未有明显降低，系统收敛慢，影响线上业务。
# 2 问题分析
## 2.1 调用来源分析
首先分析停止压测后，这些B对C的调用的来源是来自哪里。
根据全链路的traceId，发现这些调用是来自系统A的调用，但是既然是停止压测了，那A怎么还会有这些对B的调用？
从时间上看，A系统的这条traceId的日志要比B系统的这条traceId的日志早整整12分钟。也就是说，压测期间的A对B的调用，在B系统上被延迟了12分钟后执行。
这个时候初步可以判断和dubbo的线程池队列有关。
## 2.2 日志分析
在A系统上搜日志，发现有线程池耗尽的异常日志：
|12345|Caused by: java.util.concurrent.RejectedExecutionException: Thread pool is EXHAUSTED! Thread Name: DubboServerHandler-XX.XX.XX.XX:XXXX, Pool Size: 200 (active: 200, core: 200, max: 200, largest: 200), Task: 39522284 (completed: 39422084), Executor status:(isShutdown:false, isTerminated:false, isTerminating:false), in dubbo://XX.XX.XX.XX:XXXX!at com.alibaba.dubbo.common.threadpool.support.AbortPolicyWithReport.rejectedExecution(AbortPolicyWithReport.java:53)at java.util.concurrent.ThreadPoolExecutor.reject(ThreadPoolExecutor.java:830)at java.util.concurrent.ThreadPoolExecutor.execute(ThreadPoolExecutor.java:1379)at com.alibaba.dubbo.remoting.transport.dispatcher.all.AllChannelHandler.received(AllChannelHandler.java:56)|
通过IP看，是来自于系统B，说明A收到了B的dubbo线程池满的错误信息。
## 2.3 Dubbo线程池原理
dubbo的provider有2种线程池：
- IO处理线程池。（直接通过netty等来配置）
- 服务调用线程池。
**netty线程模型**
![Dubbo线程模型](http://o8sltkx20.bkt.clouddn.com/dubbo-001.jpeg)Dubbo线程模型
boss线程:
accept客户端的连接；
将接收到的连接注册到一个worker线程上
个数：
通常情况下，服务端每绑定一个端口，开启一个boss线程
worker线程:
处理注册在其身上的连接connection上的各种io事件
个数：
默认是：核数+1
注意：
一个worker线程可以注册多个connection
**Dubbo的事件派发策略**
默认是all：
|1|dispatcher dispatcher string 可选 dubbo协议缺省为all 性能调优 协议的消息派发方式，用于指定线程模型，比如：dubbo协议的all, direct, message, execution, connection等 2.1.0以上版本|
- all：
	所有消息都派发到线程池，包括请求，响应，连接事件，断开事件，心跳等。 即worker线程接收到事件后，将该事件提交到业务线程池中，自己再去处理其他事。
![Dubbo事件派发策略-all](http://o8sltkx20.bkt.clouddn.com/dubbo-003.png)Dubbo事件派发策略-all
- direct：
	worker线程接收到事件后，由worker执行到底。
- message：
	只有请求响应消息派发到线程池，其它连接断开事件，心跳等消息，直接在 IO线程上执行
- execution：
	只请求消息派发到线程池，不含响应（客户端线程池），响应和其它连接断开事件，心跳等消息，直接在 IO 线程上执行
- connection：
	在 IO 线程上，将连接断开事件放入队列，有序逐个执行，其它消息派发到线程池。
**服务调用调用线程池**
对于服务调用线程池，dubbo默认使用的是固定大小线程池，[官方文档-dubbo:protocol](http://dubbo.apache.org/zh-cn/docs/user/references/xml/dubbo-protocol.html)：
|1|threadpool threadpool string 可选 fixed 性能调优 线程池类型，可选：fixed/cached 2.0.5以上版本|
源代码：
|1234567891011121314151617181920|/*** 此线程池启动时即创建固定大小的线程数，不做任何伸缩，来源于：<code>Executors.newFixedThreadPool()</code>** @see java.util.concurrent.Executors#newFixedThreadPool(int)* @author william.liangf*/public class FixedThreadPool implements ThreadPool {public Executor getExecutor(URL url) {String name = url.getParameter(Constants.THREAD_NAME_KEY, Constants.DEFAULT_THREAD_NAME);int threads = url.getParameter(Constants.THREADS_KEY, Constants.DEFAULT_THREADS);int queues = url.getParameter(Constants.QUEUES_KEY, Constants.DEFAULT_QUEUES);return new ThreadPoolExecutor(threads, threads, 0, TimeUnit.MILLISECONDS,queues == 0 ? new SynchronousQueue<Runnable>() :(queues < 0 ? new LinkedBlockingQueue<Runnable>(): new LinkedBlockingQueue<Runnable>(queues)),new NamedThreadFactory(name, true), new AbortPolicyWithReport(name, url));}}|
可以看到这里队列的默认大小是0.拒绝策略：
|1234567891011121314151617181920212223242526|public class AbortPolicyWithReport extends ThreadPoolExecutor.AbortPolicy {protected static final Logger logger = LoggerFactory.getLogger(AbortPolicyWithReport.class);private final String threadName;private final URL url;public AbortPolicyWithReport(String threadName, URL url) {this.threadName = threadName;this.url = url;}@Overridepublic void rejectedExecution(Runnable r, ThreadPoolExecutor e) {String msg = String.format("Thread pool is EXHAUSTED!" +" Thread Name: %s, Pool Size: %d (active: %d, core: %d, max: %d, largest: %d), Task: %d (completed: %d)," +" Executor status:(isShutdown:%s, isTerminated:%s, isTerminating:%s), in %s://%s:%d!" ,threadName, e.getPoolSize(), e.getActiveCount(), e.getCorePoolSize(), e.getMaximumPoolSize(), e.getLargestPoolSize(),e.getTaskCount(), e.getCompletedTaskCount(), e.isShutdown(), e.isTerminated(), e.isTerminating(),url.getProtocol(), url.getIp(), url.getPort());logger.warn(msg);throw new RejectedExecutionException(msg);}}|
超过队列大小会直接丢弃请求，并打印错误日志，与之前看到的A系统中的异常日志吻合。
dubbo处理业务请求示意图：
![Dubbo线程模型](http://o8sltkx20.bkt.clouddn.com/dubbo-002.png)Dubbo线程模型
奇怪的是，dubbo官方默认队列大小是0，为何会出现这么多请求延迟的问题？原来是公司的dubbo版本做了定制，将队列大小默认设置为10W。
# 3 解决方案
对于当前的业务场景，在下游服务异常时需要快速失败，于是将队列大小改为比较小的值。
# 4 参考资料
- [Dubbo源代码分析八：再说Provider线程池被EXHAUSTED](http://manzhizhen.iteye.com/blog/2391177)
- [dubbo线程模型](https://www.cnblogs.com/java-zhao/p/7822766.html)
- [http://ifeve.com/dubbo-threadmodel/](http://ifeve.com/dubbo-threadmodel/)
**欢迎关注微信公众号获取更多Java相关资源：SimpleJava**
