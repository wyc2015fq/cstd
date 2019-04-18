# 【MyBatis学习08】高级映射之一对一查询 - z69183787的专栏 - CSDN博客
2017年07月28日 10:50:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：291
个人分类：[Mybatis-学习](https://blog.csdn.net/z69183787/article/category/7026142)
从这一篇博文开始，将总结一下mybatis中的几个高级映射，即一对一、一对多、多对多查询，这篇先总结一下mybatis中的一对一查询。 
　　为了模拟这些需求，事先要建立几个表，不同的表之间将对应上面提到的不同的映射，为此，我建立4个表，如下：
```
DROP TABLE IF EXISTS `items`;
DROP TABLE IF EXISTS `orders`;
DROP TABLE IF EXISTS `user`;
DROP TABLE IF EXISTS `orderdetail`;
/*items是商品表*/
CREATE TABLE `items` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(32) NOT NULL COMMENT '商品名称',
  `price` FLOAT(10,1) NOT NULL COMMENT '商品定价',
  `detail` TEXT COMMENT '商品描述',
  `pic` VARCHAR(64) DEFAULT NULL COMMENT '商品图片',
  `createtime` DATETIME NOT NULL COMMENT '生产日期',
  PRIMARY KEY (`id`)
) ENGINE=INNODB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*user是用户表*/
CREATE TABLE `user` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(32) NOT NULL COMMENT '用户名称',
  `birthday` DATE DEFAULT NULL COMMENT '生日',
  `sex` CHAR(1) DEFAULT NULL COMMENT '性别',
  `address` VARCHAR(256) DEFAULT NULL COMMENT '地址',
  PRIMARY KEY (`id`)
) ENGINE=INNODB AUTO_INCREMENT=27 DEFAULT CHARSET=utf8;
/*orders是订单表*/
CREATE TABLE `orders` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `user_id` INT(11) NOT NULL COMMENT '下单用户id',
  `number` VARCHAR(32) NOT NULL COMMENT '订单号',
  `createtime` DATETIME NOT NULL COMMENT '创建订单时间',
  `note` VARCHAR(100) DEFAULT NULL COMMENT '备注',
  PRIMARY KEY (`id`),
  KEY `FK_orders_1` (`user_id`),
  CONSTRAINT `FK_orders_id` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=INNODB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*orderdetail是订单明细表*/
DROP TABLE IF EXISTS orderdetail;
CREATE TABLE `orderdetail` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `orders_id` INT(11) NOT NULL COMMENT '订单id',
  `items_id` INT(11) NOT NULL COMMENT '商品id',
  `items_num` INT(11) DEFAULT NULL COMMENT '商品购买数量',
  PRIMARY KEY (`id`),
  KEY `FK_orderdetail_1` (`orders_id`),
  KEY `FK_orderdetail_2` (`items_id`),
  CONSTRAINT `FK_orderdetail_1` FOREIGN KEY (`orders_id`) REFERENCES `orders` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_orderdetail_2` FOREIGN KEY (`items_id`) REFERENCES `items` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=INNODB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
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
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
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
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
每个表对应的po就不写了。从上面的表中，可以分析出下面的一些关系： 
![表的关系](https://img-blog.csdn.net/20160612182703752)
这一节主要总结mybatis中一对一的映射，所以我选择左边orders和user之间的一对一关系来总结，主要是查询订单信息，关联查询创建订单的用户信息。 
我们知道，查询结果可以使用resultType，也可以使用resultMap，在这里我两种方法都使用一下，然后对比一下各个方法的特点。
## 1. resultType方法
### 1.1 sql查询语句
写查询语句的时候首先得确定查询的主表，这里是订单表，然后关联表是用户表。由于orders表中有一个外键（user_id），通过此外键关联查询用户表只能查询出一条记录，所以使用内连接。查询的sql如下：
```
SELECT 
  orders.*,
  USER.username,
  USER.sex,
  USER.address 
FROM
  orders,
  USER 
WHERE orders.user_id = user.id
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
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
有了sql，接下来就是创建pojo了
### 1.2 创建pojo
使用resultType的话，有个前提就是查询结果要想映射到pojo中去，pojo中必须包括所有查询出的列名才行。这里不仅查询出order表中的所有字段，而且还查询出了用户表中的部分字段，所以我们要自己新建一个pojo来包含所有的这些查询出来的字段才行。 
创建pojo的原则是继承包括查询字段较多的po类。
```java
//通过此类映射订单和用户查询的结果，让此类继承包括字段较多的po类
public class OrdersCustom extends Orders {
    //继承了Orders，已经有了Orders的所有属性了
    //下面添加用户属性
    private String username;
    private String sex;
    private String address;
    //省略get和set方法
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
- 9
- 10
- 11
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
接下来就可以写mapper.xml映射文件了。
### 1.3 UserMapperOrders.xml配置文件
```xml
<mapper namespace="mybatis.mapper.UserMapperOrders">
    <select id="findOrdersUser" resultType="mybatis.po.OrdersCustom">
        SELECT 
          orders.*,
          user.`username`,
          user.`sex`,
          user.`address` 
        FROM
          orders,
          USER 
        WHERE orders.`user_id` = user.`id` 
    </select>
</mapper>
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
### 1.4 UserMapperOrders.java接口
```java
public interface UserMapperOrders {
    //查询订单，关联查询用户信息
    public List<OrdersCustom> findOrdersUser() throws Exception;
}
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
注意接口和映射文件要放在同一个目录下，因为SqlMapConfig.xml文件中配置的是批量加载mapper，这里可以参考我的这篇博文：[传送门](http://blog.csdn.net/eson_15/article/details/51604171)。 
到这里就完成了使用resultType方法来实现一对一查询了，下面[测试](http://lib.csdn.net/base/softwaretest)一下：
```java
@Test
    public void testUserMapperOrders() throws Exception {
        //获取sqlSessionFactory的代码省略了
        SqlSession sqlSession = sqlSessionFactory.openSession();
        UserMapperOrders userMapperOrders = sqlSession.getMapper(UserMapperOrders.class);
        List<OrdersCustom> list = userMapperOrders.findOrdersUser();
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
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
从上面可以看出，使用resultType的流程很简单，还是和之前的一样，关键还是在sql语句上，这也是mybatis的强大之处，自己写sql语句，想怎么查就怎么查。下面再总结一下使用resultMap方法来实现一对一查询。
## 2. resultMap方法
还是上面那个sql，使用resultMap方法的思路如下：使用resultMap将查询结果中的订单信息映射到Orders对象中，在Orders类中添加User属性，将关联查询出来的用户信息映射到Orders对象中的User属性中。所以现在Orders类中添加一项User属性，我就不写具体的代码了。 
学过[hibernate](http://lib.csdn.net/base/javaee)就会感觉这里和Hibernate有点像，如果用Hibernate，这里也是在Orders类中添加一个User属性，将查询Orders关联出的User放到User属性中。
### 2.1 定义resultMap
```xml
<resultMap type="mybatis.po.Orders" id="OrdersUserResultMap">
    <!-- 配置映射订单信息 -->
    <id column="id" property="id"/>
    <result column="user_id" property="userId"/>
    <result column="number" property="number"/>
    <result column="createtime" property="createtime"/>
    <result column="note" property="note"/>
    <!-- 配置映射的关联的用户信息 -->
    <!-- association用于映射关联查询单个对象的信息
        property：要将关联查询的用户信息映射到Orders中的哪个属性
        javaType：该属性的类型
     -->
    <association property="user" javaType="mybatis.po.User">
        <id column="user_id" property="id"/>
        <result column="username" property="username"/>
        <result column="sex" property="sex"/>
        <result column="address" property="address"/>
    </association>
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
这里主要用到了`<association>`标签，值用来映射关联查询单个对象的信息，关于resultMap的一些标签属性我在前面的博客中已经解释过了，这里不再赘述。
### 2.2 UserMapperOrders.xml配置文件
```xml
<select id="findOrdersUserResultMap" resultMap="OrdersUserResultMap">
    SELECT 
      orders.*,
      user.`username`,
      user.`sex`,
      user.`address` 
    FROM
      orders,
      USER 
    WHERE orders.`user_id` = user.`id` 
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
配置和上面resultType一样的，唯一区别在于id和resultMap，注意一下就行了。
### 2.3 UserMapperOrders.java接口
```java
public interface UserMapperOrders {
    //查询订单，关联查询用户信息,使用resultMap
    public List<OrdersCustom> findOrdersUserResultMap() throws Exception;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6
也很简单，不再多说了。下面测试一下：
```java
@Test
    public void testUserMapperOrdersResultMap() throws Exception {
        SqlSession sqlSession = sqlSessionFactory.openSession();
        UserMapperOrders userMapperOrders = sqlSession.getMapper(UserMapperOrders.class);
        List<OrdersCustom> list = userMapperOrders.findOrdersUserResultMap();
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
到这里，resultMap方法完成的一对一查询映射就总结完了，从这里可以看出，resultMap方法和hibernate中其实是有点像的。下面再总结一下resultType和resultMap的区别：
## 3. resultType和resultMap
实现一对一查询：
> - resultType：使用resultType实现较为简单，如果pojo中没有包括查询出来的列名，需要增加列名对应的属性，即可完成映射。如果没有查询结果的特殊要求建议使用resultType。
- resultMap：需要单独定义resultMap，实现有点麻烦，如果对查询结果有特殊的要求，使用resultMap可以完成将关联查询映射pojo的属性中。
- resultMap可以实现延迟加载，resultType无法实现延迟加载。
