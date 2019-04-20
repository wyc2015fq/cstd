# Spring Cloud微服务-全栈技术与案例解析（文末送此书！） - 纯洁的微笑 - CSDN博客
2018年07月21日 08:36:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：2539
在Spring Cloud中我们用Hystrix来实现断路器，默认是用信号量来进行隔离的，我们可以通过配置使用线程方式隔离。
在使用线程隔离的时候，有个问题是必须要解决的，那就是在某些业务场景下通过ThreadLocal来在线程里传递数据，用信号量是没问题的，从请求进来，但后续的流程都是通一个线程。
当隔离模式为线程时，Hystrix会将请求放入Hystrix的线程池中去执行，这个时候某个请求就有A线程变成B线程了，ThreadLocal必然消失了。
下面我们通过一个简单的列子来模拟下这个流程：
```
public class CustomThreadLocal {
    static ThreadLocal<String> threadLocal = new ThreadLocal<>();
    public static void main(String[] args) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                CustomThreadLocal.threadLocal.set("猿天地");
                new Service().call();
            }
        }).start();
    }
}
class Service {
    public void call() {
        System.out.println("Service:" + Thread.currentThread().getName());
        System.out.println("Service:" + CustomThreadLocal.threadLocal.get());
        new Dao().call();
    }
}
class Dao {
    public void call() {
        System.out.println("==========================");
        System.out.println("Dao:" + Thread.currentThread().getName());
        System.out.println("Dao:" + CustomThreadLocal.threadLocal.get());
    }
}
```
我们在主类中定义了一个ThreadLocal用来传递数据，然后起了一个线程，在线程中调用Service中的call方法，并且往Threadlocal中设置了一个值。
在Service中获取ThreadLocal中的值，然后再调用Dao中的call方法，也是获取ThreadLocal中的值，我们运行下看效果：
```
Service:Thread-0
Service:猿天地
==========================
Dao:Thread-0
Dao:猿天地
```
可以看到整个流程都是在同一个线程中执行的，也正确的获取到了ThreadLocal中的值，这种情况是没有问题的。
接下来我们改造下程序，进行线程切换，将调用Dao中的call重启一个线程执行：
```
public class CustomThreadLocal {
    static ThreadLocal<String> threadLocal = new ThreadLocal<>();
    public static void main(String[] args) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                CustomThreadLocal.threadLocal.set("猿天地");
                new Service().call();
            }
        }).start();
    }
}
class Service {
    public void call() {
        System.out.println("Service:" + Thread.currentThread().getName());
        System.out.println("Service:" + CustomThreadLocal.threadLocal.get());
        //new Dao().call();
        new Thread(new Runnable() {
            @Override
            public void run() {
                new Dao().call();
            }
        }).start();
    }
}
class Dao {
    public void call() {
        System.out.println("==========================");
        System.out.println("Dao:" + Thread.currentThread().getName());
        System.out.println("Dao:" + CustomThreadLocal.threadLocal.get());
    }
}
```
再次运行，看效果：
```
Service:Thread-0
Service:猿天地
==========================
Dao:Thread-1
Dao:null
```
可以看到这次的请求是由2个线程共同完成的，在Service中还是可以拿到ThreadLocal的值，到了Dao中就拿不到了，因为线程已经切换了，这就是开始讲的ThreadLocal的数据会丢失的问题。
那么怎么解决这个问题呢，其实也很简单，只需要改一行代码即可:
```
static ThreadLocal<String> threadLocal = new InheritableThreadLocal<>();
```
将ThreadLocal改成InheritableThreadLocal，我们看下改造之后的效果：
```
Service:Thread-0
Service:猿天地
==========================
Dao:Thread-1
Dao:猿天地
```
值可以正常拿到，InheritableThreadLocal就是为了解决这种线程切换导致ThreadLocal拿不到值的问题而产生的。
至于原理大家可以去看jdk的源码，这边就不做过多讲解了，有了InheritableThreadLocal能为我们解决不少的问题。
如果需要能够在Hystrix 为线程隔离模式也能正确传递数据的话，需要我们自己去修改。
我这边以Zuul中自定义负载均衡策略来进行讲解，在Zuul中需要实现灰度发布的功能，需要在Filter中将请求的用户信息传递到自定的负载策略中，Zuul中整合了Hystrix，从Zuul Filter的请求到Ribbon的策略类中，线程已经发生了变化，变成了Hystrix提供的线程池来执行（配置隔离模式为线程）。这个时用ThreadLocal就会出问题了，数据传输会错乱。也就是我们前面分析的问题。
关于修改我说下自己分析问题的一些思路，ransmittable-thread-local可以解决这个问题，可以对线程或者线程池进行修饰，其实最终的原理就是对线程进行包装，在线程run之前和之后做一些处理来保证数据的正确传递。
# 改造思路
首先我想的就是改掉Hystrix中的线程池或者线程，只有这样才能让ransmittable-thread-local来接管线程中数据的传递。
通过调试的方式找到com.netflix.hystrix.HystrixThreadPool是Hystrix线程池的接口，里面定义了一个获取ExecutorService方法，代码如下：
```
public interface HystrixThreadPool {
    /**
     * Implementation of {@link ThreadPoolExecutor}.
     *
     * @return ThreadPoolExecutor
     */
    public ExecutorService getExecutor();
}
```
通过查找接口的实现类，发现只有一个默认的实现com.netflix.hystrix.HystrixThreadPool.HystrixThreadPoolDefault，实现也在接口中，是一个静态类
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/WRLYshvT39zVtJRfXe2ibrqHttNqJcibkggpjnN7DpUiadEibncEic9bgtdTXNQWsJ6TnTqAM9fVR3DtQUAqD1uLohg/640?wx_fmt=png)1.png
实现的方法如下：
```
@Override
 public ThreadPoolExecutor getExecutor() {
     touchConfig();
     return threadPool;
 }
```
threadPool是类中的一个变量，主要是通过touchConfig方法来设置线程的参数，touchConfig代码如下：
```
private void touchConfig() {
      final int dynamicCoreSize = properties.coreSize().get();
      final int configuredMaximumSize = properties.maximumSize().get();
      int dynamicMaximumSize = properties.actualMaximumSize();
      final boolean allowSizesToDiverge = properties.getAllowMaximumSizeToDivergeFromCoreSize().get();
      boolean maxTooLow = false;
      if (allowSizesToDiverge && configuredMaximumSize < dynamicCoreSize) {
          //if user sets maximum < core (or defaults get us there), we need to maintain invariant of core <= maximum
          dynamicMaximumSize = dynamicCoreSize;
          maxTooLow = true;
      }
      //......
}
```
这是最外层获取线程池的地方，可以根据代码一步步进去看，最终获取线程池的代码在com.netflix.hystrix.strategy.concurrency.HystrixConcurrencyStrategy.getThreadPool方法中。
上面是线程池的源码分析，我们可以改造源码，将线程池用ransmittable-thread-local进行修饰。
# 改造线程方式
另外一种是改造线程的方式，在Hystrix将命令丢入线程池的时候对线程进行修饰也可以解决此问题，因为ransmittable-thread-local对线程池进行修饰，其原理也是改造了线程，通过源码可以看出：
```
public static ExecutorService getTtlExecutorService(ExecutorService executorService) {
        if (executorService == null || executorService instanceof ExecutorServiceTtlWrapper) {
            return executorService;
        }
        return new ExecutorServiceTtlWrapper(executorService);
 }
class ExecutorServiceTtlWrapper extends ExecutorTtlWrapper implements ExecutorService {
    private final ExecutorService executorService;
    ExecutorServiceTtlWrapper(ExecutorService executorService) {
        super(executorService);
        this.executorService = executorService;
    }
    @Override
    public <T> Future<T> submit(Callable<T> task) {
        return executorService.submit(TtlCallable.get(task));
    }
    @Override
    public <T> Future<T> submit(Runnable task, T result) {
        return executorService.submit(TtlRunnable.get(task), result);
    }
    @Override
    public Future<?> submit(Runnable task) {
        return executorService.submit(TtlRunnable.get(task));
    }
    // ...........
}
```
> 
重点在TtlRunnable.get()
改造Hystrix中线程的方式，可以通过HystrixContextScheduler进行入手，Hystrix通过HystrixContextScheduler的ThreadPoolScheduler把命令submit到ThreadPoolExecutor中去执行。
通过上面的分析，最终可以定位到提交命令的代码如下：
```
private static class ThreadPoolWorker extends Worker {
        private final HystrixThreadPool threadPool;
        private final CompositeSubscription subscription = new CompositeSubscription();
        private final Func0<Boolean> shouldInterruptThread;
        public ThreadPoolWorker(HystrixThreadPool threadPool, Func0<Boolean> shouldInterruptThread) {
            this.threadPool = threadPool;
            this.shouldInterruptThread = shouldInterruptThread;
        }
        @Override
        public void unsubscribe() {
            subscription.unsubscribe();
        }
        @Override
        public boolean isUnsubscribed() {
            return subscription.isUnsubscribed();
        }
        @Override
        public Subscription schedule(final Action0 action) {
            if (subscription.isUnsubscribed()) {
                // don't schedule, we are unsubscribed
                return Subscriptions.unsubscribed();
            }
            // This is internal RxJava API but it is too useful.
            ScheduledAction sa = new ScheduledAction(action);
            subscription.add(sa);
            sa.addParent(subscription);
            ThreadPoolExecutor executor = (ThreadPoolExecutor) threadPool.getExecutor();
            FutureTask<?> f = (FutureTask<?>) executor.submit(sa);
            sa.add(new FutureCompleterWithConfigurableInterrupt(f, shouldInterruptThread, executor));
            return sa;
        }
        @Override
        public Subscription schedule(Action0 action, long delayTime, TimeUnit unit) {
            throw new IllegalStateException("Hystrix does not support delayed scheduling");
        }
}
```
核心代码在schedule方法中，只需要将schedule中的sa进行修饰即可。
改造后的代码如下：
```
public Subscription schedule(final Action0 action) {
     if (subscription.isUnsubscribed()) {
            // don't schedule, we are unsubscribed
            return Subscriptions.unsubscribed();
     }
     // This is internal RxJava API but it is too useful.
     ScheduledAction sa = new ScheduledAction(action);
     subscription.add(sa);
     sa.addParent(subscription);
     ThreadPoolExecutor executor = (ThreadPoolExecutor) threadPool.getExecutor();
     FutureTask<?> f = (FutureTask<?>) executor.submit(TtlRunnable.get(sa));
     sa.add(new FutureCompleterWithConfigurableInterrupt(f, shouldInterruptThread, executor));
     return sa;
}
```
改源码还涉及到重新打包等问题，每个项目都得用修改后的jar包，比较麻烦，最简单的做法就是在项目中建一个同样的HystrixContextScheduler类，包名也要和之前一样，让jvm优先加载，这样就能用这个修改的类来代替Hystrix原始的类。
最后我们来验证下这样的改动是否正确，首先我们在Zuul的Filter中进行值的传递：
RibbonFilterContextHolder是基于InheritableThreadLocal做的值传递,代码如下：
```
public class RibbonFilterContextHolder {
    private static final ThreadLocal<RibbonFilterContext> contextHolder = new InheritableThreadLocal<RibbonFilterContext>() {
        @Override
        protected RibbonFilterContext initialValue() {
            return new DefaultRibbonFilterContext();
        }
    };
    public static RibbonFilterContext getCurrentContext() {
        return contextHolder.get();
    }
    public static void clearCurrentContext() {
        contextHolder.remove();
    }
}
```
完整源码请参考:
https://github.com/yinjihuan/spring-cloud/blob/master/fangjia-common/src/main/java/com/fangjia/common/support/RibbonFilterContextHolder.java
```
private static AtomicInteger ac = new AtomicInteger();
    @Override
    public Object run() {
        RequestContext ctx = RequestContext.getCurrentContext();
        RibbonFilterContextHolder.getCurrentContext().add("servers",ac.addAndGet(1)+"");
        return null;
    }
```
通过AtomicInteger 进行数字的累加操作，后面测试的时候用10个线程并发测试，如如果在Ribbon的自定义负载策略中接收的值是0-9的话表示正确，否则错误。
接下来定义一个负载策略类，输出接收的值：
```
public class GrayPushRule extends AbstractLoadBalancerRule {
    private AtomicInteger nextServerCyclicCounter;
    private static final boolean AVAILABLE_ONLY_SERVERS = true;
    private static final boolean ALL_SERVERS = false;
    private static Logger log = LoggerFactory.getLogger(RoundRobinRule.class);
    public GrayPushRule() {
        this.nextServerCyclicCounter = new AtomicInteger(0);
    }
    public GrayPushRule(ILoadBalancer lb) {
        this();
        this.setLoadBalancer(lb);
    }
    public Server choose(ILoadBalancer lb, Object key) {
        String servers = RibbonFilterContextHolder.getCurrentContext().get("servers");
        System.out.println(Thread.currentThread().getName()+":"+servers);  
        return null;
    }
    public Server choose(Object key) {
        return this.choose(this.getLoadBalancer(), key);
    }
    public void initWithNiwsConfig(IClientConfig clientConfig) {
    }
}
```
然后增加配置，使用自定义的策略，还需要将Hystrix的线程池数量改小一点，这样才可以线程复用：
```
fsh-house.ribbon.NFLoadBalancerRuleClassName=com.fangjia.fsh.api.rule.GrayPushRule
# 线程隔离模式
zuul.ribbon-isolation-strategy=thread
hystrix.threadpool.default.coreSize=3
```
启动服务，用ab进行测试：
```
ab -n 10 -c 10 http://192.168.10.170:2103/fsh-house/house/1
```
输出结果如下:
```
hystrix-RibbonCommand-3:10
hystrix-RibbonCommand-2:3
hystrix-RibbonCommand-1:8
hystrix-RibbonCommand-3:10
hystrix-RibbonCommand-2:3
hystrix-RibbonCommand-1:8
hystrix-RibbonCommand-3:10
hystrix-RibbonCommand-2:3
hystrix-RibbonCommand-1:8
hystrix-RibbonCommand-3:10
```
很多数据都重复了，这就是线程复用导致的问题，接下来我们用上面讲的方式进行改造。
需要将RibbonFilterContextHolder中的InheritableThreadLocal改成TransmittableThreadLocal。
```
private static final TransmittableThreadLocal<RibbonFilterContext> contextHolder = new TransmittableThreadLocal<RibbonFilterContext>() {
        @Override
        protected RibbonFilterContext initialValue() {
            return new DefaultRibbonFilterContext();
        }
    };
```
然后在项目中新建一个HystrixContextScheduler类，包名必须是com.netflix.hystrix.strategy.concurrency，代码就按上面贴的进行改，主要是对线程进行修饰：
```
FutureTask<?> f = (FutureTask<?>) executor.submit(TtlRunnable.get(sa));
```
再次启动服务，进行测试，结果如下：
```
hystrix-RibbonCommand-2:10
hystrix-RibbonCommand-1:1
hystrix-RibbonCommand-3:7
hystrix-RibbonCommand-3:8
hystrix-RibbonCommand-1:2
hystrix-RibbonCommand-2:4
hystrix-RibbonCommand-3:5
hystrix-RibbonCommand-1:9
hystrix-RibbonCommand-2:3
hystrix-RibbonCommand-3:6
```
现在的结果已经是正确的
# 改造线程池方式
上面介绍了改造线程的方式，并且通过建一个同样的Java类来覆盖Jar包中的实现，感觉有点投机取巧，其实不用这么麻烦，Hystrix默认提供了HystrixPlugins类，可以让用户自定义线程池，下面来看看怎么使用：
在启动之前调用进行注册自定义实现的逻辑：
```
HystrixPlugins.getInstance().registerConcurrencyStrategy(new ThreadLocalHystrixConcurrencyStrategy());
```
ThreadLocalHystrixConcurrencyStrategy就是我们自定义的创建线程池的类，需要继承HystrixConcurrencyStrategy，前面也有讲到通过调试代码发现最终获取线程池的代码就在HystrixConcurrencyStrategy中。
我们只需要重写getThreadPool方法即可完成对线程池的改造，由于TtlExecutors只能修饰ExecutorService和Executor，而HystrixConcurrencyStrategy中返回的是ThreadPoolExecutor，我们需要对ThreadPoolExecutor进行包装一层，最终在execute方法中对线程修饰，也就相当于改造了线程池。
```
public class ThreadLocalHystrixConcurrencyStrategy extends HystrixConcurrencyStrategy {
    private final static Logger logger = LoggerFactory.getLogger(ThreadLocalHystrixConcurrencyStrategy.class);
    @Override
    public ThreadPoolExecutor getThreadPool(HystrixThreadPoolKey threadPoolKey, HystrixProperty<Integer> corePoolSize,
            HystrixProperty<Integer> maximumPoolSize, HystrixProperty<Integer> keepAliveTime, TimeUnit unit,
            BlockingQueue<Runnable> workQueue) {
        return this.doGetThreadPool(threadPoolKey, corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue);
    }
    @Override
    public ThreadPoolExecutor getThreadPool(HystrixThreadPoolKey threadPoolKey,
            HystrixThreadPoolProperties threadPoolProperties) {
        return this.doGetThreadPool(threadPoolKey, threadPoolProperties);
    }
}
```
在doGetThreadPool方法中就返回包装的线程池，代码如下：
```
return new ThreadLocalThreadPoolExecutor(dynamicCoreSize, dynamicMaximumSize, keepAliveTime.get(), unit, workQueue,
                    threadFactory);
```
最后就是ThreadLocalThreadPoolExecutor的代码：
```
public class ThreadLocalThreadPoolExecutor extends ThreadPoolExecutor {
    private static final RejectedExecutionHandler defaultHandler = new AbortPolicy();
    public ThreadLocalThreadPoolExecutor(int corePoolSize, int maximumPoolSize, long keepAliveTime, TimeUnit unit,
            BlockingQueue<Runnable> workQueue) {
        super(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue);
    }
    public ThreadLocalThreadPoolExecutor(int corePoolSize, int maximumPoolSize, long keepAliveTime, TimeUnit unit,
            BlockingQueue<Runnable> workQueue, ThreadFactory threadFactory) {
        super(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue, threadFactory, defaultHandler);
    }
    @Override
    public void execute(Runnable command) {
        super.execute(TtlRunnable.get(command));
    }
}
```
以上内容摘自《Spring Cloud微服务-全栈技术与案例解析》书中。
## Spring Cloud微服务-全栈技术与案例解析介绍
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/WRLYshvT39zVtJRfXe2ibrqHttNqJcibkgWx7h3DHrC59ouW5svBOhDQOy6xNia0cSsIOuT6lMQcJfQlkbwCEoA5w/640?wx_fmt=png)《Spring Cloud微服务-全栈技术与案例解析》
在互联网时代，互联网产品的最大特点就是需要快速发布新功能，支持高并发和大数据。传统的架构已经慢慢不能支撑互联网业务的发展，这时候微服务架构顺势而出。
最开始国内很多公司都是基于阿里开源的Dubbo框架来构建微服务的，由于阿里内部的原因，Dubbo已经几年没进行维护了，不过今年（2018年）又宣布重新开始维护了。反观Spring Cloud，其在国外发展得很好，但在国内，由于Dubbo导致其鲜为人知。不过从2017年开始，Spring Cloud在国内的普及度越来越高了，很多中小型互联网公司都开始拥抱Spring Cloud。
Spring Cloud拥有一整套微服务的解决方案，基于Spring Boot可实现快速集成，且开发效率很高，故其堪称中小型互联网公司的福音。而且Spring Cloud发布新功能的频率非常高，目前仅是大版本就有很多个，同时还有庞大的社区支持，照这样的发展势头，我相信未来几年国内一定是Spring Cloud的天下。
我一直在使用Spring Boot、Spring Data等一系列框架来进行开发，作为一名Spring的忠实粉丝，自然希望能够有更多开发者参与进来，于是自己坚持写Spring Cloud相关的文章，并且将文章涉及的代码整理好了放在GitHub上面进行分享。在这个过程中我得到了很多开发者朋友的关注，他们向我咨询一些微服务方面的问题，我也会自己研究和解决一些问题，然后通过文章的形式分享给各位开发者朋友。我写本书的一个目的是想进一步推广Spring Cloud在国内的使用和发展，分享自己在微服务领域的一些小小的经验。
本书内容共18章，共分为4个部分。
- 
准备篇
第一部分为准备篇，准备篇当中会对微服务以及Spring Cloud做一个整体的介绍，同时将要学习Spring Cloud的基础的内容进行讲解，比如Spring Boot。
- 
基础篇
第二部分为基础篇，基础篇中会对Spring Cloud的几大常用模块进行讲解和简单的使用。
- 
实战篇
第三部分为实战篇，实战篇当中会通过对在微服务架构中普遍遇到的问题进行实战解决讲解。
- 
高级篇
第四部分为高级篇，高级篇当中就是对Spring Cloud进行一些扩展性的使用，比如API网关怎么限流，怎么做灰度发布, 分布式任务，分布式事务，分库分表等。
微服务的诞生并非偶然，它是在互联网高速发展，技术日新月异的变化以及传统架构无法适应快速变化等多重因素的推动下诞生的产物，在 Java 领域中 Spring Cloud 是微服务架构的最佳落地，无论是学习成本还是功能的完备性都无可挑剔。此次出版的《Spring Cloud微服务-全栈技术与案例解析》一书，覆盖了 Spring Cloud 的大部分模块，帮助读者快速学会使用 Spring Cloud 的同时，也掌握了微服务典型场景的解决方案。
通过阅读本书，你将：
- 
使用Spring Boot开发Web项目
- 
学会使用Spring Cloud开发分布式系统
- 
Spring Data Redis,Mongodb,ElasticSearch框架的使用
- 
配置中心结合Zuul进行动态限流降级
- 
分布式事务的解决思路
- 
分布式任务调度框架Elastic Job的使用
- 
分库分表中间件Sharding JDBC的使用
购书链接：https://item.m.jd.com/product/12404630.html
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/PgqYrEEtEnqFAqnMIViaZEfiaicXVFZSIJaPkYGV61lX04enDu4G8zGwQ2VibF2PMl334EXmLl6ZEsJBJSGibtvhmeg/640?wx_fmt=gif)
为感谢读者对本公号一直以来的支持和鼓励，特赠送上述介绍的《Spring Cloud微服务-全栈技术与案例解析介绍》图书12本，本公号关注者和小密圈用户（知识星球）各有6个名额（名额不能重复拥有），获赠资格如下：
本公号关注者：曾经在本公号赞赏过且在该文下面留言谈谈你对读书的看法或生活的感悟，留言点赞前6名既可获取！（留言是否符合要求已是否出现在本文精选留言区为准）
小密圈（知识星球）用户：在知识星球博主发布的该活动下留言，留言点赞前6名即可获取！（小密圈用户在小密圈该活动下参与）
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnoxxqiaY3IKH7GyichpNMeJzibcXhNerEIhZ3uxzGyD7icR0UHsG8KN5eZ4q9ISz3WicAMgC6Lwwqicmjgw/640?wx_fmt=png)注：上述两种不能同时参与（即用户在公号留言和小密圈留言都中奖的话，图书依旧只有1个）。
活动截止时间：2018年7月22日12点整。
END
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpcR0YpEk9cSITT9srthjLfnyo5a7BroaQnXA0uMk8tvhZtMVLkQDYSsXP1ibtlmYibkvBLRRe6ibD7g/640?wx_fmt=png)
小密圈用户可以从阅读原文进去，也可从知识星球APP直接进入
