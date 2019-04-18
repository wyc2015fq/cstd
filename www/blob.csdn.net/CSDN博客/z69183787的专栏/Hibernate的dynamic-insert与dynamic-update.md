# Hibernate的dynamic-insert与dynamic-update - z69183787的专栏 - CSDN博客
2014年11月04日 13:36:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：702
个人分类：[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)

Hibernate允许我们在映射文件里控制insert和update语句的内容.比如在映射文件中<property 元素中的update属性设置成为false,那么这个字段,将不被包括在基本的update语句中,修改的时候,将不包括这个字段了.insert同 理.dynamic动态SQL语句的配置也是很常用的.下面介绍配置SQL语句的具体属性:
      1)<property>元素 insert属性:设置为false,在insert语句中不包含这个字段,表示永远不会被插入,默认true
      2)<property>元素 update属性:设置为false,在update语句中不包含这个字段,表示永远不会被修改,默认true
      3)<class>元素 mutable属性:设置为false就是把所有的<property>元素的update属性设置为了false,说明这个对象不会被更新,默认true
      4)<property>元素 dynamic-insert属性:设置为true,表示insert对象的时候,生成动态的insert语句,如果这个字段的值是null就不会加入到insert语句当中.默认false
      5)<property>元素 dynamic-update属性,设置为true,表示update对象的时候,生成动态的update语句,如果这个字段的值是null就不会被加入到update语句中,默认false
      6)<class>元素 dynamic-insert属性:设置为true,表示把所有的<property>元素的dynamic-insert属性设置为true,默认false
      7)<class>元素 dynamic-update属性:设置为true,表示把所有的<property>元素的dynamic-update属性设置为true,默认false
      Hibernate生成动态SQL语句的消耗的系统资源(比如CPU,内存等)是很小的,所以不会影响到系统的性能,如果表中包含N多字段,建议把 dynamic-update属性和insert属性设置为true,这样在插入和修改数据的时候,语句中只包括要插入或者修改的字段.可以节省SQL语 句的执行时间,提高程序的运行效率.

比较郁闷的时Hibernate的annotation模式一直没有试验通过，十分郁闷啊。我看网络上别人这样用过，但是不行，谁有能通过的教程了，给我留言啊；）
在hbm.xml 属性里面配置 
<property name="userName" **dynamic-update="false"**dynamic-insert="false"/> 
dynamic-update为true，表示更新一个对象时,会生成动态SQL,当属性值发生变化时，才会包含到UPDATE语句中。 
在hibernate annotation中如何配置呢?
追加
@org.hibernate.annotations.Entity(dynamicUpdate=true,dynamicInsert=true)
