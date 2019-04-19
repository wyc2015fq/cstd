# log4qt 使用（一） - baidu_33570760的博客 - CSDN博客
2017年05月03日 16:08:05[carman_风](https://me.csdn.net/baidu_33570760)阅读数：318
转载自：http://blog.csdn.net/dbzhang800/article/details/6916948
没用过Log4J和Log4CXX，初次接触Log4Qt，还真有点头昏脑胀，无论如何，先简单记录一下。
## Log4Qt
Log4Qt 是Apache Log4J 的Qt移植版，所以看Log4J的资料应该是最直接有效的(因为[Log4Qt](http://127.0.0.1/wiki/Log4Qt)的直接资料太少了)。
Log4Qt主要是用来记录日志(有助于程序调试)。有3个主要的组件：
- Logger
提供日志记录服务，可以有多个Logger存在，每个有它们自己的名字。Logger间存在隶属关系，有一个Logger称为根Logger。- Appender
用来指明将日志记录到什么地方：比如，控制台、文件、数据库，等等- Layout
控制日志的输出格式，可以类比一下C中的printf哈。
## 例子一
一个简单的使用Log4Qt的Qt程序：(使用基础配置和根logger)
#include <QtCore>
#include "logger.h"
#include "basicconfigurator.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Log4Qt::BasicConfigurator::configure();
    Log4Qt::Logger * log = Log4Qt::Logger::rootLogger();
    log->debug("Hello DbZhang800!");
    log->info("Hello Qt!");
    log->warn("Hello 1+1=2");
    return 0;
}
结果如下：
16 [0x00a56708] DEBUG root  - Hello DbZhang800!
16 [0x00a56708] INFO root  - Hello Qt!
16 [0x00a56708] WARN root  - Hello 1+1=2
## 例子二
稍微扩充一点，使用自己定义的logger
#include <QtCore>
#include "logger.h"
#include "basicconfigurator.h"
class Object
{
    Log4Qt::Logger * logger;
public:
    Object() {
        logger = Log4Qt::Logger::logger("Object");
        logger->info("Constructor");
    }
    ~Object() {
        logger->info("Destructor");
    }
};
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Log4Qt::BasicConfigurator::configure();
    Object obj;
    return 0;
}
结果：
0 [0x00a56708] INFO Object  - Constructor
0 [0x00a56708] INFO Object  - Destructor
在类Object中，我们可以使用名为"Object"的Logger。恩，直接指定类名是可以的。可是我们现在在Qt下啊。Qt的元对象系统中包含类的名字，故尔我们可以直接使用。
## 例子三
用使用元对象系统，那么必须使用Q_OBJECT宏，然后我们从元对象系统获取类的名字，然后创建一个相应的logger就行了。
不过Log4Qt，既然是为Qt准备的，那么就要为Qt程序解忧嘛，也就是宏
LOG4QT_DECLARE_QCLASS_LOGGER
这个宏展开后成为该类的一个成员函数，返回一个Logger，我们就可以直接使用了：
#include <QtCore>
#include "logger.h"
#include "propertyconfigurator.h"
class Object:public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    Object(){logger()->info("constructor");}
    ~Object(){logger()->info("destructor");}
};
#include "main.moc"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Log4Qt::PropertyConfigurator::configure(a.applicationDirPath() + "/log4qt.conf");
    Object obj;
    return 0;
}
程序的输出结果如下：
2011-10-29 13:52:06 [Object]-[INFO] constructor
2011-10-29 13:52:06 [Object]-[INFO] destructor
注意，本例中，我们没有使用默认配置，而是使用了一个配置文件log4qt.conf，放在了可执行程序所在目录下面
log4j.rootLogger=DEBUG,A1
log4j.appender.A1=org.apache.log4j.ConsoleAppender
log4j.appender.A1.layout=org.apache.log4j.PatternLayout
log4j.appender.A1.layout.ConversionPattern=%-d{yyyy-MM-dd HH:mm:ss} [%c]-[%p] %m%n
额，没错，里面的这些东西确实都是以log4j打头的，所以需要看log4j的手册来搞清楚这些东西：
- 配置rootLogger，高于DEBUG的日志被输出，定义一个appender，取名为A1
- 配置A1，输出到控制台
- 设置A1采用的layout，并设置其具体格式
### Log4j配置文件
我是不是太了解，简单整理一下：
#### 配置根Logger
其语法
log4j.rootLogger = [level], appenderName1, appenderName2, ...
其中：
- level 控制日志输出的级别：
|OFF|关闭所有日志输出|
|----|----|
|FATAL||
|**ERROR**|建议使用的几个级别？，高于指定级别的日志才被输出|
|**WARN**| |
|**INFO**| |
|**DEBUG**| |
|TRACE||
|ALL|所有日志均输出|
- appenderName 是appender的名字，指定输出到哪儿
注：除rootLogger外，还可以对各个logger分别设置
log4j.logger.myLogName = [level], appenderName1, appenderName2, ...
#### 配置Appender
语法
log4j.appender.appenderName = fully.qualified.name.of.appender.class
log4j.appender.appenderName.option1 = value1
...
log4j.appender.appenderName.optionN = valueN 
name可以任意取，Log4j提供的appender有：
|org.apache.log4j.ConsoleAppender|控制台|
|----|----|
|org.apache.log4j.FileAppender|文件|
|org.apache.log4j.DailyRollingFileAppender|每天产生一个日志文件|
|org.apache.log4j.RollingFileAppender|文件大小到达指定尺寸的时候产生一个新的文件|
### 配置layout
语法
log4j.appender.appenderName.layout = fully.qualified.name.of.layout.class
log4j.appender.appenderName.layout.option1 = value1
…
log4j.appender.appenderName.layout.optionN = valueN 
其中：
log4j提供的layout有
|org.apache.log4j.HTMLLayout|以HTML表格形式布局|
|----|----|
|org.apache.log4j.PatternLayout|可以灵活地指定布局模式|
|org.apache.log4j.SimpleLayout|包含日志信息的级别和信息字符串|
|org.apache.log4j.TTCCLayout|包含日志产生的时间、线程、类别等等信息|
格式控制符：
|%m|输出代码中指定的消息|
|----|----|
|%p|输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL|
|%c|输出logger的名字，通常就是所在类的全名|
|%n|输出一个回车换行符|
|%d|输出时间，默认格式为ISO8601，也可以指定格式，比如：%d{yyyy MMM dd HH:mm:ss}，类似：2011年10月28日 14：10：28|
