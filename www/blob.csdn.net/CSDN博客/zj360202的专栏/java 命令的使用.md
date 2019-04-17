# java 命令的使用 - zj360202的专栏 - CSDN博客





2012年12月12日 16:26:19[zj360202](https://me.csdn.net/zj360202)阅读数：414标签：[classpath																[javac](https://so.csdn.net/so/search/s.do?q=javac&t=blog)](https://so.csdn.net/so/search/s.do?q=classpath&t=blog)
个人分类：[java](https://blog.csdn.net/zj360202/article/category/1298882)








java

-classpath， 设定要搜索的类的路径，可以是目录，jar文件，zip文件（里面都是class文件），会覆盖掉所有的CLASSPATH的设定。

java -classpath classdir;jardir;zipdir;comdir com... propretiesdir

classdir是class目录，例如：c:/class

jardir是jar包的目录,例如：c:/aa.jar

zipdir是zip包目录

comdir是类路径，注意这个就到包路径前面，例如，A的包是com.aa.aa，文件路径是c:/BB/com/aa/aa/A,那么comdir就是c:/BB

com...是类路径  例如：com.taobao.timetunnel.broker.ReliableThriftBroker

前面的路径都是这个类执行中要引用的一下类和jar包

后面还有propretiesdir，就是配置文件路径，可能还有xml等等，是类执行过程中要引用的配置文件路径



假设abc.class在路径c:\src里面

可以在任何路径下执行以下命令

java -classpath c:\classes;c:\jar\abc.jar;c:\zip\abc.zip;c:\src abc






