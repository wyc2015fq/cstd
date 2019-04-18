# Log4j2 自定义 Appender - z69183787的专栏 - CSDN博客
2016年06月28日 15:25:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11503
虽然log4j2 自带了很多种Appender 但是有时候避免不了不是自己想要的，这时候我们就需要自定义Appender，使日志输出到我们指定的位置，废话少说，上菜！
```java
package com.hbzx.log;
import java.io.Serializable;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import org.apache.logging.log4j.core.Filter;
import org.apache.logging.log4j.core.Layout;
import org.apache.logging.log4j.core.LogEvent;
import org.apache.logging.log4j.core.appender.AbstractAppender;
import org.apache.logging.log4j.core.appender.AppenderLoggingException;
import org.apache.logging.log4j.core.config.plugins.Plugin;
import org.apache.logging.log4j.core.config.plugins.PluginAttribute;
import org.apache.logging.log4j.core.config.plugins.PluginElement;
import org.apache.logging.log4j.core.config.plugins.PluginFactory;
import org.apache.logging.log4j.core.layout.PatternLayout;
import com.hbzx.frame.MainFrame;
/**
 * 自定义Appender，继承 AbstractAppender 只需要覆盖自已想要的方法即可<br>
 * 类上面的注解是用来设置配置文件中的标签。
 */
@Plugin(name = "TextArea", category = "Core", elementType = "appender", printObject = true)
public class TextAreaAppender extends AbstractAppender {
    /**
     * @fields serialVersionUID
     */
    private static final long serialVersionUID = -830237775522429777L;
    private final ReadWriteLock rwLock = new ReentrantReadWriteLock();
    private final Lock readLock = rwLock.readLock();
    //需要实现的构造方法，直接使用父类就行
    protected TextAreaAppender(final String name, final Filter filter, final Layout<? extends Serializable> layout,
            final boolean ignoreExceptions) {
        super(name, filter, layout, ignoreExceptions);
    }
    @Override
    public void append(LogEvent event) {
        readLock.lock();
        try {
            final byte[] bytes = getLayout().toByteArray(event);//日志二进制文件，输出到指定位置就行
            //下面这个是要实现的自定义逻辑
            MainFrame.log(new String(bytes));
        } catch (Exception ex) {
            if (!ignoreExceptions()) {
                throw new AppenderLoggingException(ex);
            }
        } finally {
            readLock.unlock();
        }
    }
    // 下面这个方法可以接收配置文件中的参数信息
    @PluginFactory
    public static TextAreaAppender createAppender(@PluginAttribute("name") String name,
            @PluginElement("Filter") final Filter filter,
            @PluginElement("Layout") Layout<? extends Serializable> layout,
            @PluginAttribute("ignoreExceptions") boolean ignoreExceptions) {
        if (name == null) {
            LOGGER.error("No name provided for MyCustomAppenderImpl");
            return null;
        }
        if (layout == null) {
            layout = PatternLayout.createDefaultLayout();
        }
        return new TextAreaAppender(name, filter, layout, ignoreExceptions);
    }
}
```
接下来就是配置文件中的信息了！
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="info" **packages="com.hbzx.log"**>
    <Properties>
        <Property name="filePath">C:/logs/project.log</Property>
    </Properties>
    <Appenders>
        <!--这个输出控制台的配置 -->
        <Console name="console" target="SYSTEM_OUT">
            <!--控制台只输出level及以上级别的信息（onMatch），其他的直接拒绝（onMismatch） -->
            <ThresholdFilter level="DEBUG" onMatch="ACCEPT" onMismatch="DENY" />
            <PatternLayout pattern="%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] [%-5p]  %c {%F:%L} - %m%n" />
        </Console>
        <File  name="logFile" fileName="${filePath}">
            <PatternLayout pattern="%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] [%-5p]  %c {%F:%L} - %m%n" />
        </File>
        
        <!-- 这个就是自定义的Appender -->
        <TextArea name="textarea">
            <PatternLayout pattern="%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] [%-5p] {%F:%L} - %m%n" />
        </TextArea>
    </Appenders>
    <Loggers>
        <logger name="com.hbzx" level="debug" />
        <logger name="org.apache.http" level="info" />
        <logger name="org.apache.commons" level="info" />
        <logger name="io.netty" level="info" />
        <logger name="org.asynchttpclient" level="info" />
        <Root level="debug">
            <AppenderRef ref="console" />
            <AppenderRef ref="logFile" />
            <AppenderRef ref="textarea" />
        </Root>
    </Loggers>
</Configuration>
```
虽然log4j2自带了很多种Appender，但是有时候需要自定义Appender来满足业务需求。例如log4j2就没有为Scribe添加appender。
       Appender的自定义其实很简单，可以参考log4j-core源码中org.apache.logging.log4j.core.appender.mom包中其它的扩展appender的实现。主要是实现AbstractAppender和AbstractManager。
       编码完后，在log4j2.xml中添加自定义的appender后运行会发现：
**[java]**[view
 plain](http://blog.csdn.net/zero__007/article/details/52107578#)[copy](http://blog.csdn.net/zero__007/article/details/52107578#)
- 2016-08-0222:46:30,693 main ERROR Error processing element XXX ([Appenders: null]): CLASS_NOT_FOUND  
       这是由于log4j2没有加载自定义的类而抛出异常。可以在log4j2.xml中加上
**[java]**[view
 plain](http://blog.csdn.net/zero__007/article/details/52107578#)[copy](http://blog.csdn.net/zero__007/article/details/52107578#)
- <Configuration status="info" packages="com.zero.scribe.log4j2plugin">  
       这里的packages是自定义的Appender类所在包的包名，这会告诉log4j2去加载这个包下的类，而不会抛出CLASS_NOT_FOUND异常。
       倘若不想多加packages这个字段，则需要预先build一下project，我使用的是gradle构建项目，运行gradle build后在classes中会额外生成META-INF，META-INF中会有org.apache.logging.log4j.core.config.plugins目录，目录会有Log4j2Plugins.dat，该二进制文件则会告诉log4j2需要加载自定义的Appender类。
