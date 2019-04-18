# LOG4J2 使用示例 - z69183787的专栏 - CSDN博客
置顶2017年06月14日 11:10:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1507
pom：
```
<slf4j.version>1.7.10</slf4j.version>
        <log4j2.version>2.3</log4j2.version>
        <disruptor.version>3.3.4</disruptor.version>
```
```
<!-- log4j2-->
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-api</artifactId>
            <version>${slf4j.version}</version>
        </dependency>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>jcl-over-slf4j</artifactId>
            <version>${slf4j.version}</version>
        </dependency>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>jul-to-slf4j</artifactId>
            <version>${slf4j.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-api</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-core</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-slf4j-impl</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-1.2-api</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>com.lmax</groupId>
            <artifactId>disruptor</artifactId>
            <version>${disruptor.version}</version>
        </dependency>
        <!-- log4j2-->
```
log4j2.xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="WARN" >
    <properties>
        <property name="LOG_HOME">/data/applogs/emidas-coupon-service</property>
    </properties>
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n" />
        </Console>
        <RollingRandomAccessFile name="infoLog" fileName="${LOG_HOME}/app.log"
                                 filePattern="${LOG_HOME}/info.%d{yyyy-MM-dd}.log.gz" append="true">
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Filters>
                <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
            </Filters>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="warnLog" fileName="${LOG_HOME}/warn.log"
                                 filePattern="${LOG_HOME}/warn.%d{yyyy-MM-dd}.log.gz" append="true">
            <Filters>
                <ThresholdFilter level="error" onMatch="DENY" onMismatch="NEUTRAL"/>
                <ThresholdFilter level="warn" onMatch="ACCEPT" onMismatch="DENY"/>
            </Filters>
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="errorLog" fileName="${LOG_HOME}/error.log"
                                 filePattern="${LOG_HOME}/error.%d{yyyy-MM-dd}.log.gz" append="true">
            <Filters>
                <ThresholdFilter level="ERROR" onMatch="ACCEPT" onMismatch="DENY"/>
            </Filters>
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
        </RollingRandomAccessFile>
        <!-- synchronize -->
        <RollingRandomAccessFile name="synchronizeLog" fileName="${LOG_HOME}/synchronize.log"
                                 filePattern="${LOG_HOME}/synchronize.%d{yyyy-MM-dd}-%i.log.gz" append="true">
            <PatternLayout pattern="[emidas-lottery-job][%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Filters>
                <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
            </Filters>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
            <DefaultRolloverStrategy max="30"/>
        </RollingRandomAccessFile>
        <Cat name="catAppender">
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
        </Cat>
    </Appenders>
    <Loggers>
        <!-- 第三方的软件日志级别 -->
        <AsyncLogger name="org.springframework" level="info" additivity="true">
            <AppenderRef ref="warnLog"/>
            <AppenderRef ref="errorLog"/>
        </AsyncLogger>
        <!-- 应用的日志级别 -->
        <AsyncLogger name="com.xxxx.emidas.coupon" level="info" additivity="true">
            <AppenderRef ref="infoLog"/>
            <AppenderRef ref="warnLog"/>
            <AppenderRef ref="errorLog"/>
        </AsyncLogger>
// includeLocation="true" 异步日志中加入类名及行数等详细信息（性能会下降），默认false
        <AsyncLogger name="synchronize" level="info" additivity="false"  includeLocation="true">
            <AppenderRef ref="synchronizeLog"/>
            <AppenderRef ref="Console" />
        </AsyncLogger>
        <!--<logger name="java.sql.PreparedStatement" level="debug" additivity="true">-->
            <!--<AppenderRef ref="Console"/>-->
        <!--</logger>-->
        <!--<logger name="com.xxxx.emidas.coupon.dao.mapper" level="debug" additivity="false">-->
            <!--<AppenderRef ref="Console"/>-->
        <!--</logger>-->
        <!-- 剩余其他的日志级别 -->
        <asyncRoot level="info" includeLocation="true">
            <AppenderRef ref="infoLog" />
            <AppenderRef ref="Console" />
            <AppenderRef ref="catAppender"/>
        </asyncRoot>
    </Loggers>
</Configuration>
```
web.xml 配置：
```
<!--由Sprng载入的Log4j配置文件位置 -->
    <context-param>
        <param-name>log4jConfigLocation</param-name>
        <param-value>classpath:log4j2.xml</param-value>
    </context-param>
    <!--Spring默认刷新Log4j配置文件的间隔,单位为毫秒 -->
    <context-param>
        <param-name>log4jRefreshInterval</param-name>
        <param-value>60000</param-value>
    </context-param>
```
```
<listener>
        <listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
    </listener>
