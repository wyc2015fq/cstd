# Mybatis Cache探究 - z69183787的专栏 - CSDN博客
2017年07月28日 11:25:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：950
这里先不讨论第三方的cache集成（有memcache/oscache集成，控制力度跟下面说的一样比较粗，也可以跟spring3的cache做集成， spring3的cache是基于方法的缓存,具体可以在github找一下[spring](http://lib.csdn.net/base/javaee) MyBatis
 Memcached Bootstrap）。
先使用最简单的配置cache，比较粗放：
一，官方手册
http://mybatis.github.io/mybatis-3/zh/sqlmap-xml.html#cache
<cache/>
字面上看就是这样。这个简单语句的效果如下:
- 映射语句文件中的所有 select 语句将会被缓存。
- 映射语句文件中的所有 insert,update 和 delete 语句会刷新缓存。
- 缓存会使用 Least Recently Used(LRU,最近最少使用的)算法来收回。
- 根据时间表(比如 no Flush Interval,没有刷新间隔), 缓存不会以任何时间顺序 来刷新。
- 缓存会存储列表集合或对象(无论查询方法返回什么)的 1024 个引用。
- 缓存会被视为是 read/write(可读/可写)的缓存,意味着对象检索不是共享的,而 且可以安全地被调用者修改,而不干扰其他调用者或线程所做的潜在修改。
所有的这些属性都可以通过缓存元素的属性来修改。比如:
**[xml]**[view
 plain](http://blog.csdn.net/d8111/article/details/45249851#)[copy](http://blog.csdn.net/d8111/article/details/45249851#)
- <cache
- eviction="FIFO"
- flushInterval="60000"
- size="512"
- readOnly="true"/>
**最重要的是readonly=ture， 说明cache对象只读，update的时候，cache不会更新！那么换个角度来说，**
**其实代表了发生任何insert，update，delete操作的时候，会全量清理cache数据，确保一致性。**
**如果readonly=false的时候，就比较好玩了，更新数据会写入新的cache，并且DO对象要实现序列化**
**(通过序列化,慢一些但更是安全,因此默认是false).**
[08/22 11:58:27][main] DEBUG org.apache.ibatis.cache.decorators.LoggingCache55: Cache Hit Ratio [com.xx.dao.UserMapper]: 0.25
[08/22 11:58:27][main] DEBUG org.springframework.jdbc.datasource.DataSourceUtils110: Fetching JDBC Connection from DataSource
二。[测试](http://lib.csdn.net/base/softwaretest)代码
  流程是：
1.select id= 10035L，10036L的，数据
2.立即再查询10035L，cache命中
3.更新10035L数据
4.再查询10036L，cache未能命中
也就是说cache是全量清理的
**[java]**[view
 plain](http://blog.csdn.net/d8111/article/details/45249851#)[copy](http://blog.csdn.net/d8111/article/details/45249851#)
- /**
-  * 测试cache的读取和更新
-  * @author weisong
-  *
-  */
- publicclass MybatisCacheTest {  
- publicstaticvoid main(String[] args) {  
-         ApplicationContext ac = new ClassPathXmlApplicationContext("spring-core.xml","spring-task.xml","spring-jms.xml");  
-         UserMapper u = (UserMapper)ac.getBean("userMapper");  
-         User u0 = u.selectByPrimaryKey(10035L);  //sql
-         System.out.println("u0-name="+u0.getNickname());  
-         User u00 = u.selectByPrimaryKey(10035L); //cache here! DEBUG org.apache.ibatis.cache.decorators.LoggingCache55: Cache Hit Ratio [com.xx.dao.UserMapper]: 0.0
-         System.out.println("u00(cache)-name="+u00.getNickname());  
- 
-         User u1 = u.selectByPrimaryKey(10036L);  
-         u0.setNickname("wei");  
-         u.updateByPrimaryKey(u0); //update here
- 
-         User u01 = u.selectByPrimaryKey(10035L);  
-         System.out.println("u01-name="+u01.getNickname()); //should no cache, next will cache
- 
-         User u11 = u.selectByPrimaryKey(10036L);  
-         System.out.println("u11-name="+u11.getNickname()); //!!!sql , no cache here
- 
- 
-     }  
- }  
展开来继续说：
1.默然cache非常适合读多写少的小数据量级表；[大数据](http://lib.csdn.net/base/hadoop)量级很有可能cache重建的时候直接雪崩
一级缓存：
一级缓存是默认的.
[测试](http://lib.csdn.net/base/softwaretest):在WEB页面同一个查询执行两次从日志里面看同样的sql查询执行两次。
2次sql查询,看似我们使用了同一个sqlSession,但是实际上因为我们的dao继承了SqlSessionDaoSupport,而SqlSessionDaoSupport内部sqlSession的实现是使用用动态代理实现的,这个动态代理sqlSessionProxy使用一个模板方法封装了select()等操作,每一次select()查询都会自动先执行openSession(),执行完close()以后调用close()方法,相当于生成了一个新的session实例,所以**我们无需手动的去关闭这个session()**(关于这一点见下面mybatis的官方文档),当然**也无法使用mybatis的一级缓存**,也就是说mybatis的一级缓存在[spring](http://lib.csdn.net/base/javaee)中是没有作用的.
二级缓存：
配置：
<mapper namespace="com.cnblogs.yjmyzz.mybatis.mapper.OrderMapper">
    <cache 
        eviction="LRU"
        flushInterval="60000"
        size="1024"
        readOnly="true"
    />    
...
</mapper>
这里有几个要注意的地方：
eviction是缓存的淘汰[算法](http://lib.csdn.net/base/datastructure)，可选值有"LRU"、"FIFO"、"SOFT"、"WEAK"，缺省值是LRU
flashInterval指缓存过期时间，单位为毫秒，60000即为60秒，缺省值为空，即只要容量足够，永不过期
size指缓存多少个对象，默认值为1024
readOnly是否只读，如果为true，则所有相同的sql语句返回的是同一个对象（有助于提高性能，但并发操作同一条数据时，可能不安全），如果设置为false，则相同的sql，后面访问的是cache的clone副本 (通过序列化,慢一些但更是安全,因此默认是false).
上面这个是全局设置，在每条单独的sql语句上，还可以有局部设置，比如：
<select id="getOrder" parameterType="int" resultType="TOrder"  useCache="false">
        ...
</select>
useCache="false"表示该select语句不使用缓存（即使xml最开头的全局cache启用）
默认情况下，如果全局开启了缓存，insert/update/delete成功后，会自动刷新相关的缓存项，但有一点要特别注意：在mybatis与[hibernate](http://lib.csdn.net/base/javaee)混用时，由于mybatis与hibernate的缓存是无关的，如果用mybatis做select查询，用hibernate做insert/update/delete，hibernate对数据的修改，并不会刷新mybatis的缓存。
