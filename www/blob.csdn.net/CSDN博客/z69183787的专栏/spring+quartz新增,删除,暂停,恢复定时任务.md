# spring+quartz新增,删除,暂停,恢复定时任务 - z69183787的专栏 - CSDN博客
2018年04月28日 21:07:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：287
spring配置
[html][view
 plain](https://blog.csdn.net/a3781386/article/details/51470013#)[copy](https://blog.csdn.net/a3781386/article/details/51470013#)
- <beanid="scheduler"
- class="org.springframework.scheduling.quartz.SchedulerFactoryBean">
- <!--     <property name="startupDelay" value="180"/>  启动服务器后延迟启动定时任务-->
- </bean>
先定义SchedulerJob  javabean
[java][view
 plain](https://blog.csdn.net/a3781386/article/details/51470013#)[copy](https://blog.csdn.net/a3781386/article/details/51470013#)
- publicclass ScheduleJob {  
- private String jobId;//任务id
- private String jobName;//任务名称
- private String jobGroup;//任务分组
- privateint jobStatus;//任务状态 0禁用 1启用 2删除
- private String cronExpression;//任务运行时间表达式
- private String jobDesc;//任务描述
- public String getJobId() {  
- return jobId;  
-     }  
- publicvoid setJobId(String jobId) {  
- this.jobId = jobId;  
-     }  
- public String getJobName() {  
- return jobName;  
-     }  
- publicvoid setJobName(String jobName) {  
- this.jobName = jobName;  
-     }  
- public String getJobGroup() {  
- return jobGroup;  
-     }  
- publicvoid setJobGroup(String jobGroup) {  
- this.jobGroup = jobGroup;  
-     }  
- 
- publicint getJobStatus() {  
- return jobStatus;  
-     }  
- publicvoid setJobStatus(int jobStatus) {  
- this.jobStatus = jobStatus;  
-     }  
- public String getCronExpression() {  
- return cronExpression;  
-     }  
- publicvoid setCronExpression(String cronExpression) {  
- this.cronExpression = cronExpression;  
-     }  
- public String getJobDesc() {  
- return jobDesc;  
-     }  
- publicvoid setJobDesc(String jobDesc) {  
- this.jobDesc = jobDesc;  
-     }  
- }  
定时器JOB管理类
[java][view
 plain](https://blog.csdn.net/a3781386/article/details/51470013#)[copy](https://blog.csdn.net/a3781386/article/details/51470013#)
- publicclass JobManage {  
- privatestaticfinal Logger logger = LoggerFactory.getActionLog(JobManage.class);  
- private TimedTaskService timedTaskService;  
- 
- public TimedTaskService getTimedTaskService() {  
- return timedTaskService;  
-     }  
- publicvoid setTimedTaskService(TimedTaskService timedTaskService) {  
- this.timedTaskService = timedTaskService;  
-     }  
- privatestatic SchedulerFactoryBean schedulerFactoryBean;  
- privatestatic Scheduler scheduler;  
- static{  
-         schedulerFactoryBean = (SchedulerFactoryBean) BeanUtil.getBeanByClass(SchedulerFactoryBean.class);  
-         scheduler = schedulerFactoryBean.getScheduler();  
-     }  
- /**
-      * 初始化定时任务
-      * @throws SchedulerException
-      */
- publicvoid initTask() throws SchedulerException {  
- //Scheduler scheduler = schedulerFactoryBean.getScheduler();
- // 这里获取任务信息数据
-         List<ScheduleJob> jobList = timedTaskService.getAllJob();//从数据库中读取
- for (ScheduleJob job : jobList) {  
-             addJob(job);  
-         }  
-     }  
- /**
-      * 添加一个任务
-      * @param job
-      */
- publicvoid addJob(ScheduleJob job){  
- try {  
-             TriggerKey triggerKey = TriggerKey.triggerKey(job.getJobName(), job.getJobGroup());  
- // 获取trigger，即在spring配置文件中定义的 bean id="myTrigger"
-             CronTrigger trigger = (CronTrigger) scheduler.getTrigger(triggerKey);  
- // 不存在，创建一个
- if (null == trigger) {  
-                 JobDetail jobDetail = JobBuilder.newJob(QuartzJobFactory.class)  
-                         .withIdentity(job.getJobName(), job.getJobGroup()).build();  
-                 jobDetail.getJobDataMap().put("scheduleJob", job);  
- // 表达式调度构建器
-                 CronScheduleBuilder scheduleBuilder = CronScheduleBuilder.cronSchedule(job.getCronExpression());  
- // 按新的cronExpression表达式构建一个新的trigger
-                 trigger = TriggerBuilder.newTrigger().withIdentity(job.getJobName(), job.getJobGroup())  
-                         .withSchedule(scheduleBuilder).build();  
-                 scheduler.scheduleJob(jobDetail, trigger);  
-             } else {  
- // Trigger已存在，那么更新相应的定时设置
- // 表达式调度构建器
-                 CronScheduleBuilder scheduleBuilder = CronScheduleBuilder.cronSchedule(job.getCronExpression());  
- // 按新的cronExpression表达式重新构建trigger
-                 trigger = trigger.getTriggerBuilder().withIdentity(triggerKey).withSchedule(scheduleBuilder).build();  
- // 按新的trigger重新设置job执行
-                 scheduler.rescheduleJob(triggerKey, trigger);  
-             }  
- if(job.getJobStatus()==0){  
-                 pauseJob(job);  
-             }  
- 
-         } catch (SchedulerException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-     }  
- /**
-      * 暂停一个job
-      * @param scheduleJob
-      * @throws SchedulerException
-      */
- publicvoid pauseJob(ScheduleJob scheduleJob){  
- //Scheduler scheduler = schedulerFactoryBean.getScheduler();
-         logger.info(scheduleJob.getJobName(),"定时任务暂停");  
-         JobKey jobKey = JobKey.jobKey(scheduleJob.getJobName(), scheduleJob.getJobGroup());  
- try {  
-             scheduler.pauseJob(jobKey);  
-         } catch (SchedulerException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-     }  
- /**
-      * 恢复一个job
-      * @param scheduleJob
-      */
- publicvoid resumeJob(ScheduleJob scheduleJob){  
-         logger.info(scheduleJob.getJobName(),"定时任务启动");  
- //Scheduler scheduler = schedulerFactoryBean.getScheduler();
-         JobKey jobKey = JobKey.jobKey(scheduleJob.getJobName(),scheduleJob.getJobGroup());  
- try {  
-             scheduler.resumeJob(jobKey);  
-         } catch (SchedulerException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-     }  
- /**
-      * 删除一个任务
-      * @param scheduleJob
-      */
- publicvoid removeJob(ScheduleJob job){  
-         logger.info(job.getJobName(),"删除定时任务");  
-         TriggerKey triggerKey = TriggerKey.triggerKey(job.getJobName(), job.getJobGroup());  
-         JobKey jobKey = JobKey.jobKey(job.getJobName(),job.getJobGroup());  
- // 获取trigger，即在spring配置文件中定义的 bean id="myTrigger"
- //CronTrigger trigger = (CronTrigger) scheduler.getTrigger(triggerKey);
- try {  
-             scheduler.pauseTrigger(triggerKey);  
-             scheduler.unscheduleJob(triggerKey);  
-             scheduler.deleteJob(jobKey);  
-         } catch (SchedulerException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-     }  
- }  
job工厂类
[java][view
 plain](https://blog.csdn.net/a3781386/article/details/51470013#)[copy](https://blog.csdn.net/a3781386/article/details/51470013#)
- @DisallowConcurrentExecution//只有上一个人任务执行完毕才可以执行下一次任务
- publicclass QuartzJobFactory implements Job{  
- privatestaticfinal Logger logger = LoggerFactory.getActionLog(QuartzJobFactory.class);  
- @Override
- publicvoid execute(JobExecutionContext context){  
- // TODO Auto-generated method stub
- try{  
-             ScheduleJob scheduleJob = (ScheduleJob)context.getMergedJobDataMap().get("scheduleJob");  
-             logger.info(scheduleJob.getJobName(),"任务开始");  
-             ((TaskService) BeanUtil.getBean(scheduleJob.getJobName())).exe();  
-             logger.info(scheduleJob.getJobName(),"任务结束");  
-         }catch(Exception e){  
-             e.printStackTrace();  
-         }  
- //logger.info("任务结束", scheduleJob.getJobName());
-     }  
- 
- }  
具体业务接口
[java][view
 plain](https://blog.csdn.net/a3781386/article/details/51470013#)[copy](https://blog.csdn.net/a3781386/article/details/51470013#)
- publicinterface TaskService {  
- /**
-      * 具体业务逻辑
-      */
- publicabstractvoid exe();  
- }  
实现上面的接口,在spring配置bean
[html][view
 plain](https://blog.csdn.net/a3781386/article/details/51470013#)[copy](https://blog.csdn.net/a3781386/article/details/51470013#)
- <beanid="test"class="com.cmos.ccscore.service.timedTask.impl.Test"parent="baseService">
- </bean>
在数据库中插入记录.
spring获取bean工具类
[java][view
 plain](https://blog.csdn.net/a3781386/article/details/51470013#)[copy](https://blog.csdn.net/a3781386/article/details/51470013#)
- publicclass BeanUtil implements ApplicationContextAware{  
- privatestatic ApplicationContext context;  
- 
- @Override
- publicvoid setApplicationContext(ApplicationContext arg0) throws BeansException {  
- // TODO Auto-generated method stub
-         context = arg0;  
-     }  
- publicstatic Object getBean(String beanName){  
- return context.getBean(beanName);  
-     }  
- publicstatic <T> Object getBeanByClass(Class<T> c){  
- return context.getBean(c);  
-     }  
- }  
监听器用于服务启动初始化定时任务
[java][view
 plain](https://blog.csdn.net/a3781386/article/details/51470013#)[copy](https://blog.csdn.net/a3781386/article/details/51470013#)
- publicclass TaskListener implements ServletContextListener{  
- privatestaticfinal Logger logger = LoggerFactory.getActionLog(TaskListener.class);  
- @Override
- publicvoid contextDestroyed(ServletContextEvent arg0) {  
- // TODO Auto-generated method stub
-     }  
- 
- @Override
- publicvoid contextInitialized(ServletContextEvent arg0) {  
- 
-         logger.info("TaskListener", "定时任务初始化");  
-         JobManage m = (JobManage) BeanUtil.getBean("jobManage");  
- try {  
-             m.initTask();  
-         } catch (SchedulerException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-     }  
- 
- }  
[java][view
 plain](https://blog.csdn.net/a3781386/article/details/51470013#)[copy](https://blog.csdn.net/a3781386/article/details/51470013#)
- TimedTaskService 定时任务服务类就是获取前端请求对数据库的操作,需注入JobManage类来对定时任务进行管理  
最终页面效果
![](https://img-blog.csdn.net/20160521133852370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
