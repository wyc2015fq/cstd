# Spring事务内多线程处理-解决方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月29日 17:12:17[boonya](https://me.csdn.net/boonya)阅读数：18310








首先，Spring Ioc 依赖注入的就是单例对象，我们在使用的时候如果有多个相同处理且不怎么耗时的情况下一般会采用for循环直接执行，这样的情况下即使有事务管理也不会影响业务执行下去；但是，大多数的应用比如：系统与系统之间的交互就比较耗时了，此时使用for循环执行业务就不可取了，于是我们想用线程来解决这个问题。这里面踩过的坑只有自己爬出来才知道里面个中缘由。

======================**本篇多线程处理是依靠数据源配置解决的**==================

另外，附带说明一下：如果是在Spring提供的@Test下测试：如果有多线程处理，需要调用**Thread.sleep(3000);**实现线程等待，这也是测试多线程模式下所必须的。如果不加线程休眠，不等待多线程执行完毕，很有可能测不出来多线程调用的处理。


**前提：调用业务方法中实现多线程执行多个相同操作。**

**问题：如果是在Spring 事务管理下的方法内采用多线程处理任务是否可行呢？**

一般情况下，我们只需要在调用的时候@autowire 我们需要的service类即可。理想状态下，以下代码是最常见的实现：

### 1.创建一个业务线程类

```java
package com.wlyd.fmcgwms.util.thread;

import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;
import com.wlyd.fmcgwms.persistence.beans.platform.Corporation;
import com.wlyd.fmcgwms.service.basic.WmStockService;

public class UpdateProductThead extends Thread {
    
     private ThreadPoolTaskExecutor taskExecutor;
     
     public ThreadPoolTaskExecutor getTaskExecutor() {
         return taskExecutor;
     }
     
     public void setTaskExecutor(ThreadPoolTaskExecutor taskExecutor) {
         this.taskExecutor = taskExecutor;
     }
    
    ExecutorService executorService; 
    
    private WmStockService wmStockService;
    
    private List<Corporation> corporations;
    
    public UpdateProductThead( WmStockService wmStockService,List<Corporation> corporations) {
        this.wmStockService = wmStockService;
        this.corporations = corporations;
        executorService = Executors.newFixedThreadPool(corporations==null?0:corporations.size()); 
    }

    /*@Override
    public void run() {
        for (int i = 0; i < corporations.size(); i++) {
            new Thread(new DoBusinessThread(i));
        }
    }*/
    
    @Override
    public void run() {
        taskExecutor=new ThreadPoolTaskExecutor();
        for (int i = 0; i < corporations.size(); i++) {
            taskExecutor.execute(new DoBusinessThread(i));
        }
    }
    
    /*@Override
    public void run() {
        for (int i = 0; i < corporations.size(); i++) {
            executorService.execute(new DoBusinessThread(i));
            System.out.println();
        }
    }*/

    class DoBusinessThread extends Thread{
        
        private int index=0;
        
        public DoBusinessThread(int index){
            this.index=index;
        }

        @Override
        public void run() {
            wmStockService.updateProductStockProcess(corporations.get(index)); 
        }
        
    }

}
```

以上三种方式的线程run执行效果都是一样的。


### 2.测试业务线程类执行情况

```java
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
public class APIServiceTest {

       @Autowired
    WmStockService wmStockService;
    
    @Autowired
    CorporationMapper corporationMapper;
        
	@Test
	public void testUpdateProductWithThread(){
		List<Corporation> corporations=corporationMapper.findAll();
		
		new UpdateProductThead(wmStockService, corporations);
	}
}
```
我们最愿意看到的情况是程序执行下去就成功了，但是这却是事与愿违的。按照上面的写法没有一个业务线程会执行。
当我们的线程内run方法只执行如下方法时：

```java
wmStockService.updateProductStockProcess(corporation);
```
我们能执行成功有且仅有一个线程成功，然后数据源就关闭了。
```java
package com.wlyd.fmcgwms.util.thread;

import com.wlyd.fmcgwms.persistence.beans.platform.Corporation;
import com.wlyd.fmcgwms.service.basic.WmStockService;

public class UpdateProductThead extends Thread {
	
	private WmStockService wmStockService;
	
	private Corporation corporations;
	
	public UpdateProductThead( WmStockService wmStockService,Corporation corporation) {
		this.wmStockService = wmStockService;
		this.corporation = corporation; 
	}

	@Override
	public void run() {
           wmStockService.updateProductStockProcess(corporation);
       }
}
```

### 3.最原始的For循环

外部业务调用：

```java
public void updateProductStock() {
		try {
			List<Corporation> corps = corporationMapper.findAll();
			if (corps == null || corps.size() == 0) {
				return ;
			}
			for (Corporation corporation : corps) {
				// 排除系统组织账号
				if (corporation.getEsCorAlias() == null
						|| corporation.getEsCorAlias().equals("")
						|| corporation.getEsCorAlias().equals("10000")
						|| corporation.getEsCorAlias().equals("10002")) {
					continue;
				}
				wmStockService.updateProductStockProcess(corporation);
				// 开启线程处理商品库存同步---Spring管理的对象都是单例模式的，所以for批量处理的方式是不行的
				//new UpdateProductThead(wmStockService, corporation).start();
			}
		} catch (Exception e) {
			Log.getLogger(getClass()).error(">>>>调用九州通达OMS库存同步接口调用异常:"+e.getMessage());
		}

}
```
业务处理方法（详细的过程就不贴上来了）：
```java
public void updateProductStockProcess(Corporation corporation) {

      // 远程接口访问

      // 本地保存接口调用日志
}
```
测试：
```java
@Test
public void testUpdateProduct(){
	quartzJobService.updateProductStock();
}
```

这次却可以正常的运行：

