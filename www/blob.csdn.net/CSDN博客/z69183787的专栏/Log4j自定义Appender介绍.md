# Log4j自定义Appender介绍 - z69183787的专栏 - CSDN博客
2015年06月25日 17:02:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2043
最初想要在执行一段业务逻辑的时候调用一个外部接口记录审计信息，一直找不到一个比较优雅的方式，经过讨论觉得log4j自定义的appender或许可以实现此功能。后来就了解了一下log4j的这部分。
Apache Log4j 架构
Apache Log4j是当前在J2EE和J2SE开发中用得最多的日志框架（几乎所有项目都用它），因为它具有出色的性能、灵活的配置以及丰富的功能，并且在业务有特殊的要求时，可以使用自定义组件来代替框架中已有的组件来满足要求。 
log4j组件介绍
Log4j主要有三个组件： 
- Logger：负责供客户端代码调用，执行debug(Object msg)、info(Object msg)、warn(Object msg)、error(Object msg)等方法。
- Appender：负责日志的输出，Log4j已经实现了多种不同目标的输出方式，可以向文件输出日志、向控制台输出日志、向Socket输出日志等。
- Layout：负责日志信息的格式化。
Logger 层级介绍
Logger的层级是logger名字指定的，如x.y 表示两层，x层和y层，x是y的父层级，x.y所在层级是y层级 
log4j.additivity.* = false : 表示当前logger不需要打到父层级所指定的appender，只打到当前的appender； 
      默认true：表示当前logger将打印日志到当前的appender及所有的父层级所指定的appender 
![](http://dl.iteye.com/upload/attachment/582062/99507651-94eb-32aa-bc4c-ae42a5da1157.jpg)
Layout有多种
最常用且最灵活的输出格式是： org.apache.log4j.PatternLayout 
可以用以下的各项进行组合配置： 
- %c   logger名字空间的全称，如果加上{<层数>}表示列出从最内层算起的指定层数的名字空间。
- %C  调用logger的类的全名（包含包路径）。 
- %d  日志记录时间，{<日期格式>}使用ISO8601定义的日期格式。
- %F  调用logger的源文件名。 
- %l    日志事件的发生位置，包括类目名、发生的线程，以及在代码中的行数。
- %L   调用logger的代码行 %m   输出消息。 
- %M   调用logger的方法名。 
- %n   当前平台下的换行符。 
- %p   该条日志的优先级。 
- %r   从程序启动时到记录该条日志时已经经过的毫秒数。
- %t  产生该日志事件的线程名。 
- %x   按NDC（Nested Diagnostic Context，线程堆栈）顺序输出日志。
- %X   按MDC（Mapped Diagnostic Context，线程映射表）输出日志。通常用于多个客户端连接同一台服务器，方便服务器区分是那个客户端访问留下来的日志。
- %%   显示一个百分号。）
执行顺序及关系
调用Log4j输出日志时，调用各个组件的顺序:
- 1、日志信息传入 Logger。 
- 2、将日志信息封装成 LoggingEvent 对象并传入 Appender。
- 3、在 Appender 中调用 Filter 对日志信息进行过滤，调用 Layout 对日志信息进行格式化，然后输出。
图示： 
![](http://dl.iteye.com/upload/attachment/582067/f93e0917-3b35-3cf4-97f5-8c5b86b4e50d.png)
实现自定义log4j Appender
明白了log4j的结构关系实现自定义的log4j appender就迎刃而解了 
继承log4j公共的基类：AppenderSkeleton 
打印日志核心方法：abstract protected void append(LoggingEvent event); 
初始化加载资源：public void activateOptions()，默认实现为空 
释放资源：public void close() 
是否需要按格式输出文本：public boolean requiresLayout() 
正常情况下我们只需要覆盖append方法即可。然后就可以在log4j中使用了 
demo代码：
Java代码  ![收藏代码](http://gemantic.iteye.com/images/icon_star.png)
- import org.apache.log4j.AppenderSkeleton;  
- import org.apache.log4j.spi.LoggingEvent;  
- 
- publicclass HelloAppender extends AppenderSkeleton {  
- 
- private String account ;  
- 
- @Override
- protectedvoid append(LoggingEvent event) {  
-         System.out.println("Hello, " + account + " : "+ event.getMessage());  
-     }  
- 
- @Override
- publicvoid close() {  
- // TODO Auto-generated method stub
- 
-     }  
- 
- @Override
- publicboolean requiresLayout() {  
- // TODO Auto-generated method stub
- returnfalse;  
-     }  
- 
- public String getAccount() {  
- return account;  
-     }  
- 
- publicvoid setAccount(String account) {  
- this.account = account;  
-     }  
- }  
Java代码  ![收藏代码](http://gemantic.iteye.com/images/icon_star.png)
- publicstaticvoid main(String[] args) {  
-     Log log = LogFactory.getLog("helloLog") ;  
-     log.info("I am ready.") ;  
- }  
引用
log4j.properties 配置 
log4j.logger.helloLog=INFO, hello 
log4j.appender.hello=HelloAppender 
log4j.appender.hello.account=World
执行main函数，输出结果
Hello, World : I am ready.
