# log4j 不同模块输出到不同的文件 - z69183787的专栏 - CSDN博客
2017年08月29日 20:23:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：536
[http://www.cnblogs.com/0201zcr/p/5725508.html](http://www.cnblogs.com/0201zcr/p/5725508.html)
1、实现目标
　　不同业务的日志信息需要打印到不同的文件中，每天或者每个小时生成一个文件。如，注册的信息打印到register.log，每天凌晨生成一个register-年月日.log文件， 登录信息的日志打印到一个login.log文件中，login-年月日.log。
整体架构：
![](http://images2015.cnblogs.com/blog/731178/201608/731178-20160801143124903-1379251454.png)
2、maven配置
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>test</groupId>
    <artifactId>common</artifactId>
    <version>1.0-SNAPSHOT</version>
    <dependencies>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-api</artifactId>
            <version>1.7.6</version>
        </dependency>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-log4j12</artifactId>
            <version>1.7.6</version>
        </dependency>
    </dependencies>
</project>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
3、配置文件
　　默认情况下，log4j会解析*src/main/resources/log4j.properties文件，下面是log4.properties文件中的内容*####父级Logger*log4j.rootCategory=info,stdout*
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
**### 子Logger是否会中父级Logger(上面设置的log4j.rootCategory)的appender里面输出,默认是true### 为true时:子级别的Logger输出后还会在父级Logger里面输出### 为false时:log日志只会在当前级别的Logger里面输出,不会在父级Logger里面输出log4j.addivity.org.apache=truelogdir=/Users/zhengcanrui/WORK/test/logs  log4j.appender.stdout=org.apache.log4j.ConsoleAppenderlog4j.appender.stdout.layout=org.apache.log4j.PatternLayoutlog4j.appender.stdout.layout.ConversionPattern=%p\:%d{yyyyMMdd HH\:mm\:ss} %c\: %m%n#########自定义日志输出##########  log4j.appender.file=org.apache.log4j.DailyRollingFileAppenderlog4j.appender.file.File=${logdir}/file.loglog4j.appender.file.layout=org.apache.log4j.PatternLayoutlog4j.appender.file.layout.ConversionPattern=%d-[TS] %p %t %c - %m%n##register模块输出 #### 用来控制过来日志信息,如:下面至少是INFO的信息才会输出到register文件中log4j.logger.register=INFO,register    log4j.appender.register=org.apache.log4j.DailyRollingFileAppender   log4j.appender.register.File=${logdir}/register.loglog4j.appender.register.DatePattern='_'yyyy-MM-dd'.log'log4j.appender.register.layout=org.apache.log4j.PatternLayout  log4j.appender.register.layout.ConversionPattern=%d %p [%c %L %l] - %m%nlog4j.additivity.register=true  ##login模块输出 #INFO及以上几倍的log进行输出log4j.logger.login=INFO,login #以文件类型输出   log4j.appender.login=org.apache.log4j.DailyRollingFileAppender   #输出路径log4j.appender.login.File=${logdir}/login.log#配置文件输出的文件命名，这种格式文件会在凌晨生成一个文件，想在其他时间生成新文件可以查看DatePattern的相关配置log4j.appender.login.DatePattern='_'yyyy-MM-dd'.log'#输出格式log4j.appender.login.layout=org.apache.log4j.PatternLayout  log4j.appender.login.layout.ConversionPattern=%d %p [%c] - %m%n# 设置这个子Logger输出日志不在父级别logger里面输出log4j.additivity.login=false  # 开源库的日志输出控制# logger for spring   log4j.logger.org.springframework=error    # logger for MemCached  log4j.logger.com.danga.MemCached=error    # logger for c3p0  log4j.logger.com.mchange=error      org.apache.commons.httpclient=error    org.apache.http=error  **
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
几个需要注意的属性：
　　log4j.additivity-模块名：设置子logger是否在父级Logger中输出。
　　模块名的命名：log4j.appender.模块名.属性。
　　配置文件中的各个属性意义，可以查看：[http://www.cnblogs.com/0201zcr/p/4761505.html](http://www.cnblogs.com/0201zcr/p/4761505.html)
Logger类是日志包的核心，Logger的名称是大小写敏感的，并且名称之间有继承关系。子名由父名做前缀，用点号“.”分隔，如x.y是x.y.z的父亲Logger。
Logger系统中有个根logger，是所有logger的祖先，它总是存在的，并且不可以通过名字获取，可以通过Logger.getRootLogger()来获取。
4、测试类
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import org.apache.log4j.Logger;
import org.slf4j.LoggerFactory;
/**
 * Created by zhengcanrui on 16/7/27.
 */
public class LoggerUtil {
   private static final Logger file = Logger.getLogger("file");
    private static final Logger register = Logger.getLogger("register");
    private static final Logger login = Logger.getLogger("login");
    private static final Logger goldcoin = Logger.getLogger("goldcoin");
    private static final Logger recharge = Logger.getLogger("recharge");
    private static final Logger jjj = Logger.getLogger(LoggerUtil.class.getName());
    private static final Logger FILE = Logger.getLogger("appender1");
    private static org.apache.log4j.Logger log = Logger.getLogger(LoggerUtil.class);
    public static void logInfo(String log) {
        file.info(log);
    }
    public static void registerInfo() {
        register.info("[register] ddd " );
    }
    public static void loginInfo() {
        login.info("[login] 222" );
    }
    public static void main(String[] args) {
        /*logInfo("11");
        registerInfo();
        loginInfo();*/
        /*login.info("[login] 大大大大大大大大");
        register.debug("2222");
        register.info("[register] 人人人人人人人人人人");*/
        //jjj.info("test");
        //log.info(222);
        FILE.info("334343");
        register.info("2222");
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　结果：
1）控制台
```
INFO:20160801 12:27:57 appender1: 334343
INFO:20160801 12:27:57 register: 2222
```
2）生成文件
![](http://images2015.cnblogs.com/blog/731178/201608/731178-20160801122733325-469215231.png)
3）文件中内容
![](http://images2015.cnblogs.com/blog/731178/201608/731178-20160801131943512-1435156012.png)