```java
2016-07-29 16:53:46,553 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@2e8ab815]
2016-07-29 16:53:46,553 [main] DEBUG [java.sql.Connection] - ==>  Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit FROM wm_stock_10003 st LEFT JOIN cd_wh_itme_10003 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-07-29 16:53:46,553 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-07-29 16:53:46,617 [main] INFO  [com.wlyd.fmcgwms.service.platform.impl.JZTDAPIServiceImpl] - 调用九州通达OMS接口.........../updateproductstock REQUEST:{"Products":[{"AvailableProductNum":20,"ProductCode":"10003160503009","ProductNum":40,"ProductUnit":"斤"},{"AvailableProductNum":100,"ProductCode":"10003160503009","ProductNum":100,"ProductUnit":"条"},{"AvailableProductNum":13,"ProductCode":"10003160503001","ProductNum":14,"ProductUnit":"只"},{"AvailableProductNum":7,"ProductCode":"10003160503002","ProductNum":10,"ProductUnit":"只"},{"AvailableProductNum":50,"ProductCode":"10003160503002","ProductNum":100,"ProductUnit":"斤"},{"AvailableProductNum":40,"ProductCode":"10003160503005","ProductNum":100,"ProductUnit":"斤"},{"AvailableProductNum":0,"ProductCode":"10003160503005","ProductNum":5,"ProductUnit":"箱"},{"AvailableProductNum":0,"ProductCode":"10003160503007","ProductNum":30,"ProductUnit":"斤"},{"AvailableProductNum":20,"ProductCode":"10003160503007","ProductNum":25,"ProductUnit":"箱"},{"AvailableProductNum":5,"ProductCode":"10003160503004","ProductNum":55,"ProductUnit":"箱"},{"AvailableProductNum":50,"ProductCode":"10003160503006","ProductNum":149,"ProductUnit":"斤"},{"AvailableProductNum":0,"ProductCode":"10003160503006","ProductNum":6,"ProductUnit":"箱"},{"AvailableProductNum":0,"ProductCode":"10003160503003","ProductNum":100,"ProductUnit":"斤"},{"AvailableProductNum":7,"ProductCode":"10003160503008","ProductNum":7,"ProductUnit":"斤"},{"AvailableProductNum":9,"ProductCode":"10003160503008","ProductNum":9,"ProductUnit":"箱"}]}
2016-07-29 16:53:46,618 [main] INFO  [com.wlyd.fmcgwms.service.platform.impl.JZTDAPIServiceImpl] - 调用九州通达OMS接口.........../updateproductstock NO RESPONSE:Illegal character(s) in message header value: *
2016-07-29 16:53:46,622 [main] ERROR [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>调用九州通达OMS库存同步接口日志保存:true
2016-07-29 16:53:46,624 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@2e8ab815]
2016-07-29 16:53:46,624 [main] DEBUG [java.sql.Connection] - ==>  Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit FROM wm_stock_10004 st LEFT JOIN cd_wh_itme_10004 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-07-29 16:53:46,624 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-07-29 16:53:46,629 [main] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10004)测试新增组织没有库存记录！！！
2016-07-29 16:53:46,630 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@2e8ab815]
2016-07-29 16:53:46,630 [main] DEBUG [java.sql.Connection] - ==>  Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit FROM wm_stock_10005 st LEFT JOIN cd_wh_itme_10005 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-07-29 16:53:46,630 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-07-29 16:53:46,633 [main] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10005)新增组织测试预警信息菜单没有库存记录！！！
2016-07-29 16:53:46,633 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@2e8ab815]
2016-07-29 16:53:46,633 [main] DEBUG [java.sql.Connection] - ==>  Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit FROM wm_stock_10006 st LEFT JOIN cd_wh_itme_10006 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-07-29 16:53:46,633 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-07-29 16:53:46,636 [main] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10006)test1没有库存记录！！！
2016-07-29 16:53:46,637 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@2e8ab815]
2016-07-29 16:53:46,637 [main] DEBUG [java.sql.Connection] - ==>  Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit FROM wm_stock_10007 st LEFT JOIN cd_wh_itme_10007 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-07-29 16:53:46,637 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-07-29 16:53:46,640 [main] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10007)aaabbbbbb没有库存记录！！！
2016-07-29 16:53:46,640 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@2e8ab815]
2016-07-29 16:53:46,640 [main] DEBUG [java.sql.Connection] - ==>  Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit FROM wm_stock_10008 st LEFT JOIN cd_wh_itme_10008 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-07-29 16:53:46,641 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-07-29 16:53:46,643 [main] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10008)测试新增专用组织没有库存记录！！！
```
按序列执行下去就可以，但这不是我们需要的方式，我们需要线程来解决这种数据量大且耗时的操作。

**注：这里通过调度任务来开启多线程处理，不是Action/Controller发起的请求。**

### **4.配置Spring管理线程池**



```
<!-- 配置线程池 -->  
    <bean id ="taskExecutor"  class ="org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor" >  
        <!-- 线程池维护线程的最少数量 -->  
   		<property name ="corePoolSize" value ="5" />  
        <!-- 线程池维护线程所允许的空闲时间 -->  
   		<property name ="keepAliveSeconds" value ="30000" />  
        <!-- 线程池维护线程的最大数量 -->  
    	<property name ="maxPoolSize" value ="1000" />  
        <!-- 线程池所使用的缓冲队列 -->  
    	<property name ="queueCapacity" value ="200" />  
    </bean>
```


修改业务线程：





