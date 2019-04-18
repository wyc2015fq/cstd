# Struts2升级版本至2.5.10，高危漏洞又来了 - z69183787的专栏 - CSDN博客
2017年07月11日 17:21:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：582
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)
[http://blog.csdn.net/zhulin2012/article/details/61916778](http://blog.csdn.net/zhulin2012/article/details/61916778)
![这里写图片描述](http://blog.52itstyle.com/usr/uploads/2017/02/1799720926.png)
#### 前情概要
漏洞年年有，最近特别多。2017年3月6日，Apache Struts2被曝存在远程命令执行漏洞，漏洞编号:S2-045，CVE编号:CVE-2017-5638，官方评级为高危，该漏洞是由于在使用基于Jakarta插件的文件上传功能条件下，恶意用户可以通过修改HTTP请求头中的Content-Type值来触发该漏洞，进而执行任意系统命令，导致系统被黑客入侵。
漏洞分析请移步：[https://yq.aliyun.com/articles/72008](https://yq.aliyun.com/articles/72008)
#### 建议
如果这个版本在Struts2.3.5 到 Struts2.3.31 以及 Struts2.5 到 Struts2.5.10之间则存在漏洞，请升级到struts 2.3.32或2.5.10.1版本(哔了狗了狗了，公司还在用struts2)。
由于目前使用版本是struts2-core-2.3.28，于是赶紧升了下级别，略过2.3 直接升级到2.5版本。
#### 升级说明
一、找不到类
`java.lang.ClassNotFoundException: org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter`- 1
- 1
找不到那得多正常，查看了一下源码，2.5版本已变更了包路径。
```
<filter>
    <filter-name>struts2</filter-name>
    <filter-class>  org.apache.struts2.dispatcher.filter.StrutsPrepareAndExecuteFilter
    </filter-class>
  </filter>
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
二、配置好了居然找不到Action
是不是有点怀疑人生了，赶紧去[http://struts.apache.org/](http://struts.apache.org/) struts2的官网扒拉扒拉。
由于版本跨度大，2.5版本升级了很多特性，在Struts 2.5中，严格DMI被扩展，它被称为严格方法调用 又名SMI。你可以想象DMI是一个“边境警察”，SMI是一个“税务警察”，并注意内部。使用此版本，SMI默认情况下启用（strict-method-invocation属性默认设置为 true在 struts-default包中），您可以选择禁用它每个包 - 没有全局开关禁用整个应用程序的SMI。
```
SMI的工作原理如下：
<allowed-methods> / @AllowedMethods 被定义每个动作 - SMI工作，而不打开它，但只是那些动作（加上<global-allowed-methods/>）
SMI已启用，但没有<allowed-methods> / @AllowedMethods 被定义 - SMI工作，但只有<global-allowed-methods/>
SMI已禁用 - 允许调用任何与默认RegEx匹配的操作方法 - ([A-Za-z0-9_$]*)
您可以使用常量重新定义默认RegEx，如下所示 <constant name="struts.strictMethodInvocation.methodRegex" value="([a-zA-Z]*)"/>
在操作定义中使用通配符映射时，SMI有两种方式：
SMI被禁用 - 任何通配符将被替换为默认的RegEx，即：<action name="Person*" method="perform*">将被翻译成allowedMethod = "regex:perform([A-Za-z0-9_$]*)".
启用S​​MI - 不会发生通配符替换，必须严格定义通过注释或<allowed-method/>标记可以访问哪些方法。
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
也就是说你必须加入这个配置才可以，最好全局设置：
`<global-allowed-methods>regex:.*</global-allowed-methods>`- 1
- 1
三、package中元素顺序的问题
`元素类型为 "package" 的内容必须匹配 "(result-types?,interceptors?,default-interceptor-ref?,default-action-ref?`- 1
- 1
出现此报错，你就要注意了，一定要检查package中 global-allowed-methods 的位置，然后按照报错指定顺序放置配置。
四、可能会出现的问题
如果你的项目中使用的是log4j而不是log4j2，那么问题就来了，你还需要加入log4j-api-2.7.jar 和log4j-core-2.7.jar，并且配置 log4j2.xml(暂且是个空的)。
```xml
<?xml version="1.0" encoding="UTF-8"?>   
<configuration status="error">   
  <appenders>   
  </appenders>   
  <loggers>   
    <root level="info">   
    </root>   
  </loggers>   
</configuration>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
否则会一直报以下错误，至于为什么，还没深入了解。
`ERROR StatusLogger No log4j2 configuration file found. Using default configuration: logging only errors to the console.`- 1
- 1
好了，如果启动没有其他问题，应该就可以访问到久违的Action了。
原文：[http://blog.52itstyle.com/archives/430/](http://blog.52itstyle.com/archives/430/)
