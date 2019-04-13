
# tomcat OutOfMemoryError - 阳光岛主 - CSDN博客

2013年10月02日 19:54:44[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5520


部署应用服务到tomcat下，可能会抛出内存溢出异常，如下：
Exception in thread "Timer-1" java.lang.OutOfMemoryError: PermGen space


为了解决tomcat在大进行大并发请求时，出现内存溢出的问题，请修改tomcat的内存大小，其中分为以下两种方式：
**一、使用 catalina.bat 等命令行方式运行的 tomcat**
1、windows环境下，修改tomcat\bin\Catalina.bat文件
在166行左右
rem Execute Java with the applicable properties ”以下每行
%_EXECJAVA% %JAVA_OPTS% %CATALINA_OPTS% %DEBUG_OPTS% -Djava.endorsed.dirs="%JAVA_ENDORSED_DIRS%" -classpath "%CLASSPATH%" -Dcatalina.base="%CATALINA_BASE%" -Dcatalina.home="%CATALINA_HOME%" -Djava.io.tmpdir="%CATALINA_TMPDIR%" %MAINCLASS% %CMD_LINE_ARGS% %ACTION%
在 %DEBUG_OPTS% 后面添加-Xms256m -Xmx512m
2、linux环境下，打开在Tomcat的安装目录的bin文件的 ./bin/catalina.sh文件,进入编辑状态.
在注释后面加上如下脚本:
JAVA_OPTS='-Xms512m -Xmx1024m'
JAVA_OPTS="$JAVA_OPTS -server -XX:PermSize=64M -XX:MaxPermSize=256m"
或者，在echo "Using CATALINA_BASE:   $CATALINA_BASE" 下添加一行
echo "Using CATALINA_BASE:   $CATALINA_BASE"
JAVA_OPTS="-server -XX:PermSize=64M -XX:MaxPermSize=128m"
echo "Using CATALINA_HOME:   $CATALINA_HOME"
echo "Using CATALINA_TMPDIR: $CATALINA_TMPDIR"
说明： JAVA_OPTS='-Xms512m -Xmx1024m' 是设置Tomcat使用的内存的大小； -XX:PermSize=64M -XX:MaxPermSize=256m 指定类空间(用于加载类)的内存大小
保存后，重新以命令行的方式运行 tomcat ，即可，然后通过最后面介绍的如何观察tomcat现有内存情况的方法进行查看是否已经变更成功。

二、使用 系统中的 “服务”，或者开始菜单的可执行程序运行的tomcat
1、关闭 现在正在运行的tomcat
如果是tomcat 5.5 或者是 5.0.28 之后的版本，在 Java 选项卡中，下方，会有
Inital memory Pool:
Maximum memory Pool:
Thread stack size:
三个输入框，在这里即可不用做上面的操作，直接配置内存大小，只需要设置
Inital memory Pool 为 128
Maximum memory Pool 为 256
![](https://img-blog.csdn.net/20130929112122890)
点击确定后，重启tomcat 生效
如果您是低版本的tomcat，发现没有上面那几个录入框，请看下面的操作步骤
在开始菜单中，找到“Apache Tomcat 5.0”,并选择“Configure Tomcat”，在弹出的对话框窗口中，切换到 Java VM 选项卡，并在 Java Options 输入框的最前面输入
-Xms256m -Xmx512m
即输入框中的内容会像下面的代码（与自己的环境有所区别）
-Xms256m -Xmx512m
-Dcatalina.home="C:\tomcat5"
-Djava.endorsed.dirs="C:\tomcat5\common\endorsed"
-Xrs
设置完后，点击“确定”，并重启tomcat即可。

三、查看现有tomcat的内存大小情况
1、linux下，直接查看 ps aux | grep tomcat
![](https://img-blog.csdn.net/20130929112502562)

2、添加用户，修改conf/tomcat-users.xml
<?xml version='1.0' encoding='utf-8'?>
<tomcat-users>
<role rolename="tomcat"/>
<role rolename="role1"/>
<role rolename="manager"/>
<role rolename="admin"/>
<user username="tomcat" password="tomcat" roles="tomcat"/>
<user username="both" password="tomcat" roles="tomcat,role1"/>
<user username="role1" password="tomcat" roles="role1"/>
<user username="admin" password="admin" roles="admin,manager"/>
<role rolename="manager-gui"/>
<user username="tomcat" password="s3cret" roles="manager-gui"/>
</tomcat-users>
3、重启tomcat，使新增用户生效，再登录进入了Server Status页面，可以在JVM表格中看到，如下图
Free memory: 333.96 MB Total memory: 498.00 MB Max memory: 1326.25 MB
![](https://img-blog.csdn.net/20130929111156781)
上面的文字即代表了，当前空闲内存（Free memory）、当前总内存（Total memory）、最大可使用内存（Max memory）三个数据。
确定了最大内存足够大时，tomcat即可正常运转


公司的一个服务器使用Tomcat6默认配置，在后台一阵全点击服务器就报废了，查了一下就要是PERMSIZE默认值过小造成（16-64）
TOMCAT_HOME/bin/catalina.sh
添加一行：JAVA_OPTS=" -server -XX:PermSize=64M -XX:MaxPermSize=128m"
问题解决（可能为调用JAR包过多原因）下面是网上看到一些设置
JAVA_OPTS="-server -Xms800m -Xmx800m -XX:PermSize=64M -XX:MaxNewSize=256m -XX:MaxPermSize=128m -Djava.awt.headless=true "
当在对其进行并发测试时，基本上30个USER上去就当机了，还要修改默认连接数设置：以下红色四行TOMCAT6中好相没有，手工加上就可以了，基本上可以解决连接数过大引起的死机。具体数值可跟据实际情况设置
<Connector port="80" protocol="HTTP/1.1"
maxThreads="600"
minSpareThreads="100"
maxSpareThreads="500"
acceptCount="700"
connectionTimeout="20000"
redirectPort="8443" />
这样设置以后，基本上没有再当机过。。。。。
maxThreads="600"       ///最大线程数
minSpareThreads="100"///初始化时创建的线程数
maxSpareThreads="500"///一旦创建的线程超过这个值，Tomcat就会关闭不再需要
的socket线程。
acceptCount="700"//指定当所有可以使用的处理请求的线程数都被使用时，可以放到处理队列中的请求数，超过这个数的请求将不予处理

**参考推荐：**
[修改和查看tomcat内存大小](http://blog.163.com/wex660@126/blog/static/241530652012621113321255/)
[PermGen space错误解决方法](http://www.cnblogs.com/xwdreamer/archive/2011/11/21/2296930.html)
[PermSize内存设置方法（tomcat）](http://www.doc88.com/p-198264252753.html)
[tomcat的OutOfMemoryError(PermGen space)解决方法](http://hi.baidu.com/gonervergiveup/item/3c7a9b374f29749db80c03a9)
[Tomcat　java.lang.OutOfMemoryError: PermGen space及其解决方法](http://blog.csdn.net/gloomyboyo/article/details/4925505)


