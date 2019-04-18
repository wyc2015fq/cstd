# 使用Quartz2.1.6+Spring3.0实现任务调度简单示例 - z69183787的专栏 - CSDN博客
2013年12月08日 18:51:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1454
根据各位网友的指导，特别是 [lk_well](http://www.cnblogs.com/lk_well/) 朋友的分析：
从Spring3.2.4才开始支持quart-2.x
```
spring3.0还不支持quartz2.x.
楼主的例子中引用了myeclispe自带的Spring 3.0 Misc Libray，这个library中包含com.springsource.org.quartz-1.6.x.jar
其实楼主的例子还是用的quartz1.x.jar才运行成功的。
```
[lk_well](http://www.cnblogs.com/lk_well/) 的博客中也给出了正确的代码：[博文地址](http://blog.csdn.net/lk_blog/article/details/11744621)(Spring-3.2.4
 + Quartz-2.2.0集成实例)
请大家参照他的文章来做测试，谢谢。
环境：Myeclipse10+JDK1.6+Spring3.0（Myeclipse自带的） +Quartz2.1.6+Tomcat7 
因为项目中要用到任务调度，如果使用Spring自带的定时器的话，配置调度规则时不够灵活。头决定使用Quartz，发了俩链接供我参考，从那俩链接对Quartz有了初步的认识，但是那俩链接都是针对1.6版本的，1.8版本后有了大的改变，包括初始化方法都发生了改变。参照了网上的文章并阅读了官方的文档调试了将近两天依然没法调通，官方的文档并不太相近，示例也并不丰富。
后来朋友给我远程调试了下，问题解决了。
解决过程中遇到过以下问题： 
1.web.xml中关于Spring配置文件配置的顺序s问题，Spring的配置文件位置制定必须放在其他Spring其他配置之前。
2.缺少 commons-collections-3.2.jar 因为会用到org/apache/commons/collections下面的某个类，而我的项目中没有。这个要从网上下。
3.applicationContext.xml中关于Job的配置，网上有些文章中，实际工作类是继承于Spring的某个类，这个类实现了Quartz的Job接口，但是按照那个写法，和对应的配置文件，无法工作，后来用了朋友给的方法，工作类就是普通的java类，在配置文件中制定具体的方法，配置文件稍后贴上。
步骤：
1：在Myeclipse中创建Web项目，创建完毕后在项目上点右键 ，Myeclipse、Add Spring Capabilities，选择要添加的包，我把带Spring字样的都选上了，其实用不到这么多。
2：复制下载Quartz-2.16解压后根目录中quartz-all-2.1.6.jar和lib目录中的所有jar包，在项目上右键，粘贴，相同方法再添加 commons-collections-3.2.jar  到项目中，再在项目上右键、properties， Java Build Path，Add JARs，选择刚添加的那些jar包，将这些jar包引用到项目中。（你也可以不将这些jar包复制到项目中，而直接在java build path界面中添加外部的，但是为了便于项目的复制，我一般都这么做）
3.编写工作的类。代码很简单：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/* 
 * @author sixi 
 * @version 0.1 
 * Company:Tsinghua
 * Date:2012-09-15
 * Description:这是一个执行调度的具体类，在Spring配置文件中指定调用此类的work方法
 * */
package com.tsinghua.test;
import java.util.Date;
/*
 * 使用spring+Quartz执行任务调度的具体类
 * */
public class MyJob {
    /*
     * Description:具体工作的方法，此方法只是向控制台输出当前时间，
     * 输入的日志在：%tomcatRoot%\logs\tomcat7-stdout.yyyy-MM-dd.log中，
     * 其中，yyyy-MM-dd为部署的日期,经试验发现默认情况下并不是每天都生成一个stdout的日志文件
     * @return 返回void
     * */
    public void work()
    {
         System.out.println("当前时间:"+new Date().toString());  
    }
}//End of MyJob
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 4.配置web.xml，具体配置文件如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 <?xml version="1.0" encoding="UTF-8"?> 
      <web-app version="3.0" xmlns="http://java.sun.com/xml/ns/javaee"    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"    xsi:schemaLocation    http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
    <display-name>SpringQuartzDemo</display-name>
    <welcome-file-list>
        <welcome-file>index.jsp</welcome-file>
    </welcome-file-list>
    <!-- 說明：Spring的配置文件設置必須在啟動Spring Bean工廠監聽之前，否則會報錯， .Net配置文件好像沒有先後順序 -->
    <!-- Spring配置文件開始 -->
    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>/WEB-INF/classes/applicationContext.xml</param-value>
    </context-param>
    <!-- Spring配置文件結束 -->
    <!-- 启动 Spring Bean 工厂监听開始 -->
    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>
    <!-- 启动 Spring Bean 工厂监听結束 -->
</web-app>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 5.配置applicationContext.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
     http://www.springframework.org/schema/beans/spring-beans-3.0.xsd">
    <!-- 工作的bean -->
    <bean id="myJob" class="com.tsinghua.test.MyJob" />
    <!-- job的配置开始 -->
    <bean id="myJobDetail"
        class="org.springframework.scheduling.quartz.MethodInvokingJobDetailFactoryBean">
        <property name="targetObject">
            <ref bean="myJob" />
        </property>
        <property name="targetMethod">
            <value>work</value>
        </property>
    </bean>
    <!-- job的配置结束 -->
    <!-- 调度的配置开始 -->
    <bean id="crontestJobTrigger" class="org.springframework.scheduling.quartz.CronTriggerBean">
        <property name="jobDetail">
            <ref bean="myJobDetail" />
        </property>
        <property name="cronExpression">
            <value>0/1 * * * * ?</value>
        </property>
    </bean>
    <!-- 调度的配置结束 -->
    <!-- 启动触发器的配置开始 -->
- 
<!--  总管理类如果将lazy-init='false'那么容器启动就会执行调度程序   -->  
    <bean name="startQuertz" lazy-init="false" autowire="no"
        class="org.springframework.scheduling.quartz.SchedulerFactoryBean">
        <property name="triggers">
            <list>
                <ref bean="crontestJobTrigger" />
            </list>
        </property>
    </bean>
    <!-- 启动触发器的配置结束 -->
</beans>  
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 6.在Tomcat中调试，即可在Console窗口看到输出
项目代码：
[SpringQuartzDemo](http://files.cnblogs.com/sixiweb/SpringQuartzDemo.7z)
