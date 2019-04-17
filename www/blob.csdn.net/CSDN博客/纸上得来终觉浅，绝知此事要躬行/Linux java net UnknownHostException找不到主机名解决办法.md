# Linux java.net.UnknownHostException找不到主机名解决办法 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月29日 17:40:38[boonya](https://me.csdn.net/boonya)阅读数：17152








今天部署应用到Tomcat，发现启动过程中有异常，验证码前台出不来，以为是此异常导致的，后来解决了java.net.UnknownHostException，登录页面的验证码还是出不来不知是何问题。

## 详细异常输出

报错方式：java.net.UnknownHostException: 主机名: 主机名




```
java.net.UnknownHostException: RD-CD-APP122.wlyd.local: RD-CD-APP122.wlyd.local: ?..?..绉版.?..
	at java.net.InetAddress.getLocalHost(InetAddress.java:1473)
	at net.sf.ehcache.Cache.<clinit>(Cache.java:214)
	at net.sf.ehcache.config.ConfigurationHelper.createCache(ConfigurationHelper.java:296)
	at net.sf.ehcache.config.ConfigurationHelper.createDefaultCache(ConfigurationHelper.java:219)
	at net.sf.ehcache.CacheManager.configure(CacheManager.java:722)
	at net.sf.ehcache.CacheManager.doInit(CacheManager.java:439)
	at net.sf.ehcache.CacheManager.init(CacheManager.java:377)
	at net.sf.ehcache.CacheManager.<init>(CacheManager.java:259)
	at net.sf.ehcache.CacheManager.newInstance(CacheManager.java:1037)
	at net.sf.ehcache.CacheManager.newInstance(CacheManager.java:818)
	at net.sf.ehcache.CacheManager.create(CacheManager.java:799)
	at com.wlyd.fmcgwms.util.ehcache.EhcacheUtil.<clinit>(EhcacheUtil.java:18)
	at com.wlyd.fmcgwms.util.sysinit.InitSysProperties.setApplicationContext(InitSysProperties.java:55)
	at org.springframework.context.support.ApplicationContextAwareProcessor.invokeAwareInterfaces(ApplicationContextAwareProcessor.java:117)
	at org.springframework.context.support.ApplicationContextAwareProcessor.postProcessBeforeInitialization(ApplicationContextAwareProcessor.java:92)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.applyBeanPostProcessorsBeforeInitialization(AbstractAutowireCapableBeanFactory.java:396)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.initializeBean(AbstractAutowireCapableBeanFactory.java:1505)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:521)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:458)
	at org.springframework.beans.factory.support.AbstractBeanFactory$1.getObject(AbstractBeanFactory.java:296)
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:223)
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:293)
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:194)
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.preInstantiateSingletons(DefaultListableBeanFactory.java:628)
	at org.springframework.context.support.AbstractApplicationContext.finishBeanFactoryInitialization(AbstractApplicationContext.java:932)
	at org.springframework.context.support.AbstractApplicationContext.refresh(AbstractApplicationContext.java:479)
	at org.springframework.web.context.ContextLoader.configureAndRefreshWebApplicationContext(ContextLoader.java:389)
	at org.springframework.web.context.ContextLoader.initWebApplicationContext(ContextLoader.java:294)
	at org.springframework.web.context.ContextLoaderListener.contextInitialized(ContextLoaderListener.java:112)
	at org.apache.catalina.core.StandardContext.listenerStart(StandardContext.java:4994)
	at org.apache.catalina.core.StandardContext.startInternal(StandardContext.java:5492)
	at org.apache.catalina.util.LifecycleBase.start(LifecycleBase.java:150)
	at org.apache.catalina.core.ContainerBase.addChildInternal(ContainerBase.java:901)
	at org.apache.catalina.core.ContainerBase.addChild(ContainerBase.java:877)
	at org.apache.catalina.core.StandardHost.addChild(StandardHost.java:649)
	at org.apache.catalina.startup.HostConfig.deployWAR(HostConfig.java:1081)
	at org.apache.catalina.startup.HostConfig$DeployWar.run(HostConfig.java:1877)
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:471)
	at java.util.concurrent.FutureTask.run(FutureTask.java:262)
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1145)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:615)
	at java.lang.Thread.run(Thread.java:745)
Caused by: java.net.UnknownHostException: RD-CD-APP122.wlyd.local: ?..?..绉版.?..
	at java.net.Inet6AddressImpl.lookupAllHostAddr(Native Method)
	at java.net.InetAddress$1.lookupAllHostAddr(InetAddress.java:901)
	at java.net.InetAddress.getAddressesFromNameService(InetAddress.java:1293)
	at java.net.InetAddress.getLocalHost(InetAddress.java:1469)
	... 41 more
2016-12-29 17:11:38,875 [localhost-startStop-1] INFO  [org.quartz.core.SchedulerSignalerImpl] - Initialized Scheduler Signaller of type: class org.quartz.core.SchedulerSignalerImpl
2016-12-29 17:11:38,885 [localhost-startStop-1] INFO  [org.quartz.core.QuartzScheduler] - Quartz Scheduler v.1.8.5 created.
2016-12-29 17:11:38,886 [localhost-startStop-1] INFO  [org.quartz.simpl.RAMJobStore] - RAMJobStore initialized.
2016-12-29 17:11:38,888 [localhost-startStop-1] INFO  [org.quartz.core.QuartzScheduler] - Scheduler meta-data: Quartz Scheduler (v1.8.5) 'SpringJobSchedulerFactoryBean' with instanceId 'NON_CLUSTERED'
  Scheduler class: 'org.quartz.core.QuartzScheduler' - running locally.
  NOT STARTED.
  Currently in standby mode.
  Number of jobs executed: 0
  Using thread pool 'org.quartz.simpl.SimpleThreadPool' - with 10 threads.
  Using job-store 'org.quartz.simpl.RAMJobStore' - which does not support persistence. and is not clustered.
```


## 解决方法

命令：vi /etc/hosts

在最后一行添加：127.0.0.1 主机名。我的系统改成了如下图所示：

![](https://img-blog.csdn.net/20161229173755640)

修改完成，保存文件，之后就不会有此异常发生了。







