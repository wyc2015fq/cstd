# Struts2版本2.3.24升级2.5.10.1小结 - z69183787的专栏 - CSDN博客
2017年07月11日 17:22:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5093
[https://my.oschina.net/u/2552669/blog/868139](https://my.oschina.net/u/2552669/blog/868139)
因为Struts2 S2-045 Struts2 S2-046 漏洞问题 需要对struts2的版本进行升级
## 首先进行jar替换
需要移除的jar包有
struts2-json-plugin-2.3.24
struts2-core-2.3.24
ognl-3.0.6
log4j-1.2.15
xwork-core-2.3.24
新添加的jar包如下 struts2-json-plugin-2.5.10.1
struts2-core-2.5.10.1
ognl-3.1.12
log4j-api-2.7
log4j-core-2.7
log4j-1.2-api-2.7
ps：xwork-core这个类库再新版本中已经包含在struts2-core中了
在新版本中struts2 使用的是log4j2 所以需要导入log4j-api-2.7log4j-core-2.7 这两个jar包 并且为了从log4j 平稳的过度到log4j2导入log4j-1.2-api-2.7 jar（避免修改代码） 并且新的日志规则由全新的log4j2.xml文件维护 代码如下 具体规则后续在添加
```xml
<?xml version="1.0" encoding="UTF-8"?>   
<configuration status="debug">   
  <appenders>   
  </appenders>   
  <loggers>   
    <root level="info">   
    <appender-ref ref="Console"/>
    <appender-ref ref="log"/>
    </root>   
  </loggers>   
</configuration>
```
## 关于配置文件的改动
web.xml改动
原：
```xml
<filter>
    <filter-name>struts2</filter-name>
    <filter-class>
            org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter
        </filter-class>
  </filter>
```
新：
```xml
<filter>
    <filter-name>struts2</filter-name>
    <filter-class>
            org.apache.struts2.dispatcher.filter.StrutsPrepareAndExecuteFilter
        </filter-class>
  </filter>
```
struts2的配置xml的改动
首先将xml的头修改为
```xml
<!DOCTYPE struts PUBLIC
"-//Apache Software Foundation//DTD Struts Configuration 2.5//EN"
"http://struts.apache.org/dtds/struts-2.5.dtd">
```
要注意因为2.5版本升级了很多特性，在Struts 2.5中，严格DMI被扩展，它被称为严格方法调用 又名SMI。你可以想象DMI是一个“边境警察”，SMI是一个“税务警察”，并注意内部。使用此版本，SMI默认情况下启用（strict-method-invocation属性默认设置为 true在 struts-default包中），您可以选择禁用它每个包 - 没有全局开关禁用整个应用程序的SMI。
因此在使用通配符的方式的情况下会显示404
所以要在sturts2的配置文件上修改
将strict-method-invocation="false" 设置为false
或者在package包中添加<global-allowed-methods>regex:.*</global-allowed-methods>
```
<package name="videoDetail" namespace="/videoDetail" extends="json-default" strict-method-invocation="false">
        <global-allowed-methods>regex:.*</global-allowed-methods>
        <action name="*" class="com.coder.video.action.VideoDetailAction" method="{1}">
```
更多特性需要参考官方文档 参考: [http://www.tuicool.com/articles/vuimAjZ](http://www.tuicool.com/articles/vuimAjZ)
