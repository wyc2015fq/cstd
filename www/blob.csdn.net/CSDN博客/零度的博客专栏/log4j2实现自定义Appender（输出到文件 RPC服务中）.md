# log4j2实现自定义Appender（输出到文件/RPC服务中） - 零度的博客专栏 - CSDN博客
2017年09月25日 10:15:40[零度anngle](https://me.csdn.net/zmx729618)阅读数：761
### 1、背景
虽然log4j很强大，可以将日志输出到文件、DB、ES等。但是有时候确难免完全适合自己，此时我们就需要自定义Appender，使日志输出到指定的位置上。
本文，将通过两个例子说明自定义APPender，一个是将日志写入文件中，另一个是将日志发送到远程Thrift服务中。
**本文代码详见：[https://github.com/hawkingfoo/log-demo](https://github.com/hawkingfoo/log-demo)**
### 2、自定义文件Appender
#### 2.1 定义文件Appender
先上代码：
```
@Plugin(name = "FileAppender", category = "Core", elementType = "appender", printObject = true)
public class FileAppender extends AbstractAppender {
    private String fileName;
    /* 构造函数 */
    public FileAppender(String name, Filter filter, Layout<? extends Serializable> layout, boolean ignoreExceptions, String fileName) {
        super(name, filter, layout, ignoreExceptions);
        this.fileName = fileName;
    }
    @Override
    public void append(LogEvent event) {
        final byte[] bytes = getLayout().toByteArray(event);
        writerFile(bytes);
    }
    /*  接收配置文件中的参数 */
    @PluginFactory
    public static FileAppender createAppender(@PluginAttribute("name") String name,
                                              @PluginAttribute("fileName") String fileName,
                                              @PluginElement("Filter") final Filter filter,
                                              @PluginElement("Layout") Layout<? extends Serializable> layout,
                                              @PluginAttribute("ignoreExceptions") boolean ignoreExceptions) {
        if (name == null) {
            LOGGER.error("no name defined in conf.");
            return null;
        }
        if (layout == null) {
            layout = PatternLayout.createDefaultLayout();
        }
        // 创建文件
        if (!createFile(fileName)) {
            return null;
        }
        return new FileAppender(name, filter, layout, ignoreExceptions, fileName);
    }
    private static boolean createFile(String fileName) {
        Path filePath = Paths.get(fileName);
        try {
            // 每次都重新写文件，不追加
            if (Files.exists(filePath)) {
                Files.delete(filePath);
            }
            Files.createFile(filePath);
        } catch (IOException e) {
            LOGGER.error("create file exception", e);
            return false;
        }
        return true;
    }
    private void writerFile(byte[] log) {
        try {
            Files.write(Paths.get(fileName), log, StandardOpenOption.APPEND);
        } catch (IOException e) {
            LOGGER.error("write file exception", e);
        }
    }
}
```
上面代码有几个需要注意的地方：
- `@Plugin..`注解：这个注解，是为了在之后配置`log4j2.xml`时，指定的Appender Tag。
- 构造函数：除了使用父类的以外，也可以增加一些自己的配置。
- 重写`append()`方法：这里面需要实现具体的逻辑，日志的去向。
- `createAppender()`方法：主要是接收log4j2.xml中的配置项。
#### 2.2 添加log4j2.xml配置
```
<?xml version="1.0" encoding="UTF-8"?>
<configuration status="INFO" monitorInterval="30">
    <appenders>
        <!--这个输出控制台的配置-->
        <console name="Console" target="SYSTEM_OUT">
            <!--输出日志的格式-->
            <PatternLayout pattern="%highlight{[ %p ] [%-d{yyyy-MM-dd HH:mm:ss}] [%l] %m%n}"/>
        </console>
        <!-- 这个就是自定义的Appender -->
        <FileAppender name="File" fileName="log.log">
            <PatternLayout pattern="%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] [%-5p] {%F:%L} - %m%n" />
        </FileAppender>
    </appenders>
    <loggers>
        <!--过滤掉spring和mybatis的一些无用的DEBUG信息-->
        <logger name="org.springframework" level="INFO"></logger>
        <logger name="org.mybatis" level="INFO"></logger>
        <root level="all">
            <appender-ref ref="Console"/>
            <appender-ref ref="File"/>
        </root>
    </loggers>
</configuration>
```
备注：
- 上面的log配置，一共配了2个输出。一个是终端输出，一个是采用自定义的FileAppender输出到文件中。
- `<FileAppender>`标签要与自定义Appender中的类注解保持一致。
#### 2.3 测试
```
public class TestLogFile {
    private static final Logger logger = LogManager.getLogger(TestLogFile.class);
    public static void main(String[] args) {
        logger.info("1");
        logger.info("2");
        logger.info("3");
    }
}
```
可以看到，日志一共输出了2份，一份到终端中，一份到`log.log`中（具体的文件路径可在log4j2.xml中配置）。
** 注意：**
       编码完执行测试，在log4j2.xml中添加自定义的appender后运行可能会发现出现如下误： 
 main ERROR Error processing element XXX ([Appenders: null]): CLASS_NOT_FOUND 
```java
2016-08-02 22:46:30,693 main ERROR Error processing element XXX ([Appenders: null]): CLASS_NOT_FOUND
```
       这是由于log4j2没有加载自定义的类而抛出异常。可以在log4j2.xml中加上 ：
 <Configuration status="info" packages="com.custom.log.log4j2plugin">  
```java
<Configuration status="info" packages="com.zero.scribe.log4j2plugin">
```
       这里的packages是自定义的Appender类所在包的包名，这会告诉log4j2去加载这个包下的类，而不会抛出CLASS_NOT_FOUND异常。
       倘若不想多加packages这个字段，则需要预先build一下project，我使用的是gradle构建项目，运行gradle build后在classes中会额外生成META-INF，META-INF中会有org.apache.logging.log4j.core.config.plugins目录，目录会有Log4j2Plugins.dat，该二进制文件则会告诉log4j2需要加载自定义的Appender类。
### 3、自定义Thrift Appender
上一节，主要是日志的文件输出。有时我们需要将日志发送给日志收集服务，常见的方法可以写一个日志收集Agent，收集日志；或者将日志输出方当成客户端直接发送到远程。
下文，通过自定义Appender的方式，将日志输出到远程的RPC服务中。
#### 3.1 Thrift RPC服务
假设现在有一个Thrift RPC服务，实时接收日志消息。它的定义是下面的样子：
```
namespace java thrift
service LogServer {
    string getLogRes(1:string log);
}
```
服务很简单，入参是log，返回值是String。
Thrift相关知识可以查看，[Thrift RPC服务10分钟上手](http://www.jianshu.com/p/37305e1c5b13)。
#### 3.2 定义ThriftAppender
```
@Plugin(name = "ThriftAppender", category = "Core", elementType = "appender", printObject = true)
public class ThriftAppender extends AbstractAppender {
    private LogServer.Client client;
    private TTransport transport;
    /* 构造函数 */
    public ThriftAppender(String name, Filter filter, Layout<? extends Serializable> layout, boolean ignoreExceptions, String host) {
        super(name, filter, layout, ignoreExceptions);
        // 创建客户端
        createThriftClient(host);
    }
    @Override
    public void append(LogEvent event) {
        final byte[] bytes = getLayout().toByteArray(event);
        try {
            String response = client.getLogRes(new String(bytes));
            System.out.println(response);
        } catch (TException e) {
            e.printStackTrace();
        }
    }
    /*  接收配置文件中的参数 */
    @PluginFactory
    public static ThriftAppender createAppender(@PluginAttribute("name") String name,
                                              @PluginAttribute("host") String host,
                                              @PluginElement("Filter") final Filter filter,
                                              @PluginElement("Layout") Layout<? extends Serializable> layout,
                                              @PluginAttribute("ignoreExceptions") boolean ignoreExceptions) {
        if (name == null) {
            LOGGER.error("no name defined in conf.");
            return null;
        }
        if (layout == null) {
            layout = PatternLayout.createDefaultLayout();
        }
        return new ThriftAppender(name, filter, layout, ignoreExceptions, host);
    }
    @Override
    public void stop() {
        if (transport != null) {
            transport.close();
        }
    }
    private void createThriftClient(String host) {
        try {
            transport = new TFramedTransport(new TSocket(host, 9000));
            transport.open();
            TProtocol protocol = new TBinaryProtocol(transport);
            client = new LogServer.Client(protocol);
            LOGGER.info("create client success");
        } catch (Exception e) {
            LOGGER.error("create file exception", e);
        }
    }
}
```
备注：
除了和文件Appender相同的外，这里需要注意两个地方。一个是Thrift Client的创建，另一个Thrift发送log。
具体的发送逻辑，在`append()`方法中实现。
#### 3.3 添加log4j2.xml配置
```
<?xml version="1.0" encoding="UTF-8"?>
<configuration status="INFO" monitorInterval="30">
    <appenders>
        <!--这个输出控制台的配置-->
        <console name="Console" target="SYSTEM_OUT">
            <!--输出日志的格式-->
            <PatternLayout pattern="%highlight{[ %p ] [%-d{yyyy-MM-dd HH:mm:ss}] [%l] %m%n}"/>
        </console>
        <!-- 这个就是自定义的Appender -->
        <ThriftAppender name="Thrift" host="127.0.0.1">
            <PatternLayout pattern="%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] [%-5p] {%F:%L} - %m%n" />
        </ThriftAppender>
    </appenders>
    <loggers>
        <!--过滤掉spring和mybatis的一些无用的DEBUG信息-->
        <logger name="org.springframework" level="INFO"></logger>
        <logger name="org.mybatis" level="INFO"></logger>
        <root level="all">
            <appender-ref ref="Console"/>
            <appender-ref ref="Thrift"/>
        </root>
    </loggers>
</configuration>
```
这里同样是定义了两个输出路径，一个是终端，一个是Thrift服务。
#### 3.4 测试
```
public class TestThriftFile {
    private static final Logger logger = LogManager.getLogger(TestThriftFile.class);
    public static void main(String[] args) {
        logger.info("a");
        logger.info("b");
        logger.info("c");
    }
}
```
测试可以看出，Server端成功接收到了log。

