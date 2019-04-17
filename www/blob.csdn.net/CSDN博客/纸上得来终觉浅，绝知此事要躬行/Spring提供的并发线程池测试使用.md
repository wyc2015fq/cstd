# Spring提供的并发线程池测试使用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月03日 18:28:42[boonya](https://me.csdn.net/boonya)阅读数：2537








Java对线程池的支持是通过java.util.concurrent包提供的。Spring也实现了自身的线程池管理对象：org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor。


## 配置线程池

在applicationContext.xml配置如下：


<!-- spring thread poolexecutor -->

<beanid=*"threadPoolTaskExecutor"*class=*"org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor"*>

<!-- 线程池维护线程的最少数量 -->

<propertyname=*"corePoolSize"*value=*"5"*/>

<!-- 允许的空闲时间 -->

<propertyname=*"keepAliveSeconds"*value=*"180"*/>

<!-- 线程池维护线程的最大数量 -->

<propertyname=*"maxPoolSize"*value=*"100"*/>

<!-- 缓存队列 -->

<propertyname=*"queueCapacity"*value=*"200"*/>

<!-- 对拒绝task的处理策略 -->

<propertyname=*"rejectedExecutionHandler"*>

<beanclass=*"java.util.concurrent.ThreadPoolExecutor$CallerRunsPolicy"*/>

</property>

</bean>



**属性字段说明：**

corePoolSize：线程池维护线程的最少数量

keepAliveSeconds：允许的空闲时间

maxPoolSize：线程池维护线程的最大数量

queueCapacity：缓存队列

rejectedExecutionHandler：对拒绝task的处理策略

**Reject策略预定义有四种： **
	(1)ThreadPoolExecutor.AbortPolicy策略，是默认的策略,处理程序遭到拒绝将抛出运行时 RejectedExecutionException。
	(2)ThreadPoolExecutor.CallerRunsPolicy策略 ,调用者的线程会执行该任务,如果执行器已关闭,则丢弃.
	(3)ThreadPoolExecutor.DiscardPolicy策略，不能执行的任务将被丢弃.
	(4)ThreadPoolExecutor.DiscardOldestPolicy策略，如果执行程序尚未关闭，则位于工作队列头部的任务将被删除，然后重试执行程序（如果再次失败，则重复此过程）.


## **线程池测试方法**




**package** fmcgwms.thread;



**import** java.io.IOException;

**import** java.util.concurrent.atomic.AtomicInteger;

**import** org.junit.Test;

**import** org.junit.runner.RunWith;

**import** org.springframework.beans.factory.annotation.Autowired;

**import** org.springframework.test.context.ContextConfiguration;

**import** org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

**import** org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;

@RunWith(SpringJUnit4ClassRunner.**class**)

@ContextConfiguration(locations = "classpath:applicationContext.xml")

**publicclass**SpringThreadPoolTaskExecutorTest {



@Autowired

**private** ThreadPoolTaskExecutor threadPoolTaskExecutor;



@Test

**publicvoid** test() **throws** IOException{

ThreadPoolTaskExecutorpoolTaskExecutor=**new**ThreadPoolTaskExecutor();  

//线程池维护线程的最少数量

poolTaskExecutor.setCorePoolSize(5);  

//线程池所使用的缓冲队列

poolTaskExecutor.setQueueCapacity(200);  

//线程池维护线程的最大数量

poolTaskExecutor.setMaxPoolSize(100);  

//线程池维护线程所允许的空闲时间

**poolTaskExecutor.setKeepAliveSeconds(180);  **

poolTaskExecutor.initialize();



// 运行到第五个任务直接关闭了

**for** (**int**i = 0; i < 10; i++) {

poolTaskExecutor.execute(**new** TaskThread(**new** AtomicInteger(i))); 

}

// 执行完毕再关闭

**while**(threadPoolTaskExecutor.getActiveCount()==0){

System.***out***.println("===================shutdown()=");

threadPoolTaskExecutor.shutdown();

**break**;

}





System.***in***.read();

}



@Test

**publicvoid** testThreadPool() **throws** IOException{

// 运行到第五个任务直接关闭了

**for** (**int**i = 0; i < 10; i++) {

threadPoolTaskExecutor.execute(**new**TaskThread(**new**AtomicInteger(i))); 

}

// 执行完毕再关闭

**while**(threadPoolTaskExecutor.getActiveCount()==0){

System.***out***.println("===================shutdown()=");

threadPoolTaskExecutor.shutdown();

**break**;

}



System.***in***.read();

}



}



**class** TaskThread **implements** Runnable{



**private**AtomicInteger index;



**public**TaskThread(AtomicInteger index) {

**super**();

**this**.index = index;

}



@Override

**publicvoid** run() {

System.***out***.println("index="+index+"----start to process---------");

**int**i=0;

**while**(i<10000){

i++;

}

System.***out***.println("index="+index+"----Task processed!!!!!!!!!!!!!!!---------i="+i);



}



}

如果线程池需要的内存需要在4G以上的机器上测试，否则很有可能出现卡顿现象。




## 注意事项

### 无须手动关闭线程池



下面执行线程关闭的方法不会马上执行，执行之后也会影响后续的任务而不能继续执行。



错误使用方式：

threadPoolTaskExecutor.execute(**new**JobTaskThread(**new**AtomicInteger(**new**Random().nextInt(1000)))); 

// 下面的代码不会马上执行，执行后会等待线程池类所有任务执行完成而关闭

**int**enter=0;

**while** (threadPoolTaskExecutor.getActiveCount()==0&&enter==0) {

Log.*getLogger*(getClass()).info("==============shutdown==============="); 

threadPoolTaskExecutor.shutdown();// 关闭

enter++;

}



### 提升测试环境的内存



在本地测试服务器中测试发现CPU明显性能下降，往往不能满足实际的需求，如果开发环境和服务器出现卡顿现象说明内存不能满足运行的资源占用要求，可以适当对机器硬件进行升级。例如本地：打开多个浏览器，业务处理多个耗时操作。



注意：如果是在同一个服务器部署多个应用，出现卡顿的情况很正常，跟硬件有关系（与Tomcat等服务器容器无关，如果硬件受限那么最好还是使用JavaJDK提供的线程池来做）。










