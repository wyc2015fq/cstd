
# Log4j 日志详细用法 - 阳光岛主 - CSDN博客

2014年07月23日 08:15:13[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：9662


简单的说log4j就是帮助开发人员进行日志输出管理的API类库。它最重要的特点就可以配置文件灵活的设置日志信息的优先级、日志信息的输出目的地、日志信息的输出格式
Log4j 除了可以记录程序运行日志信息外还有一重要的功能就是用来显示调试信息。
程序员经常会遇到脱离java ide环境调试程序的情况，这时大多数人会选择使用System.out.println语句输出某个变量值的方法进行调试。这样会带来一个非常麻烦的问题：一旦哪天程序员决定不要显示这些System.out.println的东西了就只能一行行的把这些垃圾语句注释掉。若哪天又需调试变量值，则只能再一行行去掉这些注释恢复System.out.println语句。使用log4j可以很好的处理类似情况。

**log4j使用方法**
下面介绍的是log4j一些理论方面的知识，读者觉得枯燥的话可以跳过本节直接阅读第三节实例部分。
1、定义配置文件
首先使用配置文件将使我们的应用程序更加灵活配置log日志输出方式包括输出优先级、输出目的地、输出格式。
Log4j支持两种配置文件格式，
1）XML格式的文件，
2）Java特性文件log4j.properties（键=值）。
下面将介绍使用log4j.properties文件作为配置文件的方法：
①配置根Logger，其语法为：0
log4j.rootLogger = [ level ] , appenderName, appenderName, …
其中，level 是日志记录的优先级，分为OFF、FATAL、ERROR、WARN、INFO、DEBUG、ALL或者自定义的级别。
Log4j建议只使用四个级别，优先级从高到低分别是ERROR、WARN、INFO、DEBUG
通过在这里定义的级别，您可以控制到应用程序中相应级别的日志信息的开关。比如在这里定义了INFO级别，则应用程序中所有DEBUG级别的日志信息将不被打印出来。 appenderName就是指定日志信息输出到哪个地方。可同时指定多个输出目的地。

②配置日志信息输出目的地Appender，其语法为：
log4j.appender.appenderName = fully.qualified.name.of.appender.class
log4j.appender.appenderName.option1 = value1
…
log4j.appender.appenderName.option = valueN
其中，Log4j提供的appender有以下几种：
org.apache.log4j.ConsoleAppender（控制台），
org.apache.log4j.FileAppender（文件），
org.apache.log4j.DailyRollingFileAppender（每天产生一个日志文件），
org.apache.log4j.RollingFileAppender（文件大小到达指定尺寸的时候产生一个新的文件），
org.apache.log4j.WriterAppender（将日志信息以流格式发送到任意指定的地方）

③配置日志信息的格式（布局），其语法为：
log4j.appender.appenderName.layout = fully.qualified.name.of.layout.class
log4j.appender.appenderName.layout.option1 = value1
…
log4j.appender.appenderName.layout.option = valueN
其中，Log4j提供的layout有以下几种：
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
%d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyy MMM dd HH:mm:ss,SSS}，输出类似：2002年10月18日 22：10：28，921
%l 输出日志事件的发生位置，包括类目名、发生的线程，以及在代码中的行数。举例：Testlog4.main(TestLog4.java:10)

**2、在代码中使用Log4j**
①得到记录器
使用Log4j，第一步就是获取日志记录器，这个记录器将负责控制日志信息。其语法为：
public static Logger getLogger( String name)
通过指定的名字获得记录器，如果必要的话，则为这个名字创建一个新的记录器。Name一般取本类的名字，比如：
static Logger logger = Logger.getLogger ( ServerWithLog4j.class.getName () )

②读取配置文件
当获得了日志记录器之后，第二步将配置Log4j环境，其语法为：
BasicConfigurator.configure ()： 自动快速地使用缺省Log4j环境。
PropertyConfigurator.configure ( String configFilename)：读取使用Java的特性文件编写的配置文件。
例：
PropertyConfigurator.configure(".\\src\\log4j.properties")
DOMConfigurator.configure ( String filename ) ：读取XML形式的配置文件。
③插入记录信息（格式化日志信息）
当上两个必要步骤执行完毕，就可轻松地使用不同优先级别的日志记录语句插入到您想记录日志的任何地方，其语法如下：
Logger.debug ( Object message ) ;
Logger.info ( Object message ) ;
Logger.warn ( Object message ) ;
Logger.error ( Object message ) ;

**log4j范例程序**
下面将使用一个最简单的范例程序来进一步说明log4j的使用方法。程序代码如下：
import org.apache.log4j.*;
public classLogTest{
static Logger logger = Logger.getLogger(LogTest.class.getName());
public static void main(String[] args) {
PropertyConfigurator.configure ("src/log4j.properties")；
logger.debug("Debug ...");
logger.info("Info ...");
logger.warn("Warn ...");
logger.error("Error ...");
}
}

