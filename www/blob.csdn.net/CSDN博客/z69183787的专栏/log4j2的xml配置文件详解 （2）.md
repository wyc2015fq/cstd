# log4j2的xml配置文件详解  （2） - z69183787的专栏 - CSDN博客
2017年08月29日 20:20:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1822
                
xml默认配置为：
<?xml version="1.0" encoding="UTF-8"?>
 2<Configurationstatus="WARN">
 3<Appenders>
 4<Consolename="Console"target="SYSTEM_OUT">
 5<PatternLayoutpattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n"/>
 6</Console>
 7</Appenders>
 8<Loggers>
 9<Rootlevel="error">
10<AppenderRefref="Console"/>
11</Root>
12</Loggers>
13</Configuration>
具体标签解释：
     (1).根节点Configuration有两个属性:status和monitorinterval,有两个子节点:Appenders和Loggers(表明可以定义多个Appender和Logger).
　　　　　　　status用来指定log4j本身的打印日志的级别.
　　　　　　　monitorinterval用于指定log4j自动重新配置的监测间隔时间，单位是s,最小是5s.
　　　　(2).Appenders节点，常见的有三种子节点:Console、RollingFile、File.
Console节点用来定义输出到控制台的Appender.
　　　　　　　　name:指定Appender的名字.
　　　　　　　　target:SYSTEM_OUT 或 SYSTEM_ERR,一般只设置默认:SYSTEM_OUT.
　　　　　　　　PatternLayout:输出格式，不设置默认为:%m%n.
File节点用来定义输出到指定位置的文件的Appender.
　　　　　　　　name:指定Appender的名字.
　　　　　　　　fileName:指定输出日志的目的文件带全路径的文件名.
 append:如果为false每次运行都会自动清空日志文件，常用于临时测试.
　　　　　　　　PatternLayout:输出格式，不设置默认为:%m%n.
RollingFile节点用来定义超过指定大小自动删除旧的创建新的的Appender.
　　　　　　　　name:指定Appender的名字.
　　　　　　　　fileName:指定输出日志的目的文件带全路径的文件名.
　　　　　　　　PatternLayout:输出格式，不设置默认为:%m%n.
　　　　　　　　filePattern:指定新建日志文件的名称格式.
　　　　　　　　Policies:指定滚动日志的策略，就是什么时候进行新建日志文件输出日志.
　　　　　　　 TimeBasedTriggeringPolicy:Policies子节点，基于时间的滚动策略，interval属性用来指定多久滚动一次，默认是1 hour。modulate=true用来调整时间：比如现在是早上3am，interval是4，那么第一次滚动是在4am，接着是8am，12am...而不是7am.
　　　　　　　　SizeBasedTriggeringPolicy:Policies子节点，基于指定文件大小的滚动策略，size属性用来定义每个日志文件的大小.（查阅多处资料显示，在低一些的版本中，存在bug，这个属性比起作用，比如这里使用的2.3版本）
　　　　　　　　DefaultRolloverStrategy:用来指定同一个文件夹下最多有几个日志文件时开始删除最旧的，创建新的(通过max属性，如果max属性不设置，则默认是同一个文件夹下最多7个文件)。
　　　　(3).Loggers节点，常见的有两种:Root和Logger.
Root节点用来指定项目的根日志，如果没有单独指定Logger，那么就会默认使用该Root日志输出
　　　　　　　　　level:日志输出级别，共有8个级别，按照从低到高为：All < Trace < Debug < Info < Warn < Error < Fatal < OFF.
　　　　　　　　　AppenderRef：Root的子节点，用来指定该日志输出到哪个Appender.
Logger节点用来单独指定日志的形式，比如要为指定包下的class指定不同的日志级别等。
　　　　　　　　　level:日志输出级别，共有8个级别，按照从低到高为：All < Trace < Debug < Info < Warn < Error < Fatal < OFF.
　　　　　　　　　name:用来指定该Logger所适用的类或者类所在的包全路径,继承自Root节点.
　　　　　　　　　AppenderRef：Logger的子节点，用来指定该日志输出到哪个Appender,如果没有指定，就会默认继承自Root.如果指定了，那么会在指定的这个Appender和Root的Appender中都会输出，此时我们可以设置Logger的additivity="false"只在自定义的Appender中进行输出。
　　　　(4).关于日志level.
　　　　　　共有8个级别，按照从低到高为：All < Trace < Debug < Info < Warn < Error < Fatal < OFF.
　　　　　　All:最低等级的，用于打开所有日志记录.
　　　　　　Trace:是追踪，就是程序推进以下，你就可以写个trace输出，所以trace应该会特别多，不过没关系，我们可以设置最低日志级别不让他输出.
　　　　　　Debug:指出细粒度信息事件对调试应用程序是非常有帮助的.
　　　　　　Info:消息在粗粒度级别上突出强调应用程序的运行过程.
　　　　　　Warn:输出警告及warn以下级别的日志.
　　　　　　Error:输出错误信息日志.
　　　　　　Fatal:输出每个严重的错误事件将会导致应用程序的退出的日志.
　　　　　　OFF:最高等级的，用于关闭所有日志记录.
（5）日志输出格式：
%d{yyyy-MM-dd HH:mm:ss, SSS} : 日志生产时间
                %p : 日志输出格式
                %c : logger的名称
                %m : 日志内容，即 logger.info("message")
                %n : 换行符
                %C : Java类名
                %L : 日志输出所在行数
                %M : 日志输出所在方法名
                hostName : 本地机器名
                hostAddress : 本地ip地址
对于日志输出格式，以及Filters，Lookups更多的用法，请参考：http://www.cnblogs.com/lzb1096101803/p/5796849.html对于更多的标签使用的解析，请参考：http://www.aiuxian.com/article/p-2039401.html
