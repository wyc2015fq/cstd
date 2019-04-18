# slf4j介绍以及实现原理窥探 - z69183787的专栏 - CSDN博客
2017年07月10日 14:03:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：289
个人分类：[日志-Log4j&Log4j2](https://blog.csdn.net/z69183787/article/category/2175479)

一、概述
　　slf4j(全称是Simple Loging Facade For Java)是一个为Java程序提供日志输出的统一接口，并不是一个具体的日志实现方案，就好像我们经常使用的JDBC一样，只是一种规则而已。因此单独的slf4j是不能工作的，它必须搭配其他具体的日志实现方案，比如apache的org.apache.log4j.Logger，jdk自带的java.util.logging.Logger等等。
　　其中对与jar包：
　　　　slf4j-log4j12-x.x.x.jar是使用org.apache.log4j.Logger提供的驱动
　　　　slf4j-jdk14-x.x.x.jar是使用java.util.logging提供的驱动
　　　　slf4j-simple-x.x.x.jar直接绑定System.err
　　　　slf4j-jcl-x.x.x.jar是使用commons-logging提供的驱动
　　　　logback-classic-x.x.x.jar是使用logback提供的驱动
二、slf4j优势
　　1.与客户端很好的解耦
　　　　比如：我们发现了一位大牛开发了一个非常好而且又刚好能够满足自己需求的类库，类库里使用了apache的org.apache.log4j.Logger，然而你自己的程序在开发的时候使用的是jdk自带的java.util.logging.Logger，那么现在忧伤的问题来了：如果你想要使用，你是不是需要同时支持log4j和jdk两种日志系统？这样的话，你就需要添加两个实现同样功能的jar包并且维护两套日子配置，你是不是需要耗费更多的精力来进行维护？此时宝宝心里苦，宝宝不说。
　　2.节省内存
　　　　log4j这些传统的日志系统里面并没有占位符的概念，当我们需要打印信息的时候，我们需要如下方式进行使用。
```
1 package com.hafiz.zhang;
 2 
 3 import org.apache.log4j.Logger;
 4 
 5 /**
 6  * @author hafiz.zhang
 7  * @date 16/5/12 18:01
 8  */
 9 public class TestLog4j {
10 private static final Logger LOGGER = Logger.getLogger(TestLog4j.class);
11 
12 public static void main(String[] args) {
13         String message = "服务器出错啦.";
14         LOGGER.info("Error message is : " + message);
15     }
16 }
```
查看源码，我们发现了log4j的info函数有两种方式可供选择：
```
1 public void info(Objectmessage)
2 public void info(Objectmessage, Throwable t)
```
第一个参数是要输出的信息，假设我们要输出的是一个字符串，并且字符串中包含变量，则Objectmessage参数就必须使用字符串相加操作，就比如上面测试代码的14行一样。姑且不说字符串相加是一个比较消耗性能的操作，字符串是一个不可变对象，一旦创建就不能被修改，创建的字符串会保存在String池中，占用内存。更糟糕的是如果配置文件中配置的日志级别是ERROR的话，这行info日志根本不会输出，则相加得到的字符串对象是一个非必须对象，白白浪费了内存空间。这时候有人会说了，那我可以这样写啊：
```
1 package hafiz.zhang;
 2  
 3  import org.apache.log4j.Logger;
 4  
 5  /**
 6   * @author hafiz.zhang
 7   * @date 16/5/12 18:04
 8   */
 9  public class TestLog4j {
10  private static final Logger LOGGER = Logger.getLogger(TestLog4j.class);
11  
12  public static void main(String[] args) {
13          String message = "服务器出错啦.";
14  if (LOGGER.isInfoEnabled()) {
15              LOGGER.info("Error message is: " + message);
16          }
17      }
18  }
```
这样不就解决了白白浪费内存的问题了吗？没错，这是一个变通方案，但是这样的代码太繁琐，不直观！
下面再来看看slf4j的打日志的方式：(爽爆了)
```
1 package com.hafiz.zhang;
 2 
 3 
 4 import org.slf4j.Logger;
 5 import org.slf4j.LoggerFactory;
 6 
 7 /**
 8  * @author hafiz.zhag
 9  * @date 15/8/26 21:54
10  */
11 public class TestLog4j {
12 private static final Logger LOGGER = LoggerFactory.getLogger(TestLog4j.class);
13 
14 public static void main(String[] args) {
15         String message = "服务器出错啦.";
16         LOGGER.info("Error message is: {}", message);
17     }
18 }
```
看到没有，打日志的时候使用了{}占位符，这样就不会有字符串拼接操作，减少了无用String对象的数量，节省了内存。并且记住，在生产最终日志信息的字符串之前，这个方法会检查一个特定的日志级别是不是打开了，这不仅降低了内存消耗而且预先降低了CPU去处理字符串连接命令的时间。这里是使用SLF4J日志方法的代码，来自于slf4j-log4j12-1.6.1.jar中的Log4j的适配器类Log4jLoggerAdapter。
三、slf4j的使用方法以及实现原理
　　上面我们提到了slf4j是不能够独立工作的，要想使用我们必须带上其他的具体日志实现方案，下面我们就以log4j为例进行使用slf4j,我们需要做的工作如下：（下面的xxx表示jar包具体版本号）
　　　　1.将slf4j-api-xxx.jar加入工程classpath中
　　　　2.将slf4j-log4jxx-xxx.jar加入工程classpath中
　　　　3.将log4j-xxx.jar加入工程classpath中
　　　　4.将log4j.properties（log4j.xml）文件加入工程classpath中（与spring继承 还能使用自定义文件位置的方式指定，后续博客中我会介绍）
注：如果项目是maven项目，则前三步就变成一步，在pom.xml文件中添加以下依赖。（如果没有更高版本的slf4j-api和log4j要求，则只添加第一条依赖就可以，因为slf4j-log4j12依赖会包含slf4j-api和log4j依赖）
```
1 <dependency>
 2    <groupId>org.slf4j</groupId>
 3    <artifactId>slf4j-log4j12</artifactId>
 4    <version>1.6.4</version>
 5 </dependency>
 6 <dependency>
 7    <groupId>org.slf4j</groupId>
 8    <artifactId>slf4j-api</artifactId>
 9    <version>1.6.4</version>
10 </dependency>
11 <dependency>
12    <groupId>log4j</groupId>
13    <artifactId>log4j</artifactId>
14    <version>1.2.16</version>
15 </dependency>
```
　　slf4j工作原理窥探
　　　　首先，slf4j-api作为slf4j的接口类，使用在程序代码中，这个包提供了一个Logger类和LoggerFactory类，Logger类用来打日志，LoggerFactory类用来获取Logger;slf4j-log4j是连接slf4j和log4j的桥梁，怎么连接的呢？我们看看slf4j的LoggerFactory类的getLogger函数的源码：
```
1 /**
 2    * Return a logger named according to the name parameter using the statically
 3    * bound {@link ILoggerFactory} instance.
 4    * 
 5    * @param name
 6    *          The name of the logger.
 7    * @return logger
 8    */
 9   public static Logger getLogger(String name) {
10     ILoggerFactory iLoggerFactory = getILoggerFactory();
11     return iLoggerFactory.getLogger(name);
12   }
13 
14   /**
15    * Return a logger named corresponding to the class passed as parameter, using
16    * the statically bound {@link ILoggerFactory} instance.
17    * 
18    * @param clazz
19    *          the returned logger will be named after clazz
20    * @return logger
21    */
22   public static Logger getLogger(Class clazz) {
23     return getLogger(clazz.getName());
24   }
25 
26   /**
27    * Return the {@link ILoggerFactory} instance in use.
28    * 
29    * <p>
30    * ILoggerFactory instance is bound with this class at compile time.
31    * 
32    * @return the ILoggerFactory instance in use
33    */
34   public static ILoggerFactory getILoggerFactory() {
35     if (INITIALIZATION_STATE == UNINITIALIZED) {
36       INITIALIZATION_STATE = ONGOING_INITILIZATION;
37       performInitialization();
38 
39     }
40     switch (INITIALIZATION_STATE) {
41     case SUCCESSFUL_INITILIZATION:
42       return StaticLoggerBinder.getSingleton().getLoggerFactory();
43     case NOP_FALLBACK_INITILIZATION:
44       return NOP_FALLBACK_FACTORY;
45     case FAILED_INITILIZATION:
46       throw new IllegalStateException(UNSUCCESSFUL_INIT_MSG);
47     case ONGOING_INITILIZATION:
48       // support re-entrant behavior.
49       // See also http://bugzilla.slf4j.org/show_bug.cgi?id=106
50       return TEMP_FACTORY;
51     }
52     throw new IllegalStateException("Unreachable code");
53   }
```
查找到现在，我们发现LoggerFactory.getLogger()首先获取一个ILoggerFactory接口，然后使用该接口获取具体的Logger。获取ILoggerFactory的时候用到了一个StaticLoggerBinder类，仔细研究我们会发现StaticLoggerBinder这个类并不是slf4j-api这个包中的类，而是slf4j-log4j包中的类，这个类就是一个中间类，它用来将抽象的slf4j变成具体的log4j，也就是说具体要使用什么样的日志实现方案，就得靠这个StaticLoggerBinder类。
![](http://images2015.cnblogs.com/blog/894443/201605/894443-20160512182041343-657702165.png)
再看看slf4j-log4j包种的这个StaticLoggerBinder类创建ILoggerFactory长什么样子：
```
1 /**
 2    * The ILoggerFactory instance returned by the {@link #getLoggerFactory}
 3    * method should always be the same object
 4    */
 5   private final ILoggerFactory loggerFactory;
 6 
 7   private StaticLoggerBinder() {
 8     loggerFactory = new Log4jLoggerFactory();
 9     try {
10       Level level = Level.TRACE;
11     } catch (NoSuchFieldError nsfe) {
12       Util
13           .report("This version of SLF4J requires log4j version 1.2.12 or later. See also http://www.slf4j.org/codes.html#log4j_version");
14     }
15   }
16 
17   public ILoggerFactory getLoggerFactory() {
18     return loggerFactory;
19   }
20 
21   public String getLoggerFactoryClassStr() {
22     return loggerFactoryClassStr;
23   }
```
可以看到slf4j-log4j中的StaticLoggerBinder类创建的ILoggerFactory其实是一个org.slf4j.impl.Log4jLoggerFactory，这个类的getLogger函数代码如下：
```
1 /*
 2    * (non-Javadoc)
 3    * 
 4    * @see org.slf4j.ILoggerFactory#getLogger(java.lang.String)
 5    */
 6   public Logger getLogger(String name) {
 7     Logger slf4jLogger = null;
 8     // protect against concurrent access of loggerMap
 9     synchronized (this) {
10         slf4jLogger = (Logger) loggerMap.get(name);
11       if (slf4jLogger == null) {
12         org.apache.log4j.Logger log4jLogger;
13         if(name.equalsIgnoreCase(Logger.ROOT_LOGGER_NAME)) {
14            log4jLogger = LogManager.getRootLogger();
15         } else {
16           log4jLogger = LogManager.getLogger(name);
17         }
18         slf4jLogger = new Log4jLoggerAdapter(log4jLogger);
19         loggerMap.put(name, slf4jLogger);
20       }
21     }
22     return slf4jLogger;
23   }
```
就在其中创建了真正的org.apache.log4j.Logger，也就是我们需要的具体的日志实现方案的Logger类。就这样，整个绑定过程就完成了。
