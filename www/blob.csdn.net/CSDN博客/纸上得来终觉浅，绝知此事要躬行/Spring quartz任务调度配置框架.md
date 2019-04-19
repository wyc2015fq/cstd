# Spring quartz任务调度配置框架 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年04月10日 09:50:26[boonya](https://me.csdn.net/boonya)阅读数：218标签：[spring																[quartz](https://so.csdn.net/so/search/s.do?q=quartz&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[Spring																[作业调度](https://blog.csdn.net/boonya/article/category/1714157)](https://blog.csdn.net/boonya/article/category/1721331)
Spring 通过对[quartz](http://www.quartz-scheduler.org/)的封装至少可以追溯到spring3.0.5，[quartz](http://www.quartz-scheduler.org/)的到来让我们不再满足于简单地使用Timer来启一个定时任务。下面的例子基于一个老项目的缓存清除问题，使用spring3.0.5进行配置，另外提供spring4.x配置示例。
**目录**
[corn语法](#corn%E8%AF%AD%E6%B3%95)
[spring3.x配置](#spring3.x%E9%85%8D%E7%BD%AE)
[spring-jobs.xml配置](#spring-jobs.xml%E9%85%8D%E7%BD%AE)
[Java bean 任务实现](#Java%20bean%20%E4%BB%BB%E5%8A%A1%E5%AE%9E%E7%8E%B0)
[web.xml配置加载spring-jobs.xml](#web.xml%E9%85%8D%E7%BD%AE%E5%8A%A0%E8%BD%BDspring-jobs.xml)
[Spring4.x配置](#Spring4.x%E9%85%8D%E7%BD%AE)
[applicationContext.xml](#applicationContext.xml)
[Java 任务实现类](#Java%20%E4%BB%BB%E5%8A%A1%E5%AE%9E%E7%8E%B0%E7%B1%BB)
[启动spring测试](#%E5%90%AF%E5%8A%A8spring%E6%B5%8B%E8%AF%95)
# corn语法
CronTrigger配置格式:
格式: [秒] [分] [小时] [日] [月] [周] [年]
|序号|说明|是否必填|允许填写的值|允许的通配符|
|----|----|----|----|----|
|1|秒|是|0-59|, - * /|
|2|分|是|0-59|, - * /|
|3|小时|是|0-23|, - * /|
|4|日|是|1-31|, - * ? / L W|
|5|月|是|1-12 or JAN-DEC|, - * /|
|6|周|是|1-7 or SUN-SAT|, - * ? / L #|
|7|年|否|empty 或 1970-2099|, - * /|
**说明：**
       （1）*：表示匹配该域的任意值。假如在Minutes域使用*, 即表示每分钟都会触发事件。
　　（2）?：只能用在DayofMonth和DayofWeek两个域。它也匹配域的任意值，但实际不会。因为DayofMonth和DayofWeek会相互影响。例如想在每月的20日触发调度，不管20日到底是星期几，则只能使用如下写法： 13 13 15 20 * ?, 其中最后一位只能用？，而不能使用*，如果使用*表示不管星期几都会触发，实际上并不是这样。
　　（3）-：表示范围。例如在Minutes域使用5-20，表示从5分到20分钟每分钟触发一次 
　　（4）/：表示起始时间开始触发，然后每隔固定时间触发一次。例如在Minutes域使用5/20,则意味着5分钟触发一次，而25，45等分别触发一次. 
　　（5）,：表示列出枚举值。例如：在Minutes域使用5,20，则意味着在5和20分每分钟触发一次。 
　　（6）L：表示最后，只能出现在DayofWeek和DayofMonth域。如果在DayofWeek域使用5L,意味着在最后的一个星期四触发。 
　　（7）W:表示有效工作日(周一到周五),只能出现在DayofMonth域，系统将在离指定日期的最近的有效工作日触发事件。例如：在 DayofMonth使用5W，如果5日是星期六，则将在最近的工作日：星期五，即4日触发。如果5日是星期天，则在6日(周一)触发；如果5日在星期一到星期五中的一天，则就在5日触发。另外一点，W的最近寻找不会跨过月份 。
　　（8）LW:这两个字符可以连用，表示在某个月最后一个工作日，即最后一个星期五。 
　　（9）#:用于确定每个月第几个星期几，只能出现在DayofMonth域。例如在4#2，表示某月的第二个星期三。
**示例：**
|0 0 12 * * ?|每天12点触发|
|----|----|
|0 15 10 ? * *|每天10点15分触发|
|0 15 10 * * ?|每天10点15分触发|
|0 15 10 * * ? *|每天10点15分触发|
|0 15 10 * * ? 2005|2005年每天10点15分触发|
|0 * 14 * * ?|每天下午的 2点到2点59分每分触发|
|0 0/5 14 * * ?|每天下午的 2点到2点59分(整点开始，每隔5分触发)|
|0 0/5 14,18 * * ?|每天下午的 2点到2点59分(整点开始，每隔5分触发) 每天下午的 18点到18点59分(整点开始，每隔5分触发)|
|0 0-5 14 * * ?|每天下午的 2点到2点05分每分触发|
|0 10,44 14 ? 3 WED|3月分每周三下午的 2点10分和2点44分触发|
|0 15 10 ? * MON-FRI|从周一到周五每天上午的10点15分触发|
|0 15 10 15 * ?|每月15号上午10点15分触发|
|0 15 10 L * ?|每月最后一天的10点15分触发|
|0 15 10 ? * 6L|每月最后一周的星期五的10点15分触发|
|0 15 10 ? * 6L 2002-2005|从2002年到2005年每月最后一周的星期五的10点15分触发|
|0 15 10 ? * 6#3|每月的第三周的星期五开始触发|
|0 0 12 1/5 * ?|每月的第一个中午开始每隔5天触发一次|
|0 11 11 11 11 ?|每年的11月11号 11点11分触发(光棍节)|
**corn生成工具**
[http://www.bejson.com/othertools/cron/](http://www.bejson.com/othertools/cron/)
# spring3.x配置
## spring-jobs.xml配置
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xmlns:tx="http://www.springframework.org/schema/tx"
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
        http://www.springframework.org/schema/context
		http://www.springframework.org/schema/context/spring-context-3.0.xsd
        http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd
        http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.5.xsd">
   <!-- 线程执行器配置:考虑多任务执行情况-->
	<bean id="executor" class="org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor">
		 <property name="corePoolSize" value="10" />
		 <property name="maxPoolSize" value="100" />
		 <property name="queueCapacity" value="500" />
	</bean>
	<!-- 任务对象 -->
    <bean id="ssgjTask" class="com.forestar.cache.XHTCacheQuartzClearTask" />
    
	<!-- 配置任务 -->
	<bean id="ssgjClearJob"  class="org.springframework.scheduling.quartz.MethodInvokingJobDetailFactoryBean">
		 <property name="targetObject" ref="ssgjTask" />
		 <property name="targetMethod" value="ssgjClear" />
	</bean>
    <!-- 配置触发器 -->
    <bean id="ssgjCornTrigger"   class="org.springframework.scheduling.quartz.CronTriggerBean">
        <property name="jobDetail" ref="ssgjClearJob" />
        <property name="cronExpression" value="0 0 0 * * ?" />
    </bean>
    <!-- 注册触发器 -->
    <bean class="org.springframework.scheduling.quartz.SchedulerFactoryBean" >
        <property name="taskExecutor" ref="executor"/> 
	    <property name="triggers">
	         <!-- 注册多个Trigger -->
	         <list>
	             <!--  <ref bean="saveRoomList" /> -->
	              <ref bean="ssgjCornTrigger" />
	         </list>
	    </property>
	</bean>
</beans>
```
## Java bean 任务实现
```java
package com.xxxxx.cache;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import org.quartz.JobExecutionException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import data.common.exception.ServiceException;
import data.general.RowBase;
import data.metadatmanager.ListEntity;
import data.service.BaseDataService;
/**
 * 
 * @author PJL
 *
 * @filename XHTCacheQuartzClearTask.java
 * @date     2019年4月10日 上午9:26:33
 */
@Component
public class XHTCacheQuartzClearTask {
	@Autowired
	BaseDataService baseDataService;
	
	/**
	 * 获取总表护林员ID
	 * 
	 * @return
	 */
	@SuppressWarnings({ "rawtypes" })
	private List<String>  queryHlyIdList(){
		List<String> tableNameList = new ArrayList<String>();
		String sql = "SELECT HLY_ID from XH_HLY_TB_ZB";
		String tableName = "XH_HLY_TB_ZB";
		try {
			ListEntity<RowBase> rows = baseDataService.getDataTableSql(tableName,sql, null);
			if(null!=rows){
				for (RowBase row : rows) {
					String hlyId = row.getOriginalObjects().get("HLY_ID").toString();
					tableNameList.add(hlyId);
				}
			}
		} catch (ServiceException e) {
			e.printStackTrace();
		}
		return tableNameList;
	}
	
	/**
	 * 执行业务处理
	 * @param arg0
	 * @throws JobExecutionException
	 */
	public void ssgjClear(){
		//ArrayList list = (ArrayList) XHTCacheManager.getModeByCache("ssgj_"+userId);
		long start=new Date().getTime();
		System.out.println("执行调度任务:XHTCacheQuartzClearTask..."); 
		List<String> ids=this.queryHlyIdList();
		for (String userId : ids) {
			synchronized (this) {
				XHTCacheManager.remove("ssgj_"+userId);
			}
		}
		long end=new Date().getTime();
		System.out.println("执行调度任务:XHTCacheQuartzClearTask...完成,耗时:"+(end-start)+"ms!"); 
	}
	
}
```
## web.xml配置加载spring-jobs.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<!-- 引入外部xml文件 -->
<web-app version="2.5" 
	xmlns="http://java.sun.com/xml/ns/javaee" 
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
	http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
	
	<!-- spring文件导入 -->
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>
		    /WEB-INF/classes/resources/spring/spring-config.xml
		    /WEB-INF/classes/resources/spring/spring-jobs.xml
		    /WEB-INF/spring-servlet.xml
		</param-value>
	</context-param>
 ....
</web-app>
```
如此我们便实现了spring的quartz任务调度配置，并且可以通过配置线程执行器实现多个任务并发执行。
# Spring4.x配置
## *applicationContext.xml*
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:mvc="http://www.springframework.org/schema/mvc"
    xmlns:p="http://www.springframework.org/schema/p" xmlns:context="http://www.springframework.org/schema/context"
    xmlns:aop="http://www.springframework.org/schema/aop" xmlns:tx="http://www.springframework.org/schema/tx"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
            http://www.springframework.org/schema/context 
            http://www.springframework.org/schema/context/spring-context-3.0.xsd
            http://www.springframework.org/schema/aop 
            http://www.springframework.org/schema/aop/spring-aop-3.0.xsd
            http://www.springframework.org/schema/tx 
            http://www.springframework.org/schema/tx/spring-tx-3.0.xsd
            http://www.springframework.org/schema/mvc 
            http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd
            http://www.springframework.org/schema/context 
            http://www.springframework.org/schema/context/spring-context-3.0.xsd">
    <!-- 定义任务Bean -->
    <bean name="myJobDetail" class="org.springframework.scheduling.quartz.JobDetailFactoryBean">
        <!-- 指定具体的job类 -->
        <property name="jobClass" value="com.quartz.Myjob"></property>
        <!-- 指定job的名称 -->
        <property name="name" value="myJob"></property>
        <!-- 指定job的分组 -->
        <property name="group" value="jobs"></property>
        <!-- 必须设置为true，如果为false，当没有活动的触发器与之关联的时候回在调度器中删除该任务 -->
        <property name="durability" value="true"></property>
        <!-- 指定spring容器的可以，如果不设定在job中的jobmap中是获取不到spring容器的 -->
        <property name="applicationContextJobDataKey" value="applicationContext"></property>
    </bean>
    <!-- 定义触发器 -->
    <bean id="cronTrigger" class="org.springframework.scheduling.quartz.CronTriggerFactoryBean">
        <property name="jobDetail" ref="myJobDetail"></property>
        <!-- 定义定时表达式 -->
        <property name="cronExpression" value="1/5 * * * * ?"></property>
    </bean>
    <!-- 定义调度器 -->
    <bean class="org.springframework.scheduling.quartz.SchedulerFactoryBean">
        <property name="triggers">
            <list>
                <ref bean="cronTrigger"/>
            </list>
        </property>
    </bean>
</beans>
```
## Java 任务实现类
```java
import org.quartz.JobExecutionContext;
import org.quartz.JobExecutionException;
import org.springframework.context.ApplicationContext;
import org.springframework.scheduling.quartz.QuartzJobBean;
public class Myjob extends QuartzJobBean{
    private static int count=1;
    @Override
    protected void executeInternal(JobExecutionContext arg0) throws JobExecutionException {
        System.out.println("Myjob开始执行了。。。。"+arg0.getTrigger().getKey().getName());
        ApplicationContext applicationContext=(ApplicationContext)arg0.getJobDetail().getJobDataMap()
                .get("applicationContext");
        System.out.println("获取到的容器是："+(count++)+"|"+applicationContext);
    }
}
```
## 启动spring测试
```java
import org.springframework.context.support.ClassPathXmlApplicationContext;
public class Main {
    public static void main(String[] args) {
        new ClassPathXmlApplicationContext("classpath:applicationContext.xml");
    }
}
```
如此spring4和3的方式都差不多，只是引用实现有点差异。
