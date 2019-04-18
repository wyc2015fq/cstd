# Spring定时器的使用-多实例下定时重建索引 - z69183787的专栏 - CSDN博客
2012年11月29日 13:25:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：903
前几天接到项目需要定时重建索引的任务，一开始试了试Java自带的Timer，不知道是不是自己对Timer的了解还不够的原因，感觉Timer的功能有点弱，无法达到我的需求：需要在某一个固定时间内按某一个周期来执行任务，并且这些时间可供客户配置。最后还是用Spring定时器来解决。
这里顺便说下多实例定时重建lucene索引的问题，如果某个项目在同一台服务器上面部署了多个实例，多个实例共用同一份索引文件，那么如果想要实现定时重建索引的功能，就务必要阻止在定时时间到的时候几个实例同时触发并重建索引，很明显的问题：一来同时有多个进程对同一份索引文件进行操作，会引起冲突；二来是根本就没必要，且耗资源。所以这里我的解决方法是每次定时时间到了之后，在执行任务之前随机获取一个时间，如1-100秒中间的任意一个，等过了这个随机时间之后再进行重建索引操作，这样可以避免多个实例同一时刻触发任务，然后第一个做定时任务的会在服务器磁盘上建立一个标识文件，表示当前有实例在执行任务，其他实例过了随机时间后发现有该标识文件存在就不做任务了，当然该文件必须在执行完任务或者任务过程中有异常而导致操作停止时要删除掉！
第一步：编写执行任务的类，继承QuartzJobBean，我的任务内容在此就不写了，如下：
**[java]**[view
 plain](http://blog.csdn.net/linshutao/article/details/7672176#)[copy](http://blog.csdn.net/linshutao/article/details/7672176#)
- publicclass RebuildLuceneIndex extends QuartzJobBean{  
-     Log log = LogFactory.getLog(RebuildLuceneIndex.class);  
- private KbsEntryManager kbsEntryManager;  
- int timeout;  
- @Override
- protectedvoid executeInternal(JobExecutionContext jec)  
- throws JobExecutionException {  
- //获取配置信息，如果客户没配置的话则不启动定时器，应把它关掉：
- if(没配置定时器){  
- try {  
-              jec.getScheduler().shutdown();  
-         } catch (SchedulerException e) {  
-             log.error(e.getMessage(),e);  
-         }  
-           }else{  
- //执行你的任务
- }      
-      }  
- }  
哦对，应为注入了一个类，需提供set方法，上面的代码增加：
**[java]**[view
 plain](http://blog.csdn.net/linshutao/article/details/7672176#)[copy](http://blog.csdn.net/linshutao/article/details/7672176#)
- publicvoid setKbsEntryManager(KbsEntryManager manager) {  
- this.kbsEntryManager = manager;  
- }     
第二步：编写定时器配置文件，比如我的schedule-context.xml:
**[html]**[view
 plain](http://blog.csdn.net/linshutao/article/details/7672176#)[copy](http://blog.csdn.net/linshutao/article/details/7672176#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN" "http://www.springframework.org/dtd/spring-beans.dtd">
- 
- <beans>
- <beanname="schedulerJobDetail"class="org.springframework.scheduling.quartz.JobDetailBean">
- <propertyname="jobClass">
- <value>com.syni.im800.kb.schedule.RebuildLuceneIndex</value>
- </property>
- <propertyname="jobDataAsMap">
- <map>
- <entrykey="kbsEntryManager"value-ref="kbsEntryManager"/>
- </map>
- </property>
- </bean>
- 
- <beanid="rebuildIndexTrigger"class="com.syni.im800.kb.schedule.InitializingCronTrigger">
- <propertyname="jobDetail">
- <refbean="schedulerJobDetail"/>
- </property>
- <propertyname="cronExpression">
- <value>0 0 2 * * ?</value>
- </property>
- </bean>
- 
- <beanid="schedulerFactory"class="org.springframework.scheduling.quartz.SchedulerFactoryBean">
- <propertyname="triggers">
- <list>
- <reflocal="cronTrigger"/>
- <reflocal="rebuildIndexTrigger"/>
- </list>
- </property>
- </bean>
- 
- </beans>
上面的配置说明一下，因为cronExpression需提供可配置功能，不能写死在xml里面，所以自己写了一个InitializingCronTrigger，继承CronTriggerBean，在里面重新配置cronExpression，所以这里配置的0 0 2 * * ?其实只是为了不让容器在启动的时候报错而已。
第三步：编写第二步中的rebuildIndexTrigger：
**[java]**[view
 plain](http://blog.csdn.net/linshutao/article/details/7672176#)[copy](http://blog.csdn.net/linshutao/article/details/7672176#)
- publicclass InitializingCronTrigger extends CronTriggerBean implements Serializable{  
-     Log log  = LogFactory.getLog(InitializingCronTrigger.class);  
-     String cronExpression;  
- 
- public InitializingCronTrigger(){};  
- 
- publicvoid setCronExpression(String cronExpression_) {  
-         String day = AppConfig.getProperty(AppConfig.REBUILDINDEX_DAY_OF_WEEK);  
-         String hour = AppConfig.getProperty(AppConfig.REBUILDINDEX_HOUR_OF_DAY);  
- if(day!=null && hour!=null){  
-             cronExpression = "0 0 "+hour+" ? * "+day;  
-             log.debug("#######cronExpression:"+cronExpression);  
-         }else{  
- //这里随便设置一个值，到时间的时候会把定时器关闭
-             cronExpression = "0 0 1 ? * *";  
-         }  
- 
- try {  
- super.setCronExpression(cronExpression);      
-         } catch (Exception e) {  
-             log.error(e.getMessage(),e);  
-         }  
-     }  
- }  
说明：因为chedule-context.xml中为InitializingCronTrigger注入了cronExpression，所以这里提供set方法，这样每次定时器触发的时候会调用这个方法，然后我们就可以在里面读取我们配置文件里实际配置的值，从而构建我们的cronExpression了，上面的day，hour是我的程序从配置文件中读取用户配置的信息而已。
最后是在web.xml中配置chedule-context.xm，这一步应该不用多少了吧，如：
**[html]**[view
 plain](http://blog.csdn.net/linshutao/article/details/7672176#)[copy](http://blog.csdn.net/linshutao/article/details/7672176#)
- <!-- Context Configuration locations for Spring XML files -->
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>
-         /WEB-INF/schedule-context.xml  
- </param-value>
- </context-param>
另外，关于Spring定时器的详细使用，如配置表达式等，可参考其他文章，下面是简单的介绍表达式的配置：
![](https://img-my.csdn.net/uploads/201206/18/1339991154_8108.jpg)
![](https://img-my.csdn.net/uploads/201206/18/1339991170_7236.jpg)
