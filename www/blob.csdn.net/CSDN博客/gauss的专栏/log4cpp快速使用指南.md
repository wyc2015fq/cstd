# log4cpp快速使用指南 - gauss的专栏 - CSDN博客
2014年02月22日 12:53:35[gauss](https://me.csdn.net/mathlmx)阅读数：664
# 便利的开发工具-log4cpp快速使用指南
log4cpp是个基于LGPL的开源项目，是基于优秀的日志处理跟踪项目Java语言的log4j移植过来的。log4j介绍的文档很多，在java领域使用的也比较广泛，而这个功能强大的库对国内的C++语言开发人员却使用的不多。这里从开发人员使用的角度介绍这个库，使开发人员用最少的代价尽快掌握这种技术。下面先简单介绍一下这个项目的优点（也是log4j的优点），然后分原理，手动使用步骤，配置文件驱动方式使用步骤，其他考虑等方面进行讨论。以下讨论基于log4cpp0.3.4b
## 1. 优点
- 
提供应用程序运行上下文，方便跟踪调试；
- 
可扩展的、多种方式记录日志，包括命令行、文件、回卷文件、内存、syslog服务器、Win事件日志等；
- 
可以动态控制日志记录级别，在效率和功能中进行调整；
- 
所有配置可以通过配置文件进行动态调整；
- 
多语言支持，包括Java（log4j），C++（log4cpp、log4cplus），C（log4c），python（log4p）等
## 2. 原理
log4cpp有3个主要的组件：categories（类别）、appenders（附加目的地）、和layouts（布局）。（为了方便大家理解，文中尽量使用英文原词）
appender类用来输出日志（被layout格式化后的）到一些设备上。比如文件、syslog服务、某个socket等。可以定义自己的appender类输出日志信息到别的设备上，比如应用自身的日子处理进程、数据库等。appender和layout的关系是layout附在appender上，appender类调用layout处理完日志消息后，记录到某个设备上。log4cpp当前提供以下appender：
log4cpp::IdsaAppender          // 发送到IDS或者logger, 详细见 http://jade.cs.uct.ac.za/idsa/log4cpp::FileAppender          // 输出到文件
log4cpp::RollingFileAppender   // 输出到回卷文件，即当文件到达某个大小后回卷
log4cpp::OstreamAppender       // 输出到一个ostream类
log4cpp::RemoteSyslogAppender  // 输出到远程syslog服务器
log4cpp::StringQueueAppender   // 内存队列
log4cpp::SyslogAppender        // 本地sysloglog4cpp::Win32DebugAppender    // 发送到缺省系统调试器
log4cpp::NTEventLogAppender    // 发送到win 事件日志 
log4:cpp::SmptAppender	    //发送到mail事件日志 
layout类控制输出日志消息的显示样式（看起来像什么）。log4cpp当前提供以下layout格式：
log4cpp::BasicLayout                    // 以“时间戳 优先级（priority，下文介绍）
                       		        // 类别（category，下文介绍）
                     		        // NDC标签（nested diagnostic contexts 下文介绍）: 日志信息”。
                     			// 如：1056638652 INFO main : This is some infolog4cpp::PatternLayout  		// 让用户根据类似于 C 语言 printf 函数的转换模式
log4cpp::SimpleLayout   		// 以“优先级（priority） - 日志信息”格式显示。
//来指定输出格式。格式定义见代码附带文档。
category类真正完成记录日志功能，两个主要组成部分是appenders和priority（优先级）。优先级控制哪类日志信息可以被这个category记录，当前优先级分为：NOTSET,DEBUG,
 INFO, NOTICE, WARN, ERROR, CRIT, ALERT 或 
FATAL/EMERG。每个日志信息有个优先级，每个category有个优先级，当消息的优先级大于等于category的优先级时，这个消息才会被category记录，否则被忽略。优先级的关系如下。category类和appender的关系是，多个appender附在category上，这样一个日志消息可以同时输出到多个设备上。
NOTSET < DEBUG < INFO <NOTICE < WARN < ERROR < CRIT < ALERT < FATAL = EMERG
category被组织成一个树，子category创建时优先级缺省NOTSET，category缺省会继承父category的appender。而如果不希望这种appender的继承关系，log4cpp允许使用additivity标签，为false时新的appender取代category的appender列表。
为了更好的理解上面的概念下面以手动使用方式举例.
**3.手动使用步骤**
手动使用log4cpp的基本步骤如下：
- 
实例化一个layout对象；
- 
初始化一个appender对象；
- 
把layout对象附着在appender对象上；
- 
调用log4cpp::Category::getInstance("name").实例化一个category对象；
- 
把appender对象附到category上（根据additivity的值取代其他appender或者附加在其他appender后）。
- 
设置category的优先级；
// FileName: test_log4cpp1.cpp// Test log4cpp by manual operation.// Announce: use as your own risk.// Compile : g++ -otest1 -llog4cpp test_log4cpp1.cpp// Run     : ./test1// Tested  : RedHat 7.2 log4cpp0.3.4b// Author  : liqun (liqun@nsfocus.com)// Data    : 2003-6-27#include        "log4cpp/Category.hh"#include        "log4cpp/FileAppender.hh"#include        "log4cpp/BasicLayout.hh"int main(int argc, char* argv[]){// 1实例化一个layout 对象
        log4cpp::Layout* layout = new log4cpp::BasicLayout();// 2. 初始化一个appender 对象
        log4cpp::Appender* appender = new log4cpp::FileAppender("FileAppender","./test_log4cpp1.log");// 3. 把layout对象附着在appender对象上
        appender->setLayout(layout);// 4. 实例化一个category对象
        log4cpp::Category& warn_log = log4cpp::Category::getInstance("mywarn");// 5. 设置additivity为false，替换已有的appenderwarn_log.setAdditivity(false);// 5. 把appender对象附到category上
        warn_log.setAppender(appender);// 6. 设置category的优先级，低于此优先级的日志不被记录
        warn_log.setPriority(log4cpp::Priority::WARN);// 记录一些日志
        warn_log.info("Program info which cannot be wirten");warn_log.debug("This debug message will fail to write");warn_log.alert("Alert info");// 其他记录日志方式
        warn_log.log(log4cpp::Priority::WARN, "This will be a logged warning");log4cpp::Priority::PriorityLevel priority;bool this_is_critical = true;if(this_is_critical)priority = log4cpp::Priority::CRIT;elsepriority = log4cpp::Priority::DEBUG;warn_log.log(priority,"Importance depends on context");warn_log.critStream() << "This will show up << as " << 1 << " critical message" << log4cpp::CategoryStream::ENDLINE;// clean up and flush all appenderslog4cpp::Category::shutdown();return 0;}
## 4. 配置文件驱动方式使用步骤
另一个非常优秀的特征就是通过读取配置文件，确定category、appender、layout等对象。也是我们非常推荐的使用方式，可以灵活地通过配置文件定义所有地对象及其属性，不用重新编码，动态更改日志记录的策略。
Log4cpp 主要提供了log4cpp::PropertyConfigurator和log4cpp::SimpleConfigurator两种机制（文件格式），但log4cpp::SimpleConfigurator将来不再支持了，而且格式非常简单，这里就不多说明，自己看源码吧。
配置文件的格式和log4j的配置文件一样，是标准的java属性文件格式。下面是附带的例子配置文件：
# a simple test config#定义了3个category sub1, sub2, sub1.sub2log4j.rootCategory=DEBUG, rootAppenderlog4j.category.sub1=,A1log4j.category.sub2=INFOlog4j.category.sub1.sub2=ERROR, A2# 设置sub1.sub2 的additivity属性
log4j.additivity.sub1.sub2=false#定义rootAppender类型和layout属性
log4j.appender.rootAppender=org.apache.log4j.ConsoleAppenderlog4j.appender.rootAppender.layout=org.apache.log4j.BasicLayout#定义A1的属性
log4j.appender.A1=org.apache.log4j.FileAppenderlog4j.appender.A1.fileName=A1.loglog4j.appender.A1.layout=org.apache.log4j.SimpleLayout#定义A2的属性
log4j.appender.A2=org.apache.log4j.ConsoleAppenderlog4j.appender.A2.layout=org.apache.log4j.PatternLayoutlog4j.appender.A2.layout.ConversionPattern=The message '%m' at time %d%n
配置文件语法如下，不是很规范，结合上面的例子，应该可以看懂。
log4j / log4cpp . [category / appender].[category or appender 's name].[category or appender 's property] = [Appender / Layout / property's value / Priority, appender name1 [appender name2 ...]][appender]{ConsoleAppender}{FileAppender}  // 当appender的类型是FileAppender时，可以定义它下面的属性。
      [fileName]  string  foobar  // 格式是：属性名 值的类型 缺省值
      [append]  bool  true{RollingFileAppender}[fileName]   string  foobar[maxFileSize]  num  10*1024*1024[maxBackupIndex]  num  1[append]  bool  true{SyslogAppender}[syslogName]  string  syslog[syslogHost]  string  localhost[facility]  num  -1  // * 8 to get LOG_KERN, etc. compatible values. [portNumber]  num  -1{IdsaAppender}[idsaName]  string  foobar{Win32DebugAppender}{NTEventLogAppender}[source]  string  foobar[threshold]  string ""  // 全部 
      // 如果此类型appender需要layout，必须定义此appender的下面属性
      [layout]{BasicLayout}  {SimpleLayout}{PatternLayout}    // 当layout的值是BasicLayout时，需要定义下面的属性。
          [ConversionPattern][rootCategory][additivity][category name]  bool  true
基本使用步骤是：
- 
读取解析配置文件；
- 
实例化category对象；
- 
正常使用这些category对象进行日志处理；
下面是个简单的使用代码，使用起来是非常方便的：
// FileName: test_log4cpp2.cpp// Test log4cpp by config file.// Announce: use as your own risk.// Compile : g++ -llog4cpp test_log4cpp2.cpp// Run     : ./a.out// Tested  : RedHat 7.2 log4cpp0.3.4b// Author  : liqun (liqun@nsfocus.com)// Data    : 2003-6-27#include "log4cpp/Category.hh"#include "log4cpp/PropertyConfigurator.hh"int main(int argc, char* argv[]){// 1 读取解析配置文件
        // 读取出错, 完全可以忽略，可以定义一个缺省策略或者使用系统缺省策略
        // BasicLayout输出所有优先级日志到ConsoleAppendertry { log4cpp::PropertyConfigurator::configure("./log4cpp.conf");} catch(log4cpp::ConfigureFailure& f) {std::cout << "Configure Problem " << f.what() << std::endl;return -1;}// 2 实例化category对象
        // 这些对象即使配置文件没有定义也可以使用，不过其属性继承其父category// 通常使用引用可能不太方便，可以使用指针，以后做指针使用
        // log4cpp::Category* root = &log4cpp::Category::getRoot();log4cpp::Category& root = log4cpp::Category::getRoot();log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));log4cpp::Category& sub3 = log4cpp::Category::getInstance(std::string("sub1.sub2"));// 3 正常使用这些category对象进行日志处理。
        // sub1 has appender A1 and rootappender.sub1.info("This is some info");sub1.alert("A warning");// sub3 only have A2 appender.sub3.debug("This debug message will fail to write");sub3.alert("All hands abandon ship");sub3.critStream() << "This will show up << as " << 1 << " critical message" << log4cpp::CategoryStream::ENDLINE;sub3 << log4cpp::Priority::ERROR << "And this will be an error"  << log4cpp::CategoryStream::ENDLINE;sub3.log(log4cpp::Priority::WARN, "This will be a logged warning");return 0;}
## 5. 相关考虑
性能问题，可能是很多想使用log4cpp的程序员关心的问题。在参考资料2中有一段描述。结论就是log4j以及log4cpp是以性能为首要目标的；如果关闭日志记录的话，对性能影响可以忽略；打开日志记录，主要消耗是在记录动作，而不是库的管理过程；所以你尽可放心的使用。实在要深究性能的话。可以从下面方面提高：
输出的日志消息不要使用复杂的转换或者处理，比如：sub1.debug(string("Currentnum is") + i + GetCurStat());这种情况即使不进行日志处理，括号中的语句还是会执行。变通方法是：
if(sub1.isDebugEnabled()){sub1.debug(string("Current num is") + i + GetCurStat());}
安全性问题对于商业软件开发可能也是问题。可能不希望别人通过修改配置文件获取程序的调试等程序内部运行情况的日志信息。比较稳妥的方案或者是加密配置文件，运行中解密，输出到临时文件后读取；或者在发行版本里读取配置文件后，强行把低于某个优先级的category设到比较高的优先级。
多线程安全性问题。当前log4cpp还没有宣称自己是多线程安全的，不过其代码中大多数可能冲突的地方都增加了线程互斥控制，对多线程环境应该问题不大。但为了加入这个特性，linux下编译log4cpp时，configure请加入--with-pthreads或者—with-omnithreads选项。Win版本已经加入对MS线程的支持。
reference from:http://www.ibm.com/developerworks/cn/linux/l-log4cpp/
## 参考资料 
- 
1.Logging and Tracing in C++ Simplified: Traveling the Road LastTraveled by a Previously Invented Wheel
[http://soldc.sun.com/articles/logging.html](http://soldc.sun.com/articles/logging.html)
- 
2.log4j提供了对日志记录的控制
[http://www.ibm.com/developerworks/cn/java/jw-log4j/index.shtml](http://www.ibm.com/developerworks/cn/java/jw-log4j/index.html)
- 
3.使用Log4j进行日志操作
[http://www.ibm.com/developerworks/cn/java/l-log4j/index.shtml](http://www.ibm.com/developerworks/cn/java/l-log4j/index.html)
- 
4.log4cpp主页
[http://sourceforge.net/projects/log4cpp/](http://sourceforge.net/projects/log4cpp/)
- 
5.log4j主页
[http://jakarta.apache.org/log4j/docs/index.html](http://jakarta.apache.org/log4j/docs/index.html)
- 
6.log4cplus主页
[http://log4cplus.sourceforge.net/](http://log4cplus.sourceforge.net/)
- 
7. log4c主页
[http://log4c.sourceforge.net/](http://log4c.sourceforge.net/)