```
使用工具类：
```java
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
public class LogUtil {
    /**
     * 获得Logger
     * @param clazz 日志发出的类
     * @return Logger
     */
    public static Logger get(Class<?> clazz) {
        return LogManager.getLogger(clazz);
    }
    /**
     * 获得Logger
     * @param name 自定义的日志发出者名称
     * @return Logger
     */
    public static Logger get(String name) {
        return LogManager.getLogger(name);
    }
    /**
     * @return 获得日志，自动判定日志发出者
     */
    public static Logger get() {
        StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
        return LogManager.getLogger(stackTrace[2].getClassName());
    }
    //----------------------------------------------------------- Logger method start
    //------------------------ Trace
    /**
     * Trace等级日志，小于debug<br>
     * 由于动态获取Logger，效率较低，建议在非频繁调用的情况下使用！！
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void trace(String format, Object... arguments) {
        trace(innerGet(), format, arguments);
    }
    /**
     * Trace等级日志，小于Debug
     * @param log 日志对象
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void trace(Logger log, String format, Object... arguments) {
        log.trace(format, arguments);
    }
    //------------------------ debug
    /**
     * Debug等级日志，小于Info<br>
     * 由于动态获取Logger，效率较低，建议在非频繁调用的情况下使用！！
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void debug(String format, Object... arguments) {
        debug(innerGet(), format, arguments);
    }
    /**
     * Debug等级日志，小于Info
     * @param log 日志对象
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void debug(Logger log, String format, Object... arguments) {
        log.debug(format, arguments);
    }
    //------------------------ info
    /**
     * Info等级日志，小于Warn<br>
     * 由于动态获取Logger，效率较低，建议在非频繁调用的情况下使用！！
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void info(String format, Object... arguments) {
        info(innerGet(), format, arguments);
    }
    /**
     * Info等级日志，小于Warn
     * @param log 日志对象
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void info(Logger log, String format, Object... arguments) {
        log.info(format, arguments);
    }
    //------------------------ warn
    /**
     * Warn等级日志，小于Error<br>
     * 由于动态获取Logger，效率较低，建议在非频繁调用的情况下使用！！
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void warn(String format, Object... arguments) {
        warn(innerGet(), format, arguments);
    }
    /**
     * Warn等级日志，小于Error
     * @param log 日志对象
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void warn(Logger log, String format, Object... arguments) {
        log.warn(format, arguments);
    }
    /**
     * Warn等级日志，小于Error<br>
     * 由于动态获取Logger，效率较低，建议在非频繁调用的情况下使用！！
     * @param e 需在日志中堆栈打印的异常
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void warn(Throwable e, String format, Object... arguments) {
        warn(innerGet(), e, format(format, arguments));
    }
    /**
     * Warn等级日志，小于Error
     * @param log 日志对象
     * @param e 需在日志中堆栈打印的异常
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void warn(Logger log, Throwable e, String format, Object... arguments) {
        log.warn(format(format, arguments), e);
    }
    //------------------------ error
    /**
     * Error等级日志<br>
     * 由于动态获取Logger，效率较低，建议在非频繁调用的情况下使用！！
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void error(String format, Object... arguments) {
        error(innerGet(), format, arguments);
    }
    /**
     * Error等级日志<br>
     * @param log 日志对象
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void error(Logger log, String format, Object... arguments) {
        log.error(format, arguments);
    }
    /**
     * Error等级日志<br>
     * 由于动态获取Logger，效率较低，建议在非频繁调用的情况下使用！！
     * @param e 需在日志中堆栈打印的异常
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void error(Throwable e, String format, Object... arguments) {
        error(innerGet(), e, format(format, arguments));
    }
    /**
     * Error等级日志<br>
     * 由于动态获取Logger，效率较低，建议在非频繁调用的情况下使用！！
     * @param log 日志对象
     * @param e 需在日志中堆栈打印的异常
     * @param format 格式文本，{} 代表变量
     * @param arguments 变量对应的参数
     */
    public static void error(Logger log, Throwable e, String format, Object... arguments) {
        log.error(format(format, arguments), e);
    }
    //----------------------------------------------------------- Logger method end
    //----------------------------------------------------------- Private method start
    /**
     * 格式化文本
     * @param template 文本模板，被替换的部分用 {} 表示
     * @param values 参数值
     * @return 格式化后的文本
     */
    private static String format(String template, Object... values) {
        return String.format(template.replace("{}", "%s"), values);
    }
    /**
     * @return 获得日志，自动判定日志发出者
     */
    private static Logger innerGet() {
        StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
        return LogManager.getLogger(stackTrace[3].getClassName());
    }
    //----------------------------------------------------------- Private method end
}
```
使用：
1、根据类名
```java
private static final Logger logger = LogUtil.get(AuthAspect.class);
logger.info("Authority check failed :appName = {} , method = {} ",appName,joinPoint.getSignature().getName());
```
2、根据日志Logger配置
```java
private static final Logger logger = LogUtil.get("synchronize");
logger.info("Synchronize failed || packageId already exists ,packageId = {}",packageId);
```
