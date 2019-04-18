# 在Spring3中使用注解(@Scheduled)创建计划任务 - z69183787的专栏 - CSDN博客
2014年08月13日 09:50:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2785
Spring3中加强了注解的使用，其中计划任务也得到了增强，现在创建一个计划任务只需要两步就完成了：
- 创建一个Java类，添加一个无参无返回值的方法，在方法上用@Scheduled注解修饰一下；
- 在Spring配置文件中添加三个<task:**** />节点；
最后说明一下，第一步创建的Java类要成为Spring可管理的Bean，可以直接写在XML里，也可以@Component一下
### 示例如下
计划任务类：
Java代码  ![收藏代码](http://zywang.iteye.com/images/icon_star.png)
- /**
-  * com.zywang.spring.task.SpringTaskDemo.java
-  * @author ZYWANG 2011-3-9
-  */
- package com.zywang.spring.task;  
- 
- import org.springframework.scheduling.annotation.Scheduled;  
- import org.springframework.stereotype.Component;  
- 
- /**
-  * Spring3 @Scheduled 演示
-  * @author ZYWANG 2011-3-9
-  */
- @Component
- publicclass SpringTaskDemo {  
- 
- @Scheduled(fixedDelay = 5000)  
- void doSomethingWithDelay(){  
-         System.out.println("I'm doing with delay now!");  
-     }  
- 
- @Scheduled(fixedRate = 5000)  
- void doSomethingWithRate(){  
-         System.out.println("I'm doing with rate now!");  
-     }  
- 
- @Scheduled(cron = "0/5 * * * * *")  
- void doSomethingWith(){  
-         System.out.println("I'm doing with cron now!");  
-     }  
- }  
Spring配置文件：
Xml代码  ![收藏代码](http://zywang.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:task="http://www.springframework.org/schema/task"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd  
-         http://www.springframework.org/schema/task http://www.springframework.org/schema/task/spring-task-3.0.xsd">
- <!-- Enables the Spring Task @Scheduled programming model -->
- <task:executorid="executor"pool-size="5"/>
- <task:schedulerid="scheduler"pool-size="10"/>
- <task:annotation-drivenexecutor="executor"scheduler="scheduler"/>
- </beans>
* 以上内容基于Spring 3.0.5 版本运行，参考文档为spring-framework-reference-3.0.5.pdf*
* 以上内容基于Spring 3.0.5 版本运行，参考文档为spring-framework-reference-3.0.5.pdf*
在spring3 中的task 命名空间。可以部分去取代 quartz，并且支持注解方式。但是如果使用更加复杂的任务调度。还是建议是使用quartz。以下就使用 task 和 quartz来进行任务调度的方法进行距离。
- 使用 注解来 来调度任务
编写一个任务实例。
**[java]**[view
 plain](http://blog.csdn.net/remote_roamer/article/details/6573173#)[copy](http://blog.csdn.net/remote_roamer/article/details/6573173#)
- /***********************************************************************
-   *
-   *   TestJob.java
-   *
-   *   ****所有，
-   *   受到法律的保护，任何公司或个人，未经授权不得擅自拷贝。
-   *   @copyright       Copyright:   2000-2011  
-   *   @creator          徐泽宇 <br/>
-   *   @create-time   2011-6-27   下午11:51:16
-   *   @revision         $Id:     *
-   ***********************************************************************/
- package com.alcor.aerie.quartz;  
- import org.slf4j.Logger;  
- import org.slf4j.LoggerFactory;  
- import org.springframework.scheduling.annotation.Scheduled;  
- import org.springframework.stereotype.Service;  
- @Service
- publicclass TestJob {  
- /**
-      * Logger for this class
-      */
- privatestaticfinal Logger logger = LoggerFactory.getLogger(TestJob.class);  
- @Scheduled(fixedDelay = 1000)   
- publicvoid work()  
-     {  
- if (logger.isDebugEnabled()) {  
-             logger.debug("work() - start"); //$NON-NLS-1$
-         }  
-         logger.info("Spring Quartz的TestJob任务被调用！");   
- if (logger.isDebugEnabled()) {  
-             logger.debug("work() - end"); //$NON-NLS-1$
-         }  
-     }   
- }  
注意其中的@Scheduled 标签
配置spring的ApplicationContext.xml
**[xhtml]**[view
 plain](http://blog.csdn.net/remote_roamer/article/details/6573173#)[copy](http://blog.csdn.net/remote_roamer/article/details/6573173#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:context="http://www.springframework.org/schema/context"
- xmlns:task="http://www.springframework.org/schema/task"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-         http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-         http://www.springframework.org/schema/fex  
-         http://www.springframework.org/schema/fex/spring-fex-1.5.xsd  
-         http://www.springframework.org/schema/task   
-         http://www.springframework.org/schema/task/spring-task-3.0.xsd   
-         http://www.springframework.org/schema/context  
-         http://www.springframework.org/schema/context/spring-context-3.0.xsd">
- 
- <!--  定时器开关  开始-->
- <task:annotation-driven/>
- <!--  定时器开关  结束-->
- 
- </beans>
注意：其中xmlns里面加入 ：  xmlns:task="http://www.springframework.org/schema/task" 。在xsi:schemaLocation中加入   http://www.springframework.org/schema/task       
 http://www.springframework.org/schema/task/spring-task-3.0.xsd 。在配置中加入
<!--  定时器开关  开始-->
    <task:annotation-driven/>
<!--  定时器开关  结束-->
如果开发的是web应用，那么在web.xml里面加入
<listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
</listener>
就可以在context启动的时候，自动扫描和装备这个被调度的任务。
- 不使用注解，而通过配置来调度任务
编写一个任务实例
**[c-sharp]**[view
 plain](http://blog.csdn.net/remote_roamer/article/details/6573173#)[copy](http://blog.csdn.net/remote_roamer/article/details/6573173#)
- /***********************************************************************
-   *
-   *   TestJob.java
-   *
-   *   ****所有，
-   *   受到法律的保护，任何公司或个人，未经授权不得擅自拷贝。
-   *   @copyright       Copyright:   2000-2011  
-   *   @creator          徐泽宇 <br/>
-   *   @create-time   2011-6-27   下午11:51:16
-   *   @revision         $Id:     *
-   ***********************************************************************/
- package com.alcor.aerie.quartz;  
- import org.slf4j.Logger;  
- import org.slf4j.LoggerFactory;  
- import org.springframework.scheduling.annotation.Scheduled;  
- import org.springframework.stereotype.Service;  
- @Service   
- publicclass TestJob {  
- /**
-      * Logger for this class
-      */
- privatestatic final Logger logger = LoggerFactory.getLogger(TestJob.class);  
- publicvoid work()  
-     {  
- if (logger.isDebugEnabled()) {  
-             logger.debug("work() - start"); //$NON-NLS-1$
-         }  
-         logger.info("Spring Quartz的TestJob任务被调用！");   
- if (logger.isDebugEnabled()) {  
-             logger.debug("work() - end"); //$NON-NLS-1$
-         }  
-     }   
- }  
注意：这里work方法上没有@Scheduled注解了
在ApplicationContext.xml中加入调度方法
**[c-sharp]**[view
 plain](http://blog.csdn.net/remote_roamer/article/details/6573173#)[copy](http://blog.csdn.net/remote_roamer/article/details/6573173#)
- <task:scheduled-tasks>  
-     <task:scheduled ref="testJob" method="work" cron="1/3 * 2-23 * * ?"/>  
- </task:scheduled-tasks>  
cron表达式
原文来源于：
[http://biaoming.iteye.com/blog/39532](http://biaoming.iteye.com/blog/39532)
一个cron表达式有至少6个（也可能7个）有空格分隔的时间元素。
#### 按顺序依次为
#### 秒（0~59）
分钟（0~59）
小时（0~23）
天（月）（0~31，但是你需要考虑你月的天数）
月（0~11）
天（星期）（1~7 1=SUN 或 SUN，MON，TUE，WED，THU，FRI，SAT）
7.年份（1970－2099）
其中每个元素可以是一个值(如6),一个连续区间(9-12),一个间隔时间(8-18/4)(/表示每隔4小时),一个列表(1,3,5),通配符。由于"月份中的日期"和"星期中的日期"这两个元素互斥的,必须要对其中一个设置?.
0 0 10,14,16 * * ? 每天上午10点，下午2点，4点
0 0/30 9-17 * * ?   朝九晚五工作时间内每半小时
0 0 12 ? * WED 表示每个星期三中午12点
"0 0 12 * * ?" 每天中午12点触发 
"0 15 10 ? * *" 每天上午10:15触发 
"0 15 10 * * ?" 每天上午10:15触发 
"0 15 10 * * ? *" 每天上午10:15触发 
"0 15 10 * * ? 2005" 2005年的每天上午10:15触发 
"0 * 14 * * ?" 在每天下午2点到下午2:59期间的每1分钟触发 
"0 0/5 14 * * ?" 在每天下午2点到下午2:55期间的每5分钟触发 
"0 0/5 14,18 * * ?" 在每天下午2点到2:55期间和下午6点到6:55期间的每5分钟触发 
"0 0-5 14 * * ?" 在每天下午2点到下午2:05期间的每1分钟触发 
"0 10,44 14 ? 3 WED" 每年三月的星期三的下午2:10和2:44触发 
"0 15 10 ? * MON-FRI" 周一至周五的上午10:15触发 
"0 15 10 15 * ?" 每月15日上午10:15触发 
"0 15 10 L * ?" 每月最后一日的上午10:15触发 
"0 15 10 ? * 6L" 每月的最后一个星期五上午10:15触发 
"0 15 10 ? * 6L 2002-2005" 2002年至2005年的每月的最后一个星期五上午10:15触发 
"0 15 10 ? * 6#3" 每月的第三个星期五上午10:15触发
有些子表达式能包含一些范围或列表
例如：子表达式（**天（星期）** ）可以为 “MON-FRI”，“MON，WED，FRI”，“MON-WED,SAT”
“*”字符代表所有可能的值
因此，“*”在子表达式（**月** ）里表示每个月的含义，“*”在子表达式（**天（星期）** ）表示星期的每一天
“/”字符用来指定数值的增量
例如：在子表达式（分钟）里的“0/15”表示从第0分钟开始，每15分钟
         在子表达式（分钟）里的“3/20”表示从第3分钟开始，每20分钟（它和“3，23，43”）的含义一样
“？”字符仅被用于天（月）和天（星期）两个子表达式，表示不指定值
当2个子表达式其中之一被指定了值以后，为了避免冲突，需要将另一个子表达式的值设为“？”
“L” 字符仅被用于天（月）和天（星期）两个子表达式，它是单词“last”的缩写
但是它在两个子表达式里的含义是不同的。
在天（月）子表达式中，“L”表示一个月的最后一天
在天（星期）自表达式中，“L”表示一个星期的最后一天，也就是SAT
如果在“L”前有具体的内容，它就具有其他的含义了
例如：“6L”表示这个月的倒数第６天，“ＦＲＩＬ”表示这个月的最一个星期五
注意：在使用“L”参数时，不要指定列表或范围，因为这会导致问题
|字段||允许值||允许的特殊字符|
|----|----|----|----|----|
|`秒`||`0-59`||`, - * /`|
|`分`||`0-59`||`, - * /`|
|`小时`||`0-23`||`, - * /`|
|`日期`||`1-31`||`, - * ? / L W C`|
|`月份`||`1-12 或者 JAN-DEC`||`, - * /`|
|`星期`||`1-7 或者 SUN-SAT`||`, - * ? / L C #`|
|`年（可选）`||`留空, 1970-2099`||`, - * /`|