```java
package com.wlyd.fmcgwms.util.thread;

import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;
import com.wlyd.fmcgwms.persistence.beans.platform.Corporation;
import com.wlyd.fmcgwms.service.basic.WmStockService;
/**
 * 多线程业务处理类
 * 
 * @packge com.wlyd.fmcgwms.util.thread.UpdateProductThead
 * @date   2016年7月29日  下午5:56:22
 * @author pengjunlin
 * @comment   
 * @update
 */
public class UpdateProductThead extends Thread {
	
	public static final int THREAD_COMMON=0;
	
	public static final int THREAD_EXCUTOR_SERVICE=1;
	
	public static final int THREAD_POOL_TASK_EXCUTOER=2;
	
	private ThreadPoolTaskExecutor taskExecutor;
	
	private ExecutorService executorService; 
	
	private WmStockService wmStockService;
	
	private List<Corporation> corporations;
	
	private int threadType;
	
	public UpdateProductThead(int threadType,ThreadPoolTaskExecutor taskExecutor,WmStockService wmStockService,List<Corporation> corporations) {
		this.wmStockService = wmStockService;
		this.corporations = corporations;
		this.executorService = Executors.newFixedThreadPool(corporations==null?0:corporations.size()); 
		this.threadType=threadType;
	}

	@Override
	public void run() {
		for (int i = 0; i < corporations.size(); i++) {
			switch (threadType) {
			case 0:
				// 方式一
				new Thread(new DoBusinessThread(i));
				break;
			case 1:
				// 方式二
				taskExecutor.execute(new DoBusinessThread(i));
				break;
			case 2:
				// 方式三
				executorService.execute(new DoBusinessThread(i));
				break;
			default:
				break;
			}
			
		}
	}
	

	class DoBusinessThread extends Thread{
		
		private int index=0;
		
		public DoBusinessThread(int index){
			this.index=index;
		}

		@Override
		public void run() {
			wmStockService.updateProductStockProcess(corporations.get(index)); 
		}
		
	}

}
```
修改测试：





```java
@Test
public void testUpdateProductWithThread(){
	List<Corporation> corporations=corporationMapper.findAll();
		
	new UpdateProductThead(UpdateProductThead.THREAD_POOL_TASK_EXCUTOER,taskExecutor,wmStockService, corporations);
}
```

运行测试 还是没有执行，spring输出日志：



```java
2016-08-01 09:28:07,422 [Thread-4] INFO  [org.springframework.context.support.GenericApplicationContext] - Closing org.springframework.context.support.GenericApplicationContext@7ac7a4e4: startup date [Mon Aug 01 09:28:03 CST 2016]; root of context hierarchy
2016-08-01 09:28:07,426 [Thread-4] INFO  [org.springframework.context.support.DefaultLifecycleProcessor] - Stopping beans in phase 2147483647
2016-08-01 09:28:07,427 [Thread-4] INFO  [org.quartz.core.QuartzScheduler] - Scheduler SpringJobSchedulerFactoryBean_$_NON_CLUSTERED paused.
2016-08-01 09:28:07,427 [Thread-4] INFO  [org.springframework.beans.factory.support.DefaultListableBeanFactory] - Destroying singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@4e08711f: defining beans [wmStockAlertRlueServiceImpl,wmWarringInfoServiceImpl,cdWarehouseServiceImpl,cdWhCustomerServiceImpl,cdWhItmeServiceImpl,cdWhLocAreaServiceImpl,cdWhLocationServiceImpl,wmStockAdjustServiceImpl,wmStockFreezeServiceImpl,wmStockReleaseServiceImpl,wmStockServiceImpl,esMenuFunctionsServiceImpl,esMrRightsServiceImpl,esMuRightsServiceImpl,WMWareHouseStatServiceImpl,esDictionaryServiceImpl,esDictionaryTypeServiceImpl,esExceptionServiceImpl,esIconsServiceImpl,esMenuServiceImpl,esPropertiesServiceImpl,esRoleServiceImpl,esUserServiceImpl,wmIntoOrderBatchCodenoServiceImpl,wmIntoOrderinfoServiceImpl,wmIntoOrderServiceImpl,wmReceiptServiceImpl,wmReturnInfoServiceImpl,wmInventorySheetDetailServiceImpl,wmInventorySheetServiceImpl,wmWarehouseMoveServiceImpl,ebInterfaceLogServiceImpl,ebIntoStockLogServiceImpl,ebOperateLogServiceImpl,ebOutStockLogServiceImpl,ebSysLogServiceImpl,wmBatchPickingServiceImpl,wmOutOrderDetailServiceImpl,wmOutOrderServiceImpl,APIForOMSServiceImpl,APIServiceImpl,corporationServiceImpl,esSaasServiceInfoServiceImpl,JZTDAPIServiceImpl,padServiceImpl,pdaApiImpl,quartzJobServiceImpl,statisticalChartsServiceImpl,stockWarringJobServiceImpl,syncOrderStutusServiceImpl,timerTaskServiceImpl,reportServiceImpl,wmItemDailyReportServiceImpl,cdWhUnpickServiceImpl,settingsServiceImpl,org.springframework.context.annotation.internalConfigurationAnnotationProcessor,org.springframework.context.annotation.internalAutowiredAnnotationProcessor,org.springframework.context.annotation.internalRequiredAnnotationProcessor,org.springframework.context.annotation.internalCommonAnnotationProcessor,org.springframework.context.annotation.internalPersistenceAnnotationProcessor,org.springframework.beans.factory.config.PropertyPlaceholderConfigurer#0,dataSource,sqlSessionFactory,sqlSession,org.mybatis.spring.mapper.MapperScannerConfigurer#0,transactionManager,txAdvice,org.springframework.aop.config.internalAutoProxyCreator,pc,org.springframework.aop.support.DefaultBeanFactoryPointcutAdvisor#0,InitSysProperties,DailyTaskJob,DailyTaskJobMethod,DailyTaskCronTriggerBean,DailyTaskJobMethodSaas,DailyTaskCronTriggerBeanSaas,StockWarringTaskJob,StockWarringTaskJobMethod,StockWarringTaskCronTriggerBean,CycleSendInterfaceInfoTaskJob,CycleSendInterfaceInfoTaskJobMethod,CycleSendInterfaceInfoTaskCronTriggerBean,SpringJobSchedulerFactoryBean,taskExecutor,org.springframework.context.annotation.ConfigurationClassPostProcessor.importAwareProcessor,wmStockAlertRuleMapper,wmWarringInfoMapper,cdCustomerMapper,cdWarehouseMapper,cdWhItmeMapper,cdWhLocAreaMapper,cdWhLocationMapper,wmStockAdjustMapper,wmStockFreezeMapper,wmStockMapper,wmStockReleaseMapper,esMenuFunctionsMapper,esMrRightsMapper,esMuRightsMapper,esDictionaryMapper,esDictionaryTypeMapper,ebUserWarehouseMapper,esExceptionMapper,esIconsMapper,esMenuMapper,esPropertiesMapper,esRoleMapper,esUserMapper,wmIntoOrderBatchCodenoMapper,wmIntoOrderCodenoMapper,wmIntoOrderInfoMapper,wmIntoOrderMapper,wmReceiptInfoMapper,wmReturnInfoMapper,wmInventorySheetDetailMapper,wmInventorySheetMapper,wmWarehouseMoveCodenoMapper,wmWarehouseMoveMapper,ebInterfaceLogMapper,ebIntoStockLogMapper,ebOperateLogMapper,ebOutStockLogMapper,ebSysLogMapper,wmAssignedPickcarMapper,wmBatchPickingMapper,wmOutOrderCodenoMapper,wmOutOrderDetailMapper,wmOutOrderMapper,wmStockAssignedMapper,corporationMapper,esSaasServiceInfoMapper,statisticalChartsMapper,wmItemTransferMapper,reportMapper,wmDailyItemReportMapper,settingsMapper,wmUnpickMapper]; root of factory hierarchy
2016-08-01 09:28:07,451 [Thread-4] INFO  [org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor] 
            - Shutting down ExecutorService 'taskExecutor'
2016-08-01 09:28:07,452 [Thread-4] INFO  [org.springframework.scheduling.quartz.SchedulerFactoryBean] - Shutting down Quartz Scheduler
2016-08-01 09:28:07,452 [Thread-4] INFO  [org.quartz.core.QuartzScheduler] - Scheduler SpringJobSchedulerFactoryBean_$_NON_CLUSTERED shutting down.
2016-08-01 09:28:07,452 [Thread-4] INFO  [org.quartz.core.QuartzScheduler] - Scheduler SpringJobSchedulerFactoryBean_$_NON_CLUSTERED paused.
2016-08-01 09:28:07,459 [Thread-4] INFO  [org.quartz.core.QuartzScheduler] - Scheduler SpringJobSchedulerFactoryBean_$_NON_CLUSTERED shutdown complete.
2016-08-01 09:28:07,482 [Thread-4] INFO  [com.alibaba.druid.pool.DruidDataSource] - {dataSource-1} closed
```

