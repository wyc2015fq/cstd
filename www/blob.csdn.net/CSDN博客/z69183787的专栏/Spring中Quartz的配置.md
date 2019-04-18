# Spring中Quartz的配置 - z69183787的专栏 - CSDN博客
2013年12月08日 18:33:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：908
Quartz是一个强大的企业级任务调度框架，Spring中继承并简化了Quartz，下面就看看在Spring中怎样配置Quartz：
参考文章【[http://www.cnblogs.com/kay/archive/2007/11/02/947372.html](http://www.cnblogs.com/kay/archive/2007/11/02/947372.html)】
            【[http://www.diybl.com/course/3_program/java/javajs/2008530/118164.html](http://www.diybl.com/course/3_program/java/javajs/2008530/118164.html)】
各种企业应用几乎都会碰到任务调度的需求，就拿论坛来说：每隔半个小时生成精华文章的RSS文件，每天凌晨统计论坛用户的积分排名，每隔30分钟执行锁定用户解锁任务。对于一个典型的MIS系统来说，在每月1号凌晨统计上个月各部门的业务数据生成月报表，每半个小时查询用户是否已经有快到期的待处理业务……，这样的例子俯拾皆是，不胜枚举。
    Quartz 在开源任务调度框架中的翘首，它提供了强大任务调度机制，难能可贵的是它同时保持了使用的简单性。Quartz 允许开发人员灵活地定义触发器的调度时间表，并可以对触发器和任务进行关联映射。
Spring为创建Quartz的Scheduler、Trigger和JobDetail提供了便利的FactoryBean类，以便能够在Spring 容器中享受注入的好处。此外Spring还提供了一些便利工具类直接将Spring中的Bean包装成合法的任务。Spring进一步降低了使用Quartz的难度，能以更具Spring风格的方式使用Quartz。概括来说它提供了两方面的支持： 
    1)为Quartz的重要组件类提供更具Bean风格的扩展类； 
    2)提供创建Scheduler的BeanFactory类，方便在Spring环境下创建对应的组件对象，并结合Spring容器生命周期进行启动和停止的动作。 
配置文件：
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN""http://www.springframework.org/dtd/spring-beans.dtd">
<beans>
<!-- 要调用的工作类 -->
<bean id="quartzJob"class="test.Task"></bean>
<!-- 定义调用对象和调用对象的方法 -->
<bean id="jobtask"
class="org.springframework.scheduling.quartz.MethodInvokingJobDetailFactoryBean">
<!-- 调用的类 -->
<property name="targetObject">
<ref bean="quartzJob"/>
</property>
<!-- 调用类中的方法 -->
<property name="targetMethod">
<value>work</value>
</property>
</bean>
<!-- 定义触发时间 -->
<bean id="doTime"
class="org.springframework.scheduling.quartz.CronTriggerBean">
<property name="jobDetail">
<ref bean="jobtask"/>
</property>
<!-- cron表达式 -->
<property name="cronExpression">
<value>10,15,20,25,30,35,40,45,50,55****?</value>
</property>
</bean>
<!-- 总管理类 如果将lazy-init='false'那么容器启动就会执行调度程序  -->
<bean id="startQuertz" lazy-init="false" autowire="no"
class="org.springframework.scheduling.quartz.SchedulerFactoryBean">
<property name="triggers">
<list>
<ref bean="doTime"/>
</list>
</property>
</bean>
</beans>
要调度的工作类：
publicclass Task{
publicvoid work() {
        System.out.println("Quartz的任务调度！");
    }
}
主测试文件：
publicclass QuartzTest {
publicstaticvoid main(String[] args) {
        System.out.println("Test start.");
        ApplicationContext context =new ClassPathXmlApplicationContext("applicationContext.xml");
//如果配置文件中将startQuertz bean的lazy-init设置为false 则不用实例化
        System.out.print("Test end..\n");
    }
}
注意：项目中需要加载需要加入spring.jar   quartz-all-1.6.0.jar   log4j-1.2.14.jar   commons-collections.jar   jta.jar   commons-logging.jar这几个包
关于cron表达式（来自网络）：
Cron 表达式包括以下 7 个字段：
- 秒
- 分
- 小时
- 月内日期
- 月
- 周内日期
- 年（可选字段）
Cron 触发器利用一系列特殊字符，如下所示：
- 反斜线（/）字符表示增量值。例如，在秒字段中“5/15”代表从第 5 秒开始，每 15 秒一次。
- 问号（?）字符和字母 L 字符只有在月内日期和周内日期字段中可用。问号表示这个字段不包含具体值。所以，如果指定月内日期，可以在周内日期字段中插入“?”，表示周内日期值无关紧要。字母 L 字符是 last 的缩写。放在月内日期字段中，表示安排在当月最后一天执行。在周内日期字段中，如果“L”单独存在，就等于“7”，否则代表当月内周内日期的最后一个实例。所以“0L”表示安排在当月的最后一个星期日执行。
- 在月内日期字段中的字母（W）字符把执行安排在最靠近指定值的工作日。把“1W”放在月内日期字段中，表示把执行安排在当月的第一个工作日内。
- 井号（#）字符为给定月份指定具体的工作日实例。把“MON#2”放在周内日期字段中，表示把任务安排在当月的第二个星期一。
- 星号（*）字符是通配字符，表示该字段可以接受任何可能的值。
字段 允许值 允许的特殊字符 
秒 0-59 , - * / 
分 0-59 , - * / 
小时 0-23 , - * / 
日期 1-31 , - * ? / L W C 
月份 1-12 或者 JAN-DEC , - * / 
星期 1-7 或者 SUN-SAT , - * ? / L C # 
年（可选） 留空, 1970-2099 , - * /
表达式意义 
"0 0 12 * * ?" 每天中午12点触发 
"0 15 10 ? * *" 每天上午10:15触发 
"0 15 10 * * ?" 每天上午10:15触发 
"0 15 10 * * ? *" 每天上午10:15触发 
"0 15 10 * * ? 2005" 2005年的每天上午10:15触发 
"0 * 14 * * ?" 在每天下午2点到下午2:59期间的每1分钟触发 
"0 0/5 14 * * ?" 在每天下午2点到下午2:55期间的每5分钟触发 
"0 0/5 14,18 * * ?" 在每天下午2点到2:55期间和下午6点到6:55期间的每5分钟触发 
"0 0-5 14 * * ?" 在每天下午2点到下午2:05期间的每1分钟触发 
"0 10,44 14 ? 3 WED" 每年三月的星期三的下午2:10和2:44触发 
"0 15 10 ? * MON-FRI" 周一至周五的上午10:15触发 
"0 15 10 15 * ?" 每月15日上午10:15触发 
"0 15 10 L * ?" 每月最后一日的上午10:15触发 
"0 15 10 ? * 6L" 每月的最后一个星期五上午10:15触发 
"0 15 10 ? * 6L 2002-2005" 2002年至2005年的每月的最后一个星期五上午10:15触发 
"0 15 10 ? * 6#3" 每月的第三个星期五上午10:15触发 
每天早上6点
0 6 * * *
每两个小时
0 */2 * * * 
晚上11点到早上8点之间每两个小时，早上八点
0 23-7/2，8 * * *
每个月的4号和每个礼拜的礼拜一到礼拜三的早上11点
0 11 4 * 1-3 
1月1日早上4点
0 4 1 1 *
更多知识：
http://www.ibm.com/developerworks/cn/java/j-quartz/index.html
http://www.javaeye.com/topic/117244
