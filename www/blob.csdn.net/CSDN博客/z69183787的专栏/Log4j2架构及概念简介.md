# Log4j2架构及概念简介 - z69183787的专栏 - CSDN博客
2016年06月28日 15:29:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：672
log4j——Log for java。
此文为读log4j2 user guaid时的翻译及笔记。log4j2与log4j在Logger的继承关系和配置方式上都做出了修改。个人感觉比较有意思的是Logger对象与LoggerConfig解耦的设计，以及Filter中的传递机制，有点像网络包分发，不过多了很多可调控性。
# 前言
log4j2可以按照开发人员预先的设定，在指定的位置和情况下打印log语句，并且可以酌情关闭某些log语句，如开发阶段debug类型的语句等。并且，可以使用layout来定义输出语句的格式，像C语言的printf函数一样。如：
![](http://images.cnitblog.com/blog/189953/201301/14194920-deda5c813e7940b5802e711d8c1cea32.jpg)
要实现这样标准化的日志输出，只需要在工程中引入log4j2的相关jar包，并向LogManager对象申请一个Logger对象的引用，然后调用该对象的相应方法即可，如：
![](http://images.cnitblog.com/blog/189953/201301/14195536-c6ee8bc76fe14c70873eb76972ff3ad1.jpg)
在log4j2中，一共有五种log level，分别为TRACE, DEBUG,INFO, WARN, ERROR 以及FATAL。
- FATAL：用在极端的情形中，即必须马上获得注意的情况。这个程度的错误通常需要触发运维工程师的寻呼机。
- ERROR：显示一个错误，或一个通用的错误情况，但还不至于会将系统挂起。这种程度的错误一般会触发邮件的发送，将消息发送到alert list中，运维人员可以在文档中记录这个bug并提交。
- WARN：不一定是一个bug，但是有人可能会想要知道这一情况。如果有人在读log文件，他们通常会希望读到系统出现的任何警告。
- INFO：用于基本的、高层次的诊断信息。在长时间运行的代码段开始运行及结束运行时应该产生消息，以便知道现在系统在干什么。但是这样的信息不宜太过频繁。
- DEBUG：用于协助低层次的调试。
- TRACE：用于展现程序执行的轨迹。
# Log4j2类图
![](http://images.cnitblog.com/blog/189953/201301/14192611-69d263d97294475f90416842f1bf4876.jpg)
通过类图可用看到：
每一个log上下文对应一个configuration，configuration中详细描述了log系统的各个LoggerConfig、Appender（输出目的地）、EventLog过滤器等。每一个Logger又与一个LoggerConfig相关联。另外，可以看到Filter的种类很多，有聚合在Configuration中的filter、有聚合在LoggerConfig中的filter也有聚合在Appender中的filter。不同的filter在过滤LogEvent时的行为和判断依据是不同的，具体可参加本文后面给出的文档。
应用程序通过调用log4j2的API并传入一个特定的名称来向LogManager请求一个Logger实例。LogManager会定位到适当的 LoggerContext 然后通过它获得一个Logger。如果LogManager不得不新建一个Logger，那么这个被新建的Logger将与LoggerConfig相关联，这个LoggerConfig的名称中包含如下信息中的一种：①与Logger名称相同的②父logger的名称③ root 。当一个LoggerConfig的名称与一个Logger的名称可以完全匹配时，Logger将会选择这个LoggerConfig作为自己的配置。如果不能完全匹配，那么Logger将按照最长匹配串来选择自己所对应的LoggerConfig。LoggerConfig对象是根据配置文件来创建的。LoggerConfig会与Appenders相关联，Appenders用来决定一个log
 request将被打印到那个目的地中，可选的打印目的地很多，如console、文件、远程socket server等。。LogEvent是由Appenders来实际传递到最终输出目的地的，而在EvenLog到达最终被处理之前，还需要经过若干filter的过滤，用来判断该EventLog应该在何处被转发、何处被驳回、何处被执行。
# Log4j中各个概念的简要介绍
Logger间的层次关系
相比于纯粹的System.out.println方式，使用logging API的最首要以及最重要的优势是可以在禁用一些log语句块的同时允许其他的语句块的输出。这一能力建立在一种假设之上，即所有在应用中可能出现的logging语句可以按照开发者定义的标准分成不同的类型。
在 Log4j 1.x版本时，Logger的层次是靠Logger类之间的关系来维护的。但在Log4j2中， Logger的层次则是靠LoggerConfig对象之间的关系来维护的。
Logger和LoggerConfig均是有名称的实体。Logger的命名是大小写敏感的，并且服从如下的分层命名规则。（与java包的层级关系类似）。例如：com.foo是com.foo.Bar的父级；java是java.util的父级，是java.util.vector的祖先。
 root LoggerConfig位于LoggerConfig层级关系的最顶层。它将永远存在与任何LoggerConfig层次中。任何一个希望与root LoggerConfig相关联的Logger可以通过如下方式获得：
Logger logger = LogManager.getLogger(LogManager.ROOT_LOGGER_NAME);
其他的Logger实例可以调用LogManager.getLogger 静态方法并传入想要得到的Logger的名称来获得。
LoggerContext
LoggerContext在Logging System中扮演了锚点的角色。根据情况的不同，一个应用可能同时存在于多个有效的LoggerContext中。在同一LoggerContext下，log system是互通的。如：Standalone Application、Web Applications、Java EE Applications、"Shared" Web Applications 和REST Service Containers，就是不同广度范围的log上下文环境。
Configuration
每一个LoggerContext都有一个有效的Configuration。Configuration包含了所有的Appenders、上下文范围内的过滤器、LoggerConfigs以及StrSubstitutor.的引用。在重配置期间，新与旧的Configuration将同时存在。当所有的Logger对象都被重定向到新的Configuration对象后，旧的Configuration对象将被停用和丢弃。
Logger
如前面所述， Loggers 是通过调用LogManager.getLogger方法获得的。Logger对象本身并不实行任何实际的动作。它只是拥有一个name 以及与一个LoggerConfig相关联。它继承了AbstractLogger类并实现了所需的方法。当Configuration改变时，Logger将会与另外的LoggerConfig相关联，从而改变这个Logger的行为。
获得Logger：
使用相同的名称参数来调用getLogger方法将获得来自同一个Logger的引用。如：
Logger x = Logger.getLogger("wombat");
Logger y = Logger.getLogger("wombat");
x和y指向的是同一个Logger对象。
log4j环境的配置是在应用的启动阶段完成的。优先进行的方式是通过读取配置文件来完成。
log4j使采用类名（包括完整路径）来定义Logger 名变得很容易。这是一个很有用且很直接的Logger命名方式。使用这种方式命名可以很容易的定位这个log message产生的类的位置。当然，log4j也支持任意string的命名方式以满足开发者的需要。不过，使用类名来定义Logger名仍然是最为推崇的一种Logger命名方式。
LoggerConfig
当Logger在configuration中被描述时，LoggerConfig对象将被创建。LoggerConfig包含了一组过滤器。LogEvent在被传往Appender之前将先经过这些过滤器。过滤器中包含了一组Appender的引用。Appender则是用来处理这些LogEvent的。
Log层次：
每一个LoggerConfig会被指定一个Log层次。可用的Log层次包括TRACE, DEBUG,INFO, WARN, ERROR 以及FATAL。需要注意的是，在log4j2中，Log的层次是一个Enum型变量，是不能继承或者修改的。如果希望获得跟多的分割粒度，可用考虑使用Markers来替代。
在Log4j 1.x 和Logback 中都有“层次继承”这么个概念。但是在log4j2中，由于Logger和LoggerConfig是两种不同的对象，因此“层次继承”的概念实现起来跟Log4j 1.x 和Logback不同。具体情况下面的五个例子。
例子一：
![](http://images.cnitblog.com/blog/189953/201301/14192513-8e6c7f34d2684c6aae98a7542973aa8d.jpg)
可用看到，应用中的LoggerConfig只有root这一种。因此，对于所有的Logger而言，都只能与该LoggerConfig相关联而没有别的选择。
例子二：
![](http://images.cnitblog.com/blog/189953/201301/14192503-b8fbb92cd926486fba2e4402988b3d4f.jpg)
在例子二中可以看到，有5种不同的LoggerConfig存在于应用中，而每一个Logger都被与最匹配的LoggerConfig相关联着，并且拥有不同的Log Level。
例子三：
![](http://images.cnitblog.com/blog/189953/201301/14192454-1bb1a23b0cdf44efbd398cea688c1df3.jpg)
可以看到Logger root、X、X.Y.Z都找到了与各种名称相同的LoggerConfig。而LoggerX.Y没有与其名称相完全相同的LoggerConfig。怎么办呢？它最后选择了X作为它的LoggerConfig，因为X LoggerConfig拥有与其最长的匹配度。
例子四：
![](http://images.cnitblog.com/blog/189953/201301/14192446-39c68aabb8b24929bbe253499ff83ddc.jpg)
可以看到，现在应用中有两个配置好的LoggerConfig：root和X。而Logger有四个：root、X、X.Y、X.Y.Z。其中，root和X都能找到完全匹配的LoggerConfig，而X.Y和X.Y.Z则没有完全匹配的LoggerConfig，那么它们将选择哪个LoggerConfig作为自己的LoggerConfig呢？由图上可知，它们都选择了X而不是root作为自己的LoggerConfig，因为在名称上，X拥有最长的匹配度。
例子五
![](http://images.cnitblog.com/blog/189953/201301/14192434-abddc8beb57a46ec92e151ffe255a1c4.jpg)
可以看到，现在应用中有三个配置好的LoggerConfig，分别为：root、X、X.Y。同时，有四个Logger，分别为：root、X、X.Y以及X.YZ。其中，名字能完全匹配的是root、X、X.Y。那么剩下的X.YZ应该匹配X还是匹配X.Y呢？答案是X。因为匹配是按照标记点（即“.”）来进行的，只有两个标记点之间的字串完全匹配才算，否则将取上一段完全匹配的字串的长度作为最终匹配长度。
Filter
与防火墙过滤的规则相似，log4j2的过滤器也将返回三类状态：Accept（接受）, Deny（拒绝） 或Neutral（中立）。其中，Accept意味着不用再调用其他过滤器了，这个LogEvent将被执行；Deny意味着马上忽略这个event，并将此event的控制权交还给过滤器的调用者；Neutral则意味着这个event应该传递给别的过滤器，如果再没有别的过滤器可以传递了，那么就由现在这个过滤器来处理。
Appender
由logger的不同来决定一个logging request是被禁用还是启用只是log4j2的情景之一。log4j2还允许将logging request中log信息打印到不同的目的地中。在log4j2的世界里，不同的输出位置被称为Appender。目前，Appender可以是console、文件、远程socket服务器、Apache Flume、JMS以及远程 UNIX 系统日志守护进程。一个Logger可以绑定多个不同的Appender。
可以调用当前Configuration的addLoggerAppender函数来为一个Logger增加。如果不存在一个与Logger名称相对应的LoggerConfig，那么相应的LoggerConfig将被创建，并且新增加的Appender将被添加到此新建的LoggerConfig中。尔后，所有的Loggers将会被通知更新自己的LoggerConfig引用（PS：一个Logger的LoggerConfig引用是根据名称的匹配长度来决定的，当新的LoggerConfig被创建后，会引发一轮配对洗牌）。
在某一个Logger中被启用的logging request将被转发到该Logger相关联的的所有Appenders上，并且还会被转发到LoggerConfig的父级的Appenders上。
这样会产生一连串的遗传效应。例如，对LoggerConfig B来说，它的父级为A，A的父级为root。如果在root中定义了一个Appender为console，那么所有启用了的logging request都会在console中打印出来。另外，如果LoggerConfig A定义了一个文件作为Appender，那么使用LoggerConfig A和LoggerConfig B的logger 的logging request都会在该文件中打印，并且同时在console中打印。
如果想避免这种遗传效应的话，可以在configuration文件中做如下设置：
additivity="false"
这样，就可以关闭Appender的遗传效应了。具体解释见：
![](http://images.cnitblog.com/blog/189953/201301/14192413-ec04b03336b04924a39cc291ebfcd72c.jpg)
Layout
通常，用户不止希望能定义log输出的位置，还希望可以定义输出的格式。这就可以通过将Appender与一个layout相关联来实现。Log4j中定义了一种类似C语言printf函数的打印格式，如"%r [%t] %-5p %c - %m%n" 格式在真实环境下会打印类似如下的信息：
176 [main] INFO  org.foo.Bar - Located nearest gas station.
其中，各个字段的含义分别是：
%r 指的是程序运行至输出这句话所经过的时间（以毫秒为单位）；
%t 指的是发起这一log request的线程；
%c 指的是log的level；
%m 指的是log request语句携带的message。
%n 为换行符。
 Reference
《[log4j user guide](http://logging.apache.org/log4j/2.x/log4j-users-guide.pdf)》
