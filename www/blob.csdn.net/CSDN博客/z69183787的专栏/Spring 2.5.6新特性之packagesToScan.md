# Spring 2.5.6新特性之packagesToScan - z69183787的专栏 - CSDN博客
2012年11月30日 16:44:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1173
如果你使用早前版本的Spring，又恰好采用了Annotation注解方式(而非传统XML方式)配置Hibernate对象关系映射，那么在通过org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean配置sessionFactory时，你一定对annotatedClasses、annotatedPackages有一种说不出的胸闷的感觉，如此以高配置性见长的Spring，怎么在这一个小小的环节上就不能做得再灵活些呢，一定要一个个手写Class路径么？
估计有不少人无奈选择了从AnnotationSessionFactoryBean继承一个自定义的子类，自己实现扫描逻辑，找出@Entity注解过的类清单配置进去。
Spring 2.5.6里有个不怎么起眼的改进，那就是在AnnotationSessionFactoryBean上增加了一个新的方法：
[setPackagesToScan(String[]
 packagesToScan)](http://static.springframework.org/spring/docs/2.5.x/api/org/springframework/orm/hibernate3/annotation/AnnotationSessionFactoryBean.html#setPackagesToScan%28java.lang.String%5B%5D%29)
有了这个方法，我们不再需要自己动手去实现实体类的扫描了，直接在Spring配置文件中AnnotationSessionFactoryBean这个section上增加类似如下的一个property即可(假定你需要加载的实体类所在的包名match这个字符串"com.**.bo")：
<property name="packagesToScan" value="com.**.bo"/>
你也可以以清单的方式指定多于1条的匹配字串，如：
<property name="packagesToScan">
<list>
<value>com.abc.core.bo</value>
<value>com.abc.auditing.bo</value>
</list>
</property>
