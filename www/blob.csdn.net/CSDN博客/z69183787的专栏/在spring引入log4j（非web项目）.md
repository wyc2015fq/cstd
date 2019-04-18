# 在spring引入log4j（非web项目） - z69183787的专栏 - CSDN博客
2018年03月24日 15:50:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1382
[https://blog.csdn.net/u012578322/article/details/78012183](https://blog.csdn.net/u012578322/article/details/78012183)
在spring中使用log4j
- 引入log4j软件包
- 配置log4j属性
- 加载log4j配置文件- 默认加载
- 手动加载
- 使用logger
本文的整体代码结构是在已经引入spring基本应用的前提下，在spring配置文件中通过@Bean注解创建一个Logger bean，然后在测试代码中使用。
**[java]**[view plain](https://blog.csdn.net/u012578322/article/details/78012183#)[copy](https://blog.csdn.net/u012578322/article/details/78012183#)
- import java.io.File;  
- import java.io.FileInputStream;  
- import java.io.FileNotFoundException;  
- import java.net.URL;  
- 
- import org.apache.log4j.Logger;  
- import org.apache.logging.log4j.core.config.ConfigurationSource;  
- import org.apache.logging.log4j.core.config.Configurator;  
- import org.springframework.context.annotation.Bean;  
- import org.springframework.context.annotation.ComponentScan;  
- import org.springframework.context.annotation.Configuration;  
- import org.springframework.context.annotation.EnableAspectJAutoProxy;  
- import org.springframework.context.support.PropertySourcesPlaceholderConfigurer;  
- 
- @Configuration
- @EnableAspectJAutoProxy
- @ComponentScan({"com.markey.messageboard.app.impl","com.markey.messageboard.aop"})  
- publicclass SpringConfig {  
- //  @Bean
- //  public static PropertySourcesPlaceholderConfigurer placeholderConfigurer(){
- //      PropertySourcesPlaceholderConfigurer placeholderConfigurer = new PropertySourcesPlaceholderConfigurer();
- //      return placeholderConfigurer;
- //  }
- @Bean
- publicstatic Logger logger(){  
-         String path="/com/log4j.properties";  
-         URL url=SpringConfig.class.getResource(path);  
-         ConfigurationSource source;  
- try {  
-             source = new ConfigurationSource(new FileInputStream(new File(url.getPath())),url);  
-             Configurator.initialize(null, source);  
-         } catch (FileNotFoundException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-         Logger logger = Logger.getLogger(SpringConfig.class);  
- return logger;  
-     }  
- }  
**[java]**[view plain](https://blog.csdn.net/u012578322/article/details/78012183#)[copy](https://blog.csdn.net/u012578322/article/details/78012183#)
- import org.apache.log4j.Logger;  
- import org.junit.Test;  
- import org.junit.runner.RunWith;  
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.beans.factory.annotation.Value;  
- import org.springframework.core.env.Environment;  
- import org.springframework.test.context.ContextConfiguration;  
- import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;  
- 
- import com.markey.messageboard.app.Dosomething;  
- import com.markey.messageboard.configs.SpringConfig;  
- import com.markey.messageboard.model.Messageboard;  
- 
- @RunWith(SpringJUnit4ClassRunner.class)  
- @ContextConfiguration(classes = SpringConfig.class)  
- //@PropertySource("classpath:/test.properties")
- //@PropertySource("classpath:/log4j.properties")
- publicclass helloSpring {  
- 
- @Autowired
-     Environment evn;  
- @Autowired
-     Dosomething dosomething;  
- @Autowired
-     Logger logger;  
- @Value("${god.hello}") String titleString;  
- @Test
- publicvoid testWorld(){  
- //      System.out.println(evn.containsProperty("god.hello"));
- //      Messageboard messageBoard = new Messageboard();
- //      messageBoard.setTitle(titleString);
- //      System.out.println(dosomething.readWall(messageBoard));
- //      dosomething.readWall(messageBoard);
-         logger.error("hello,i am error messages");  
-     }  
- 
- }  
# 一、引入log4j软件包
使用log4j涉及两个库文件：log4j和log4j-core
mava引用如下：
**[plain]**[view plain](https://blog.csdn.net/u012578322/article/details/78012183#)[copy](https://blog.csdn.net/u012578322/article/details/78012183#)
- <!--https://mvnrepository.com/artifact/log4j/log4j -->  
- 
- <dependency>  
- 
-    <groupId>log4j</groupId>  
- 
-    <artifactId>log4j</artifactId>  
- 
-    <version>1.2.17</version>  
- 
- </dependency>  
- 
- <!-- https://mvnrepository.com/artifact/org.apache.logging.log4j/log4j-core-->  
- 
- <dependency>  
- 
-    <groupId>org.apache.logging.log4j</groupId>  
- 
-    <artifactId>log4j-core</artifactId>  
- 
-    <version>2.7</version>  
- 
- </dependency>  
# 二、配置log4j属性
log4j配置文件支持xml格式和properites格式，下面为例properites介绍需要配置的内容。
配置文件分为三部分配置：
l  日志级别配置
l  输出目的地配置
l  输出样式配置
1.     日志级别配置
日志级别配置分为两种：根目录级别和包目录基本。
顾名思义，根目录级别就是定义总的日志级别，具体到某个特定的包路径下的类，还可以定制日志级别。
Log4j自定义的日志级别有：OFF >FATAL > ERROR > WARN > INFO > DEBUG > ALL
但是建议只使用ERROR > WARN > INFO > DEBUG这四种。通过在这里定义的级别，您可以控制到应用程序中相应级别的日志信息的开关。比如在这里定 义了 INFO 级别，则应用程序中所有 DEBUG 级别的日志信息将不被打印出来 。
**[plain]**[view plain](https://blog.csdn.net/u012578322/article/details/78012183#)[copy](https://blog.csdn.net/u012578322/article/details/78012183#)
- ###定义根日志级别为info###  
- ###定义com.markey.messageboard日志级别为debug###  
**[plain]**[view plain](https://blog.csdn.net/u012578322/article/details/78012183#)[copy](https://blog.csdn.net/u012578322/article/details/78012183#)
- ###定义com.markey.messageboard.springtest日志级别为info###  
log4j.rootLogger=infolog4j.logger.com.markey.messageboard=DEBUGlog4j.logger.com.markey.messageboard.springtest=info
2.     输出目的地配置
定义输出目的地，定义完成后需要配置根目录启用，可以定义多个目的地输出。
Log4j 提供的 appender 有以下几种：
1.    org.apache.log4j.ConsoleAppender（控制台），
2.    org.apache.log4j.FileAppender （文件），
3.    org.apache.log4j.DailyRollingFileAppender（每天产生一个日志文件），
4.    4.org.apache.log4j.RollingFileAppender（文件大小到达指定尺寸的时候产生一个   新的文件）
5.    org.apache.log4j.WriterAppender（将日志信息以流格式发送到任意指定的地方）
**[html]**[view plain](https://blog.csdn.net/u012578322/article/details/78012183#)[copy](https://blog.csdn.net/u012578322/article/details/78012183#)
- ###配置输出目的地###  
- log4j.rootLogger=info,<spanstyle="color:#FF0000;">stdout,errorfile,infofile </span>
- log4j.logger.com.markey.messageboard=DEBUG
- log4j.logger.com.markey.messageboard.springtest=info
- 
- #console config  
- log4j.appender.<spanstyle="color:#FF0000;">stdout</span>=org.apache.log4j.ConsoleAppender  
- 
- #errorfile config  
- log4j.appender.<spanstyle="color:#FF0000;">errorfile</span>=org.apache.log4j.DailyRollingFileAppender   
- log4j.appender.errorfile.File= c:/logs/app_logs/error.log   
- 
- #infofile config  
- log4j.appender.<spanstyle="color:#FF0000;">infofile</span>=org.apache.log4j.DailyRollingFileAppender   
- log4j.appender.infofile.File=c:/logs/app_logs/info.log  
3.     输出样式配置
定义日志样式，在输入目的地配置中使用
Log4j 提供的 layout 有以下4种：
　　       1.org.apache.log4j.HTMLLayout （以 HTML 表格形式布局），
　　       2.org.apache.log4j.PatternLayout （可以灵活地指定布局模式），
　　       3.org.apache.log4j.SimpleLayout （包含日志信息的级别和信息字符串），
　　       4.org.apache.log4j.TTCCLayout （包含日志产生的时间、线程、类别等等信息）
　　PatternLayout模式使用最多，其 用类似 C 语言中的 printf 函数的打印格式格式化日志信息，打印参数如下：
         %m 输出代码中指定的消息
　　 %p 输出优先级，即 DEBUG， INFO ， WARN ， ERROR ， FATAL
　　 %r 输出自应用启动到输出该log 信息耗费的毫秒数
　　 %c 输出所属的类目，通常就是所在类的全名
　　 %t 输出产生该日志事件的线程名
　　 %n 输出一个回车换行符，Windows 平台为 “rn” ， Unix 平台为 “n”
　　 %d 输出日志时间点的日期或时间
　　 %l 输出日志事件的发生位置，包括类目名、发生的线程，以及在代码中的行数。
Log4j还有其他一些配置：
l  指定输出到文件的日志级别：
l  指定输出文件路径
l  指定输出日志中的日期格式
l  指定日志日志文件写入方式是否为文件后追加
l  指定日志编码
**[plain]**[view plain](https://blog.csdn.net/u012578322/article/details/78012183#)[copy](https://blog.csdn.net/u012578322/article/details/78012183#)
- log4j.rootLogger=info,stdout,errorfile,infofile ###格式为[level] ,appender1，appender2，……  
- log4j.logger.com.markey.messageboard=DEBUG  
- log4j.logger.com.markey.messageboard.springtest=info  
- 
- #console config  
- log4j.appender.stdout=org.apache.log4j.ConsoleAppender ###定义一个控制台输出目的地  
- log4j.appender.stdout.layout=org.apache.log4j.PatternLayout  
- log4j.appender.stdout.layout.ConversionPattern=%m%n  
- log4j.appender.stdout.encoding=GB18030  
- 
- #errorfile config  
- log4j.appender.errorfile=org.apache.log4j.DailyRollingFileAppender ###定义一个每日日志输出目的地  
- log4j.appender.errorfile.File= c:/logs/app_logs/error.log ###输出文件路径  
- log4j.appender.errorfile.Threshold=error  ###只有error以上级别的日志才会被输出  
- log4j.appender.errorfile.DatePattern='.'yyyy-MM-dd ###指定日期格式  
- log4j.appender.errorfile.Append=true ###指定日志文件以追加方式写入  
- log4j.appender.errorfile.layout=org.apache.log4j.PatternLayout   
- log4j.appender.errorfile.layout.ConversionPattern=[%-5p]|%d|%C|%L|%m%n  
- log4j.appender.errorfile.encoding=GB18030 ###指定日志文件编码  
- 
- #infofile config  
- log4j.appender.infofile=org.apache.log4j.DailyRollingFileAppender   
- log4j.appender.infofile.File=c:/logs/app_logs/info.log  
- log4j.appender.infofile.Threshold=info ###只有error以上级别的日志才会被输出  
- log4j.appender.infofile.DatePattern='.'yyyy-MM-dd  
- log4j.appender.infofile.Append=true  
- log4j.appender.infofile.layout=org.apache.log4j.PatternLayout   
- log4j.appender.infofile.layout.ConversionPattern=[%-5p]|%d|%C|%L|%m%n  
- log4j.additivity.infofile=false  
- log4j.appender.infofile.encoding=GB18030  
# 三、加载log4j配置文件
## 1、默认加载
将log4j.properites文件放在src目录下，log4j框架会自动加载配置文件并应用。
例如我的配置文件放在如下位置，则无需其他配置，会被自动加载。
![](https://img-blog.csdn.net/20170917184323188?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjU3ODMyMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 2、手动加载
如果想要使用相对路径加载配置文件，则需要使用到log4j-core中的ConfigurationSource类和Configurator类。
例如我把配置文件放在如下src目录下代码的一级目录com下：
![](https://img-blog.csdn.net/20170917184429859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjU3ODMyMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
则需要手动进行配置文件的加载，主要逻辑就是：根据相对路劲找到配置文件，使用log4j的配置文件加载器Configurator来加载。
**[java]**[view plain](https://blog.csdn.net/u012578322/article/details/78012183#)[copy](https://blog.csdn.net/u012578322/article/details/78012183#)
- publicclass SpringConfig {  
- @Bean
- publicstatic PropertySourcesPlaceholderConfigurer placeholderConfigurer(){  
-         PropertySourcesPlaceholderConfigurer placeholderConfigurer = new PropertySourcesPlaceholderConfigurer();  
- return placeholderConfigurer;  
-     }  
- @Bean
- publicstatic Logger logger(){  
-         String path="/com/log4j.properties";//定义配置文件路径
-         URL url=SpringConfig.class.getResource(path);//转化配置文件路径
- try {  
-             ConfigurationSource source = new ConfigurationSource(  
- new FileInputStream(new File(url.getPath())),url);//找到配置文件
-             Configurator.initialize(null, source);//初始化配置
-         } catch (FileNotFoundException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-         Logger logger = Logger.getLogger(SpringConfig.class);  
- return logger;  
-     }  
- }  
