# mybatis高级映射 - 深之JohnChen的专栏 - CSDN博客

2017年11月08日 23:08:13[byxdaz](https://me.csdn.net/byxdaz)阅读数：773
个人分类：[MyBatis](https://blog.csdn.net/byxdaz/article/category/6892925)



转载自   http://blog.csdn.net/zd836614437/article/details/51814108

一、介绍 

resultType ： 

作用：将查询结果按照sql列名pojo属性名一致性映射到pojo中。 

场合：

常见一些明细记录的展示，比如用户购买商品明细，将关联查询信息全部展示在页面时，此时可直接使用 resultType 将每一条记录映射到 pojo 中，在前端页面遍历 list （ list 中是 pojo ）即可。

resultMap ：

使用 association 和 collection 完成一对一和一对多高级映射（对结果有特殊的映射要求）。

association ：

作用：将关联查询信息映射到一个pojo对象中。

场合：

为了方便查询关联信息可以使用 association 将关联订单信息映射为用户对象的 pojo 属性中，比如：查询订单及关联用户信息。

使用 resultType 无法将查询结果映射到 pojo 对象的 pojo 属性中，根据对结果集查询遍历的需要选择使用 resultType 还是 resultMap 。

collection ：

作用：将关联查询信息映射到一个list集合中。

场合：

为了方便查询遍历关联信息可以使用 collection 将关联信息映射到 list 集合中，比如：查询用户权限范围模块及模块下的菜单，可使用 collection 将模块映射到模块 list 中，将菜单列表映射到模块对象的菜单 list 属性中，这样的作的目的也是方便对查询结果集进行遍历查询。

如果使用 resultType 无法将查询结果映射到 list 集合中。

二、一对一查询

resultType 和 resultMap 实现一对一查询小结：

a.resultType ：使用 resultType 实现较为简单，如果 pojo 中没有包括查询出来的列名，需要增加列名对应的属性，即可完成映射。

b.如果没有查询结果的特殊要求建议使用 resultType 。

c.resultMap ：需要单独定义 resultMap ，实现有点麻烦，如果对查询结果有特殊的要求，使用 resultMap 可以完成将关联查询映射 pojo 的属性中。

d.resultMap 可以实现延迟加载， resultType 无法实现延迟加载。

2.1.需求：查询订单信息，关联查询用户信息；

2.2 resultType实现

```java
public class Orders {
  /** 主键订单Id */
  private Integer id;
  /** 下单用户id */
  private Integer userid;
  /** 订单号 */
  private String number;
  /** 创建订单时间 */
  private Date createTime;
  /** 备注 */
  private String note;
  // 用户信息
  private User user;
  // 订单明细
  private List<OrderDetail> orderdetails;
     //  getter and setter ......
}

public class OrdersCustom extends Orders {
  // 添加用户的属性
  private String username;
  private String sex;
  private String address;
    // getter and setter......
}

public interface OrdersCustomMapper {
    /** 查询订单，关联查询用户信息 */
    public List<OrdersCustom> findOrdersUser();
}
```

```
<mapper namespace="com.mybatis.mapper.OrdersCustomMapper">
    <!-- 查询订单，关联查询用户信息 -->
  <select id="findOrdersUser" resultType="com.mybatis.entity.OrdersCustom">
  SELECT t1.*,
    t2.username,
    t2.sex,
    t2.address
  FROM
    orders t1,
    t_user t2
  WHERE t1.user_id=t2.id
  </select>
</mapper>
```
2.3 resultMap实现

```
<resultMap type="com.mybatis.entity.Orders" id="OrdersUserResultMap">
          <!-- 配置映射的订单信息 -->

          <!-- id:查询列中的唯一标识,订单信息中的唯一标识，如果多列组成唯一标识(如：一般数据库设计中的字典表 使用联合主键)，就需要配置多个id 
              column:订单信息的唯一标识 列
              property:订单信息的唯一标识列所映射到orders中的那个属性(假如：数据库中orders表中的主键为orders_id,而实体属性名称为ordersId,
                  则这个配置应为<id column="orders_id" property="ordersId"/>,类似hibernate实体映射文件配置)。
          -->
         <id column="id" property="id"/>
         <result column="user_id" property="userid"/>
         <result column="number" property="number"/>
         <result column="createtime" property="createTime"/>
         <result column="note" property="note"/>

         <!-- 配置映射的关联用户信息 -->

         <!--association:用于映射关联查询单个对象的信息
             property:要将关联查询的用户信息映射到Orders中那个属性
           -->
         <association property="user" javaType="com.mybatis.entity.User">
             <!-- id:关联查询用户的唯一标识 
                 column:指定唯一标识用户信息的列
                 property:映射到user的那个属性
             -->
             <id column="user_id" property="id"/>
             <result column="username" property="username"/>
             <result column="sex" property="sex"/>
             <result column="address" property="address"/>
         </association>

     </resultMap>
```

```
<!-- 查询订单，关联查询用户信息,使用resultMap实现 -->
      <select id="findOrdersUserResultMap" resultMap="OrdersUserResultMap">
              SELECT t1.*,
                  t2.username,
                  t2.sex,
                  t2.address
              FROM
                  orders t1,
                  t_user t2
             WHERE t1.user_id=t2.id
     </select>
```

三、一对多查询

mybatis 使用 resultMap 的 collection 对关联查询的多条记录映射到一个 list 集合属性中。

使用 resultType 实现：将订单明细映射到orders中的orderdetails中，需要自己处理，使用双重循环遍历，去掉重复记录，将订单明细放在orderdetails中。

3.1 需求：查询订单(关联用户)及订单明细； 

3.2 映射思路 

3.2. 在 orders.java 类中添加 List orderDetails 属性(上面实体已添加) 。

最终会将订单信息映射到 orders 中，订单所对应的订单明细映射到 orders 中的 orderDetails 属性中.

3.3 核心代码

```
<!-- 查询订单关联查询用户及订单明细 -->
<select id="findOrdersAndOrderDetailResultMap" resultMap="ordersAndOrderDetailResultMap">
    SELECT 
      t1.*,
      t2.username,
      t2.sex,
      t2.address,
      t3.id orderdetail_id,
      t3.items_id,
      t3.items_num,
      t3.orders_id
    FROM
      orders t1,
      t_user t2,
      orderdetail t3
    WHERE t1.user_id = t2.id AND t3.orders_id=t1.id
</select>
```

```
<!-- 查询订单(关联用户)及订单明细的resultMap -->
<resultMap type="com.mybatis.entity.Orders" id="ordersAndOrderDetailResultMap" extends="OrdersUserResultMap">
  <!-- 订单信息 -->
  <!-- 关联用户信息 -->
  <!-- 使用extends继承，不用在中配置订单信息和用户信息的映射-->
  <!-- 关联订单明细信息 
    一个订单关联查询出了多条订单明细,要使用collection映射
    collection:对关联查询到的多条记录映射到集合中
    property:将关联查询到的多条记录映射到orders类的那个属性
    ofType:指定映射的集合属性中pojo的类型
  -->
  <collection property="orderdetails" ofType="com.mybatis.entity.OrderDetail">
    <!-- id:唯一标识
       property：要将订单明细的唯一标识映射到com.mybatis.entity.OrderDetail的那个属性
     -->
    <id column="orderdetail_id" property="id"/>
    <result column="items_id" property="itemsId"/>
    <result column="items_num" property="itemsNum"/>
    <result column="orders_id" property="ordersId"/>
  </collection>
</resultMap>
```

四、多对多查询

4.1.需求：查询用户以及用户购买的商品信息

4.2.映射思路

将用户信息映射到 user 中。

在 user 类中添加订单列表属性 List orderslist ，将用户创建的订单映射到 orderslist;

在 Orders 中添加订单明细列表属性 Listorderdetials ，将订单的明细映射到 orderdetials;

在 OrderDetail 中添加 Items 属性，将订单明细所对应的商品映射到 Item；

4.3 核心代码

```
<!-- 查询用户即购买的商品信息的ResultMap -->
  <resultMap type="com.mybatis.entity.User" id="userAndItemsResultMap">
    <!-- 用户信息 -->
    <id column="user_id" property="id"/>
    <result column="username" property="username"/>
    <result column="sex" property="sex"/>
    <result column="address" property="address"/>
  <!-- 订单信息
    一个用户对应多个订单，使用collection映射 -->
    <collection property="ordersList" ofType="com.mybatis.entity.Orders">
       <id column="id" property="id"/>
       <result column="user_id" property="userid"/>
      <result column="number" property="number"/>
      <result column="createtime" property="createTime"/>
      <result column="note" property="note"/>
     <!-- 订单明细
         一个订单包括 多个明细
        -->
        <collection property="orderdetails" ofType="com.mybatis.entity.OrderDetail">
            <id column="orderdetail_id" property="id"/>
           <result column="items_id"   property="itemsId"/>
           <result column="items_num"  property="itemsNum"/>
           <result column="orders_id"  property="ordersId"/>
           <!-- 商品信息
              一个订单明细对应一个商品
             -->
           <association property="items" javaType="com.mybatis.entity.Items">
             <id column="items_id" property="id"/>
             <result column="items_name" property="itemsName"/>
             <result column="items_detail" property="detail"/>
             <result column="items_price" property="price"/>
           </association>
        </collection>
      </collection>
  </resultMap>
<!-- 查询用户及用户购买的商品信息，使用resulaMap-->
<select id="findUserAndItemsResultMap" resultMap="userAndItemsResultMap">
    SELECT 
         t1.*,
         t2.username,
         t2.sex,
         t2.address,
         t3.id orderdetail_id,
         t3.items_id,
         t3.items_num,
         t3.orders_id,
         t4.itemsname items_name,
         t4.detail items_detail,
         t4.price items_price
    FROM
        orders t1,
        t_user t2,
        orderdetail t3,
        items t4
    WHERE t1.user_id =  t2.id AND  t3.orders_id=t1.id AND t3.items_id = t4.id
</select>
```

