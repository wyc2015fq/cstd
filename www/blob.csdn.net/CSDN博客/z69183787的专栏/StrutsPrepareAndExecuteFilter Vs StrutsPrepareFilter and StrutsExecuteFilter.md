# StrutsPrepareAndExecuteFilter Vs StrutsPrepareFilter and StrutsExecuteFilter - z69183787的专栏 - CSDN博客
2015年07月18日 20:54:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1430
I am learning Struts2. I have a question in mind what is the difference between in these two implementation of Filter
```
<filter>
    <filter-name>struts-prepare</filter-name>
    <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareFilter</filter-class>
</filter>
<filter>
    <filter-name>struts-execute</filter-name>
    <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsExecuteFilter</filter-class>
</filter>
<filter-mapping>
    <filter-name>struts-prepare</filter-name>
    <url-pattern>/*</url-pattern>
    <dispatcher>REQUEST</dispatcher>
    <dispatcher>FORWARD</dispatcher>
</filter-mapping>
<filter-mapping>
    <filter-name>struts-execute</filter-name>
    <url-pattern>/*</url-pattern>
    <dispatcher>REQUEST</dispatcher>
    <dispatcher>FORWARD</dispatcher>
</filter-mapping>
```
And
```
<filter>
    <filter-name>struts</filter-name>
    <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter</filter-class>
</filter>
<filter-mapping>
    <filter-name>struts</filter-name>
    <url-pattern>/*</url-pattern>
    <dispatcher>REQUEST</dispatcher>
    <dispatcher>FORWARD</dispatcher>
</filter-mapping>
```
Is both have same behavior or not. My opinion is that they both are different but I don't in which term.
Thanks
They identical, StrutsPrepareAndExecuteFilter is a combination of StrutsPrepareFilter and StrutsExecuteFilter, have a look at the implementation here
- [StrutsPrepareFilter](http://grepcode.com/file/repo1.maven.org/maven2/org.apache.struts/struts2-core/2.1.6/org/apache/struts2/dispatcher/ng/filter/StrutsPrepareFilter.java)
- [StrutsExecuteFilter](http://grepcode.com/file/repo1.maven.org/maven2/org.apache.struts/struts2-core/2.2.1/org/apache/struts2/dispatcher/ng/filter/StrutsExecuteFilter.java)
- [StrutsPrepareAndExecuteFilter](http://grepcode.com/file/repo1.maven.org/maven2/org.apache.struts/struts2-core/2.2.1/org/apache/struts2/dispatcher/ng/filter/StrutsPrepareAndExecuteFilter.java)
Notice is only:
```
Handles both the preparation and execution phases of the Struts dispatching 
process.
This filter is better to use when you don't have another filter that needs 
access to action context information, such as Sitemesh.
```
StrutsPrepareAndExecuteFilter是自2.1.3开始就替代了FilterDispatcher的.!
这样的改革当然是有好处的.!
为什么这么说.? 应该知道如果我们自己定义过滤器的话, 是要放在strtus2的过滤器之前的, 如果放在struts2过滤器之后,你自己的过滤器对action的过滤作用就废了,不会有效!除非你是访问[jsp](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=8&fv=11&is_app=0&jk=14ab738c5479fd97&k=jsp&k0=jsp&kdi0=0&luki=1&n=10&p=baidu&q=sayyescpr&rb=0&rs=1&seller_id=1&sid=97fd79548c73ab14&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1366390&u=http%3A%2F%2Fblog.knowsky.com%2F261351.htm&urlid=0)/html!
那我现在有需求, 我必须使用Action的环境,而又想在执行action之前拿filter做一些事, 用FilterDispatcher是做不到的.!
那么StrutsPrepareAndExecuteFilter可以把他拆分成StrutsPrepareFilter和StrutsExecuteFilter,可以在这两个[过滤器](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=8&fv=11&is_app=0&jk=14ab738c5479fd97&k=%B9%FD%C2%CB%C6%F7&k0=%B9%FD%C2%CB%C6%F7&kdi0=0&luki=10&n=10&p=baidu&q=sayyescpr&rb=0&rs=1&seller_id=1&sid=97fd79548c73ab14&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1366390&u=http%3A%2F%2Fblog.knowsky.com%2F261351.htm&urlid=0)之间加上我们自己的过滤器.!
给你打个比喻, 现在有病人要做手术, 现在[struts](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=8&fv=11&is_app=0&jk=14ab738c5479fd97&k=struts&k0=struts&kdi0=0&luki=9&n=10&p=baidu&q=sayyescpr&rb=0&rs=1&seller_id=1&sid=97fd79548c73ab14&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1366390&u=http%3A%2F%2Fblog.knowsky.com%2F261351.htm&urlid=0)2要做两件事,
 搭病床(环境),执行手术.! 那么打麻药的工作呢.? 不可能要病人站着打吧, 所以必须有病床的环境,打完麻药之后再动手术.! 这个比喻非常形象了.!
