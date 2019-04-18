# Log4j2 使用自定义Logger， Log中打印不出类名 - z69183787的专栏 - CSDN博客
2017年08月29日 15:06:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5448
2017-08-29更新：
20:02：
 <AsyncLogger name="lottery" level="info" additivity="false" includeLocation="true">
            <AppenderRef ref="lotteryLog"/>
        </AsyncLogger>
使用异步日志需要加上  includeLocation="true" ，解决没有类名和行数之类的 日志详细信息的问题
但据官网描述，这样好像就丧失异步性能方面的优势了 http://logging.apache.org/log4j/2.x/manual/appenders.html
Extracting location is an expensive operation (it can make logging 5 - 20 times slower). T
o improve performance, location is not included by default when adding a log event to the queue. 
You can change this by setting includeLocation="true".
发现时 异步Logger的问题，，
换成<Logger name="lottery" level="info" additivity="false">
            <AppenderRef ref="lotteryLog"/>
        </Logger>
就好了。。。
发现getLogger（）传入class，始终能打出类名
若直接使用 传入 LogManager.getLogger( class name） 时，也可以打出，但使用包装类嵌套一层就无法打出
LogUtil.get(class name)
public static Logger get(String name) {
        return LogManager.getLogger(name);
    }
。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
如果想看到具体类和行数的话，还是传入 类 Class 吧，然后将 自定义logger的name 更改为 对应的包名 或者类名
即将需要 统一记录的 类放置在同一个包下，使用xxxx.xxx 包名进行 logger的定义，由于父关系，则会被一起打出
配置文件：
```
<RollingRandomAccessFile name="lotteryLog" fileName="${LOG_HOME}/lottery.log"
                                 filePattern="${LOG_HOME}/lottery.%d{yyyy-MM-dd}-%i.log.gz" append="true">
            <PatternLayout pattern="[main][%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Filters>
                <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
            </Filters>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
            <DefaultRolloverStrategy max="30"/>
        </RollingRandomAccessFile>
```
```
<AsyncLogger name="lottery" level="info" additivity="false">
            <AppenderRef ref="lotteryLog"/>
        </AsyncLogger>
```
java：
```java
private final static Logger logger = LogUtil.get(DrawPrizeRemoteServiceImpl.class);
    private final static Logger log2 = LogManager.getLogger("lottery");
    private static final org.slf4j.Logger log = LoggerFactory.getLogger("lottery");
```
均可以打印日志文件，但发现除了 getLogger方法传入是 类 或者 传入的类名在 log4j配置文件中 能找到匹配的logger 的之外，
其余打印出的日志文件 在 类名 这块（%class） 和行数（%line） 都是 问号
[main][2016-12-27 00:00:00.003][Pigeon-Server-Request-Processor-34-thread-49][INFO][?][] XXXXXXX
当使用类做参数或有完整类名匹配的logger时，才会打出完整信息
[2017-08-29 14:13:26.630][Pigeon-Server-Request-Processor-34-thread-3][INFO][xxxxxxxx.DrawPrizeRemoteServiceImpl][89]:dasdsadasdasdasd