从日志看，应该是spring不支持多线程处理，该线程一启动就被关闭了。**错了，并不是spring不支持多线程，那是因为我忘了写线程的调用start()，于是才写了下面的第5点;**



```java
new UpdateProductThead(UpdateProductThead.THREAD_POOL_TASK_EXCUTOER,taskExecutor,wmStockService, corporations).start();
```
重新运行，控制台输出：



```java
2016-08-01 13:52:46,518 [Thread-4] INFO  [com.alibaba.druid.pool.DruidDataSource] - {dataSource-1} closed
Exception in thread "pool-1-thread-8" Exception in thread "pool-1-thread-2" org.springframework.transaction.CannotCreateTransactionException: 
Could not open JDBC Connection for transaction; nested exception is com.alibaba.druid.pool.DataSourceDisableException
	at org.springframework.jdbc.datasource.DataSourceTransactionManager.doBegin(DataSourceTransactionManager.java:241)
	at org.springframework.transaction.support.AbstractPlatformTransactionManager.getTransaction(AbstractPlatformTransactionManager.java:372)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.createTransactionIfNecessary(TransactionAspectSupport.java:417)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.invokeWithinTransaction(TransactionAspectSupport.java:255)
	at org.springframework.transaction.interceptor.TransactionInterceptor.invoke(TransactionInterceptor.java:94)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.interceptor.ExposeInvocationInterceptor.invoke(ExposeInvocationInterceptor.java:91)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:204)
	at com.sun.proxy.$Proxy81.updateProductStockProcess(Unknown Source)
	at com.wlyd.fmcgwms.util.UpdateProductThread$DoBusinessThread.run(UpdateProductThread.java:71)
	at java.util.concurrent.ThreadPoolExecutor.runWorker(Unknown Source)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(Unknown Source)
	at java.lang.Thread.run(Unknown Source)
Caused by: com.alibaba.druid.pool.DataSourceDisableException
	at com.alibaba.druid.pool.DruidDataSource.pollLast(DruidDataSource.java:1399)
	at com.alibaba.druid.pool.DruidDataSource.getConnectionInternal(DruidDataSource.java:1021)
	at com.alibaba.druid.pool.DruidDataSource.getConnectionDirect(DruidDataSource.java:898)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:882)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:872)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:97)
	at org.springframework.jdbc.datasource.DataSourceTransactionManager.doBegin(DataSourceTransactionManager.java:203)
```
数据源提前关闭了，这里初步可以看出是数据库连接池的问题。接下来进一步测试（因为此前没有调用start()所以没看出真实的问题所在）。

### 5.Spring注入线程类处理

再次尝试使用@service扫描实现线程处理：



