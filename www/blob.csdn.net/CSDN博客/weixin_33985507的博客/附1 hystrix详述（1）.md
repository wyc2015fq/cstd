# 附1 hystrix详述（1） - weixin_33985507的博客 - CSDN博客
2016年05月23日 21:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**一、hystrix的作用**
- 控制被依赖服务的延时和失败
- 防止在复杂系统中的级联失败
- 可以进行快速失败（不需要等待）和**快速恢复**（当依赖服务失效后又恢复正常，其对应的线程池会被清理干净，即剩下的都是未使用的线程，相对于整个 Tomcat 容器的线程池被占满需要耗费更长时间以恢复可用来说，此时系统可以快速恢复）
- getFallback（失败时指定的操作）和优雅降级
- 实现近实时的检测、报警、运维
**二、hystrix实现中需要注意的点**
- 为每一个依赖服务维护一个线程池（或者信号量），当线程池占满+queueSizeRejectionThreshold占满，该依赖服务将会立即拒绝服务而不是排队等待
- 引入『熔断器』机制，在依赖服务失效比例超过阈值时，手动或者自动地切断服务一段时间
- 当请求依赖服务时出现拒绝服务、超时或者短路（多个依赖服务顺序请求，前面的依赖服务请求失败，则后面的请求不会发出）时，执行该依赖服务的失败回退逻辑
**三、hystrix线程模型**
![](https://images2015.cnblogs.com/blog/866881/201605/866881-20160523192218022-2013399224.png)
- **为每一个服务提供一个线程池，线程池数量可以配置**。（**每个Netflix API实例有超过 40 个线程池**，每个线程池有 5 到 20 个工作线程（绝大部分设置为 10 个线程））
- 注意：如果不特殊指定ThreadPool，则commandGroupKey就是ThreadPool的配置。By default Hystrix uses this to define the command thread-pool unless a separate one is defined。
- 每个依赖服务都被隔离开来，Hystrix 会严格控制其在延迟发生时对资源的占用，并在任何失效发生时，执行失败回退逻辑
- 注意：隔离是非常好的，隔离后，延时的被调用服务只会耗尽自己的线程池，之后进入失败，见下图（附：[清晰大图](https://raw.githubusercontent.com/wiki/Netflix/Hystrix/images/request-example-with-latency-1280.png)）；如果不隔离，可能会耗尽整个tomcat的线程池，导致整个系统瘫痪。
- ![](https://images2015.cnblogs.com/blog/866881/201605/866881-20160523202508991-300344002.png)
- tomcat线程（即调用线程）将请求接过来后，交给被依赖服务自己的线程池中的线程去执行（此时调用线程就可以去做其他的事情了，这与mina2的线程模型有相似之处）![](https://images2015.cnblogs.com/blog/866881/201605/866881-20160523203020631-1702688822.png)
**四、hystrix执行流程（工作机理）**
![](https://images2015.cnblogs.com/blog/866881/201605/866881-20160523192801506-768041285.png)
附：[清晰大图](https://raw.githubusercontent.com/wiki/Netflix/Hystrix/images/hystrix-command-flow-chart.png)
**步骤**：（配合上图去看）
1、构建HystrixCommand或者HystrixObservableCommand对象
2、执行命令（execute()、queue()、observe()）
3、如果请求结果缓存这个特性被启用，并且缓存命中，则缓存的回应会立即通过一个 `Observable` 对象的形式返回。
4、检查熔断器状态，确定请求线路是否是开路，如果请求线路是开路，Hystrix 将不会执行这个命令，而是直接使用『失败回退逻辑』（即不会执行run()，直接执行getFallback()）
5、如果和当前需要执行的命令相关联的线程池和请求队列（或者信号量，如果不使用线程池）满了，Hystrix 将不会执行这个命令，而是直接使用『失败回退逻辑』（即不会执行run()，直接执行getFallback()）
6、执行`HystrixCommand.run()或HystrixObservableCommand.construct()，如果这两个方法执行超时或者执行失败，则执行getFallback()；如果正常结束，Hystrix 在添加一些日志和监控数据采集之后，直接返回`回应
7、Hystrix 会将请求成功，失败，被拒绝或超时信息报告给熔断器，熔断器维护一些用于统计数据用的计数器。
这些计数器产生的统计数据使得熔断器在特定的时刻，能短路某个依赖服务的后续请求，直到恢复期结束，若恢复期结束根据统计数据熔断器判定线路仍然未恢复健康，熔断器会再次关闭线路。
**五、熔断器执行流程（工作机理）**
![](https://images2015.cnblogs.com/blog/866881/201605/866881-20160523200649100-1073265393.png)
附：[清晰大图](https://raw.githubusercontent.com/wiki/Netflix/Hystrix/images/circuit-breaker-1280.png)
1、假设线路内的容量（请求QPS）达到一定阈值（通过 `HystrixCommandProperties.circuitBreakerRequestVolumeThreshold()` 配置），同时，假设线路内的错误率达到一定阈值（通过 `HystrixCommandProperties.circuitBreakerErrorThresholdPercentage()` 配置）
2、熔断器将从『闭路』转换成『开路』
3、若此时是『开路』状态，熔断器将短路后续所有经过该熔断器的请求，这些请求直接走『失败回退逻辑』（即不走run()，直接走getFallback()）
4、经过一定时间（即『休眠窗口』，通过 `HystrixCommandProperties.circuitBreakerSleepWindowInMilliseconds()` 配置），后续第一个请求将会被允许通过熔断器（此时熔断器处于『半开』状态），若该请求失败，熔断器将又进入『开路』状态，且在休眠窗口内保持此状态；若该请求成功，熔断器将进入『闭路』状态，回到逻辑1循环往复。
**六、线程池与信号量**
使用场景：对于那些本来延迟就比较小的请求（例如访问本地缓存成功率很高的请求）来说，线程池带来的开销是非常高的，这时，你可以考虑采用其他方法，例如非阻塞信号量（不支持超时），来实现依赖服务的隔离，使用信号量的开销很小。但**绝大多数情况下，Netflix 更偏向于使用线程池来隔离依赖服务**，因为其带来的额外开销可以接受，并且能支持包括超时在内的所有功能。
如果你对客户端库有足够的信任（延迟不会过高），并且你只需要控制系统负载，那么你可以使用信号量。
七、请求合并
参考：
[https://github.com/Netflix/Hystrix/wiki](https://github.com/Netflix/Hystrix/wiki)
[http://youdang.github.io/categories/%E7%BF%BB%E8%AF%91/](http://youdang.github.io/categories/%E7%BF%BB%E8%AF%91/) 对官网翻译的非常好，可惜没有翻译完
