# Spring集成Quartz定时任务框架介绍和Cron表达式详解 - z69183787的专栏 - CSDN博客
2013年12月08日 18:46:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1031
在JavaEE系统中，我们会经常用到定时任务，比如每天凌晨生成前天报表，每一小时生成汇总数据等等。
我们可以使用java.util.Timer结合java.util.TimerTask来完成这项工作，但时调度控制非常不方便，并且我们需要大量的代码。
使用Quartz框架无疑是非常好的选择，并且与Spring可以非常方便的集成，下面介绍它们集成方法和Cron表达式的详细介绍。
**一、增加所依赖的JAR包**
1、增加Spring的Maven依赖
```
<dependency>
 <groupId>org.springframework</groupId>
 <artifactId>spring-webmvc</artifactId>
 <version>3.0.5.RELEASE</version>
</dependency>
```
2、增加Quartz的Maven依赖
```
<dependency>
 <groupId>org.quartz-scheduler</groupId>
 <artifactId>quartz</artifactId>
 <version>1.8.4</version>
</dependency>
```
**二、增加定时业务逻辑类**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class ExpireJobTask {
    /** Logger */
    private static final Logger logger = LoggerFactory.getLogger(ExpireJobTask.class);
 
    /**
     * 业务逻辑处理
     */
    public void doBiz() {
  // 执行业务逻辑
  // ........
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
ExpireJobTask业务逻辑类与一般普通的类没有任务区别，它定义的doBiz方法即为调度业务方法。
**三、增加Spring配置**
1、增加一个线程池
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!-- 线程执行器配置，用于任务注册 -->
<bean id="executor" class="org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor">
 <property name="corePoolSize" value="10" />
 <property name="maxPoolSize" value="100" />
 <property name="queueCapacity" value="500" />
</bean>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
2、定义业务逻辑处理类
```
<!-- 业务对象 -->
<bean id="bizObject" class="com.aboy.potak.common.toolkit.scheduling.ExpireJobTask" />
```
3、增加调度业务逻辑
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!-- 调度业务 -->
<bean id="jobDetail" class="org.springframework.scheduling.quartz.MethodInvokingJobDetailFactoryBean">
 <property name="targetObject" ref="bizObject" />
 <property name="targetMethod" value="doBiz" />
</bean>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
上面的配置中，我们以bizObject.doBiz方法为将要调度的业务执行逻辑。
4、增加调度触发器
```
<bean id="cronTrigger" class="org.springframework.scheduling.quartz.CronTriggerBean">
 <property name="jobDetail" ref="jobDetail" />
 <property name="cronExpression" value="10 0/1 * * * ?" />
</bean>
```
Cron表达式“10 */1 * * * ?”意为：从10秒开始，每1分钟执行一次。
```
<bean id="taskTrigger" class="org.springframework.scheduling.quartz.SimpleTriggerBean">
 <property name="jobDetail" ref="jobDetail" />
 <property name="startDelay" value="10000" />
 <property name="repeatInterval" value="60000" />
</bean>
```
该调度表示，延迟10秒启动，然后每隔1分钟执行一次。
5、增加调度
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!-- 设置调度 -->
<bean class="org.springframework.scheduling.quartz.SchedulerFactoryBean">
 <property name="triggers">
  <list>
   <ref bean="cronTrigger" />
  </list>
 </property>
 <property name="taskExecutor" ref="executor" />
</bean>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
triggers属性中，我们可以增加多个触发器。
到此，Spring已经与Quartz完美的结合了，我们接下来的工作就是启动系统，开始调度了。
**四、Cron表达式的详细用法**
字段 允许值 允许的特殊字符 
秒 0-59 , - * / 
分 0-59 , - * / 
小时 0-23 , - * / 
日期 1-31 , - * ? / L W C 
月份 1-12 或者 JAN-DEC , - * / 
星期 1-7 或者 SUN-SAT , - * ? / L C # 
年（可选） 留空, 1970-2099 , - * /
例子：
0/5 * * * * ? ： 每5秒执行一次
“*”字符被用来指定所有的值。如："*"在分钟的字段域里表示“每分钟”。 
“?”字符只在日期域和星期域中使用。它被用来指定“非明确的值”。当你需要通过在这两个域中的一个来指定一些东西的时候，它是有用的。看下面的例子你就会明白。 
月份中的日期和星期中的日期这两个元素时互斥的一起应该通过设置一个问号来表明不想设置那个字段。
“-”字符被用来指定一个范围。如：“10-12”在小时域意味着“10点、11点、12点”。
“,”字符被用来指定另外的值。如：“MON,WED,FRI”在星期域里表示”星期一、星期三、星期五”。
“/”字符用于指定增量。如：“0/15”在秒域意思是每分钟的0，15，30和45秒。“5/15”在分钟域表示每小时的5，20，35和50。符号“*”在“/”前面（如：*/10）等价于0在“/”前面（如：0/10）。记住一条本质：表达式的每个数值域都是一个有最大值和最小值的集合，如：秒域和分钟域的集合是0-59，日期域是1-31，月份域是1-12。字符“/”可以帮助你在每个字符域中取相应的数值。如：“7/6”在月份域的时候只有当7月的时候才会触发，并不是表示每个6月。
L是‘last’的省略写法可以表示day-of-month和day-of-week域，但在两个字段中的意思不同，例如day-of-month域中表示一个月的最后一天。如果在day-of-week域表示‘7’或者‘SAT’，如果在day-of-week域中前面加上数字，它表示一个月的最后几天，例如‘6L’就表示一个月的最后一个星期五。
字符“W”只允许日期域出现。这个字符用于指定日期的最近工作日。例如：如果你在日期域中写 “15W”，表示：这个月15号最近的工作日。所以，如果15号是周六，则任务会在14号触发。如果15好是周日，则任务会在周一也就是16号触发。如果是在日期域填写“1W”即使1号是周六，那么任务也只会在下周一，也就是3号触发，“W”字符指定的最近工作日是不能够跨月份的。字符“W”只能配合一个单独的数值使用，不能够是一个数字段，如：1-15W是错误的。
“L”和“W”可以在日期域中联合使用，LW表示这个月最后一周的工作日。
字符“#”只允许在星期域中出现。这个字符用于指定本月的某某天。例如：“6#3”表示本月第三周的星期五（6表示星期五，3表示第三周）。“2#1”表示本月第一周的星期一。“4#5”表示第五周的星期三。
字符“C”允许在日期域和星期域出现。这个字符依靠一个指定的“日历”。也就是说这个表达式的值依赖于相关的“日历”的计算结果，如果没有“日历”关联，则等价于所有包含的“日历”。如：日期域是“5C”表示关联“日历”中第一天，或者这个月开始的第一天的后5天。星期域是“1C”表示关联“日历”中第一天，或者星期的第一天的后1天，也就是周日的后一天（周一）。
**五、表达式举例**
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
------------------------
欢迎大家批评指正:
http://obullxl.iteye.com
http://www.cnblogs.com/obullxl
http://hi.baidu.com/obullxl
-----------------------
------ 缘在，惜缘；缘去，随缘！ 博客园：http://obullxl.cnblogs.com 百度引擎：http://obullxl.duapp.com 红帽引擎：https://obullxl-osnode.rhcloud.com GitHub：http://obullxl.github.io JavaEye：http://obullxl.iteye.com ------