```java
<pre name="code" class="java">package com.wlyd.fmcgwms.service.thread;

import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import javax.annotation.PostConstruct;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;
import org.springframework.stereotype.Service;
import com.wlyd.fmcgwms.persistence.beans.Settings;
import com.wlyd.fmcgwms.persistence.beans.platform.Corporation;
import com.wlyd.fmcgwms.persistence.mapper.SettingsMapper;
import com.wlyd.fmcgwms.service.basic.WmStockService;
import com.wlyd.fmcgwms.util.base.BaseServiceImpl;
/**
 * 多线程业务处理类
 * 
 * @packge com.wlyd.fmcgwms.util.thread.UpdateProductThead
 * @date   2016年7月29日  下午5:56:22
 * @author pengjunlin
 * @comment   
 * @update
 */
@Service
public class UpdateProductThead  extends BaseServiceImpl<Settings, String> implements Runnable {
	
	public static final int THREAD_COMMON=0;
	
	public static final int THREAD_EXCUTOR_SERVICE=1;
	
	public static final int THREAD_POOL_TASK_EXCUTOER=2;
	
	@Autowired
	private ThreadPoolTaskExecutor taskExecutor;
	
	private ExecutorService executorService; 
	
	private WmStockService wmStockService;
	
	private List<Corporation> corporations;
	
	private int threadType;
	
	@Autowired
	private SettingsMapper settingsMapper;

	@PostConstruct
	public void init() {
		this.dao = settingsMapper;
	}
	
	public UpdateProductThead() {
		super();
	}

	public UpdateProductThead(int threadType,WmStockService wmStockService,List<Corporation> corporations) {
		this.wmStockService=wmStockService;
		this.corporations = corporations;
		this.executorService = Executors.newFixedThreadPool(corporations==null?0:corporations.size()); 
		this.threadType=threadType;
	}

	@Override
	public void run() {
		for (int i = 0; i < corporations.size(); i++) {
			switch (threadType) {
			case 0:
				// 方式一
				new Thread(new DoBusinessThread(wmStockService,i));
				break;
			case 1:
				// 方式二
				taskExecutor.execute(new DoBusinessThread(wmStockService,i));
				//new DoLogicThread(0).start();
				break;
			case 2:
				// 方式三
				executorService.execute(new DoBusinessThread(wmStockService,i));
				break;
			default:
				break;
			}
			
		}
	}
	

	class DoBusinessThread extends Thread{
		
		WmStockService wmStockService;
		
		private int index=0;
		
		public DoBusinessThread(WmStockService wmStockService,int index){
			this.wmStockService=wmStockService;
			this.index=index;
		}

		@Override
		public void run() {
			wmStockService.updateProductStockProcess(corporations.get(index)); 
		}
		
	}
	
	class DoLogicThread extends Thread{
		
		private int index=0;
		
		public DoLogicThread(int index){
			this.index=index;
		}

		@Override
		public void run() {
			System.out.println("Do logic counting:"+index);
		}
		
	}

}
```

运行测试方法：



```java
@Autowired
CorporationMapper corporationMapper;
    
@Autowired
WmStockService wmStockService;

@Test
public void testUpdateProductthread(){
	List<Corporation> corporations=corporationMapper.findAll();
	new Thread(new UpdateProductThead(UpdateProductThead.THREAD_POOL_TASK_EXCUTOER,wmStockService, corporations)).start();
}
```


明确地给出了线程处理是数据源设置的问题，控制台输出如下：





```java
2016-08-01 13:57:11,892 [Thread-4] INFO  [com.alibaba.druid.pool.DruidDataSource] - {dataSource-1} closed
Exception in thread "pool-1-thread-2" org.springframework.transaction.CannotCreateTransactionException: 
Could not open JDBC Connection for transaction; nested exception is com.alibaba.druid.pool.DataSourceDisableException
	at org.springframework.jdbc.datasource.DataSourceTransactionManager.doBegin(DataSourceTransactionManager.java:241)
	at org.springframework.transaction.support.AbstractPlatformTransactionManager.getTransaction(AbstractPlatformTransactionManager.java:372)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.createTransactionIfNecessary(TransactionAspectSupport.java:417)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.invokeWithinTransaction(TransactionAspectSupport.java:255)
	at org.springframework.transaction.interceptor.TransactionInterceptor.invoke(TransactionInterceptor.java:94)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.interceptor.ExposeInvocationInterceptor.invoke(ExposeInvocationInterceptor.java:91)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:204)
	at com.sun.proxy.$Proxy81.updateProductStockProcess(Unknown Source)
	at com.wlyd.fmcgwms.service.thread.UpdateProductThead$DoBusinessThread.run(UpdateProductThead.java:101)
	at java.util.concurrent.ThreadPoolExecutor.runWorker(Unknown Source)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(Unknown Source)
	at java.lang.Thread.run(Unknown Source)
Caused by: com.alibaba.druid.pool.DataSourceDisableException
	at com.alibaba.druid.pool.DruidDataSource.pollLast(DruidDataSource.java:1399)
	at com.alibaba.druid.pool.DruidDataSource.getConnectionInternal(DruidDataSource.java:1021)
	at com.alibaba.druid.pool.DruidDataSource.getConnectionDirect(DruidDataSource.java:898)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:882)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:872)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:97)
	at org.springframework.jdbc.datasource.DataSourceTransactionManager.doBegin(DataSourceTransactionManager.java:203)
	... 13 more
Exception in thread "pool-1-thread-4" org.springframework.transaction.CannotCreateTransactionException:
 Could not open JDBC Connection for transaction; nested exception is com.alibaba.druid.pool.DataSourceDisableException
	at org.springframework.jdbc.datasource.DataSourceTransactionManager.doBegin(DataSourceTransactionManager.java:241)
	at org.springframework.transaction.support.AbstractPlatformTransactionManager.getTransaction(AbstractPlatformTransactionManager.java:372)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.createTransactionIfNecessary(TransactionAspectSupport.java:417)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.invokeWithinTransaction(TransactionAspectSupport.java:255)
	at org.springframework.transaction.interceptor.TransactionInterceptor.invoke(TransactionInterceptor.java:94)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.interceptor.ExposeInvocationInterceptor.invoke(ExposeInvocationInterceptor.java:91)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:204)
	at com.sun.proxy.$Proxy81.updateProductStockProcess(Unknown Source)
	at com.wlyd.fmcgwms.service.thread.UpdateProductThead$DoBusinessThread.run(UpdateProductThead.java:101)
	at java.util.concurrent.ThreadPoolExecutor.runWorker(Unknown Source)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(Unknown Source)
	at java.lang.Thread.run(Unknown Source)
Caused by: com.alibaba.druid.pool.DataSourceDisableException
	at com.alibaba.druid.pool.DruidDataSource.pollLast(DruidDataSource.java:1399)
	at com.alibaba.druid.pool.DruidDataSource.getConnectionInternal(DruidDataSource.java:1021)
	at com.alibaba.druid.pool.DruidDataSource.getConnectionDirect(DruidDataSource.java:898)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:882)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:872)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:97)
	at org.springframework.jdbc.datasource.DataSourceTransactionManager.doBegin(DataSourceTransactionManager.java:203)
	... 13 more
Exception in thread "pool-1-thread-8" org.springframework.transaction.CannotCreateTransactionException:
 Could not open JDBC Connection for transaction; nested exception is com.alibaba.druid.pool.DataSourceDisableException
	at org.springframework.jdbc.datasource.DataSourceTransactionManager.doBegin(DataSourceTransactionManager.java:241)
	at org.springframework.transaction.support.AbstractPlatformTransactionManager.getTransaction(AbstractPlatformTransactionManager.java:372)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.createTransactionIfNecessary(TransactionAspectSupport.java:417)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.invokeWithinTransaction(TransactionAspectSupport.java:255)
	at org.springframework.transaction.interceptor.TransactionInterceptor.invoke(TransactionInterceptor.java:94)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.interceptor.ExposeInvocationInterceptor.invoke(ExposeInvocationInterceptor.java:91)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:204)
	at com.sun.proxy.$Proxy81.updateProductStockProcess(Unknown Source)
	at com.wlyd.fmcgwms.service.thread.UpdateProductThead$DoBusinessThread.run(UpdateProductThead.java:101)
	at java.util.concurrent.ThreadPoolExecutor.runWorker(Unknown Source)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(Unknown Source)
	at java.lang.Thread.run(Unknown Source)
Caused by: com.alibaba.druid.pool.DataSourceDisableException
	at com.alibaba.druid.pool.DruidDataSource.pollLast(DruidDataSource.java:1399)
	at com.alibaba.druid.pool.DruidDataSource.getConnectionInternal(DruidDataSource.java:1021)
	at com.alibaba.druid.pool.DruidDataSource.getConnectionDirect(DruidDataSource.java:898)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:882)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:872)
	at com.alibaba.druid.pool.DruidDataSource.getConnection(DruidDataSource.java:97)
	at org.springframework.jdbc.datasource.DataSourceTransactionManager.doBegin(DataSourceTransactionManager.java:203)
	... 13 more
Exception in thread "pool-1-thread-6" org.springframework.transaction.CannotCreateTransactionException:
 Could not open JDBC Connection for transaction; nested exception is com.alibaba.druid.pool.DataSourceDisableException
	at org.springframework.jdbc.datasource.DataSourceTransactionManager.doBegin(DataSourceTransactionManager.java:241)
	at org.springframework.transaction.support.AbstractPlatformTransactionManager.getTransaction(AbstractPlatformTransactionManager.java:372)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.createTransactionIfNecessary(TransactionAspectSupport.java:417)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.invokeWithinTransaction(TransactionAspectSupport.java:255)
	at org.springframework.transaction.interceptor.TransactionInterceptor.invoke(TransactionInterceptor.java:94)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.interceptor.ExposeInvocationInterceptor.invoke(ExposeInvocationInterceptor.java:91)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:204)
	at com.sun.proxy.$Proxy81.updateProductStockProcess(Unknown Source)
	at com.wlyd.fmcgwms.service.thread.UpdateProductThead$DoBusinessThread.run(UpdateProductThead.java:101)
	at java.util.concurrent.ThreadPoolExecutor.runWorker(Unknown Source)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(Unknown Source)
	at java.lang.Thread.run(Unknown Source)
```

