# spring线程池ThreadPoolExecutor配置并且得到任务执行的结果 - z69183787的专栏 - CSDN博客
2016年05月11日 12:01:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3573
用ThreadPoolExecutor的时候，又想知道被执行的任务的执行情况，这时就可以用FutureTask。
原创不易，转载请注明出处：[spring线程池ThreadPoolExecutor配置并且得到任务执行的结果](http://www.zuidaima.com/share/1724478138158080.htm)
代码下载地址：[http://www.zuidaima.com/share/1724478138158080.htm](http://www.zuidaima.com/share/1724478138158080.htm)
ThreadPoolTask
|`01`|`package``com.zuidaima.threadpool;`|
|`02`||
|`03`|`import``java.io.Serializable;`|
|`04`|`import``java.util.concurrent.Callable;`|
|`05`||
|`06`|`public``class``ThreadPoolTask ``implements````Callable<String>, Serializable {```|
|`07`||
|`08`|`    ``private``static``final``long````serialVersionUID =````0``;`|
|`09`||
|`10`|`    ````// 保存任务所需要的数据```|
|`11`|`    ``private````Object threadPoolTaskData;```|
|`12`||
|`13`|`    ``private``static``int````consumeTaskSleepTime =````2000``;`|
|`14`||
|`15`|`    ``public````ThreadPoolTask(Object tasks) {```|
|`16`|`        ``this````.threadPoolTaskData = tasks;```|
|`17`|`    ``}`|
|`18`||
|`19`|`    ``public``synchronized````String call()````throws````Exception {```|
|`20`|`        ````// 处理一个任务，这里的处理方式太简单了，仅仅是一个打印语句```|
|`21`|`        ``System.out.println(``"开始执行任务："````+ threadPoolTaskData);```|
|`22`|`        ````String result =````""``;`|
|`23`|`        ````// //便于观察，等待一段时间```|
|`24`|`        ``try``{`|
|`25`|`            ````// long r = 5/0;```|
|`26`|`            ``for``(``int````i =````0````; i <````100000000````; i++) {```|
|`27`||
|`28`|`            ``}`|
|`29`|`            ````result =````"OK"``;`|
|`30`|`        ``} ``catch````(Exception e) {```|
|`31`|`            ``e.printStackTrace();`|
|`32`|`            ````result =````"ERROR"``;`|
|`33`|`        ``}`|
|`34`|`        ````threadPoolTaskData =````null``;`|
|`35`|`        ``return``result;`|
|`36`|`    ``}`|
|`37`|`}`|
模拟客户端提交的线程
|`01`|`package``com.zuidaima.threadpool;`|
|`02`||
|`03`|`import``java.util.concurrent.ExecutionException;`|
|`04`|`import``java.util.concurrent.FutureTask;`|
|`05`|`import``java.util.concurrent.TimeUnit;`|
|`06`||
|`07`|`import``org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;`|
|`08`||
|`09`|`public``class``StartTaskThread ``implements````Runnable {```|
|`10`||
|`11`|`    ``private````ThreadPoolTaskExecutor threadPoolTaskExecutor;```|
|`12`|`    ``private``int``i;`|
|`13`||
|`14`|`    ``public````StartTaskThread(ThreadPoolTaskExecutor threadPoolTaskExecutor,````int````i) {```|
|`15`|`        ``this````.threadPoolTaskExecutor = threadPoolTaskExecutor;```|
|`16`|`        ``this````.i = i;```|
|`17`|`    ``}`|
|`18`||
|`19`|`    ``@Override`|
|`20`|`    ``public``synchronized``void````run() {```|
|`21`|`        ````String task =``````"task@ "``````+ i;```|
|`22`|`        ``System.out.println(``"创建任务并提交到线程池中："````+ task);```|
|`23`|`        ````FutureTask<String> futureTask =````new``FutureTask<String>(`|
|`24`|`                ``new``ThreadPoolTask(task));`|
|`25`|`        ``threadPoolTaskExecutor.execute(futureTask);`|
|`26`|`        ````// 在这里可以做别的任何事情```|
|`27`|`        ````String result =````null``;`|
|`28`|`        ``try``{`|
|`29`|`            ````// 取得结果，同时设置超时执行时间为1秒。同样可以用future.get()，不设置执行超时时间取得结果```|
|`30`|`            ````result = futureTask.get(````1000````, TimeUnit.MILLISECONDS);```|
|`31`|`        ``} ``catch````(InterruptedException e) {```|
|`32`|`            ``futureTask.cancel(``true``);`|
|`33`|`        ``} ``catch````(ExecutionException e) {```|
|`34`|`            ``futureTask.cancel(``true``);`|
|`35`|`        ``} ``catch````(Exception e) {```|
|`36`|`            ``futureTask.cancel(``true``);`|
|`37`|`            ````// 超时后，进行相应处理```|
|`38`|`        ``} ``finally``{`|
|`39`|`            ``System.out.println(``"task@"````+ i +````":result="````+ result);```|
|`40`|`        ``}`|
|`41`||
|`42`|`    ``}`|
|`43`|`}`|
SPRING配置文件
|`01`|```<?xml version=````"1.0"``encoding=``"UTF-8"``?>`|
|`02`|```<beans xmlns=````"[http://www.springframework.org/schema/beans](http://www.springframework.org/schema/beans)"`|
|`03`|`    ``xmlns:xsi=``"[http://www.w3.org/2001/XMLSchema-instance](http://www.w3.org/2001/XMLSchema-instance)"``xmlns:mvc=``"[http://www.springframework.org/schema/mvc](http://www.springframework.org/schema/mvc)"`|
|`04`|`    ``xmlns:aop=``"[http://www.springframework.org/schema/aop](http://www.springframework.org/schema/aop)"``xmlns:context=``"[http://www.springframework.org/schema/context](http://www.springframework.org/schema/context)"`|
|`05`|`    ``xsi:schemaLocation="`|
|`06`|`        ``http:``//www.springframework.org/schema/beans [http://www.springframework.org/schema/beans/spring-beans-3.0.xsd](http://www.springframework.org/schema/beans/spring-beans-3.0.xsd)`|
|`07`|`        ``http:``//www.springframework.org/schema/context [http://www.springframework.org/schema/context/spring-context-3.0.xsd](http://www.springframework.org/schema/context/spring-context-3.0.xsd)`|
|`08`|`        ``http:``//www.springframework.org/schema/mvc [http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd](http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd)`|
|`09`|`        ``http:``//www.springframework.org/schema/aop [http://www.springframework.org/schema/aop/spring-aop-3.0.xsd](http://www.springframework.org/schema/aop/spring-aop-3.0.xsd)`|
|`10`|`        ``">`|
|`11`|`    ````<bean id=````"threadPoolTaskExecutor"`|
|`12`|`        ``class``=``"org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor"``>`|
|`13`||
|`14`|`        ````<!-- 核心线程数，默认为````1``-->`|
|`15`|`        ````<property name=````"corePoolSize"``value=``"10"``/>`|
|`16`||
|`17`|`        ````<!-- 最大线程数，默认为Integer.MAX_VALUE -->```|
|`18`|`        ````<property name=````"maxPoolSize"``value=``"50"``/>`|
|`19`||
|`20`|`        ````<!-- 队列最大长度，一般需要设置值>=notifyScheduledMainExecutor.maxNum；默认为Integer.MAX_VALUE```|
|`21`|`            ````<property name=````"queueCapacity"``value=``"1000"````/> -->```|
|`22`||
|`23`|`        ````<!-- 线程池维护线程所允许的空闲时间，默认为60s -->```|
|`24`|`        ````<property name=````"keepAliveSeconds"``value=``"300"``/>`|
|`25`||
|`26`|`        ````<!-- 线程池对拒绝任务（无线程可用）的处理策略，目前只支持AbortPolicy、CallerRunsPolicy；默认为后者 -->```|
|`27`|`        ````<property name=````"rejectedExecutionHandler"``>`|
|`28`|`            ````<!-- AbortPolicy:直接抛出java.util.concurrent.RejectedExecutionException异常 -->```|
|`29`|`            ````<!-- CallerRunsPolicy:主线程直接执行该任务，执行完之后尝试添加下一个任务到线程池中，可以有效降低向线程池内添加任务的速度 -->```|
|`30`|`            ````<!-- DiscardOldestPolicy:抛弃旧的任务、暂不支持；会导致被丢弃的任务无法再次被执行 -->```|
|`31`|`            ````<!-- DiscardPolicy:抛弃当前任务、暂不支持；会导致被丢弃的任务无法再次被执行 -->```|
|`32`|`            ``<bean ``class``=``"java.util.concurrent.ThreadPoolExecutor$CallerRunsPolicy"``/>`|
|`33`|`        ``</property>`|
|`34`|`    ``</bean>`|
|`35`|`</beans>`|
测试类
|`01`|`package``com.zuidaima.test;`|
|`02`||
|`03`|`import``org.junit.Test;`|
|`04`|`import``org.junit.runner.RunWith;`|
|`05`|`import``org.springframework.beans.factory.annotation.Autowired;`|
|`06`|`import``org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;`|
|`07`|`import``org.springframework.test.context.ContextConfiguration;`|
|`08`|`import``org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;`|
|`09`|`import``org.springframework.test.context.junit4.SpringJUnit4ClassRunner;`|
|`10`||
|`11`|`import``com.zuidaima.threadpool.StartTaskThread;`|
|`12`||
|`13`|`@RunWith``(SpringJUnit4ClassRunner.``class``)`|
|`14`|```// 指定的运行runner，并且把你所指定的Runner作为参数传递给它```|
|`15`|`@ContextConfiguration````(locations =````"classpath*:applicationContext.xml"``)`|
|`16`|`public``class``TestThreadPool ``extends````AbstractJUnit4SpringContextTests {```|
|`17`||
|`18`|`    ``private``static``int````produceTaskSleepTime =````10``;`|
|`19`||
|`20`|`    ``private``static``int````produceTaskMaxNumber =````1000``;`|
|`21`||
|`22`|`    ``@Autowired`|
|`23`|`    ``private````ThreadPoolTaskExecutor threadPoolTaskExecutor;```|
|`24`||
|`25`|`    ``public````ThreadPoolTaskExecutor getThreadPoolTaskExecutor() {```|
|`26`|`        ``return``threadPoolTaskExecutor;`|
|`27`|`    ``}`|
|`28`||
|`29`|`    ``public``void``setThreadPoolTaskExecutor(`|
|`30`|`            ````ThreadPoolTaskExecutor threadPoolTaskExecutor) {```|
|`31`|`        ``this````.threadPoolTaskExecutor = threadPoolTaskExecutor;```|
|`32`|`    ``}`|
|`33`||
|`34`|`    ``@Test`|
|`35`|`    ``public``void````testThreadPoolExecutor() {```|
|`36`|`        ``for``(``int````i =````1````; i <= produceTaskMaxNumber; i++) {```|
|`37`|`            ``try``{`|
|`38`|`                ``Thread.sleep(produceTaskSleepTime);`|
|`39`|`            ``} ``catch````(InterruptedException e1) {```|
|`40`|`                ``e1.printStackTrace();`|
|`41`|`            ``}`|
|`42`|`            ``new``Thread(``new````StartTaskThread(threadPoolTaskExecutor, i)).start();```|
|`43`|`        ``}`|
|`44`||
|`45`|`    ``}`|
|`46`||
|`47`|`}`|
原文中有些纰漏，我已经修改
项目截图(基于maven构建)
![](http://www.zuidaima.com/scale_images.htm?url=http://www.zuidaima.com/images/1/201403/20140311232610102.jpg)
运行截图：
![](http://www.zuidaima.com/scale_images.htm?url=http://www.zuidaima.com/images/1/201403/20140311232545862.jpg)
如果遇到cpu忙执行超过1秒的会返回null
![](http://www.zuidaima.com/scale_images.htm?url=http://www.zuidaima.com/images/1/201403/20140311232723304.jpg)
