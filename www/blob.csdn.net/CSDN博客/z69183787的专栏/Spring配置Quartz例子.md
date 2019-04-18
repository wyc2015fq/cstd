# Spring配置Quartz例子 - z69183787的专栏 - CSDN博客
2013年12月08日 18:48:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：869
个人分类：[定时任务调度																[Spring-定时任务](https://blog.csdn.net/z69183787/article/category/7070712)](https://blog.csdn.net/z69183787/article/category/2175967)
1.引入除spring.jar外的Quartz的jar包quartz-all-1.8.6.jar
2.任务类 QuartzJob
|`01`|`public``class````QuartzJob {```|
|`02`|`    `|
|`03`|`    ``public``void``work1()`|
|`04`|`    ``{`|
|`05`|`    ``System.out.println(``"Quartz的任务调度！！！work1启用……"``);`|
|`06`|`    ``}`|
|`07`|`    `|
|`08`|`    ``public``void``work2()`|
|`09`|`    ``{`|
|`10`|`    ``System.out.println(``"Quartz的任务调度！！！work2启用……"``);`|
|`11`|`    ``}`|
|`12`||
|`13`||
|`14`|`}`|
2.Spring 配置 quartz-config.xml
[查看源码](http://www.open-open.com/lib/view/open1358482348406.html#viewSource)
[打印](http://www.open-open.com/lib/view/open1358482348406.html#printSource)[?](http://www.open-open.com/lib/view/open1358482348406.html#about)
|`01`|`<?``xml``version``=``"1.0"``encoding``=``"UTF-8"``?>`|
|`02`|`<``beans``xmlns``=``"http://www.springframework.org/schema/beans"`|
|`03`|`    ``xmlns:xsi``=``"http://www.w3.org/2001/XMLSchema-instance"``xmlns:jee``=``"http://www.springframework.org/schema/jee"`|
|`04`|`    ````xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.0.xsd```|
|`05`|```http://www.springframework.org/schema/jee http://www.springframework.org/schema/jee/spring-jee-2.0.xsd">```|
|`06`||
|`07`||
|`08`|`    ````<!-- 线程执行器配置，用于任务注册 -->```|
|`09`|`    ``<``bean``id``=``"executor"`|
|`10`|`        ``class``=``"org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor"``>`|
|`11`|`        ``<``property``name``=``"corePoolSize"``value``=``"10"``/>`|
|`12`|`        ``<``property``name``=``"maxPoolSize"``value``=``"100"``/>`|
|`13`|`        ``<``property``name``=``"queueCapacity"``value``=``"500"``/>`|
|`14`|`    ``</``bean``>`|
|`15`||
|`16`|`    ````<!-- 要调用的工作类 -->```|
|`17`|`    ``<``bean``id``=``"quartzJob"``class``=``"QuartzJob"``></``bean``>`|
|`18`||
|`19`||
|`20`||
|`21`|`    ````<!--调度业务 : 定义调用对象和调用对象的方法 -->```|
|`22`|`    ``<``bean``id``=``"jobtask1"`|
|`23`|`        ``class``=``"org.springframework.scheduling.quartz.MethodInvokingJobDetailFactoryBean"``>`|
|`24`|`        ````<!-- 调用的类 -->```|
|`25`|`        ``<``property``name``=``"targetObject"``ref``=``"quartzJob"``/>`|
|`26`|`        ````<!-- 调用类中的方法 -->```|
|`27`|`        ``<``property``name``=``"targetMethod"``value``=``"work1"``/>`|
|`28`|`    ``</``bean``>`|
|`29`|`    `|
|`30`|`    ``<``bean``id``=``"jobtask2"`|
|`31`|`        ``class``=``"org.springframework.scheduling.quartz.MethodInvokingJobDetailFactoryBean"``>`|
|`32`|`        ````<!-- 调用的类 -->```|
|`33`|`        ``<``property``name``=``"targetObject"``ref``=``"quartzJob"``/>`|
|`34`|`        ````<!-- 调用类中的方法 -->```|
|`35`|`        ``<``property``name``=``"targetMethod"``value``=``"work2"``/>`|
|`36`|`    ``</``bean``>`|
|`37`|`    `|
|`38`|`    ````<!-- 定义触发时间 -->```|
|`39`|`    ``<``bean``id``=``"doTime1"``class``=``"org.springframework.scheduling.quartz.CronTriggerBean"``>`|
|`40`|`        ``<``property``name``=``"jobDetail"``ref``=``"jobtask1"``/>`|
|`41`|`        ````<!-- cron表达式 -->``````<!-- 每隔10秒执行一次 -->```|
|`42`|`        ``<``property``name``=``"cronExpression"``value``=````"0/3 * * * * ?"````/>`|
|`43`|`    ``</``bean``>`|
|`44`|`    `|
|`45`|`    ``<``bean``id``=``"doTime2"``class``=``"org.springframework.scheduling.quartz.CronTriggerBean"``>`|
|`46`|`        ``<``property``name``=``"jobDetail"``ref``=``"jobtask2"``/>`|
|`47`|`        ``<``property``name``=``"cronExpression"``value``=````"0/5 * * * * ?"````/>`|
|`48`|`    ``</``bean``>`|
|`49`|`    `|
|`50`|`    ````<!--设置调度 总管理类 如果将lazy-init='false'那么容器启动就会执行调度程序 -->```|
|`51`|`    ``<``bean``id``=``"startQuertz"``lazy-init``=``"false"``autowire``=``"no"`|
|`52`|`        ``class``=``"org.springframework.scheduling.quartz.SchedulerFactoryBean"``>`|
|`53`|`        ``<``property``name``=``"triggers"``>`|
|`54`|`            ``<``list``>`|
|`55`|`                ``<``ref``bean``=``"doTime1"``/>`|
|`56`|`                ``<``ref``bean``=``"doTime2"``/>`|
|`57`|`            ``</``list``>`|
|`58`|`        ``</``property``>`|
|`59`|`        `|
|`60`|`        ``<``property``name``=``"taskExecutor"``ref``=``"executor"``/>`|
|`61`|`    ``</``bean``>`|
|`62`||
|`63`|`</``beans``>`|
3.加载 <!-- 加载spring -->
|`1`|`<``context-param``> `|
|`2`|`        ``<``param-name``>contextConfigLocation</``param-name``> `|
|`3`|`        ``<``param-value``> `|
|`4`|`            ``/WEB-INF/classes/quartz-config.xml `|
|`5`|`        ``</``param-value``> `|
|`6`|`    ``</``context-param``>`|
