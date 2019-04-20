# 面试的角度诠释Java工程师（二） - Spark高级玩法 - CSDN博客
2018年04月18日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：57
上一篇：[面试的角度诠释Java工程师（一）](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484694&idx=1&sn=075b3e2ceafd31d27c4fc735011b03e8&chksm=9f38e63ea84f6f288407c64349f6c0b6a183c3b61fa50a27d0ad601734880b9a5f9901f29d47&scene=21#wechat_redirect)
# 三、框架篇
### **框架基础**
反射：反射是Java开发的一类动态相关机制。因为本身Java语言并不是一款动态语言，如果我们想要得到程序动态的效果，因此便引入了反射机制这一概念。
怎么表达反射？
![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGDosvtukicTy7ibqEyG430BV9EEp6gKHziaAJcAHS8bmFurZa6OMqYEd5bw/640?wxfrom=5&wx_lazy=1)
能用反射做什么？
PS：某面试题：Java中创建实例化对象有哪些方式？
①最常见的创建对象方法，使用new语句创建一个对象。
②通过工厂方法返回对象，例:String s =String.valueOf()。（工厂方法涉及到框架）
③动用反射机制创建实例化对象，Class类的三种方法或者通过类类型的newInstance()实例方法。
④调用对象的clone()方法。（俗称克隆方法）
⑤通过I/O留的反序列化手段，调用ObjectInputStream对象的readObject()方法。
注解：日常开发中，注解的使用简化了不少代码，同时也减少了程序员的日常工作量。
Java注解可以细分为JDK注解和第三方注解。其具体使用方法，可以点开链接看哦。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGDBibZpyxuVXf9ib6LjKqYic8cxLF9ozKYLrx2FDNiavQHlvv7UytqKxYgNw/640)
### **ORM**
认识Hibernate和Mybatis之前，必然要先了解一下什么是ORM？
ORM是三个单词的缩写：Object/Relationship Mapping。翻译过来就是“对象/关系映射”。
为什么要有ORM？
在用面向对象思想编写应用程序的时候，最终都是把对象的信息保存在关系型数据库中，这样我们就需要编写很多与底层数据库相关的SQL语句。显然这样是很不便捷的，ORM框架技术就可以解决这些繁琐的问题。
ORM存在的意义？
彻底抛弃书写SQL语句的思想，完全使用面向对象的思想开发。
为什么要抛弃程序中书写SQL语句的思想？
①不同的数据库使用的SQL语法不同，例：同样一段SQL脚本，能在T-SQL中运行，但不一定能保证可以在PL-SQL中运行。
②同样的功能在不同的数据库有不同的实现方式，例：分页SQL。
③程序过分依赖SQL，对程序的移植、扩展和维护带来很大的麻烦。
### **Hibernate框架**
Hibernate是Java领域内的一款开源的ORM框架技术。
Hibernate的机制原理：
①Hibernate对JDBC进行非常轻量级的封装。
（从本质上而言，最终Hibernate还是通过JDBC来进行对数据库的操作，只不过在JDBC的基础上进行了一定的封装）
②与EBJ（本身是JavaEE的规范，重量级的ORM框架技术）不同，Hibernate属于轻量级的ORM框架技术。
③Hibernate介于数据库层和业务逻辑层之间，起到持久化层的作用。
HQL数据查询基础
检索对象——from子句
选择——select子句
限制——where子句
排序——order by子句
HQL和SQL语句不同，HQL中from子句是必须的，可以没有select子句。
Hibernate的常用属性：
cascade：设置级联关系。
inverse：设置由哪一方维护关联关系。
例：下面的这段代码集合了常用属性以及表与表之间的多对一和一对多关系。
```
<hibernate-mapping>
    <class name="" table="">
        <id name="id" type="java.lang.Long">
            <column name="ID" />
            <generator class="increment" />
        </id>
        <many-to-one name="" class=""
            fetch="">
            <column name="" />
        </many-to-one>
        ......
        <set name="" inverse="true"  cascade="all" lazy="false">
            <key>
                <column name="" />
            </key>
            <one-to-many class="" />
        </set>
    </class>
</hibernate-mapping>
```
### **Struts2框架**
Struts是流行和成熟的基于MVC设计模式的Web应用程序框架。
Struts2动态方法的调用：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGDF3IEA40T3pXTnCmTIphQ5Uk4L1DjXnnWTq2z9poY3LdvuuL6rHQJMQ/640)
**例:**
①指定method属性的方法。
```
<action name="" method="" class="">
<result>/index.jsp</result>
</action>
```
②感叹号的方式。
```
<action name="" class="">
<result>/index.jsp</result>
</action>
<constant name="structs.enable.DynamicMethodInvocation" value="true"></constant>
```
③通配符的方式。
```
<action name="_*" method="{1}" class="">
<result>/index.jsp</result>
<result>/{1}.jsp</result>
</action>
```
在使用通配符方式的时候，如果要使用多个method的话，可以再在`method="{1}{2}..."`里面这样添加，同时需要修改`name=""`里面的名字+后缀`_*_*...`,还要配置所需要的`<result>/{}.jsp</result>`标签
**Struts2访问servlet api**：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGDwAsO0SRZwjGvVmUTzEW6GMdcqljlaGsfsw90T2rrAh1EovQEyFk9Kg/640)
Struts2拦截器工作原理：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGDkbd3iakN19obho6x7bicdCzZtib5ibjSwedALHAbJ088RdicnOPpBFXRibSg/640)
思路：
当Struts在执行一个action的时候，会在action执行之前去执行一些拦截器，如果有多个拦截器会依次进行执行。
①首先执行拦截器一，执行拦截器一的处理方法并做一些处理操作。
②如果还有其它的拦截器，将执行拦截器二、拦截器三...进行拦截。
③当所有的拦截器都执行之后，将执行action操作，并调用action中的业务处理方法进行业务的处理。
④调用完之后，会返回一个字符串结果，也就是Result结果集。然后进行视图的匹配。
⑤再将前面执行的拦截器进行反向的再执行一次，最终响应到客户端。
### **Spring框架**
Spring是一个轻量级的控制反转（IOC）和面向切面（AOP）的容器框架和开源框架。
Spring框架的好处：
①从大小和开销两个方面而言Spring都是轻量级的。
②通过控制反转的技术达到松耦合的目的。
③提供了面向切面编程的丰富支持，允许通过分离应用的业务逻辑和系统级服务进行内聚性的开发。
④包含并管理应用对象的配置和生命周期，从这个意义上讲它是一种容器。
⑤将简单的组件配置、组合成为复杂的应用，从这个意义上讲它是一种框架。
什么是Spring的IOC？
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGDYXiaNicvQqFcKRLJI7dkpMiasWeQjtY9H1A69EhEZUJej6icgc8xmMTcibQ/640)
思路：
①当业务的对象进入了Spring的容器。
（Your Business Objects）→（The Spring Container）
②然后通过配置的原数据来生产出符合系统需要的对象。
（Configuration Metadata）→（The Spring Container）→（Fully Configured system）
③当要使用这些对象的时候去Spring容器里面直接拿出来用就可以了。
**什么是Spring的AOP？**
AOP：通过预编译方式和运行期动态代理实现程序功能的统一维护的一种技术。
主要功能有：日志记录、性能统计、安全控制、事务处理、异常处理等。
注：
预编译方式：AspectJ。
动态代理方式：JDK动态代理和CGLib动态代理。（也就是SpringAOP和JbossAOP）
Spring的AOP用途：
①提供了声明式的的企业服务，特别是EJB的替代服务的声明。
②允许用户定义自己的方法，以完成OOP和AOP的互补使用。
Spring的事务管理
Spring的事务管理高层抽象主要包括3个接口：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGDGg1twmqZzMakaTu8UB1qLHWXrbNeAialHFa6c27ko7XVBiaFcic2YAO5g/640)
TransactionDefinition定义事务传播行为（7种）:
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGDicqTl2sjghsHL9yxREibT5Y4icQVNJFYR7ia3YzTibGtdUscwdrG07JibhQg/640)
### **Spring MVC框架**
**Spring MVC是一种前端控制器的实现形式，它的基本概念分为静态概念 和 动态概念。**
静态概念：如图下示。DispatcherServlet就是Spring MVC的前端控制器。
思路：
①当浏览器端用户的请求通过DispatcherServlet进行了分发，到达Cotroller层。
②到达Cotroller层之后，便生产出我们所需要的业务数据Model。
③然后Model层再通过DispatcherServlet进行传递给我们的View层。
④最后完成了最终的页面呈现。
总结：MVC将业务逻辑和页面实现了分离，其核心就是通过DispatcherServlet实现的。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGDe3Et4EqXvlWTRfiaq7EMG6hRcGukyQbZPZSp7FDVq0xHp5icnicNvObqw/640)
动态概念：如图下示。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGDMoL6H8SvcJ9enPaRtpibA4Cicg0cibucLmqBNeVaSBjI59e76LAFaTj8g/640)
思路：
①当浏览器的请求Request到达DispatcherServlet。（因为DispatcherServlet也是一个Servlet，所有的Request能够被它拦截到）
②然后DispatcherServlet会搜索寻找到一个Mapping，也就是HandlerMaping，并将其功能代理给了HandlerMaping。
③然后HandlerMaping根据本身的配置，找到需要用到的Controller和HandlerInterceptor。
④然后把Controller和HandlerInterceptor制成一个可执行的链条，也就是Handler/HandlerAdapter的适配器。
⑤Handler/HandlerAdapter的适配器将信息返回给了DispatcherServlet，DispatcherServlet便开始调用这个一般化的处理器Handler/HandlerAdapter。
⑥Controller的目的就是生成ModelAndView模型，并且返还给DispatcherServlet。
⑦DispatcherServlet是不会管理视图显示的，所以它就调用ViewResolver视图解析器并通过该方法返回到View对象。（ViewResolver的作用是告诉DispatcherServlet哪个视图是用来解析当前这种场景的）
⑧然后ModelAndView将模型数据传递到View，完成了页面呈现。
Spring MVC拦截器工作流程：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGD5ibKJlpcGctSDqwZlbc6Ol8HLauK6GqVWkEoylicYOWSLG6Ny1cTJw1Q/640)
### **Mybatis框架**
Mybatis和Hibernate一样，是一款开源的ORM框架的技术。
Mybatis的机制原理：
①Mybatis支持普通的SQL查询、存储过程和高级映射的持久层框架。
②Mybatis将大量的SQL语句从程序里面剖离出来，配置在配置文件中，实现SQL的灵活配置。
**OGNL表达式**：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIOOELYiaAXV3lnXgTiaY4bBGD5TPpMIQaNtqgPVgtoUkp6dwsBqfhrJgNJAwe5bRWJWicuLXb5pVVUPA/640)
Mybatis框架和Hibernate框架比较：
①“全自动化”的Hibernate对数据库结构提供了完整的封装和全套映射机制。
②“半自动化”的Mybatis提供的ORM机制，对业务逻辑实现人员而言，面对的是纯粹的Java对象。
③Hibernate会自动生成SQL语句，而MyBatis不会在运行期间自动生成SQL语句执行。
# **四、工具篇**
### **JUnit单元测试**
JUnit是一套基于测试驱动开发的测试框架。
注：
JUnit4不需要继承junit.framework.TestCase类。
JUnit3中所有的方法都必须使用@Test注解，并且需要在方法名前加test作为前缀，需要继承junit.framework.TestCase类。
### **版本管理工具SVN**
SVN是一个开放源代码的版本控制系统。
举个例子：
①个人的代码→口袋里的钱
②版本控制工具中的代码→卡里的钱
③版本控制工具中的客户端→卡
④版本控制工具中的服务端→刷卡机
注：在实际版本管理当中，你所写的代码，需要提交给服务端，然后别人要修改你的代码，就会从服务端更新当前目录下的代码，再进行修改和提交。所以，“卡里的钱”可以理解为团队公有的代码体系。
### **项目管理工具maven**
maven是基于项目对象模型（POM），可以通过一小段描述信息来管理项目的构建、报告和文档的软件项目管理工具。
注：
maven也是一套强大的自动化的管理工具，覆盖了编译、测试、运行、清理、打包和部署，整个项目构建的周期。
它还提供了一个仓库的概念，统一的帮助开发人员管理项目所依赖的第三方的架包，最大可能的避免环境配置不同所产生的问题（在你的电脑上能运行，在我的电脑上就不能运行）。
如果大家觉得我写的对你有帮助，请顺手点个赞支持一下呗；如果大家觉得我有写的不对的地方，欢迎大家多多发言。谢谢！
- 
**原文：www.jianshu.com/p/a0b8b6f17888**
**推荐阅读**
**1，[几种简单的负载均衡算法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484672&idx=1&sn=665bf603aabaf1c780a23ffcece428c8&chksm=9f38e628a84f6f3e46c20d7dc7c6a2077be9b6c715c527263982cc1fcf80883242ce8ea8b8ee&scene=21#wechat_redirect)**
**2，[Java多线程的应用场景和应用目的举例](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484671&idx=1&sn=65dd4638f961aa172952f86f41fa3c63&chksm=9f38e7d7a84f6ec1b52bbcc13cf036811663f7aff8041c502ccb39912c3d4e1969bbf155cf62&scene=21#wechat_redirect)**
**3，[ConcurrentLinkedQueue和LinkedBlockingQueue用法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484673&idx=1&sn=6ac659bf246d7f5ea801f7998e80380b&chksm=9f38e629a84f6f3fd382d4d45955f51114e94d29b51b6ee3611bb5beb2564d55230f1d9bbd2e&scene=21#wechat_redirect)**
**4，[从零开始 Spark 性能调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484670&idx=1&sn=f1401758c5d177da61bfcddb5fb0e67b&chksm=9f38e7d6a84f6ec0d6cdafe0bcba3353436ac8faef8a397c6cd1cdda2d1bd5e8ce8ec181f12c&scene=21#wechat_redirect)**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
