# log4j输出多个自定义日志文件，动态配置路径 - z69183787的专栏 - CSDN博客
2015年07月03日 08:11:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3108
1.    log4j输出多个自定义日志文件
 log4j的强大功能无可置疑，但实际应用中免不了遇到某个功能需要输出独立的日志文件的情况，怎样才能把所需的内容从原有日志中分离，形成单独的日志文件呢？其实只要在现有的log4j基础上稍加配置即可轻松实现这一功能。
　　先看一个常见的log4j.properties文件，它是在控制台和myweb.log文件中记录日志：
log4j.rootLogger=DEBUG, stdout, logfile
log4j.category.org.springframework=ERROR
log4j.category.org.apache=INFO
log4j.appender.stdout=org.apache.log4j.ConsoleAppender
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern=%d %p [%c] - %m%n
log4j.appender.logfile=org.apache.log4j.RollingFileAppender
log4j.appender.logfile.File=${myweb.root}/WEB-INF/log/myweb.log
log4j.appender.logfile.MaxFileSize=512KB
log4j.appender.logfile.MaxBackupIndex=5
log4j.appender.logfile.layout=org.apache.log4j.PatternLayout
log4j.appender.logfile.layout.ConversionPattern=%d %p [%c] - %m%n
如果想对不同的类输出不同的文件(以cn.com.Test为例)，先要在Test.java中定义:
private static Log logger = LogFactory.getLog(Test.class);
　　然后在log4j.properties中加入:
log4j.logger.cn.com.Test= DEBUG, test
log4j.appender.test=org.apache.log4j.FileAppender
log4j.appender.test.File=${myweb.root}/WEB-INF/log/**test.log**
log4j.appender.test.layout=org.apache.log4j.PatternLayout
log4j.appender.test.layout.ConversionPattern=%d %p [%c] - %m%n
　　也就是让cn.com.Test中的logger使用log4j.appender.test所做的配置。
　　但是，如果在同一类中需要输出多个日志文件呢？其实道理是一样的，先在Test.java中定义:
private static Log logger1 = LogFactory.getLog("myTest1");
private static Log logger2 = LogFactory.getLog("myTest2");
　　然后在log4j.properties中加入:
log4j.logger.myTest1= DEBUG, test1
log4j.appender.test1=org.apache.log4j.FileAppender
log4j.appender.test1.File=${myweb.root}/WEB-INF/log/**test1.log**
log4j.appender.test1.layout=org.apache.log4j.PatternLayout
log4j.appender.test1.layout.ConversionPattern=%d %p [%c] - %m%n
log4j.logger.myTest2= DEBUG, test2
log4j.appender.test2=org.apache.log4j.FileAppender
log4j.appender.test2.File=${myweb.root}/WEB-INF/log/**test2.log**
log4j.appender.test2.layout=org.apache.log4j.PatternLayout
log4j.appender.test2.layout.ConversionPattern=%d %p [%c] - %m%n
　　也就是在用logger时给它一个自定义的名字(如这里的"myTest1")，然后在log4j.properties中做出相应配置即可。别忘了不同日志要使用不同的logger(如输出到**test1.log**的要用**logger1**.info("abc"))。
　　还有一个问题，就是这些自定义的日志默认是同时输出到log4j.rootLogger所配置的日志中的，如何能只让它们输出到自己指定的日志中呢？别急，这里有个开关：
log4j.**additivity**.myTest1 = false
　　它用来设置是否同时输出到log4j.rootLogger所配置的日志中，设为false就不会输出到其它地方啦！注意这里的"myTest1"是你在程序中给logger起的那个自定义的名字！
如果你说，我只是不想同时输出这个日志到log4j.rootLogger所配置的logfile中，stdout里我还想同时输出呢！那也好办，把你的log4j.logger.myTest1 = DEBUG, test1改为下式就OK啦！
log4j.logger.myTest1=DEBUG, test1, **stdout**
2.动态配置路径
若程序需要的日志路径需要不断的变化，而又不可能每次都去改配置文件，那就要采取两种方法。
第一种
        log4j的配置文件支持windows的环境变量，格式类似velocity:${env}，那我们就用环境变量表示可能会变化的路径。上文已使用“log4j.appender.test1.File=${myweb.root}/WEB-INF/log/**test1.log**”。
第二种
       这种方法是不用配置文件，而是在程序里用代码配置，代码是活的，所以路径肯定可以写活。示例如下：
**[java]**[view plain](http://blog.csdn.net/wiwipetter/article/details/4390579#)[copy](http://blog.csdn.net/wiwipetter/article/details/4390579#)
- Logger myTest = Logger.getLogger("myTest");  
- 
- Layout layout = new PatternLayout("%d %p [%c] - %m%n");  
- 
- Appender appender = new FileAppender(layout, logFilePath);  
- 
- myTest.addAppender(appender);  
 附：ConversionPattern参数的格式含义
%c 输出日志信息所属的类的全名
%d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyy-MM-dd HH:mm:ss }，输出类似：2002-10-18- 22：10：28
%f 输出日志信息所属的类的类名
%l 输出日志事件的发生位置，即输出日志信息的语句处于它所在的类的第几行
%m 输出代码中指定的信息，如log(message)中的message
%n 输出一个回车换行符，Windows平台为“/r/n”，Unix平台为“/n”
%p 输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL。如果是调用debug()输出的，则为DEBUG，依此类推
%r 输出自应用启动到输出该日志信息所耗费的毫秒数
%t 输出产生该日志事件的线程名
