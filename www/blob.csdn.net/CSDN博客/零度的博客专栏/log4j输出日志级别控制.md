# log4j输出日志级别控制 - 零度的博客专栏 - CSDN博客
2016年04月27日 14:18:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：12424
我们知道：
log4j.logger.XX cover ==> log4j.rootLogger
log4j.appender.XX.Threshold决定了最低接收级别
也就是说rootLogger首先决定所有log的输出级别，而log4j.logger.XX是对XX路径下的jar包的输出级别的修改，直接覆盖rootLogger
但是对于不同的输出口，他们的接收级别是不同的。
举例1：
log4j.appender.XX.Threshold门槛高于log4j.logger.XX时，
log4j.logger.com.test=debug
log4j.appender.A.Threshold=INFO
log4j.appender.B.Threshold=WARN
那么就算设定com.test输出的级别是debug，但是A只接收INFO级别以上的log，而B只接收WARN级别以上的log
举例2：
log4j.appender.XX.Threshold门槛低于log4j.logger.XX时，
log4j.logger.com.test=info
log4j.appender.A.Threshold=DEBUG
log4j.appender.B.Threshold=WARN
那么com.test只输出info级别的，而A下设定的是debug，那么com.test输出的所有的日志在A下可以全部显示(当然是info以上的，因为com.test只给出了info以上的)。而在B下设定是warn，虽然com.test输出info级别以上的log给B，但是B只接收warn以上的log。
通过这样的方式配置，可以限定在不同的输出口过滤不同等级的日志。
应用举例需求：
我们不太想输出一些第三方jar的日志，但是error级别以上的我们需要，我们想输出我们自己的包下debug以上级别的日志，但是在控制台我想看到所有的，而在日志文件下，我却只想写入warn级别以上的日志。对于这样的情况如何配置呢？
这个时候我们就根据依据的来配置一下log4j
首先来点背景设定
我们自己的jar包都是com.test开头
A代表控制台输出
B代表文件输出
当然那些都只是一个命名
好，我们开始。首先所有的第三方jar都不太想看到输出日志，但是error的级别以上的需要
log4j.rootLogger=error, A,B
这句的意思就是设定所有log输出给A,B的等级为error
然后，我们想输出我们自己的包下debug以上级别的日志。
log4j.logger.com.test=debug
这句是设定com.test包下的所有log输出级别为debug,这里的设定直接覆盖了rootLogger对com.test包的作用。
最后，在控制台我想看到所有的，而在日志文件下，我却只想写入warn级别以上的日志。
log4j.appender.A=org.apache.log4j.ConsoleAppender
#config A position output contain DEBUG level and up
log4j.appender.A.Threshold=DEBUG
log4j.appender.A.layout=org.apache.log4j.PatternLayout 
log4j.appender.A.layout.ConversionPattern=%d [%-5p] %l:%t - %m %n
#in the daily files
log4j.appender.B=org.apache.log4j.DailyRollingFileAppender
log4j.appender.B.Threshold=WARN
log4j.appender.B.File=${root.path}/log/MyProject.log
log4j.appender.B.DatePattern='.'yyyy-MM-dd'.log'
log4j.appender.B.layout=org.apache.log4j.PatternLayout 
log4j.appender.B.layout.ConversionPattern=%d [%-5p] %l:%t - %m %n
和日志级别无关的代码就不解释了。直接重点，
log4j.appender.A.Threshold=DEBUG
log4j.appender.B.Threshold=WARN
表示A的接收级别为debug，而B的接收级别为warn。这样，A就可以看到debug以上的所有日志，而写入B的只有warn级别以上的日志。
