
# maven项目使用log4j - anlcy - 博客园






# [maven项目使用log4j](https://www.cnblogs.com/camilla/p/8334402.html)
日志是应用软件中不可缺少的部分，Apache的开源项目 log4j 是一个功能强大的日志组件,提供方便的日志记录。
1.maven项目在pom.xml导入log4j依赖：
<dependency>
<groupId>log4j</groupId>
<artifactId>log4j</artifactId>
<version>1.2.17</version>
</dependency>
2.新建 log4j.properties 放到src\main\xxx目录下（log4j默认读取跟目录下名字为 log4j.properties 文件），文件配示例如下：
\#设置日志的级别，定义日志信息的输出目的
log4j.rootLogger=DEBUG, A1 ,R
\#定义A1的输出目的地为控制台
log4j.appender.A1=org.apache.log4j.ConsoleAppender
\#布局为 PatternLayout 可以灵活地指定布局模式。
log4j.appender.A1.layout=org.apache.log4j.PatternLayout
\#设置输出格式
log4j.appender.A1.layout.ConversionPattern=%-d{yyyy-MM-dd HH\:mm\:ss} [%c]-[%p] %m%n
\#定义R的输出目的地为文件，并且文件大小到达指定尺寸的时候产生一个新的文件
log4j.appender.R=org.apache.log4j.RollingFileAppender
\#设置输出的文件地址
log4j.appender.R.File=D:\\Test_Log4j.log
\#设置文件大小伟100 kb 文件到达100时，产生一个新文件，
\#MaxBackupIndex 最大记录的文件数为1 查过一个文件删除文件较早的。
log4j.appender.R.MaxFileSize=100KB log4j.appender.R.MaxBackupIndex=1
\#以下和上面一样
log4j.appender.R.layout=org.apache.log4j.PatternLayout
log4j.appender.R.layout.ConversionPattern=%p %t %c - %m%n
3.创建测试类
import org.apache.log4j.Logger;
public class Test {
private static Logger logger = Logger.getLogger(Test.class);
public static void main(String[] args) {
// 记录debug级别的信息
logger.debug("This is debug message.");
// 记录info级别的信息
logger.info("This is info message.");
// 记录error级别的信息
logger.error("This is error message.");
}
}
4.运行程序test查看结果在D:\\Test_Log4j.log





