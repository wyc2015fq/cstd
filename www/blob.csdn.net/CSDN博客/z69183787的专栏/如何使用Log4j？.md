# 如何使用Log4j？ - z69183787的专栏 - CSDN博客
2013年05月09日 16:27:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：724
个人分类：[日志-Log4j&Log4j2](https://blog.csdn.net/z69183787/article/category/2175479)
**1、 Log4j是什么？**  Log4j可以帮助调试（有时候debug是发挥不了作 用的）和分析，要下载和了解更详细的内容，还是访问其官方网站吧：[http://jakarta.apache.org/log4j](http://jakarta.apache.org/log4j)。
**2、Log4j的概念**
   Log4j中有三个主要的组件，它们分别是Logger、Appender和Layout，Log4j
 允许开发人员定义多个Logger，每个Logger拥有自己的名字，Logger之间通过名字来表明隶属关系。有一个Logger称为Root，它永远 存在，且不能通过名字检索或引用，可以通过Logger.getRootLogger()方法获得，其它Logger通过 Logger.getLogger(String name)方法。
   Appender则是用来指明将所有的log信息存放到什么地方，Log4j中支持多种appender，如console、files、GUI
 components、NT Event Loggers等，一个Logger可以拥有多个Appender，也就是你既可以将Log信息输出到屏幕，同时存储到一个文件中。
   Layout的作用是控制Log信息的输出方式，也就是格式化输出的信息。
   Log4j中将要输出的Log信息定义了5种级别，依次为DEBUG、INFO、WARN、ERROR和FATAL，当输出时，只有级别高过配置中规定的 级别的信息才能真正的输出，这样就很方便的来配置不同情况下要输出的内容，而不需要更改代码，这点实在是方便啊。
**3、Log4j的配置文件**
  虽然可以不用配置文件，而在程序中实现配置，但这种方法在如今的系统开发中显然是不可取的，能采用配置文件的地方一定一定要用配置文件。Log4j支持两 种格式的配置文件：XML格式和Java的property格式，本人更喜欢后者，首先看一个简单的例子吧，如下：
  log4j.rootLogger=debug, **stdout, R**
  log4j.appender.**stdout**=org.apache.log4j.ConsoleAppender
  log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
  # Pattern to output the caller's file name and line number.
  log4j.appender.stdout.layout.ConversionPattern=%5p [%t] **(%F:%L)** - %m%n
  log4j.appender.**R**=org.apache.log4j.RollingFileAppender
  log4j.appender.R.File=example.log
  log4j.appender.R.MaxFileSize=**100KB**
  # Keep one backup file
  log4j.appender.R.MaxBackupIndex=1
  log4j.appender.R.layout=org.apache.log4j.PatternLayout
  log4j.appender.R.layout.ConversionPattern=%p %t %c - %m%n         
  首先，是设置root，格式为 log4j.rootLogger=[level],appenderName, ...，其中level就是设置需要输出信息的级别，后面是appender的输出的目的地，appenderName就是指定日志信息输出到哪个地方。您可以同时指定多个输出目的地。配置日志信息输出目的地Appender，其语法为
  log4j.appender.appenderName = fully.qualified.name.of.appender.class
  log4j.appender.appenderName.option1 = value1
  ...
  log4j.appender.appenderName.option = valueN
Log4j提供的appender有以下几种：
  org.apache.log4j.ConsoleAppender（控制台）
  org.apache.log4j.FileAppender（文件）
  org.apache.log4j.DailyRollingFileAppender（每天产生一个日志文件）
  org.apache.log4j.RollingFileAppender（文件大小到达指定尺寸的时候产生新文件）
  org.apache.log4j.WriterAppender（将日志信息以流格式发送到任意指定的地方）
配置日志信息的格式（布局），其语法为：
  log4j.appender.appenderName.layout = fully.qualified.name.of.layout.class
  log4j.appender.appenderName.layout.option1 = value1
  ....
  log4j.appender.appenderName.layout.option = valueN
Log4j提供的layout有以下几种：
  org.apache.log4j.HTMLLayout（以HTML表格形式布局），
  org.apache.log4j.PatternLayout（可以灵活地指定布局模式），
  org.apache.log4j.SimpleLayout（包含日志信息的级别和信息字符串），
  org.apache.log4j.TTCCLayout（包含日志产生的时间、线程、类别等等信息） 
Log4J采用类似C语言中的printf函数的打印格式格式化日志信息，打印参数如下：
 %m 输出代码中指定的消息
%p 输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL 
　　%r 输出自应用启动到输出该log信息耗费的毫秒数 
　　%c 输出所属的类目，通常就是所在类的全名 
　　%t 输出产生该日志事件的线程名 
　　%n 输出一个回车换行符，Windows平台为“\r\n”，Unix平台为“\n” 
　　%d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyy MMM dd HH:mm:ss,SSS}，输出类似：2002年10月18日22：10：28，921 
　　%l 输出日志事件的发生位置，包括类目名、发生的线程，以及在代码中的行数。举例：Testlog4.main(TestLog4.java:10)
**4、Log4j在程序中的使用**
  要在自己的程序中使用Log4j，首先需要将commons-logging.jar和logging-log4j-1.2.9.jar导入到构建路径中。然后再将log4j.properties放到src根目录下。这样就可以在程序中使用log4j了。在类中使用log4j，首先声明一个静态变量Logger
 logger=Logger.getLog("classname")；现在就可以使用了，用法如下：logger.debug("debug message")或者logger.info("info message")，看下面一个小例子：
  import com.foo.Bar;
  import org.apache.log4j.Logger;
  import org.apache.log4j.PropertyConfigurator;
  public class MyApp {
    static Logger logger = Logger.getLogger(MyApp.class.getName());
    public static void main(String[] args) {
      // BasicConfigurator replaced with PropertyConfigurator.
      PropertyConfigurator.configure(args[0]);
      logger.info("Entering application.");
      Bar bar = new Bar();
      bar.doIt();
      logger.info("Exiting application.");
    }
  }
