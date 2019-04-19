# Log4j配置与使用 - 零度的博客专栏 - CSDN博客
2016年04月27日 13:48:32[零度anngle](https://me.csdn.net/zmx729618)阅读数：447
1、Log4j的思想
Log4j真的很简单，简单到令人发指的地步。不是要记录日志吗？那就给你一个Log，然后你用Log来写东西就行了，先来一个完整类示例：
**package** test;
**import** org.apache.commons.logging.Log;
**import** org.apache.commons.logging.LogFactory;
**publicclass** Test {
**static** Log *log* = LogFactory.*getLog*(Test.**class**);
**publicvoid** log(){
*log*.debug("Debug info.");
*log*.info("Info info");
*log*.warn("Warn info");
*log*.error("Error info");
*log*.fatal("Fatal info");
        }
/**
***@param**args
*/
**publicstaticvoid** main(String[] args) {
           Test test = **new** Test();
           test.log();
        }
}
别怕，看完这篇文章你就会觉得很简单了。
       Log4j默认把日志信息分为五个等级
       debug < info < warn < error < fatal
虽然可以自己添加等级，但是我觉得没有必要，五个够用了吧！你要写入信息的时候就把信息归为五个等级中的一个，然后调用相应的函数即可。 
       分五个等级到底有什么用呢？日志信息到底写到哪里去了？
       “LogFactory.*getLog*(Test.**class**)”又是什么意思捏？接着往下看吧！
Log4j的关键之处在于它的继承思想。也就是一个Log可以继承另外一个Log的属性（输出到哪里，日志等级，日志格式等等）。怎么继承？
Log4j是根据Log的名字来判断继承关系的，比如：
名字为“com.zhlmmc.lib”的Log就是“com.zhlmmc.lib.log”的parent，明白了吧！Log4j还有一个rootLogger，相当于Java的Object。
回过头来看“LogFactory.*getLog*(Test.**class**)”这里的“Test.**class**”事实上传进去的是Test这个类的完整路径（包名+类名），“test.Test”。这样如果存在“test”这个Log那么Test这个Log就继承它，否则就继承rootLogger。那具体的Log属性是在哪里定义的呢？
2、常见的配置文件
虽然可以用xml或者在运行时用Java来配置Log4j，但还是properties文件好用啊！
log4j.rootLogger=info,stdout
log4j.appender.stdout=org.apache.log4j.ConsoleAppender
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
# Pattern to output the caller's file name and line number.
log4j.appender.stdout.layout.ConversionPattern=%5p[%t](%F:%L)-%m%n
分析一下：
第一行，配置log4j.rootLogger你明白吧。应为它是根，总得配置一下，否则别的Log继承什么啊。其他的Log可以配置也可以不配置。等号后面的第一个参数表示日志级别，可以填五个级别中的一种，后面的参数都是让Log知道输出到哪里，如果你想让日志输出到两个地方就加两个输出参数，比如：
log4j.rootLogger=info,stdout, file
这里的info表示，该Log的日志级别为info，所有级别小于info的日志都不会被记录。比如使用这个配置文件的话，我刚开始举的那个类中
*log*.debug("Debug info.");
这句话是不起作用的，因为debug的级别小于info。这样就很容易控制什么信息在调试的时候要显示，什么信息在发布的时候要去掉。这些都不用改代码，很方便吧。
但，stdout和file又是什么呢？
接着往下看，就是配置stdout了，这个名字是随便取的，你可以叫它A：
log4j.appender.A=org.apache.log4j.ConsoleAppender
那么上面的rootLogger的参数stdout也要改成A，其他用到的地方当然也要改。这里的关键不是名字，而是appender类型，比如这里的“ConsoleAppender”，看明白了吧，输出到Console。
配置日志信息输出目的地Appender，其语法为
  log4j.appender.appenderName = fully.qualified.name.of.appender.class
   log4j.appender.appenderName.option = valueN 
Log4j提供的appender有以下几种：
  org.apache.log4j.ConsoleAppender（控制台）
  org.apache.log4j.FileAppender（文件）
  org.apache.log4j.DailyRollingFileAppender（每天产生一个日志文件）
  org.apache.log4j.RollingFileAppender（文件大小到达指定尺寸的时候产生新文件）
  org.apache.log4j.WriterAppender（将日志信息以流格式发送到任意指定的地方）
配置日志信息的格式（布局），其语法为：
  log4j.appender.appenderName.layout = fully.qualified.name.of.layout.class
   log4j.appender.appenderName.layout.option = valueN 
Log4j提供的layout有以下几种：
  org.apache.log4j.HTMLLayout（以HTML表格形式布局），
  org.apache.log4j.PatternLayout（可以灵活地指定布局模式），
  org.apache.log4j.SimpleLayout（包含日志信息的级别和信息字符串），
  org.apache.log4j.TTCCLayout（包含日志产生的时间、线程、类别等等信息） 
Log4J采用类似C语言中的printf函数的打印格式格式化日志信息，打印参数如下： 
 %m 输出代码中指定的消息
%p 输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL 
　　%r 输出自应用启动到输出该log信息耗费的毫秒数 
　　%c 输出所属的类目，通常就是所在类的全名 
　　%t 输出产生该日志事件的线程名 
　　%n 输出一个回车换行符，Windows平台为“\r\n”，Unix平台为“\n” 
　　%d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyy MMM dd HH:mm:ss,SSS}，输出类似： 2002年10月18日 22：10：28，921 
　　%l 输出日志事件的发生位置，包括类目名、发生的线程，以及在代码中的行数。举例：Testlog4.main(TestLog4.java:10)
在实际的项目开发中，很可能遇到所引用的包用Log4j来记录日志，比如Hibernate。那么在这里你可以很容易的控制这个包如何记录日志。比如在上面的配置文件中加一行：
log4j.logger.org.hibernate=fatal
那么，所有org.hibernate包下面的类就只会显示很少的信息，因为fatal的级别最高啊。
log4j配置实例：使用properties
# Output pattern : date [thread] priority category - message
log4j.rootLogger=WARN, Console, RollingFile
#Console
log4j.appender.Console=org.apache.log4j.ConsoleAppender
log4j.appender.Console.layout=org.apache.log4j.PatternLayout
log4j.appender.Console.layout.ConversionPattern=%d [%t] %-5p [%c] - %m%n
#RollingFile
log4j.appender.RollingFile=org.apache.log4j.DailyRollingFileAppender
log4j.appender.RollingFile.File=logs/lms.log
log4j.appender.RollingFile.layout=org.apache.log4j.PatternLayout
log4j.appender.RollingFile.layout.ConversionPattern=%d [%t] %-5p [%c] - %m%n
#Project defalult level
log4j.logger.org.hibernate=INFO
log4j2配置实例：使用xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration status="error">    
  <appenders>    
    <Console name="Console" target="SYSTEM_OUT">    
        <ThresholdFilter level="trace" onMatch="ACCEPT" onMismatch="DENY"/>    
        <PatternLayout pattern="%d{HH:mm:ss.SSS} %-5level %class{36} %L %M - %msg%xEx%n"/>   
    </Console> 
    <File name="Filelog" fileName="/logs/lms.log" append="false">    
        <PatternLayout pattern="%d{HH:mm:ss.SSS} %-5level %class{36} %L %M - %msg%xEx%n"/>   
    </File> 
    <RollingFile name="RollingFile" fileName="/logs/app.log"  filePattern="logs/$${date:yyyy-MM}/app-%d{MM-dd-yyyy}-%i.log.gz">   
      <PatternLayout pattern="%d{yyyy.MM.dd 'at' HH:mm:ss z} %-5level %class{36} %L %M - %msg%xEx%n"/>   
      <SizeBasedTriggeringPolicy size="500 MB" />    
    </RollingFile> 
  </appenders>    
  <loggers>    
    <root level="trace">    
      <appender-ref ref="RollingFile"/>    
      <appender-ref ref="Filelog"/>    
      <appender-ref ref="Console"/>    
    </root>
  </loggers>    
</configuration> 
3、使用
      别怕，这可不是部署Tomcat。把log4j的包和commons-logging的包（log4j需要commons-logging加在一起才两个）放到classpath下面。然后把配置文件保存为log4j.properties，也放在classpath下面（如果用Eclipse的话，放在src目录下即可）。然后你就可以跑了。
