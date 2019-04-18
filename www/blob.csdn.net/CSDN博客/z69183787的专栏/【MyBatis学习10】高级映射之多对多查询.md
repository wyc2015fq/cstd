# 【MyBatis学习10】高级映射之多对多查询 - z69183787的专栏 - CSDN博客
2017年07月28日 10:55:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：240
个人分类：[Mybatis-学习](https://blog.csdn.net/z69183787/article/category/7026142)
本文来总结一下mybatis中的多对多映射，从第8节的文章中可以看出，用户表和商品表示多对多关系，它们两的多对多是通过订单项和订单明细这两张表所关联起来的，那么这一节主要来总结一下用户表和商品表之间的多对多映射。 
首先在上一节的基础上继续写sql，
```
SELECT 
  orders.*,
  user.`username`,
  user.`sex`,
  user.`address`,
  orderdetail.`id` orderdetail_id,
  orderdetail.`items_id`,
  orderdetail.`items_num`,
  orderdetail.`orders_id`,
  items.`name` items_name,
  items.`detail` items_detail,
  items.`price` items_price
FROM
  orders,
  USER,
  orderdetail,
  items
WHERE orders.`user_id`=user.`id` AND orders.`id` = orderdetail.`orders_id` AND orderdetail.`items_id`=items.`id`
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
因为多对多比较复杂，总公共有四张表，我们先来分析一下思路：
> - 将用户信息映射到User中；
- 在User类中添加订单列表属性`List<Orders>ordersList`，将用户创建的订单映射到ordersList中；
- 在Orders中添加订单明细列表属性`List<OrderDetail>orderDetails`，将订单的明细映射到orderDetails中；
- 在OrderDetail中添加Items属性，将订单明细所对应的商品映射到Items中。
经过这样分析后，感觉虽然有点复杂，但是好像不是很难的样子，映射的方法也跟前面的一样，只不过这里表有点多，关系有点复杂。下面来写映射文件：
```xml
<select id="findUserAndItemsResultMap" resultMap="UserAndItemsResultMap">
    SELECT 
      orders.*,
      user.`username`,
      user.`sex`,
      user.`address`,
      orderdetail.`id` orderdetail_id,
      orderdetail.`items_id`,
      orderdetail.`items_num`,
      orderdetail.`orders_id`,
      items.`name` items_name,
      items.`detail` items_detail,
      items.`price` items_price
    FROM
      orders,
      USER,
      orderdetail,
      items
    WHERE orders.`user_id`=user.`id` AND orders.`id` = orderdetail.`orders_id` AND orderdetail.`items_id`=items.`id`
</select>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
接下来，写名为UserAndItemsResultMap的resultMap，由于主表查询的是User，所以在resultMap中的type应该写成User的完全限定名或者别名：
```xml
<resultMap type="mybatis.po.User" id="UserAndItemsResultMap">
    <!-- 用户信息 -->
    <id column="user_id" property="id"/>
    <result column="username" property="username"/>
    <result column="sex" property="sex"/>
    <result column="address" property="address"/>
    <!-- 订单信息 -->
    <!-- 一个用户对应多个订单，使用collection -->
    <collection property="ordersList" ofType="mybatis.po.Orders">
        <id column="id" property="id"/>
        <result column="user_id" property="userId"/>
        <result column="number" property="number"/>
        <result column="createtime" property="createtime"/>
        <result column="note" property="note"/>
        <!-- 订单明细信息 -->
        <!-- 一个订单包括多个明细，使用collection -->
        <collection property="orderdetails" ofType="mybatis.po.Orderdetail">
            <id column="orderdetail_id" property="id"/>
            <result column="items_id" property="itemsId"/>
            <result column="items_num" property="itemsNum"/>
            <result column="orders_id" property="ordersId"/>
            <!-- 商品信息 -->
            <!-- 一个明细对应一个商品信息，使用association -->
            <association property="items" javaType="mybatis.po.Items">
                <id column="items_id" property="id"/>
                <result column="items_name" property="name"/>
                <result column="items_detail" property="detail"/>
                <result column="items_price" property="price"/>
            </association>
        </collection>
    </collection>
</resultMap>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
是不是看起来有点复杂了，但是很有条理，一步步的深入即可，仔细看看，其实不是很复杂，就是有点多而已，一个个套进去呗~ 
下面就是写mapper接口了：
```java
public interface UserMapperOrders {
    //省去不相关代码
    //查询用户购买商品信息
    public List<User> findUserAndItemsResultMap() throws Exception;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
[测试](http://lib.csdn.net/base/softwaretest)一下：
```java
@Test
public void findUserAndItemsResultMap() throws Exception {
    SqlSession sqlSession = sqlSessionFactory.openSession();
    UserMapperOrders userMapperOrders = sqlSession.getMapper(UserMapperOrders.class);
    List<User> list = userMapperOrders.findUserAndItemsResultMap();
    System.out.println(list);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 1
- 2
- 3
- 4
- 5
- 6
- 7
这样多对多的映射就搞定了。不过还有个问题，就是这里多对多的查询会把所有关联的表的信息都查询出来，然后放到pojo中的对应的List或者某个类中，所以即使我只查了个用户信息，但是这个用户里包含了订单，订单项，商品等信息，感觉装的有点多，好像有时候并不需要这么多冗余的数据出来，但是如果用resultType的话查询出来的字段必须对应pojo中的属性，如果有List等，需要手动装入才行。所以下面总结一下对于这种查询数据比较多的时候，resultType和resultMap各有什么作用？
> - 比如我们只需要将查询用户购买的商品信息明细清单（如用户名、用户地址、购买商品名称、购买商品时间、购买商品数量），那么我们完全不需要其他的信息，这个时候就没必要使用resultMap将所有的信息都搞出来，我们可以自己定义一个pojo，包含我们需要的字段即可，然后查询语句只查询我们需要的字段，这样使用resultType会方便很多。
- 如果我们需要查询该用户的所有详细信息，比如用户点击该用户或者鼠标放上去，会出来跟该用户相关的订单啊，订单明细啊，商品啊之类的，然后我们要点进去看下详细情况的时候，那就需要使用resultMap了，必须将所有信息都装到这个User中，然后具体啥信息再从User中取，很好理解。
- 总结一点：使用resultMap是针对那些对查询结果映射有特殊要求的功能，，比如特殊要求映射成list中包括多个list。否则使用resultType比较直接。
到这里，mybatis的多对多映射就总结完了。 
