# MyBatis高级映射之一对多查询 - 零度的博客专栏 - CSDN博客
2017年09月14日 11:40:18[零度anngle](https://me.csdn.net/zmx729618)阅读数：202
      上一篇博文总结了一下一对一的映射，本文主要总结一下一对多的映射，从上一篇文章中的映射关系图中可知，订单项和订单明细是一对多的关系，所以本文主要来查询订单表，然后关联订单明细表，这样就有一对多的问题出来了。 
　　首先还是先写sql语句，在写sql语句的时候遵循两点：
       1、查询的主表是哪个？ 订单表
       2、查询的关联表是哪个？ 订单明细表
       明确了主表和关联表，下面就可以写sql了，我们在上一节的sql基础上添加订单明细表的关联即可。
```
```
```
SELECT 
  orders.*,
  user.`username`,
  user.`sex`,
  user.`address`,
  orderdetail.`id` orderdetail_id,
  orderdetail.`items_id`,
  orderdetail.`items_num`,
  orderdetail.`orders_id`
FROM
  orders,
  USER,
  orderdetail 
WHERE
```
`  orders.`user_id`=user.`id` AND orders.`id` = orderdetail.`orders_id` `- 1
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
![](http://static.blog.csdn.net/images/save_snippets.png)
　　这样我们就查询出了订单表中的所有字段，user表和orderdetail表的部分字段，当然也可以查询所有字段，这个根据具体需求来定。看一下查询结果： 
![查询结果](https://img-blog.csdn.net/20160613090555302)
　　从结果中可以看出，订单的信息有重复，订单项是不重复的，因为一对多嘛，这很好理解。所以如果我们用resultType来做映射的话就会出现订单信息的重复，我们不希望出现这个结果，即对orders的映射不能出现重复记录的情况。那么我们就需要在Orders.[Java](http://lib.csdn.net/base/java)类中添加一个
```
List<OrderDetail>
 orderDetails
```
属性来封装订单明细项（比较简单，代码就不贴了），最终会将订单信息映射到Orders中，该订单所对应的订单明细映射到Orders中的orderDetails属性中（这跟[hibernate](http://lib.csdn.net/base/javaee)中有点类似，如果是hibernate，也会在Orders类中维护一个装OrderDetail的List）。 
　　有了这个思路，接下来就开始写映射文件了。
```xml
<select id="findOrdersAndOrderDetailResultMap" resultMap="OrdersAndOrderDetailResultMap">
```
```xml
```
```xml
SELECT 
      orders.*,
      user.`username`,
      user.`sex`,
      user.`address`,
      orderdetail.`id` orderdetail_id,
      orderdetail.`items_id`,
      orderdetail.`items_num`,
      orderdetail.`orders_id`
    FROM
      orders,
      USER,
      orderdetail 
    WHERE orders.`user_id`=user.`id` AND orders.`id` = orderdetail.`orders_id`
```
```xml
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
![](http://static.blog.csdn.net/images/save_snippets.png)
　　所以我们要定义一个名为OrdersAndOrderDetailResultMap的resultMap，如下：
```xml
<resultMap type="mybatis.po.Orders" id="OrdersAndOrderDetailResultMap" extends="OrdersUserResultMap">
```
```xml
<!-- 配置映射订单信息和关联的用户信息和上面的一样，继承上面的即可 -->
    <!-- 配置关联的订单明细信息 -->
    <collection property="orderdetails" ofType="mybatis.po.Orderdetail">
        <id column="orderdetail_id" property="id"/>
        <result column="items_id" property="itemsId"/>
        <result column="items_num" property="itemsNum"/>
        <result column="orders_id" property="ordersId"/>
    </collection>
```
```xml
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
![](http://static.blog.csdn.net/images/save_snippets.png)
　　这里看到了一个继承，因为订单信息和关联的用户信息和前面一对一是完全一样的，我们就不需要再写一遍了，`<resultMap>`支持继承，直接继承那个resultMap即可，然后加上订单明细这部分即可。 
`<collection>`是用来处理一对多映射的标签，property属性是Orders.java类中对应的装OrderDetail的List的属性名，就是刚刚定义的那个List，ofType属性表示该List中装的是啥，可以是完全限定名，也可以是别名。然后`<collection>`里面的标签和属性就和前面一样了，不再赘述。 
　　然后定义一下mapper接口即可：
```java
```
```java
public interface UserMapperOrders {
    //省去不相关代码
    //查询订单（关联用户）及订单明细
    public List<Orders> findOrdersAndOrderDetailResultMap() throws Exception;
```
```java
```
```java
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
![](http://static.blog.csdn.net/images/save_snippets.png)
　　到此位置，一对多的映射就写好了，下面[测试](http://lib.csdn.net/base/softwaretest)一下：
```java
@Test
public void testFindOrdersAndOrderDetailResultMap() throws Exception {
```
```java
SqlSession sqlSession = sqlSessionFactory.openSession();
    UserMapperOrders userMapperOrders = sqlSession.getMapper(UserMapperOrders.class);
    List<Orders> list = userMapperOrders.findOrdersAndOrderDetailResultMap();
    System.out.println(list);
```
```java
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
![](http://static.blog.csdn.net/images/save_snippets.png)
　　一对多就总结到这吧，下一篇博文将总结一下mybatis中多对多映射。 