**程序说明：**
① static Logger logger = Logger.getLogger(LogTest.class.getName());就是创建一个属于LogTest类的Logger对象，创建时要告知Logger你当前的Class是什么。
② PropertyConfigurator.configure ("src/log4j.properties")就是说使用当前工程目录下的src文件夹中的log4j.properties文件作为配置文件。若将log4j.properties放在工程根目录下也可不写此句，程序会自动找到配置文件。
③ logger.debug就是输出debug的信息，logger.info就是输出提示信息，logger.warn就是显示警告信息，logger.error就是显示错误信息。
下面是配置文件log4j.properties的内容：
log4j.rootCategory=DEBUG, stdout，R

log4j.appender.stdout=org.apache.log4j.ConsoleAppender
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern=%5p (%F:%L) - %m%n

log4j.appender.R=org.apache.log4j.RollingFileAppender
log4j.appender.R.File=log.txt
log4j.appender.R.MaxFileSize=100KB
log4j.appender.R.MaxBackupIndex=1
log4j.appender.R.layout=org.apache.log4j.PatternLayout
log4j.appender.R.layout.ConversionPattern=%d{yyyy MMM dd HH:mm:ss} %-5p %c - %m%n

**程序说明：**
① log4j.rootCategory=DEBUG, stdout，R
就是说我要显示所有优先权等於和高于Debug的信息。
"stdout"，”R”表示我定义了两个输出端(随便什么名字都好)

②下面的三行说stdout输出端其实是标准输出Console，也就是屏幕。输出的格式是PatternLayout。转换方式是%5p (%F:%L) - %m%n，即前五格用来显示优先权，再显示当前的文件名，加当前的行数。最后%m是logger.debug()或logger.info()或logger.warn()或logger.error()里的信息。%n表示回车空行

③再加上下面六行则log信息不光显示在屏幕上，而且将被保存在一个叫"log.txt"的文件里，文件最大为100KB。如果文件大小超过100KB，文件会被备份成"log.txt.1"，新的"log.txt"继续记录log信息。

接下来我们可以改变log4j.properties，而不需重新编译就可以控制log信息是否显示、log信息的输出端类型、输出方式、输出格式，等等。
**举例如下：**
①在log4j.properties文件里把"log4j.rootCategory=DEBUG,stdout,R"改写成"log4j.rootCategory=OFF, stdout,R"，这样所有的log信息都不会显示了；解决了本文开始提出的问题。
②在log4j.properties文件里把"log4j.rootCategory=DEBUG,stdout,R"改写成"log4j.rootCategory=INFO, stdout,R"，这样只显示INFO, WARN, ERROR的log信息，而DEBUG信息不会被显示；

**在web程序中使用log4j注意问题**
1、 由于jsp或servlet在执行状态时没有当前路径概念，所有使用PropertyConfigurator.configure（String）语句找log4j.properties文件时要给出相对于当前jsp或servlet的路径转化成为一个绝对的文件系统路径。
方法是使用servletcontext.getrealpath(string)语句。例：
//得到当前jsp路径
String prefix =   getServletContext().getRealPath("/");
//读取log4j.properties
PropertyConfigurator.configure(prefix+"\\WEB-INF\\log4j.properties");
2、相应的log4j.properties设置某个属性时也要在程序中设置绝对路径。例：
log4j.appender.R.File属性设置日志文件存放位置。我们可以用读写.properties配置文件的方法进行灵活设置。

在spring框架下，使用log4j更是容易，下面介绍一下spring下的log4j应用。
1）首先要下载相应的jar包（log4j.jar）
2）配置web.xml，在web.xml中加入如下配置
<context-param>
<param-name>log4jConfigLocation</param-name>
<param-value>/WEB-INF/config/log4j.properties</param-value>
</context-param>
<context-param>
<param-name>log4jRefreshInterval</param-name>
<param-value>6000</param-value>
</context-param>
<listener>
<listener-class>
org.springframework.web.util.Log4jConfigListener
</listener-class>
</listener>
说明：在上文的配置里，Log4jConfigListener会去WEB-INF/config/log4j.propeties 读取配置文件;
开一条watchdog线程每60秒扫描一下配置文件的变化（这样在web服务启动后再去修改配置文件也不用重新启动web服务了）;
并把web目录的路径压入一个叫webapp.root的系统变量（webapp.root将在log4j.properties文件中使用）。


**参考推荐：**
[log4j的两种配置xml与properties](http://blog.csdn.net/ankerdiao/article/details/25723353)
[log4j动态配置路径输出多个自定义日志文件](http://blog.csdn.net/wiwipetter/article/details/4390579)



