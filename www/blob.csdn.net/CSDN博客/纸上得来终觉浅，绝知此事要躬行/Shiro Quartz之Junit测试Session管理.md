# Shiro Quartz之Junit测试Session管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年07月24日 15:30:53[boonya](https://me.csdn.net/boonya)阅读数：5806








Shiro的quartz主要API上提供了org.apache.shiro.session.mgt.quartz下session管理的两个类：QuartzSessionValidationJob和QuartzSessionValidationScheduler.

下面我们来看看shiro quartz使用Junit是怎样通过测试的：



```java
package org.apache.shiro.session.mgt.quartz;

import java.util.Date;
import org.apache.shiro.util.LifecycleUtils;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.quartz.JobDetail;
import org.quartz.Scheduler;
import org.quartz.SchedulerException;
import org.quartz.Trigger;
import org.quartz.TriggerUtils;

public class QuartzSessionValidationTest
{
	
	private Scheduler scheduler;

	@Before
	public void setUp()
	{
		try
		{
			scheduler =new QuartzSessionValidationScheduler().getScheduler();
		} catch (SchedulerException e)
		{
			e.printStackTrace();
		}
	}

	@After
	public void tearDown()
	{
		LifecycleUtils.destroy(scheduler);
	}
	
	@Test
	public void testJob(){
		
		JobDetail job=new JobDetail("sessionJob", "Jobs", QuartzSessionValidationJob.class);
		try
		{
			scheduler.start();
			
			Trigger trigger=TriggerUtils.makeSecondlyTrigger(60);
			trigger.setStartTime(new Date());
			trigger.setName("trigger");
			scheduler.scheduleJob(job, trigger);
			
		} catch (SchedulerException e)
		{
			e.printStackTrace();
		}
	}

}
```





