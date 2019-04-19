# Spring3整合Quartz2实现定时任务及动态任务调整(添加删除暂停恢复)--推荐 - 零度的博客专栏 - CSDN博客
2016年05月17日 17:44:07[零度anngle](https://me.csdn.net/zmx729618)阅读数：3947
### 1、[常规整合](http://www.meiriyouke.net/?p=82)
[**http://www.dexcoder.com/selfly/article/308**](http://www.dexcoder.com/selfly/article/308)
最近工作中需要用到定时任务的功能，虽然Spring3也自带了一个轻量级的定时任务实现，但感觉不够灵活，功能也不够强大。在考虑之后，决定整合更为专业的Quartz来实现定时任务功能。
首先，当然是添加依赖的jar文件，我的项目是maven管理的，以下的我项目的依赖：
```
<dependencies>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-core</artifactId>
        <version>${spring.version}</version>
    </dependency>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>${spring.version}</version>
    </dependency>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-web</artifactId>
        <version>${spring.version}</version>
    </dependency>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-tx</artifactId>
        <version>${spring.version}</version>
    </dependency>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-jdbc</artifactId>
        <version>${spring.version}</version>
    </dependency>
    <dependency>
        <groupId>org.mybatis</groupId>
        <artifactId>mybatis</artifactId>
        <version>${mybatis.version}</version>
    </dependency>
    <dependency>
        <groupId>org.aspectj</groupId>
        <artifactId>aspectjweaver</artifactId>
        <version>1.7.4</version>
    </dependency>
    <dependency>
        <groupId>org.mybatis</groupId>
        <artifactId>mybatis-spring</artifactId>
        <version>${mybatis.spring.version}</version>
    </dependency>
    <dependency>
        <groupId>org.slf4j</groupId>
        <artifactId>slf4j-log4j12</artifactId>
        <version>${slf4j.version}</version>
    </dependency>
    <dependency>
        <groupId>commons-lang</groupId>
        <artifactId>commons-lang</artifactId>
        <version>${commons.lang.version}</version>
    </dependency>
    <dependency>
        <groupId>commons-dbcp</groupId>
        <artifactId>commons-dbcp</artifactId>
        <version>${commons.dbcp.version}</version>
    </dependency>
    <dependency>
        <groupId>com.oracle</groupId>
        <artifactId>ojdbc14</artifactId>
        <version>${ojdbc.version}</version>
    </dependency>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context-support</artifactId>
        <version>${spring.version}</version>
    </dependency>
    <dependency>
        <groupId>org.quartz-scheduler</groupId>
        <artifactId>quartz</artifactId>
        <version>${quartz.version}</version>
    </dependency>
</dependencies>
```
      或许你应该看出来了，我的项目是spring整合了mybatis，目前spring的最新版本已经到了4.x系列，但是最新版的mybatis-spring的整合插件所依赖推荐的依然是spring 3.1.3.RELEASE，所以这里没有用spring的最新版而是用了推荐的3.1.3.RELEASE，毕竟最新版本的功能一般情况下也用不到。至于quartz，则是用了目前的最新版2.2.1之所以在这里特别对版本作一下说明，是因为spring和quartz的整合对版本是有要求的。
     spring3.1以下的版本必须使用quartz1.x系列，3.1以上的版本才支持quartz 2.x，不然会出错。
> 
至于原因，则是spring对于quartz的支持实现，org.springframework.scheduling.quartz.CronTriggerBean继承了org.quartz.CronTrigger，在quartz1.x系列中org.quartz.CronTrigger是个类，而在quartz2.x系列中org.quartz.CronTrigger变成了接口，从而造成无法用spring的方式配置quartz的触发器（trigger）。
     在Spring中使用Quartz有两种方式实现：第一种是任务类继承QuartzJobBean，第二种则是在配置文件里定义任务类和要执行的方法，类和方法可以是普通类。很显然，第二种方式远比第一种方式来的灵活。
      这里采用的就是第二种方式。
spring配置文件：
```
<!-- 使用MethodInvokingJobDetailFactoryBean，任务类可以不实现Job接口，通过targetMethod指定调用方法-->
<bean id="taskJob" class="com.tyyd.dw.task.DataConversionTask"/>
<bean id="jobDetail" class="org.springframework.scheduling.quartz.MethodInvokingJobDetailFactoryBean">
    <property name="group" value="job_work"/>
    <property name="name" value="job_work_name"/>
    <!--false表示等上一个任务执行完后再开启新的任务-->
    <property name="concurrent" value="false"/>
    <property name="targetObject">
        <ref bean="taskJob"/>
    </property>
    <property name="targetMethod">
        <value>run</value>
    </property>
</bean>
 
<!--  调度触发器 -->
<bean id="myTrigger"
      class="org.springframework.scheduling.quartz.CronTriggerFactoryBean">
    <property name="name" value="work_default_name"/>
    <property name="group" value="work_default"/>
    <property name="jobDetail">
        <ref bean="jobDetail" />
    </property>
    <property name="cronExpression">
        <value>0/5 * * * * ?</value>
    </property>
</bean>
 
<!-- 调度工厂 -->
<bean id="scheduler" class="org.springframework.scheduling.quartz.SchedulerFactoryBean">
    <property name="triggers">
        <list>
            <ref bean="myTrigger"/>
        </list>
    </property>
</bean>
Task类则是一个普通的Java类，没有继承任何类和实现任何接口(当然可以用注解方式来声明bean)：
//@Component
public class DataConversionTask{
 
    /** 日志对象 */
    private static final Logger LOG = LoggerFactory.getLogger(DataConversionTask.class);
 
    public void run() {
 
        if (LOG.isInfoEnabled()) {
            LOG.info("数据转换任务线程开始执行");
        }
    }
}
```
      至此，简单的整合大功告成，run方法将每隔5秒执行一次，因为配置了concurrent等于false，所以假如run方法的执行时间超过5秒，在执行完之前即使时间已经超过了5秒下一个定时计划执行任务仍不会被开启，如果是true，则不管是否执行完，时间到了都将开启。
       这里，顺便贴一下cronExpression表达式备忘：
字段 允许值 允许的特殊字符
秒 0-59 , – * /
分 0-59 , – * /
小时 0-23 , – * /
日期 1-31 , – * ? / L W C
月份 1-12 或者 JAN-DEC , – * /
星期 1-7 或者 SUN-SAT , – * ? / L C #
年（可选） 留空, 1970-2099 , – * /
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
###### 2、[动态添加任务](http://www.meiriyouke.net/?p=131)
      前面，我们已经对Spring 3和Quartz 2用配置文件的方式进行了整合，如果需求比较简单的话应该已经可以满足了。但是很多时候，我们常常会遇到需要动态的添加或修改任务，而spring中所提供的定时任务组件却只能够通过修改xml中trigger的配置才能控制定时任务的时间以及任务的启用或停止，这在带给我们方便的同时也失去了动态配置任务的灵活性。我搜索了一些网上的解决方法，都没有很好的解决这个问题，而且大多数提到的解决方案都停留在Quartz 1.x系列版本上，所用到的代码和API已经不能适用于新版本的Spring和Quartz。没办法只能靠自己了，花了点时间好好研究了一下Spring和Quartz中相关的代码。
       首先，我们来回顾一下spring中使用quartz的配置代码：
```
<!-- 使用MethodInvokingJobDetailFactoryBean，任务类可以不实现Job接口，通过targetMethod指定调用方法-->
<bean id="taskJob" class="com.tyyd.dw.task.DataConversionTask"/>
<bean id="jobDetail" class="org.springframework.scheduling.quartz.MethodInvokingJobDetailFactoryBean">
    <property name="group" value="job_work"/>
    <property name="name" value="job_work_name"/>
    <!--false表示等上一个任务执行完后再开启新的任务-->
    <property name="concurrent" value="false"/>
    <property name="targetObject">
        <ref bean="taskJob"/>
    </property>
    <property name="targetMethod">
        <value>execute</value>
    </property>
</bean>
 
<!--  调度触发器 -->
<bean id="myTrigger"
      class="org.springframework.scheduling.quartz.CronTriggerFactoryBean">
    <property name="name" value="work_default_name"/>
    <property name="group" value="work_default"/>
    <property name="jobDetail">
        <ref bean="jobDetail" />
    </property>
    <property name="cronExpression">
        <value>0/5 * * * * ?</value>
    </property>
</bean>
 
<!-- 调度工厂 -->
<bean id="scheduler" class="org.springframework.scheduling.quartz.SchedulerFactoryBean">
    <property name="triggers">
        <list>
            <ref bean="myTrigger"/>
        </list>
    </property>
</bean>
```
       所有的配置都在xml中完成，包括cronExpression表达式，十分的方便。但是如果我的任务信息是保存在数据库的，想要动态的初始化，而且任务较多的时候不是得有一大堆的xml配置？或者说我要修改一下trigger的表达式，使原来5秒运行一次的任务变成10秒运行一次，这时问题就来了，试过在配置文件中不传入cronExpression等参数，但是启动时就报错了，难道我每次都修改xml文件然后重启应用吗，这显然不合适的。最理想的是在与spring整合的同时又能实现动态任务的添加、删除及修改配置。
我们来看一下spring实现quartz的方式，先看一下上面配置文件中定义的jobDetail。其实上面生成的jobDetail并不是我们定义的Bean，因为在Quartz 2.x版本中JobDetail已经是一个接口（当然以前的版本也并非直接生成JobDetail）：
- publicinterfaceJobDetailextendsSerializable,Cloneable{...｝
       Spring是通过将其转换为MethodInvokingJob或StatefulMethodInvokingJob类型来实现的，这两个都是静态的内部类，MethodInvokingJob类继承于QuartzJobBean，而StatefulMethodInvokingJob则直接继承于MethodInvokingJob。 这两个类的实现区别在于有状态和无状态，对应于quartz的Job和StatefulJob，具体可以查看quartz文档，这里不再赘述。先来看一下它们实现的QuartzJobBean的主要代码：
```
/**
 * This implementation applies the passed-in job data map as bean property
 * values, and delegates to <code>executeInternal</code> afterwards.
 * @see #executeInternal
 */
public final void execute(JobExecutionContext context) throws JobExecutionException {
    try {
        // Reflectively adapting to differences between Quartz 1.x and Quartz 2.0...
        Scheduler scheduler = (Scheduler) ReflectionUtils.invokeMethod(getSchedulerMethod, context);
        Map mergedJobDataMap = (Map) ReflectionUtils.invokeMethod(getMergedJobDataMapMethod, context);
 
        BeanWrapper bw = PropertyAccessorFactory.forBeanPropertyAccess(this);
        MutablePropertyValues pvs = new MutablePropertyValues();
        pvs.addPropertyValues(scheduler.getContext());
        pvs.addPropertyValues(mergedJobDataMap);
        bw.setPropertyValues(pvs, true);
    }
    catch (SchedulerException ex) {
        throw new JobExecutionException(ex);
    }
    executeInternal(context);
}
 
/**
 * Execute the actual job. The job data map will already have been
 * applied as bean property values by execute. The contract is
 * exactly the same as for the standard Quartz execute method.
 * @see #execute
 */
protected abstract void executeInternal(JobExecutionContext context) throws JobExecutionException;
//还有MethodInvokingJobDetailFactoryBean中的代码：
public void afterPropertiesSet() throws ClassNotFoundException, NoSuchMethodException {
    prepare();
 
    // Use specific name if given, else fall back to bean name.
    String name = (this.name != null ? this.name : this.beanName);
 
    // Consider the concurrent flag to choose between stateful and stateless job.
    Class jobClass = (this.concurrent ? MethodInvokingJob.class : StatefulMethodInvokingJob.class);
 
    // Build JobDetail instance.
    if (jobDetailImplClass != null) {
        // Using Quartz 2.0 JobDetailImpl class...
        this.jobDetail = (JobDetail) BeanUtils.instantiate(jobDetailImplClass);
        BeanWrapper bw = PropertyAccessorFactory.forBeanPropertyAccess(this.jobDetail);
        bw.setPropertyValue("name", name);
        bw.setPropertyValue("group", this.group);
        bw.setPropertyValue("jobClass", jobClass);
        bw.setPropertyValue("durability", true);
        ((JobDataMap) bw.getPropertyValue("jobDataMap")).put("methodInvoker", this);
    }
    else {
        // Using Quartz 1.x JobDetail class...
        this.jobDetail = new JobDetail(name, this.group, jobClass);
        this.jobDetail.setVolatility(true);
        this.jobDetail.setDurability(true);
        this.jobDetail.getJobDataMap().put("methodInvoker", this);
    }
 
    // Register job listener names.
    if (this.jobListenerNames != null) {
        for (String jobListenerName : this.jobListenerNames) {
            if (jobDetailImplClass != null) {
                throw new IllegalStateException("Non-global JobListeners not supported on Quartz 2 - " +
                        "manually register a Matcher against the Quartz ListenerManager instead");
            }
            this.jobDetail.addJobListener(jobListenerName);
        }
    }
 
    postProcessJobDetail(this.jobDetail);
}
```
      上面主要看我们目前用的Quartz 2.0版本的实现部分，到这里或许你已经明白Spring对Quartz的封装原理了。Spring就是通过这种方式在最后Job真正执行时反调用到我们所注入的类和方法。
      现在，理解了Spring的实现原理后，我们就可以来设计我们自己的了。在设计时我想到以下几点：
      1、减少spring的配置文件，为了实现一个定时任务，spring的配置代码太多了。
      2、用户可以通过页面等方式添加、启用、禁用某个任务。
      3、用户可以修改某个已经在运行任务的运行时间表达式，CronExpression。
      4、为方便维护，简化任务的运行调用处理，任务的运行入口即Job实现类最好只有一个，该Job运行类相当于工厂类，在实际调用时把任务的相关信息通过参数方式传入，由该工厂类根据任务信息来具体执行需要的操作。
在上面的思路下来进行我们的开发吧。
##### 一、spring配置文件
通过研究，发现要实现我们的功能，只需要以下配置：
- <beanid="schedulerFactoryBean"class="org.springframework.scheduling.quartz.SchedulerFactoryBean"/>
##### 二、任务运行入口，即Job实现类，在这里我把它看作工厂类：
```
/**
 * 定时任务运行工厂类
 * 
 * User: liyd
 * Date: 14-1-3
 * Time: 上午10:11
 */
public class QuartzJobFactory implements Job {
 
    @Override
    public void execute(JobExecutionContext context) throws JobExecutionException {
        System.out.println("任务成功运行");
        ScheduleJob scheduleJob = (ScheduleJob)context.getMergedJobDataMap().get("scheduleJob");
        System.out.println("任务名称 = [" + scheduleJob.getJobName() + "]");
    }
}
```
这里我们实现的是无状态的Job，如果要实现有状态的Job在以前是实现StatefulJob接口，在我使用的quartz 2.2.1中，StatefulJob接口已经不推荐使用了，换成了注解的方式，只需要给你实现的Job类加上注解@DisallowConcurrentExecution即可实现有状态：
- /**
- * 定时任务运行工厂类
- * <p/>
- * User: liyd
- * Date: 14-1-3
- * Time: 上午10:11
- */
- @DisallowConcurrentExecution
- publicclassQuartzJobFactoryimplementsJob{...}
##### 三、创建任务
既然要动态的创建任务，我们的任务信息当然要保存在某个地方了，这里我们新建一个保存任务信息对应的实体类：
```
/**
 * 计划任务信息
 * 
 * User: liyd
 * Date: 14-1-3
 * Time: 上午10:24
 */
public class ScheduleJob {
 
    /** 任务id */
    private String jobId;
 
    /** 任务名称 */
    private String jobName;
 
    /** 任务分组 */
    private String jobGroup;
 
    /** 任务状态 0禁用 1启用 2删除*/
    private String jobStatus;
 
    /** 任务运行时间表达式 */
    private String cronExpression;
 
    /** 任务描述 */
    private String desc;
 
    getter and setter ....
}
```
接下来我们创建测试数据，实际应用中该数据可以保存在数据库等地方，我们把任务的分组名+任务名作为任务的唯一key，和quartz中的实现方式一致：
```
/** 计划任务map */
private static Map<String, ScheduleJob> jobMap = new HashMap<String, ScheduleJob>();
 
static {
    for (int i = 0; i < 5; i++) {
        ScheduleJob job = new ScheduleJob();
        job.setJobId("10001" + i);
        job.setJobName("data_import" + i);
        job.setJobGroup("dataWork");
        job.setJobStatus("1");
        job.setCronExpression("0/5 * * * * ?");
        job.setDesc("数据导入任务");
        addJob(job);
    }
}
 
/**
 * 添加任务
 * @param scheduleJob
 */
public static void addJob(ScheduleJob scheduleJob) {
    jobMap.put(scheduleJob.getJobGroup() + "_" + scheduleJob.getJobName(), scheduleJob);
}
```
有了调度工厂，有了任务运行入口实现类，有了任务信息，接下来就是创建我们的定时任务了，在这里我把它设计成一个Job对应一个trigger，两者的分组及名称相同，方便管理，条理也比较清晰，在创建任务时如果不存在新建一个，如果已经存在则更新任务，主要代码如下：
```
//schedulerFactoryBean 由spring创建注入
Scheduler scheduler = schedulerFactoryBean.getScheduler();
 
//这里获取任务信息数据
List<ScheduleJob> jobList = DataWorkContext.getAllJob();
 
for (ScheduleJob job : jobList) {
 
    TriggerKey triggerKey = TriggerKey.triggerKey(job.getJobName(), job.getJobGroup());
 
    //获取trigger，即在spring配置文件中定义的 bean id="myTrigger"
    CronTrigger trigger = (CronTrigger) scheduler.getTrigger(triggerKey);
 
    //不存在，创建一个
    if (null == trigger) {
        JobDetail jobDetail = JobBuilder.newJob(QuartzJobFactory.class)
            .withIdentity(job.getJobName(), job.getJobGroup()).build();
        jobDetail.getJobDataMap().put("scheduleJob", job);
 
        //表达式调度构建器
        CronScheduleBuilder scheduleBuilder = CronScheduleBuilder.cronSchedule(job
            .getCronExpression());
 
        //按新的cronExpression表达式构建一个新的trigger
        trigger = TriggerBuilder.newTrigger().withIdentity(job.getJobName(), job.getJobGroup()).withSchedule(scheduleBuilder).build();
 
        scheduler.scheduleJob(jobDetail, trigger);
    } else {
        // Trigger已存在，那么更新相应的定时设置
        //表达式调度构建器
        CronScheduleBuilder scheduleBuilder = CronScheduleBuilder.cronSchedule(job
            .getCronExpression());
 
        //按新的cronExpression表达式重新构建trigger
        trigger = trigger.getTriggerBuilder().withIdentity(triggerKey)
            .withSchedule(scheduleBuilder).build();
 
        //按新的trigger重新设置job执行
        scheduler.rescheduleJob(triggerKey, trigger);
    }
}
```
如此，可以说已经完成了我们的动态任务创建，大功告成了。有了上面的代码，添加和修改任务是不是也会了，顺道解决了？
上面我们创建的5个测试任务，都是5秒执行一次，都将调用QuartzJobFactory的execute方法，但是传入的任务信息参数不同，execute方法中的如下代码就是得到具体的任务信息，包括任务分组和任务名：
- ScheduleJob scheduleJob =(ScheduleJob)context.getMergedJobDataMap().get("scheduleJob");
有了任务分组和任务名即确定了该任务的唯一性，接下来需要什么操作实现起来是不是就很容易了？
以后需要添加新的定时任务只需要在任务信息列表中加入记录即可，然后在execute方法中通过判断任务分组和任务名来实现你具体的操作。
以上已经初始实现了我们需要的功能，增加和修改也已经可以通过源代码举一反三出来，但是我们在实际开发的时候需要进行测试，如果一个任务是1个小时运行一次的，测试起来是不是很不方便？当然你可以修改任务的运行时间表达式，但相信这不是最好的方法，接下来我们就要实现在不对当前任务信息做任何修改的情况下触发任务，并且该触发只会运行一次作测试用。待续，，，
###  3、[动态暂停 恢复 修改和删除任务](http://www.meiriyouke.net/?p=140)
       前面我们已经完成了spring 3和quartz 2的整合以及动态添加定时任务，我们接着来完善它，使之能支持更多的操作，例如暂停、恢复、修改等。
在动态添加定时任务中其实已经涉及到了其中的一些代码，这里我们再来细化的理一理。先来看一下我们初步要实现的目标效果图，这里我们只在内存中操作，并没有把quartz的任何信息保存到数据库，即使用的是RAMJobStore，当然如果你有需要，可以实现成JDBCJobStore，那样任务信息将会更全面，貌似还有专门的监控工具，不过本人没有用过。我们要先列出计划中的定时任务以及正在执行中的定时任务，这里的正在执行中指的是任务已经触发线程还没执行完的情况。比如每天2点执行一个数据导入操作，这个操作执行时间需要5分钟，在这5分钟之内这个任务才是运行中的任务。当任务正常时可以使用暂停按钮，任务暂停时可以使用恢复按钮。
      trigger各状态说明：
None：Trigger已经完成，且不会在执行，或者找不到该触发器，或者Trigger已经被删除
NORMAL:正常状态
PAUSED：暂停状态
COMPLETE：触发器完成，但是任务可能还正在执行中
BLOCKED：线程阻塞状态
ERROR：出现错误
##### 一、计划中的任务
指那些已经添加到quartz调度器的任务，因为quartz并没有直接提供这样的查询接口，所以我们需要结合JobKey和Trigger来实现，核心代码：
```
Scheduler scheduler = schedulerFactoryBean.getScheduler();
GroupMatcher<JobKey> matcher = GroupMatcher.anyJobGroup();
Set<JobKey> jobKeys = scheduler.getJobKeys(matcher);
List<ScheduleJob> jobList = new ArrayList<ScheduleJob>();
for (JobKey jobKey : jobKeys) {
    List<? extends Trigger> triggers = scheduler.getTriggersOfJob(jobKey);
    for (Trigger trigger : triggers) {
        ScheduleJob job = new ScheduleJob();
        job.setJobName(jobKey.getName());
        job.setJobGroup(jobKey.getGroup());
        job.setDesc("触发器:" + trigger.getKey());
        Trigger.TriggerState triggerState = scheduler.getTriggerState(trigger.getKey());
        job.setJobStatus(triggerState.name());
        if (trigger instanceof CronTrigger) {
            CronTrigger cronTrigger = (CronTrigger) trigger;
            String cronExpression = cronTrigger.getCronExpression();
            job.setCronExpression(cronExpression);
        }
        jobList.add(job);
    }
}
```
上面代码中的jobList就是我们需要的计划中的任务列表，需要注意一个job可能会有多个trigger的情况，在下面讲到的立即运行一次任务的时候，会生成一个临时的trigger也会出现在这。这里把一个Job有多个trigger的情况看成是多个任务。我们前面包括在实际项目中一般用到的都是CronTrigger ，所以这里我们着重处理了下CronTrigger的情况。
##### 二、运行中的任务
实现和计划中的任务类似，核心代码：
```
Scheduler scheduler = schedulerFactoryBean.getScheduler();
List<JobExecutionContext> executingJobs = scheduler.getCurrentlyExecutingJobs();
List<ScheduleJob> jobList = new ArrayList<ScheduleJob>(executingJobs.size());
for (JobExecutionContext executingJob : executingJobs) {
    ScheduleJob job = new ScheduleJob();
    JobDetail jobDetail = executingJob.getJobDetail();
    JobKey jobKey = jobDetail.getKey();
    Trigger trigger = executingJob.getTrigger();
    job.setJobName(jobKey.getName());
    job.setJobGroup(jobKey.getGroup());
    job.setDesc("触发器:" + trigger.getKey());
    Trigger.TriggerState triggerState = scheduler.getTriggerState(trigger.getKey());
    job.setJobStatus(triggerState.name());
    if (trigger instanceof CronTrigger) {
        CronTrigger cronTrigger = (CronTrigger) trigger;
        String cronExpression = cronTrigger.getCronExpression();
        job.setCronExpression(cronExpression);
    }
    jobList.add(job);
}
```
##### 三、暂停任务
这个比较简单，核心代码：
- Scheduler scheduler = schedulerFactoryBean.getScheduler();
- JobKey jobKey =JobKey.jobKey(scheduleJob.getJobName(), scheduleJob.getJobGroup());
- scheduler.pauseJob(jobKey);
##### 四、恢复任务
和暂停任务相对，核心代码：
- Scheduler scheduler = schedulerFactoryBean.getScheduler();
- JobKey jobKey =JobKey.jobKey(scheduleJob.getJobName(), scheduleJob.getJobGroup());
- scheduler.resumeJob(jobKey);
##### 五、删除任务
删除任务后，所对应的trigger也将被删除
- Scheduler scheduler = schedulerFactoryBean.getScheduler();
- JobKey jobKey =JobKey.jobKey(scheduleJob.getJobName(), scheduleJob.getJobGroup());
- scheduler.deleteJob(jobKey);
##### 六、立即运行任务
这里的立即运行，只会运行一次，方便测试时用。quartz是通过临时生成一个trigger的方式来实现的，这个trigger将在本次任务运行完成之后自动删除。trigger的key是随机生成的，例如：DEFAULT.MT_4k9fd10jcn9mg。在我的测试中，前面的DEFAULT.MT是固定的，后面部分才随机生成。
- Scheduler scheduler = schedulerFactoryBean.getScheduler();
- JobKey jobKey =JobKey.jobKey(scheduleJob.getJobName(), scheduleJob.getJobGroup());
- scheduler.triggerJob(jobKey);
##### 七、更新任务的时间表达式
更新之后，任务将立即按新的时间表达式执行：
```
Scheduler scheduler = schedulerFactoryBean.getScheduler();
 
TriggerKey triggerKey = TriggerKey.triggerKey(scheduleJob.getJobName(),
    scheduleJob.getJobGroup());
 
//获取trigger，即在spring配置文件中定义的 bean id="myTrigger"
CronTrigger trigger = (CronTrigger) scheduler.getTrigger(triggerKey);
 
//表达式调度构建器
CronScheduleBuilder scheduleBuilder = CronScheduleBuilder.cronSchedule(scheduleJob
    .getCronExpression());
 
//按新的cronExpression表达式重新构建trigger
trigger = trigger.getTriggerBuilder().withIdentity(triggerKey)
    .withSchedule(scheduleBuilder).build();
 
//按新的trigger重新设置job执行
scheduler.rescheduleJob(triggerKey, trigger);
```
        到这里，我们的spring3 整合quartz 2的定时任务功能终于是告一段落了，对常用的一些功能进行了实现，相信可以满足一般项目的需求了。
