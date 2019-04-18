# p6spy介绍 - z69183787的专栏 - CSDN博客
2015年02月03日 19:14:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1828
## P6spy是什么？
P6Spy 是针对数据库访问操作的动态监测框架（为开源项目，项目首页：www.p6spy.com）它使得数据库数据可无缝截取和操纵，而不必对现有应用程序的代码作任何修改。P6Spy 分发包包括P6Log，它是一个可记录任何 Java 应用程序的所有JDBC事务的应用程序。其配置完成使用时，可以进行数据访问性能的监测。
我们最需要的功能，查看sql语句，不是预编译的带问号的哦，而是真正的数据库执行的sql，更直观，更简单。
下载地址：[http://www.p6spy.com/download.html](http://www.p6spy.com/download.html)
目前最新的版本为1.3
## 需要准备什么？
我们最需要的是这些：
Spy.properties是p6spy的配置文件，里面定义了类似log4j的日志格式，文件路径等。
P6spy.jar则是主要的jar文件。
![](http://pic002.cnblogs.com/img/czy88840616/200912/2009122115404655.jpg)
使用maven的可以直接加入p6spy依赖，但是spy.properties依旧得copy啦~
![](http://pic002.cnblogs.com/img/czy88840616/200912/2009122115411734.jpg)
## 开始一个demo
1、  copy压缩包里的spy.properties到classpath下，你想敲也可以…………
2、  打开spy.properties，里面比较重要的有
realdriver=org.gjt.mm.mysql.Driver
代表真正的数据库驱动
上面有一些注释的驱动，直接把#号去掉就可以，保留一个生效即可
![](http://pic002.cnblogs.com/img/czy88840616/200912/2009122115420250.jpg)
我本地因为有一个mysql数据库来测试，所以我使用mysql驱动
![](http://pic002.cnblogs.com/img/czy88840616/200912/2009122115421777.jpg)
3、  简单地使用spring的jdbc模板来查询，测试输出的sql
![](http://pic002.cnblogs.com/img/czy88840616/200912/2009122115423973.jpg)
```
<bean id="dataSource" class="com.p6spy.engine.spy.P6DataSource">
<constructor-arg ref="dataSourceTarget"></constructor-arg>
</bean>
```
这段表明了使用p6spy来监控sql，相当于一个代理的数据源，但是实际上引用的还是我们自己的真正的数据源，但是这里使用构造注入不太友好，呵呵。
4、  简单执行demo
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)代码
package p6spy;
import java.util.List;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.jdbc.core.JdbcTemplate;
public class SqlDemo {
public static void main(String[] args) {
      ApplicationContext context = new ClassPathXmlApplicationContext("p6spy/spring.xml");
      JdbcTemplate jdbcTemplate = (JdbcTemplate) context.getBean("jdbcTemplate");
      List list = jdbcTemplate.queryForList("select * from user where id=?", new Object[]{1});
      System.out.println(list.size());
   }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
其中包含一个带参数的sql    select * from user where id=?
成功运行后查看日志，默认的文件名为spy.log,会生成在项目根目录下。
![](http://pic002.cnblogs.com/img/czy88840616/200912/2009122115431192.jpg)
看看，是不是预编译的sql和实际的sql都打出来了
如果出现
Warning: driver com.mysql.jdbc.Driver is a real driver in spy.properties, but it has been loaded before p6spy.  p6spy will not wrap these connections.  Either prevent the driver from
 loading, or try setting 'deregisterdrivers' to true in spy.properties
出现这句话的时候把deregisterdrivers=true
对于一个已经存在的工程，我们只要做3步就可以了
1、  导入p6spy.jar包
2、  将工程的数据库驱动换成相对应的p6spy驱动
3、  将spy.properties的真实驱动项指向你真正的驱动
Enjoy it……
这个包在测试的时候相当的方便，特别是调试sql的时候。。。
