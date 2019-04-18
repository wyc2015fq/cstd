# log4j2 不使用配置文件，动态生成logger对象 - z69183787的专栏 - CSDN博客
2017年08月29日 21:00:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1394
[http://www.cnblogs.com/0201zcr/p/5726072.html](http://www.cnblogs.com/0201zcr/p/5726072.html)
官方：[http://logging.apache.org/log4j/2.x/manual/customconfig.html](http://logging.apache.org/log4j/2.x/manual/customconfig.html)
大家平时使用Log4j一般都是在classpath下放置一个log4j的配置文件，比如log4j.xml，里面配置好Appenders和Loggers，但是前一阵想做某需求的时候，想要的效果是每一个任务都要有一个单独的日志文件记录下来，比如job.001.log,job.002.log这种，这种完全不能使用配置文件来设置。
整体架构：
![](http://images2015.cnblogs.com/blog/731178/201608/731178-20160801161248637-940760805.jpg)
### Log4j的组成：
  Log4j由三个重要的组成构成：日志记录器(Loggers)，输出端(Appenders)和日志格式化器(Layout)。
1.日志记录器(Loggers)：控制要输出哪些日志记录语句，对日志信息进行级别限制。
2.输出端(Appenders)：指定了日志将打印到控制台还是文件中。
3.日志格式化器(Layout)：控制日志信息的显示格式。
　　于是点开Log4j的源码，研究了一番，发现使用Log4j的底层代码完全可以动态生成新的Logger，比较关键的类有
```
org.apache.logging.log4j.core.Layout负责格式
org.apache.logging.log4j.core.Appender就是配置文件里的Appender，我们使用FileAppender，也可以使用RollingFileAppender，
org.apache.logging.log4j.core.config.Configuration和org.apache.logging.log4j.core.LoggerContext是负责Log4j的配置的。
```
#### LoggerContext
　　在日志系统中，LoggerContext扮演者重要的角色。然而，根据实际情况，一个应用中可能存在多个有效的LoggerContext。
#### Configuration　　
　　每一个LoggerContext都有一个有效的Configuaration，它包含了**所有的**Appenders、context-wide Filters、LoggerConfigs以及对StrSubstitutor的引用。在重新配置期间，两个Configuaration会同时存在；一旦日志器被重新赋予新的Configuaration，旧的Configuaration就会停止工作并丢弃。
#### LoggerConfig
　　LoggerConfig将会在Loggers在logging configuration中被声明的时候创建。在LoggerConfig 拥有一列类的过滤器（Filter)，这些过滤器将会过来所有的记录日志的事件，只有符合要求的日志才会被传递到Appenders。因为LoggerConfig需要将事件传递给Appenders，所以它拥有一系列Appenders的引用。
#### Appender
　　 根据logger请求选择去接受或者拒绝该只是他们的一个能力。Log4j允许日志打印服务打印到多个目的地上。在Log4j的说法是一个输出的目的地对应着一个Appdender。现在的Appender允许是console, files, remote socket servers, Apache Flume, JMS, remote UNIX Syslog daemons, and various database APIs。
　　是在Log4j2.3下测试通过的。注意在不使用Logger的时候一定要调用stop方法将Logger移除，不然我自己测试生成7000多个Logger后便会报错open too many file。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
**import org.apache.logging.log4j.Level;*import org.apache.logging.log4j.LogManager;import org.apache.logging.log4j.core.Appender;import org.apache.logging.log4j.core.Layout;import org.apache.logging.log4j.core.LoggerContext;import org.apache.logging.log4j.core.appender.FileAppender;import org.apache.logging.log4j.core.config.AppenderRef;import org.apache.logging.log4j.core.config.Configuration;import org.apache.logging.log4j.core.config.LoggerConfig;import org.apache.logging.log4j.core.layout.PatternLayout;import org.slf4j.Logger;import org.slf4j.LoggerFactory;/** *  */public class JobLogFactory {  private JobLogFactory() {  }  public static void start(int jobId) {　　 //为false时，返回多个LoggerContext对象，   true：返回唯一的单例LoggerContext　　 final LoggerContext ctx = (LoggerContext) LogManager.getContext(false);    final Configuration config = ctx.getConfiguration();　　 //创建一个展示的样式：PatternLayout，   还有其他的日志打印样式。    Layout layout = PatternLayout.createLayout(PatternLayout.DEFAULT_CONVERSION_PATTERN,         config, null, null, true, false, null, null);    //TriggeringPolicy tp = SizeBasedTriggeringPolicy.createPolicy("10MB");    //Appender appender = RollingFileAppender.createAppender(String.format("    // logs/test/syncshows-job-%s.log", jobID),     //       "/logs/test/" + jobID + "/syncshows-job-" + jobID + ".log.gz",     //       "true", jobID, null, null, null, tp, null, layout, null,     //       null, null, null, config);　　 //  日志打印方式——输出为文件　　　Appender appender = FileAppender.createAppender(        String.format("logs/test/syncshows-job-%s.log", jobId), "true", "false",         "" + jobId, null, "true", "true", null, layout, null, null, null, config);    appender.start();    config.addAppender(appender);   AppenderRef ref = AppenderRef.createAppenderRef("" + jobId, null, null);    AppenderRef[] refs = new AppenderRef[]{ref};    LoggerConfig loggerConfig = LoggerConfig.createLogger("false", Level.ALL, "" + jobId,        "true", refs, null, config, null);    loggerConfig.addAppender(appender, null, null);    config.addLogger("" + jobId, loggerConfig);    ctx.updateLoggers();  }  public static void stop(int jobId) {    final LoggerContext ctx = (LoggerContext) LogManager.getContext(false);    final Configuration config = ctx.getConfiguration();    config.getAppender("" + jobId).stop();    config.getLoggerConfig("" + jobId).removeAppender("" + jobId);    config.removeLogger("" + jobId);    ctx.updateLoggers();  }  /**   * 获取Logger   *   * 如果不想使用slf4j,那这里改成直接返回Log4j的Logger即可   * @param jobId   * @return   */  public static Logger createLogger(int jobId) {    start(jobId);    return LoggerFactory.getLogger("" + jobId);  }}*
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
测试类：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import org.slf4j.Logger;
/**
 * 
simple test
 */
public class LoggerTest {
  public static void main(String[] args) {
    for(int i=0;i<50000;i++) {
      Logger logger = JobLogFactory.createLogger(i);
      logger.info("Testing testing testing 111");
      logger.debug("Testing testing testing 222");
      logger.error("Testing testing testing 333");
      JobLogFactory.stop(i);
    }
  }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
