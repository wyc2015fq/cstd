# java.lang.NoSuchFieldError: VERSION_2_3_0 报错解决方案 - z69183787的专栏 - CSDN博客
2017年03月01日 16:04:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2442
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
java.lang.NoSuchFieldError: VERSION_2_3_0
    at org.apache.struts2.views.freemarker.FreemarkerManager.createConfiguration(FreemarkerManager.java:331)
    at org.apache.struts2.views.freemarker.FreemarkerManager.init(FreemarkerManager.java:282)
    at org.apache.struts2.views.freemarker.FreemarkerManager.getConfiguration(FreemarkerManager.java:269)
    at org.apache.struts2.dispatcher.DefaultDispatcherErrorHandler.init(DefaultDispatcherErrorHandler.java:47)
    at org.apache.struts2.dispatcher.Dispatcher.init(Dispatcher.java:488)
    at org.apache.struts2.dispatcher.ng.InitOperations.initDispatcher(InitOperations.java:74)
    at org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter.init(StrutsPrepareAndExecuteFilter.java:57)
    at org.apache.catalina.core.ApplicationFilterConfig.initFilter(ApplicationFilterConfig.java:281)
    at org.apache.catalina.core.ApplicationFilterConfig.getFilter(ApplicationFilterConfig.java:262)
    at org.apache.catalina.core.ApplicationFilterConfig.<init>(ApplicationFilterConfig.java:107)
    at org.apache.catalina.core.StandardContext.filterStart(StandardContext.java:4746)
    at org.apache.catalina.core.StandardContext.startInternal(StandardContext.java:5399)
    at org.apache.catalina.util.LifecycleBase.start(LifecycleBase.java:150)
    at org.apache.catalina.core.ContainerBase.addChildInternal(ContainerBase.java:901)
    at org.apache.catalina.core.ContainerBase.addChild(ContainerBase.java:877)
    at org.apache.catalina.core.StandardHost.addChild(StandardHost.java:633)
    at org.apache.catalina.startup.HostConfig.deployDirectory(HostConfig.java:1114)
    at org.apache.catalina.startup.HostConfig$DeployDirectory.run(HostConfig.java:1672)
    at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:441)
    at java.util.concurrent.FutureTask$Sync.innerRun(FutureTask.java:303)
    at java.util.concurrent.FutureTask.run(FutureTask.java:138)
    at java.util.concurrent.ThreadPoolExecutor$Worker.runTask(ThreadPoolExecutor.java:886)
    at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:908)
    at java.lang.Thread.run(Thread.java:619)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**好吧,今天公司主管看到一个新闻说除了struts 2.3.20.2，2.3.24.2，2.3.28.1几个版本外其他版本都不安全,因而	对旧版进行升级,这个项目不是maven项目,要把jar包一个一个替换,但是不是简单换了jar包就行了,有些旧版特性新版不	一定支持,新版兼容到的旧系统其他jar不一定支持,所以升级也是一个技术活啊。。。简单解决一个百度上好像都没人讨论	过的一个版本问题,这个版本问题的解决方案是要替换freemarker的包到freemarker-2.3.23.jar,也就是最高版本,同时	也提醒自己遇到问题一定不要慌,要根据出错提示分步进行解决.技术上的问题都是有办法解决的,要安静的思考一下解决方案.**
# [struts2更新版本操作有关事项备注](http://www.cnblogs.com/wangxuerui/p/5292228.html)
**struts2更新版本操作有关事项备注，**
**更新主要jar包：struts2-convention-plugin-version，struts2-core-version, struts2-spring-plugin-version  **
**依赖jar包:xwork-core-version**
**报错应更新有关jar包: freemarker-verson，log4j-version**
**mevan实例：**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!-- struts2 所有jar包 -->
        <dependency>
            <groupId>org.apache.struts</groupId>
            <artifactId>struts2-core</artifactId>
            <!--<version>2.2.1</version>-->
            <version>2.3.24.1</version>
            <exclusions>
                <exclusion>
                    <groupId>com.sun</groupId>
                    <artifactId>tools</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <groupId>org.apache.struts</groupId>
            <artifactId>struts2-spring-plugin</artifactId>
            <!--<version>2.2.1</version>-->
            <version>2.3.24.1</version>
        </dependency>
        <dependency>
            <groupId>org.apache.struts</groupId>
            <artifactId>struts2-convention-plugin</artifactId>
            <!--<version>2.2.1</version>-->
            <version>2.3.24.1</version>
        </dependency>
        <dependency>
            <groupId>org.freemarker</groupId>
            <artifactId>freemarker</artifactId>
            <version>2.3.23</version>
        </dependency>
        <dependency>
            <groupId>log4j</groupId>
            <artifactId>log4j</artifactId>
            <version>1.2.17</version>
          </dependency>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**xwork-core-version此包不更新涉及报错：**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
  <filter>
       <filter-name>StrutsPrepareFilter</filter-name>
       <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareFilter</filter-class>
    </filter>
    <filter>
       <filter-name>StrutsExecuteFilter</filter-name>
       <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsExecuteFilter</filter-class>
    </filter>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**此配置文件 类丢失错误**
**freemarker-verson，log4j-version 包不更新涉及报错：**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
java.lang.NoSuchFieldError: VERSION_2_3_0
    at org.apache.struts2.views.freemarker.FreemarkerManager.createConfiguration(FreemarkerManager.java:331)
    at org.apache.struts2.views.freemarker.FreemarkerManager.init(FreemarkerManager.java:282)
    at org.apache.struts2.views.freemarker.FreemarkerManager.getConfiguration(FreemarkerManager.java:269)
    at org.apache.struts2.dispatcher.DefaultDispatcherErrorHandler.init(DefaultDispatcherErrorHandler.java:47)
    at org.apache.struts2.dispatcher.Dispatcher.init(Dispatcher.java:488)
    at org.apache.struts2.dispatcher.ng.InitOperations.initDispatcher(InitOperations.java:74)
    at org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter.init(StrutsPrepareAndExecuteFilter.java:57)
    at org.apache.catalina.core.ApplicationFilterConfig.initFilter(ApplicationFilterConfig.java:281)
    at org.apache.catalina.core.ApplicationFilterConfig.getFilter(ApplicationFilterConfig.java:262)
    at org.apache.catalina.core.ApplicationFilterConfig.<init>(ApplicationFilterConfig.java:107)
    at org.apache.catalina.core.StandardContext.filterStart(StandardContext.java:4746)
    at org.apache.catalina.core.StandardContext.startInternal(StandardContext.java:5399)
    at org.apache.catalina.util.LifecycleBase.start(LifecycleBase.java:150)
    at org.apache.catalina.core.ContainerBase.addChildInternal(ContainerBase.java:901)
    at org.apache.catalina.core.ContainerBase.addChild(ContainerBase.java:877)
    at org.apache.catalina.core.StandardHost.addChild(StandardHost.java:633)
    at org.apache.catalina.startup.HostConfig.deployDirectory(HostConfig.java:1114)
    at org.apache.catalina.startup.HostConfig$DeployDirectory.run(HostConfig.java:1672)
    at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:441)
    at java.util.concurrent.FutureTask$Sync.innerRun(FutureTask.java:303)
    at java.util.concurrent.FutureTask.run(FutureTask.java:138)
    at java.util.concurrent.ThreadPoolExecutor$Worker.runTask(ThreadPoolExecutor.java:886)
    at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:908)
    at java.lang.Thread.run(Thread.java:619)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