好了终于找到问题，就是数据库连接池多线程配置的问题。



### 6.配置数据库连接池



```
<bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource" init-method="init"    destroy-method="close" >
    <property name="driverClassName">
      <value>${jdbc.driverClass}</value>
    </property>
    <property name="url">
      <value>${jdbc.jdbcUrl}</value>
    </property>
    <property name="username">
      <value>${jdbc.user}</value>
    </property>
    <property name="password">
      <value>${jdbc.password}</value>
    </property>
    <!-- 连接池最大使用连接数 -->
    <property name="maxActive">
      <value>20</value>
    </property>
    <!-- 初始化连接大小 -->
    <property name="initialSize">
      <value>10</value>
    </property>
    <!-- 获取连接最大等待时间 -->
    <property name="maxWait">
      <value>60000</value>
    </property>
    <!-- 连接池最大空闲 -->
    <property name="maxIdle">
      <value>20</value>
    </property>
    <!-- 连接池最小空闲 -->
    <property name="minIdle">
      <value>3</value>
    </property>
    <!-- 自动清除无用连接 -->
    <property name="removeAbandoned">
      <value>true</value>
    </property>
    <!-- 清除无用连接的等待时间 -->
    <property name="removeAbandonedTimeout">
      <value>180</value>
    </property>
    <!-- 连接属性 -->
    <property name="connectionProperties">
      <value>clientEncoding=UTF-8</value>
    </property>
  </bean>
```

将initialSize改成大于1的数字（因为是开发环境，默认配成的是1）。

配置完成后测试运行，整个问题总算是解决了，控制台输出如下：



