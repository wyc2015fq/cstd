# hibernate中悲观锁和乐观锁和lazy加载 - z69183787的专栏 - CSDN博客
2014年04月02日 19:11:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：663
hibernate中经常用到当多个人对同一数据同时进行修改的时候，会发生脏数据，造成数据的不一致性，解决办法是可以通过悲观锁和乐观锁来实现。 
悲观锁：在数据有加载的时候就给其进行加锁，直到该锁被释放掉，其他用户才可以进行修改，优点：数据的一致性保持得很好，缺点：不适合多个用户并发访问。当一个锁住的资源不被释放掉的时候，这个资源永远不会被其他用户进行修改，容易造成无限期的等待。 
乐观锁：就是在对数据进行修改的时候，对数据才去版本或者时间戳等方式来比较，数据是否一致性来实现加锁。优点比较好。
Java代码  ![收藏代码](http://langhua9527.iteye.com/images/icon_star.png)
- 1、在悲观锁中，只要在加载的时候，才去session中的load方法，进行枷锁,session.load(****.class,1,LockMode.UPDATE);  
- 
-       Hibernate将事务管理委托给底层的JDBC或者JTA，默认是基于JDBC Transaction的。  
-       Hibernate支持“悲观锁（Pessimistic Locking）”和“乐观锁（Optimistic Locking）”。  
-       悲观锁对数据被外界修改持保守态度，因此，在整个数据处理过程中，将数据处于锁定状态。悲观锁的实现，往往依靠数据库提供的锁机制。Hibernate通过使用数据库的for update子句实现了悲观锁机制。Hibernate的加锁模式有：  
- 1. LockMode.NONE：无锁机制  
- 2. LockMode.WRITE：Hibernate在Insert和Update记录的时候会自动获取  
- 3. LockMode.READ：Hibernate在读取记录的时候会自动获取  
- 4. LockMode.UPGRADE：利用数据库的for update子句加锁  
- 5. LockMode.UPGRADE_NOWAIT：Oracle的特定实现，利用Oracle的for update nowait子句实现加锁  
- 2、乐观锁大多是基于数据版本（Version）记录机制实现。Hibernate在其数据访问引擎中内置了乐观锁实现，可以通过class描述符的optimistic-lock属性结合version描述符指定。optimistic-lock属性有如下可选取值：  
- 1. none：无乐观锁  
- 2. version：通过版本机制实现乐观锁  
- 3. dirty：通过检查发生变动过的属性实现乐观锁  
- 4. all：通过检查所有属性实现乐观锁  
2、乐观锁： 
1>在悲观锁的基础上加入private int version;和相关的setter、getter方法。 
2>映射文件配置在class标签里面加入optimistic-lock="version"，然后在的id标签后面加入<version name="version"/> 
Xml代码  ![收藏代码](http://langhua9527.iteye.com/images/icon_star.png)
- <hibernate-mapping>
- <classname="com.langhua.hibernate.pojo.CollectionMapping"table="t_lock"optimistic-lock="version">
- <idname="id">
- <generatorclass="native"/>
- </id>
- <versionname="version"/>
- <propertyname="name"/>
- <propertyname="totle"/>
- </class>
- </hibernate-mapping>
lazy策略可以用在： 
* <class>标签上：可以取值true/false 
* <property>标签上，可以取值true/false，这个特性需要类增强 
* <set>/<list>等集合上，可以取值为true/false/extra 
* <one-to-one>/<many-to-one>等标签上，可以取值false/proxy/no-proxy 
重要的概念： 
1、lazy的概念，指在需要的时候才发出sql 
2、lazy策略只是在session打开期间才是有效的 
注意： 
Hibernate类级别的lazy加载策略：可以控制什么时候加载这些普通对象属性 
Hibernate集合属性的懒加载策略： 
在集合属性上，可以配置懒加载策略，取值为：true/false/extra 
true:默认取值，它的意思是只有在调用这个集合获取里面的元素对象时，才发出查询语句，加载其集合元素的数据 
false:取消懒加载特性，即在加载对象的同时，就发出第二条查询语句加载其关联集合的数据 
extra:一种比较聪明的懒加载策略，即调用集合的size/contains等方法的时候，hibernate并不会去加载整个集合的数据，而是发出一条聪明的SQL语句，以便获得需要的值，只有在真正需要用到这些集合元素对象数据的时候，才去发出查询语句加载所有对象的数据 
Hibernate单端关联懒加载策略：即在<one-to-one>/<many-to-one>标签上可以配置 
懒加载策略。 
可以取值为：false/proxy/no-proxy 
false:取消懒加载策略，即在加载对象的同时，发出查询语句，加载其关联对象 
proxy:这是hibernate对单端关联的默认懒加载策略，即只有在调用到其关联对象的方法的时候才真正发出查询语句查询其对象数据，其关联对象是代理类no-proxy:这种懒加载特性需要对类进行增强，使用no-proxy，其关联对象不是代理类 
注意：在class标签上配置的lazy属性不会影响到关联对象!!!
