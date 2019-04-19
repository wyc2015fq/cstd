# mybatis的嵌套查询和延迟加载分析 - 零度的博客专栏 - CSDN博客
2017年08月03日 10:41:17[零度anngle](https://me.csdn.net/zmx729618)阅读数：291标签：[mybatis](https://so.csdn.net/so/search/s.do?q=mybatis&t=blog)
个人分类：[Mybatis](https://blog.csdn.net/zmx729618/article/category/6309398)
本文我们研究mybatis的嵌套查询和延迟加载。 
1.预备知识 
resultMap是mybatis里的一个高级功能。通过利用association和collection，可以做到将多个表关联到到一起，但又不用写JOIN这种复杂SQL，有点类似于hibernate、JPA。 
如果不熟悉resultMap的话，可以读一下[官方的文档](http://mybatis.github.io/mybatis-3/zh/sqlmap-xml.html#Result_Maps)。 
2.官方例子 
学习最好的方法就是看例子 
我这里下载了官方的mybatis3.3.0-SNAPSHOT[源码](https://github.com/mybatis/)，借用里面一个测试程序来跟踪一下嵌套查询和延迟加载这两个特性。 
找到org.apache.ibatis.submitted.cglib_lazy_error包，里面有两个测试程序， 
CglibNPETest是测试嵌套查询的，没有用延迟加载。 
CglibNPELazyTest则用了延迟加载。 
2.1 表结构和测试数据 
CreateDB.sql 
Sql代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- createtable person (  
-   id int,  
-   firstName varchar(100),  
-   lastName varchar(100),  
-   parent intDEFAULTNULL
- );  
- 
- INSERTINTO person (id, firstName, lastName, parent) VALUES (1, 'John sr.', 'Smith', null);  
- INSERTINTO person (id, firstName, lastName, parent) VALUES (2, 'John', 'Smith', 1);  
- INSERTINTO person (id, firstName, lastName, parent) VALUES (3, 'John jr.', 'Smith', 2);  
表结构我们只要关心parent字段就可以了，是说这个人的父亲是谁。然后插入3条记录，3的父亲是2，2的父亲是1
2.2 Bean定义 
Person.java 
Java代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- publicclass Person {  
- 
- private Long id;  
- private String firstName;  
- private String lastName;  
- private Person parent;  
- }  
2.3 Mapper定义 
Person.xml 
Xml代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- <resultMapid="personMap"type="Person">
- <idproperty="id"column="Person_id"/>
- <resultproperty="firstName"column="Person_firstName"/>
- <resultproperty="lastName"column="Person_lastName"/>
- <associationproperty="parent"column="Person_parent"select="selectById"/>
- </resultMap>
- 
- <selectid="selectById"resultMap="personMap"parameterType="int">
-     SELECT  
- <includerefid="columns"/>
-     FROM Person  
-     WHERE id = #{id,jdbcType=INTEGER}  
- </select>
可以看到要关联父子，没有采用写JOIN语句的方法，而是在resultMap里定义了一个association，然后最后的select="selectById"表明要用一个嵌套查询来查得父亲记录。 
3.测试准备 
为了看的清楚一点，我们打开DEBUG的log，最简单的可以采用STDOUT_LOGGING，将日志输出到控制台。 
两个文件，ibatisConfig.xml是CglibNPETest用的，ibatisConfigLazy.xml是CglibNPELazyTest用的。 
ibatisConfig.xml 
Xml代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- <settings>
- <settingname="logImpl"value="STDOUT_LOGGING"/>
- </settings>
ibatisConfigLazy.xml 
Xml代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- <settings>
- <settingname="proxyFactory"value="CGLIB"/>
- <settingname="lazyLoadingEnabled"value="true"/>
- <settingname="logImpl"value="STDOUT_LOGGING"/>
- </settings>
![](http://static.blog.csdn.net/images/save_snippets.png)
4.嵌套查询测试 
CglibNPETest.testAncestorAfterQueryingParents方法 
断点分别设在这2句话上 
Java代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- Person expectedAncestor = personMapper.selectById(1);  
- Person person = personMapper.selectById(3);  
先运行selectById(1)，观察日志 
Txt代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- ==>  Preparing: SELECT Person.id AS Person_id, Person.firstName AS Person_firstName, Person.lastName AS Person_lastName, Person.parent AS Person_parent FROM Person WHERE id = ?   
- ==> Parameters: 1(Integer)  
- <==    Columns: PERSON_ID, PERSON_FIRSTNAME, PERSON_LASTNAME, PERSON_PARENT  
- <==        Row: 1, John sr., Smith, null  
- <==      Total: 1
![](http://static.blog.csdn.net/images/save_snippets.png)
mybatis发了1条SQL取得id为1的记录。 
然后运行selectById(3)，观察日志 
Txt代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- ==>  Preparing: SELECT Person.id AS Person_id, Person.firstName AS Person_firstName, Person.lastName AS Person_lastName, Person.parent AS Person_parent FROM Person WHERE id = ?   
- ==> Parameters: 3(Integer)  
- <==    Columns: PERSON_ID, PERSON_FIRSTNAME, PERSON_LASTNAME, PERSON_PARENT  
- <==        Row: 3, John jr., Smith, 2
- ====>  Preparing: SELECT Person.id AS Person_id, Person.firstName AS Person_firstName, Person.lastName AS Person_lastName, Person.parent AS Person_parent FROM Person WHERE id = ?   
- ====> Parameters: 2(Integer)  
- <====    Columns: PERSON_ID, PERSON_FIRSTNAME, PERSON_LASTNAME, PERSON_PARENT  
- <====        Row: 2, John, Smith, 1
- <====      Total: 1
- <==      Total: 1
![](http://static.blog.csdn.net/images/save_snippets.png)
可以看到mybatis采用了发2条SQL的方法来实现这个嵌套查询的功能。先 select 3, 再 select 2，同时注意下图右上角person的类型的确是如假包换的Person型。 
![](http://dl2.iteye.com/upload/attachment/0101/6539/aeb7dc73-4ce3-336b-a622-c8e018cde56c.png)
进一步深入，一步步跟踪进去，调用堆栈如图所示，这张图大家不要看错，调用顺序是从下往上的，所以请从下往上看。 
![](http://dl2.iteye.com/upload/attachment/0101/6537/85e6a743-4e27-3a3e-a3f5-76a0e535bcee.png)
最下面的$Proxy5.selectById想必大家一定都知道了，表明了personMapper是一个代理，这就是为什么我们只需要定义mapper的接口，而不需要实现的原因了，mybatis用JDK的动态代理帮我们实现了。 
接下来这段调用流程的入口点我们可以看到是CachingExecutor.query，目的是为了取得id=3的记录 
CachingExecutor.query 
-->SimpleExecutor.query 
-->SimpleExecutor.prepareStatement 
-->RoutingStatementHandler.query 
-->PreparedStatementHandler.query 
取得记录后，交给DefaultResultSetHandler处理，要做的事情是将Resultset转换成一个List 
----->DefaultResultSetHandler.<E> handleResultSets 
----->DefaultResultSetHandler.handleResultSet 
----->DefaultResultSetHandler.handleRowValues 
----->DefaultResultSetHandler.handleRowValuesForSimpleResultMap 
----->DefaultResultSetHandler.getRowValue 
怎么转，肯定先要创建bean，然后再把属性一个个设上去咯，这些都是用反射来做到的。 
-------->DefaultResultSetHandler.createResultObject 
-------->DefaultResultSetHandler.createResultObject 
        先用反射new一个Person对象 
但是如果是嵌套查询且要延迟加载，则用cglib或javassist生成一个代理，这个后文再说。 
-------->ProxyFactory.createProxy 
----->DefaultResultSetHandler.applyAutomaticMappings 
----->DefaultResultSetHandler.applyPropertyMappings 
开始把属性一个个设上去咯 
----->DefaultResultSetHandler.getPropertyMappingValue 
----->typeHandler.getResult 
      如果是普通的值就用相应的typeHandler来从resultset中取得值 
然后就是parent这种有嵌套查询的则调用此嵌套查询方法 
----->getNestedQueryMappingValue 
-------->lazyLoader.addLoader 
         有延迟加载则addLoader，这个后文再说。 
-------->ResultLoader.loadResult 
         没有延迟加载则立即加载 
----------->ResultLoader.selectList 
----------->CachingExecutor.query 
这里的CachingExecutor.query，目的是为了取得id=2的记录 
然后看到了没，这是一个递归调用，这样又转回去了，一个轮回。。。。。。这样就可以不断递归取到父亲、爷爷、曾祖父咯。。。。。。 
不过mybatis还是做了一点优化的，看到日志里只发了2条SQL取3和2两条记录，而1这条记录因为之前就取过了嘛，已经在缓存里了，所以没必要重复取了。当然这也是防死循环的一个方法了，我们看下官方文档的说明： 
引用
本地缓存机制（Local Cache）防止循环引用（circular references）和加速重复嵌套查询。默认值为 SESSION，这种情况下会缓存一个会话中执行的所有查询。
要注意的是这个本地缓存是一级缓存。而二级缓存的处理则是通过CachingExecutor处理的。 
不理解一级缓存、二级缓存的，可参考这篇文章 [MyBatis 
 缓存机制深度解剖 / 自定义二级缓存](http://denger.iteye.com/blog/1126423) 。 
5.延迟加载测试（cglib） 
CglibNPELazyTest.testAncestorAfterQueryingParents方法 
同样的断点分别设在这2句话上 
Java代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- Person expectedAncestor = personMapper.selectById(1);  
- Person person = personMapper.selectById(3);  
我们略过第一句话，执行selectById(3)以后观察日志，发现mybatis只发了1条SQL取得3这条记录 
Txt代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- ==>  Preparing: SELECT Person.id AS Person_id, Person.firstName AS Person_firstName, Person.lastName AS Person_lastName, Person.parent AS Person_parent FROM Person WHERE id = ?   
- ==> Parameters: 3(Integer)  
- <==    Columns: PERSON_ID, PERSON_FIRSTNAME, PERSON_LASTNAME, PERSON_PARENT  
- <==        Row: 3, John jr., Smith, 2
- <==      Total: 1
而当调用了下面的话person.getParent()以后，mybatis才去发另一条SQL取得2这条记录 
Txt代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- ==>  Preparing: SELECT Person.id AS Person_id, Person.firstName AS Person_firstName, Person.lastName AS Person_lastName, Person.parent AS Person_parent FROM Person WHERE id = ?   
- ==> Parameters: 2(Integer)  
- <==    Columns: PERSON_ID, PERSON_FIRSTNAME, PERSON_LASTNAME, PERSON_PARENT  
- <==        Row: 2, John, Smith, 1
- <==      Total: 1
这便是延迟加载的效果了，和hibernate如出一辙啊。如何做到的呢，进一步跟踪。 
DefaultResultSetHandler.getRowValue 
-------->DefaultResultSetHandler.createResultObject 
但是如果是嵌套查询且要延迟加载，则用cglib或javassist生成一个代理。 
-------->ProxyFactory.createProxy 
看图，这次生成的person是一个冒牌的person，它的类型是Person
bdd8787e类型的，是由cglib创建的一个代理 
![](http://dl2.iteye.com/upload/attachment/0101/6541/4810bb7c-7b42-3af9-b552-c4b619101945.png)
然后就是parent这种有嵌套查询的则调用此嵌套查询方法 
----->getNestedQueryMappingValue 
-------->lazyLoader.addLoader 
         有延迟加载则addLoader,把要延迟加载的属性记到ResultLoaderMap里（一个哈希表） 
然后当我们调用person.getParent()以后，图中可清楚的看到这个方法被拦截啦! 
![](http://dl2.iteye.com/upload/attachment/0101/6543/d1289d8f-4d7b-3432-8213-d58db18bd668.png)
Person
bdd8787e.getParent 
-->CglibProxyFactory$EnhancedResultObjectProxyImpl.intercept 
-->ResultLoaderMap.load 
-->ResultLoaderMap$LoadPair.load 
-------->ResultLoader.loadResult 
         立即加载 
----------->ResultLoader.selectList 
----------->CachingExecutor.query 
看到了没，又转回CachingExecutor.query这个入口点了，所以就可以发另1条SQL来取得id=2这条记录了 
6.延迟加载测试（javassist） 
这次我们把cglib换成javassist试一下 
ibatisConfigLazy.xml 
Xml代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- <settings>
- <settingname="proxyFactory"value=""JAVASSIST""/>
- <settingname="lazyLoadingEnabled"value="true"/>
- <settingname="logImpl"value="STDOUT_LOGGING"/>
- </settings>
还是用和cglib相同的方法断点调试，看图，这次生成的person的类型是Person_$$_jvst844_0类型的，是由javassist创建的一个代理 
![](http://dl2.iteye.com/upload/attachment/0101/6547/c030284f-ad5f-3d89-a12b-1df5a426dc4f.png)
然后当我们调用person.getParent()以后，图中可清楚的看到这个方法被拦截啦! 
![](http://dl2.iteye.com/upload/attachment/0101/6549/eb617095-3ca9-3b0c-81e5-3889dd5c2431.png)
Person_$$_jvst844_0.getParent 
-->JavassistProxyFactory$EnhancedResultObjectProxyImpl.invoke 
然后后面就和cglib一模一样了。 
7.resultMap与resultType比较 
resultMap虽然强大，从设计上看很牛叉，但是笔者这里还是提一下自己的观点，笔者觉得一般情况下用用resultType足够了，没必要用resultMap 
resultMap 
优点：使用嵌套查询的话（association@select）多表不用写JOIN这种复杂SQL。 
缺点：“N+1 查询问题”，会导致成百上千的 SQL 语句被执行，不过可以通过延迟加载一部分解决这个性能问题。另一种根治的方法就是用嵌套的resultMap，不过这样写出来的resultMap就更复杂了。 
resultType 
优点：自己写多表关联的SQL比较踏实，可以做SQL的性能调优。 
缺点：导致大量的DTO需要创建，不过可以考虑将多个SQL的select出来的字段做一个最大的并集，这些SQL共用一个DTO 
8.总结 
mybatis的嵌套查询和延迟加载，虽然大家可能不会用到这个功能（至少笔者觉得不实用），但是设计思想是可以借鉴的。提供了cglib,javassist两种方法来实现延迟加载，这和hibernate的延迟加载如出一辙啊！另外一级缓存和二级缓存的使用，也是和hibernate思想一致!里面用到的一些技术，如反射，动态代理，字节码（cglib,javassist）则是java的基础，另加许多设计模式的运用，使得mybatis源码显得比较优雅，大家品读mybatis源码对自己一定是一个提高。 