```java
2016-08-01 14:21:30,815 [main] INFO  [org.quartz.core.QuartzScheduler] -
 JobFactory set to: org.springframework.scheduling.quartz.AdaptableJobFactory@29a23c3d
2016-08-01 14:21:30,824 [main] INFO  [org.springframework.context.support.DefaultLifecycleProcessor] - Starting beans in phase 2147483647
2016-08-01 14:21:30,824 [main] INFO  [org.springframework.scheduling.quartz.SchedulerFactoryBean] - Starting Quartz Scheduler now
2016-08-01 14:21:30,824 [main] INFO  [org.quartz.core.QuartzScheduler] - Scheduler SpringJobSchedulerFactoryBean_$_NON_CLUSTERED started.
2016-08-01 14:21:30,834 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@402f80f5]
2016-08-01 14:21:30,834 [main] DEBUG [java.sql.Connection] - ==> 
 Preparing: select ES_COR_ID, ES_COR_NAME,ES_COR_ALIAS, ES_COR_ADDRESS, ES_COR_JURIDICAL_PERSON, ES_COR_CONTACT_EMAIL, 
ES_COR_CONTACT_TEL,ES_COR_DESCRIBE, ES_COR_NOTE,ES_COR_CREATE_TIME,ES_COR_MODIFY_TIME from es_corporation 
2016-08-01 14:21:30,834 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-08-01 14:21:30,873 [pool-1-thread-2] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@8c928a2]
2016-08-01 14:21:30,873 [pool-1-thread-6] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@44290393]
2016-08-01 14:21:30,874 [pool-1-thread-2] DEBUG [java.sql.Connection] - ==> 
 Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,
st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit FROM wm_stock_10001 st 
LEFT JOIN cd_wh_itme_10001 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-08-01 14:21:30,874 [pool-1-thread-6] DEBUG [java.sql.Connection] - ==> 
 Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,
st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit FROM wm_stock_10005 st
 LEFT JOIN cd_wh_itme_10005 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-08-01 14:21:30,874 [pool-1-thread-6] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-08-01 14:21:30,874 [pool-1-thread-1] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@402f80f5]
2016-08-01 14:21:30,875 [pool-1-thread-1] DEBUG [java.sql.Connection] - ==>  
Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit
 FROM wm_stock_10000 st LEFT JOIN cd_wh_itme_10000 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-08-01 14:21:30,875 [pool-1-thread-1] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-08-01 14:21:30,874 [pool-1-thread-5] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@59fa1469]
2016-08-01 14:21:30,874 [pool-1-thread-2] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-08-01 14:21:30,875 [pool-1-thread-5] DEBUG [java.sql.Connection] - ==> 
 Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit
 FROM wm_stock_10004 st LEFT JOIN cd_wh_itme_10004 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-08-01 14:21:30,875 [pool-1-thread-5] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-08-01 14:21:30,876 [pool-1-thread-3] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@177f3dab]
2016-08-01 14:21:30,876 [pool-1-thread-3] DEBUG [java.sql.Connection] - ==>
 Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit
 FROM wm_stock_10002 st LEFT JOIN cd_wh_itme_10002 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-08-01 14:21:30,876 [pool-1-thread-3] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-08-01 14:21:30,878 [Thread-4] INFO  [org.springframework.context.support.GenericApplicationContext] 
- Closing org.springframework.context.support.GenericApplicationContext@7ac7a4e4: startup date [Mon Aug 01 14:21:27 CST 2016]; root of context hierarchy
2016-08-01 14:21:30,879 [Thread-4] INFO  [org.springframework.context.support.DefaultLifecycleProcessor] 
- Stopping beans in phase 2147483647
2016-08-01 14:21:30,880 [Thread-4] INFO  [org.quartz.core.QuartzScheduler] - Scheduler SpringJobSchedulerFactoryBean_$_NON_CLUSTERED paused.
2016-08-01 14:21:30,880 [Thread-4] INFO  [org.springframework.beans.factory.support.DefaultListableBeanFactory] 
- Destroying singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@4e08711f: defining beans [wmStockAlertRlueServiceImpl,wmWarringInfoServiceImpl,cdWarehouseServiceImpl,cdWhCustomerServiceImpl,cdWhItmeServiceImpl,cdWhLocAreaServiceImpl,cdWhLocationServiceImpl,wmStockAdjustServiceImpl,wmStockFreezeServiceImpl,wmStockReleaseServiceImpl,wmStockServiceImpl,esMenuFunctionsServiceImpl,esMrRightsServiceImpl,esMuRightsServiceImpl,WMWareHouseStatServiceImpl,esDictionaryServiceImpl,esDictionaryTypeServiceImpl,esExceptionServiceImpl,esIconsServiceImpl,esMenuServiceImpl,esPropertiesServiceImpl,esRoleServiceImpl,esUserServiceImpl,wmIntoOrderBatchCodenoServiceImpl,wmIntoOrderinfoServiceImpl,wmIntoOrderServiceImpl,wmReceiptServiceImpl,wmReturnInfoServiceImpl,wmInventorySheetDetailServiceImpl,wmInventorySheetServiceImpl,wmWarehouseMoveServiceImpl,ebInterfaceLogServiceImpl,ebIntoStockLogServiceImpl,ebOperateLogServiceImpl,ebOutStockLogServiceImpl,ebSysLogServiceImpl,wmBatchPickingServiceImpl,wmOutOrderDetailServiceImpl,wmOutOrderServiceImpl,APIForOMSServiceImpl,APIServiceImpl,corporationServiceImpl,esSaasServiceInfoServiceImpl,JZTDAPIServiceImpl,padServiceImpl,pdaApiImpl,quartzJobServiceImpl,statisticalChartsServiceImpl,stockWarringJobServiceImpl,syncOrderStutusServiceImpl,timerTaskServiceImpl,reportServiceImpl,wmItemDailyReportServiceImpl,cdWhUnpickServiceImpl,settingsServiceImpl,updateProductThead,org.springframework.context.annotation.internalConfigurationAnnotationProcessor,org.springframework.context.annotation.internalAutowiredAnnotationProcessor,org.springframework.context.annotation.internalRequiredAnnotationProcessor,org.springframework.context.annotation.internalCommonAnnotationProcessor,org.springframework.context.annotation.internalPersistenceAnnotationProcessor,org.springframework.beans.factory.config.PropertyPlaceholderConfigurer#0,dataSource,sqlSessionFactory,sqlSession,org.mybatis.spring.mapper.MapperScannerConfigurer#0,transactionManager,txAdvice,org.springframework.aop.config.internalAutoProxyCreator,pc,org.springframework.aop.support.DefaultBeanFactoryPointcutAdvisor#0,InitSysProperties,DailyTaskJob,DailyTaskJobMethod,DailyTaskCronTriggerBean,DailyTaskJobMethodSaas,DailyTaskCronTriggerBeanSaas,StockWarringTaskJob,StockWarringTaskJobMethod,StockWarringTaskCronTriggerBean,CycleSendInterfaceInfoTaskJob,CycleSendInterfaceInfoTaskJobMethod,CycleSendInterfaceInfoTaskCronTriggerBean,SpringJobSchedulerFactoryBean,taskExecutor,org.springframework.context.annotation.ConfigurationClassPostProcessor.importAwareProcessor,wmStockAlertRuleMapper,wmWarringInfoMapper,cdCustomerMapper,cdWarehouseMapper,cdWhItmeMapper,cdWhLocAreaMapper,cdWhLocationMapper,wmStockAdjustMapper,wmStockFreezeMapper,wmStockMapper,wmStockReleaseMapper,esMenuFunctionsMapper,esMrRightsMapper,esMuRightsMapper,esDictionaryMapper,esDictionaryTypeMapper,ebUserWarehouseMapper,esExceptionMapper,esIconsMapper,esMenuMapper,esPropertiesMapper,esRoleMapper,esUserMapper,wmIntoOrderBatchCodenoMapper,wmIntoOrderCodenoMapper,wmIntoOrderInfoMapper,wmIntoOrderMapper,wmReceiptInfoMapper,wmReturnInfoMapper,wmInventorySheetDetailMapper,wmInventorySheetMapper,wmWarehouseMoveCodenoMapper,wmWarehouseMoveMapper,ebInterfaceLogMapper,ebIntoStockLogMapper,ebOperateLogMapper,ebOutStockLogMapper,ebSysLogMapper,wmAssignedPickcarMapper,wmBatchPickingMapper,wmOutOrderCodenoMapper,wmOutOrderDetailMapper,wmOutOrderMapper,wmStockAssignedMapper,corporationMapper,esSaasServiceInfoMapper,statisticalChartsMapper,wmItemTransferMapper,reportMapper,wmDailyItemReportMapper,settingsMapper,wmUnpickMapper]; root of factory hierarchy
2016-08-01 14:21:30,880 [pool-1-thread-6] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10005)新增组织测试预警信息菜单没有库存记录！！！
2016-08-01 14:21:30,884 [Thread-4] INFO  [org.springframework.scheduling.quartz.SchedulerFactoryBean] - Shutting down Quartz Scheduler
2016-08-01 14:21:30,884 [Thread-4] INFO  [org.quartz.core.QuartzScheduler] - Scheduler SpringJobSchedulerFactoryBean_$_NON_CLUSTERED shutting down.
2016-08-01 14:21:30,884 [Thread-4] INFO  [org.quartz.core.QuartzScheduler] - Scheduler SpringJobSchedulerFactoryBean_$_NON_CLUSTERED paused.
2016-08-01 14:21:30,884 [pool-1-thread-5] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10004)测试新增组织没有库存记录！！！
2016-08-01 14:21:30,885 [pool-1-thread-9] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@7086ced1]
2016-08-01 14:21:30,885 [pool-1-thread-9] DEBUG [java.sql.Connection] - ==>  Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit FROM wm_stock_10008 st LEFT JOIN cd_wh_itme_10008 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-08-01 14:21:30,885 [pool-1-thread-9] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-08-01 14:21:30,888 [pool-1-thread-3] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10002)test没有库存记录！！！
2016-08-01 14:21:30,888 [pool-1-thread-7] DEBUG [java.sql.Connection] - ooo Using Connection [com.mysql.jdbc.JDBC4Connection@767cef01]
2016-08-01 14:21:30,888 [pool-1-thread-7] DEBUG [java.sql.Connection] - ==>  Preparing: SELECT cd.CDSK_ITEM_CODE ProductCode,st.WMST_NOW_NUMBER ProductNum,st.WMST_ENABLED_NUMBER AvailableProductNum,st.WMST_SKU_UNIT ProductUnit FROM wm_stock_10006 st LEFT JOIN cd_wh_itme_10006 cd ON cd.CD_ITEM_ID=st.WMST_SKU_ID GROUP BY st.WMST_SKU_NAME,st.WMST_SKU_UNIT 
2016-08-01 14:21:30,892 [pool-1-thread-7] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2016-08-01 14:21:30,895 [Thread-4] INFO  [org.quartz.core.QuartzScheduler] - Scheduler SpringJobSchedulerFactoryBean_$_NON_CLUSTERED shutdown complete.
2016-08-01 14:21:30,896 [Thread-4] INFO  [org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor] - Shutting down ExecutorService 'taskExecutor'
2016-08-01 14:21:30,900 [Thread-4] INFO  [com.alibaba.druid.pool.DruidDataSource] - {dataSource-1} closed
2016-08-01 14:21:30,901 [pool-1-thread-9] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10008)测试新增专用组织没有库存记录！！！
2016-08-01 14:21:30,903 [pool-1-thread-7] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10006)test1没有库存记录！！！
```

