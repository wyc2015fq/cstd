# spring-IOC、springMVC、mybatis思路梳理 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月15日 20:03:16[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：88


springMVC

解决的问题：能够实现一个controllor 来完成，前端用户对数据库增删改查的操作。并且从前端获取数据的时候，在形参的位置放一个pojo实体，就能实现自动赋值。免除了以前 每从前端获取一个参数就需要调用request.getParameter("id")的繁琐步骤。

过程： MVC中的servlet的init方法会读取我们配置的xml文件，把指定包下的类全部扫描，同时放入IOC容器，然后是放入HandlerMapping中，HandlerMapping中有个map，key就是url，value就是 类名、方法名、参数列表。当用户在url输入地址的时候，servlet能够解析url地址，然后 通过映射，用key取value，然后再交给HandlerAdapter来执行方法，执行完以后返回一个参数，servlet拿着这个参数去找域名解析器，然后显示页面。

表单值自动赋给pojo原理：通过request.getParameterMapping 来获得一个map集合，然后map.keyset 获得一个 装着key的set集合，然后再拿着这个key去跟pojo的属性对应去找，找到了就赋值。

springIOC

解决的问题：service层中不需要new dao层对象，controllor中不需要new service 对象。

原理：监听器监听，服务器一启动，就读取配置文件，然后还是扫包，扫完了把带注解的类放入IOC容器中，这个容器能实现控制反转--依赖注入。说白了就是，service层需要dao层的实现类，就去IOC容器里面做登记了，IOC发现dao层的实现类也在这做登记了，就直接把值 给了service 了。IOC容器就相当于个中介。但前提得加注释。 


mybatis 

解决问题：不用写实现类，也不用写jdbc，写两个配置文件，一个是写要扫的包，还有维护sql语句的xml地址。另一个配置文件写的就是 需要的实现类对应的接口地址以及调用的方法名，还有sql的查询语句。 然后mybatis 就用自己的JDBC ，我们配置sql语句，给我们查询并返回的结果。放回的结果还可以给我们放到我们规定的接口对应的实现类的方法所规定的返回类型的对象中。  具体原理 目前还不清楚，懂了以后再写。

