# Mybatis批量插入返回插入成功后的主键id - z69183787的专栏 - CSDN博客
2017年08月04日 15:58:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3949
我们都知道Mybatis在插入单条数据的时候有两种方式返回自增主键：
1、对于支持生成自增主键的[数据库](http://lib.csdn.net/base/mysql)：增加
 useGenerateKeys和keyProperty ，<insert>标签属性。
2、不支持生成自增主键的数据库：使用<selectKey>。
但是怎么对批量插入数据返回自增主键的解决方式网上看到的还是比较少，至少百度的结果比较少。
Mybatis官网资料提供如下：
First, if your database supports auto-generated key fields (e.g. [MySQL](http://lib.csdn.net/base/mysql) and
 SQL Server), then you can simply set useGeneratedKeys="true" and set the keyProperty to
 the target property and you're done. For example, if the Authortable above had used an auto-generated column type
 for the id, the statement would be modified as follows:
<insertid="insertAuthor"useGeneratedKeys="true"keyProperty="id">
  insert into Author (username,password,email,bio)
  values (#{username},#{password},#{email},#{bio})
</insert>
If your database also supports multi-row insert, you
 can pass a list or an array of Authors and retrieve
 the auto-generated keys.
<insertid="insertAuthor"useGeneratedKeys="true"keyProperty="id">
  insert into Author (username, password, email, bio) values
  <foreachitem="item"collection="list"separator=",">
    (#{item.username}, #{item.password}, #{item.email}, #{item.bio})
  </foreach></insert>
从官网资料可以看出Mybatis是支持批量插入时返回自增主键的。
但是在本地[测试](http://lib.csdn.net/base/softwaretest)的时候使用上述方式确实不能返回自增id，而且还报错（不认识keyProperty中指定的Id属性），然后在网上找相关资料。终于在Stackoverflow上面找到了一些信息。
解决办法：
1、升级Mybatis版本到3.3.1。官方在这个版本中加入了批量新增返回主键id的功能
2、在Dao中不能使用@param注解。
3、Mapper.xml中使用list变量（parameterType="[Java](http://lib.csdn.net/base/java).util.List"）接受Dao中的参数集合。
下面是具体代码过程，可供参考
mapper.xml层代码
- <!-- 批量新增 -->
- <insertid="batchInsert"parameterType="java.util.List"useGeneratedKeys="true"keyProperty="id">
-         INSERT INTO  
- <includerefid="t_shop_resource"/>
-         (relation_id, summary_id, relation_type)  
-         VALUES  
- <foreachcollection="list"index="index"item="shopResource"separator=",">
-             (  
-                 #{shopResource.relationId}, #{shopResource.summaryId}, #{shopResource.relationType}  
-             )  
- </foreach>
- </insert>
dao实现层代码
- public List<ShopResource> batchinsertCallId(List<ShopResource> shopResourceList)  
-     {  
- this.getSqlSession().insert(getStatement(SQL_BATCH_INSERT_CALL_ID), shopResourceList);  
- return shopResourceList;// 重点介绍
-     }  
为什么最后返回的参数不是挑用mybatis后的insert的返回值呢，细心的话可以发现，如果使用debug模式观察，会看到调用mybatis后insert的返回值是[]，也就是空集合元素，在mybatis3.3.1中，虽然加入了批量新增返回主键id的功能，但是它是这样运行的，在需要新增插入新元素集合对象时，它会需要参数对象，当执行完插入操作后，给之前的参数对象设置id值，也就是改变了需要插入对象集合中的元素的属性id值， 所以接收返回时，返回方法形参参数即可，同样的地址引用改变了内容，返回后的集合也是改变后的集合。
参考地址：
[http://stackoverflow.com/questions/18566342/mybatis-use-generated-keys-for-batch-insert](http://stackoverflow.com/questions/18566342/mybatis-use-generated-keys-for-batch-insert)
[http://stackoverflow.com/questions/28453475/mybatis-getting-id-from-inserted-array-of-object-returns-error](http://stackoverflow.com/questions/28453475/mybatis-getting-id-from-inserted-array-of-object-returns-error)
[https://github.com/mybatis/mybatis-3/pull/324](https://github.com/mybatis/mybatis-3/pull/324)
[http://stackoverflow.com/questions/28453475/mybatis-getting-id-from-inserted-array-of-object-returns-error#](http://stackoverflow.com/questions/28453475/mybatis-getting-id-from-inserted-array-of-object-returns-error#)
[https://github.com/abel533/mybatis-3/blob/master/src/main/java/org/apache/ibatis/executor/keygen/Jdbc3KeyGenerator.java](https://github.com/abel533/mybatis-3/blob/master/src/main/java/org/apache/ibatis/executor/keygen/Jdbc3KeyGenerator.java)