库存多线程操作查询：



```java
2016-08-01 14:42:21,468 [pool-1-thread-5] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10004)测试新增组织没有库存记录！！！
2016-08-01 14:42:21,468 [pool-1-thread-7] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10006)test1没有库存记录！！！
2016-08-01 14:42:21,471 [pool-1-thread-2] ERROR [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>>>>>>>>>>>>>>有库存。。。。。。。。。。。。tenantCode:10001
2016-08-01 14:42:21,471 [pool-1-thread-3] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10002)test没有库存记录！！！
2016-08-01 14:42:21,468 [pool-1-thread-8] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10007)aaabbbbbb没有库存记录！！！
2016-08-01 14:42:21,475 [pool-1-thread-4] ERROR [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>>>>>>>>>>>>>>有库存。。。。。。。。。。。。tenantCode:10003
2016-08-01 14:42:21,470 [pool-1-thread-6] INFO  [com.wlyd.fmcgwms.service.basic.impl.WmStockServiceImpl] - >>>>组织(10005)新增组织测试预警信息菜单没有库存记录！！！
2016-08-01 14:42:21,479 [Thread-4] INFO  [com.alibaba.druid.pool.DruidDataSource] - {dataSource-1} closed
```



**注：第5点中，实际问题是因为我自己少写了start()而不自知，所以才写了第5点的尝试，实际上完全没有必要，平时编程的时候还是注意点，避免走不必要的弯路。**

**实际上Spring提供了一个批处理的工具spring-batch 有兴趣的可以研究研究，可以替代上面的方案。**








