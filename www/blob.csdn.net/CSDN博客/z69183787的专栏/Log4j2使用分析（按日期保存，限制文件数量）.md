# Log4j2使用分析（按日期保存，限制文件数量） - z69183787的专栏 - CSDN博客
2016年12月12日 16:43:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11369
本人近年都在做[Android](http://lib.csdn.net/base/android)项目，最近有个纯[Java](http://lib.csdn.net/base/javase)项目紧张，临时介入其中。日志模块，控制端显示不同等级日志同时保存到文件中，要求每天保存一个日志文件，日志目录保留最近30天日志文件。最初没想到用库，自己采用线程池方式写日志，在写日志时先判断是否有30天前的日志删除，然后对比时间判定是否有当天日志文件，之后采用BufferedWriter写日志。写好后，客户进行代码审查，不太相信我写的效率，要求采用公共控件的方式来写日志。无奈就找到了Log4j2。
以上，无聊记录一下，本文中不会涉及太基本的配置与字段的意义，部分会较详细说明一下。
1.官网下载[Log4j包](http://www.apache.org/dyn/closer.lua/logging/log4j/2.5/apache-log4j-2.5-bin.zip)；
2.将其中的log4j-api-2.5.jar，log4j-core-2.5.jar加入工程；
3.src目录下加入log4j2.xml，配置好相关属性；
4.需用的代码中加入static Logger sLogger = LogManager.getLogger(LogManage.class.getName()); sLogger.debug(strLog);即可调用使用。
现就log4j2.xml注释说明：
```
<?xml version="1.0" encoding="UTF-8"?>  
<Configuration>  
    <Appenders>  
    <!-- 输出到控制台 -->  
        <Console name="CONSOLE" target="SYSTEM_OUT">  
            <!-- 输出格式 -->  
            <PatternLayout pattern="ROOT:%d %-5p %C %L - %m%n" />  
        </Console>  
        <!-- 按天备份一个日志 -->  
        <!-- fileName为生成的文件名，x为路径，也可以采用相对路径模式，filePattern为时间到达后产生新日志，旧日志的文件名 -->  
        <RollingFile name="TASK" fileName="x:/xxxx/logs.log"  
            filePattern="x:/xxxx/logs_%d{yyyy-MM-dd}.log" >   
            <!-- 输出格式 -->  
            <PatternLayout pattern="%d %-5p %m%n" />  
            <Policies>  
            <!-- 每1天更新一次，此处查阅网上和官方示例中，都是以小时出现，我测试是以天为单位。（官方文档中说明按item类型是否是小时，但没找到在哪里设置item类型）另有其他各类型策略，请参阅官方文档 --> 
<!-- TimeBasedTriggeringPolicy需要和filePattern配套使用，由于filePattern配置的时间最小粒度是dd天，所以表示每一天新建一个文件保存日志。SizeBasedTriggeringPolicy表示当文件大小大于指定size时，生成新的文件保存日志。 --> 
                <TimeBasedTriggeringPolicy modulate="true"  
                    interval="1" />  
                <!-- 此处为每个文件大小策略限制，使用它一般会在文件中filePattern采用%i模式 -->  
                <!-- <SizeBasedTriggeringPolicy size="128KB" /> -->  
            </Policies>  
            <!-- 最多备份30天以内的日志，此处为策略限制，Delete中可以按自己需要用正则表达式编写 -->  
        <!-- DefaultRolloverStrategy字段中加入max=“30”经测试是配合SizeBasedTriggeringPolicy限制%i的存在数量，并没有发现是网上流传的是最多保存多少个文件的限制，也或许是我写的有问题 -->  
         <DefaultRolloverStrategy>  
        <Delete basePath="x:/xxxx" maxDepth="1">  
                     <IfFileName glob="logs_*.log" />  
                     <IfLastModified age="30d" />  
            </Delete>  
            </DefaultRolloverStrategy>  
        </RollingFile>  
    </Appenders>  
    <!-- 链接到Console和RollingFile标签实现debug以上等级的输出 -->  
    <loggers>    
    <root level="debug">    
        <appender-ref ref="CONSOLE"/>    
        <AppenderRef ref="TASK" />  
    </root>    
    </loggers>    
</Configuration>
```
代码片段说明，查看注释即可：
**[java]**[view
 plain](http://blog.csdn.net/giventian/article/details/51075419#)[copy](http://blog.csdn.net/giventian/article/details/51075419#)
- import org.apache.logging.log4j.LogManager;  
- import org.apache.logging.log4j.Logger;  
- import org.dom4j.Document;  
- import org.dom4j.DocumentHelper;  
- import org.dom4j.Element;  
- 
- import taskservice.GlobalInstance;  
- 
- publicclass LogManage implements ILogManage {  
- 
- private ServiceState mServiceState;  
- privatefinal String strLogFilePath = GlobalInstance.taskServiceConfig.strLogFilePath;  
- //初始化logger
- static Logger sLogger = LogManager.getLogger(LogManage.class.getName());  
- 
- public LogManage() {  
-         mServiceState = new ServiceState();  
-     }  
- 
- @Override
- publicvoid log(int level, String strLog) {  
-         GlobalInstance.taskServiceUi.logInfo(strLog);  
- //输出日志
- switch (level) {  
- case LogLevel.DEBUG:  
-             sLogger.debug(strLog);  
- break;  
- case LogLevel.INFO:  
-             sLogger.info(strLog);  
- break;  
- case LogLevel.WARNING:  
-             sLogger.warn(strLog);  
- break;  
- case LogLevel.ERROR:  
-             sLogger.error(strLog);  
- break;  
- default:  
- break;  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
工程配置如图所示：
以上，主要为实现日志按日期保存，并只保留最近30天的日志实现方式。
