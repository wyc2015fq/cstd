# log 4 qt 使用（三） - baidu_33570760的博客 - CSDN博客
2017年05月03日 16:10:59[carman_风](https://me.csdn.net/baidu_33570760)阅读数：310
                
转自：http://blog.csdn.net/dbzhang800/article/details/6917462
- 
接前面[Log4Qt
 使用笔记(一)](http://blog.csdn.net/dbzhang800/article/details/6916948)，继续学习一下，关注环境变量和初始化：
## Logger
在前面，我们提到，除了一个根root之外，还可以有多个命名的Logger存在，为了直观，我们可以看LogManager的成员函数：
- static Logger * logLogger ()
- static Logger * qtLogger ()
- static Logger * rootLogger ()
- 
static Logger * logger (const QString &rName)
奇怪，除了我们提到的，还有qtLogger和logLogger两个东西存在，这两个是什么东西？原来
|马甲|真实身份|作用|
|----|----|----|
|qtLogger()|logger("Qt")|用来处理Qt中的qDebug() qWarning ...|
|logLogger()|logger("[Log4Qt](http://127.0.0.1/wiki/Log4Qt)")|用来记录该库自身的日志|
## 环境变量
Log4Qt 有4个环境变量：
|LOG4QT_DEBUG|控制Log4Qt自身输出日志的级别，前面的logLogger()|
|----|----|
|LOG4QT_DEFAULTINITOVERRIDE|是否忽略默认的初始化（除"false"外均忽略）|
|LOG4QT_CONFIGURATION|用来指定初始化用的配置文件，在前文的例子中我们代码中指定的配置文件，也可以在此指定|
|LOG4QT_CONFIGURATORCLASS|这个东西？在Log4Qt中似乎没有使用(fixme)|
继续看例子：
### 例子四
将前面的例子一，注释掉BasicConfigure
#include <QtCore>
#include "logger.h"
//#include "basicconfigurator.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
// Log4Qt::BasicConfigurator::configure();
    Log4Qt::Logger * log = Log4Qt::Logger::rootLogger();
    log->debug("Hello DbZhang800!");
    log->info("Hello Qt!");
    log->warn("Hello 1+1=2");
    return 0;
}
此时运行程序，将没有任何输出。
F:\bin\> console.exe
F:\bin\>
而如果设置环境变量：
F:\bin>set LOG4QT_CONFIGURATION=F:\bin\log4qt.conf
2011-10-29 19:30:55 [root]-[DEBUG] Hello DbZhang800!
2011-10-29 19:30:55 [root]-[INFO] Hello Qt!
2011-10-29 19:30:55 [root]-[WARN] Hello 1+1=2
F:\bin\>
配置的文件的内容如下：
log4j.rootLogger=DEBUG,A1
log4j.appender.A1=org.apache.log4j.ConsoleAppender
log4j.appender.A1.layout=org.apache.log4j.PatternLayout
log4j.appender.A1.layout.ConversionPattern=%-d{yyyy-MM-dd HH:mm:ss} [%c]-[%p] %m%n
### 例子五
接下来看看 LOG4QT_DEFAULTINITOVERRIDE 这个东西有什么用。
同样是前一个例子：如果设置还设置了这个环境变量，结果如何？
F:\bin> set LOG4QT_CONFIGURATION=F:\bin\log4qt.conf
F:\bin> set LOG4QT_DEFAULTINITOVERRIDE=true
F:\bin> console.exe
F:\bin
恩，又没有任何输出了。
只要我们设置了该环境变量，而且其值不是false，那么就会跳过这些初始化文件。
重新起作用，需要设为false
F:\bin> set LOG4QT_DEFAULTINITOVERRIDE=false
或者删除
F:\bin> set LOG4QT_DEFAULTINITOVERRIDE=
## 初始化
只要前面的 LOG4QT_DEFAULTINITOVERRIDE 没有被设置为flase以外的值，初始化时就会尝试加载默认配置，它找依次尝试3个东西
- 1. 我们提到的LOG4QT_CONFIGURATION指定的配置文件，如果不存在，则
- 2. 默认的QSettings 中的 Log4Qt/Properties 中的内容，比如：下面代码设置的东西就可以
     QSettings s;
     s.beginGroup("Log4Qt");
     s.beginGroup("Properties");
     s.setValue("log4j.appender.A1", "org.apache.log4j.FileAppender");
     s.setValue("log4j.appender.A1.file", "F:/bin/myapp.log");
     s.setValue("log4j.appender.A1.layout", "org.apache.log4j.TTCCLayout");
     s.setValue("log4j.appender.A1.layout.DateFormat", "ISO8601");
     s.setValue("log4j.rootLogger", "TRACE, A1");- 
3. 如果还没找到，则尝试当前**工作目录**下的 log4qt.properties 文件
## 还有点什么？
零零散散，随便记录一下，备忘...
### 实现细节
环境变量在 静态单件对象InitialisationHelper 的初始化时被读取。(对用户透明)
这些值在程序内可以通过
QHash< QString, QString > Log4Qt::InitialisationHelper::environmentSettings ( )         
来获取。对应的key
|LOG4QT_DEBUG|Debug|
|----|----|
|LOG4QT_DEFAULTINITOVERRIDE|DefaultInitOverride|
|LOG4QT_CONFIGURATION|Configuration|
|LOG4QT_CONFIGURATORCLASS|ConfiguratorClass|
### 例子六
忘记qDebug()这些东西了，补一个例子
#include <QtCore>
#include "logmanager.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Log4Qt::LogManager::setHandleQtMessages(true);
    qDebug("Hello DbZhang800!");
    qWarning("Hello Qt!");
    qErrnoWarning("Hello 1+1=2");
    return 0;
}
结果如下：
2011-10-29 19:59:10 [Qt]-[DEBUG] Hello DbZhang800!
2011-10-29 19:59:10 [Qt]-[WARN] Hello Qt!
2011-10-29 19:59:10 [Qt]-[ERROR] Hello 1+1=2 ()
采用的前面例子使用的配置文件。
