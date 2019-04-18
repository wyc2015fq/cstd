# MyBatis总结 - 深之JohnChen的专栏 - CSDN博客

2017年11月08日 23:13:59[byxdaz](https://me.csdn.net/byxdaz)阅读数：782
个人分类：[MyBatis](https://blog.csdn.net/byxdaz/article/category/6892925)



1、接口绑定：两种方法，基于注解或者基于xml文档mapper，但要注意mapper的namespace要与接口路径完全一致。

2、对象关系映射格式转换：通过设置resultMap和ResultType，将数据库中的记录转换为代码的bean对象。得到list或者对象。

3、当实体类中的属性名和表中的字段名不一致时，使用MyBatis进行查询操作时无法查询出相应的结果的问题以及针对问题采用的两种办法：

　　解决办法一: 通过在查询的sql语句中定义字段名的别名，让字段名的别名和实体类的属性名一致，这样就可以表的字段名和实体类的属性名一一对应上了，这种方式是通过在sql语句中定义别名来解决字段名和属性名的映射关系的。

　　解决办法二: 通过<resultMap>来映射字段名和实体类属性名的一一对应关系。这种方式是使用MyBatis提供的解决方式来解决字段名和属性名的映射关系的。

<selectid="selectOrderResultMap" parameterType="int"resultMap="orderResultMap">

   select * from orders where order_id=#{id}

</select>

<!--通过<resultMap>映射实体类属性名和表的字段名对应关系 -->

<resultMaptype="me.gacl.domain.Order" id="orderResultMap">

   <!-- 用id属性来映射主键字段 -->

   <id property="id" column="order_id"/>

   <!-- 用result属性来映射非主键字段 -->

   <result property="orderNo" column="order_no"/>

   <result property="price"column="order_price"/>

</resultMap>

//注：order_id, order_no, order_price为数据库表orders字段；id，no,price为Order类属性。

4、缓存

　　正如大多数持久层框架一样，MyBatis 同样提供了一级缓存和二级缓存的支持

1）一级缓存: 基于PerpetualCache 的 HashMap本地缓存，其存储作用域为 Session，当 Session flush 或 close 之后，该Session中的所有 Cache 就将清空。

2）二级缓存与一级缓存其机制相同，默认也是采用PerpetualCache，HashMap存储，不同在于其存储作用域为 Mapper(Namespace)，并且可自定义存储源，如 Ehcache。

使用二级缓存时，需要开启二级缓存。开启二级缓存，在userMapper.xml文件中添加如下：

<mapper namespace="com.zychen.mapper.userMapper">

<!-- 开启二级缓存 -->

<cache/>

cache标签常用属性：

<cache 

eviction="FIFO"  <!--回收策略为先进先出-->

flushInterval="60000" <!--自动刷新时间60s-->

size="512" <!--最多缓存512个引用对象-->

readOnly="true"/> <!--只读-->

二级缓存说明

2.1. 映射语句文件中的所有select语句将会被缓存。

2.2. 映射语句文件中的所有insert，update和delete语句会刷新缓存。

2.3. 缓存会使用Least Recently Used（LRU，最近最少使用的）算法来收回。

2.4. 缓存会根据指定的时间间隔来刷新。

2.5. 缓存会存储1024个对象。

3）对于缓存数据更新机制，当某一个作用域(一级缓存Session/二级缓存Namespaces)的进行了 C/U/D 操作后，默认该作用域下所有 select 中的缓存将被clear。

5、mybatis做like模糊查询

1）.  参数中直接加入%%

　　param.setUsername("%CD%");

      param.setPassword("%11%");

       <select  id="selectPersons"resultType="person" parameterType="person">

              selectid,sex,age,username,password from person where true 

                     <iftest="username!=null"> AND username LIKE #{username}</if>

                     <iftest="password!=null">AND password LIKE #{password}</if>

       </select>

2）.  bind标签

<select id="selectPersons"resultType="person" parameterType="person">

 <bind name="pattern" value="'%' + _parameter.username+ '%'" />

 select id,sex,age,username,password 

 from person

 where username LIKE #{pattern}

</select>

3）.${}

<select id="selectPersons"resultType="person" parameterType="person">

 select id,sex,age,username,password 

 from person

 where username LIKE ‘%${username}%’

</select>

6、#{}与${}区别

#{}表示一个占位符号，比如：#{id}，其中的id表示接收输入的参数，参数名为id，如果输入的参数是简单类型，#{}中的参数名可以为任意名。

${}表示一个拼接符号，将接收到参数的内容不加任何修饰拼接在sql中，但是拼接sql会引起sql注入。

7、selectOne和selectList

selectOne表示查询出一条记录进行映射。

selectList表示查询出一个列表（多条记录）进行映射。

8、resultMap可以实现高级映射（使用association、collection实现一对一及一对多映射）

一对一关联，使用assacation标签。

assacation标签的属性进行解释一下：
|property|对象属性的名称|
|----|----|
|javaType|对象属性的类型|
|column|所对应的外键字段名称|
|select|使用另一个查询封装的结果|

一对多关联，使用collection标签。collection标签来解决一对多的关联查询，ofType属性指定集合中元素的对象类型。

比如：

 <collection property="questions" ofType="map" javaType="list">

对应的java 形态为 ：List<Map<String,Object>>

实例使用参考这篇文章：http://www.cnblogs.com/xdp-gacl/p/4264440.html

9、延迟加载，虽然association、collection能实现表间关联，同时association、collection也具备延迟加载功能。

延迟加载：先从单表查询、需要时再从关联表去关联查询，大大提高数据库性能，因为查询单表要比关联查询多张表速度要快。

使用association实现延迟加载

需要定义两个mapper的方法对应的statement。

1）、只查询订单信息

　　SELECT * FROM orders

在查询订单的statement中使用association去延迟加载（执行）下边的satatement(关联查询用户信息)

2）、关联查询用户信息

　　通过上边查询到的订单信息中user_id去关联查询用户信息

OrderMapper.xml的延迟加载的核心代码：

　　使用association中的select指定延迟加载去执行的statement的id。

<!-- 查询订单关联查询用户，用户信息按需延迟加载的 resultMap定义 -->

   <resultMap type="com.mybatis.entity.Orders" id="ordersUserLazyLoading">

                <!--对订单信息进行映射配置  -->

           <id column="id" property="id"/>

           <result column="user_id" property="userid"/>

           <result column="number" property="number"/>

           <result column="createtime"property="createTime"/>

           <result column="note" property="note"/>

            <!-- 实现对用户信息进行延迟加载

                select：指定延迟加载需要执行的statement的id（是根据user_id查询用户信息的statement） 

                column：订单信息中关联用户信息查询的列，是user_id

                关联查询的sql理解为：

                 SELECT orders.*,

                    (SELECT username FROM USERWHERE orders.user_id = user.id)username,

                    (SELECT sex FROM USER WHEREorders.user_id = user.id)sex

                 FROM orders

           -->

            <associationproperty="user" javaType="com.mybatis.entity.User"select="findUserById" column="user_id"/>

   </resultMap>

       <!-- 根据Id查询用户，用于测试延迟加载 -->

       <select id="findUserById" parameterType="int"resultType="com.mybatis.entity.User" >

                select * from t_user whereid=#{id}

       </select>

   <!-- 查询订单关联用户，用户信息延迟加载 -->

   <select id="findOrdersUserLazyLoading"resultMap="ordersUserLazyLoading">

           select * from orders

</select>

OrderMapper.java的代码：

public interface OrdersCustomMapper {

   /**查询订单，关联查询用户，用户按需延迟加载*/

   public List<Orders>findOrdersUserLazyLoading();

   /**根据Id查询用户（这个方法本应该放在UserMapper类的，测试方便先放在这）*/

   public User findUserById(int id);

}

测试思路及代码：

1）、执行上边mapper方法（findOrdersUserLazyLoading），内部去调用OrdersMapperCustom中的findOrdersUserLazyLoading只查询orders信息（单表）。

2）、在程序中去遍历上一步骤查询出的List<Orders>，当我们调用Orders中的getUser方法时，开始进行延迟加载。

3）、延迟加载，去调用findUserbyId这个方法获取用户信息。

// 查询用户关联查询用户，用户信息延迟加载

       @Test

       public void TestFindOrdersUserLazyLoading() {

           SqlSession sqlSession = sqlSessionFactory.openSession();

           // 创建代理对象

           OrdersCustomMapper oc = sqlSession.getMapper(OrdersCustomMapper.class);

           // 调用mapper的方法

           List<Orders> list = oc.findOrdersUserLazyLoading();

           for(Orders order: list){

                //执行getUser()去查询用户信息，这里实现延迟加载

                User user = order.getUser();

                System.out.println(user);

           }

           sqlSession.close();

       }

延迟加载在mybatis核心配置文件sqlMapConfig.xml中的配置

mybatis默认没有开启延迟加载，需要在MyBaits-Config.xml中setting配置。

在mybatis核心配置文件中配置：

　　　　lazyLoadingEnabled、aggressiveLazyLoading
|设置项|描述|允许值|默认值|
|----|----|----|----|
|lazyLoadingEnabled|全局性设置懒加载。如果设为‘false’，则所有相关联的都会被初始化加载。|true | false|false|
|aggressiveLazyLoading|当设置为‘true’的时候，懒加载的对象可能被任何懒属性全部加载。否则，每个属性都按需加载。|true | false|true|

<!-- 全局参数的配置 -->

   <settings>

           <!--打开延迟加载的开关  -->

       <setting name="lazyLoadingEnabled"value="true"/>

           <!--将积极加载改为消极加载及按需加载  -->

       <setting name="aggressiveLazyLoading"value="false"/>

</settings>

10、mybatis的分页功能，可以自己利用mysql代码实现，也可以利用mybatis分页插件,如pageHelper。

11、spring在于mybatis集成时，spring负责什么呢？谁来维护datasource，谁来建立sqlSessionFactory?

答:spring作为多个框架的粘合剂，spring负责建立datasource,sqlsessionFactpry。充分利用spring的ioc和aop功能。

spring 配置文件：applicationContext.xml 

 <bean id="dataSource"class="org.apache.commons.dbcp.BasicDataSource">//spring管理配置datasource

    <property name="driverClassName"value="com.mysql.jdbc.Driver"/> 

    <property name="url" value="jdbc:mysql://127.0.0.1:3306/mybatis?characterEncoding=utf8"/>

    <property name="username" value="root"/> 

    <property name="password" value="password"/> 

 </bean> 

 <bean id="sqlSessionFactory"class="org.mybatis.spring.SqlSessionFactoryBean"> //spring管理配置sqlsessionFactory

    <!--dataSource属性指定要用到的连接池--> 

    <property name="dataSource" ref="dataSource"/>

    <!--configLocation属性指定mybatis的核心配置文件--> 

    <property name="configLocation"value="config/Configuration.xml"/> 

 </bean>

