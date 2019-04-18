# jps 命令使用 - z69183787的专栏 - CSDN博客
2018年06月26日 10:30:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：395
[https://www.cnblogs.com/vinozly/p/5068077.html](https://www.cnblogs.com/vinozly/p/5068077.html)
jps(Java Virtual Machine Process Status Tool)是JDK1.5提供的一个显示当前所有java进程pid的命令，简单实用，非常适合在linux/unix平台上简单察看当前java进程的一些简单情况。很多人都是用过unix系统里的ps命令，这个命令主要是用来显示当前系统的进程情况，有哪些进程以及进程id。 jps 也是一样，它的作用是显示当前系统的java进程情况及进程id。我们可以通过它来查看我们到底启动了几个java进程（因为每一个java程序都会独占一个java虚拟机实例），并可通过opt来查看这些进程的详细启动参数。
使用方法：在当前命令行下打jps(jps存放在JAVA_HOME/bin/jps，使用时为了方便请将JAVA_HOME/bin/加入到Path) 。
$> jps
23991 Jps
23651 Resin
比较常用的参数：
-q 只显示pid，不显示class名称,jar文件名和传递给main方法的参数
$>  jps -q
28680
23789
23651
-m 输出传递给main方法的参数，在嵌入式jvm上可能是null
$> jps -m
28715 Jps -m
23789 BossMain
23651 Resin -socketwait 32768 -stdout /resin/log/stdout.log -stderr /resin/log/stderr.log
-l 输出应用程序main class的完整package名或者应用程序的jar文件完整路径名
$> jps -l
28729 sun.tools.jps.Jps
23789 com.asiainfo.aimc.bossbi.BossMain
23651 com.caucho.server.resin.Resin
-v 输出传递给JVM的参数
$> jps -v
23789 BossMain
28802 Jps -Denv.class.path=/data/aoxj/bossbi/twsecurity/java/trustwork140.jar:/data/aoxj/bossbi/twsecurity/java/:/data/aoxj/bossbi/twsecurity/java/twcmcc.jar:/data/aoxj/jdk15/lib/rt.jar:/data/aoxj/jdk15/lib/tools.jar -Dapplication.home=/data/aoxj/jdk15 -Xms8m
23651 Resin -Xss1m -Dresin.home=/data/aoxj/resin -Dserver.root=/data/aoxj/resin -Djava.util.logging.manager=com.caucho.log.LogManagerImpl -Djavax.management.builder.initial=com.caucho.jmx.MBeanServerBuilderImpl
