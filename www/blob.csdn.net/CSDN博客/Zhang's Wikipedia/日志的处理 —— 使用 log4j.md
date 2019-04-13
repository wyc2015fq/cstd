
# 日志的处理 —— 使用 log4j - Zhang's Wikipedia - CSDN博客


2018年04月27日 22:07:33[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：104个人分类：[项目规范](https://blog.csdn.net/lanchunhui/article/category/6215724)



通过 log4j，日志信息不仅打印到 console，而且输出到指定文件，根据配置信息；
```python
<
```
```python
dependency
```
```python
>
```
```python
<
```
```python
groupId
```
```python
>
```
```python
log4j
```
```python
</
```
```python
groupId
```
```python
>
```
```python
<
```
```python
artifactId
```
```python
>
```
```python
log4j
```
```python
</
```
```python
artifactId
```
```python
>
```
```python
<
```
```python
version
```
```python
>
```
```python
1.2.17
```
```python
</
```
```python
version
```
```python
>
```
```python
</
```
```python
dependency
```
```python
>
```
## 1. startup
如下 demo 所示为使用 logger 的 三种简单方法；
publicclassLogTest{privateLogger logger;@Beforepublicvoidinit()
    {
        logger = Logger.getLogger(LogTest.class);
        logger.removeAllAppenders();
        Logger.getRootLogger().removeAllAppenders();
    }// 使用默认配置@TestpublicvoidtestLogCreate()
    {
        BasicConfigurator.configure();
        logger.info("using the default configure");
    }// 手动构造 Appender@TestpublicvoidtestAddAppenderWithStream()
    {
        logger.addAppender(newConsoleAppender(newPatternLayout("%p %t %m%n"), ConsoleAppender.SYSTEM_OUT
        ));
        logger.info("add appender with stream");
    }// Appender 的构造中可以不含输出流@TestpublicvoidtestAddAppenderWithoutStream()
    {
        logger.addAppender(newConsoleAppender(newPatternLayout("%p %t %m%n")));
        logger.info("add appender without stream");
    }
}
## 2. 注意
注意区别 log4j 下的 Logger 类和 java Logging 下的 Logger 类；
import java.util.logging.Logger;


