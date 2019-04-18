# JPA使用动态的SCHEMA配置方法 - z69183787的专栏 - CSDN博客
2014年04月12日 11:57:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3928
最近在做一个项目，使用Spring+Struts2+JPA(Hibernate)的框架，项目接近尾声，现在要为测试和以后的正式运营开始做准备了。但是突然发现了一个小问题，发现使用MyEclipse自动生成的持久化对象所对应的Schema信息都是直接在类内部使用注解声明的方式来设置的，类似如下的格式@Table(name = “xxx”, schema=”xxx”)。
这样子就带来一个坏处，就是数据库的schema肯定是会有变更需求的，而把schema直接写死在持久化类中，等于每次变化schema的名字都要修改一遍所有的PO对象的注解声明，还要对类进行重新编译，非常麻烦的事情。
想到以前的hibernate也是有分表配置方式和系统统一配置方式的，当然hibernate的配置都是在xml配置文件中，修改都不需要重新编译类文件，相对来说问题不大。但是对于JPA的注解方式就影响太大了。而这次开发也正好使用的Hibernate作为JPA的实现，所以，考虑可以使用Hibernate统一配置方式，给JPA的配置加一个hibernate.default_schema的property属性。
<property name=”hibernate.default_schema” value=”myschema”/>
然后去掉了PO类中的注解，测试一下，一切OK。
当然，在使用oracle数据库的时候，用户名和schema如果是一样的，也可以直接删除PO类中的schema声明，不必专门配置对应的schema信息，JPA会默认使用该用户名对应的schema。
使用JPA还是非常方便的，很多时候就是把作为Provider的原框架包的元素变为property的属性就可以了。
要把Provider变为其他的比如Toplink、OpenJPA也同样处理即可。
而使用接口编程的优势，在这里又一次被印证了。
当然，JPA还提供了另外一种方式来定义，就是使用可选的orm.xml文件，可以在该文件中指定默认的schema。
